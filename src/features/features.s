.include "src/features/macro.s"
DEFINE_FEATURE asm_features_region
DEFINE_EXTERNAL_FUNCTION CWhenZombieBorn // 存放whenZombieBorn的函数地址
DEFINE_EXTERNAL_FUNCTION whenPaperZombieBorn // 存放whenPaperZombieBorn的函数地址
DEFINE_EXTERNAL_FUNCTION CWhenZombieHitting// 存放whenZombieHitting的函数地址
DEFINE_EXTERNAL_FUNCTION CWhenZombiesListInit // 存放CWhenZombiesListInit的函数地址
DEFINE_EXTERNAL_FUNCTION whenPogoJump
DEFINE_EXTERNAL_FUNCTION whenPogoJumpOverTallnut
DEFINE_EXTERNAL_FUNCTION whenPogoPaperDroped
DEFINE_EXTERNAL_FUNCTION whenPoleVaultingJump
DEFINE_EXTERNAL_FUNCTION whenZombiePropertiesInit
DEFINE_EXTERNAL_FUNCTION detectZombiesVelocity
DEFINE_EXTERNAL_FUNCTION judgeAdventureLevel
DEFINE_EXTERNAL_FUNCTION judgeAdventureScene
DEFINE_EXTERNAL_FUNCTION CJudgeNewLevelFreshCountdown
DEFINE_EXTERNAL_FUNCTION CWhenNewWave
DEFINE_EXTERNAL_FUNCTION CWhenGameStart
DEFINE_EXTERNAL_FUNCTION CWhenGameEnd
DEFINE_EXTERNAL_FUNCTION whenCardsSelectionInit
DEFINE_EXTERNAL_FUNCTION CWhenPlantDiying
DEFINE_EXTERNAL_FUNCTION CWhenZombieMeaningPlant
DEFINE_EXTERNAL_FUNCTION CWhenPlantHurtting
DEFINE_EXTERNAL_FUNCTION CWhenPlantShotProjectile
DEFINE_EXTERNAL_FUNCTION CWhenGraveBusterEaten
DEFINE_EXTERNAL_FUNCTION oneOfThree // 存放1/3的rand函数指针
DEFINE_EXTERNAL_FUNCTION oneOfFifteen // 存放1/3的rand函数指针
DEFINE_EXTERNAL_FUNCTION oneOfOneHundred // 存放rand函数的指针
// 植物被咬死时效果
DEFINE_FEATURE whenPlantDiying
  FUNCTIONCALLGUARD CWhenPlantDiying
  FUNCTIONEXIT
restoreR0:
  restoreRegisters
ret:
  bx lr
DEFINE_FEATURE whenZombieMeaningPlant
  FUNCTIONCALLGUARD CWhenZombieMeaningPlant
  FUNCTIONEXIT
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
DEFINE_FEATURE whenZombieBorn
  FUNCTIONCALLGUARD CWhenZombieBorn
  FUNCTIONEXIT
DEFINE_FEATURE paperHP
  FUNCTIONCALLGUARD whenPaperZombieBorn // whenPaperZombieBorn
  FUNCTIONEXIT
DEFINE_FEATURE whenZombieHitting
  push {r0, r1}
  str r2, [r3, #212] // 僵尸当前血量
  ldr	r1, [fp, #-52]
  callCFunction CWhenZombieHitting // whenZombieHitting
  restoreRegisters
  pop {r0, r1}
  b ret
DEFINE_FEATURE whenZombieHittingArmor1
  push {r0, r1}
  str r2, [r3, #220] // 僵尸一级防具
  ldr	r1, [fp, #-44]
  callCFunction CWhenZombieHitting // CWhenZombieHitting
  restoreRegisters
  pop {r0, r1}
  b ret
DEFINE_FEATURE whenZombiesListInit
  FUNCTIONCALLGUARD CWhenZombiesListInit // CWhenZombiesListInit
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
DEFINE_FEATURE whenPlantHurtting
  mov r0, #15
  FUNCTIONCALLGUARD CWhenPlantHurtting
  FUNCTIONEXIT
DEFINE_FEATURE zombieEatting
  ldr r3, [sp, #0xc] // 僵尸地址,3c是上下文中得出的
  ldr r3, [r3, #0x30] // 僵尸代码
  cmp r3, #5 // 报纸
  bne 0x4
  mov r3, #12 // 每次12点
  b ret
  mov r3, r0
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
DEFINE_FEATURE whenPlantShotProjectile
  FUNCTIONCALLGUARD CWhenPlantShotProjectile
  FUNCTIONEXIT
  // 仿香蒲,无用
  // mov r2, #8
  // str r2, [r3, #104]
  // mov r2, #1
  // str r2, [r3, #100]
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
// 出怪判定，花园
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
  // 7-1到8-10都是一路水路
  cmp r1, #60
  ble out
  cmp r1, #80
  bgt out
  mov r2, #2
  out:
  pop {r1}
  bx lr
// VELOCITY:.word 0x3fb33333 // 1.3
//
// VELOCITY0:.word 0x3d4ccccd // 1.3
// DEFINE_FEATURE alterZombiesVelocity
//   FUNCTIONCALLGUARD detectZombiesVelocity
//   cmp r0, #1
//   bne _1out
//   // vmov.f32 s14, #1.5
//   // vldr.f32 s13, VELOCITY
//   // vmul.f32 s15, s15, s13
//   vmov.f32 s15, #1.5
//   b _2out
//   _1out:
//   vldr  s15, [r3, #296]
//   _2out:
//   restoreRegisters1
//   // 需使用上层函数的fp
//   // UGLY CODE
//   mov sp, fp
//   pop {fp}
//   pop {pc}
DEFINE_FEATURE alterZombiesVelocity
  vldr s15, [r3, #64]
  FUNCTIONCALLGUARD detectZombiesVelocity
  // vmov.f32 s14, #1.5
  // vldr.f32 s13, VELOCITY
  // vmul.f32 s15, s15, s13
  vmov s14, r0
  vmul.f32 s15, s14
  restoreRegisters1
  vstr s15, [r3, #64]
  pop {fp, pc}
DEFINE_FEATURE judgeNewLevelFreshCountdown
  FUNCTIONCALLGUARD CJudgeNewLevelFreshCountdown
  FUNCTIONEXIT
DEFINE_FEATURE whenNewWave
  str r1, [r2, r3]
  FUNCTIONCALLGUARD CWhenNewWave
  FUNCTIONEXIT
DEFINE_FEATURE whenBloverBlowed
  mov r1, #0
  ldr r3, [r11, #-16]
  FUNCTIONCALLGUARD CWhenPlantHurtting
  restoreRegisters1
  mov r3, #0
  mov sp, fp
  pop {fp, pc}
DEFINE_FEATURE whenGameStart
  str	r2, [r3, #1992]	// 0x7c8
  FUNCTIONCALLGUARD CWhenGameStart
  FUNCTIONEXIT
DEFINE_FEATURE whenGameEnd
  str	r2, [r3, #1992]	// 0x7c8
  FUNCTIONCALLGUARD CWhenGameEnd
  FUNCTIONEXIT
DEFINE_FEATURE whenGraveBusterEaten
  str r2, [r3, #96]
  FUNCTIONCALLGUARD CWhenGraveBusterEaten
  FUNCTIONEXIT
DEFINE_FEATURE_END asm_features_region_end
