#!/usr/bin/env bash
set -euo pipefail

if [[ -z "${PSPDEV:-}" ]]; then
    echo "ERROR: PSPDEV is not set." >&2
    exit 1
fi

CMAKE_BIN="${1:-cmake}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
FIXTURE_INCLUDE="${PSPDEV}/psp/include/pspsdk-find-probe.h"
FIXTURE_LIBRARY="${PSPDEV}/psp/lib/libpspsdk-find-probe.a"
FIXTURE_CONFIG_DIR="${PSPDEV}/psp/lib/cmake/pspsdk-find-probe"
FIXTURE_CONFIG="${FIXTURE_CONFIG_DIR}/pspsdk-find-probe-config.cmake"
BUILD_DIR="${ROOT}/build-find-prefix-probe"

cleanup() {
    rm -f "${FIXTURE_INCLUDE}" "${FIXTURE_LIBRARY}" "${FIXTURE_CONFIG}"
    rmdir "${FIXTURE_CONFIG_DIR}" 2>/dev/null || true
    rm -rf "${BUILD_DIR}"
}
trap cleanup EXIT

mkdir -p "${PSPDEV}/psp/include" "${PSPDEV}/psp/lib" "${FIXTURE_CONFIG_DIR}"
printf '#define PSPSDK_FIND_PROBE 1\n' > "${FIXTURE_INCLUDE}"
: > "${FIXTURE_LIBRARY}"
printf 'set(PSPSDK_FIND_PROBE_CONFIG_FOUND TRUE)\n' > "${FIXTURE_CONFIG}"

rm -rf "${BUILD_DIR}"
"${CMAKE_BIN}" -S "${ROOT}/find-prefix-probe" -B "${BUILD_DIR}" \
    -DCMAKE_TOOLCHAIN_FILE="${PSPDEV}/psp/share/pspdev.cmake" \
    -DCMAKE_BUILD_TYPE=Release
