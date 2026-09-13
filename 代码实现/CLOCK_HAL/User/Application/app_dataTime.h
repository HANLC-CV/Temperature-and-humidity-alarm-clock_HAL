#ifndef _app_dataTime_h__
#define _app_dataTime_h__


//时间获取---应用层
#include "DS1302Z.h"

typedef struct  //这个是从芯片里读到的
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t week;
    int8_t hour;
    int8_t minute;
    uint8_t second;
    uint8_t flag_pm;
    uint8_t flag_12;
} app_dataTime_t;

typedef struct  //这个是从芯片里读到的
{
    int8_t hour;
    int8_t minute;
    uint8_t flag_on;
    uint8_t flag_have_alarm;
} alarm_dataTime_t;
/**
 * @brief 获取时间
*/
void app_dataTime_get(app_dataTime_t* dataTime);

/**
 * @brief 设置时间
*/
void app_dataTime_set(app_dataTime_t* dataTime);



#endif
