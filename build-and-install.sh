#!/bin/bash

set -e

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${ROOT}/install-permissions.sh"
pspdev_require_unprivileged_build

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

## Copy licenses.
pspdev_run_install mkdir -p "${PSPDEV}/psp/share/licenses/pspsdk"
pspdev_run_install cp "${ROOT}/LICENSE" "${PSPDEV}/psp/share/licenses/pspsdk/"

pspdev_run_install mkdir -p "${PSPDEV}/share/licenses/PrxEncrypter"
pspdev_run_install cp "${ROOT}/tools/PrxEncrypter/LICENSE" "${PSPDEV}/share/licenses/PrxEncrypter/"

## Store build information.
if git -C "${ROOT}" rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    pspdev_record_build_info "pspsdk" "$(git -C "${ROOT}" log -1 --format="pspsdk %H %cs %s")"
fi
