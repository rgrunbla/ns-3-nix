/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 SEBASTIEN DERONNE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Sebastien Deronne <sebastien.deronne@gmail.com>
 */
#include <math.h>
#include <tuple>
#include <map>
#include <iostream>
#include <deque>
#include <queue>
#include "common.hpp"
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/wifi-phy.h"
#include "ns3/ssid.h"
#include "ns3/mobility-helper.h"
#include "ns3/mobility-model.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/trace-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/on-off-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/pointer.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/box.h"
#include "ns3/wifi-mac-header.h"
#include "ns3/waypoint.h"
#include "ns3/wifi-net-device.h"
#include "ns3/node-list.h"
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/custom-antenna-model.h"
#include "ns3/ssid.h"
#include "ns3/mobility-helper.h"
#include "ns3/constant-velocity-helper.h"
#include "ns3/constant-velocity-mobility-model.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/pointer.h"
#include "ns3/propagation-loss-model.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("vht-wifi-network");

int32_t simulation_id = -1;
int main(int argc, char *argv[])
{

  /* General configuration */
  double simulationTime = 10; // seconds
  int N = 2;
  std::string agent_type = "Drone";
  int scenarioType = SIMPLE;

  /* Mobility configuration */
  double distance = 1000.0; // meters

  /* Wifi configuration */
  bool enablePcap = false;
  int mcs = -1;
  std::string wifiManager = "ns3::IdealWifiManager";
  int channelWidth = 20;
  bool sgi = true;

  /* Application configuration */
  bool udp = true;
  std::string dataRate = "500Mbps";
  uint32_t packetSize = 1024; // bytes

  CommandLine cmd;
  cmd.AddValue("scenarioType", "Scenario type", scenarioType);
  cmd.AddValue("enablePcap", "Enable pcap", enablePcap);
  cmd.AddValue("distance",
               "Distance in meters between the station and the access point",
               distance);
  cmd.AddValue("agentType", "Type of the agents (Client,Drone)", agent_type);
  cmd.AddValue("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.AddValue("udp", "UDP if set to 1, TCP otherwise", udp);
  cmd.AddValue("wifiManager", "Use the specified wifiManager", wifiManager);
  cmd.AddValue("mcs", "Test a specific MCS (0-9)", mcs);
  cmd.AddValue("channelWidth", "Specifies the channel width (20, 40, 80, 160",
               channelWidth);
  cmd.AddValue("sgi", "Specifies whether the short guard interval is used",
               sgi);
  cmd.AddValue("N", "Number of nodes in the simulation", N);
  cmd.AddValue("dataRate", "Datarate", dataRate);
  cmd.AddValue("packetSize", "Packet Size to use for the On/Off applications",
               packetSize);
  cmd.Parse(argc, argv);

  Simulation simulation = Simulation();

  /* Node creation */
  std::vector<std::string> agent_types;

  if (scenarioType == SIMPLE || scenarioType == CHAIN)
  {
    for (int i = 0; i < N; ++i)
    {
      agent_types.push_back(agent_type);
    }
  }
  else if (scenarioType == SINK)
  {
    agent_types.push_back("Client");
    for (int i = 1; i < N; ++i)
    {
      agent_types.push_back(agent_type);
    }
  }

  simulation.init(agent_types, N, simulationTime, static_cast<ScenarioType>(scenarioType));

  /* Mobility Configuration */
  Ptr<UniformRandomVariable> randomVariable =
      CreateObject<UniformRandomVariable>();

  Ptr<ListPositionAllocator> positionAlloc =
      CreateObject<ListPositionAllocator>();

  if (scenarioType == SIMPLE || scenarioType == CHAIN)
  {
    for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i)
    {
      NS_LOG_DEBUG("Placing node " << i << " at distance "
                                   << i * (distance / (N - 1)) << "\n");
      positionAlloc->Add(Vector(0.0 + (i * distance) / (N - 1), 0.0, 0.0));
    }
  }
  else if (scenarioType == SINK)
  {
    double radius = distance;
    NS_LOG_DEBUG("Placing sink 0 at position (0.0, 0.0, 0.0)\n");
    positionAlloc->Add(Vector(0.0, 0.0, 0.0));

    for (uint32_t i = 1; i < NodeList::GetNNodes(); ++i)
    {
      NS_LOG_DEBUG("Placing node " << i << " at distance " << radius << " from node 0 randomly\n");
      double random_angle = randomVariable->GetValue() * 2 * M_PI / (randomVariable->GetMax() - randomVariable->GetMin());
      double x = std::cos(random_angle) * radius;
      double y = std::sin(random_angle) * radius;
      positionAlloc->Add(Vector(x, y, 0.0));
    }
  }

  simulation.configureMobility(positionAlloc, randomVariable);

  /* Wifi Configuration */
  simulation.configureWifi(enablePcap, channelWidth, mcs, sgi, wifiManager);

  /* Internet stack*/
  simulation.configureInternet();

  /* Routing */
  simulation.configureRouting();

  /* Applications */
  simulation.configureApplications(dataRate, udp, packetSize);

  /* Mab between mac addresses and node IDs */
  simulation.createNodeMap();

  FlowMonitorHelper fmHelper;
  Ptr<FlowMonitor> contFlowMon = fmHelper.InstallAll();
  Ptr<FlowMonitor> flowMon = fmHelper.InstallAll();
  flowsdata_t flows;
  Simulator::Schedule(Seconds(0.0), &Simulation::ThroughputMonitorFirst,
                      &simulation, flows, &fmHelper, contFlowMon);

  simulation.configureTraces();

  Packet::EnableChecking();

  Simulator::Stop(Seconds(simulationTime + 2));

  Simulator::Run();

  flowMon->CheckForLostPackets();
  Ptr<Ipv4FlowClassifier> classifier =
      DynamicCast<Ipv4FlowClassifier>(fmHelper.GetClassifier());
  FlowMonitor::FlowStatsContainer stats = flowMon->GetFlowStats();
  std::cout << "summary,Flow,Source Address,Destination Address,Tx Packets,Tx "
               "Bytes,Rx Packets,Rx Bytes\n";
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i =
           stats.begin();
       i != stats.end(); ++i)
  {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
    if (t.destinationPort == 1337)
      continue;
    std::cout << "summary"
              << "," << i->first << "," << t.sourceAddress << ","
              << t.destinationAddress << "," << i->second.txPackets << ","
              << i->second.txBytes << "," << i->second.rxPackets << ","
              << i->second.rxBytes << "\n";
  }

  Simulator::Destroy();

  /* Communication with Phi - End of Simulation */
  simulation.end();
  return 0;
}
