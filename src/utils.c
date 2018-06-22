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
void *insert(void *_Target, size_t len) {
  struct list **target = _Target;
  struct list *node = malloc(len);
  next(node) = NULL;
  if (*target == NULL) {
    *target = node;
  } else {
    // real是当前链表的尾节点
    next(real(*target)) = node;
  }
  real(*target) = node;
  return node;
}
void destroy(void *_Node) {
  struct list **node = _Node;
  struct list *helper;
  while (*node != NULL) {
    helper = next(*node);
    free(*node);
    *node = helper;
  }
  *node = NULL;
}
void insert_task(struct task **target, int row, int col) {
  struct task *node = insert((struct list **)target, sizeof(struct task));
  node->row = row;
  node->col = col;
}
void pop(struct task **target) {
  struct task *helper = next(*target);
  free(*target);
  *target = helper;
}
int has(struct task *target, int row, int col) {
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
void parseRowAndCol(const char *buf, struct task **task) {
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
void parseAddr(const char *rec, void **out) { sscanf(rec, "%p", out); }
void parseInt(const char *rec, int *out) { sscanf(rec, "%d", out); }
#undef CHECK
#undef DIGIT
