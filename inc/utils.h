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
#define PANIC raise(SIGINT)
struct list {
  struct list *next;
  struct list *real;
};
struct task {
  struct list list;
  int32_t row;
  int32_t col;
};
#define next(x) (*(__typeof__(x) *)&(((struct list *)(x))->next))
#define real(x) (*(__typeof__(x) *)&(((struct list *)(x))->real))
extern void pop(struct task **);
extern int has(struct task *, int, int);
extern void insert_task(struct task **, int, int);
extern void destroy(void *);
extern void *insert(void *, size_t);
extern void parseRowAndCol(const char *, struct task **);
extern const char *__attribute__((format(printf, 1, 2)))
to_string(const char *, ...);

void parseAddr(const char *, void **);
void parseInt(const char *, int *);

#endif //__UTILS__H
