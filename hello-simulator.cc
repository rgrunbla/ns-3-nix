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
#include "ns3/waypoint-mobility-model.h"
#include "ns3/wifi-net-device.h"
#include <math.h>
#include <tuple>
#include <map>
#include <iostream>
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


// This is a simple example in order to show how to configure an IEEE 802.11ac Wi-Fi network.
//
// It outputs the UDP or TCP goodput for every VHT MCS value, which depends on the MCS value (0 to 9, where 9 is
// forbidden when the channel width is 20 MHz), the channel width (20, 40, 80 or 160 MHz) and the guard interval (long
// or short). The PHY bitrate is constant over all the simulation run. The user can also specify the distance between
// the access point and the station: the larger the distance the smaller the goodput.
//
// The simulation assumes a single station in an infrastructure network:
//
//  STA     AP
//    *     *
//    |     |
//   n1     n2
//
//Packets in this simulation belong to BestEffort Access Class (AC_BE).

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("vht-wifi-network");



/*
 * Antenna model
 */

double custom_antenna_model(double theta, double phi)
{
  std::cout << "theta = " << theta << ", phi = " << phi << std::endl;
  float antenna_gain[360]{
        -6.8f, -6.8f, -6.8f, -6.8f, -6.7f, -6.7f, -6.7f, -6.6f, -6.6f, -6.6f,
        -6.5f, -6.5f, -6.4f, -6.4f, -6.3f, -6.3f, -6.3f, -6.2f, -6.2f, -6.1f,
        -6.1f, -6.0f, -6.0f, -5.9f, -5.9f, -5.8f, -5.7f, -5.7f, -5.6f, -5.5f,
        -5.4f, -5.3f, -5.2f, -5.1f, -5.0f, -4.9f, -4.7f, -4.6f, -4.5f, -4.4f,
        -4.3f, -4.2f, -4.1f, -4.0f, -3.9f, -3.8f, -3.7f, -3.6f, -3.5f, -3.4f,
        -3.3f, -3.1f, -3.0f, -2.8f, -2.7f, -2.5f, -2.3f, -2.2f, -2.0f, -1.8f,
        -1.6f, -1.5f, -1.3f, -1.1f, -1.0f, -0.8f, -0.7f, -0.6f, -0.5f, -0.4f,
        -0.3f, -0.2f, -0.2f, -0.1f, -0.1f, -0.1f, -0.0f, -0.0f, -0.0f, -0.0f,
        -0.0f, -0.0f, -0.0f, -0.0f, -0.0f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f,
        -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.0f, -0.0f,
        -0.0f, -0.0f, -0.0f, -0.0f, -0.0f, -0.1f, -0.1f, -0.1f, -0.2f, -0.2f,
        -0.3f, -0.4f, -0.5f, -0.6f, -0.7f, -0.8f, -0.9f, -1.0f, -1.2f, -1.3f,
        -1.5f, -1.7f, -1.8f, -2.0f, -2.2f, -2.3f, -2.5f, -2.7f, -2.9f, -3.0f,
        -3.2f, -3.3f, -3.5f, -3.6f, -3.8f, -3.9f, -4.1f, -4.2f, -4.3f, -4.4f,
        -4.5f, -4.6f, -4.7f, -4.7f, -4.8f, -4.9f, -4.9f, -5.0f, -5.1f, -5.2f,
        -5.2f, -5.3f, -5.4f, -5.5f, -5.6f, -5.6f, -5.7f, -5.8f, -5.9f, -6.0f,
        -6.1f, -6.2f, -6.3f, -6.4f, -6.4f, -6.5f, -6.6f, -6.7f, -6.7f, -6.8f,
        -6.8f, -6.9f, -6.9f, -7.0f, -7.0f, -7.0f, -7.0f, -7.0f, -7.0f, -7.0f,
        -7.0f, -7.0f, -6.9f, -6.9f, -6.9f, -6.8f, -6.8f, -6.7f, -6.7f, -6.6f,
        -6.5f, -6.5f, -6.4f, -6.3f, -6.2f, -6.2f, -6.1f, -6.0f, -6.0f, -5.9f,
        -5.8f, -5.8f, -5.7f, -5.6f, -5.6f, -5.5f, -5.5f, -5.4f, -5.3f, -5.3f,
        -5.2f, -5.1f, -5.1f, -5.0f, -4.9f, -4.9f, -4.8f, -4.7f, -4.6f, -4.6f,
        -4.5f, -4.4f, -4.3f, -4.2f, -4.1f, -4.0f, -3.8f, -3.7f, -3.6f, -3.4f,
        -3.3f, -3.1f, -3.0f, -2.8f, -2.6f, -2.4f, -2.3f, -2.1f, -1.9f, -1.7f,
        -1.6f, -1.4f, -1.2f, -1.1f, -0.9f, -0.8f, -0.7f, -0.5f, -0.4f, -0.3f,
        -0.2f, -0.2f, -0.1f, -0.1f, -0.0f, -0.0f, -0.0f, 0.0f,  -0.0f, -0.0f,
        -0.0f, -0.0f, -0.0f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f,
        -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f,
        -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.2f, -0.2f, -0.3f,
        -0.3f, -0.4f, -0.5f, -0.6f, -0.7f, -0.8f, -0.9f, -1.1f, -1.2f, -1.4f,
        -1.6f, -1.7f, -1.9f, -2.1f, -2.3f, -2.4f, -2.6f, -2.8f, -2.9f, -3.1f,
        -3.2f, -3.4f, -3.5f, -3.6f, -3.8f, -3.9f, -4.0f, -4.2f, -4.3f, -4.4f,
        -4.5f, -4.7f, -4.8f, -4.9f, -5.0f, -5.1f, -5.2f, -5.3f, -5.3f, -5.4f,
        -5.5f, -5.5f, -5.6f, -5.7f, -5.7f, -5.8f, -5.9f, -5.9f, -6.0f, -6.1f,
        -6.1f, -6.2f, -6.2f, -6.3f, -6.4f, -6.4f, -6.5f, -6.6f, -6.6f, -6.7f,
        -6.7f, -6.7f, -6.8f, -6.8f, -6.8f, -6.8f, -6.8f, -6.8f, -6.8f, -6.8};
    int index = ((phi + M_PI) * 180.0) / M_PI;
    if (index == 360) index--;
    return antenna_gain[index];
}

