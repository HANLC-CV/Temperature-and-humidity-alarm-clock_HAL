#ifndef _led_h__
#define _led_h__

#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"


typedef struct   //这里是我想要展示的，虽然我读到的是24小时制，但我想按12小时制展示
{
    uint8_t flag_12;
    uint8_t flag_pm;
    uint8_t is_celsius;//是否是摄氏度
    uint8_t is_alarm_1;
    uint8_t is_alarm_2;
    uint8_t is_alarm_five_day;
    uint8_t is_charging;//是否在充电
}led_show_select;

typedef enum{
    TIME_SET_YEAR=0,
    TIME_SET_MONTH,
    TIME_SET_DAY,
    TIME_SET_HOUR,
    TIME_SET_MINUTE,
    TIME_SET_END,

}Show_Time_Set_Type;

typedef enum
{

    ALARM_SET_HOUR=0,
    ALARM_SET_MINUTE,
    ALARM_SET_END,

}Show_Alarm_Set_Type;

#define LED_CLK_H HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, GPIO_PIN_SET)
#define LED_CLK_L HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, GPIO_PIN_RESET)

#define LED_SDI_H HAL_GPIO_WritePin(LED_SDI_GPIO_Port, LED_SDI_Pin, GPIO_PIN_SET)
#define LED_SDI_L HAL_GPIO_WritePin(LED_SDI_GPIO_Port, LED_SDI_Pin, GPIO_PIN_RESET)

#define LED_LE_H HAL_GPIO_WritePin(LED_LE_GPIO_Port, LED_LE_Pin, GPIO_PIN_SET)
#define LED_LE_L HAL_GPIO_WritePin(LED_LE_GPIO_Port, LED_LE_Pin, GPIO_PIN_RESET)

#define LED_OEA_H HAL_GPIO_WritePin(LED_OEA_GPIO_Port, LED_OEA_Pin, GPIO_PIN_SET)
#define LED_OEA_L HAL_GPIO_WritePin(LED_OEA_GPIO_Port, LED_OEA_Pin, GPIO_PIN_RESET)

#define LED_OEB_H HAL_GPIO_WritePin(LED_OEB_GPIO_Port, LED_OEB_Pin, GPIO_PIN_SET)
#define LED_OEB_L HAL_GPIO_WritePin(LED_OEB_GPIO_Port, LED_OEB_Pin, GPIO_PIN_RESET)

extern uint16_t led_clk_h_s[3];
extern uint16_t led_clk_h_m[10];
/**
 * @ brief  开启led总开关
 */
void led_start(void);
/**
 * @ brief  关闭led总开关
 */
void led_stop(void);

/**
 * @brief  led写入数据   
 * @param  p00 VCCP00电源控制
 * @param  p01 VCCP01电源控制
 * @param  p02 VCCP02电源控制
 * @param  data 两个驱动芯片的数据
 */
void led_write(uint8_t p00, uint8_t p01, uint8_t p02, uint32_t data);

/**
 * @brief  设置时钟
 * @param  hours 小时
 * @param  minutes 分钟
 */
void led_set_clock(uint8_t hours,uint8_t minutes,uint8_t );


/**
 * @brief  设置全部信息，因为温湿度所用的右侧灯管，在芯片中要一次性写入32位，其中16位是左灯管的时钟信息
 *         正常界面的展示
 * @param  temperature 温度
 */
void led_set_all(uint8_t hours,uint8_t minutes,uint8_t clock_flag ,int8_t temperature,int8_t humidity,led_show_select *led_type_select);

/**
 * @brief  设置时间
 *          时间设置界面的展示
 */        

void led_time_set(Show_Time_Set_Type time_set_type,uint16_t value);
#endif
