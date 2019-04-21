/*
 * File    : src/common/PvzUtils.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-04-20
 * Module  :
 * License : MIT
 */
#include "common/PvzUtils.h"
#include "common/common.h"
#include <string.h>
#include <string>
#include <vector>
std::vector<std::string> splitBy(std::string expr, char separator) {
  std::vector<std::string> vec;
  std::string::size_type bpos = 0, epos = 0;
  while ((epos = expr.find(separator, bpos)) != std::string::npos) {
    vec.emplace_back(expr.substr(bpos, epos - bpos));
    bpos = epos + 1;
  }
  if (expr.back() != separator)
    vec.emplace_back(expr.substr(bpos));
  return vec;
}
std::vector<PvzPoint> parsePoints(const char *expr) {
  std::vector<PvzPoint> vec;
  for (auto &&pointString : splitBy(expr, ',')) {
    auto &&pointVec = splitBy(pointString, '.');
    error<bool>(pointVec.size() != 2).except(true, "Invalid format");
    // XXX We are use atoi now...
    // because the stoi function is not support in android-ndk-r12b...
    int x = atoi(pointVec[0].c_str()), y = atoi(pointVec[1].c_str());
    vec.emplace_back(PvzPoint{x, y});
  }
  return vec;
}
std::vector<int> parseInts(const char *expr) {
  std::vector<int> vec;
  for (auto &&intsString : splitBy(expr, ','))
    vec.emplace_back(atoi(intsString.c_str()));
  return vec;
}
