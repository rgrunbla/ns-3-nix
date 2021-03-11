# the compiler: gcc for C program, define as g++ for C++
CC = clang++

DEBUG=-DNS3_LOG_ENABLE

LIBS = libns3-dev-core-debug libns3-dev-antenna-debug libns3-dev-internet-debug libns3-dev-spectrum-debug libns3-dev-applications-debug libns3-dev-lr-wpan-debug libns3-dev-stats-debug libns3-dev-bridge-debug libns3-dev-lte-debug libns3-dev-traffic-control-debug libns3-dev-buildings-debug libns3-dev-mobility-debug libns3-dev-uan-debug libns3-dev-config-store-debug libns3-dev-netanim-debug libns3-dev-virtual-net-device-debug libns3-dev-core-debug libns3-dev-network-debug libns3-dev-wave-debug libns3-dev-csma-debug libns3-dev-point-to-point-debug libns3-dev-wifi-debug libns3-dev-energy-debug libns3-dev-point-to-point-layout-debug libns3-dev-wimax-debug libns3-dev-fd-net-device-debug libns3-dev-propagation-debug
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -Wextra $(shell pkg-config --cflags $(LIBS))
LDFLAGS=$(shell pkg-config --libs $(LIBS))

# the build target executable:
TARGET = hello-simulator

all: $(TARGET)

$(TARGET): $(TARGET).cc
	$(CC) $(DEBUG) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(TARGET).cc

clean:
	$(RM) $(TARGET)