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

main:
  subu $sp, $sp, 112
  sw $ra, 108($sp)
  sw $fp, 104($sp)
  addi $fp, $sp, 112
  sw $s0, -52($fp)
  sw $s1, -56($fp)
  sw $s2, -60($fp)
  sw $s3, -64($fp)
  sw $s4, -68($fp)
  sw $s5, -72($fp)
  sw $s6, -76($fp)
  sw $s7, -80($fp)
  li $s0, 0
  li $s1, 1
  li $s2, 0
  jal read
  move $t0, $v0
  move $s3, $t0
  sw $t0, -100($fp)
  sw $s0, -84($fp)
  sw $s1, -88($fp)
  sw $s2, -92($fp)
  sw $s3, -96($fp)
label1:
  lw $s0, -92($fp)
  lw $s1, -96($fp)
  blt $s0, $s1, label2
  sw $s0, -92($fp)
  sw $s1, -96($fp)
  j label3
label2:
  lw $s0, -84($fp)
  lw $s1, -88($fp)
  add $t0, $s0, $s1
  move $s2, $t0
  move $a0, $s1
  jal write
  move $s0, $s1
  move $s1, $s2
  lw $s3, -92($fp)
  addi $t1, $s3, 1
  move $s3, $t1
  sw $t0, -104($fp)
  sw $t1, -112($fp)
  sw $s0, -84($fp)
  sw $s1, -88($fp)
  sw $s2, -108($fp)
  sw $s3, -92($fp)
  j label1
label3:
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
