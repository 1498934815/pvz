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

struct block_ref {
  const void *ref;
  size_t len;
};

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
  struct block_ref ref;
};

struct AST *parseInstructions(const char *);
#endif //__CONSOLE__H
