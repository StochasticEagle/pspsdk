# VLF beta / Despertar del Cementerio lineage

This directory is the preservation and future reconstruction home for the pre-1.0 VLF generation used by the Despertar del Cementerio / M33-era tooling.

## Identity

This generation is intentionally named `vlf_beta` in the repository to distinguish it from the later official VLF 1.0 release. The historical module filename itself remains `vlf.prx`; binary filenames are not renamed.

The retained beta-generation module currently used by this repository is:

- `src/pre-built/vlf.prx`
- Git blob: `b76bb29a3882b226d33451a76609c9ec6435e2e0`
- Size: 408,746 bytes

That blob is identical to `modules/vlf.elf` in the preserved DC-M33 tree:

- Repository: https://github.com/balika011/DC-M33
- Historical path: `modules/vlf.elf`

The beta API header preserved here comes from:

- Repository: https://github.com/mathieulh/Despertar-Del-Cementerio
- Historical path: `include/vlf.h`

## ABI distinction

This beta generation predates the official VLF 1.0 API. In particular, the beta header represents VLF object handles as integer values. Dark_AleX's VLF 1.0 release notes state that the official release is incompatible with the beta and changes the Add-family APIs to return opaque object types.

For that reason `vlf_beta` and `vlf_1` must remain separate preservation targets even where implementation code later proves substantially related.

## Reconstruction status

Complete implementation source for this beta generation has not yet been recovered.

Future reconstruction should use the retained beta module, headers, consumers, and the VLF 1.0 libraries/modules as comparative evidence. Where original identifiers cannot be recovered, reconstructed code should use neutral abstract names for functions, types, classes/structures, and variables rather than inventing historical names.

A reconstructed implementation should not replace the retained binary until ABI, exports, behavior, and compatibility have been validated.
