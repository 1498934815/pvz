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
#include "../inc/utils.h"

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
  const char *ref;
  size_t len;
};

struct ASTQueue {
  struct list list;
  struct AST **AST;
};

struct AST *parseInstructions(const char *);
#endif //__CONSOLE__H
