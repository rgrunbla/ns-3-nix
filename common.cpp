#include "common.hpp"
#include <bits/stdint-uintn.h>
#include <regex>
#include <assert.h>

inline static double constrainAngleRad(double x)
{
  x = fmod(x, (2 * M_PI));
  if (x < 0)
    x += (2 * M_PI);
  return x;
}

inline static double constrainAngleDeg(double x)
{
  x = fmod(x, 360);
  if (x < 0)
    x += 360;
  return x;
}

inline static double distanceAngDeg(double a, double b)
{
  a = constrainAngleDeg(a);
  b = constrainAngleDeg(b);
  if (std::abs(b - a) <= 180)
  {
    return b - a;
  }
  else
  {
    if (b > a)
    {
      return -(360 - std::abs(b - a));
    }
    else
    {
      return (360 - std::abs(b - a));
    }
  }
}

inline double uap_ac_mesh_antenna(double theta, double phi)
{
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
      -0.2f, -0.2f, -0.1f, -0.1f, -0.0f, -0.0f, -0.0f, -0.0f, -0.0f, -0.0f,
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
  if (index == 360)
    index--;
  return antenna_gain[index];
}

inline double uap_ac_mesh_pro_antenna(double theta, double phi)
{
  float antenna_gain[360]{
      -16.8f, -16.6f, -16.1f, -15.3f, -14.4f, -13.5f, -12.6f, -11.7f, -10.9f,
      -10.2f, -9.5f, -8.8f, -8.2f, -7.6f, -7.1f, -6.6f, -6.2f, -5.8f, -5.4f,
      -5.0f, -4.7f, -4.4f, -4.2f, -3.9f, -3.7f, -3.6f, -3.4f, -3.3f, -3.2f,
      -3.1f, -3.1f, -3.1f, -3.1f, -3.2f, -3.3f, -3.4f, -3.5f, -3.6f, -3.8f,
      -4.0f, -4.2f, -4.4f, -4.5f, -4.7f, -4.8f, -4.9f, -4.9f, -4.9f, -4.7f,
      -4.6f, -4.3f, -4.0f, -3.7f, -3.4f, -3.0f, -2.7f, -2.4f, -2.1f, -1.8f,
      -1.5f, -1.3f, -1.1f, -1.0f, -0.9f, -0.8f, -0.8f, -0.8f, -0.9f, -1.0f,
      -1.1f, -1.3f, -1.4f, -1.6f, -1.8f, -2.0f, -2.2f, -2.3f, -2.3f, -2.3f,
      -2.2f, -2.1f, -1.9f, -1.6f, -1.4f, -1.1f, -0.9f, -0.7f, -0.5f, -0.4f,
      -0.4f, -0.4f, -0.4f, -0.6f, -0.8f, -1.0f, -1.3f, -1.7f, -2.1f, -2.5f,
      -2.8f, -3.2f, -3.4f, -3.6f, -3.6f, -3.5f, -3.3f, -3.0f, -2.7f, -2.4f,
      -2.1f, -1.8f, -1.5f, -1.3f, -1.1f, -1.0f, -0.9f, -0.9f, -0.9f, -1.0f,
      -1.1f, -1.2f, -1.4f, -1.6f, -1.9f, -2.2f, -2.5f, -2.8f, -3.2f, -3.5f,
      -3.8f, -4.1f, -4.3f, -4.5f, -4.6f, -4.7f, -4.7f, -4.6f, -4.5f, -4.4f,
      -4.2f, -4.0f, -3.9f, -3.7f, -3.5f, -3.4f, -3.3f, -3.2f, -3.1f, -3.1f,
      -3.1f, -3.1f, -3.1f, -3.2f, -3.3f, -3.4f, -3.5f, -3.7f, -3.9f, -4.1f,
      -4.4f, -4.7f, -5.0f, -5.3f, -5.7f, -6.1f, -6.5f, -7.0f, -7.5f, -8.0f,
      -8.6f, -9.3f, -9.9f, -10.7f, -11.5f, -12.3f, -13.1f, -13.9f, -14.7f,
      -15.4f, -15.8f, -15.9f, -15.7f, -15.2f, -14.5f, -13.7f, -12.9f, -12.0f,
      -11.2f, -10.5f, -9.7f, -9.1f, -8.4f, -7.9f, -7.3f, -6.8f, -6.4f, -5.9f,
      -5.5f, -5.2f, -4.8f, -4.5f, -4.3f, -4.0f, -3.8f, -3.6f, -3.5f, -3.3f,
      -3.2f, -3.1f, -3.1f, -3.0f, -3.0f, -3.1f, -3.1f, -3.2f, -3.2f, -3.4f,
      -3.5f, -3.6f, -3.8f, -4.0f, -4.1f, -4.3f, -4.4f, -4.5f, -4.6f, -4.6f,
      -4.6f, -4.5f, -4.3f, -4.1f, -3.8f, -3.5f, -3.1f, -2.8f, -2.5f, -2.2f,
      -1.9f, -1.6f, -1.4f, -1.2f, -1.1f, -1.0f, -0.9f, -0.9f, -0.9f, -1.0f,
      -1.1f, -1.3f, -1.5f, -1.8f, -2.0f, -2.3f, -2.6f, -2.8f, -3.0f, -3.1f,
      -3.2f, -3.1f, -2.9f, -2.7f, -2.3f, -2.0f, -1.6f, -1.2f, -0.9f, -0.6f,
      -0.4f, -0.2f, -0.1f, -0.0f, 0.0f, -0.0f, -0.1f, -0.3f, -0.5f, -0.7f,
      -0.9f, -1.2f, -1.4f, -1.6f, -1.7f, -1.8f, -1.9f, -1.9f, -1.8f, -1.7f,
      -1.5f, -1.4f, -1.2f, -1.1f, -1.0f, -0.9f, -0.8f, -0.8f, -0.8f, -0.8f,
      -0.9f, -1.0f, -1.1f, -1.3f, -1.5f, -1.7f, -2.0f, -2.3f, -2.6f, -3.0f,
      -3.3f, -3.6f, -3.9f, -4.2f, -4.5f, -4.6f, -4.8f, -4.8f, -4.8f, -4.7f,
      -4.6f, -4.5f, -4.3f, -4.1f, -4.0f, -3.8f, -3.6f, -3.5f, -3.4f, -3.3f,
      -3.2f, -3.1f, -3.1f, -3.1f, -3.2f, -3.2f, -3.3f, -3.4f, -3.6f, -3.8f,
      -4.0f, -4.2f, -4.5f, -4.8f, -5.1f, -5.4f, -5.8f, -6.2f, -6.7f, -7.2f,
      -7.7f, -8.3f, -8.9f, -9.6f, -10.3f, -11.0f, -11.9f, -12.7f, -13.6f,
      -14.5f, -15.4f, -16.2f, -16.7f};
  int index = ((phi + M_PI) * 180.0) / M_PI;
  if (index == 360)
    index--;
  return antenna_gain[index];
}

