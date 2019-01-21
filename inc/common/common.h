 /*
  * File    : inc/common/common.h
  * Project :
  * Author  : ze00
  * Email   : zerozakiGeek@gmail.com
  * Date    : 2019-01-18
  * Module  : 
  * License : MIT
  */
#ifndef __DEFS__H
#define __DEFS__H
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 0x7a65
#ifdef DEBUG
#include <stdio.h>
#define DEBUG_LOG(fmt, args...) printf(fmt "\n", ##args)
#else
#define DEBUG_LOG(...)
#endif
#endif // __DEFS__H
