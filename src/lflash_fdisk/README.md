# lflash_fdisk

This directory preserves the recovered source for the legacy `lflash_fdisk.prx` module.

## Provenance

Imported from the historical Despertar del Cementerio source repository:

- Repository: https://github.com/mathieulh/Despertar-Del-Cementerio
- Upstream branch: `master`
- Original path: `lflash_fdisk/`
- Archival commit observed during recovery: `8cb49d9bebe40487cb52080a5f3cec0382c1bf8f`

Only source and build-description files are imported here. Historical generated artifacts such as `*.o`, `*.elf`, and the upstream-built PRX are intentionally not duplicated.

The currently retained known-good binary remains at:

`src/pre-built/lflash_fdisk.prx`

## Status

The historical Makefile and build support are preserved as recovered source. This module is not yet wired into the current CMake build and does not replace the retained PRX until a modern source build is verified.
