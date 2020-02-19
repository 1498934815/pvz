whenPlantDead:
  cmp r2, #50
  bge whenPlantDead_original_code
  ldr r2, [r3, #0x30]
  cmp r2, #3
  beq littlenut
  cmp r2, #23
  beq tallnut
  b whenPlantDead_original_code
littlenut:
  mov r2, #2
  str r2, [r3, #0x30]
  mov r2, #1
  str r2, [r3, #0x5c]
  b ret
tallnut:
  mov r2, #17
  str r2, [r3, #0x30]
ret:
  bx lr
whenPlantDead_original_code:
  str r2, [r3, #76]
  b ret
whenPlantDead_end:
.global whenPlantDead
.global whenPlantDead_end
