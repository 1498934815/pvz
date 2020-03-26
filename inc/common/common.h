/*
 * File    : inc/common/common.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-18
 * Module  :
 * License : MIT
 */
#ifndef INC_COMMON_COMMON_H
#define INC_COMMON_COMMON_H
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 0x6b65

#define BLOB GIT_REPO "/blob/" GIT_BRANCH
#define CODE_TXT BLOB "/doc/code.txt"
#define README_MD BLOB "/README.md"

#include <stdio.h>

#ifdef SERVER
#include <android/log.h>
#define TAG "ZEROZAKI"
#define uiprint(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define uiprintf(...) uiprint(__VA_ARGS__)
#define NOTICE
#define HINT
#define ERR
#else
#define uiprint(...) puts(__VA_ARGS__)
#define uiprintf(...) printf(__VA_ARGS__)
#define NOTICE "\033[33;1m[NOTICE]\033[0m "
#define HINT "\033[32;1m[HINT]\033[0m "
#define ERR "\033[31;1m[ERR]\033[0m "
#endif

#define uihint(...) uiprint(HINT __VA_ARGS__)
#define uihintf(...) uiprintf(HINT __VA_ARGS__)
#define uinotice(...) uiprint(NOTICE __VA_ARGS__)
#define uinoticef(...) uiprintf(NOTICE __VA_ARGS__)
#define uierror(...) uiprint(ERR __VA_ARGS__)
#define uierrorf(...) uiprintf(ERR __VA_ARGS__)
#define uiscanf(...) scanf(__VA_ARGS__)
#define uivscanf(...) vscanf(__VA_ARGS__)

#ifdef DEBUG
#ifdef SERVER
#define DEBUG_LOG(fmt, args...) uiprintf(fmt, ##args)
#else
#define DEBUG_LOG(fmt, args...) uiprintf(fmt "\n", ##args)
#endif
#else
#define DEBUG_LOG(...)
#endif

#define in_range(val, lb, rb) ((val) >= (lb) && (val) <= (rb))

#define POINTERSIZE (sizeof(void *))

#define POINTER_DIFFERENCE(p1, p2) ((uintptr_t)p1 - (uintptr_t)p2)

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
template <typename Err = int, typename Ty = int> struct error {
  Err errcode;
  int reason;
  Ty value;
  error(Err code, Ty val = Ty()) : errcode(code), reason(errno), value(val) {}
  error &when(Err err, Ty val) {
    if (errcode == err)
      value = val;
    return *this;
  }
  operator Err() {
    return errcode;
  }
  Ty &getValue() {
    return value;
  }
  error &except(Err code, const char *message) {
    // XXX We will send a INTERRUPT_SIGNAL to all child threads and then the
    // syscall will produce a EINTR, we should ignore it on this time and set
    // the value as a defaultt
    if (reason == EINTR) {
      value = Ty(0);
    } else if (errcode == code) {
      uierrorf("PANIC, code:%d, errno:%d(%s), message:'%s'\n", errcode, reason,
               strerror(reason), message);
      exit(1);
    }
    return *this;
  }
};
inline void clientPanic() {
  raise(SIGINT);
}
enum class msgStatus : unsigned int {
  NONE,
  EOR,
  COMMAND,
  REMOTE_ERROR,
};
struct msgPack {
  enum msgStatus status;
  unsigned id;
  union {
    void *ptr;
    int_least64_t val;
  };
  char msg[128];
};

inline msgPack makeMsgPack(unsigned id, int_least64_t val,
                           const char *msg = nullptr,
                           msgStatus status = msgStatus::NONE) {
  msgPack pack = {
      .status = status,
      .id = id,
      .val = val,
  };
  if (msg != nullptr) {
    assert(strlen(msg) < sizeof(msgPack::msg) || !"Out of buffer size");
    strcpy(pack.msg, msg);
  }
  return pack;
}
inline msgPack makeMsgPack(int_least64_t val, const char *msg = nullptr,
                           msgStatus status = msgStatus::NONE) {
  return makeMsgPack(0, val, msg, status);
}
#endif // INC_COMMON_COMMON_H
