#!/bin/bash

set -e

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${ROOT}/install-permissions.sh"

## Make sure PSPDEV is set.
if [ -z "${PSPDEV:-}" ]; then
    echo "ERROR: The PSPDEV environment variable has not been set."
    exit 1
fi

PROC_NR=$(getconf _NPROCESSORS_ONLN)

## Keep the PSPSDK build incremental. Regenerate configure only when its
## Autoconf inputs changed, then rerun configure in place so changes to the
## installed PSP toolchain/sysroot are picked up without discarding objects.
cd "${ROOT}"

needs_bootstrap=0
if [[ ! -x "${ROOT}/configure" ]] ||
   [[ "${ROOT}/configure.ac" -nt "${ROOT}/configure" ]]; then
    needs_bootstrap=1
elif find "${ROOT}/m4" -type f -name '*.m4' -newer "${ROOT}/configure" \
       -print -quit | grep -q .; then
    needs_bootstrap=1
elif find "${ROOT}" \
       -path "${ROOT}/.git" -prune -o \
       -path "${ROOT}/components" -prune -o \
       -type f -name Makefile.am -newer "${ROOT}/configure" \
       -print -quit | grep -q .; then
    needs_bootstrap=1
fi

if (( needs_bootstrap )); then
    ./bootstrap
fi

./configure
make -j "$PROC_NR"

## Install PSPSDK.
pspdev_run_install make -j "$PROC_NR" install

## GCC needs to include libcglue, libpthreadglue, libpspprof,
#  libpsputility, libpsprtc, libpspnet_inet, libpspnet_resolver,
#  libpspsdk, libpspmodinfo, libpspuser, and libpspkernel from
#  PSPSDK to be able to build executables because they are part
#  of the standard libraries.

LIBDIR="${PSPDEV}/psp/lib"

pspdev_run_install ln -sf "../sdk/lib/libcglue.a" "${LIBDIR}/libcglue.a"
pspdev_run_install ln -sf "../sdk/lib/libpthreadglue.a" "${LIBDIR}/libpthreadglue.a"
pspdev_run_install ln -sf "../sdk/lib/libpspprof.a" "${LIBDIR}/libpspprof.a"
pspdev_run_install ln -sf "../sdk/lib/libpsputility.a" "${LIBDIR}/libpsputility.a"
pspdev_run_install ln -sf "../sdk/lib/libpsprtc.a" "${LIBDIR}/libpsprtc.a"
pspdev_run_install ln -sf "../sdk/lib/libpspnet_inet.a" "${LIBDIR}/libpspnet_inet.a"
pspdev_run_install ln -sf "../sdk/lib/libpspnet_resolver.a" "${LIBDIR}/libpspnet_resolver.a"
pspdev_run_install ln -sf "../sdk/lib/libpspsdk.a" "${LIBDIR}/libpspsdk.a"
pspdev_run_install ln -sf "../sdk/lib/libpspmodinfo.a" "${LIBDIR}/libpspmodinfo.a"
pspdev_run_install ln -sf "../sdk/lib/libpspuser.a" "${LIBDIR}/libpspuser.a"
pspdev_run_install ln -sf "../sdk/lib/libpspkernel.a" "${LIBDIR}/libpspkernel.a"

## Validate the installed PSP toolchain integration before package builds.
## These probes catch C++ include-next ordering regressions and missing ASM
## PSPSDK include paths in both CMake and the legacy make fragments.
SMOKE_SRC="${ROOT}/tests/toolchain-smoke"
SMOKE_CMAKE_BUILD="${ROOT}/build/toolchain-smoke-cmake"
SMOKE_MAKE_BUILD="${ROOT}/build/toolchain-smoke-make"

cmake -S "${SMOKE_SRC}" -B "${SMOKE_CMAKE_BUILD}" \
    -DCMAKE_TOOLCHAIN_FILE="${PSPDEV}/psp/share/pspdev.cmake" \
    -DCMAKE_BUILD_TYPE=Release
cmake --build "${SMOKE_CMAKE_BUILD}" --parallel "${PROC_NR}"

make -C "${SMOKE_SRC}" probe \
    PSP_BUILD_FRAGMENT=build.mak \
    BUILD_DIR="${SMOKE_MAKE_BUILD}" \
    PROBE_NAME=build
make -C "${SMOKE_SRC}" probe \
    PSP_BUILD_FRAGMENT=build_prx.mak \
    BUILD_DIR="${SMOKE_MAKE_BUILD}" \
    PROBE_NAME=build_prx

## Copy licenses.
pspdev_run_install mkdir -p "${PSPDEV}/psp/share/licenses/pspsdk"
pspdev_run_install cp "${ROOT}/LICENSE" "${PSPDEV}/psp/share/licenses/pspsdk/"

pspdev_run_install mkdir -p "${PSPDEV}/share/licenses/PrxEncrypter"
pspdev_run_install cp "${ROOT}/tools/PrxEncrypter/LICENSE" "${PSPDEV}/share/licenses/PrxEncrypter/"

## Store build information.
if git -C "${ROOT}" rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    pspdev_record_build_info "pspsdk" "$(git -C "${ROOT}" log -1 --format="pspsdk %H %cs %s")"
fi