/* Map between the MAC addresses and the NS-3 node simulation ID */
std::map<std::string, int32_t> nodes;

/* Mab between the nodes simulation ID and the last node simulation ID they
 * transmitted to */
std::map<int32_t, int32_t> last_transmitted;

zmq::context_t ctx;
zmq::socket_t sock_ns3(ctx, ZMQ_REQ);

int32_t simulation_id = -1;

std::string content;
GlobalContainer global_container = GlobalContainer();
Meta meta = Meta();
Meso meso = Meso();
MonitorQuery monitor_query = MonitorQuery();

Throughputs throughputs = Throughputs();

std::string ConvertAddrToString(Address addr) {
  std::ostringstream stream;
  stream << addr;
  std::string str = stream.str();
  return str;
}

std::string ConvertIpv4AddrToString(Ipv4Address addr) {
  std::ostringstream stream;
  stream << addr;
  std::string str = stream.str();
  return str;
}

typedef std::tuple<std::string, std::string, uint32_t, uint64_t, uint32_t,
                   uint64_t>
    flowdata_t;

std::vector<std::pair<double, std::map<int, flowdata_t>>> flows;

void ThroughputMonitor(FlowMonitorHelper *fmHelper, Ptr<FlowMonitor> flowMon) {
  float duration = 1.0;
  flowMon->CheckForLostPackets();
  Ptr<Ipv4FlowClassifier> classifier =
      DynamicCast<Ipv4FlowClassifier>(fmHelper->GetClassifier());
  FlowMonitor::FlowStatsContainer stats = flowMon->GetFlowStats();

  std::map<int, flowdata_t> instantflows;

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i =
           stats.begin();
       i != stats.end(); ++i) {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
    instantflows[i->first] = std::make_tuple(
        ConvertIpv4AddrToString(t.sourceAddress),
        ConvertIpv4AddrToString(t.destinationAddress), i->second.txPackets,
        i->second.txBytes, i->second.rxPackets, i->second.rxBytes);
    if (flows.begin() != flows.end()) {
      if (flows.back().second.find(i->first) != flows.back().second.end()) {
        std::cout << "continuous"
                  << "," << Simulator::Now().GetSeconds() << "," << i->first
                  << "," << std::get<0>(flows.back().second[i->first]) << ","
                  << std::get<1>(flows.back().second[i->first]) << ","
                  << std::get<2>(instantflows[i->first]) -
                         std::get<2>(flows.back().second[i->first])
                  << ","
                  << ((float)(std::get<3>(instantflows[i->first]) -
                              std::get<3>(flows.back().second[i->first])))
                  << ","
                  << std::get<4>(instantflows[i->first]) -
                         std::get<4>(flows.back().second[i->first])
                  << ","
                  << ((float)(std::get<5>(instantflows[i->first]) -
                              std::get<5>(flows.back().second[i->first])))
                  << "\n";
      }
    }
  }
  flows.push_back(std::make_pair(Simulator::Now().GetSeconds(), instantflows));
  Simulator::Schedule(Seconds(duration), &ThroughputMonitor, fmHelper, flowMon);
}

