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

bool shouldRight(enum kinds lhs, enum kinds rhs) {
  switch (lhs) {
  case OPERATOR:
    if (IN_RANGE(rhs, DECINT, IDENTITY))
      return true;
  default:;
  }
  return false;
}
bool shouldLeft(enum kinds lhs, enum kinds rhs) {
  switch (lhs) {
  case OPERATOR:
    if (IN_RANGE(rhs, DECINT, IDENTITY))
      return true;
  default:;
  }
  return false;
}
bool shouldParent(enum kinds lhs, enum kinds rhs) {
  switch (rhs) {
  case OPERATOR:
    if (IN_RANGE(lhs, OPERATOR, IDENTITY))
      return true;
  default:;
  }
  return false;
}
void __insertAST(struct AST **seqs, struct AST *ins) {
  struct AST *prev = *seqs;
  if (shouldLeft(prev->kind, ins->kind) && prev->left == NULL) {
    prev->left = ins;
  } else if (shouldRight(prev->kind, ins->kind) && prev->right == NULL) {
    prev->right = ins;
  } else if (shouldParent(prev->kind, ins->kind) &&
             (ins->left == NULL || ins->right == NULL)) {
    *(ins->left ? &ins->right : &ins->left) = prev;
    *seqs = ins;
  } else {
    printf("SYNATX ERROR\n");
  }
}
void insertAST(struct AST **seqs, enum kinds kind, const char *ref,
               size_t len) {
  struct AST *ins = (struct AST *)malloc(sizeof(struct AST));
  ins->left = NULL;
  ins->right = NULL;
  ins->ref.ref = ref;
  ins->ref.len = len;
  ins->kind = kind;

  if (*seqs == NULL) {
    *seqs = ins;
  } else {
    __insertAST(seqs, ins);
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

typedef int (*PassFunction)(struct AST **, const char, size_t);
void handleTokens(PassFunction passFunction, struct AST **seqs,
                  const char **ins) {
  const char *ori = *ins;
  char C;
  size_t consumed = 0;
  int status;
  enum kinds kind;
consume:
  C = **ins;
  if (C == 0 || isspace(C))
    goto end;
  status = passFunction(seqs, C, consumed);
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
  goto consume;
failed:
  ++*ins;
  // FIXME:consumed是相对于某个Token的
  printf("SYNATX ERROR AT COL %d\n", consumed + 1);
  return;
end:
  kind = KIND(status);
  insertAST(seqs, kind, ori, consumed);
  return;
}
int handleNumeric(struct AST **seqs, const char C, size_t consumed) {
  if (isdigit(C))
    return RETCODE(PASSED, DECINT);
  else if (isxdigit(C) && consumed >= 2)
    return RETCODE(PASSED, HEXINT);
  else if ((inseq(C, "xX")) && consumed == 1)
    return RETCODE(PASSED, HEXINT);
  else if (inseq(C, "+-"))
    return NOKIND(END);
  else
    return NOKIND(FAILED);
}
int handleOperator(struct AST **seqs, const char C, size_t consumed) {
  if (inseq(C, "+-"))
    return RETCODE(PASSED, OPERATOR);
  else
    return NOKIND(FAILED);
}
int handleIdentity(struct AST **seqs, const char C, size_t consumed) {
  if (isalpha(C))
    return RETCODE(PASSED, IDENTITY);
  else if (isdigit(C) && consumed != 0)
    return RETCODE(PASSED, IDENTITY);
  else if (inseq(C, "+-"))
    return NOKIND(END);
  else
    return NOKIND(FAILED);
}
int handleSubExprBegin(struct AST **seqs, const char C, size_t consumed) {
  return RETCODE(PASSED, SUBEXPRBEGIN);
}
int handleSubExprEnd(struct AST **seqs, const char C, size_t consumed) {
  return RETCODE(PASSED, SUBEXPREND);
}
struct AST *parseInstructions(const char *ins) {
  const char *val = ins;
  char C;
  struct AST *seqs = NULL;
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
