#include "NV020D.h"




/**
 * @brief  DS1302Z 延时函数
 * @param  us: 延时的微秒数
 */
void NV020D_DELAY_us(uint32_t us)
{
    uint32_t start = us*8;
    while (start--)
    {
        __NOP();
        // Do nothing, just wait
    }
}   


/**
 * @brief  NV020D 初始化延时，上电后延时500ms
 * 
 */
void NV020D_Init(void)
{
    vTaskDelay(500);
}


/**
 * @brief  NV020D 写入一个字节
 * @param  data: 要发送的字节
 */
void NV020D_send_byte(uint8_t data)
{
    //1.初始化
    NVD_CLK_H;
    if(data&0x01)
    {
        NVD_SDA_H;
    }
    else 
    {
        NVD_SDA_L;
    }
    
    //2.发送起始命令
    NVD_CLK_L;
    vTaskDelay(5);

    for(uint8_t i=0;i<8;i++)
    {
        if(data&(0x01<<i))
        {
            NVD_SDA_H;
        }
        else 
        {
            NVD_SDA_L;
        }
        NVD_CLK_L;
        vTaskDelay(1);
        NVD_CLK_H;
        vTaskDelay(1);
    }
    NVD_SDA_H;
}



/***
 * @brief  NV020D 发送指令函数---单线模式
 * @param  command: 要发送的指令
 */
void NV020D_Single_SendCommand(uint8_t command)
{
    NVD_SINGLE_CLK_H;

    NVD_SINGLE_CLK_L;
    vTaskDelay(4);

    for(uint8_t i=0;i<8;i++)
    {
        if(command&(0x01<<i))
        {  //发送1
            NVD_SINGLE_CLK_H;
            //NV020D_DELAY_us(1200);
            vTaskDelay(3);
            NVD_SINGLE_CLK_L;
            //NV020D_DELAY_us(400);
            vTaskDelay(1);
        }
        else
        {   //发送0
            NVD_SINGLE_CLK_H;
            //NV020D_DELAY_us(400);
            vTaskDelay(1);
            NVD_SINGLE_CLK_L;
            //NV020D_DELAY_us(1200);
            vTaskDelay(3);
        }
    }
    NVD_SINGLE_CLK_H;
    vTaskDelay(100);
}


/***
 * @brief  NV020D 发送指令函数
 * @param  command: 要发送的指令
 */
void NV020D_SendCommand(uint8_t command)
{
    while (NVD_BUSY_READ==GPIO_PIN_RESET)
    {
        vTaskDelay(10);
    }
    vTaskDelay(100);
    NV020D_send_byte(0xf1);
    //0-0xdf 是播放语音  
    NV020D_send_byte(command);
    NV020D_send_byte(0xf3);
    uint8_t sum=(uint8_t)(0xf1+command+0xf3);
    NV020D_send_byte(sum);
}

/** 
 * @brief  音量调节
 */
void NV020D_Volume(uint8_t volume)
{
    if(volume>7)
    {
        volume=7;
    }
    NV020D_Single_SendCommand(0xE0+volume);

}

/**
 * @brief  NV020D 播放暂停
 */
void NV020D_PlayPause(void)
{
    NV020D_Single_SendCommand(0xFE);

}
