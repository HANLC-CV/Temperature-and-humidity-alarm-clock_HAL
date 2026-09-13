#ifndef _torch_h__
#define _torch_h__



#include "gpio.h"


typedef enum
{
    TORCH_None = 0,
    TORCH_up,
}TORCH_type_value;


/**
 * @brief  读取当前触摸按键的状态，是否被触摸
 * @retval TORCH_ON或者TORCH_OFF
 */
TORCH_type_value TORCH_Scan(void);








#endif
