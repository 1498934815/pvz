/*
 * File      : inc/features/features.h
 * Project   :
 * Author    : ze00
 * Email     : zerozakiGeek@gmail.com
 * Date      : 2020-02-13
 * Module    :
 * License   : MIT
 */
#ifndef INC_FEATURES_FEATURES_H
#define INC_FEATURES_FEATURES_H
#include <sys/types.h>
extern "C" {
struct feature {
  const char *name;
  off_t offset;
  void (*code)();
  size_t codesize;
  int originalcode;
  off_t buffer;
};
extern struct feature features[];
}
#endif // INC_FEATURES_FEATURES_H
