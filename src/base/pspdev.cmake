if(DEFINED ENV{PSPDEV})
    SET(PSPDEV $ENV{PSPDEV})
else()
    message(FATAL_ERROR "The environment variable PSPDEV needs to be defined.")
endif()

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_SYSTEM_PROCESSOR mips)
SET(CMAKE_C_COMPILER "${PSPDEV}/bin/psp-gcc")
SET(CMAKE_CXX_COMPILER "${PSPDEV}/bin/psp-g++")
SET(CMAKE_C_FLAGS_INIT "-DPSP -D__PSP__ -D_PSP_FW_VERSION=600")
SET(CMAKE_CXX_FLAGS_INIT "-DPSP -D__PSP__ -D_PSP_FW_VERSION=600")
# CMake does not infer the PSP C/sysroot headers for Generic targets, so
# provide them explicitly for C. For C++, psp-g++ must retain its native
# libstdc++ -> libc include ordering: putting ${PSPDEV}/psp/include ahead of
# the compiler's C++ headers breaks libstdc++ #include_next directives.
SET(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES
    "${PSPDEV}/psp/include"
    "${PSPDEV}/psp/sdk/include")
SET(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
    "${PSPDEV}/psp/sdk/include")
SET(CMAKE_EXE_LINKER_FLAGS_INIT "-L${PSPDEV}/lib -L${PSPDEV}/psp/lib -L${PSPDEV}/psp/sdk/lib -Wl,-zmax-page-size=128")
#SET(CMAKE_SHARED_LINKER_FLAGS_INIT "...")
#SET(CMAKE_STATIC_LINKER_FLAGS_CONFIG_INIT "...")
#SET(CMAKE_STATIC_LINKER_FLAGS_INIT "...")
SET(CMAKE_INSTALL_PREFIX "${PSPDEV}/psp" CACHE PATH "install path")

SET(CMAKE_FIND_ROOT_PATH ${PSPDEV} ${PSPDEV}/psp ${PSPDEV}/psp/sdk)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
SET(CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_FIND_ROOT_PATH})

find_program(PSP_PKG_CONFIG_EXECUTABLE NAMES pkg-config pkgconf NO_CMAKE_FIND_ROOT_PATH)
if(NOT PSP_PKG_CONFIG_EXECUTABLE)
    message(FATAL_ERROR "A host pkg-config implementation is required.")
endif()
SET(PKG_CONFIG_EXECUTABLE "${PSP_PKG_CONFIG_EXECUTABLE}" CACHE FILEPATH "Path to host pkg-config" FORCE)
SET(PKG_CONFIG_ARGN "--static" CACHE STRING "Arguments passed to pkg-config" FORCE)
SET(PKG_CONFIG_USE_CMAKE_PREFIX_PATH FALSE CACHE BOOL "Keep host paths out of PSP pkg-config lookup" FORCE)
SET(ENV{PKG_CONFIG_DIR} "")
SET(ENV{PKG_CONFIG_PATH} "")
SET(ENV{PKG_CONFIG_SYSROOT_DIR} "${PSPDEV}")
SET(ENV{PKG_CONFIG_LIBDIR} "${PSPDEV}/psp/lib/pkgconfig:${PSPDEV}/psp/share/pkgconfig")

## Add Default PSPSDK Libraries according to build.mak. Header search is
## language-specific above; do not globally re-add the libc include directory.
link_directories( ${PSPDEV}/lib ${PSPDEV}/psp/lib ${PSPDEV}/psp/sdk/lib)

SET(PLATFORM_PSP TRUE)
SET(PSP TRUE)

include("${PSPDEV}/psp/share/CreatePBP.cmake")
include("${PSPDEV}/psp/share/AddPrxModule.cmake")
