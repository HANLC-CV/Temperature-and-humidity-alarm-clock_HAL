#include "mic.h"







/**
 * @brief  读取当前麦克风的状态，是否被触发
 * @retval MIC_None或者MIC_up
 */
MIC_type_value MIC_Scan(void)
{
    //读取值为低电平，说明麦克风被触发
    if(HAL_GPIO_ReadPin(MIC_IN_GPIO_Port,MIC_IN_Pin) == GPIO_PIN_RESET)
    {
        return MIC_up;
    }

    return MIC_None;
}
