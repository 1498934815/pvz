/*
 * File    : inc/common/PvzUtils.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-04-20
 * Module  :
 * License : MIT
 */
#ifndef INC_COMMON_PVZUTILS_H
#define INC_COMMON_PVZUTILS_H
#include "common/PvzCommon.h"
#include <vector>

std::vector<PvzPoint> parsePoints(const char *);
std::vector<int> parseInts(const char *);

#endif // INC_COMMON_PVZUTILS_H
