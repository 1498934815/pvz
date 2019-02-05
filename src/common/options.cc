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
#include <utility>
#include <algorithm>
#include <common/options.h>
void Options::addOption(unsigned id, flags flags, const char *name,
                        const char *description) {
  options.emplace_back(option{id, flags, name, description});
}
const option *Options::getOption(unsigned id) {
  auto &&it = std::find(options.begin(), options.end(), id);
  return it != options.end() ? &*it : nullptr;
}
std::vector<option> &&Options::getOptions() {
  return std::move(options);
}
Options::Options() {
  unsigned id = 0;
#define DEFINE_CHEAT(flags, name, description)                                 \
  addOption(id++, flags, name, description)
#include <common/cheats_def.h>
#undef DEFINE_CHEAT
}
