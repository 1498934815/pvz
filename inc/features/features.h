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
#include <map>
#include <sys/types.h>
#include <vector>
extern "C" {
struct feature {
  off_t offset;
  int extracode;
  void *code;
  int originalcode[2];
  off_t buffer;
};
struct features_group {
  const char *name;
  struct feature *features;
  bool isEnabled;
};
extern struct features_group features[];
extern void loadPvzFeatures(Communicator *);
extern void enableAllHidenGames();
extern std::map<int, std::vector<std::vector<int>>> zombiesSeeds;
extern std::map<int, std::vector<std::vector<int>>> extGamesSeeds;
}
// JNI_OnUnload
// #define FEATURES_BUFFER_BASE 0x8e5ddc
// 0076c21c T __gnu_Unwind_RaiseException
#define FEATURES_BUFFER_BASE 0x76c21c
#endif // INC_FEATURES_FEATURES_H
