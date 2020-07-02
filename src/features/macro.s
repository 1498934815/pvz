// @param off 储存函数绝对地址的相对偏移
.macro callCFunction off
  push {r0 - r3, lr} // 保护寄存器
  mov lr, pc // 设置返回地址
  ldr pc, $\off
.endm
.macro restoreRegisters
  pop {r0 - r3, lr}
.endm
.macro callCFunction1 off
  push {r0 - r3} // 保护寄存器
  mov lr, pc // 设置返回地址
  ldr pc, $\off
.endm
.macro restoreRegisters1
  pop {r0 - r3}
.endm
.macro globalizeSymbol name
  .global \name
.endm
.macro DEFINE_EXTERNAL_FUNCTION name
  \name: .word 0
.endm
.macro DEFINE_FEATURE name
globalizeSymbol \name
\name:
.endm
.macro DEFINE_FEATURE_END name
DEFINE_FEATURE \name
.endm
.macro FUNCTIONCALLGUARD func
  push {fp, lr}
  mov fp, sp
  callCFunction1 \func
.endm
.macro FUNCTIONEXIT
  restoreRegisters1
  mov sp, fp
  pop {fp, pc}
.endm
// .word 0x0这里是手工加一个NULL
.macro DEFINE_EXT_GAME name file
globalizeSymbol extgames_\name
extgames_\name:
.incbin "\file"
.word 0x0
.endm
