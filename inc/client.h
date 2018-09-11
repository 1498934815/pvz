/*
 * File    : inc/client.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-07-31
 * Module  :
 * License : MIT
 */
#ifndef __CLIENT__H
#define __CLIENT__H

#include "../inc/utils.h"
extern const char *doCmd(const char *);
// XXX GNU扩展
// 如果args是个空包，##args前的逗号会被无视
#define do_cmd(fmt, args...) doCmd(to_string("%d" fmt, option->id, ##args))
#define do_cmd_with_arg(fmt, args...) do_cmd(":" fmt, ##args)
#define sendI(I) do_cmd_with_arg("%d", I)
#define sendS(S) do_cmd_with_arg("%s", S)

#endif //__CLIENT__H