void ThroughputMonitorFirst(FlowMonitorHelper *fmHelper,
                            Ptr<FlowMonitor> flowMon) {
  std::cout << "continuous,Time,Flow,Source Address,Destination Address,Tx "
               "Packets,Tx Bytes,Rx Packets,Rx Bytes\n";
  ThroughputMonitor(fmHelper, flowMon);
}

void createNodeMap() {
  for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i) {
    Ptr<Node> node = NodeList::GetNode(i);
    for (uint32_t j = 0; j < node->GetNDevices(); ++j) {
      if (ConvertAddrToString(node->GetDevice(j)->GetAddress()) !=
          "03-06-00:00:00:00:00:00") {
        nodes.insert(std::pair<std::string, uint32_t>(
            ConvertAddrToString(node->GetDevice(j)->GetAddress()),
            node->GetId()));
        // std::cout << ConvertAddrToString(node->GetDevice(j)->GetAddress()) <<
        // " " << node->GetId() << "\n";
      }
    }
  }
}

std::tuple<int32_t, int32_t, int32_t, int32_t, int32_t> getIds(Ptr<Packet> p) {
  Mac48Address Addr1 = Mac48Address();
  Mac48Address Addr2 = Mac48Address();
  Mac48Address Addr3 = Mac48Address();
  Mac48Address Addr4 = Mac48Address();
  int n1 = -1, n2 = -1, n3 = -1, n4 = -1;

  int32_t data_type = 99;
  PacketMetadata::ItemIterator i = p->BeginItem();
  while (i.HasNext()) {
    PacketMetadata::Item item = i.Next();
    if (!item.isFragment) {
      switch (item.type) {
      case PacketMetadata::Item::PAYLOAD:
        break;
      case PacketMetadata::Item::HEADER:
      case PacketMetadata::Item::TRAILER:
        if (item.tid.GetName() == "ns3::WifiMacHeader") {
          Callback<ObjectBase *> constructor = item.tid.GetConstructor();
          NS_ASSERT(!constructor.IsNull());
          // Ptr<> and DynamicCast<> won't work here as all headers are from
          // ObjectBase, not Object
          ObjectBase *instance = constructor();
          NS_ASSERT(instance != 0);
          WifiMacHeader *wifiMacHeader =
              dynamic_cast<WifiMacHeader *>(instance);
          NS_ASSERT(wifiMacHeader != 0);
          wifiMacHeader->Deserialize(item.current);
          if (wifiMacHeader->IsData() && wifiMacHeader->HasData()) {
            data_type = 0;
          } else if (wifiMacHeader->IsAck() or wifiMacHeader->IsBlockAck()) {
            data_type = 1;
          } else {
            data_type = 99;
          }
          Addr1 = wifiMacHeader->GetAddr1();
          Addr2 = wifiMacHeader->GetAddr2();
          Addr3 = wifiMacHeader->GetAddr3();
          Addr4 = wifiMacHeader->GetAddr4();
        }
        break;
      }
    }
  }
  // std::cout << "[" << Simulator::Now().GetSeconds() << "] " << Addr1 << " "
  // << Addr2 << " " << Addr3 << " " << Addr4 << "\n"; std::cout << "[" <<
  // Simulator::Now().GetSeconds() << "] " << ConvertAddrToString(Addr1) << " "
  // << ConvertAddrToString(Addr2) << " " << ConvertAddrToString(Addr3) << " "
  // << ConvertAddrToString(Addr4) << "\n";
  if (nodes.count(ConvertAddrToString(Addr1)) != 0) {
    n1 = nodes[ConvertAddrToString(Addr1)];
  }
  if (nodes.count(ConvertAddrToString(Addr2)) != 0) {
    n2 = nodes[ConvertAddrToString(Addr2)];
  }
  if (nodes.count(ConvertAddrToString(Addr3)) != 0) {
    n3 = nodes[ConvertAddrToString(Addr3)];
  }
  if (nodes.count(ConvertAddrToString(Addr4)) != 0) {
    n4 = nodes[ConvertAddrToString(Addr4)];
  }
  return std::tuple<int32_t, int32_t, int32_t, int32_t, int32_t>(data_type, n1,
                                                                 n2, n3, n4);
}

