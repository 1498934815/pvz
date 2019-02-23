/*
 * File    : src/common/options.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-25
 * Module  :
 * License : MIT
 */
#include <vector>
#include <algorithm>
#include <common/options.h>
void Options::addOption(attr attr, const char *name, const char *description) {
  static unsigned id = 0;
  options.emplace_back(option{id++, attr, name, description});
}
void Options::uiPrint() {
  for (auto &&o : getOptions()) {
    uiprintf("%d.%s\n", o.id, o.name);
  }
}
const option *Options::getOption(unsigned id) {
  auto &&it = std::find(options.begin(), options.end(), id);
  return it != options.end() ? &*it : nullptr;
}
const std::vector<option> &Options::getOptions() {
  return options;
}
Options::Options() {
#define DEFINE_CHEAT(attr, name, description) addOption(attr, name, description)
#include <common/cheats_def.h>
#undef DEFINE_CHEAT
}
