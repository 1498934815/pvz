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
#include <signal.h>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../inc/pvz.h"
#include "../inc/defs.h"
#include "../inc/utils.h"
#include "../inc/base.h"
int initConnection() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(SERVER_PORT);
  sin.sin_addr.s_addr = inet_addr(SERVER_ADDR);
  if (connect(sockfd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
    close(sockfd);
    return 1;
  }
  info.sock = sockfd;
  return 0;
}
void initClientCore() {
  if (initConnection()) {
    printf("也许您已经退出或者并没有启动Pvz?\n");
    exit(-1);
  }
  extern void detectPVZ();
  extern void getRemoteBase();
  detectPVZ();
  getRemoteBase();
}
int getSock() { return info.sock; }
const char *doCmd(const char *cmd) {
  static BufferType rec;
  static BufferType snd;
  memset(snd, 0, sizeof(snd));
  memset(rec, 0, sizeof(rec));
  size_t len = strlen(cmd);
  sprintf(snd, "%zu:%s", len, cmd);
  if (send(getSock(), snd, strlen(snd), 0) == -1 ||
      recv(getSock(), rec, BUFSIZE, 0) <= 0) {
    close(getSock());
    // 重试
    initClientCore();
  } else if (strcmp(rec, UN_INIT) == 0) {
    printf(ERR "您所选的选项'%s' 需要先开始游戏\n", cmd);
  }
  return rec;
}
void getRemoteBase() { parseAddr(doCmd(GETBASE), &info.base); }
void detectPVZ() { parseInt(doCmd(GETPID), &info.pid); }
void *getField() { return info.base; }
void *getStatus() {
  void *v;
  parseAddr(doCmd(GETSTATUS), &v);
  return v;
}
void catchSIGINT() {
  setbuf(stdin, NULL);
  destroy((__list **)&info.task);
  longjmp(env, SETJMP_RET);
}
void registeSigHandle() { signal(SIGINT, catchSIGINT); }

void doClientInit() {
  initClientCore();
  printf("PID:%d 基址:%p\n", info.pid, info.base);
  puts(NOTICE "部分功能的解释见 " HELP_TXT);
  puts(NOTICE "关于进入其他无尽 " CODE_TXT);
}
int main(int argc, char **argv) {
  doClientInit();
  int option;
  BufferType buf;
  registeSigHandle();
  if (setjmp(env) == SETJMP_RET) {
    puts(ERR "无效输入");
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
    puts("12.混乱存档");
    puts("13.冒险跳关");
    puts("14.改金币");
    puts("15.修改第一个卡槽");
    puts("16.过第二周目");
    puts("17.退出");

#define GETOPT(mess, opt)                                                      \
  printf(mess);                                                                \
  if (scanf("%d", &opt) != 1) {                                                \
    PANIC;                                                                     \
  }
    GETOPT("请输入:", option);
#define GETOPT_V(mess) GETOPT(mess, info.val)
#define do_send(fmt, ...) doCmd(to_string("%d:" fmt, option, __VA_ARGS__))
#define sendV() do_send("%d", info.val)
    switch (option) {
    case 1:
    case 8:
    case 13:
    case 14:
      GETOPT_V("更改为?");
      sendV();
      break;
    case 4: {
      printf("要将梯子僵尸放于何列?\n例如:1.2,1.3(行与列以英文句号分隔,"
             "多个行列以英文逗号分隔)");
      setbuf(stdin, NULL);
      if (fgets(buf, sizeof(buf), stdin) == NULL)
        PANIC;
      // 作形式检查
      // 如果失败会引发SIGINT
      parseRowAndCol(buf, &info.task);
      destroy((__list **)&info.task);
      do_send("%s", buf);
    } break;
    case 5: {
      printf("要去除何处的莲叶或破坏何处的南瓜?(行与列以英文句号分隔,"
             "多个行列以英文逗号分隔)");
      setbuf(stdin, NULL);
      if (fgets(buf, sizeof(buf), stdin) == NULL)
        PANIC;
      parseRowAndCol(buf, &info.task);
      destroy((__list **)&info.task);
      do_send("%s", buf);
    } break;
    case 6:
      printf("PID:%d 状态与信息:%p 基址:%p\n", info.pid, getStatus(),
             getField());
      break;
    case 11:
      puts("部分代码见" CODE_TXT);
      GETOPT_V("请输入场景代码:");
      sendV();
      break;
    case 12:
      puts("请进入泳池无尽查看效果 && 请确保game1_{mode}.dat存在");
      puts("见" CODE_TXT "底部的说明");
      GETOPT_V("请输入欲混乱的模式的代码:");
      sendV();
      break;
    case 15:
      GETOPT_V("请输入卡槽代码:");
      sendV();
      break;
    case 17:
      goto out;
    default:
      // FIXME:ugly code
      if (IN_RANGE(option, 1, 17)) {
        doCmd(to_string("%d", option));
      } else {
        puts(ERR "输入错误...");
      }
    }
  }
out:
  close(getSock());
  return 0;
}
#undef GETOPT
#undef GETOPT_V
#undef do_send
#undef sendV
