# PSPTEST

PSPTEST is PSPSDK's runtime verification framework. It is separate from `src/samples`: samples teach programmers how to use APIs, while PSPTEST verifies API behavior on PSP hardware.

PSPSDK installs `psptest.h` and `libpsptest.a`. Test modules live below this directory and use a `Makefile.test` so they can be built independently into an `EBOOT.PBP`.

## Result format

Each test suite writes a line-oriented result file. The default path is `psptest-results.log`; a runner can override it with `--psptest-output=<path>`.

A launcher can pass `--psptest-return=<path-to-launcher-EBOOT.PBP>`. After the suite writes its result, PSPTEST executes that EBOOT with `sceKernelLoadExec()` and passes `--psptest-result=<result-path>` back to the launcher. The framework does not make build-time assumptions about hardware availability: tests are run, and unsupported hardware or runtime API failures are represented by the test result.

The stable records are:

```text
PSPTEST<TAB>1
SUITE<TAB>suite-name
CASE<TAB>PASS|FAIL|SKIP|INTERACTIVE_PASS|INTERACTIVE_FAIL<TAB>case-name<TAB>assertions=N[<TAB>message=...]
SUMMARY<TAB>pass=N<TAB>fail=N<TAB>skip=N<TAB>total=N
RETURN<TAB>FAIL<TAB>code=N
```

## Coverage markers

Place `PSPTEST_COVERS(function_name);` at file scope for each public API exercised by a test module. The macro emits the function name into the `.psptest_coverage` ELF section so host tooling can later compare declared test coverage against exported/public APIs.

## Adding a PSPSDK test module

Create `psptest/<module>/Makefile.test` and source files. Link with `-lpsptest`; the framework target in this directory is the minimal reference implementation.

Run all currently implemented modules with:

```bash
make -C psptest
```

Interactive hardware tests must explicitly record their outcome with `psptest_interactive_result()`. If an interactive case returns without recording a result, PSPTEST records it as `SKIP`.
