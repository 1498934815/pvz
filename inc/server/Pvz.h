/*
 * File    : inc/server/Pvz.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-25
 * Module  :
 * License : MIT
 */
#ifndef __PVZ__H
#define __PVZ__H
#include <stdlib.h>

void __writeMem(void *ptr, void *src, size_t size) {
  memcpy(ptr, src, size);
}
template <typename Ty> void writeMem(void *ptr, Ty &value) {
  __writeMem(ptr, reinterpret_cast<void *>(&value), sizeof(value));
}
template <typename Ty> Ty readMem(void *ptr, Ty &value) {
  __writeMem(reinterpret_cast<void *>(&value), ptr, sizeof(value));
  return value;
}
#define implGet(name, type)                                                    \
  type get##name(void *ptr, type &value) {                                     \
    return readMem<type>(ptr, value);                                          \
  }                                                                            \
  type get##name(void *ptr) {                                                  \
    static type buf;                                                           \
    return get##name(ptr, buf);                                                \
  }
#define implSet(name, type)                                                    \
  void set##name(void *ptr, type &value) {                                     \
    writeMem<type>(ptr, value);                                                \
  }
#define implGetSet(name, type) implGet(name, type) implSet(name, type)
implGetSet(I32, int32_t);
implGetSet(F32, float);
implGetSet(Ptr, void *);
#undef implGet
#endif // __PVZ__H
