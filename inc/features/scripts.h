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

#define LUA_SCRIPTS_PATH "/sdcard/Android/data/com.popcap.pvz_na/scripts/"
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
void waitLuaScript(luaScript *);
extern luaScript luaScripts[64];
#endif // INC_FEATURES_SCRIPTS_H
