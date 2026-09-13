#include "app_switch.h"


extern led_show_select led_type_select;
extern uint8_t led_en;
extern Show_Time_Set_Type show_time_set;
extern Show_Alarm_Set_Type show_alarm_set;
extern app_dataTime_t dataTime;
extern alarm_dataTime_t alarm_dataTime1;
extern alarm_dataTime_t alarm_dataTime2;
uint8_t key_clock_count = 0;

uint8_t sound_control_flag=0;//声控标签

uint32_t sound_control_lasttime=0;//上次有人说话时间

extern led_show_type_t led_show_mode;

/**
 * @brief 按键处理
 */
void key_switch_process(void)
{
    //周期性读取按键值
    KEY_type_value key_value = KEY_Scan();

    switch (key_value) {
        case KEY_TIME_SET:
            //按键按下
            led_type_select.flag_12=(led_type_select.flag_12+1)%2; //切换12/24小时制
            break;
        case KEY_TIME_SET_LONG:
            led_show_mode=LED_SHOW_MODE_TIME_SET;//进入时间设置模式
            led_type_select.flag_12=0;
            //按键按下
            break;
        case KEY_UP:
            //按键按下
            led_type_select.is_celsius=(led_type_select.is_celsius+1)%2;
            break;
        case KEY_DOWN:
            //按键按下
            led_show_mode=LED_SHOW_MODE_VOLUME_SET;
            break;
        case KEY_ALARM_SET:
            //按键按下
            led_show_mode=LED_SHOW_MODE_VOLUME_SET;
            break;
        case KEY_ALARM_SET_LONG:
            led_show_mode=LED_SHOW_MODE_ALARM_SET;
            //闹钟设置时候也用24小时制去设置
            led_type_select.flag_12=0;
            led_type_select.is_alarm_1=1;
            led_type_select.is_alarm_2=0;
            break;
        case KEY_ALARM_EN:
            key_clock_count++;
            if(key_clock_count%2==0)
            {
                led_type_select.is_alarm_1=0;
                led_type_select.is_alarm_2=0;
                if(key_clock_count==6)
                {
                    key_clock_count=0;
                }
            }
            else if(key_clock_count==1)
            {
                led_type_select.is_alarm_1=1;
            }
            else if(key_clock_count==3)
            {
                led_type_select.is_alarm_2=1;
            }
            else if(key_clock_count==5)
            {
                led_type_select.is_alarm_1=1;
                led_type_select.is_alarm_2=1;
            }
            break;
        case KEY_ALARM_5:
            led_type_select.is_alarm_five_day=(led_type_select.is_alarm_five_day+1)%2;
            break;
        default:
            break;
    }
}



/**
 * @brief 拨动开关处理
 */
void key_rotary_process(void)
{
    //LIGHT_OFF时为声控模式   ===》》声控开关  一次声控亮5s
    LIGHT_type_value light_type = LIGHT_Scan();
    LED_ON_type_value led_on = LED_ON_Scan();

    if(MIC_Scan()==MIC_up)
    {
        sound_control_flag=1;
        sound_control_lasttime=xTaskGetTickCount();
    }
    
    //先判断总开关
    if(led_on==LED_ON)
    {
        if(light_type==LIGHT_ON)//常量模式
        {
            led_en=1;
        }
        else if(sound_control_flag==1)
        {

                if(xTaskGetTickCount()-sound_control_lasttime<5000)
                {
                    led_en=1;
                    
                }
                else
                {
                    led_en=0;sound_control_flag=0;
                }
            
        }
        else
        {
            led_en=0;
        }
        
    }
    else
    {
        led_en=0;
    }
}

/*
@brief 时间设置时候的按键逻辑
*/
void key_time_set_process(void)
{
    KEY_type_value key_value = KEY_Scan();  
    switch (key_value) {
        case KEY_UP:
        //当前数值加一
        //1.判断当前页面是谁
        if(show_time_set==TIME_SET_YEAR)
        {
            dataTime.year++;
            if(dataTime.year>2099)
            {
                dataTime.year=2000;
            }
        }
        else if(show_time_set==TIME_SET_MONTH)
        {
            dataTime.month++;
            if(dataTime.month>12)
            {
                dataTime.month=1;
            }
        }
        else if(show_time_set==TIME_SET_DAY)
        {
            dataTime.day++;
            if(dataTime.day>31)
            {
                dataTime.day=1;
            }
        }
        else if(show_time_set==TIME_SET_HOUR)
        {
            dataTime.hour++;
            if(dataTime.hour>23)
            {
                dataTime.hour=0;
            }
        }
        else if(show_time_set==TIME_SET_MINUTE)
        {
            dataTime.minute++;
            if(dataTime.minute>59)
            {
                dataTime.minute=0;
            }
        }
            break;
        case KEY_DOWN:
        if(show_time_set==TIME_SET_YEAR)
        {
            dataTime.year--;
            if(dataTime.year<2000)
            {
                dataTime.year=2099;
            }
        }
        else if(show_time_set==TIME_SET_MONTH)
        {
            dataTime.month--;
            if(dataTime.month<1)
            {
                dataTime.month=12;
            }
        }
        else if(show_time_set==TIME_SET_DAY)
        {
            dataTime.day--;
            if(dataTime.day<1)
            {
                dataTime.day=31;
            }
        }
        else if(show_time_set==TIME_SET_HOUR)
        {
            dataTime.hour--;
            if(dataTime.hour<0)
            {
                dataTime.hour=23;
            }
        }
        else if(show_time_set==TIME_SET_MINUTE)
        {
            dataTime.minute--;
            if(dataTime.minute<0)
            {
                dataTime.minute=59;
            }
        }
            //当前数值减一
            break;
        case KEY_TIME_SET:
            //确认当前设置，进入下一个，如果是最后一个就退出
            show_time_set++;
            if(show_time_set==TIME_SET_END)
            {
                led_show_mode=LED_SHOW_MODE_NORMAL;
                dataTime.flag_12=led_type_select.flag_12;
                dataTime.flag_pm=led_type_select.flag_pm;
                app_dataTime_set(&dataTime);
            }
            break;
            
    }
}

