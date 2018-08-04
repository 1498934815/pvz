/*
 * File    : src/console.c
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-07-31
 * Module  :
 * License : MIT
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../inc/defs.h"
#include "../inc/console.h"

struct ASTQueue *gQueue;
bool shouldRight(struct AST *lhs, struct AST *rhs) {
  switch (lhs->kind) {
  case OPERATOR:
    if (IN_RANGE(rhs->kind, DECINT, SUBEXPRBEGIN))
      return true;
    break;
  case SUBEXPRBEGIN:
    if (rhs->kind == SUBEXPREND)
      return true;
    break;
  default:;
  }
  return false;
}
bool shouldLeft(struct AST *lhs, struct AST *rhs) {
  switch (lhs->kind) {
  case OPERATOR:
    if (IN_RANGE(rhs->kind, DECINT, SUBEXPRBEGIN))
      return true;
    break;
  case SUBEXPRBEGIN:
    if (IN_RANGE(rhs->kind, DECINT, SUBEXPRBEGIN))
      return true;
    break;
  default:;
  }
  return false;
}
bool shouldParent(struct AST *lhs, struct AST *rhs) {
  switch (rhs->kind) {
  case OPERATOR:
    if (IN_RANGE(lhs->kind, OPERATOR, IDENTITY))
      return true;
    if (lhs->kind == SUBEXPRBEGIN && lhs->right != NULL)
      return true;
    break;
  default:;
  }
  return false;
}
struct AST **__insertAST(struct AST **target, struct AST *ins) {
  struct AST *prev = *target;
  if (prev->kind == SUBEXPRBEGIN && prev->left != NULL && prev->right == NULL &&
      ins->kind != SUBEXPREND) {
    prev->left = *__insertAST(&prev->left, ins);
    return &prev->left;
  }
  if (shouldLeft(prev, ins) && prev->left == NULL) {
    prev->left = ins;
    return &prev->left;
  } else if (shouldRight(prev, ins) && prev->right == NULL) {
    prev->right = ins;
    return &prev->right;
  } else if (shouldParent(prev, ins)) {
    ins->left = prev;
    *target = ins;
    return target;
  } else {
    printf("SYNATX ERROR\n");
  }
  return NULL;
}
struct AST **getMostAST(void) {
  return real(gQueue)->AST;
}
void pushAST(struct AST **AST) {
  ((struct ASTQueue *)insert(&gQueue, sizeof(gQueue)))->AST = AST;
}
void popAST(void) {
  struct ASTQueue *p, *q;
  for (p = gQueue; p != real(gQueue); p = next(p))
    q = p;
  pop(q);
}
void insertAST(struct AST **seqs, enum kinds kind, const char *ref,
               size_t len) {
  struct AST *ins = (struct AST *)malloc(sizeof(struct AST));
  struct AST **ret;
  ins->left = NULL;
  ins->right = NULL;
  ins->ref = ref;
  ins->len = len;
  ins->kind = kind;

  if (*seqs == NULL) {
    *seqs = ins;
  } else {
    ret = __insertAST(getMostAST(), ins);
    if (ret != NULL) {
      switch ((*ret)->kind) {
      case SUBEXPRBEGIN:
        pushAST(ret);
        break;
      case SUBEXPREND:
        if (getMostAST() != seqs)
          popAST();
      default:;
      }
    }
  }
}

#define inseq(C, seqs) (strchr(seqs, C) != NULL)
#define PASSED 0
#define END 1
#define FAILED 2
#define SHIFT 8
#define RETCODE(status, kind) (status << SHIFT | kind)
#define STATUS(status) (status >> SHIFT)
#define KIND_MASK ((1 << SHIFT) - 1)
#define KIND(status) (status & KIND_MASK)
#define NOKIND(status) RETCODE(status, NONE)

bool checkTokenLength(enum kinds kind, size_t consumed) {
  switch (kind) {
  case SUBEXPRBEGIN:
  case SUBEXPREND:
  case OPERATOR:
    return consumed == 1;
  default:;
  }
  return false;
}
typedef int (*PassFunction)(const char, size_t);
void handleTokens(PassFunction passFunction, struct AST **seqs,
                  const char **ins) {
  const char *ori = *ins;
  char C;
  size_t consumed = 0;
  int status;
  enum kinds kind = NONE;
consume:
  C = **ins;
  if (C == 0 || isspace(C))
    goto end;
  status = passFunction(C, consumed);
  switch (STATUS(status)) {
  case PASSED:
    goto success;
  case END:
    goto end;
  case FAILED:
    goto failed;
  }
success:
  ++*ins;
  ++consumed;
  const enum kinds k = KIND(status);
  // XXX 0xFFFF0
  // 处理到最后的'0'时
  // 会认为是DECINT
  // 实际上如果得到过HEXINT就应该一直都是HEXINT
  // 此处依赖于enum kinds的先后顺序
  kind = k > kind ? k : kind;
  if (checkTokenLength(kind, consumed))
    goto end;
  goto consume;
failed:
  ++*ins;
  // FIXME:consumed是相对于某个Token的
  printf("SYNATX ERROR AT COL %s\n", strndup(ori, consumed));
  return;
end:
  insertAST(seqs, kind, ori, consumed);
  return;
}
int handleNumeric(const char C, size_t consumed) {
  if (isdigit(C))
    return RETCODE(PASSED, DECINT);
  else if (isxdigit(C) && consumed >= 2)
    return RETCODE(PASSED, HEXINT);
  else if ((inseq(C, "xX")) && consumed == 1)
    return RETCODE(PASSED, HEXINT);
  else if (inseq(C, "+-]"))
    return NOKIND(END);
  else
    return NOKIND(FAILED);
}
int handleOperator(const char C, size_t consumed) {
  if (inseq(C, "+-"))
    return RETCODE(PASSED, OPERATOR);
  else
    return NOKIND(FAILED);
}
int handleIdentity(const char C, size_t consumed) {
  if (isalpha(C))
    return RETCODE(PASSED, IDENTITY);
  else if (isdigit(C) && consumed != 0)
    return RETCODE(PASSED, IDENTITY);
  else if (inseq(C, "+-]"))
    return NOKIND(END);
  else
    return NOKIND(FAILED);
}
int handleSubExprBegin(const char C, size_t consumed) {
  // XXX 无论下一个字符是什么
  // 在此返回NOKIND(END)可以把下一个字符的处理交给其他的handleXXX
  if (C == '[')
    return RETCODE(PASSED, SUBEXPRBEGIN);
  else
    return NOKIND(END);
}
int handleSubExprEnd(const char C, size_t consumed) {
  if (C == ']')
    return RETCODE(PASSED, SUBEXPREND);
  else
    return NOKIND(END);
}
struct AST *parseInstructions(const char *ins) {
  const char *val = ins;
  char C;
  struct AST *seqs = NULL;
  gQueue = NULL;
  pushAST(&seqs);
parse:
  while (isspace(*val))
    ++val;
  C = *val;
  if (C == 0) {
    return seqs;
  }
  // 0xFFFFFFF
  // 123456
  if (isdigit(C)) {
    handleTokens(handleNumeric, &seqs, &val);
  } else if (inseq(C, "+-")) {
    handleTokens(handleOperator, &seqs, &val);
  } else if (isalpha(C)) {
    handleTokens(handleIdentity, &seqs, &val);
  } else if (C == '[') {
    handleTokens(handleSubExprBegin, &seqs, &val);
  } else if (C == ']') {
    handleTokens(handleSubExprEnd, &seqs, &val);
  } else {
    printf("Unexpected Token %c\n", C);
    return seqs;
  }
  goto parse;
}
