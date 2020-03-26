.include "src/features/macro.s"
asm_features_region:
random:
.word 0 // 存放rand函数的指针
// 植物被咬死时效果
whenPlantDiying:
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
// 返回指令
ret:
  bx lr
whenPlantDiying_end:
// 第六大关时场景换为月夜
adventureSixthLevelScene:
  mov r1, #0x5
  b ret
adventureSixthLevelScene_end:
// 金银币掉落有概率变成钻石
dropSeedsCard:
  cmp r2, #4
  blt coin
  b set
coin:
  callCFunction -0x70
  cmp r0, #1
  bne restoreR0
  restoreRegisters
  mov r2, #6
  b set
restoreR0:
  restoreRegisters
set:
  str r2, [r3, #100]
  b ret
dropSeedsCard_end:
// 僵尸一类饰品
zombieHat:
  str r3, [r2, #0xd4]
  ldr r3, [r2, #0x30]
  cmp r3, #6 // 铁门
  beq zombieHat1
  cmp r3, #1 // 旗帜
  beq zombieHat1
  cmp r3, #23 // 白眼巨人
  beq zombieSize
  b ret
zombieHat1:
  mov r3, #2
  str r3, [r2, #0xd0]
  mov r3, #1100
  str r3, [r2, #0xdc]
  b ret
zombieSize:
  callCFunction -0xd4
  cmp r0, #1
  bne end1
  restoreRegisters
  mov r3, #0x3F000000
  str r3, [r2, #0x128]
  b ret
end1:
  restoreRegisters
  b ret
zombieHat_end:
globalizeSymbol asm_features_region asm_features_region_end
globalizeSymbol whenPlantDiying whenPlantDiying_end
globalizeSymbol adventureSixthLevelScene adventureSixthLevelScene_end
globalizeSymbol dropSeedsCard dropSeedsCard_end
globalizeSymbol zombieHat zombieHat_end
asm_features_region_end:
