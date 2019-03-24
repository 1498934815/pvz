/*
 * File    : inc/server/Pvz.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-25
 * Module  :
 * License : MIT
 */
#ifndef INC_SERVER_PVZ_H
#define INC_SERVER_PVZ_H
#include <stdlib.h>

inline void __writeMem(void *ptr, void *src, size_t size) {
  memcpy(ptr, src, size);
}
template <typename Ty> void writeMem(void *ptr, Ty &value) {
  __writeMem(ptr, reinterpret_cast<void *>(&value), sizeof(value));
}
template <typename Ty> Ty readMem(void *ptr, Ty &value) {
  __writeMem(reinterpret_cast<void *>(&value), ptr, sizeof(value));
  return value;
}
struct PvzOffset {
  const char *name;
  off_t offset;
};
PvzOffset *__getOffset(const char *);
off_t getOffset(const char *);

#define PVZ_CORE_LIB "libpvz.so"
#define PVZ_CORE_LIB_HELPER "Java_com_popcap_pvz_1na_PvZActivity_onPVZCreate"
#define PVZ_CORE_LIB_BSS_MEM_OFF 0xc5e000
void *__getBase();
void *__getStatus();
void *incr(void *, long);
void *incrFrom(void *, const char *);
void *incrFromBase(const char *);
void *incrFromStatus(const char *);
#define implGet(name, type)                                                    \
  inline type get##name(void *ptr, type &value) {                              \
    return readMem<type>(ptr, value);                                          \
  }                                                                            \
  inline type get##name(void *ptr) {                                           \
    static type buf;                                                           \
    return get##name(ptr, buf);                                                \
  }
#define implSet(name, type)                                                    \
  inline void set##name(void *ptr, type &value) {                              \
    writeMem<type>(ptr, value);                                                \
  }
#define implGetSet(name, type) implGet(name, type) implSet(name, type)
implGetSet(I32, int32_t);
implGetSet(U32, uint32_t);
implGetSet(F32, float);
implGetSet(Ptr, void *);
#undef implGet
#undef implSet
#undef implGetset
#endif // INC_SERVER_PVZ_H
