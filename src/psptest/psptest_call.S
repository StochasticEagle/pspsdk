    .set noreorder
    .set nomips16
    .text
    .align 2

    .macro PSPTEST_GP_CALL name
    .globl \name
    .ent \name
\name:
    addiu $sp, $sp, -16
    sw $ra, 12($sp)
    sw $gp, 8($sp)
    move $gp, $a0
    move $t9, $a1
    move $a0, $a2
    jalr $t9
    nop
    lw $gp, 8($sp)
    lw $ra, 12($sp)
    addiu $sp, $sp, 16
    jr $ra
    nop
    .end \name
    .endm

    PSPTEST_GP_CALL psptest_call_test_with_gp
    PSPTEST_GP_CALL psptest_call_lifecycle_with_gp
