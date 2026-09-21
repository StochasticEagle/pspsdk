# VLF 1.0

This directory preserves the surviving official VLF 1.0 developer package lineage attributed to Dark_AleX.

Repository naming uses `vlf_1` to distinguish this generation from the earlier beta/DC lineage kept under `src/vlf_beta/`. Historical binary filenames such as `vlf.prx` and `vlf_without_png.prx` are preserved unchanged.

## Provenance

Imported from the VLF rehost:

- Repository: https://github.com/MichelMichels/psp-vlf
- Upstream branch: `main`
- Imported tree: commit `d088c11123e68efb1b78a740f12526c5fd25f892`
- Original project attribution: Dark_AleX

The upstream README is preserved as `UPSTREAM_README.md`.

The imported package includes:

- the VLF 1.0 API header;
- `libvlfgu.a`, `libvlfgui.a`, `libvlflibc.a`, and `libvlfutils.a`;
- `vlf.prx` and `vlf_without_png.prx`;
- the associated intraFont source/build tree;
- the login-screen sample source and its preserved binary package.

No submodule is used.

## Relationship to vlf_beta

VLF 1.0 is not treated as a drop-in replacement for the beta/DC generation. The public API changed incompatibly, including the transition from integer object handles to opaque VLF object types.

The two generations are therefore retained separately:

- `src/vlf_beta/` — beta/DC ABI and reconstruction target;
- `src/vlf_1/` — official VLF 1.0 package.

## Source-recovery status

The surviving VLF 1.0 package contains relocatable static archives and complete public headers, but the original VLF implementation source has not yet been recovered.

Those archives are valuable reconstruction references because they retain object-level structure, symbols, relocations, and code boundaries unavailable from a stripped monolithic module alone.

A future recovery effort may compare the beta module against VLF 1.0 at the function/object level to identify shared implementations. Reconstructed source should preserve known public API names and use neutral abstract identifiers where original private names, classes/structures, or variables cannot be recovered.

The retained binaries remain authoritative preservation artifacts until any reconstructed implementation is verified for ABI and behavioral compatibility.
