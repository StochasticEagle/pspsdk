# PSPTEST GCC 16.2 core SDK modules

Tests live in PSPSDK and execute in PSPDEV's PSPTEST runner as separate PRXs.
Each suite uses deterministic assertions; no human input or privilege escalation.

| Module | Coverage |
|---|---|
| compiler-abi | O32 type layout, integer and 64-bit helpers, Allegrex instructions, atomics |
| libc-runtime | byte/string functions, allocation, sort/search, math library |
| kernel-sync | semaphores, event flags, threads, monotonic clock |
| system-memory | memory accounting, partition allocations and release |
| file-io | removable-storage file read/write/seek/delete; writes only in PSPTEST results |
| device-services | read-only display, controller, and RTC sampling |

Runtime success is established only when result logs from real PSP execution
contain passing cases. Building the PRXs is a separate compile/link gate.

This list is not exhaustive PSPSDK coverage. The PSPDEV coverage report tracks
remaining untested exported SDK functions and prevents confusing compilation
with runtime correctness.
