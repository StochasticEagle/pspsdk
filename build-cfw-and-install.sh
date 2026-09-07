#!/bin/bash

set -e

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [ -z "${PSPDEV:-}" ]; then
    echo "ERROR: The PSPDEV environment variable has not been set."
    exit 1
fi

PROC_NR=$(getconf _NPROCESSORS_ONLN)

cd "${ROOT}"
make -f Makefile-cfw -j "$PROC_NR" install
