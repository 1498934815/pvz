/*
 * File    : inc/common/common.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-18
 * Module  :
 * License : MIT
 */
#ifndef __DEFS__H
#define __DEFS__H
#include <errno.h>
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 0x7a65
#ifdef DEBUG
#include <stdio.h>
#define DEBUG_LOG(fmt, args...) printf(fmt "\n", ##args)
#else
#define DEBUG_LOG(...)
#endif

#define BLOB GIT_REPO "/blob/" GIT_BRANCH
#define CODE_TXT BLOB "/doc/code.txt"
#define README_MD BLOB "/README.md"

#define NOTICE "\033[33;1m[NOTICE]\033[0m "
#define ERR "\033[31;1m[ERR]\033[0m "

#define uiprint(...) puts(__VA_ARGS__)
#define uiprintf(...) printf(__VA_ARGS__)
#define uinotice(...) uiprint(NOTICE __VA_ARGS__)
#define uinoticef(...) uiprintf(NOTICE __VA_ARGS__)
#define uierror(...) uiprint(ERROR __VA_ARGS__)
#define uierrorf(...) uiprint(ERROR __VA_ARGS__)
template <typename Ty> class Singleton {
  static Ty *instance;

public:
  Singleton() {
    if (getInstance() == nullptr)
      instance = reinterpret_cast<Ty *>(this);
  }
  static Ty *getInstance() {
    return instance;
  }
};
template <typename Ty> Ty *Singleton<Ty>::instance = nullptr;
template <typename Err, typename Ty = void *> struct error {
  Err errcode;
  int reason;
  Ty value;
  error(Err code, Ty val) : errcode(code), reason(errno), value(val) {}
  error &when(Err err, Ty val) {
    if (errcode == err)
      value = val;
    return *this;
  }
  operator Err() {
    return errcode;
  }
  operator Ty() {
    return value;
  }
};
#endif // __DEFS__H
