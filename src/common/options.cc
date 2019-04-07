/*
 * File    : src/common/options.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-25
 * Module  :
 * License : MIT
 */
#include "common/options.h"
#define foreachExternalOption(val)                                             \
  for (option *val = externalOptions; val->name != nullptr; ++val)
void Options::uiPrint() {
  unsigned pad;
  foreachExternalOption(o) {
    // 补充适当的空格
    pad = maxname - o->wide;
    // 奇数的ID或者已是最后一个时
    // 则换行
    if (o->id % 2 == 1 || o->id == lastID)
      uiprintf("%2u.%s\n", o->id, o->name);
    else
      uiprintf("%2u.%s%*s", o->id, o->name, pad, "\t");
  }
  fflush(stdout);
}
option *Options::getOption(unsigned id) {
  foreachExternalOption(o) {
    if (o->id == id)
      return o;
  }
  return nullptr;
}
PvzDaemon *Options::getDaemon(unsigned id) {
  return &getOption(id)->daemon;
}
Options::Options() {
  unsigned id = 0;
  foreachExternalOption(o) {
    o->id = id++;
    o->wide = strlen(o->name) / 3 * 2;
    if (o->wide > maxname)
      maxname = o->wide;
    if (o->attr & DAEMON_CALLBACK)
      o->daemon = {
          .callback = o->daemon_callback,
          .on = false,
      };
  }
  lastID = id - 1;
}
