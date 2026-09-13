#include "app_freeRTOS.h"

//采集任务 ms级别延时 
void task_collect(void*pvParments);
#define TASK_COLLECT_STACK_SIZE 128
#define TASK_COLLECT_PRIORITY 1
TaskHandle_t task_collect_handle;

int8_t temperature;
int8_t humidity;
app_dataTime_t dataTime={0};

//展示类任务
void task_show(void*pvParments);
#define TASK_SHOW_STACK_SIZE 128
#define TASK_SHOW_PRIORITY 2
TaskHandle_t task_show_handle;

//led灯展示模式
led_show_type_t led_show_mode=LED_SHOW_MODE_NORMAL;

//时间设置时，当前设置的是什么
Show_Time_Set_Type show_time_set=TIME_SET_YEAR;

//时钟设置，当前设置的是什么
Show_Alarm_Set_Type show_alarm_set=ALARM_SET_HOUR;
alarm_dataTime_t alarm_dataTime1={0};
alarm_dataTime_t alarm_dataTime2={0};

//标识12/24h  AM/PM  华氏度/摄氏度    闹钟1/2   充电标识   5天闹钟标识
led_show_select led_type_select={.flag_12=0,.flag_pm=0,.is_alarm_1=0,.is_alarm_2=0,.is_alarm_five_day=0,.is_celsius=1,.is_charging=0};


//触发类任务
void task_switch(void*pvParments);
#define TASK_SWITCH_STACK_SIZE 128
#define TASK_SWITCH_PRIORITY 3
TaskHandle_t task_switch_handle;

//LED灯的总开关
uint8_t led_en=1;


//闹钟任务
void task_alarm(void*pvParments);
#define TASK_ALARM_STACK_SIZE 128
#define TASK_ALARM_PRIORITY 4
TaskHandle_t task_alarm_handle;

//触摸开关的计数值
uint8_t torch_value_sum=0;


//音量调节任务
void task_volume(void*pvParments);
#define TASK_VOLUME_SIZE 128
#define TASK_VOLUME_PRIORITY 4
TaskHandle_t task_volume_handle;

int8_t nv02d_volume=5;//音量大小参数
/*
@brief  启动freeRTOS
@retval None
*/
void app_freeRTOS_init(void)
{
    //1.创建任务

    xTaskCreate(task_collect,"task_collect",TASK_COLLECT_STACK_SIZE,NULL,TASK_COLLECT_PRIORITY,&task_collect_handle);

    xTaskCreate(task_show,"task_show",TASK_SHOW_STACK_SIZE,NULL,TASK_SHOW_PRIORITY,&task_show_handle);

    xTaskCreate(task_switch,"task_switch",TASK_SWITCH_STACK_SIZE,NULL,TASK_SWITCH_PRIORITY,&task_switch_handle);

    xTaskCreate(task_alarm,"task_alarm",TASK_ALARM_STACK_SIZE,NULL,TASK_ALARM_PRIORITY,&task_alarm_handle);

    xTaskCreate(task_volume,"task_volume",TASK_VOLUME_SIZE,NULL,TASK_VOLUME_PRIORITY,&task_volume_handle);
    //2.启动调度器
    vTaskStartScheduler();
}


void task_collect(void*pvParments)
{
    //初始化
    DHT11_Init();
    uint8_t collect_count_num=9;
    while(1)
    {
        //周期读取温度
        collect_count_num++;
        if(collect_count_num>10)
        {
            DHT11_ReadData(&temperature, &humidity);
            collect_count_num=0;
        }
        //读取时间
        if(led_show_mode!=LED_SHOW_MODE_TIME_SET)
        {
            app_dataTime_get(&dataTime);
        }
        vTaskDelay(1000);
    }
}

void task_show(void*pvParments)
{
    led_show_start();
    while(1)
    {
        if(led_en==1)
        {
            if(led_show_mode==LED_SHOW_MODE_NORMAL||led_show_mode==LED_SHOW_MODE_VOLUME_SET)
            {
                led_show_normal(&dataTime,temperature,humidity,&led_type_select);
            }
            else if(led_show_mode==LED_SHOW_MODE_TIME_SET)
            {
                led_show_time_set(&dataTime,temperature,humidity,&led_type_select);
            }
            else if(led_show_mode==LED_SHOW_MODE_TIME_SET)
            {
                led_show_alarm_set(&dataTime,temperature,humidity,&led_type_select);
            }
        }
        else
        {
            led_stop();
        }
        
    }
}

void task_switch(void*pvParments)
{

    while(1)
    {
        //根据不同模式，选择不同的按键开关逻辑
        if(led_show_mode==LED_SHOW_MODE_NORMAL)
        {
            key_switch_process();
        }
        else if(led_show_mode==LED_SHOW_MODE_TIME_SET)
        {
            key_time_set_process();
        }
        
        //拨动开关
        key_rotary_process();

        //触摸开关
        TORCH_type_value torch_value=TORCH_Scan();
        if(torch_value==TORCH_up)
        {
            torch_value_sum++;
            if(torch_value_sum>=5)
            {
             torch_value_sum=0;
            }
        }
        vTaskDelay(50);
    }
}

