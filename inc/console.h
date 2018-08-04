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

#include <stddef.h>

enum kinds {
  NONE,
  OPERATOR,
  DECINT,
  HEXINT,
  IDENTITY,
  SUBEXPRBEGIN,
  SUBEXPREND,
};

struct AST {
  struct AST *left;
  struct AST *right;
  enum kinds kind;
  const void *ref;
  size_t len;
};

struct AST *parseInstructions(const char *);
#endif //__CONSOLE__H