void DevMonitorSnifferRx(uint32_t node, Ptr<const Packet> packet,
                         uint16_t channelFreqMhz, WifiTxVector txVector,
                         struct MpduInfo aMpdu,
                         struct SignalNoiseDbm signalNoise) {
  if ((aMpdu.type == NORMAL_MPDU) || (aMpdu.type == SINGLE_MPDU) ||
      (aMpdu.type == FIRST_MPDU_IN_AGGREGATE)) {
    // packet->Print(std::cout);
    // std::cout << "\n";
    Ptr<Packet> copy = packet->Copy();
    std::tuple<int32_t, int32_t, int32_t, int32_t, int32_t> ids = getIds(copy);

    // std::cout << "My id: " << node << "\n";
    // std::cout << "Results: " << std::get<1>(ids) << ", "  << std::get<2>(ids)
    // << ", "  << std::get<3>(ids) << ", "  << std::get<4>(ids) << "\n";

    monitor_query.Clear();

    if (std::get<0>(ids) == 0) { /* Data Frame */
      if ((int)node != std::get<1>(ids)) {
        // std::cout << "[" << Simulator::Now().GetSeconds() << "] " << "On node
        // " << node << " received data from node " << std::get<2>(ids) << "
        // (not for me)\n";
        return;
      } else {
        // std::cout << "[" << Simulator::Now().GetSeconds() << "] " << "On node
        // " << node << " received data from node " << std::get<2>(ids) << "
        // (for me)\n";
        monitor_query.set_source_agent_id(std::get<2>(ids));
      }
    } else if (std::get<0>(ids) == 1) {    /* Ack Frame */
      if ((int)node == std::get<1>(ids)) { /* Ack Frame for Me */
        if (last_transmitted.find(node) != last_transmitted.end()) {
          monitor_query.set_source_agent_id(last_transmitted[node]);
          // std::cout << "On node " << node << " received an ack from node " <<
          // last_transmitted[node] << "\n";
        } else {
          return;
        }
      } else { /* Ack Frame not for Me */
        // std::cout << "On node " << node << " received an ack not for me\n";
        return;
      }
    } else {
      // std::cout << "Non ack / data packet, ignoring.\n";
      return;
    }

    monitor_query.set_clock(Simulator::Now().GetSeconds());
    monitor_query.set_dest_agent_id(node);
    if (monitor_query.source_agent_id() == monitor_query.dest_agent_id()) {
      std::cout << "fuckery\n";
    }
    monitor_query.set_power(signalNoise.signal);
    monitor_query.SerializeToString(&content);

    MesoSend(simulation_id, content, Meso_MessageType_MONITOR_QUERY, sock_ns3);
    AckRecv(sock_ns3);
  }
}

