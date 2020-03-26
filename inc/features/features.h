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
#include "server/Pvz.h"
#include <sys/types.h>
extern "C" {
struct feature {
  off_t offset;
  void *code;
  size_t codesize;
  int originalcode;
  off_t buffer;
};
struct features_group {
  const char *name;
  struct feature *features;
  bool isEnabled;
};
extern struct features_group features[];
extern void loadPvzFeatures(Communicator *);
}
#define FEATURES_BUFFER_BASE 0x8e5ddc
#endif // INC_FEATURES_FEATURES_H
