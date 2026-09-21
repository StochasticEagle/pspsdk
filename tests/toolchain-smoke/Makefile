PSPSDK := $(shell psp-config --pspsdk-path)
PSP_BUILD_FRAGMENT ?= build.mak
BUILD_DIR ?= build
PROBE_NAME ?= make

CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

include $(PSPSDK)/lib/$(PSP_BUILD_FRAGMENT)

.PHONY: probe
probe:
	mkdir -p "$(BUILD_DIR)"
	$(CXX) $(CXXFLAGS) -c cxx_include_probe.cpp -o "$(BUILD_DIR)/$(PROBE_NAME)_cxx_include_probe.o"
