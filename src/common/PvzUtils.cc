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
#include <stdarg.h>
#include <string.h>
#include <string>
#include <vector>
std::vector<std::string> splitBy(std::string expr, char separator) {
  std::vector<std::string> vec;
  std::string::size_type bpos = 0, epos = 0;
  while ((epos = expr.find(separator, bpos)) != std::string::npos) {
    if (epos != 0)
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
    if (pointVec.size() != 2) {
      uierror("Invalid format");
      clientPanic();
    }
    vec.emplace_back(PvzPoint{std::stoi(pointVec[0]), std::stoi(pointVec[1])});
  }
  return vec;
}
std::vector<int> parseInts(const char *expr) {
  std::vector<int> vec;
  for (auto &&intsString : splitBy(expr, ','))
    vec.emplace_back(std::stoi(intsString));
  return vec;
}
const char *formatBuffer(const char *fmt, ...) {
  static char buffer[256];
  va_list va;
  va_start(va, fmt);
  vsnprintf(buffer, sizeof(buffer) - 1, fmt, va);
  va_end(va);
  return buffer;
}
