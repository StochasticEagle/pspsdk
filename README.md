# PSP Software Development Kit

[![CI](https://img.shields.io/github/actions/workflow/status/StochasticEagle/pspsdk/.github/workflows/compilation.yml?branch=dev%2Ffork&style=for-the-badge&logo=github&label=CI)](https://github.com/StochasticEagle/pspsdk/actions/workflows/compilation.yml)

[StochasticEagle/pspdev](https://github.com/StochasticEagle/pspdev/tree/dev/fork)

## Introduction

PSPSDK is a collection of open source libraries and tools written for Sony's Playstation Portable (PSP) gaming console. It is part of the [PSPDEV SDK](https://github.com/StochasticEagle/pspdev/tree/dev/fork).

## Features

### PSPSDK provides a full set of libraries for creating PSP software:

* Stub libraries and headers for interfacing with the PSP operating system,
  ranging from threading libraries, file io, display driver and wifi networking.
* Basic runtime support (crt0) for executables and libraries.
* A libcglue library for fulfill newlib system call requirements.
* Support code for linking with the full Standard C Library provided with the
  PSPDEV toolchain.
* An implementation of the libGU graphics library. libGU provides an interface
  to the 2D and 3D hardware acceleration features found in the PSP's Graphic
  Engine.
* An implementation of the libGUM library. libGUM provides an interface for
  manipulating matrices for use in 3D software.
* A simple audio library that can be used to play back PCM audio streams.
* Support for building static executables and PRX files (relocatable modules).

### PSPSDK also includes several tools to assist in building PSP software:

* `bin2c`, `bin2o`, and `bin2s` for converting binary files into C source, object
  files, and assembler source files, respectively.
* `mksfo` and `mksfoex` for creating PARAM.SFO files.
* `pack-pbp` and `unpack-pbp` for adding and removing files from EBOOT.PBP.
* `psp-config` for locating PSPDEV tools and libraries.
* `psp-prxgen` for converting specially made ELFs to PRX files.
* `psp-build-exports` for creating export tables
* `psp-fixup-imports` for fixing up import tables post-linking to remove unused
  functions from the executable.

Documentation for the libraries are also provided, and can be found in the
`doc/` directory of the PSPSDK source and binary distributions.

A library for Make (`build.mak`) is also included to provide an easy way to build
simple programs and libraries. See any PSPSDK sample program for details on how
`build.mak` is used.

### Integrated CFW PRX modules

The dynamic PRX modules formerly maintained in
`StochasticEagle/psp-dynamic-libraries` are fully integrated into PSPSDK so
they share the same SDK interfaces and build lifecycle. The source-built PRX
modules are:

- `IOPrivileged`
- `IPL_Updater`
- `KBooti_Updater`
- `LibPNG`
- `PSPAV`
- `PSPFTP`
- `PSPIdentHelper`
- `USBDeviceDriver`
- `Unarchiver`
- `idStorageRegen`

`build-cfw-and-install.sh` builds the complete in-tree dynamic PRX set and
stages the resulting PRXs directly under `build/prx/`.

Five legacy PRX-only modules do not currently have complete, verified
source-and-build recipes and remain intentional versioned binary inputs under
`src/pre-built/`:

- `intraFont-vlf.prx`
- `lflash_fdisk.prx`
- `libpsardumper.prx`
- `pspdecrypt.prx`
- `vlf.prx`

These PRX-only inputs are explicitly enumerated by the root CMake build and are
staged alongside the source-built PRXs in `build/prx/`.

The migrated module source originates from
`StochasticEagle/psp-dynamic-libraries` and remains GPLv3-licensed; see
`LICENSE.GPLv3`.

## Installation

See [StochasticEagle/pspdev](https://github.com/StochasticEagle/pspdev/tree/dev/fork) for the complete forked PSPDEV environment.

## Installation from source

### Requirements

To use PSPSDK you must have the following software installed:

* [PSPDEV Toolchain](https://github.com/StochasticEagle/psp-toolchain-allegrex/tree/dev/fork)
* [GNU Make](http://www.gnu.org/software/make/)
* [Git client](https://git-scm.com/downloads)
* [GNU autoconf](http://www.gnu.org/software/autoconf/) and [automake](http://sourceware.org/automake/)(GNU Autotools)
* [Zlib development libraries and headers](https://www.zlib.net/)

The following packages are not required to build PSPSDK, but are used to build
documentation:

* [Doxygen](http://doxygen.nl/)
* [Graphviz](http://www.graphviz.org/)

### Building

PSPSDK can be found in the Git repository located at
https://github.com/StochasticEagle/pspsdk. You can do the following command to download this fork:

```bash
git clone --branch dev/fork https://github.com/StochasticEagle/pspsdk.git
```

Once you've downloaded PSPSDK, run the following command from the pspsdk directory to
create the configure script and support files (you must have `autoconf` and
`automake` installed):

```bash
./bootstrap
```

PSPSDK uses the GNU autotools (`autoconf` and `automake`) for its build system. To
install PSPSDK, run the following commands:

```bash
./configure
make
make doxygen-doc
make install
```

> [!NOTE]
> If you haven't installed Doxygen or don't want to build the library documentation, you can skip the `make doxygen-doc` command.

> [!TIP]
> You can use `build-and-install.sh` script for convenience.

## Notes

* This is a BETA release of PSPSDK. Some of the features and tools described
  here may not be fully implemented.

* By default PSPSDK will install into the directory where the PSPDEV toolchain
  is installed. If you decide to install PSPSDK somewhere else then you must
  define a PSPSDK environment variable that points to your alternate directory.
  The psp-config build utility will look for PSPSDK in the location specified in
  the PSPSDK environment variable first, or use its own location to determine
  where PSPSDK is installed.

* The Makefile templates provided by the sample code are designed for building a
  single executable or a library, but not both. If you plan on using these
  templates in your project to build both libraries and executables be aware
  that you will have to structure your project so that each library and
  executable are built in a seperate directory.

## Bugs

If you find a bug in PSPSDK, open an issue at https://github.com/StochasticEagle/pspsdk/issues. If possible, include any
code or documentation that can be used by the PSPSDK developers to recreate the
bug.

## License

PSPSDK is a mixed-license repository. Most of the original PSPSDK source is
distributed under the repository's [BSD-compatible license](https://github.com/StochasticEagle/pspsdk/blob/dev/fork/LICENSE).
Some incorporated components retain different licenses:

- `tools/PrxEncrypter/` is licensed under the GNU General Public License
  version 3.
- CFW PRX source migrated from `StochasticEagle/psp-dynamic-libraries`
  retains its GNU General Public License version 3 terms; see
  [`LICENSE.GPLv3`](https://github.com/StochasticEagle/pspsdk/blob/dev/fork/LICENSE.GPLv3).
- `src/BootLoadEx/` contains its own GNU General Public License version 3
  license file.
- `src/LibPspExploit/` is distributed under the WTFPL version 2 license in
  that directory.
- `src/iplsdk/` is distributed under the MIT license in that directory.
- Individual imported files may carry additional third-party or public-domain
  notices. Those notices remain applicable to those files and must be preserved
  when the source is moved or modified. This includes the AES source used by
  `idStorageRegen`, which carries OpenSSL and public-domain notices.

The license file or notice closest to a component or source file governs that
material where it differs from the repository-level BSD-compatible license.

## Resources

### Official Source Documentation

This is generated automatically from the repository `dev/fork` branch:
https://stochasticeagle.github.io/pspsdk/

### Additional Documentation

Here are links to additional community made documentation for contributors to the PSPSDK, mostly on the PSP hardware:

 - [PSP Allegrex documentation](https://pspdev.github.io/vfpu-docs/):
   Non-official documentation on the PSP CPU and VFPU.
 - [Unofficial PSP docs](https://uofw.github.io/upspd/docs/): A collection of docs
   from different authors and sources, covering more specific topics.
 - [Yet Another PSP Documentation](http://hitmen.c02.at/files/yapspd/): A very
   detailed hardware documentation, including software interfaces.

### Discord

You can find PSPDEV Maintainers over at https://discord.gg/bePrj9W in the `#psp-toolchain` channel :)

## Thanks

The PSPSDK developers wish to thank all the people who have contributed bug
fixes, ideas and support for the project. Also big thanks to nem for kicking off
PSP development with all his work, the original imports system is based on his
work in the hello world demo.