/**
 * @brief 闹钟设置的按键逻辑
 */
void key_alarm_set_process(void)
{
    KEY_type_value key_value = KEY_Scan();  
    switch (key_value) {
        case KEY_UP:
            if(show_alarm_set==ALARM_SET_HOUR)
            {
                if(led_type_select.is_alarm_1==1)
                {
                    alarm_dataTime1.hour++;
                    if(alarm_dataTime1.hour>23)
                    {
                        alarm_dataTime1.hour=0;
                    }
                }
                else if(led_type_select.is_alarm_2==1)
                {
                    alarm_dataTime2.hour++;
                    if(alarm_dataTime2.hour>23)
                    {
                        alarm_dataTime2.hour=0;
                    }
                }
            }
            else if(show_alarm_set==ALARM_SET_MINUTE)
            {
                if(led_type_select.is_alarm_1==1)
                {
                    alarm_dataTime1.minute++;
                    if(alarm_dataTime1.minute>59)
                    {
                        alarm_dataTime1.minute=0;
                    }
                }
                else if(led_type_select.is_alarm_2==1)
                {
                    alarm_dataTime2.minute++;
                    if(alarm_dataTime2.minute>59)
                    {
                        alarm_dataTime2.minute=0;
                    }
                }
            }
            break;
        case KEY_DOWN:
        if(show_alarm_set==ALARM_SET_HOUR)
        {   
            if(led_type_select.is_alarm_1==1)
            {
                alarm_dataTime1.hour--;
                if(alarm_dataTime1.hour<0)
                {
                    alarm_dataTime1.hour=23;
                }
            }
            else if(led_type_select.is_alarm_2==1)
            {
                alarm_dataTime2.hour--;
                if(alarm_dataTime2.hour<0)
                {
                    alarm_dataTime2.hour=23;
                }
            }
        }
        else if(show_alarm_set==ALARM_SET_MINUTE)
        {
            if(led_type_select.is_alarm_1==1)
            {
                alarm_dataTime1.minute--;
                if(alarm_dataTime1.minute<0)
                {
                    alarm_dataTime1.minute=59;
                }
            }
            else if(led_type_select.is_alarm_2==1)
            {
                alarm_dataTime2.minute--;
                if(alarm_dataTime2.minute<0)
                {
                    alarm_dataTime2.minute=59;
                }
            }
        }
        case KEY_TIME_SET:
        //确认闹钟设置到下一个设置页面，比如时到分,设置完成知道保存闹钟数据到全局变量
        //闹钟设置完之后要让key_clock_count值对应上闹钟设置
        show_alarm_set++;
        if(show_alarm_set==ALARM_SET_END)
        {
            if(key_clock_count%2==0)
            {
                if(led_type_select.is_alarm_1==1)
                {
                    key_clock_count=1;
                }
                else if(led_type_select.is_alarm_2==1)
                {
                    key_clock_count=3;
                }
            }
            else if(key_clock_count==1)
            {
                if(led_type_select.is_alarm_2==1)
                {
                    key_clock_count=5;
                    led_type_select.is_alarm_1=1;
                }
            }
            else if(key_clock_count==3)
            {
                if(led_type_select.is_alarm_1==1)
                {
                    key_clock_count=5;
                    led_type_select.is_alarm_2=1;
                }
            }

            led_show_mode=LED_SHOW_MODE_NORMAL;
            show_alarm_set=ALARM_SET_HOUR;

        }
        case KEY_ALARM_EN:
        //切换设置的是闹钟1还是2
        if(led_type_select.is_alarm_1==1)
        {
            led_type_select.is_alarm_1=0;
            led_type_select.is_alarm_2=1;
        }
        else
        {
            led_type_select.is_alarm_1=1;
            led_type_select.is_alarm_2=0;

        }
    }
}
