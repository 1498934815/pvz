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
#define do_cmd(fmt, ...) doCmd(to_string("%d" fmt, option->id __VA_ARGS__))
#define do_cmd_with_arg(fmt, ...) do_cmd(":" fmt, __VA_ARGS__)
// XXX ','不可少, 什么时候C也有和__VA_OPT__类似的东西
#define sendI(I) do_cmd_with_arg("%d", , I)
#define sendS(S) do_cmd_with_arg("%s", , S)

#endif //__CLIENT__H
