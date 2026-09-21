#!/bin/bash

set -e

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${ROOT}/install-permissions.sh"

if [ -z "${PSPDEV:-}" ]; then
    echo "ERROR: The PSPDEV environment variable has not been set."
    exit 1
fi

PROC_NR=$(getconf _NPROCESSORS_ONLN)
PRX_BUILD="${ROOT}/build/prx"

cd "${ROOT}"

## Re-run CMake in place so compiler/SDK/package changes refresh the cache
## while unchanged PRX objects remain available for incremental rebuilds.
make -f Makefile-cfw -j "${PROC_NR}" all
pspdev_run_install make -f Makefile-cfw install-files

# Build the in-tree PRX set. Source-built and intentional versioned pre-built
# PRXs are written directly into build/prx.
cmake -S "${ROOT}" -B "${PRX_BUILD}" \
    -DCMAKE_TOOLCHAIN_FILE="${PSPDEV}/psp/share/pspdev.cmake" \
    -DCMAKE_BUILD_TYPE=Release \
    -DPSPSDK_BUILD_CFW_LIBRARIES=OFF \
    -DPSPSDK_BUILD_DYNAMIC_MODULES=ON

cmake --build "${PRX_BUILD}" --parallel "${PROC_NR}"
