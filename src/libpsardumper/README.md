# libpsardumper

This directory preserves recovered source for the legacy `libpsardumper.prx` module.

## Provenance

Imported from a preserved NewPSARDumper tree:

- Repository: https://github.com/hz86/procfw-chn
- Upstream branch: `master`
- Original path: `contrib/newpsardumper/libpsardumper/`
- Related historical lineage: https://github.com/mathieulh/3.90-M33

The imported module includes its original Makefile, export definition, implementation, and `pspDecrypt.S` glue. The historical build references headers such as `libpsardumper.h` and `pspdecrypt.h`, which are already present in the integrated PSP CFW SDK headers in this repository.

The currently retained known-good binary remains at:

`src/pre-built/libpsardumper.prx`

## Status

The recovered source is preserved in-tree but is not yet wired into the current CMake build. The retained PRX remains the active binary until a modern source build is verified.
