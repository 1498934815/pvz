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
#include <stdbool.h>
#include <pthread.h>
#include "../inc/defs.h"

#define PANIC raise(SIGINT)
struct list {
  struct list *next;
  struct list *real;
};
struct task {
  struct list list;
  uint32_t row;
  uint32_t col;
};
struct daemon {
  struct list list;
  cheat_function callback;
  unsigned startID;
  unsigned cancelID;
  pthread_t tid;
  bool on;
};
#define next(x) (*(__typeof__(x) *)&(((struct list *)(x))->next))
#define real(x) (*(__typeof__(x) *)&(((struct list *)(x))->real))
extern void pop(void *);
extern bool has(struct task *, int, int);
extern void insert_task(struct task **, int, int);
extern void insert_daemon(struct daemon **, unsigned, unsigned, cheat_function);
extern void destroy(void *);
extern void *insert(void *, size_t);
extern void parseRowAndCol(const char *, struct task **);
extern const char *__attribute__((format(printf, 1, 2)))
to_string(const char *, ...);

extern void parseAddr(const char *, void **);
extern void parseInt(const char *, int *);

extern void strip(const char *);

#endif //__UTILS__H
