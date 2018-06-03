/*
 * File    : src/utils.c
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-01-19
 * Module  :
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../inc/defs.h"
#include "../inc/pvz.h"
#include "../inc/utils.h"
void *insert(__list **target, size_t len) {
  __list *node = malloc(len);
  node->next = NULL;
  if (*target == NULL) {
    *target = node;
  } else {
    // real是当前链表的尾节点
    ((__list *)(*target)->real)->next = node;
  }
  (*target)->real = node;
  return node;
}
void destroy(__list **node) {
  __list *helper;
  while (*node != NULL) {
    helper = (*node)->next;
    free(*node);
    *node = helper;
  }
  *node = NULL;
}
void insert_task(__task **target, int row, int col) {
  __task *node = insert((__list **)target, sizeof(__task));
  node->row = row;
  node->col = col;
}
void pop(__task **target) {
  __task *helper = next(*target);
  free(*target);
  *target = helper;
}
int has(__task *target, int row, int col) {
  while (target != NULL) {
    if (target->row == row && target->col == col)
      return 1;
    target = next(target);
  }
  return 0;
}
void strip(const char *buf) {
  char *temp;
  if ((temp = strchr(buf, '\n')))
    *temp = 0;
}
void parseRowAndCol(const char *buf, __task **task) {
  strip(buf);
  const char *val = buf;
  int row, col;
  enum statusMachine {
    NEED_DOT,
    NEED_COMMA,
    NEED_ROW,
    NEED_COL,
  } status = NEED_ROW;
parse:
#define CHECK(stmt)                                                            \
  if (!(stmt)) {                                                               \
    printf("%s\n", buf);                                                       \
    printf("%*s\n", (int)((val - buf) + 1), "^");                              \
    goto panic;                                                                \
  }
#define DIGIT() (*val - '0')
  if (*val == '\0') {
    if (status != NEED_COMMA) {
      --val;
      CHECK(0);
    }
    goto out;
  }
  while (isspace(*val))
    ++val;
  switch (status) {
  case NEED_ROW:
    CHECK(isdigit(*val) && IN_RANGE(DIGIT(), 1, 6));
    row = DIGIT();
    status = NEED_DOT;
    val++;
    goto parse;
  case NEED_COL:
    CHECK(isdigit(*val) && IN_RANGE(DIGIT(), 1, 9));
    col = DIGIT();
    status = NEED_COMMA;
    val++;
    // 不允许重复
    if (!has(*task, row, col))
      insert_task(task, row, col);
    goto parse;
  case NEED_DOT:
    CHECK('.' == *val);
    status = NEED_COL;
    val++;
    goto parse;
  case NEED_COMMA:
    CHECK(',' == *val);
    status = NEED_ROW;
    val++;
    goto parse;
  }
panic:
  PANIC;
out:
  return;
}
const char *to_string(const char *fmt, ...) {
  static BufferType buf;
  va_list va;
  va_start(va, fmt);
  vsprintf(buf, fmt, va);
  va_end(va);
  return buf;
}
#undef CHECK
#undef DIGIT
