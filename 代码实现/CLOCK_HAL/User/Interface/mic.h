#ifndef _MIC_H__
#define _MIC_H__

#include "gpio.h"


typedef enum
{
    MIC_None = 0,
    MIC_up,
}MIC_type_value;


/**
 * @brief  读取当前麦克风的状态，是否被触发
 * @retval MIC_None或者MIC_up
 */
MIC_type_value MIC_Scan(void);


#endif
