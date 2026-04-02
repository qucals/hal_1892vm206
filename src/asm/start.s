  .section .text.startup, "ax", @progbits
  .globl _start
  .ent _start

_start:
  /* Запретить прерывания на старте */
  mfc0 $t0, $12
  li $t1, ~1
  and $t0, $t0, $t1
  mtc0 $t0, $12
  nop
  nop
  nop

  la $sp, __stack_top
  la $gp, _gp

  la $t0, __bss_start
  la $t1, __bss_end

1:
  bgeu $t0, $t1, 2f
  nop
  sw $zero, 0($t0)
  addiu $t0, $t0, 4
  b 1b
  nop

2:
  jal main
  nop

3:
  b 3b
  nop

  .end _start
