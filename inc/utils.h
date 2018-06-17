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
#include "../inc/defs.h"
#define PANIC                                                                  \
  do {                                                                         \
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
extern void insert_task(__task **, int, int);
extern void destroy(__list **);
extern void *insert(__list **, size_t);
extern void parseRowAndCol(const char *, __task **);
extern const char *__attribute__((format(printf, 1, 2)))
to_string(const char *, ...);

void parseAddr(const char *, void **);
void parseInt(const char *, int *);

#endif //__UTILS__H
