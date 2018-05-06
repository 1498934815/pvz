/*
 * File    : src/client.c
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-04-12
 * Module  :
 * License : MIT
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <strings.h>
#include "../inc/defs.h"
#include "../inc/utils.h"
#include "../inc/base.h"
#include "../inc/pvz.h"
#include "../inc/client.h"
void doClientInit() {
  initConnection();
  detectPVZ();
  getRemoteBase();
  printf("PID:%d 基址:%p\n", info.pid, info.base);
  printf("注意:请勿使用非1/4/9/16/17的项\n");
}
int main(int argc, char **argv) {
  doClientInit();
  int option;
  BufferType buf;
  registeSigHandle();
  if (setjmp(env) == SETJMP_RET) {
    puts("");
  }
  while (1) {
    puts("1.改阳光");
    puts("2.僵尸秒杀");
    puts("3.卷心菜投手加强");
    puts("4.免费植物");
    puts("5.黄油糊脸");
    puts("6.僵尸加强二倍");
    puts("7.植物血量增加二倍");
    puts("8.植物攻速极限");
    puts("9.搭梯");
    puts("10.炸荷叶烂南瓜");
    puts("11.自动发炮");
    puts("12.不自动发炮");
    puts("13.输出调试信息");
    puts("14.过关");
    puts("15.修改当前无尽轮数");
    puts("16.只出梯子僵尸");
    puts("17.阵型压力测试");
    puts("18.跳关");
    puts("19.退出");

#define GETOPT(mess, opt)                                                      \
  printf(mess);                                                                \
  if (scanf("%d", &opt) != 1) {                                                \
    PANIC;                                                                     \
  }
    GETOPT("请输入:", option);
#define sendV(fmt, ...) doCmd(to_string("%d:" fmt, option, __VA_ARGS__))
    switch (option) {
    case 1:
      GETOPT("更改为?", info.val);
      sendV("%d", info.val);
      break;
    case 9: {
      printf("要将梯子僵尸放于何列?\n例如:1.2,1.3,(行与列以英文句号分隔,"
             "多个行列以英文逗号分隔)");
      setbuf(stdin, NULL);
      if (fgets(buf, sizeof(buf), stdin) == NULL)
        PANIC;
      // 作形式检查
      // 如果失败会引发SIGINT
      parseRowAndCol(buf, &info.task);
      destroy((__list **)&info.task, NULL);
      sendV("%s", buf);
    } break;
    case 10: {
      printf("要去除何处的莲叶或破坏何处的南瓜?(行与列以英文句号分隔,"
             "多个行列以英文逗号分隔");
      setbuf(stdin, NULL);
      if (fgets(buf, sizeof(buf), stdin) == NULL)
        PANIC;
      parseRowAndCol(buf, &info.task);
      destroy((__list **)&info.task, NULL);
      sendV("%s", buf);
    } break;
    case 13:
      printf("PID:%d 状态与信息:%p 基址:%p\n", info.pid, getStatus(),
             getField());
      break;
    case 15:
      GETOPT("更改为?", info.val);
      sendV("%d", info.val);
      break;
    case 18:
      printf("请看https://github.com/ze00/pvz/blob/client/doc/code.txt\n");
      GETOPT("请输入场景代码:", info.val);
      sendV("%d", info.val);
      break;
    case 19:
      goto out;
    default:
      if (IN_RANGE(option, 1, 18)) {
        doCmd(to_string("%d", option));
      } else {
        printf("输入错误...\n");
      }
    }
  }
out:
  close(getSock());
  return 0;
}
#undef GETOPT
