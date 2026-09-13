#ifndef __app_switch_h__
#define __app_switch_h__

#include "key.h"
#include "led.h"
#include "mic.h"
#include "app_show.h"
#include "app_dataTime.h"

/**
 * @brief 正常显示时的按键处理
 */
void key_switch_process(void);

/**
 * @brief 拨动开关处理
 */
void key_rotary_process(void);


/*
@brief 时间设置时候的按键逻辑
*/
void key_time_set_process(void);


/**
 * @brief 闹钟设置的按键逻辑
 */
void key_alarm_set_process(void);

#endif
