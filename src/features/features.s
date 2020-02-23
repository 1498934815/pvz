whenPlantDead:
  str r2, [r3, #76]
  cmp r2, #50
  bge ret
  ldr r2, [r3, #0x30]
  cmp r2, #3
  beq littlenut
  cmp r2, #23
  beq tallnut
  b ret
littlenut:
  mov r2, #2
  str r2, [r3, #0x30]
  mov r2, #1
  str r2, [r3, #0x5c]
  b ret
tallnut:
  mov r2, #17
  str r2, [r3, #0x30]
  b ret
ret:
  bx lr
whenPlantDead_end:
.global whenPlantDead
.global whenPlantDead_end
