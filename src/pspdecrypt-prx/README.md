# pspdecrypt PRX

This directory preserves recovered source for the legacy `pspdecrypt.prx` module.

The directory is named `pspdecrypt-prx` to distinguish the implementation from the existing `src/pspDecrypt/` SDK import-stub library. The stub library is not the implementation of the historical PRX.

## Provenance

Imported from a preserved NewPSARDumper tree:

- Repository: https://github.com/hz86/procfw-chn
- Upstream branch: `master`
- Original path: `contrib/newpsardumper/pspdecrypt/`
- Related historical lineage: https://github.com/mathieulh/3.90-M33

The imported module includes its original Makefile, implementation, and export definition. The historical build references headers and kernel libraries that are part of the integrated PSP CFW SDK/toolchain environment in this repository.

The currently retained known-good binary remains at:

`src/pre-built/pspdecrypt.prx`

## Status

The recovered source is preserved in-tree but is not yet wired into the current CMake build. The retained PRX remains the active binary until a modern source build is verified.
