cmake_minimum_required(VERSION 3.20)

project(pspsdk_find_prefix_probe LANGUAGES C)

find_path(PSPSDK_FIND_PROBE_INCLUDE NAMES pspsdk-find-probe.h REQUIRED)
find_library(PSPSDK_FIND_PROBE_LIBRARY NAMES pspsdk-find-probe REQUIRED)
find_package(pspsdk-find-probe CONFIG REQUIRED)

if(NOT PSPSDK_FIND_PROBE_CONFIG_FOUND)
    message(FATAL_ERROR "config-mode package probe was not loaded")
endif()

set(expected_include "$ENV{PSPDEV}/psp/include")
set(expected_library "$ENV{PSPDEV}/psp/lib/libpspsdk-find-probe.a")

if(NOT PSPSDK_FIND_PROBE_INCLUDE STREQUAL expected_include)
    message(FATAL_ERROR "find_path resolved '${PSPSDK_FIND_PROBE_INCLUDE}', expected '${expected_include}'")
endif()

if(NOT PSPSDK_FIND_PROBE_LIBRARY STREQUAL expected_library)
    message(FATAL_ERROR "find_library resolved '${PSPSDK_FIND_PROBE_LIBRARY}', expected '${expected_library}'")
endif()
