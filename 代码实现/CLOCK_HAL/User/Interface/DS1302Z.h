#ifndef _DS1302Z_H__
#define _DS1302Z_H__

//时间日期记录
#include "gpio.h"


#define DS1302Z_SECOND_ADDR 0x80
#define DS1302Z_MINUTE_ADDR 0x82
#define DS1302Z_HOUR_ADDR 0x84
#define DS1302Z_DAY_ADDR 0x86
#define DS1302Z_MONTH_ADDR 0x88
#define DS1302Z_WEEK_ADDR 0x8A
#define DS1302Z_YEAR_ADDR 0x8C

#define DS1302Z_READ_ADDR 0x01
#define DS1302Z_WRITE_ADDR 0x00

#define DS1302Z_WRITE_PROTECT_ADDR 0x8E 

//控制CLK引脚
#define DS_CLK_H HAL_GPIO_WritePin(DS_CLK_GPIO_Port, DS_CLK_Pin, GPIO_PIN_SET)
#define DS_CLK_L HAL_GPIO_WritePin(DS_CLK_GPIO_Port, DS_CLK_Pin, GPIO_PIN_RESET)

//控制IO引脚
#define DS_IO_H HAL_GPIO_WritePin(DS_IO_GPIO_Port, DS_IO_Pin, GPIO_PIN_SET)
#define DS_IO_L HAL_GPIO_WritePin(DS_IO_GPIO_Port, DS_IO_Pin, GPIO_PIN_RESET)

//控制RST引脚
#define DS_RST_H HAL_GPIO_WritePin(DS_RST_GPIO_Port, DS_RST_Pin, GPIO_PIN_SET)
#define DS_RST_L HAL_GPIO_WritePin(DS_RST_GPIO_Port, DS_RST_Pin, GPIO_PIN_RESET)

//读取IO引脚
#define DS_IO_READ HAL_GPIO_ReadPin(DS_IO_GPIO_Port, DS_IO_Pin)

/**
 * @brief  DS1302Z 读取一个寄存器的值
 * @param  addr: 寄存器地址
 * @retval 寄存器的值
 */
uint8_t DS1302Z_ReadByte(uint8_t addr);


/**
 * @brief  DS1302Z 写入一个寄存器的值
 * @param  addr: 寄存器地址
 * @param  data: 要写入的数据
 * @retval None
 */
void DS1302Z_WriteByte(uint8_t addr, uint8_t data);

#endif
