// @param off 储存函数绝对地址的相对偏移
.macro callCFunction off
  push {r0 - r7, lr} // 保护寄存器
  mov lr, pc // 设置返回地址
  ldr pc, [pc, #\off]
.endm
.macro restoreRegisters
  pop {r0 - r7, lr}
.endm
.macro globalizeSymbol name end
  .global \name
  .global \end
.endm
