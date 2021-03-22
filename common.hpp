#include "ns3/attribute.h"
#include "ns3/arp-cache.h"
#include "ns3/boolean.h"
#include "ns3/callback.h"
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/constant-velocity-helper.h"
#include "ns3/constant-velocity-mobility-model.h"
#include "ns3/custom-antenna-model.h"
#include "ns3/double.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/log.h"
#include "ns3/mobility-helper.h"
#include "ns3/mobility-model.h"
#include "ns3/node-list.h"
#include "ns3/node.h"
#include "ns3/object-vector.h"
#include "ns3/on-off-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/packet.h"
#include "ns3/pointer.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/quaternion.h"
#include "ns3/random-variable-stream.h"
#include "ns3/simulator.h"
#include "ns3/ssid.h"
#include "ns3/string.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/uinteger.h"
#include "ns3/wifi-mac-header.h"
#include "ns3/wifi-net-device.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-helper.h"
#include <bits/stdint-uintn.h>
#include <deque>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace ns3;

template <typename T, int MaxLen, typename Container = std::deque<T>>
class FixedQueue : public std::queue<T, Container> {
public:
  void push(const T &value) {
    if (this->size() == MaxLen) {
      this->c.pop_front();
    }
    std::queue<T, Container>::push(value);
  }
};

std::string ConvertAddrToString(Address);
std::string ConvertIpv4AddrToString(Ipv4Address);

enum ScenarioType { SIMPLE, SINK, CHAIN };

struct PacketInfo;

struct PacketInfo {
  int32_t data_type;
  int32_t addr1;
  int32_t addr2;
  int32_t addr3;
  int32_t addr4;
};

typedef std::tuple<std::string, std::string, uint32_t, uint64_t, uint32_t,
                   int64_t>
    flowdata_t;
typedef std::vector<std::pair<double, std::map<int, flowdata_t>>> flowsdata_t;

class Drone {
public:
  Drone();

  /* Start the drone controller and sensors */
  void start();

  /* Main execution loop for the drone controller. */
  void controller(double duration);

  /* Returns the vavue of the magnetometer */
  double magnetometer();

  /* Antenna model of the drone */
  double custom_antenna_model(double theta, double phi);

  /* Id of the drone, which corresponds to the id of the ns-3 node it hosts */
  int32_t id = -1;

  /* Type of the Drone */
  std::string agent_type;

  /* Size of the search window */
  int window = 360;

  /* Current position to attain */ 
  double goal = -1;

  /* Center of the search space */ 
  double center = nan("");

  /* Whether we are in an exploration phase or not */
  bool exploration = true;

  double last_change = 0.0;
  double speed = 0.50;
  int angle = 0;
  int timeout = 0;
  double delay = 0.0;
  
  /* Id of the drone I last transmitted to.
  Used to detect acknowledgements for me. */
  int32_t last_transmitted = -1;

  /* Received transmission powers from neighbours, message queue */
  std::map<int32_t, FixedQueue<double, 64>> power_histories;

  /* Received transmission powers from neighbours as a function of my
   * orientation */
  std::map<int32_t, std::vector<double>> power_curves;
  std::vector<double> power_curve = std::vector<double>(360, nan(""));
};

class Simulation {

public:
  Simulation();

  void createNodeMap();

  PacketInfo getPacketInfo(Ptr<Packet>);

  void ThroughputMonitor(flowsdata_t &, FlowMonitorHelper *, Ptr<FlowMonitor>);
  void ThroughputMonitorFirst(flowsdata_t &, FlowMonitorHelper *,
                              Ptr<FlowMonitor>);

  void DevMonitorSnifferRx(std::string, Ptr<const Packet>, uint16_t,
                           WifiTxVector, struct MpduInfo, struct SignalNoiseDbm,
                           uint16_t staId);

  void DevMonitorSnifferTx(std::string, Ptr<const Packet>, uint16_t,
                           WifiTxVector, struct MpduInfo, uint16_t staId);

  void init(std::vector<std::string>, int, double, ScenarioType, bool, Ptr<UniformRandomVariable>);
  void configureTraces();
  void configureMobility(Ptr<ListPositionAllocator>,
                         Ptr<UniformRandomVariable>);
  void configureWifi(bool, int, int, bool, std::string, std::string, double, bool);
  void configureInternet();
  void configureRouting();
  void configureApplications(std::string, bool, uint32_t);
  void end();

  void set_id(int32_t simulation_id);
  const int32_t get_id();

private:
  int32_t simulation_id;

  /* Map between the MAC addresses and the NS-3 node simulation ID */
  std::map<std::string, int32_t> nodes;

  /* Map between the nodes simulation ID and their object */
  std::map<int32_t, Drone> drones;

  void MacTx(std::string, Ptr<const Packet>);
  void MacTxDataFailed(std::string, ns3::Mac48Address);
  std::map<int32_t, int64_t> macTx;
  std::map<int32_t, int64_t> macTxDataFailed;

  NodeContainer wifiNodes;
  NetDeviceContainer devices;
  Ipv4InterfaceContainer interfaces;

  ApplicationContainer serverApp;

  /* Configuration */

  /* General parameters */
  double simulationTime;
  ScenarioType scenarioType;

  /* Wifi */
  bool enablePcap;
  int channelWidth;
  int mcs;
  bool sgi;
  std::string wifiManager;

  /* Applications */
  bool udp;
  uint32_t packetSize;
  std::string dataRate;
};