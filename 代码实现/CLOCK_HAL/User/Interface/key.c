#include "key.h"





/*
 * @brief  获取指定按键是否被按下
 * @retval 按下了返回1，否则返回0
 */
uint8_t key_press(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(10); //消抖
        if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
        {
            while(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1); //延时1ms，防止while循环过快，导致CPU占用过高
            }
            return 1;
        }
    }
    return 0;
}


/**
 * @brief  读取当前按下的按键值，只返回最先按下的那个
 * @retval 按键对应的枚举，或者NONE
 */
KEY_type_value KEY_Scan(void)
{
    //短按====》电平值默认1，按下为0，选择按下后松开生效
    if(key_press(UP_GPIO_Port,UP_Pin))
    {
        return KEY_UP;
    }
    else if(key_press(DOWN_GPIO_Port,DOWN_Pin))
    {
        return KEY_DOWN;
    }
    else if(key_press(ALARM_EN_GPIO_Port,ALARM_EN_Pin))
    {
        return KEY_ALARM_EN;
    }
    else if(key_press(ALARM_5_GPIO_Port,ALARM_5_Pin))
    {
        return KEY_ALARM_5;
    }
    

    //长按
    if(HAL_GPIO_ReadPin(TIME_SET_GPIO_Port,TIME_SET_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(10); //消抖
        if(HAL_GPIO_ReadPin(TIME_SET_GPIO_Port,TIME_SET_Pin) == GPIO_PIN_RESET)
        {
            TickType_t start_time = xTaskGetTickCount(); //获取系统时间
            while(HAL_GPIO_ReadPin(TIME_SET_GPIO_Port,TIME_SET_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1); //延时1ms，防止while循环过快，导致CPU占用过高
            }
            TickType_t end_time = xTaskGetTickCount();
            if((end_time - start_time) >= 3000) //长按时间大于3秒
            {
                return KEY_TIME_SET_LONG;
            }
            else
            {
                return KEY_TIME_SET;    
            }
        }
    }
    else if(HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port,ALARM_SET_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(10); //消抖
        if(HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port,ALARM_SET_Pin) == GPIO_PIN_RESET)
        {
            TickType_t start_time = xTaskGetTickCount();
            while(HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port,ALARM_SET_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1); //延时1ms，防止while循环过快，导致CPU占用过高
            }
            TickType_t end_time = xTaskGetTickCount();
            if((end_time - start_time) >= 3000) //长按时间大
            {
                return KEY_ALARM_SET_LONG;
            }
            else
            {
                return KEY_ALARM_SET;    
            }
        }
    }

    return KEY_NONE;
}


/**
 * @brief  读取当前LED灯板的状态，拨动开关的状态
 * @retval LED_ON或者LED_OFF
 */
LED_ON_type_value LED_ON_Scan(void)
{
    if(HAL_GPIO_ReadPin(LED_ON_GPIO_Port,LED_ON_Pin) == GPIO_PIN_RESET)
    {
        return LED_ON;
    }

    return LED_OFF;
    
}


/**
 * @brief  读取当前灯光的模式，是声控还是常量
 * @retval LIGHT_ON或者LIGHT_OFF
 */
LIGHT_type_value LIGHT_Scan(void)
{
    if(HAL_GPIO_ReadPin(LIGHT_GPIO_Port,LIGHT_Pin) == GPIO_PIN_RESET)
    {
        return LIGHT_ON;
    }

    return LIGHT_OFF;
}
