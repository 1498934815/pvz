/*
 * File    : src/server/Pvz.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#include <server/Pvz.h>
#include <server/PvzServer.h>
PvzOffset *__getOffset(const char *name) {
  return nullptr;
}
off_t getOffset(const char *name) {
  return __getOffset(name)->off;
}
void *incrFrom(void *ptr, const char *name) {
  uintptr_t uiptr = reinterpret_cast<intptr_t>(ptr);
  return reinterpret_cast<void *>(uiptr + getOffset(name));
}
void *incrFromBase(const char *name) {
  return incrFrom(PvzServer::getInstance()->getBase(), name);
}
void *incrFromStatus(const char *name) {
  return incrFrom(PvzServer::getInstance()->getStatus(), name);
}
