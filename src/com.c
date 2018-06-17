/*
 * File    : src/com.c
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-06-17
 * Module  :
 * License : MIT
 */
#include "../inc/com.h"
#define OPTION(name, command, description, server_attr, user_attr)             \
  { 0, name, #command, description, command, server_attr, user_attr }

struct pvz_option pvz_options[] = {
    OPTION("改阳光", setSun, NULL, SERVER_GETV, USER_GETINT),

    OPTION("免费植物", freePlants, NULL, SERVER_NOT_INGAME, USER_DONOTHING),

    OPTION("黄油糊脸", coverZombies, NULL, SERVER_DONOTHING, USER_DONOTHING),

    OPTION("搭梯", putLadder,
           "要将梯子僵尸放于何列?\n例如:1.2,1.3(行与列以英文句号分隔,"
           "多个行列以英文逗号分隔):",
           SERVER_NEED_ZOMBIES, USER_GETSTRING | USER_GETCOLROW),

    OPTION("炸荷叶烂南瓜", fuck_LilyPad_Pumpkin,
           "要去除何处的莲叶或破坏何处的南瓜?(行与列以英文句号分隔,"
           "多个行列以英文逗号分隔):",
           SERVER_DONOTHING, USER_GETSTRING | USER_GETCOLROW),

    OPTION("输出调试信息", NULL, NULL, SERVER_DONOTHING, USER_INFO),

    OPTION("退出", NULL, NULL, SERVER_DONOTHING, USER_EXIT),
};
unsigned getOptionsLength() { return ARRAY_SIZE(pvz_options); }

void doInitOptions() {
  struct pvz_option *option;
  for (unsigned idx = 0; idx < getOptionsLength(); ++idx) {
    option = &pvz_options[idx];
    option->id = idx;
  }
}

struct pvz_option *getOption(unsigned id) {
  struct pvz_option *option;
  for (unsigned idx = 0; idx < getOptionsLength(); ++idx) {
    option = &pvz_options[idx];
    if (option->id == id)
      return option;
  }
  return NULL;
}
