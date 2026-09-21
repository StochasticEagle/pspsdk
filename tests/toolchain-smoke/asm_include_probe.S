#include "pspstub.s"

.text
.set noreorder
.globl pspsdk_asm_include_probe
.ent pspsdk_asm_include_probe
pspsdk_asm_include_probe:
    jr $ra
    nop
.end pspsdk_asm_include_probe
