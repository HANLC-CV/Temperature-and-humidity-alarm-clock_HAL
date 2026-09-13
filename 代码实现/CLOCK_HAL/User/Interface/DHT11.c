#include "DHT11.h"


uint8_t data[5]={0}; //存放读取的数据

/**
 * @brief  DHT11 延时函数
 * @param  us: 延时的微秒数
 */
void DHT11_DELAY_us(uint32_t us)
{
    uint32_t start = us*8;
    while (start--)
    {
        __NOP();
        // Do nothing, just wait
    }
}   
/**
 * @brief  DHT11上电1s之后才能用
 * @retval None
 */
void DHT11_Init(void)
{
    DHT11_DATA_H; //确保初始是高电平
    vTaskDelay(1000); //延时1s
}

/**
 * @brief  DHT11读取数据函数
 * @param temperature 温度指针
 * @param humidity 湿度指针
 */
void DHT11_ReadData(int8_t *temperature, int8_t *humidity)
{
    //1.发送起始信号====》拉低引脚18-30ms  =20ms
    DHT11_DATA_L;
    vTaskDelay(20); //延时20ms
    DHT11_DATA_H;

    //一下代码应该连续执行，不能被打断====》可以添加临界区
    taskENTER_CRITICAL();
    //2.接受响应信号
    uint32_t count_max=0xfffff;//2s左右
    while(DHT11_DATA_READ==GPIO_PIN_SET&&count_max--){} //等待DHT11拉低引脚
    while(DHT11_DATA_READ==GPIO_PIN_RESET&&count_max--){} //等待83us;
    while(DHT11_DATA_READ==GPIO_PIN_SET&&count_max--){} //等待87us

    if(count_max==0) //超时
    {
        *temperature=0;
        *humidity=0;
        return;
    }

    //3.读取数据
    for(int i=0;i<5;i++)
    {
        data[i]=0;
        for(int j=0;j<8;j++)
        {
            //3.1 首先是54us低电平
            while(DHT11_DATA_READ==GPIO_PIN_SET)//
            {
            }
            DHT11_DELAY_us(40); 
            if(DHT11_DATA_READ==GPIO_PIN_SET) //如果是高电平，说明是1
            {
                data[i]|=(0x01<<(7-i));
                //等待拉低进去进行下一轮读数
                while(DHT11_DATA_READ==GPIO_PIN_SET)//
                {
                }
            }
        }
    }
    //4.校验数据
    uint32_t sum=data[0]+data[1]+data[2]+data[3];
    if((uint8_t)sum==data[4])
    {
        *temperature=data[2];
        *humidity=data[0];
        if(data[3]&0x80) //如果是负数
        {
            *temperature=-*temperature;
        }
    }
    taskEXIT_CRITICAL();
}
