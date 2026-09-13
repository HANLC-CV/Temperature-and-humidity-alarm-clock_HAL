#include "torch.h"

/**
 * @brief  读取当前触摸按键的状态，是否被触摸
 * @retval TORCH_ON或者TORCH_OFF
 */
TORCH_type_value TORCH_Scan(void)
{
    if(HAL_GPIO_ReadPin(TORCH_GPIO_Port,TORCH_Pin) == GPIO_PIN_RESET)
    {
        return TORCH_up;
    }

    return TORCH_None;

}
