/*
 * File    : src/common/options.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-25
 * Module  :
 * License : MIT
 */
#include <algorithm>
#include <common/options.h>
#define foreachExternalOption(val)                                             \
  for (option *val = externalOptions; val->name != nullptr; ++val)
void Options::uiPrint() {
  foreachExternalOption(o) {
    uiprintf("%d.%s\n", o->id, o->name);
  }
}
const option *Options::getOption(unsigned id) {
  foreachExternalOption(o) {
    if (o->id == id)
      return o;
  }
  return nullptr;
}
Options::Options() {
  unsigned id = 0;
  foreachExternalOption(o) {
    o->id = id++;
  }
}
