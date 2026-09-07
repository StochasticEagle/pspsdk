#!/bin/bash

set -e

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

## Make sure PSPDEV is set.
if [ -z "${PSPDEV:-}" ]; then
    echo "ERROR: The PSPDEV environment variable has not been set."
    exit 1
fi

PROC_NR=$(getconf _NPROCESSORS_ONLN)

## Build PSPSDK.
cd "${ROOT}"
./bootstrap
./configure
make -j "$PROC_NR"

## Install PSPSDK.
make -j "$PROC_NR" install

## GCC needs to include libcglue, libpthreadglue, libpspprof,
#  libpsputility, libpsprtc, libpspnet_inet, libpspnet_resolver,
#  libpspsdk, libpspmodinfo, libpspuser, and libpspkernel from
#  PSPSDK to be able to build executables because they are part
#  of the standard libraries.

LIBDIR="${PSPDEV}/psp/lib"

ln -sf "../sdk/lib/libcglue.a" "${LIBDIR}/libcglue.a"
ln -sf "../sdk/lib/libpthreadglue.a" "${LIBDIR}/libpthreadglue.a"
ln -sf "../sdk/lib/libpspprof.a" "${LIBDIR}/libpspprof.a"
ln -sf "../sdk/lib/libpsputility.a" "${LIBDIR}/libpsputility.a"
ln -sf "../sdk/lib/libpsprtc.a" "${LIBDIR}/libpsprtc.a"
ln -sf "../sdk/lib/libpspnet_inet.a" "${LIBDIR}/libpspnet_inet.a"
ln -sf "../sdk/lib/libpspnet_resolver.a" "${LIBDIR}/libpspnet_resolver.a"
ln -sf "../sdk/lib/libpspsdk.a" "${LIBDIR}/libpspsdk.a"
ln -sf "../sdk/lib/libpspmodinfo.a" "${LIBDIR}/libpspmodinfo.a"
ln -sf "../sdk/lib/libpspuser.a" "${LIBDIR}/libpspuser.a"
ln -sf "../sdk/lib/libpspkernel.a" "${LIBDIR}/libpspkernel.a"

## Copy licenses.
mkdir -p "${PSPDEV}/psp/share/licenses/pspsdk"
cp "${ROOT}/LICENSE" "${PSPDEV}/psp/share/licenses/pspsdk/"

mkdir -p "${PSPDEV}/share/licenses/PrxEncrypter"
cp "${ROOT}/tools/PrxEncrypter/LICENSE" "${PSPDEV}/share/licenses/PrxEncrypter/"

## Store build information.
if git -C "${ROOT}" rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    BUILD_FILE="${PSPDEV}/build.txt"

    if [[ -f "${BUILD_FILE}" ]]; then
        sed -i'' '/^pspsdk /d' "${BUILD_FILE}"
    fi

    git -C "${ROOT}" log -1 --format="pspsdk %H %cs %s" >> "${BUILD_FILE}"
fi
