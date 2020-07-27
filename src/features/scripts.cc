/*
 * File      : src/features/scripts.cc
 * Project   :
 * Author    : ze00
 * Email     : zerozakiGeek@gmail.com
 * Date      : 2020-05-23
 * Module    :
 * License   : MIT
 */
#include "features/scripts.h"
#include "common/PvzUtils.h"
#include "common/common.h"
#include "features/features.h"
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"
#include "server/Pvz.h"
#include <dirent.h>
#include <fnmatch.h>
#include <libgen.h>
#include <pthread.h>
#include <string.h>
#include <vector>
#define LUA_NAME(name) lua_##name
#define DEFINE_GET_0(name, rname)                                              \
  static int LUA_NAME(rname)(lua_State * state) {                              \
    int n = lua_gettop(state);                                                 \
    if (n != 0) {                                                              \
      lua_atpanic(state, lua_##rname);                                         \
    }                                                                          \
    lua_pushinteger(state, (lua_Integer)name());                               \
    return 1;                                                                  \
  }
#define DEFINE_GET_1(name, rname)                                              \
  static int LUA_NAME(rname)(lua_State * state) {                              \
    int n = lua_gettop(state);                                                 \
    if (n != 1) {                                                              \
      lua_atpanic(state, lua_##rname);                                         \
    }                                                                          \
    int l = lua_tointeger(state, 1);                                           \
    lua_pushinteger(state, (lua_Integer)name((void *)l));                      \
    return 1;                                                                  \
  }
#define DEFINE_SET_2(name, rname)                                              \
  static int LUA_NAME(rname)(lua_State * state) {                              \
    int n = lua_gettop(state);                                                 \
    if (n != 2) {                                                              \
      lua_atpanic(state, lua_##rname);                                         \
    }                                                                          \
    int l = lua_tointeger(state, 1);                                           \
    int r = lua_tointeger(state, 2);                                           \
    name(l, (void *)r);                                                        \
    return 0;                                                                  \
  }
static void collectObjects(Communicator *_com, void *object) {
  lua_State *state = *(lua_State **)_com;
  int *n = ((int **)_com)[1];
  ++*n;
  lua_pushinteger(state, *n);
  lua_pushinteger(state, (lua_Integer)object);
  lua_settable(state, -3);
}
#define DEFINE_GET_OBJECTS(rname, name)                                        \
  static int LUA_NAME(rname)(lua_State * state) {                              \
    int n = lua_gettop(state);                                                 \
    if (n != 0) {                                                              \
      lua_atpanic(state, lua_##rname);                                         \
    }                                                                          \
    lua_newtable(state);                                                       \
    void *datas[2] = {state, &n};                                              \
    each##name((Communicator *)datas, collectObjects);                         \
    return 1;                                                                  \
  }
int LUA_NAME(setZombiesSeeds)(lua_State *state) {
  int n = lua_gettop(state);
  if (n < 3) {
    lua_atpanic(state, LUA_NAME(setZombiesSeeds));
  }
  // 跳过level、which
  n -= 2;
  int level = lua_tointeger(state, 1);
  int which = lua_tointeger(state, 2);
  if (zombiesSeeds.find(level) == zombiesSeeds.end())
    zombiesSeeds[level] = std::vector<std::vector<int>>{{}, {}, {}};
  zombiesSeeds[level][which] = std::vector<int>();
  for (int i = 0; i < n; ++i) {
    zombiesSeeds[level][which].push_back(lua_tointeger(state, 3 + i));
    DEBUG_LOG("LEVEL %d WHICH %d I %d C %d", level, which, i,
              zombiesSeeds[level][which].back());
  }
  return 0;
}
int LUA_NAME(putGrave)(lua_State *state) {
  int n = lua_gettop(state);
  if (n != 2) {
    lua_atpanic(state, LUA_NAME(putGrave));
  }
  int col = lua_tointeger(state, 1);
  int row = lua_tointeger(state, 2);
  lua_pushinteger(state, (lua_Integer)putGrave(col, row));
  return 1;
}
int LUA_NAME(putPlant)(lua_State *state) {
  int n = lua_gettop(state);
  if (n != 3) {
    lua_atpanic(state, LUA_NAME(putPlant));
  }
  int code = lua_tointeger(state, 1);
  int col = lua_tointeger(state, 2);
  int row = lua_tointeger(state, 3);
  lua_pushinteger(state, (lua_Integer)putPlant(code, col, row));
  return 1;
}
int LUA_NAME(putZombie)(lua_State *state) {
  int n = lua_gettop(state);
  if (n != 3) {
    lua_atpanic(state, LUA_NAME(putZombie));
  }
  int code = lua_tointeger(state, 1);
  int col = lua_tointeger(state, 2);
  int row = lua_tointeger(state, 3);
  putZombie(code, col, row);
  return 0;
}
DEFINE_GET_0(__getCoreLib, getCoreLib)
DEFINE_GET_0(__getStatus, getStatus)
DEFINE_GET_0(__getBase, getBase)
DEFINE_GET_0(__getSaves, getSaves)
DEFINE_GET_1(getI32, getI32)
DEFINE_GET_1(getPtr, getPtr)
DEFINE_SET_2(setI32, setI32)
DEFINE_GET_OBJECTS(getPlants, Plant)
DEFINE_GET_OBJECTS(getZombies, Zombie)
DEFINE_GET_OBJECTS(getItems, Item)
DEFINE_GET_OBJECTS(getProjectiles, Projectile)
DEFINE_GET_OBJECTS(getVases, Vase)

lua_State *initLuaScript() {
  lua_State *state = luaL_newstate();
  luaL_openlibs(state);
#define REG_LUA_FUNCTION(name) lua_register(state, #name, LUA_NAME(name))
  REG_LUA_FUNCTION(getCoreLib);
  REG_LUA_FUNCTION(getBase);
  REG_LUA_FUNCTION(getStatus);
  REG_LUA_FUNCTION(getSaves);
  REG_LUA_FUNCTION(getI32);
  REG_LUA_FUNCTION(getPtr);
  REG_LUA_FUNCTION(getPlants);
  REG_LUA_FUNCTION(getZombies);
  REG_LUA_FUNCTION(getItems);
  REG_LUA_FUNCTION(getProjectiles);
  REG_LUA_FUNCTION(getVases);
  REG_LUA_FUNCTION(setI32);
  REG_LUA_FUNCTION(setZombiesSeeds);
  REG_LUA_FUNCTION(putGrave);
  REG_LUA_FUNCTION(putPlant);
  REG_LUA_FUNCTION(putZombie);
  return state;
}
int runLuaScript(luaScript *luaScript) {
  int ret;
  if (luaScript->type & luaScriptType::dostring)
    ret = luaL_dostring(luaScript->state, luaScript->dostring);
  else
    ret = luaL_dofile(luaScript->state, luaScript->name);
  luaScript->ret = ret;
  if (ret != LUA_OK)
    DEBUG_LOG("FAILED TO RUN %s", luaScript->name);
  luaScript->tid = 0;
  return ret;
}
void runLuaScriptOnNewThread(luaScript *luaScript) {
  pthread_create(&luaScript->tid, nullptr, (void *(*)(void *))runLuaScript,
                 (void *)luaScript);
  pthread_detach(luaScript->tid);
}
void waitLuaScript(luaScript *luaScript) {
  pthread_join(luaScript->tid, nullptr);
}
#define LUA_SCRIPT_MAX 64
luaScript luaScripts[LUA_SCRIPT_MAX] = {};
void loadLuaScripts() {
  DIR *dir = opendir(LUA_SCRIPTS_PATH);
  if (dir == nullptr) {
    DEBUG_LOG("NO SCRIPTS DIRECTORY");
    return;
  }
  struct dirent *ent;
  int count = 0;
  std::vector<pthread_t> threads;
  while ((ent = readdir(dir)) != nullptr) {
    if (ent->d_type & DT_REG && fnmatch("*.lua", ent->d_name, 0) == 0) {
      const char *name = strdup(formatBuffer("%s%s", LUA_SCRIPTS_PATH, ent->d_name));
      luaScripts[count].name = name;
      luaScripts[count].state = initLuaScript();
      if (strncmp(ent->d_name, "C_", 2) == 0) {
        luaScripts[count].type = luaScriptType::continuous;
        runLuaScriptOnNewThread(&luaScripts[count]);
      } else
        luaScripts[count].type = luaScriptType::oneshot;
      // runLuaScript(&luaScripts[count]);
      count++;
      if (count == LUA_SCRIPT_MAX)
        break;
    }
  }
}
