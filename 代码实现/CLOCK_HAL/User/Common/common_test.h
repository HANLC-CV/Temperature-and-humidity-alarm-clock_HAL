#ifndef _common_test_h_
#define _common_test_h_


#include "usart.h"
#include "stdio.h"
#include "stdarg.h"


//1.设置一个日志打印开关，用于调试,如要关闭注释掉#define debug_LOG_ON 1即可。
#define debug_LOG_ON 1
#ifdef debug_LOG_ON
//2.日志同时打印文件位置和行号
#define debug_print(fmt, ...) printf("[%s:%d] "fmt"\r\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define debug_print(fmt, ...)
#endif

#endif
