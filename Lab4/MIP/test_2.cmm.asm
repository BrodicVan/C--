.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

fact:
  subu $sp, $sp, 96
  sw $ra, 92($sp)
  sw $fp, 88($sp)
  addi $fp, $sp, 96
  sw $s0, -52($fp)
  sw $s1, -56($fp)
  sw $s2, -60($fp)
  sw $s3, -64($fp)
  sw $s4, -68($fp)
  sw $s5, -72($fp)
  sw $s6, -76($fp)
  sw $s7, -80($fp)
  sw $a0, -84($fp)
  lw $s0, -84($fp)
  li $t9, 1
  beq $s0, $t9, label1
  sw $s0, -84($fp)
  j label2
label1:
  lw $s0, -84($fp)
  move $v0, $s0
  lw $s0, -52($fp)
  lw $s1, -56($fp)
  lw $s2, -60($fp)
  lw $s3, -64($fp)
  lw $s4, -68($fp)
  lw $s5, -72($fp)
  lw $s6, -76($fp)
  lw $s7, -80($fp)
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
  sw $s0, -84($fp)
  j label3
label2:
  lw $s0, -84($fp)
  addi $t0, $s0, -1
  sw $t0, -12($fp)
  sw $t1, -16($fp)
  sw $t2, -20($fp)
  sw $t3, -24($fp)
  sw $t4, -28($fp)
  sw $t5, -32($fp)
  sw $t6, -36($fp)
  sw $t7, -40($fp)
  sw $t8, -44($fp)
  sw $t9, -48($fp)
  move $a0, $t0
  jal fact
  lw $t0, -12($fp)
  lw $t1, -16($fp)
  lw $t2, -20($fp)
  lw $t3, -24($fp)
  lw $t4, -28($fp)
  lw $t5, -32($fp)
  lw $t6, -36($fp)
  lw $t7, -40($fp)
  lw $t8, -44($fp)
  lw $t9, -48($fp)
  move $t1, $v0
  mul $t2, $s0, $t1
  move $v0, $t2
  lw $s0, -52($fp)
  lw $s1, -56($fp)
  lw $s2, -60($fp)
  lw $s3, -64($fp)
  lw $s4, -68($fp)
  lw $s5, -72($fp)
  lw $s6, -76($fp)
  lw $s7, -80($fp)
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
  sw $t0, -88($fp)
  sw $t1, -96($fp)
  sw $t2, -92($fp)
  sw $s0, -84($fp)
label3:

main:
  subu $sp, $sp, 96
  sw $ra, 92($sp)
  sw $fp, 88($sp)
  addi $fp, $sp, 96
  sw $s0, -52($fp)
  sw $s1, -56($fp)
  sw $s2, -60($fp)
  sw $s3, -64($fp)
  sw $s4, -68($fp)
  sw $s5, -72($fp)
  sw $s6, -76($fp)
  sw $s7, -80($fp)
  jal read
  move $t0, $v0
  move $s0, $t0
  sw $t0, -88($fp)
  sw $s0, -84($fp)
  lw $s1, -84($fp)
  li $t9, 1
  bgt $s1, $t9, label4
  sw $t0, -88($fp)
  sw $s0, -84($fp)
  sw $s1, -84($fp)
  j label5
label4:
  sw $t0, -12($fp)
  sw $t1, -16($fp)
  sw $t2, -20($fp)
  sw $t3, -24($fp)
  sw $t4, -28($fp)
  sw $t5, -32($fp)
  sw $t6, -36($fp)
  sw $t7, -40($fp)
  sw $t8, -44($fp)
  sw $t9, -48($fp)
  lw $s0, -84($fp)
  move $a0, $s0
  jal fact
  lw $t0, -12($fp)
  lw $t1, -16($fp)
  lw $t2, -20($fp)
  lw $t3, -24($fp)
  lw $t4, -28($fp)
  lw $t5, -32($fp)
  lw $t6, -36($fp)
  lw $t7, -40($fp)
  lw $t8, -44($fp)
  lw $t9, -48($fp)
  move $t0, $v0
  move $s1, $t0
  sw $t0, -96($fp)
  sw $s0, -84($fp)
  sw $s1, -92($fp)
  j label6
label5:
  li $s0, 1
  sw $s0, -92($fp)
label6:
  lw $s0, -92($fp)
  move $a0, $s0
  jal write
  li $t8, 0
  move $v0, $t8
  lw $s0, -52($fp)
  lw $s1, -56($fp)
  lw $s2, -60($fp)
  lw $s3, -64($fp)
  lw $s4, -68($fp)
  lw $s5, -72($fp)
  lw $s6, -76($fp)
  lw $s7, -80($fp)
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
