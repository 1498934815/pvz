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
#include "common/PvzCommon.h"
#include "common/common.h"
#include <pthread.h>
enum attr {
  NONE = 0,
  EXIT = 1 << 0,
  DEBUG_INFO = 1 << 1,
  GAMING = 1 << 2,
  GETINT = 1 << 3,
  GETPOINTS = 1 << 4,
  GETINTS = 1 << 5,
  DAEMON_CALLBACK = 1 << 6,
  // Keep it
  CANCEL_DAEMON_CALLBACK = 1 << 7,
  PLANTS_CALLBACK = 1 << 8,
  ZOMBIES_CALLBACK = 1 << 9,
  MOWERS_CALLBACK = 1 << 10,
  ITEMS_CALLBACK = 1 << 11,
  GAMES_CODE = 1 << 12,
  CARDS_CODE = 1 << 13,
  ZOMBIES_CODE = 1 << 14,
  FIELDS_CODE = 1 << 15,
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
  unsigned width;
  PvzDaemon daemon;
};
class Options : public Singleton<Options> {
  unsigned maxname = 0;
  unsigned lastID = 0;

public:
  Options();
  void addOption(attr, const char *, const char *);
  void uiPrint();
  option *getOption(unsigned);
  PvzDaemon *getDaemon(unsigned);
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
      DEFINE_OPTION(DEBUG_INFO, "打印调试信息", nullptr, nullptr),             \
      DEFINE_OPTION(EXIT, "退出", nullptr, nullptr),                           \
      DEFINE_OPTION(NONE, nullptr, nullptr, nullptr),                          \
  };
#endif // INC_COMMON_OPTIONS_H
