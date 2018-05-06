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
    puts("2.免费植物");
    puts("3.黄油糊脸");
    puts("4.搭梯");
    puts("5.炸荷叶烂南瓜");
    puts("6.输出调试信息");
    puts("7.过关");
    puts("8.修改当前无尽轮数");
    puts("9.只出梯子僵尸");
    puts("10.阵型压力测试");
    puts("11.跳关");
    puts("12.退出");

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
    case 4: {
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
    case 5: {
      printf("要去除何处的莲叶或破坏何处的南瓜?(行与列以英文句号分隔,"
             "多个行列以英文逗号分隔");
      setbuf(stdin, NULL);
      if (fgets(buf, sizeof(buf), stdin) == NULL)
        PANIC;
      parseRowAndCol(buf, &info.task);
      destroy((__list **)&info.task, NULL);
      sendV("%s", buf);
    } break;
    case 6:
      printf("PID:%d 状态与信息:%p 基址:%p\n", info.pid, getStatus(),
             getField());
      break;
    case 8:
      GETOPT("更改为?", info.val);
      sendV("%d", info.val);
      break;
    case 11:
      printf(
          "部分代码见https://github.com/ze00/pvz/blob/client/doc/code.txt\n");
      GETOPT("请输入场景代码:", info.val);
      sendV("%d", info.val);
      break;
    case 12:
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
