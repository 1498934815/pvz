/*
 * File      : inc/features/scripts.h
 * Project   :
 * Author    : ze00
 * Email     : zerozakiGeek@gmail.com
 * Date      : 2020-05-23
 * Module    :
 * License   : MIT
 */
#ifndef INC_FEATURES_SCRIPTS_H
#define INC_FEATURES_SCRIPTS_H
#include "lua/lua.h"
#include <pthread.h>
enum luaScriptType {
  oneshot = 1,
  continuous = 2,
  dostring = 4,
};
struct luaScript {
  union {
    const char *name;
    const char *dostring;
  };
  enum luaScriptType type;
  lua_State *state;
  pthread_t tid;
  int ret;
};
void loadLuaScripts();
int runLuaScript(luaScript *);
void runLuaScriptOnNewThread(luaScript *);
extern luaScript luaScripts[64];
#endif // INC_FEATURES_SCRIPTS_H
