# the compiler: gcc for C program, define as g++ for C++
CC = clang++

DEBUG=-DNS3_LOG_ENABLE

#LIBS = libns3-dev-core-debug libns3-dev-flow-monitor-debug libns3-dev-antenna-debug libns3-dev-internet-debug libns3-dev-spectrum-debug libns3-dev-applications-debug libns3-dev-lr-wpan-debug libns3-dev-stats-debug libns3-dev-bridge-debug libns3-dev-lte-debug libns3-dev-traffic-control-debug libns3-dev-buildings-debug libns3-dev-mobility-debug libns3-dev-uan-debug libns3-dev-config-store-debug libns3-dev-netanim-debug libns3-dev-virtual-net-device-debug libns3-dev-core-debug libns3-dev-network-debug libns3-dev-wave-debug libns3-dev-csma-debug libns3-dev-point-to-point-debug libns3-dev-wifi-debug libns3-dev-energy-debug libns3-dev-point-to-point-layout-debug libns3-dev-wimax-debug libns3-dev-fd-net-device-debug libns3-dev-propagation-debug
#LIBS = libns3-dev-core-optimized libns3-dev-flow-monitor-optimized libns3-dev-antenna-optimized libns3-dev-internet-optimized libns3-dev-spectrum-optimized libns3-dev-applications-optimized libns3-dev-lr-wpan-optimized libns3-dev-stats-optimized libns3-dev-bridge-optimized libns3-dev-lte-optimized libns3-dev-traffic-control-optimized libns3-dev-buildings-optimized libns3-dev-mobility-optimized libns3-dev-uan-optimized libns3-dev-config-store-optimized libns3-dev-netanim-optimized libns3-dev-virtual-net-device-optimized libns3-dev-core-optimized libns3-dev-network-optimized libns3-dev-wave-optimized libns3-dev-csma-optimized libns3-dev-point-to-point-optimized libns3-dev-wifi-optimized libns3-dev-energy-optimized libns3-dev-point-to-point-layout-optimized libns3-dev-wimax-optimized libns3-dev-fd-net-device-optimized libns3-dev-propagation-optimized
LIBS = libns3.33-core-debug libns3.33-flow-monitor-debug libns3.33-antenna-debug libns3.33-internet-debug libns3.33-spectrum-debug libns3.33-applications-debug libns3.33-lr-wpan-debug libns3.33-stats-debug libns3.33-bridge-debug libns3.33-lte-debug libns3.33-traffic-control-debug libns3.33-buildings-debug libns3.33-mobility-debug libns3.33-uan-debug libns3.33-config-store-debug libns3.33-netanim-debug libns3.33-virtual-net-device-debug libns3.33-core-debug libns3.33-network-debug libns3.33-wave-debug libns3.33-csma-debug libns3.33-point-to-point-debug libns3.33-wifi-debug libns3.33-energy-debug libns3.33-point-to-point-layout-debug libns3.33-wimax-debug libns3.33-fd-net-device-debug libns3.33-propagation-debug
LIBS = libns3.33-core-optimized libns3.33-flow-monitor-optimized libns3.33-antenna-optimized libns3.33-internet-optimized libns3.33-spectrum-optimized libns3.33-applications-optimized libns3.33-lr-wpan-optimized libns3.33-stats-optimized libns3.33-bridge-optimized libns3.33-lte-optimized libns3.33-traffic-control-optimized libns3.33-buildings-optimized libns3.33-mobility-optimized libns3.33-uan-optimized libns3.33-config-store-optimized libns3.33-netanim-optimized libns3.33-virtual-net-device-optimized libns3.33-core-optimized libns3.33-network-optimized libns3.33-wave-optimized libns3.33-csma-optimized libns3.33-point-to-point-optimized libns3.33-wifi-optimized libns3.33-energy-optimized libns3.33-point-to-point-layout-optimized libns3.33-wimax-optimized libns3.33-fd-net-device-optimized libns3.33-propagation-optimized

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -Wextra $(shell pkg-config --cflags $(LIBS))
CFLAGS  = -O3 -Wall -Wextra $(shell pkg-config --cflags $(LIBS))
LDFLAGS=$(shell pkg-config --libs $(LIBS))

# the build target executable:
TARGET = hello-simulator
DEPS = common

all: $(TARGET)

$(TARGET): $(TARGET).cpp $(DEPS).cpp
	$(CC) $(DEBUG) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(TARGET).cpp $(DEPS).cpp

clean: 
	$(RM) *.o *~ $(TARGET)
