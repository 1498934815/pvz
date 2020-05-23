.include "src/features/macro.s"
DEFINE_FEATURE asm_features_region
DEFINE_EXTERNAL_FUNCTION whenZombieBorn // 存放whenZombieBorn的函数地址
DEFINE_EXTERNAL_FUNCTION whenPaperZombieBorn // 存放whenPaperZombieBorn的函数地址
DEFINE_EXTERNAL_FUNCTION whenZombieHitting // 存放whenZombieHitting的函数地址
DEFINE_EXTERNAL_FUNCTION whenZombiesListInit // 存放whenZombiesListInit的函数地址
DEFINE_EXTERNAL_FUNCTION whenPogoJump
DEFINE_EXTERNAL_FUNCTION whenPogoJumpOverTallnut
DEFINE_EXTERNAL_FUNCTION whenPogoPaperDroped
DEFINE_EXTERNAL_FUNCTION whenPoleVaultingJump
DEFINE_EXTERNAL_FUNCTION whenZombiePropertiesInit
DEFINE_EXTERNAL_FUNCTION judgeAdventureLevel
DEFINE_EXTERNAL_FUNCTION judgeAdventureScene
DEFINE_EXTERNAL_FUNCTION whenCardsSelectionInit
DEFINE_EXTERNAL_FUNCTION whenPlantHurtting
DEFINE_EXTERNAL_FUNCTION whenPlantShotting
DEFINE_EXTERNAL_FUNCTION oneOfThree // 存放1/3的rand函数指针
DEFINE_EXTERNAL_FUNCTION oneOfFifteen // 存放1/3的rand函数指针
DEFINE_EXTERNAL_FUNCTION oneOfOneHundred // 存放rand函数的指针
// 植物被咬死时效果
DEFINE_FEATURE whenPlantDiying
  str r2, [r3, #76]
  cmp r2, #10
  bge ret
  ldr r2, [r3, #0x30]
  cmp r2, #3
  beq littlenut
  cmp r2, #23
  beq tallnut
  cmp r2, #8
  beq littlemushroom
  cmp r2, #24 // 水兵菇
  beq littlemushroom
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
littlemushroom:
  callCFunction oneOfThree
  cmp r0, #1
  bne restoreR0
  restoreRegisters
  mov r2, #9
  str r2, [r3, #0x30]
  mov r2, #1
  str r2, [r3, #0x64]
  mov r2, #3000
  str r2, [r3, #0x68]
  b ret
restoreR0:
  restoreRegisters
ret:
  bx lr
tallnut1:
  callCFunction oneOfThree
  cmp r0, #1
  bne restoreR0
  restoreRegisters
  mov r2, #14
  str r2, [r3, #0x30]
  mov r2, #1
  str r2, [r3, #0x5c]
  b ret
DEFINE_FEATURE meanPlant
  push {r2}
  ldr r2, [r3, #0x30]
  cmp r2, #23
  beq tallnut1
  cmp r2, #8
  beq littlemushroom
  cmp r2, #3
  beq littlemushroom
  cmp r2, #24 // 水兵菇
  beq littlemushroom
  pop {r2}
  strb  r2, [r3, #334]
  b ret
// 第六大关时场景换为月夜
DEFINE_FEATURE adventureSixthLevel
  callCFunction judgeAdventureLevel
  restoreRegisters
  pop {lr}
  sub sp, r11, #0
  pop {r11}
  b ret
DEFINE_FEATURE adventureSixthLevelScene
  FUNCTIONCALLGUARD judgeAdventureScene
  FUNCTIONEXIT
// 金银币掉落有概率变成钻石
DEFINE_FEATURE dropSeedsCard
  cmp r2, #4
  blt coin
  b set
coin:
  callCFunction oneOfFifteen
  cmp r0, #1
  bne __restoreR0
  restoreRegisters
  mov r2, #6
  b set
__restoreR0:
  restoreRegisters
set:
  str r2, [r3, #100]
  b ret
DEFINE_FEATURE dropSeedsCard_end
// 僵尸一类饰品
DEFINE_FEATURE zombieHat
  FUNCTIONCALLGUARD whenZombieBorn
  FUNCTIONEXIT
DEFINE_FEATURE paperHP
  FUNCTIONCALLGUARD whenPaperZombieBorn // whenPaperZombieBorn
  FUNCTIONEXIT
DEFINE_FEATURE zombieHypno
  push {r1}
  str r2, [r3, #212] // 僵尸当前血量
  ldr	r1, [r11, #-52]
  callCFunction whenZombieHitting // whenZombieHitting
  restoreRegisters
  pop {r1}
  b ret
DEFINE_FEATURE zombieIce
  push {r1}
  str r2, [r3, #220] // 僵尸一级防具
  ldr	r1, [r11, #-44]
  callCFunction whenZombieHitting // whenZombieHitting
  restoreRegisters
  pop {r1}
  b ret
DEFINE_FEATURE replaceZombiesSeed
  FUNCTIONCALLGUARD whenZombiesListInit // whenZombiesListInit
  FUNCTIONEXIT
DEFINE_FEATURE pogoNewLine
  FUNCTIONCALLGUARD whenPogoJump // whenPogoJump
  FUNCTIONEXIT
DEFINE_FEATURE pogoJumpOverTallnut
  FUNCTIONCALLGUARD whenPogoJumpOverTallnut
  FUNCTIONEXIT
DEFINE_FEATURE pogoPaperDroped
  FUNCTIONCALLGUARD whenPogoPaperDroped
  FUNCTIONEXIT
DEFINE_FEATURE poleVaultingJumpTwice
  FUNCTIONCALLGUARD whenPoleVaultingJump
  FUNCTIONEXIT
DEFINE_FEATURE plantHurtting
  FUNCTIONCALLGUARD whenPlantHurtting
  FUNCTIONEXIT
DEFINE_FEATURE zombieEatting
  mov r3, #8
  b ret
DEFINE_FEATURE thiefQuickly
  mov r2, #1
  b ret
DEFINE_FEATURE zombiePropertiesInit
  FUNCTIONCALLGUARD whenZombiePropertiesInit
  mov r2, r0
  pop {r0, r1}
  add sp, #4
  pop {r3}
  str r2, [r3, #0x30]
  mov sp, fp
  pop {fp}
  str r2, [fp, #-232]
  pop {pc}
  // ldr	r2, [r11, #-232]
DEFINE_FEATURE randomProjectile
  FUNCTIONCALLGUARD whenPlantShotting
  FUNCTIONEXIT
  mov r2, #8
  str r2, [r3, #104]
  mov r2, #9
  str r2, [r3, #100]
  bx lr
DEFINE_FEATURE bringGrave
  mov r2, #1 // 墓碑
  bx lr
DEFINE_FEATURE noScaredMushroom
  mov r2, #0
  bx lr
DEFINE_FEATURE cardsSelectionInit
  str r4, [r3, #2004]
  FUNCTIONCALLGUARD whenCardsSelectionInit
  FUNCTIONEXIT
DEFINE_FEATURE alterGardensZombiesType
  mov r2, #1
  push {r1}
  ldr r1, [r3, #0x838]
  cmp r1, #0
  bne out
  ldr r1, [r3, #0x7c4]
  add r1, r3, #0x5600
  add r1, r1, #0xac
  ldr r1, [r1]
  cmp r1, #60
  blt out
  mov r2, #2
  out:
  pop {r1}
  bx lr
DEFINE_FEATURE_END asm_features_region_end
