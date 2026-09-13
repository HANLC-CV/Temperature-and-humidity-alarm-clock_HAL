#include "led.h"


uint16_t led_clk_h_s[3]={0x0000,0x0006,0x005b};
uint16_t led_clk_h_g[10]={0x1f80,0x0300,0x2d80,0x2780,0x3300,0x3680,0x3e80,0x0380,0x3f80,0x3780};

uint16_t led_clk_m_s[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uint16_t led_clk_m_g[10]={0x1f80,0x0300,0x2d80,0x2780,0x3300,0x3680,0x3e80,0x0380,0x3f80,0x3780};

uint16_t led_th_s[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uint16_t led_th_g[10]={0x1f80,0x0300,0x2d80,0x2780,0x3300,0x3680,0x3e80,0x0380,0x3f80,0x3780};

uint16_t led_hu_s[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uint16_t led_hu_g[10]={0x1f80,0x0300,0x2d80,0x2780,0x3300,0x3680,0x3e80,0x0380,0x3f80,0x3780};

extern  uint8_t torch_value_sum;
/**
 * @ brief  开启led总开关
 */
void led_start(void)
{
    HAL_GPIO_WritePin(P04_GPIO_Port, P04_Pin, GPIO_PIN_RESET);
}
/**
 * @ brief  关闭led总开关
 */
void led_stop(void)
{
    LED_OEA_H;
    LED_OEB_H;
}



/**
 * @brief  led写入数据
 * @param  p00 VCCP00电源控制
 * @param  p01 VCCP01电源控制
 * @param  p02 VCCP02电源控制
 * @param  data 两个驱动芯片的数据
 */
void led_write(uint8_t p00, uint8_t p01, uint8_t p02, uint32_t data)
{

    for (uint8_t i = 0; i < 32; i++)
    {
        if(data & (0x80000000>>i)) 
        {
            LED_SDI_H;
        }
        else
        {
            LED_SDI_L;
        }
        LED_CLK_H;
        LED_CLK_L;
    }
    //锁存
    LED_LE_H; 
    LED_LE_L;
    HAL_GPIO_WritePin(VCC_P00_GPIO_Port, VCC_P00_Pin, (GPIO_PinState)p00);
    HAL_GPIO_WritePin(VCC_P01_GPIO_Port, VCC_P01_Pin, (GPIO_PinState)p01);
    HAL_GPIO_WritePin(VCC_P02_GPIO_Port, VCC_P02_Pin, (GPIO_PinState)p02);
    //使能OEAB
    LED_OEA_L;
    LED_OEB_L;
}


/**
 * @brief  设置时钟
 * @param  hours 小时
 * @param  minutes 分钟
 */
void led_set_clock(uint8_t hours,uint8_t minutes,uint8_t clock_flag )
{
    if(clock_flag)
    {
        led_write(0,1,1,led_clk_h_s[hours/10]+led_clk_h_g[hours%10]+0x20);
    }
    else
    {
        led_write(0,1,1,led_clk_h_s[hours/10]+led_clk_h_g[hours%10]);
    }
    vTaskDelay(5);
    led_write(1,0,1,led_clk_m_s[minutes/10]+led_clk_m_g[minutes%10]);
    vTaskDelay(5);
}

/**
 * @brief  设置全部信息，因为温湿度所用的右侧灯管，在芯片中要一次性写入32位，其中16位是左灯管的时钟信息
 *         后增加部分数字不显示功能，用于实现时间设置时的部分数字不展示，同时保持温湿度信息一直展示。
 * @param  temperature 温度
 * @param  humidity 湿度
 * @param  clock_flag  标记时和分中间的点是否亮
 */
void led_set_all(uint8_t hours,uint8_t minutes,uint8_t clock_flag ,int8_t temperature,int8_t humidity,led_show_select *led_type_select)
{

    uint16_t vccp02_data=0x38;

    //1.VCCP00导电时的32位
    uint16_t data_right=led_hu_s[humidity/10]+led_hu_g[humidity%10]+0x4000;//末尾加上符号
    
    uint16_t data_left=0;
    uint16_t am_pm_data=0;
    //1.1添加am-pm
    if(led_type_select->flag_12)
    {
        if(led_type_select->flag_pm)
        {
            am_pm_data=0x8000;
        }
        else
        {
            am_pm_data=0x4000;
        }
    }
    if(clock_flag)
    {
        if(hours>99)
        {
            data_left=0x20+am_pm_data;
        }
        else
        {
            data_left=led_clk_h_s[hours/10]+led_clk_h_g[hours%10]+am_pm_data;
        }
        
    }
    else
    {   
        if(hours>99)
        {
            data_left=am_pm_data;
        }
        else
        {
            data_left=led_clk_h_s[hours/10]+led_clk_h_g[hours%10]+am_pm_data;
        }
    }
    //1.2 添加5天闹钟标识
    if(led_type_select->is_alarm_five_day)
    {
        data_right+=0x8000;
    }
    led_write(0,1,1,(data_right<<16)|data_left);
    vTaskDelay(5-torch_value_sum);


    //2.VCCP01导电时的32位

    //2.0 区分摄氏度和华氏度
    uint16_t temperature_data=0;
    if(led_type_select->is_celsius)
    {
        temperature_data=0x4000;  //末尾的摄氏度和华氏度符号
    }
    else
    {
        temperature_data=0x8000;
    }
        //3.温度默认就是摄氏度
    int8_t temp_now=temperature;
    if(led_type_select->is_celsius==0)
    {
        temp_now = temperature*9/5+32;
    }
    if(temp_now<0)
    {
        temp_now=-temp_now;
        vccp02_data+=0x02;//显示处负号
    }
    if(temp_now>99)
    {
        temp_now-=99;
        vccp02_data+=0x05;//点亮百位
    }
    //2.1 拼接温度
    
    data_right=led_th_s[temp_now/10]+led_th_g[temp_now%10]+temperature_data;//末尾加上符号 目前是摄氏度


    //拼接分钟
    if(minutes>99)
    {
        data_left=0;
    }
    else 
   {
    data_left=led_clk_m_s[minutes/10]+led_clk_m_g[minutes%10]; 
   }
    //2.1 闹钟1和2标识
    uint16_t alarm_data=0;
    if(led_type_select->is_alarm_1)
    {
        alarm_data+=0x4000;
    }
    if(led_type_select->is_alarm_2)
    {
        alarm_data+=0x8000;
    }
    data_left+=alarm_data;



    HAL_GPIO_WritePin(VCC_P00_GPIO_Port, VCC_P00_Pin, GPIO_PIN_SET);
    vTaskDelay(torch_value_sum);
    led_write(1,0,1,(data_right<<16)|data_left);
    vTaskDelay(5-torch_value_sum);
    HAL_GPIO_WritePin(VCC_P01_GPIO_Port, VCC_P01_Pin, GPIO_PIN_SET);
    vTaskDelay(torch_value_sum);

    //3.VCCP02导电时的32位
    led_write(1,1,0,(vccp02_data<<16));
    vTaskDelay(5-torch_value_sum);
    HAL_GPIO_WritePin(VCC_P02_GPIO_Port, VCC_P02_Pin, GPIO_PIN_SET);
    vTaskDelay(torch_value_sum);
    //特殊灯，是否充电
    if(led_type_select->is_charging)
    {
        HAL_GPIO_WritePin(P03_GPIO_Port, P03_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(P03_GPIO_Port, P03_Pin, GPIO_PIN_SET);
    }
}

/**
 * @brief  时间设置时，显示当前界面的数字
            num大于99不亮
 */
void led_set_time_number(uint16_t num1,uint16_t num2,uint8_t point )
{
    uint16_t vcc_p00_data=0;
    uint16_t vcc_p01_data=0;
    uint8_t point_data=0;
    if(point)
    {
        point_data=0x20;
    }
    if(num1>99)
    {
        vcc_p00_data=point_data+0;
    }
    else
    {
        vcc_p00_data=led_clk_h_s[num1/10]+led_clk_h_g[num1%10]+point_data;
    }
    if(num2>99)
    {
        vcc_p01_data=0;
    }
    else
    {
        vcc_p01_data=led_clk_m_s[num2/10]+led_clk_m_g[num2%10]+point_data;
    }
    led_write(0,1,1,vcc_p00_data);
    vTaskDelay(5);
    led_write(1,0,1,vcc_p01_data);
    vTaskDelay(5);
}

uint32_t time_set_last_time=0;
uint8_t led_flag=0;
void led_time_set(Show_Time_Set_Type time_set_type,uint16_t value)
{

        //没过一秒切换一次clock_flag
    if(xTaskGetTickCount()-time_set_last_time>500)
    {
        led_flag = !led_flag;
        time_set_last_time=xTaskGetTickCount();
    }
    switch (time_set_type)
    {
        case TIME_SET_YEAR:
        //显示年份=>可修改 => 不能再采集数据
        //一直显示20  闪烁显示后两位
        if(led_flag)
        {
            led_set_time_number(value/100,value%100,0 ); 
        }
        else
        {
            led_set_time_number(value/100,0xff,0); 
        }
            break;
        case TIME_SET_MONTH:
        //显示月份，在前两位闪烁显示，后两位灭掉
        if(led_flag)
        {
            led_set_time_number(value,0xff,0 ); 
        }
        else
        {
            led_set_time_number(0xff,0xff,0); 
        }
            break;
        case TIME_SET_DAY:
        //显示日期，在后两位两位闪烁显示，前两位灭掉
        if(led_flag)
        {
            led_set_time_number(0xff,value,0 ); 
        }
        else
        {
            led_set_time_number(0xff,0xff,0); 
        }
            break;
        case TIME_SET_HOUR:
        //显示小时，在前两位闪烁显示，后两位灭掉
        if(led_flag)
        {
            led_set_time_number(value,0xff,1 ); 
        }
        else
        {
            led_set_time_number(0xff,0xff,1); 
        }
            break;
        case TIME_SET_MINUTE:
        //显示分钟，在后两位闪烁显示，前两位灭掉
            if(led_flag)
            {
                led_set_time_number(0xff,value,1 ); 
            }
            else
            {
                led_set_time_number(0xff,0xff,1); 
            }    
            break;
        default:
            break;
        
    }
}
