/*
 * File    : inc/common/options.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-25
 * Module  :
 * License : MIT
 */
#ifndef __OPTIONS__H
#define __OPTIONS__H
#include <vector>
#include <common/common.h>
enum flags {
  NONE,
};
struct option {
  unsigned id;
  flags flags;
  const char *name;
  const char *description;
};
class Options : public Singleton<Options> {
  std::vector<option> options;

public:
  Options();
  void addOption(unsigned, flags, const char *, const char *);
  option &&getOption(unsigned);
  std::vector<option> &&getOptions();
};
#endif // __OPTIONS__H
