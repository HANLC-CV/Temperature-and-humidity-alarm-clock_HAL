#ifndef __NV020D_H__
#define __NV020D_H__


#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"


#define NVD_CLK_H HAL_GPIO_WritePin(NVD_CLK_GPIO_Port,NVD_CLK_Pin,GPIO_PIN_SET)
#define NVD_CLK_L HAL_GPIO_WritePin(NVD_CLK_GPIO_Port,NVD_CLK_Pin,GPIO_PIN_RESET)  

#define NVD_SDA_H HAL_GPIO_WritePin(NVD_SDA_GPIO_Port,NVD_SDA_Pin,GPIO_PIN_SET)
#define NVD_SDA_L HAL_GPIO_WritePin(NVD_SDA_GPIO_Port,NVD_SDA_Pin,GPIO_PIN_RESET)  

#define NVD_BUSY_READ  HAL_GPIO_ReadPin(NVD_BUSY_GPIO_Port,NVD_BUSY_Pin)


#define NVD_SINGLE_CLK_H HAL_GPIO_WritePin(NVD_CLK_GPIO_Port,NVD_CLK_Pin,GPIO_PIN_SET)
#define NVD_SINGLE_CLK_L HAL_GPIO_WritePin(NVD_CLK_GPIO_Port,NVD_CLK_Pin,GPIO_PIN_RESET) 

/**
 * @brief  NV020D 初始化延时，上电后延时500ms
 * 
 */
void NV020D_Init(void);


/***
 * @brief  NV020D 发送指令函数
 * @param  command: 要发送的指令
 */
void NV020D_Single_SendCommand(uint8_t command);



/***
 * @brief  NV020D 发送指令函数
 * @param  command: 要发送的指令
 */
void NV020D_SendCommand(uint8_t command);


/** 
 * @brief  音量调节
 */
void NV020D_Volume(uint8_t volume);

/**
 * @brief  NV020D 播放暂停
 */
void NV020D_PlayPause(void);

#endif
