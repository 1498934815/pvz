/*
 * File    : cheater.c
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2017-08-15
 * Module  :
 * License : MIT
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include "pvz.h"
#include "pvz_offset.h"
#include "cheater.h"
int main(int argc, char **argv) {
  checkRootState();
  int *base = getDynamicBase();
  baseInfo.base = (char *)base;
  baseInfo.heap_base = (char *)getHeapBase();
#ifdef DEBUG
  printf("Dynamic base %p,heap base %p\n",baseInfo.base, baseInfo.heap_base);
#endif
  baseInfo.heap_buf =
      createBuf(baseInfo.heap_end, baseInfo.heap_base, &baseInfo.heap_size);
  int option;
  registeSigHandle();
  if (setjmp(env) == SETJMP_RET) {
    puts("");
  }
  while (1) {
    puts("1.改金币");
    puts("2.僵尸秒杀");
    puts("3.卷心菜投手加强");
    puts("4.无冷却");
    puts("5.黄油糊脸");
    puts("6.僵尸加强二倍");
    puts("7.输出植物地址");
    puts("8.输出僵尸地址");
    puts("9.植物血量增加二倍");
    puts("10.植物攻速增加二倍");
    puts("11.搭梯");
    puts("12.退出");
#define GETOPT(mess,opt) printf(mess);\
    if (scanf("%d", &opt) != 1) {\
      setbuf(stdin, NULL);\
      printf("无效输入\n");\
      raise(SIGINT); \
    }
    GETOPT("请输入:",option);
    switch (option) {
    case 1:
      GETOPT("更改为?",baseInfo.newVal);
      changeCoins();
      break;
    case 2:
      while (1) {
        findZombies(letZombiesFragile);
        usleep(500000);
      }
      break;
    case 3:
      increaseCabbageHurler();
      break;
    case 4:
      removeColdDown();
      break;
    case 5:
      findZombies(coverZombies);
      break;
    case 6:
      findZombies(increaseZombies);
      break;
    case 7:
      findPlants(report);
      break;
    case 8:
      findZombies(report);
      break;
    case 9:
      findPlants(increasePlants);
      break;
    case 10:
      findPlants(increasePlantsAttack);
      break;
    case 11:
      GETOPT("要将梯子僵尸放于何列?",baseInfo.newVal);
      findZombies(putLadder);
      break;
    case 12:
      free(baseInfo.heap_buf);
      return 0;
    default:
      printf("输入错误...\n");
    }
  }
  free(baseInfo.heap_buf);
  return 0;
}
