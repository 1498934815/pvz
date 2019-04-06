/*
 * File    : inc/common/options.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-25
 * Module  :
 * License : MIT
 */
#ifndef INC_COMMON_OPTIONS_H
#define INC_COMMON_OPTIONS_H
#include <common/PvzCommon.h>
#include <common/common.h>
#include <pthread.h>
enum attr {
  NONE,
  EXIT = 1,
  DEBUG_INFO = 2,
  GAMING = 4,
  GETINT = 8,
  GETSTRING = 16,
  DAEMON_CALLBACK = 32,
  CANCEL_DAEMON_CALLBACK = 64,
  PLANTS_CALLBACK = 128,
  ZOMBIES_CALLBACK = 256,
  MOWERS_CALLBACK = 512,
};

struct PvzDaemon {
  daemon_callback callback;
  pthread_t tid;
  Communicator *com;
  bool on;
};

struct option {
  unsigned id;
  int attr;
  const char *name;
  const char *description;
  union {
    normal_callback normal_callback;
    object_callback object_callback;
    daemon_callback daemon_callback;
  };
  unsigned wide;
  PvzDaemon daemon;
  operator int() {
    return id;
  }
};
class Options : public Singleton<Options> {
  unsigned maxname = 0;
  unsigned lastID = 0;

public:
  Options();
  void addOption(attr, const char *, const char *);
  void uiPrint();
  option *getOption(unsigned);
};
extern option externalOptions[];
#ifdef SERVER
#define DEFINE_OPTION(attr, name, description, callback)                       \
  {                                                                            \
    0, attr, name, description, {callback}, 0, {}                              \
  }
#else
#define DEFINE_OPTION(attr, name, description, callback)                       \
  {                                                                            \
    0, attr, name, description, {nullptr}, 0, {}                               \
  }
#endif
#define DEFINE_EXTERNAL_OPTIONS(...)                                           \
  option externalOptions[] = {                                                 \
      __VA_ARGS__,                                                             \
      DEFINE_OPTION(DEBUG_INFO, "输出调试信息", nullptr, nullptr),             \
      DEFINE_OPTION(EXIT, "退出", nullptr, nullptr),                           \
      DEFINE_OPTION(NONE, nullptr, nullptr, nullptr),                          \
  };
#endif // INC_COMMON_OPTIONS_H
