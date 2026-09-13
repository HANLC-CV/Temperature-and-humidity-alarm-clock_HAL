#ifndef _DHT11_H__
#define _DHT11_H__


#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"


#define DHT11_DATA_H     HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_SET)
#define DHT11_DATA_L     HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_RESET)
#define DHT11_DATA_READ  HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin)

/**
 * @brief  DHT11上电1s之后才能用
 * @retval None
 */
void DHT11_Init(void);

/**
 * @brief  DHT11读取数据函数
 * @param temperature 温度指针
 * @param humidity 湿度指针
 */
void DHT11_ReadData(int8_t *temperature, int8_t *humidity);

#endif
