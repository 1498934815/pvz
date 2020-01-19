/*
 * File    : inc/common/PvzCommon.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#ifndef INC_COMMON_PVZCOMMON_H
#define INC_COMMON_PVZCOMMON_H
#include "common/common.h"
#include <pthread.h>
#include <unistd.h>

enum BuiltinsCommand {
  GETVERSION,
  GETPID,
  GETBASE,
  GETSTATUS,
  GETSAVES,
  GETCORELIB,
  GETWAVE,
  GET_TOTAL_HITPOINT,
  GET_HITPOINT_BOUNDARY,
  GET_TOTAL_FRESH_COUNTDOWN,
  GET_FRESH_COUNTDOWN,
};

enum fieldTypes {
  DAY,
  NIGHT,
  POOL,
  FOG,
  ROOF,
  MOONNIGHT,
  MUSHROOM_GARDEN,
  GARDEN,
};

struct PvzPoint {
  int x;
  int y;
};

class Communicator;
typedef void (*normal_callback)(Communicator *, msgPack *);
typedef void (*object_callback)(Communicator *, void *);
typedef void (*daemon_callback)(Communicator *);

extern const char *zombiesCodesMap[];
extern const char *gamesCodesMap[];
extern const char *cardsCodesMap[];
extern const char *fieldsCodesMap[];
#endif // INC_COMMON_PVZCOMMON_H
