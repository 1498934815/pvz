/*
 * File    : inc/com.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-06-17
 * Module  :
 * License : MIT
 */
#ifndef __COM__H
#define __COM__H
#include <stdlib.h>
#include "../inc/defs.h"
#include "../inc/cheat.h"
enum server_attr {
  SERVER_NULL = 1,
  SERVER_GETINT = 2,
  SERVER_NOT_INGAME = 4,
  SERVER_NEED_ZOMBIES = 8,
  SERVER_NEED_PLANTS = 16,
  SERVER_GETCOLROW = 32,
  SERVER_CANCEL_DAEMON = 64,
  SERVER_RUNAS_DAEMON = 128,
};
enum user_attr {
  USER_NULL = 1,
  USER_GETINT = 2,
  USER_GETSTRING = 4,
  USER_GETCOLROW = 8,
  USER_DEBUGINFO = 16,
  USER_EXIT = 32,
};
struct pvz_option {
  unsigned id;
  unsigned wide;
  const char *name;
  const char *notice;
  cheat_function callback;
  enum server_attr server_attr;
  enum user_attr user_attr;
};

extern struct pvz_option pvz_options[];

#define for_each_option(option)                                                \
  for (struct pvz_option *option = pvz_options; option->name != NULL; ++option)

unsigned getOptionsLength(void);
void doInitOptions(void);
struct pvz_option *getOption(unsigned);

extern unsigned padding;
extern unsigned leastID;
#endif //__COM__H
