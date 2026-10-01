    .set noreorder
    .set nomips16
    .text
    .align 2
    .globl psptest_call_with_gp
    .ent psptest_call_with_gp
psptest_call_with_gp:
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
    .end psptest_call_with_gp
