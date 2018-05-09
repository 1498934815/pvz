/*
 * File    : defs.h
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2017-08-17
 * Module  :
 * License : MIT
 */
#ifndef __DEFS__H
#define __DEFS__H
#include <limits.h>
#define SERVER_ADDR "127.0.0.1"
// 'ze' -> 0x7a65
#define SERVER_PORT 0x7a65
#define BUFSIZE 4096
#define IN_RANGE(obj, min, max) (obj >= min && obj <= max)
#define SETJMP_RET 0xff
#define WAIT_USECONDS (500000)

#define PAGE_START(x) ((x)&PAGE_MASK)
#define PAGE_END(x) PAGE_START((x) + (PAGE_SIZE - 1))

typedef char BufferType[BUFSIZE];
#endif //__DEFS__H
