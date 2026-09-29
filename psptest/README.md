# PSPTEST

PSPTEST is PSPSDK's runtime verification framework. PSPSDK owns the framework source, headers, library source, and PSPSDK-specific test-module sources under this directory.

Generated test artifacts are not written into the PSPSDK source tree. PSPDEV owns the integrated PSPTEST build: its stage 6 discovers PSPSDK and psp-packages test modules, compiles them from their checked-out source locations, and writes generated files only below PSPDEV's ignored `build/` directory.

PSPSDK installs `psptest.h` and `libpsptest.a` for normal SDK consumers. PSPDEV stage 6 additionally builds the framework directly from the checked-out PSPSDK source so a targeted PSPTEST build cannot accidentally mix a newer test module with an older installed PSPTEST header/library.

## Module contract

Each test module lives in `psptest/<module>/` and contains a `Makefile.test`. A runtime module:

- builds as one user PRX;
- links with `-lpsptest`;
- uses `PSPTEST_MODULE(...)` or `PSPTEST_MODULE_WITH_HEAP(...)`;
- writes its result through the control block supplied by the persistent PSPDEV launcher;
- must not emit generated files into its source directory.

`make -C psptest list` lists the implemented PSPSDK test modules. The integrated program is built from PSPDEV with:

```bash
./build.sh 6
./build.sh p 6
```

## Result format

Each suite writes a line-oriented result file:

```text
PSPTEST<TAB>1
SUITE<TAB>suite-name
CASE<TAB>PASS|FAIL|SKIP|INTERACTIVE_PASS|INTERACTIVE_FAIL<TAB>case-name<TAB>assertions=N[<TAB>message=...]
SUMMARY<TAB>pass=N<TAB>fail=N<TAB>skip=N<TAB>total=N
```

The persistent launcher loads one test PRX at a time, supplies a `PspTestModuleControl`, waits for completion, then stops and unloads that module.

## Coverage markers

Place `PSPTEST_COVERS(function_name);` at file scope for each public API exercised by a test module. The macro emits the function name into the `.psptest_coverage` ELF section so host tooling can compare declared test coverage against public APIs.

Interactive hardware tests must explicitly record their outcome with `psptest_interactive_result()`. If an interactive case returns without recording a result, PSPTEST records it as `SKIP`.
