/*
 * File    : inc/console.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-07-31
 * Module  :
 * License : MIT
 */
#ifndef __CONSOLE__H
#define __CONSOLE__H

#include "../inc/utils.h"

struct block_ref {
  const void *ref;
  size_t len;
};

enum operates {
  VALUE,
  MEMREAD,
  MEMWRITE,
};

struct instruction {
  struct list list;
  enum operates operate;
  struct block_ref ref;
};

void parseInstructions(const char *);
#endif //__CONSOLE__H