void DevMonitorSnifferTx(uint32_t node, Ptr<const Packet> packet,
                         uint16_t channelFreqMhz, WifiTxVector txVector,
                         struct MpduInfo aMpdu) {
  if ((aMpdu.type == NORMAL_MPDU) || (aMpdu.type == SINGLE_MPDU) ||
      (aMpdu.type == FIRST_MPDU_IN_AGGREGATE)) {
    Ptr<Packet> copy = packet->Copy();
    std::tuple<int32_t, int32_t, int32_t, int32_t, int32_t> ids = getIds(copy);

    if (std::get<0>(ids) == 0) { /* Data Frame */
      if (std::get<1>(ids) != -1) {
        last_transmitted[node] = std::get<1>(ids);
        // std::cout << "[" << Simulator::Now().GetSeconds() << "] " << "On node
        // " << node << " I transmit data to " << std::get<1>(ids) << "\n";
      }
    } else if (std::get<0>(ids) == 1) { /* Ack */
      // std::cout << "[" << Simulator::Now().GetSeconds() << "] " <<  "On node
      // " << node << " sending an ack.\n";
    } else { /* Something else I send */
      // std::cout << "[" << Simulator::Now().GetSeconds() << "] " <<  "Sending
      // other type of traffic\n";
    }
  }
}

