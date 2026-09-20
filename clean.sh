#!/bin/bash

set -e

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${ROOT}"

# CFW additions use their own makefile and build products.
if [[ -f Makefile-cfw ]]; then
    make -f Makefile-cfw clean
fi

rm -rf "${ROOT}/build/prx"

# Remove the normal autotools build products and generated build system.
./bootstrap --clean
