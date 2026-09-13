#include "DS1302Z.h"


/**
 * @brief  DS1302Z 延时函数
 * @param  us: 延时的微秒数
 */
void DS1302Z_DELAY_us(uint32_t us)
{
    uint32_t start = us*8;
    while (start--)
    {
        __NOP();
        // Do nothing, just wait
    }
}   

/**
 * @brief  DS1302Z 读取一个寄存器的值
 * @param  addr: 寄存器地址
 * @retval 寄存器的值
 */
uint8_t DS1302Z_ReadByte(uint8_t addr)
{
    addr|=(0x01); //确保地址的最低位为1，表示读操作
    uint8_t data = 0;
    //1.初始化引脚
    DS_RST_L; //拉低RST引脚，准备通信
    DS_CLK_L; //拉低CLK引脚，准备通信
    //2.拉高RST引脚，开始通信
    DS_RST_H;
    DS1302Z_DELAY_us(5); //延时5us
    for(int i=0; i<8; i++)
    {
        //3.发送地址
        if(addr & (0x01 << i)) //判断最低位是否为1
        {
            DS_IO_H; //发送1
        }
        else
        {
            DS_IO_L; //发送0
        }
        DS_CLK_H; //拉高CLK引脚，准备发送下一位
        DS1302Z_DELAY_us(1); //延时1us
        DS_CLK_L; //拉低CLK引脚，准备发送下一位
    }
    DS_IO_H; //释放IO引脚，准备读取数据
    for(int i=0; i<8; i++)
    {
        //4.读取数据
        
        if(DS_IO_READ) //读取IO引脚的值
        {
            data |= (0x01 << i); //如果为1，则将对应位设置为1
        }
        if(i<7) //如果不是最后一位，则拉高CLK引脚，准备读取下一位
        {
            DS_CLK_H; //拉高CLK引脚，
            DS_CLK_L; //拉低CLK引脚，准备读取下一位
        }   
    }
    DS_RST_L; //拉低RST引脚，结束通信
    return data; //返回读取到的数据
}


/**
 * @brief  DS1302Z 写入一个寄存器的值
 * @param  addr: 寄存器地址
 * @param  data: 要写入的数据
 * @retval None
 */
void DS1302Z_WriteByte(uint8_t addr, uint8_t data)
{
    addr&=(0xfe); //确保地址的最低位为0，表示写操作
    //1.初始化引脚
    DS_RST_L; //拉低RST引脚，准备通信
    DS_CLK_L; //拉低CLK引脚，准备通信

    //2.拉高RST引脚，开始通信
    DS_RST_H; //拉高RST引脚，开始通信
    DS1302Z_DELAY_us(5); //延时5us

    for(int i=0; i<8; i++)
    {
        //3.发送地址
        if(addr & (0x01 << i)) //判断最低位是否为1
        {
            DS_IO_H; //发送1
        }
        else
        {
            DS_IO_L; //发送0
        }
        DS_CLK_H; //拉高CLK引脚，准备发送下一位
        DS1302Z_DELAY_us(1); //延时1us
        DS_CLK_L; //拉低CLK引脚，准备发送下一位
    }
    //4.发送数据
    for(int i=0; i<8; i++)
    {
        if(data & (0x01 << i)) //判断最低位是否为1
        {
            DS_IO_H; //发送1
        }
        else
        {
            DS_IO_L; //发送0
        }
        DS_CLK_H; //拉高CLK引脚，准备发送下一位
        DS1302Z_DELAY_us(1); //延时1us
        DS_CLK_L; //拉低CLK引脚，准备发送下一位
    }
    DS_RST_L; //拉低RST引脚，结束通信

}