void task_alarm(void*pvParments)
{
    NV020D_Init();
    uint8_t conut_alarm=0;
    uint8_t last_torch_value_sum=0;
    while(1)
    {
        //只有正常模式才去响应闹钟
        if(led_type_select.is_alarm_five_day&&dataTime.week>=6)
        {

        }
        else
        {
            if(led_show_mode==LED_SHOW_MODE_NORMAL)
            {
                            //判断是否开启了某个闹钟
                if(led_type_select.is_alarm_1==1)
                {
                    //时间存储和闹钟保存都是24小时制度
                    if(alarm_dataTime1.hour==dataTime.hour&&alarm_dataTime1.minute==dataTime.minute)
                    {
                        if(alarm_dataTime1.flag_on==0)
                        {
                            alarm_dataTime1.flag_on=1;//打开闹钟
                            last_torch_value_sum=torch_value_sum;
                            //播放音乐
                            NV020D_Single_SendCommand(0);
                        }
                        else if(alarm_dataTime1.flag_on==1&&alarm_dataTime1.flag_have_alarm==0)
                        {
                            
                            //1.响了一分钟 或触摸按键，关闭
                            conut_alarm++;
                            if(conut_alarm>=5) //过5s再响一次
                            {
                                NV020D_Single_SendCommand(0);
                            }

                            if(last_torch_value_sum!=torch_value_sum)
                            {
                                NV020D_Single_SendCommand(0XFE);
                                //额外记录闹钟已经响过了
                                alarm_dataTime1.flag_have_alarm=1;
                            }
                        }
                    }
                    else
                    {
                        //出一分钟了关闭闹钟
                        if(alarm_dataTime1.flag_on==1)
                        {
                            alarm_dataTime1.flag_have_alarm=0;
                            alarm_dataTime1.flag_on=0;
                            NV020D_Single_SendCommand(0XFE);
                        }
                        
                    }
                }
                else if(led_type_select.is_alarm_2==1)
                {
                                //时间存储和闹钟保存都是24小时制度
                    if(alarm_dataTime2.hour==dataTime.hour&&alarm_dataTime2.minute==dataTime.minute)
                    {
                        if(alarm_dataTime2.flag_on==0)
                        {
                            alarm_dataTime2.flag_on=1;//打开闹钟
                            last_torch_value_sum=torch_value_sum;
                            //播放音乐
                            NV020D_Single_SendCommand(0);
                        }
                        else if(alarm_dataTime2.flag_on==1&&alarm_dataTime2.flag_have_alarm==0)
                        {
                            
                            //1.响了一分钟 或触摸按键，关闭
                            conut_alarm++;
                            if(conut_alarm>=5) //过5s再响一次
                            {
                                NV020D_Single_SendCommand(0);
                            }

                            if(last_torch_value_sum!=torch_value_sum)
                            {
                                NV020D_Single_SendCommand(0XFE);
                                //额外记录闹钟已经响过了
                                alarm_dataTime2.flag_have_alarm=1;
                            }
                        }
                    }
                    else
                    {
                        //出一分钟了关闭闹钟
                        if(alarm_dataTime2.flag_on==1)
                        {
                            alarm_dataTime2.flag_have_alarm=0;
                            alarm_dataTime2.flag_on=0;
                            NV020D_SendCommand(0XFE);
                        }
                        
                    }
                }
            }
        }

        vTaskDelay(1000);
    }
}

void task_volume(void*pvParments)
{
    NV020D_Init();
    NV020D_Single_SendCommand(0xe0+nv02d_volume);
    uint8_t count_volume=99;
    while(1)
    {
        if(led_show_mode==LED_SHOW_MODE_VOLUME_SET)
        {   
            count_volume++;
            if(count_volume>=100)
            {
                //循环播放音频
                NV020D_Single_SendCommand(0);
                count_volume=0;
            }
        KEY_type_value key_value=KEY_Scan();
        if(key_value==KEY_UP)
        {
            nv02d_volume++;
            if(nv02d_volume>7)
            {
                nv02d_volume=7;
            }
            NV020D_Single_SendCommand(0xe0+nv02d_volume);
        }
        else if(key_value==KEY_DOWN)
        {
            nv02d_volume--;
            if(nv02d_volume<0)
            {
                nv02d_volume=0;
            }
            NV020D_Single_SendCommand(0xe0+nv02d_volume);
        }
        else if(key_value==KEY_ALARM_SET)
        {
            NV020D_Single_SendCommand(0xfe);
            led_show_mode=LED_SHOW_MODE_NORMAL;
            count_volume=99;
        }
        }
        vTaskDelay(50);
    }
}
