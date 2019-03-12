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
#include <vector>
#include <common/common.h>
enum attr {
  NONE,
  EXIT = 1,
  DEBUG_INFO = 2,
  GAMING = 4,
  GETINT = 8,
  GETSTRING = 16,
};
class option {
public:
  unsigned id;
  attr attr;
  const char *name;
  const char *description;
  operator int() {
    return id;
  }
};
class Options : public Singleton<Options> {
  std::vector<option> options;

public:
  Options();
  void addOption(attr, const char *, const char *);
  void uiPrint();
  const option *getOption(unsigned);
  const std::vector<option> &getOptions();
};
#endif // INC_COMMON_OPTIONS_H
