/*
 * File    : src/console.c
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-07-31
 * Module  :
 * License : MIT
 */
#include <stdio.h>
#include <ctype.h>
#include "../inc/client.h"
#include "../inc/console.h"
void insert_instruction(struct instruction **seqs, enum operates operate,
                        const char *ref, size_t len) {
  struct instruction *ins = (struct instruction *)insert(seqs, sizeof(**seqs));
  ins->ref.ref = ref;
  ins->operate = operate;
}

#define inseq(C, seqs) (strchr(seqs, C) != NULL)
#define SUCCESS 0
#define END 1
#define FAILED 2
typedef int (*PassFunction)(struct instruction **, const char, size_t);
void handleTokens(PassFunction passFunction, struct instruction **seqs,
                  const char **ins) {
  const char *ori = *ins;
  char C;
  size_t consumed = 0;
consume:
  C = **ins;
  if (C == 0 || isspace(C))
    goto end;
  switch (passFunction(seqs, C, consumed)) {
  case SUCCESS:
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
  printf("FAILED AT COL %d\n", consumed + 1);
  return;
end:
  printf("PARSED %d %*s\n", consumed, consumed, ori);
  return;
}
int handleNumeric(struct instruction **seqs, const char C, size_t consumed) {
  if (isdigit(C) || (isxdigit(C) && consumed >= 2))
    return SUCCESS;
  else if ((inseq(C, "xX")) && consumed == 1)
    return SUCCESS;
  else if (inseq(C, "+-"))
    return END;
  else
    return FAILED;
}
int handleOperator(struct instruction **seqs, const char C, size_t consumed) {
  if (inseq(C, "+-"))
    return SUCCESS;
  else
    return FAILED;
}
int handleIdentity(struct instruction **seqs, const char C, size_t consumed) {
  if (isalpha(C))
    return SUCCESS;
  else if (isdigit(C) && consumed != 0)
    return SUCCESS;
  else if (inseq(C, "+-"))
    return END;
  else
    return FAILED;
}
void parseInstructions(const char *ins) {
  const char *val = ins;
  char C;
  struct instruction *seqs = NULL;
parse:
  while (isspace(*val))
    ++val;
  C = *val;
  if (C == 0) {
    return;
  }
  // 0xFFFFFFF
  // 0777
  // 123456
  if (isdigit(C)) {
    handleTokens(handleNumeric, &seqs, &val);
  } else if (inseq(C, "+-")) {
    handleTokens(handleOperator, &seqs, &val);
  } else if (isalpha(C)) {
    handleTokens(handleIdentity, &seqs, &val);
  } else {
    printf("Unexpected Token %c\n", C);
    return;
  }
  goto parse;
}