inline std::string ConvertAddrToString(Address addr)
{
  std::ostringstream stream;
  stream << addr;
  std::string str = stream.str();
  return str;
}

inline std::string ConvertIpv4AddrToString(Ipv4Address addr)
{
  std::ostringstream stream;
  stream << addr;
  std::string str = stream.str();
  return str;
}

inline int32_t extractNodeId(std::string context)
{
  const std::regex re("/NodeList/([0-9]*)/DeviceList/");
  std::vector<int32_t> results;
  for (std::sregex_iterator i =
           std::sregex_iterator(context.begin(), context.end(), re);
       i != std::sregex_iterator(); ++i)
  {
    std::smatch m = *i;
    return std::stoi(m[1].str().c_str());
  }
  return -1;
}

Drone::Drone() {}

void Drone::start()
{
  Simulator::Schedule(Seconds(0.01), &Drone::controller, this, 0.01);
}

double Drone::magnetometer()
{
  return NodeList::GetNode(id)
      ->GetObject<ConstantVelocityMobilityModel>()
      ->GetOrientation()
      .yaw();
}

void Drone::controller(double duration)
{
  angle = constrainAngleDeg((magnetometer() * 180.0 / M_PI));
  Simulator::Schedule(Seconds(duration), &Drone::controller, this, duration);

  /*
   * Data Update 
   */

  for (auto &q : power_histories)
  {
    /*
     * If there is no power curve for the node, we create one which is empty
     */

    if (power_curves.find(q.first) == power_curves.end())
    {
      power_curves[q.first] = std::vector<double>(360, std::nan(""));
    }

    /*
     * We fill the power curve with the current angle. We assume the angle
     * didn't change this much since the previous execution of the controller
     */
    while (q.second.size() > 0)
    {
      if (std::isnan(power_curves[q.first][angle]))
      {
        power_curves[q.first][angle] = q.second.front();
      }
      else
      {
        power_curves[q.first][angle] =
            (power_curves[q.first][angle] + q.second.front()) / 2.0;
      }
      q.second.pop();
    }

    /* Printing the content of the power curves */
    /* std::cout << "Node " << id << "\n";

     for (auto &q : power_curves) {
       for (auto x : q.second) {
         std::cout << x << ", ";
       }
       std::cout << "\n";
     }*/
  }

  // std::cout << "Node " << id << " : " << angle << "\n";
  /* Voisinage considéré */
  if (std::isnan(center))
  {
    center = angle;
  }
  int radius = window / 2;
  int min_limit = center - radius;
  int max_limit = center + radius;
  //std::cout << min_limit << "     " << center << "      " << max_limit << "     " << max_limit - min_limit << "\n";
  //std::cout << angle << "\n";
  /* Find the first angle where we lack data */
  int new_angle = angle;
  bool missing_data = false;
  bool has_neighbors = false;
  int missing_agent = -1;

  /* We search missing data close to our current position */

  /* New */
  int smallest_distance = 1000000;
  for (int i = min_limit; i < max_limit; ++i)
  {
    for (auto &q : power_histories)
    {
      has_neighbors = true;
      //std::cout << constrainAngleDeg(i) << ", " << angle << " " << distanceAngDeg(angle, i) << "\n";
      if (std::isnan(power_curves[q.first][constrainAngleDeg(i)]))
      {
        if (std::abs(distanceAngDeg(angle, i)) < smallest_distance)
        {
          //std::cout << "Old smallest: " << smallest_distance << " new: " << std::abs(distanceAngDeg(angle, i)) << " min " << min_limit << " " << max_limit << "\n";
          smallest_distance = std::abs(distanceAngDeg(angle, i));
          new_angle = constrainAngleDeg(i);
          missing_data = true;
          missing_agent = q.first;
        }
      }
    }
  }

  if (max_limit == min_limit)
  {
    for (auto &q : power_histories)
    {
      has_neighbors = true;
    }
  }

  /* We lack data in the current search window */
  if (missing_data)
  {
    //std::cout << "Node " << id << " at angle " << angle << " lacks data on something: " << missing_agent << " at angle " << new_angle << " (distance = " << smallest_distance << ")\n";
    //std::cout << distanceAngDeg(angle, new_angle) << "\n";
    assert(smallest_distance <= 180.0);
    goal = new_angle;
    if (new_angle == angle)
    {
      timeout += 1;
    }
    else
    {
      timeout = 0;
    }
    if (timeout >= 10)
    { // We spent 0.1 second at the current angle
      for (auto &q : power_histories)
      { // We iterate over all the agents
        if (std::isnan(power_curves[q.first][new_angle]))
        {
          // This agent lacks data for this
          // angle event if we spent some
          // time in the configuration
          power_curves[q.first][angle] = -100.0;
        }
      }
      timeout = 0;
    }
  }

  /* Our search window is full */
  if (!missing_data && has_neighbors && ((goal == -1) || std::abs(goal - angle) < 1.0))
  {
    /* std::cout << "We have data on everything !\n";
    std::cout << "Reducing the window. \n";
    std::cout << "Current angle: " << angle << "\n";
    std::cout << min_limit << "     " << center << "      " << max_limit << "     " << max_limit - min_limit << "\n"; */

    double max_val = -10000000;
    int max_index = angle;
    // For each angle:
    for (int i = min_limit; i < max_limit; ++i)
    {
      double acc = 0.0;
      for (auto &q : this->power_histories)
      {
        acc += power_curves[q.first][constrainAngleDeg(i)];
      }
      power_curve[constrainAngleDeg(i)] = acc;
      if (acc >= max_val)
      {
        max_index = constrainAngleDeg(i);
        max_val = acc;
      }
    }
    new_angle = max_index;
    window = window / 2;
    //    power_curves.clear();

    //    std::cout << "Replacing " << goal << " with " << new_angle << "\n";
    goal = new_angle;
    center = goal;
    /*  for (auto x : power_curve) {
          std::cout << x << ",";
        }
        std::cout << "\n"; */
  }
  else
  {
    //std::cout << missing_data << " " << has_neighbors << " " << goal << " " << angle << "\n";
  }

  if (has_neighbors)
  {
    //std::cout << id << " angle: " << angle << ", goal: " << goal << "\n";
    if (std::abs(int(angle) - goal) >= 0.0001)
    {
      last_change = Simulator::Now().GetSeconds();
      if (distanceAngDeg(angle, goal) >= 0.0)
      {
        // std::cout << "We move right\n";
        NodeList::GetNode(id)
            ->GetObject<ConstantVelocityMobilityModel>()
            ->SetAngularVelocity(Vector(0.0, 0.0, std::abs(speed)));
      }
      else
      {
        NodeList::GetNode(id)
            ->GetObject<ConstantVelocityMobilityModel>()
            ->SetAngularVelocity(Vector(0.0, 0.0, -std::abs(speed)));
      }
    }
    else
    {
      //std::cout << "Already there!\n";
      NodeList::GetNode(id)
          ->GetObject<ConstantVelocityMobilityModel>()
          ->SetAngularVelocity(Vector(0.0, 0.0, 0.0));
    }
  }
  else
  {
    //std::cout << "No neighbours\n";
  }

  if (!has_neighbors)
  {
    if (Simulator::Now().GetSeconds() > delay) {
    NodeList::GetNode(id)
        ->GetObject<ConstantVelocityMobilityModel>()
        ->SetAngularVelocity(Vector(0.0, 0.0, speed));
    }
  }

  /*
    Ptr<Node> node = NodeList::GetNode(id);
    Ptr<ConstantVelocityMobilityModel> mob =
        node->GetObject<ConstantVelocityMobilityModel>();
    Quaternion orientation = mob->GetOrientation();
    std::cout << "[" << Simulator::Now().GetSeconds() << "]"
              << " Current yaw for drone " << id << " : " << orientation.yaw()
              << "\n";
    std::cout << "[" << Simulator::Now().GetSeconds() << "]"
              << " Current pitch for drone " << id << " : " <<
    orientation.pitch()
              << "\n";
    std::cout << "[" << Simulator::Now().GetSeconds() << "]"
              << " Current roll for drone " << id << " : " << orientation.roll()
              << "\n";
    std::cout << (mob->GetOrientation()) << "\n";
    std::cout << "'" << (mob->GetAngularVelocity()) << "'\n"; */
}

