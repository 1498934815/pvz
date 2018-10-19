/*
 * File    : src/com.c
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-06-17
 * Module  :
 * License : MIT
 */
#include "../inc/pvz.h"
#include "../inc/com.h"
#include "../inc/utils.h"
#define OPTION(name, command, notice, server_attr, user_attr)                  \
  { 0, 0, name, notice, command, server_attr, user_attr }

struct pvz_option pvz_options[] = {
    OPTION("改阳光", setSun, NULL, SERVER_GETINT, USER_GETINT),
    OPTION("改金币", changeCoins, NULL, SERVER_GETINT | SERVER_NOT_INGAME,
           USER_GETINT),
    OPTION("免费植物", freePlants, NULL, SERVER_NOT_INGAME, USER_NULL),
    OPTION("关闭免费植物", shutdownFreePlants, NULL, SERVER_NOT_INGAME,
           USER_NULL),
    OPTION("黄油糊脸", coverZombies, NULL, SERVER_NEED_ZOMBIES, USER_NULL),
    OPTION("只出梯子僵尸", callLadder, NULL, SERVER_NULL, USER_NULL),
    OPTION("只出巨人僵尸", callGargantuar, NULL, SERVER_NULL, USER_NULL),
    OPTION(
        "搭梯", putLadder,
        "要将场上的梯子僵尸移动到何行何列?\n例如:1.2,1.3(行与列以英文句号分隔,"
        "多个行列以英文逗号分隔):",
        SERVER_NEED_ZOMBIES | SERVER_GETCOLROW,
        USER_GETSTRING | USER_GETCOLROW),
    OPTION("炸荷叶烂南瓜", fuck_LilyPad_Pumpkin,
           "要去除何处的莲叶或破坏何处的南瓜?(行与列以英文句号分隔,"
           "多个行列以英文逗号分隔):",
           SERVER_NEED_PLANTS | SERVER_GETCOLROW,
           USER_GETSTRING | USER_GETCOLROW),
    OPTION("阵型压力测试", doLimits, NULL, SERVER_NULL, USER_NULL),
    OPTION("过关", pass, NULL, SERVER_NULL, USER_NULL),
    OPTION("修改当前无尽轮数", setFlags, NULL, SERVER_GETINT, USER_GETINT),
    OPTION("修改当前游戏模式", switchMode, "部分代码见" CODE_TXT "\n",
           SERVER_GETINT, USER_GETINT),
    OPTION("冒险跳关", jump, NULL, SERVER_GETINT | SERVER_NOT_INGAME,
           USER_GETINT),
    OPTION("过第二周目", pass2life, NULL, SERVER_NOT_INGAME, USER_NULL),
    OPTION("修改第一个卡槽", changeCardCode, NULL, SERVER_GETINT, USER_GETINT),
    OPTION("切换游戏场景类型", switchFieldType,
           "场景代码(0-7)见" CODE_TXT "\n请更改后重新进入当前关卡\n",
           SERVER_GETINT, USER_GETINT),
    OPTION("自动收集物品", autoCollect, NULL, SERVER_RUNAS_DAEMON, USER_NULL),
    OPTION("取消自动收集", NULL, NULL, SERVER_NOT_INGAME | SERVER_CANCEL_DAEMON,
           USER_NULL),
    OPTION("触发场上推车", triggerMowers, NULL, SERVER_NULL, USER_NULL),
    OPTION("砸罐子随机效果", randomEffects, NULL, SERVER_RUNAS_DAEMON, USER_NULL),
    OPTION("取消砸罐子随机效果", NULL, NULL, SERVER_CANCEL_DAEMON, USER_NULL),
    OPTION("输出调试信息", NULL, NULL, SERVER_NULL, USER_DEBUGINFO),
    OPTION("退出", NULL, NULL, SERVER_NULL, USER_EXIT),
    OPTION(NULL, NULL, NULL, SERVER_NULL, USER_NULL),
};
unsigned padding = 0;
unsigned leastID = 0;
void doInitOptions(void) {
  unsigned idx = 0;
  const struct pvz_option *prev;
  for_each_option(option) {
    option->id = idx;
    // 一个中文字符相当于三个字节
    // 字宽相当于两个空格
    option->wide = strlen(option->name) / 3 * 2;
    padding = option->wide > padding ? option->wide : padding;
    if (option->server_attr & SERVER_CANCEL_DAEMON)
      // 通过「取消」的id来找到daemon函数
      insert_daemon(&info->daemon, prev->id, option->id, prev->callback);
    ++idx;
    prev = option;
  }
  leastID = idx - 1;
}

struct pvz_option *getOption(unsigned id) {
  for_each_option(option) {
    if (option->id == id)
      return option;
  }
  return NULL;
}
