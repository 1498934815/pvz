/*
 * File    : inc/utils.h
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-01-19
 * Module  :
 */
#ifndef __UTILS__H
#define __UTILS__H
#include <stdint.h>
#include <unistd.h>
#include "defs.h"
#define PANIC                                                                  \
  do {                                                                         \
    printf("无效输入\n");                                                      \
    raise(SIGINT);                                                             \
  } while (0)
typedef struct __list {
  void *next;
  void *real;
} __list;
typedef struct __task {
  __list list;
  int32_t row;
  int32_t col;
} __task;
#define next(x) ((x)->list.next)
#define real(x) ((x)->list.real)
extern void pop(__task **);
extern int has(__task *, int, int);
extern void destroy(__list **, void (*)(void *));
extern void *insert(__list **, size_t);
extern void parseRowAndCol(const char *, __task **);
extern const char *__attribute__((format(printf, 1, 2)))
to_string(const char *, ...);
#endif //__UTILS__H