Simulation::Simulation() {}

void Simulation::set_id(int32_t simulation_id)
{
  this->simulation_id = simulation_id;
}

const int32_t Simulation::get_id() { return this->simulation_id; }

void Simulation::createNodeMap()
{
  for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i)
  {
    Ptr<Node> node = NodeList::GetNode(i);
    for (uint32_t j = 0; j < node->GetNDevices(); ++j)
    {
      if (ConvertAddrToString(node->GetDevice(j)->GetAddress()) !=
          "03-06-00:00:00:00:00:00")
      {
        this->nodes.insert(std::pair<std::string, uint32_t>(
            ConvertAddrToString(node->GetDevice(j)->GetAddress()),
            node->GetId()));
      }
    }
  }
}

PacketInfo Simulation::getPacketInfo(Ptr<Packet> p)
{
  PacketInfo packetInfo;
  Mac48Address Addr1 = Mac48Address();
  Mac48Address Addr2 = Mac48Address();
  Mac48Address Addr3 = Mac48Address();
  Mac48Address Addr4 = Mac48Address();

  PacketMetadata::ItemIterator i = p->BeginItem();
  while (i.HasNext())
  {
    PacketMetadata::Item item = i.Next();
    if (!item.isFragment)
    {
      switch (item.type)
      {
      case PacketMetadata::Item::PAYLOAD:
        break;
      case PacketMetadata::Item::HEADER:
      case PacketMetadata::Item::TRAILER:
        if (item.tid.GetName() == "ns3::WifiMacHeader")
        {
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
          if (wifiMacHeader->IsData() && wifiMacHeader->HasData())
          {
            packetInfo.data_type = 0;
          }
          else if (wifiMacHeader->IsAck() or wifiMacHeader->IsBlockAck())
          {
            packetInfo.data_type = 1;
          }
          else
          {
            packetInfo.data_type = 99;
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
  if (this->nodes.count(ConvertAddrToString(Addr1)) != 0)
  {
    packetInfo.addr1 = this->nodes[ConvertAddrToString(Addr1)];
  }
  else
  {
    packetInfo.addr1 = -1;
  }
  if (this->nodes.count(ConvertAddrToString(Addr2)) != 0)
  {
    packetInfo.addr2 = this->nodes[ConvertAddrToString(Addr2)];
  }
  else
  {
    packetInfo.addr2 = -1;
  }
  if (this->nodes.count(ConvertAddrToString(Addr3)) != 0)
  {
    packetInfo.addr3 = this->nodes[ConvertAddrToString(Addr3)];
  }
  else
  {
    packetInfo.addr3 = -1;
  }
  if (this->nodes.count(ConvertAddrToString(Addr4)) != 0)
  {
    packetInfo.addr4 = this->nodes[ConvertAddrToString(Addr4)];
  }
  else
  {
    packetInfo.addr4 = -1;
  }

  return packetInfo;
}

void Simulation::ThroughputMonitor(flowsdata_t &flows,
                                   FlowMonitorHelper *fmHelper,
                                   Ptr<FlowMonitor> flowMon)
{
  float duration = 1.0;
  flowMon->CheckForLostPackets();
  Ptr<Ipv4FlowClassifier> classifier =
      DynamicCast<Ipv4FlowClassifier>(fmHelper->GetClassifier());
  FlowMonitor::FlowStatsContainer stats = flowMon->GetFlowStats();

  std::map<int, flowdata_t> instantflows;

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i =
           stats.begin();
       i != stats.end(); ++i)
  {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
    instantflows[i->first] = std::make_tuple(
        ConvertIpv4AddrToString(t.sourceAddress),
        ConvertIpv4AddrToString(t.destinationAddress), i->second.txPackets,
        i->second.txBytes, i->second.rxPackets, i->second.rxBytes);
    if (flows.begin() != flows.end())
    {
      if (flows.back().second.find(i->first) != flows.back().second.end())
      {
        if (t.destinationPort == 1337)
          continue;
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
  Simulator::Schedule(Seconds(duration), &Simulation::ThroughputMonitor, this,
                      flows, fmHelper, flowMon);
}

void Simulation::ThroughputMonitorFirst(flowsdata_t &flows,
                                        FlowMonitorHelper *fmHelper,
                                        Ptr<FlowMonitor> flowMon)
{
  std::cout << "continuous,Time,Flow,Source Address,Destination Address,Tx "
               "Packets,Tx Bytes,Rx Packets,Rx Bytes\n";
  this->ThroughputMonitor(flows, fmHelper, flowMon);
}

void Simulation::DevMonitorSnifferRx(
    std::string nodeIdStr, Ptr<const Packet> packet, uint16_t channelFreqMhz,
    WifiTxVector txVector, struct MpduInfo aMpdu,
    struct SignalNoiseDbm signalNoise, uint16_t staId)
{
  int32_t source_agent_id = -1;
  int32_t destination_agent_id = -1;
  double rxPower = 0.0;

  int32_t node = extractNodeId(nodeIdStr);
  if ((aMpdu.type == NORMAL_MPDU) || (aMpdu.type == SINGLE_MPDU) ||
      (aMpdu.type == FIRST_MPDU_IN_AGGREGATE))
  {

    Ptr<Packet> copy = packet->Copy();
    PacketInfo packetInfo = this->getPacketInfo(copy);

    // MonitorQuery monitor_query = MonitorQuery();

    if (packetInfo.data_type == 0)
    { /* Data Frame */
      if ((int)node != packetInfo.addr1)
      {
        return;
      }
      else
      {
        source_agent_id = packetInfo.addr2;
      }
    }
    else if (packetInfo.data_type == 1)
    { /* Ack Frame */
      if ((int)node == packetInfo.addr1)
      { /* Ack Frame for Me */
        if (drones[node].last_transmitted != -1)
        {
          source_agent_id = drones[node].last_transmitted;
        }
        else
        {
          return;
        }
      }
      else
      { /* Ack Frame not for Me */
        return;
      }
    }
    else
    {
      /* std::cout << "Non ack / data packet, ignoring.\n"; */
      return;
    }
    destination_agent_id = node;
    if (drones[node].power_histories.find(source_agent_id) ==
        drones[node].power_histories.end())
    {
      FixedQueue<double, 64> queue;
      drones[node].power_histories[source_agent_id] = queue;
    }
    drones[node].power_histories[source_agent_id].push(signalNoise.signal);
  }
}

void Simulation::DevMonitorSnifferTx(std::string nodeIdStr,
                                     Ptr<const Packet> packet,
                                     uint16_t channelFreqMhz,
                                     WifiTxVector txVector,
                                     struct MpduInfo aMpdu, uint16_t staId)
{
  uint32_t node = extractNodeId(nodeIdStr);
  if ((aMpdu.type == NORMAL_MPDU) || (aMpdu.type == SINGLE_MPDU) ||
      (aMpdu.type == FIRST_MPDU_IN_AGGREGATE))
  {
    Ptr<Packet> copy = packet->Copy();
    PacketInfo packetInfo = this->getPacketInfo(copy);
    if (packetInfo.data_type == 0)
    { /* Data Frame */
      if (packetInfo.addr1 != -1)
      {
        drones[node].last_transmitted = packetInfo.addr1;
      }
    }
    else if (packetInfo.data_type == 1)
    { /* Ack */
    }
    else
    { /* Something else I send */
    }
  }
}

void Simulation::MacTx(std::string nodeIdStr, Ptr<const Packet> packet)
{
  uint32_t node = extractNodeId(nodeIdStr);
  if (this->macTx.count(node))
  {
    this->macTx[node]++;
  }
  else
  {
    this->macTx[node] = 1;
  }
}

void Simulation::MacTxDataFailed(std::string nodeIdStr,
                                 ns3::Mac48Address addr)
{
  uint32_t node = extractNodeId(nodeIdStr);
  if (this->macTxDataFailed.count(node))
  {
    this->macTxDataFailed[node]++;
  }
  else
  {
    this->macTxDataFailed[node] = 1;
  }
}

void Simulation::init(std::vector<std::string> agent_types, int agent_number,
                      double simulationTime, ScenarioType scenarioType, bool enableDelayedStart, Ptr<UniformRandomVariable> randomVariable)
{
  this->simulationTime = simulationTime;
  this->wifiNodes.Create(agent_number);
  this->scenarioType = scenarioType;

  for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i)
  {
    Ptr<Node> node = NodeList::GetNode(i);
    /* Create a Drone object associated to the node */
    drones[node->GetId()] = Drone();
    drones[node->GetId()].agent_type = agent_types[i];
    drones[node->GetId()].id = node->GetId();
    if (enableDelayedStart) {
      drones[node->GetId()].delay = 1.0 + (randomVariable->GetValue() * 19.0) / (randomVariable->GetMax() - randomVariable->GetMin());
    }
    drones[node->GetId()].speed = 1.00 - 0.05 + (randomVariable->GetValue() * 0.20) / (randomVariable->GetMax() - randomVariable->GetMin());
    if (randomVariable->GetValue() >= ((randomVariable->GetMax() - randomVariable->GetMin()) / 2.0))
    {
      drones[node->GetId()].speed *= -1.0;
    }
    if (agent_types[i] == "Drone")
    {
      drones[node->GetId()].start();
    }
  }
}

  void Simulation::configureMobility(Ptr<ListPositionAllocator> positionAlloc,
                                     Ptr<UniformRandomVariable> randomVariable)
  {

    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
    mobility.Install(wifiNodes);

    /* Actually set up the positions */
    for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i)
    {
      Ptr<ConstantVelocityMobilityModel> mob =
          NodeList::GetNode(i)->GetObject<ConstantVelocityMobilityModel>();
      mob->SetPosition(positionAlloc->GetNext());
    }

    /* Set up the nodes initial orientations */
    for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i)
    {
      Ptr<ConstantVelocityMobilityModel> mob =
          NodeList::GetNode(i)->GetObject<ConstantVelocityMobilityModel>();
      double random_angle = randomVariable->GetValue() * 2 * M_PI /
                            (randomVariable->GetMax() - randomVariable->GetMin());

      Quaternion random_orientation = Quaternion(random_angle, Vector(0, 0, 1));

      NodeList::GetNode(i)
          ->GetObject<ConstantVelocityMobilityModel>()
          ->SetOrientation(random_orientation);
      NodeList::GetNode(i)
          ->GetObject<ConstantVelocityMobilityModel>()
          ->SetAngularVelocity(Vector(0.0, 0.0, 0.0));
    }

    std::cout
        << "initialpositions,agent_id,position_x,position_y,position_z,"
           "orientation_x,orientation_y,orientation_z,orientation_w,angle\n";

    for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i)
    {
      Ptr<Node> node = NodeList::GetNode(i);
      Ptr<ConstantVelocityMobilityModel> mob =
          node->GetObject<ConstantVelocityMobilityModel>();
      Quaternion orientation = mob->GetOrientation();
      Vector position = mob->GetPosition();
      std::cout << "initialpositions"
                << "," << node->GetId() << "," << position.x << "," << position.y
                << "," << position.z << "," << orientation.x << ","
                << orientation.y << "," << orientation.z << "," << orientation.w
                << ","
                << constrainAngleDeg((drones[i].magnetometer() * 180.0 / M_PI))
                << "\n";
    }
  }

  void Simulation::configureWifi(bool enablePcap, int channelWidth, int mcs,
                                 bool sgi, std::string wifiManager, std::string antennaModel, double antennaGain, bool enableFastFading)
  {
    this->enablePcap = enablePcap;
    this->channelWidth = channelWidth;
    this->mcs = mcs;
    this->sgi = sgi;
    this->wifiManager = wifiManager;

    YansWifiPhyHelper phy = YansWifiPhyHelper();
    // phy.DisablePreambleDetectionModel();
    phy.SetPcapDataLinkType(YansWifiPhyHelper::DLT_IEEE802_11_RADIO);

    YansWifiChannelHelper channel;
    channel.AddPropagationLoss("ns3::FriisPropagationLossModel");
    if (enableFastFading)
    {
      channel.AddPropagationLoss("ns3::NakagamiPropagationLossModel");
    }
    channel.AddPropagationLoss("ns3::AntennaPropagationLossModel");
    channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
    phy.SetChannel(channel.Create());

    WifiHelper wifi;
    wifi.SetStandard(WIFI_PHY_STANDARD_80211ac);
    WifiMacHelper mac;

    if (mcs >= 0 && mcs <= 9)
    {
      if (mcs == 9 && channelWidth == 20)
      {
        std::cout << "MCS 9 and 20 MHZ width is disabled."
                  << "\n";
        exit(0);
      }
      std::ostringstream oss;
      oss << "VhtMcs" << mcs;
      wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode",
                                   StringValue(oss.str()), "ControlMode",
                                   StringValue(oss.str()));
    }
    else
    {
      wifi.SetRemoteStationManager(wifiManager);
    }

    Ssid ssid = Ssid("ns3-80211ac");

    mac.SetType("ns3::AdhocWifiMac");

    this->devices = wifi.Install(phy, mac, this->wifiNodes);

    if (enablePcap)
    {
      phy.EnablePcap("STA", devices);
    }

    /* Set channel width */
    Config::Set("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth",
                UintegerValue(channelWidth));

    /* Set guard interval */
    Config::Set("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/HtConfiguration/"
                "ShortGuardIntervalSupported",
                BooleanValue(sgi));

    /* Post-install configuration for channel width, guard interval, NSS and
   * Antennas */

    std::map<std::string, std::function<double(double, double)>> antennas = {
        {"uap-ac-mesh", [antennaGain](double theta, double phi) { return antennaGain + uap_ac_mesh_antenna(theta, phi); }},
        {"uap-ac-mesh-pro", [antennaGain](double theta, double phi) { return antennaGain + uap_ac_mesh_pro_antenna(theta, phi); }},
        {"isotropic", [antennaGain](double theta, double phi) { return 0.0; }}};

    for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i)
    {
      Ptr<NetDevice> nd = devices.Get(i);
      Ptr<WifiNetDevice> wnd = nd->GetObject<WifiNetDevice>();
      Ptr<WifiPhy> wp = wnd->GetPhy();
      wp->SetNumberOfAntennas(2);
      wp->SetMaxSupportedTxSpatialStreams(2);
      wp->SetMaxSupportedRxSpatialStreams(2);

      Ptr<CustomAntennaModel> apAntenna = CreateObject<CustomAntennaModel>();

      if (drones[i].agent_type == "Drone")
      {
        apAntenna->SetModel(antennas[antennaModel]);
      }
      else if (drones[i].agent_type == "Client")
      {
        apAntenna->SetModel(antennas["isotropic"]);
      }
      else
      {
        std::cerr << "Unknown agent type.\n";
        std::exit(-1);
      }
      apAntenna->Install(NodeList::GetNode(i));
    }
  }

  void Simulation::configureTraces()
  {
    for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i)
    {
      Config::Connect(
          "/NodeList/" + std::to_string(i) +
              "/DeviceList/*/$ns3::WifiNetDevice/Phy/MonitorSnifferRx",
          MakeCallback(&Simulation::DevMonitorSnifferRx, this));
      Config::Connect(
          "/NodeList/" + std::to_string(i) +
              "/DeviceList/*/$ns3::WifiNetDevice/Phy/MonitorSnifferTx",
          MakeCallback(&Simulation::DevMonitorSnifferTx, this));

      Config::Connect("/NodeList/" + std::to_string(i) +
                          "/DeviceList/*/$ns3::WifiNetDevice/"
                          "RemoteStationManager/MacTxDataFailed",
                      MakeCallback(&Simulation::MacTxDataFailed, this));

      Config::Connect("/NodeList/" + std::to_string(i) +
                          "/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx",
                      MakeCallback(&Simulation::MacTx, this));
    }
  }

  void Simulation::configureInternet()
  {
    InternetStackHelper stack;
    stack.Install(this->wifiNodes);

    Ipv4AddressHelper address;
    address.SetBase("192.168.1.0", "255.255.255.0");
    this->interfaces = address.Assign(this->devices);
  }

  void Simulation::configureRouting()
  {
    /*   for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i) {
    Ptr<Ipv4> ipv4 = wifiNodes.Get(i)->GetObject<Ipv4>();
    NS_LOG_DEBUG("Node " << i << " has " << ipv4->GetNInterfaces()
                         << " interfaces:\n");
    for (uint32_t j = 0; j < ipv4->GetNInterfaces(); ++j) {
      NS_LOG_DEBUG("\tInterface " << j << " has " << ipv4->GetNAddresses(j)
                                  << " addresses:\n");
      for (uint32_t k = 0; k < ipv4->GetNAddresses(j); ++k) {
        NS_LOG_DEBUG("\t\t" << ipv4->GetAddress(j, k).GetLocal() << "\n");
      }
    }
  } */

    Ipv4StaticRoutingHelper staticRoutingHelper;

    if ((this->scenarioType == SIMPLE) || (this->scenarioType == CHAIN))
    {
      for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i)
      {
        Ptr<Ipv4> ipv4 = wifiNodes.Get(i)->GetObject<Ipv4>();
        Ptr<Ipv4StaticRouting> staticRouting =
            staticRoutingHelper.GetStaticRouting(ipv4);

        if (i > 0)
        {
          staticRouting->AddHostRouteTo(
              Ipv4Address(("192.168.1." + std::to_string(i)).c_str()), 1);

          for (uint32_t j = 0; j < i - 1; ++j)
          {
            staticRouting->AddHostRouteTo(
                Ipv4Address(("192.168.1." + std::to_string(j + 1)).c_str()),
                Ipv4Address(("192.168.1." + std::to_string(i)).c_str()), 1);
          }
        }

        for (uint32_t j = i + 2; j < NodeList::GetNNodes(); ++j)
        {
          staticRouting->AddHostRouteTo(
              Ipv4Address(("192.168.1." + std::to_string(j + 1)).c_str()),
              Ipv4Address(("192.168.1." + std::to_string(i + 2)).c_str()), 1);
        }

        if (i < NodeList::GetNNodes() - 1)
        {
          staticRouting->AddHostRouteTo(
              Ipv4Address(("192.168.1." + std::to_string(i + 2)).c_str()), 1);
        }
      }
    }
    else if (this->scenarioType == SINK)
    {
      for (uint32_t i = 1; i < NodeList::GetNNodes(); ++i)
      {
        Ptr<Ipv4> ipv4 = wifiNodes.Get(i)->GetObject<Ipv4>();
        Ptr<Ipv4StaticRouting> staticRouting =
            staticRoutingHelper.GetStaticRouting(ipv4);
        staticRouting->AddHostRouteTo(
            Ipv4Address(("192.168.1." + std::to_string(0 + 1)).c_str()), 1);
      }

      for (uint32_t i = 1; i < NodeList::GetNNodes(); ++i)
      {
        Ptr<Ipv4> ipv4 = wifiNodes.Get(0)->GetObject<Ipv4>();
        Ptr<Ipv4StaticRouting> staticRouting =
            staticRoutingHelper.GetStaticRouting(ipv4);
        staticRouting->AddHostRouteTo(
            Ipv4Address(("192.168.1." + std::to_string(i + 1)).c_str()), 1);
      }
    }
  }

  void Simulation::configureApplications(std::string dataRate, bool udp,
                                         uint32_t packetSize)
  {
    this->dataRate = dataRate;
    this->udp = udp;
    this->packetSize = packetSize;

    uint32_t payloadSize; // 1500 byte IP packet
    if (this->udp)
    {
      payloadSize = 1472; // bytes
    }
    else
    {
      payloadSize = 1448; // bytes
      Config::SetDefault("ns3::TcpSocket::SegmentSize",
                         UintegerValue(payloadSize));
    }

    /* Generate traffic to populate the ARP tables before the start of the
   * applications */
    for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i)
    {
      uint16_t port = 1337;
      PacketSinkHelper sink(
          "ns3::UdpSocketFactory",
          InetSocketAddress(this->interfaces.GetAddress(i), port));
      ApplicationContainer serverApp = sink.Install(wifiNodes.Get(i));
      serverApp.Start(Seconds(0.0));
      serverApp.Stop(Seconds(1));

      if (this->scenarioType == CHAIN)
      {
        /* Node Before */
        if (i > 0)
        {
          UdpClientHelper client(
              InetSocketAddress(this->interfaces.GetAddress(i - 1), port));
          client.SetAttribute("MaxPackets", UintegerValue(10));
          client.SetAttribute("Interval", TimeValue(Seconds(.05)));
          client.SetAttribute("PacketSize", UintegerValue(12));

          ApplicationContainer clientApp = client.Install(wifiNodes.Get(i));
          clientApp.Start(Seconds(i * 0.02));
          clientApp.Stop(Seconds(1));
        }
        /* Node After */
        if (i < NodeList::GetNNodes() - 1)
        {
          UdpClientHelper client(
              InetSocketAddress(this->interfaces.GetAddress(i + 1), port));
          client.SetAttribute("MaxPackets", UintegerValue(10));
          client.SetAttribute("Interval", TimeValue(Seconds(.05)));
          client.SetAttribute("PacketSize", UintegerValue(12));

          ApplicationContainer clientApp = client.Install(wifiNodes.Get(i));
          clientApp.Start(Seconds(i * 0.02));
          clientApp.Stop(Seconds(1));
        }
      }
      else if ((this->scenarioType == SIMPLE) || (this->scenarioType == SINK))
      {
        if (i != 0)
        {
          UdpClientHelper client(
              InetSocketAddress(this->interfaces.GetAddress(0), port));
          client.SetAttribute("MaxPackets", UintegerValue(10));
          client.SetAttribute("Interval", TimeValue(Seconds(.05)));
          client.SetAttribute("PacketSize", UintegerValue(12));

          ApplicationContainer clientApp = client.Install(wifiNodes.Get(i));
          clientApp.Start(Seconds(i * 0.02));
          clientApp.Stop(Seconds(1));
        }
      }
    }

    /* Broadcast 10 times per second with the neighbours */
    for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i)
    {
      uint16_t port = 9;
      PacketSinkHelper sink(
          "ns3::UdpSocketFactory",
          InetSocketAddress(this->interfaces.GetAddress(i), port));
      ApplicationContainer serverApp = sink.Install(wifiNodes.Get(i));
      serverApp.Start(Seconds(0.0 + i * 0.01));
      serverApp.Stop(Seconds(this->simulationTime + 1));

      UdpClientHelper client(InetSocketAddress(
          this->interfaces.GetAddress(0).GetSubnetDirectedBroadcast(
              Ipv4Mask("/24")),
          port));
      client.SetAttribute("MaxPackets",
                          UintegerValue(2 * this->simulationTime * 1 / (0.10)));
      client.SetAttribute("Interval", TimeValue(Seconds(.10)));
      client.SetAttribute("PacketSize", UintegerValue(12));

      ApplicationContainer clientApp = client.Install(wifiNodes.Get(i));
      clientApp.Start(Seconds(1.01 + i * 0.01));
      clientApp.Stop(Seconds(this->simulationTime + 1));
    }

    uint16_t port = 1234;
    std::string socketFactory;

    if (udp)
    {
      socketFactory = "ns3::UdpSocketFactory";
    }
    else
    {
      socketFactory = "ns3::TcpSocketFactory";
    }

    PacketSinkHelper sink(
        socketFactory, InetSocketAddress(this->interfaces.GetAddress(0), port));
    this->serverApp = sink.Install(wifiNodes.Get(0));
    this->serverApp.Start(Seconds(0.0));
    this->serverApp.Stop(Seconds(this->simulationTime + 1));

    if ((this->scenarioType == SIMPLE) || (this->scenarioType == CHAIN))
    {
      OnOffHelper onoff(socketFactory,
                        InetSocketAddress(this->interfaces.GetAddress(0), port));
      onoff.SetConstantRate(DataRate(this->dataRate), this->packetSize);
      onoff.SetAttribute("StartTime", TimeValue(Seconds(1.000000)));
      ApplicationContainer clientApp =
          onoff.Install(wifiNodes.Get(NodeList::GetNNodes() - 1));
      clientApp.Start(Seconds(1.0));
      clientApp.Stop(Seconds(this->simulationTime + 1));
    }
    else if (this->scenarioType == SINK)
    {
      for (uint32_t i = 1; i < NodeList::GetNNodes(); ++i)
      {

        OnOffHelper onoff(
            socketFactory,
            InetSocketAddress(this->interfaces.GetAddress(0), port));
        onoff.SetConstantRate(DataRate(dataRate), this->packetSize);
        onoff.SetAttribute("StartTime", TimeValue(Seconds(1.0)));
        ApplicationContainer clientApp = onoff.Install(wifiNodes.Get(i));
        clientApp.Start(Seconds(1.0));
        clientApp.Stop(Seconds(simulationTime + 1));
      }
    }
  }

  void Simulation::end()
  {
    uint64_t rxBytes = 0;
    rxBytes = DynamicCast<PacketSink>(serverApp.Get(0))->GetTotalRx();

    std::cout << "lost,type,value\n";
    for (const auto &kv : this->macTx)
    {
      std::cout << "lost,macTx," << kv.first << "," << kv.second << "\n";
    }

    for (const auto &kv : this->macTxDataFailed)
    {
      std::cout << "lost,macTxDataFailed," << kv.first << "," << kv.second
                << "\n";
    }

    std::cout << "phi,dataRate,wifiManager,mcs,Rx Bytes\n";
    std::cout << "phi"
              << "," << this->dataRate << "," << this->wifiManager << ","
              << this->mcs << "," << rxBytes << "\n";

    std::cout
        << "positions,agent_id,position_x,position_y,position_z,orientation_x,"
           "orientation_y,orientation_z,orientation_w,angle,last_change\n";
    for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i)
    {
      Ptr<Node> node = NodeList::GetNode(i);
      Ptr<ConstantVelocityMobilityModel> mob =
          node->GetObject<ConstantVelocityMobilityModel>();
      Quaternion orientation = mob->GetOrientation();
      Vector position = mob->GetPosition();
      std::cout << "positions"
                << "," << i << "," << position.x << "," << position.y << ","
                << position.z << "," << orientation.x << "," << orientation.y
                << "," << orientation.z << "," << orientation.w << "," << drones[i].angle
                << "," << drones[i].last_change << "\n";
    }
    std::cout << std::flush;
  }