int main(int argc, char *argv[])
{
  bool udp = true;
  bool useRts = false;
  double simulationTime = 10; //seconds
  double distance = 1.0;      //meters
  int mcs = -1;               // -1 indicates an unset value
  double minExpectedThroughput = 0;
  double maxExpectedThroughput = 0;

  CommandLine cmd(__FILE__);
  cmd.AddValue("distance", "Distance in meters between the station and the access point", distance);
  cmd.AddValue("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.AddValue("udp", "UDP if set to 1, TCP otherwise", udp);
  cmd.AddValue("useRts", "Enable/disable RTS/CTS", useRts);
  cmd.AddValue("mcs", "if set, limit testing to a specific MCS (0-9)", mcs);
  cmd.AddValue("minExpectedThroughput", "if set, simulation fails if the lowest throughput is below this value", minExpectedThroughput);
  cmd.AddValue("maxExpectedThroughput", "if set, simulation fails if the highest throughput is above this value", maxExpectedThroughput);
  cmd.Parse(argc, argv);

  if (useRts)
  {
    Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue("0"));
  }

  double prevThroughput[8];
  for (uint32_t l = 0; l < 8; l++)
  {
    prevThroughput[l] = 0;
  }
  std::cout << "MCS value"
            << "\t\t"
            << "Channel width"
            << "\t\t"
            << "short GI"
            << "\t\t"
            << "Throughput" << '\n';
  int minMcs = 0;
  int maxMcs = 9;
  if (mcs >= 0 && mcs <= 9)
  {
    minMcs = mcs;
    maxMcs = mcs;
  }
  for (int mcs = minMcs; mcs <= maxMcs; mcs++)
  {
    uint8_t index = 0;
    double previous = 0;
    for (int channelWidth = 20; channelWidth <= 160;)
    {
      if (mcs == 9 && channelWidth == 20)
      {
        channelWidth *= 2;
        continue;
      }
      for (int sgi = 0; sgi < 2; sgi++)
      {
        uint32_t payloadSize; //1500 byte IP packet
        if (udp)
        {
          payloadSize = 1472; //bytes
        }
        else
        {
          payloadSize = 1448; //bytes
          Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(payloadSize));
        }

        NodeContainer wifiStaNode;
        wifiStaNode.Create(1);
        NodeContainer wifiApNode;
        wifiApNode.Create(1);

        YansWifiChannelHelper channel;
        channel.AddPropagationLoss("ns3::FriisPropagationLossModel");

        channel.AddPropagationLoss("ns3::AntennaPropagationLossModel");
        Ptr<CustomAntennaModel> apAntenna = CreateObject<CustomAntennaModel>();
        apAntenna->SetModel(custom_antenna_model);
        apAntenna->Install(wifiStaNode.Get(0));

        Ptr<CustomAntennaModel> staAntenna = CreateObject<CustomAntennaModel>();
        staAntenna->SetModel(custom_antenna_model);
        staAntenna->Install(wifiApNode.Get(0));

        channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
        YansWifiPhyHelper phy;
        phy.SetChannel(channel.Create());

        WifiHelper wifi;
        wifi.SetStandard(WIFI_STANDARD_80211ac);
        WifiMacHelper mac;

        std::ostringstream oss;
        oss << "VhtMcs" << mcs;
        wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue(oss.str()),
                                     "ControlMode", StringValue(oss.str()));

        Ssid ssid = Ssid("ns3-80211ac");

        mac.SetType("ns3::StaWifiMac",
                    "Ssid", SsidValue(ssid));

        NetDeviceContainer staDevice;
        staDevice = wifi.Install(phy, mac, wifiStaNode);

        mac.SetType("ns3::ApWifiMac",
                    "EnableBeaconJitter", BooleanValue(false),
                    "Ssid", SsidValue(ssid));

        NetDeviceContainer apDevice;
        apDevice = wifi.Install(phy, mac, wifiApNode);

        // Set channel width
        Config::Set("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue(channelWidth));

        // Set guard interval
        Config::Set("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/HtConfiguration/ShortGuardIntervalSupported", BooleanValue(sgi));

        // mobility.
        MobilityHelper mobility;
        mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");

        mobility.Install(wifiApNode);
        mobility.Install(wifiStaNode);

        for(NodeContainer::Iterator n = wifiApNode.Begin (); n != wifiApNode.End (); n++) {
          Ptr<Node> node = *n;
          node->GetObject<ConstantVelocityMobilityModel> ()->SetPosition(Vector(0.0, 0.0, 0.0));
          node->GetObject<ConstantVelocityMobilityModel> ()->SetAngularVelocity(Vector(0.0, 0.0, 0.08));
        }

        for(NodeContainer::Iterator n = wifiStaNode.Begin (); n != wifiStaNode.End (); n++) {
          Ptr<Node> node = *n;
          node->GetObject<ConstantVelocityMobilityModel> ()->SetPosition(Vector(distance, 0.0, 0.0));
          node->GetObject<ConstantVelocityMobilityModel> ()->SetAngularVelocity(Vector(0.0, 0.0, 0.0));
        }
        /* Internet stack*/
        InternetStackHelper stack;
        stack.Install(wifiApNode);
        stack.Install(wifiStaNode);

        Ipv4AddressHelper address;
        address.SetBase("192.168.1.0", "255.255.255.0");
        Ipv4InterfaceContainer staNodeInterface;
        Ipv4InterfaceContainer apNodeInterface;

        staNodeInterface = address.Assign(staDevice);
        apNodeInterface = address.Assign(apDevice);

        /* Setting applications */
        ApplicationContainer serverApp;
        if (udp)
        {
          //UDP flow
          uint16_t port = 9;
          UdpServerHelper server(port);
          serverApp = server.Install(wifiStaNode.Get(0));
          serverApp.Start(Seconds(0.0));
          serverApp.Stop(Seconds(simulationTime + 1));

          UdpClientHelper client(staNodeInterface.GetAddress(0), port);
          client.SetAttribute("MaxPackets", UintegerValue(4294967295u));
          client.SetAttribute("Interval", TimeValue(Time("0.00002"))); //packets/s
          client.SetAttribute("PacketSize", UintegerValue(payloadSize));
          ApplicationContainer clientApp = client.Install(wifiApNode.Get(0));
          clientApp.Start(Seconds(1.0));
          clientApp.Stop(Seconds(simulationTime + 1));
        }
        else
        {
          //TCP flow
          uint16_t port = 50000;
          Address localAddress(InetSocketAddress(Ipv4Address::GetAny(), port));
          PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", localAddress);
          serverApp = packetSinkHelper.Install(wifiStaNode.Get(0));
          serverApp.Start(Seconds(0.0));
          serverApp.Stop(Seconds(simulationTime + 1));

          OnOffHelper onoff("ns3::TcpSocketFactory", Ipv4Address::GetAny());
          onoff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
          onoff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
          onoff.SetAttribute("PacketSize", UintegerValue(payloadSize));
          onoff.SetAttribute("DataRate", DataRateValue(1000000000)); //bit/s
          AddressValue remoteAddress(InetSocketAddress(staNodeInterface.GetAddress(0), port));
          onoff.SetAttribute("Remote", remoteAddress);
          ApplicationContainer clientApp = onoff.Install(wifiApNode.Get(0));
          clientApp.Start(Seconds(1.0));
          clientApp.Stop(Seconds(simulationTime + 1));
        }

        Ipv4GlobalRoutingHelper::PopulateRoutingTables();

        Simulator::Stop(Seconds(simulationTime + 1));
        Simulator::Run();

        uint64_t rxBytes = 0;
        if (udp)
        {
          rxBytes = payloadSize * DynamicCast<UdpServer>(serverApp.Get(0))->GetReceived();
        }
        else
        {
          rxBytes = DynamicCast<PacketSink>(serverApp.Get(0))->GetTotalRx();
        }
        double throughput = (rxBytes * 8) / (simulationTime * 1000000.0); //Mbit/s

        Simulator::Destroy();

        std::cout << mcs << "\t\t\t" << channelWidth << " MHz\t\t\t" << sgi << "\t\t\t" << throughput << " Mbit/s" << std::endl;

        //test first element
        if (mcs == 0 && channelWidth == 20 && sgi == 0)
        {
          if (throughput < minExpectedThroughput)
          {
            NS_LOG_ERROR("Obtained throughput " << throughput << " is not expected!");
            exit(1);
          }
        }
        //test last element cout
        if (mcs == 9 && channelWidth == 160 && sgi == 1)
        {
          if (maxExpectedThroughput > 0 && throughput > maxExpectedThroughput)
          {
            NS_LOG_ERROR("Obtained throughput " << throughput << " is not expected!");
            exit(1);
          }
        }
        //test previous throughput is smaller (for the same mcs)
        if (throughput > previous)
        {
          previous = throughput;
        }
        else
        {
          NS_LOG_ERROR("Obtained throughput " << throughput << " is not expected!");
          exit(1);
        }
        //test previous throughput is smaller (for the same channel width and GI)
        if (throughput > prevThroughput[index])
        {
          prevThroughput[index] = throughput;
        }
        else
        {
          NS_LOG_ERROR("Obtained throughput " << throughput << " is not expected!");
          exit(1);
        }
        index++;
      }
      channelWidth *= 2;
    }
  }
  return 0;
}
