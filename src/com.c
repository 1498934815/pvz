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
#define OPTION(name, command, notice, server_attr, user_attr)                  \
  { 0, name, notice, command, server_attr, user_attr }

struct pvz_option pvz_options[] = {
    OPTION("改阳光", setSun, NULL, SERVER_GETINT, USER_GETINT),

    OPTION("改金币", changeCoins, NULL, SERVER_GETINT | SERVER_NOT_INGAME,
           USER_GETINT),

    OPTION("免费植物", freePlants, NULL, SERVER_NOT_INGAME, USER_DONOTHING),

    OPTION("黄油糊脸", coverZombies, NULL, SERVER_NEED_ZOMBIES, USER_DONOTHING),

    OPTION("只出梯子僵尸", callLadder, NULL, SERVER_DONOTHING, USER_DONOTHING),

    OPTION("只出巨人僵尸", callGargantuar, NULL, SERVER_DONOTHING,
           USER_DONOTHING),

    OPTION("搭梯", putLadder,
           "要将梯子僵尸放于何列?\n例如:1.2,1.3(行与列以英文句号分隔,"
           "多个行列以英文逗号分隔):",
           SERVER_NEED_ZOMBIES | SERVER_GETCOLROW,
           USER_GETSTRING | USER_GETCOLROW),

    OPTION("炸荷叶烂南瓜", fuck_LilyPad_Pumpkin,
           "要去除何处的莲叶或破坏何处的南瓜?(行与列以英文句号分隔,"
           "多个行列以英文逗号分隔):",
           SERVER_NEED_PLANTS | SERVER_GETCOLROW,
           USER_GETSTRING | USER_GETCOLROW),

    OPTION("阵型压力测试", doLimits, NULL, SERVER_DONOTHING, USER_DONOTHING),

    OPTION("过关", pass, NULL, SERVER_DONOTHING, USER_DONOTHING),

    OPTION("修改当前无尽轮数", setFlags, NULL, SERVER_GETINT, USER_GETINT),

    OPTION("跳关", switchMode, "部分代码见" CODE_TXT, SERVER_GETINT,
           USER_GETINT),

    OPTION("冒险跳关", jump, NULL, SERVER_GETINT | SERVER_NOT_INGAME,
           USER_GETINT),

    OPTION("过第二周目", pass2life, NULL, SERVER_NOT_INGAME, USER_DONOTHING),

    OPTION("混乱存档", moveSaves,
           "请进入泳池无尽查看效果 && 请确保game1_{mode}.dat存在\n见" CODE_TXT
           "底部的说明",
           SERVER_GETINT | SERVER_NOT_INGAME, USER_GETINT),

    OPTION("修改第一个卡槽", changeCardCode, NULL, SERVER_GETINT, USER_GETINT),

    OPTION("切换场景", switchFieldType,
           "场景代码见" CODE_TXT
           "\n请更改后重新进入当前关卡\n请输入场景类型代码(0-7):",
           SERVER_GETINT, USER_GETINT),

    OPTION("输出调试信息", NULL, NULL, SERVER_DONOTHING, USER_DEBUGINFO),

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
