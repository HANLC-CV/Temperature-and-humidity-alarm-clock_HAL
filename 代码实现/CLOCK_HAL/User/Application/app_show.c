#include "app_show.h"

uint8_t clock_flag = 0; //时钟的点要不要亮。
uint32_t clock_last_time = 0;
extern Show_Time_Set_Type show_time_set;
extern Show_Alarm_Set_Type show_alarm_set;
extern alarm_dataTime_t alarm_dataTime1;
extern alarm_dataTime_t alarm_dataTime2;
/**
 * @brief 启动led灯显示
 */
void led_show_start(void)
{
    led_start();
    //最开始时间值，用于后边1s一次的判断
    clock_last_time=xTaskGetTickCount();
}


/**
 * @brief led灯正常状态的温湿度展示
 */
void led_show_normal(app_dataTime_t *dataTime,int8_t temp,int8_t hum,led_show_select *led_type_select)
{
    //1.获取真实时间
    uint8_t hour =0;
    if(dataTime->flag_12==1&&dataTime->flag_pm==1)
    {
        hour = dataTime->hour+12;
    }
    else
    {
        hour = dataTime->hour;
    }
    //2.设置展示时间,我要不要已12制展示
    if(led_type_select->flag_12==1)
    {
        if(hour>12)
        {
            hour = hour-12;
            led_type_select->flag_pm = 1;
        }
    }


    led_set_all(hour,dataTime->minute,clock_flag,temp,hum,led_type_select);
    //没过一秒切换一次clock_flag
    if(xTaskGetTickCount()-clock_last_time>500)
    {
        clock_flag = !clock_flag;
        clock_last_time=xTaskGetTickCount();
    }

}


/**
 * @brief led灯时间设置展示
 */
void led_show_time_set(app_dataTime_t *dataTime,int8_t temp,int8_t hum,led_show_select *led_type_select)
{
    //判断当前处于的页面然后调用方法void led_time_set(Show_Time_Set_Type time_set_type,uint16_t value)
    //1.当前处于的页面
    if(xTaskGetTickCount()-clock_last_time>500)
    {
        clock_flag = !clock_flag;
        clock_last_time=xTaskGetTickCount();
    }
    if(show_time_set==TIME_SET_YEAR)
    {
        if(clock_flag)
        {
            led_set_all(dataTime->year/100,dataTime->year%100,0 ,temp,hum,led_type_select);
        }
        else
        {
            led_set_all(dataTime->year/100,0xff,0 ,temp,hum,led_type_select);
        }
        //led_time_set(TIME_SET_YEAR,dataTime->year);
    }
    else if(show_time_set==TIME_SET_MONTH)
    {
        //led_time_set(TIME_SET_MONTH,dataTime->month);
        if(clock_flag)
        {
            led_set_all(dataTime->month,0xff,0 ,temp,hum,led_type_select);
        }
        else
        {
            led_set_all(0xff,0xff,0 ,temp,hum,led_type_select);
        }
    }
    else if(show_time_set==TIME_SET_DAY)
    {
        //led_time_set(TIME_SET_DAY,dataTime->day);
        if(clock_flag)
        {
            led_set_all(0xff,dataTime->day,0 ,temp,hum,led_type_select);
        }
        else
        {
            led_set_all(0xff,0xff,0 ,temp,hum,led_type_select);
        }
    }
    else if(show_time_set==TIME_SET_HOUR)
    {
        //led_time_set(TIME_SET_HOUR,dataTime->hour);
        if(clock_flag)
        {
            led_set_all(dataTime->hour,0xff,1 ,temp,hum,led_type_select);
        }
        else
        {
            led_set_all(0xff,0xff,1 ,temp,hum,led_type_select);
        }
    }
    else if(show_time_set==TIME_SET_MINUTE)
    {
        //led_time_set(TIME_SET_MINUTE,dataTime->minute);
        if(clock_flag)
        {
            led_set_all(0xff,dataTime->minute,1 ,temp,hum,led_type_select);
        }
        else
        {
            led_set_all(0xff,0xff,1 ,temp,hum,led_type_select);
        }
    }

    //后续正常显示温湿度页面
}


/**
 * @brief led闹钟设置界面展示
 */

 void led_show_alarm_set(app_dataTime_t *dataTime,int8_t temp,int8_t hum,led_show_select *led_type_select)
 { 
    if(xTaskGetTickCount()-clock_last_time>500)
    {
        clock_flag = !clock_flag;
        clock_last_time=xTaskGetTickCount();
    }
    if(show_alarm_set==ALARM_SET_HOUR)
    {
        if(clock_flag)
        {
            if(led_type_select->is_alarm_1==1)
            {
                led_set_all(alarm_dataTime1.hour,0xff,1 ,temp,hum,led_type_select);
            }
            else if(led_type_select->is_alarm_2==1)
            {
                led_set_all(alarm_dataTime2.hour,0xff,1 ,temp,hum,led_type_select);
            }
        }
        else
        {
            led_set_all(0xff,0xff,1 ,temp,hum,led_type_select);
        }
    }
    else if(show_alarm_set==ALARM_SET_MINUTE)
    { 
        if(clock_flag)
        {
            if(led_type_select->is_alarm_1==1)
            {
                led_set_all(alarm_dataTime1.minute,0xff,1 ,temp,hum,led_type_select);
            }
            else if(led_type_select->is_alarm_2==1)
            {
                led_set_all(alarm_dataTime2.minute,0xff,1 ,temp,hum,led_type_select);
            }
        }
        else
        {
            led_set_all(0xff,0xff,1 ,temp,hum,led_type_select);
        }
    }
 }
