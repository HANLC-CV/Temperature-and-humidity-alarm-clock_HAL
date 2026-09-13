#ifndef _app_freeRTOS_h
#define _app_freeRTOS_h



#include "FreeRTOS.h"
#include "task.h"

#include "DHT11.h"
#include "NV020D.h"
#include "key.h"
#include "torch.h"

#include "app_dataTime.h"
#include "app_show.h"
#include "app_switch.h"


/*
@brief  启动freeRTOS
@retval None
*/
void app_freeRTOS_init(void);

#endif
