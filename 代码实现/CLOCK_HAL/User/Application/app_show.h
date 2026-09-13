#ifndef _app_show_h__
#define _app_show_h__




#include "led.h"
#include "app_dataTime.h"
#include "FreeRTOS.h"
#include "task.h"

typedef enum
{
    LED_SHOW_MODE_NORMAL,  //正常显示
    LED_SHOW_MODE_TIME_SET, //时间设置显示
    LED_SHOW_MODE_ALARM_SET, //闹钟设置显示
    LED_SHOW_MODE_VOLUME_SET,//音量调节显示
}led_show_type_t;




/**
 * @brief 启动led灯显示
 */
void led_show_start(void);

/**
 * @brief led灯正常状态的温湿度展示
 */
void led_show_normal(app_dataTime_t *dataTime,int8_t temp,int8_t hum,led_show_select *led_type_select);

/**
 * @brief led灯时间设置展示
 */
void led_show_time_set(app_dataTime_t *dataTime,int8_t temp,int8_t hum,led_show_select *led_type_select);

/**
 * @brief led闹钟设置界面展示
 */

 void led_show_alarm_set(app_dataTime_t *dataTime,int8_t temp,int8_t hum,led_show_select *led_type_select);
#endif 
