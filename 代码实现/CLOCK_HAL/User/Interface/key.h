#ifndef _key_h__
#define _key_h__

#include "gpio.h"
#include "common_test.h"
#include "FreeRTOS.h"
#include "task.h"

typedef enum
{
    KEY_NONE = 0,
    KEY_TIME_SET,       // 短按
    KEY_TIME_SET_LONG,  // 长按
    KEY_UP,             // 上调
    KEY_DOWN,           // 下调
    KEY_ALARM_SET,      // 时钟设置短按
    KEY_ALARM_SET_LONG, // 时钟设置长按
    KEY_ALARM_EN,       // 闹钟使能
    KEY_ALARM_5,        // 5天闹钟
} KEY_type_value;


typedef enum
{
    LED_OFF = 0,
    LED_ON,
} LED_ON_type_value;

typedef enum
{
    LIGHT_OFF = 0,
    LIGHT_ON,
} LIGHT_type_value;

/**
 * @brief  读取当前按下的按键值，只返回最先按下的那个
 * @retval 按键对应的枚举，或者NONE
 */
KEY_type_value KEY_Scan(void);  


/**
 * @brief  读取当前LED灯板的状态，拨动开关的状态
 * @retval LED_ON或者LED_OFF
 */
LED_ON_type_value LED_ON_Scan(void);


/**
 * @brief  读取当前灯光的模式，是声控还是常量
 * @retval LIGHT_ON或者LIGHT_OFF
 */
LIGHT_type_value LIGHT_Scan(void);

#endif
