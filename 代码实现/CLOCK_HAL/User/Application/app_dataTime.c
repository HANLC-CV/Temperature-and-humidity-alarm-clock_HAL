#include "app_dataTime.h"


/**
 * @brief 获取时间
*/
void app_dataTime_get(app_dataTime_t* dataTime)
{
    //1.年份
    uint8_t year_data=DS1302Z_ReadByte(DS1302Z_YEAR_ADDR);
    dataTime->year=(year_data>>4)*10+(year_data&0x0F);

    //2.月份
    uint8_t month_data=DS1302Z_ReadByte(DS1302Z_MONTH_ADDR);
    dataTime->month=(month_data>>4)*10+(month_data&0x0F);

    //3.日期
    uint8_t day_data=DS1302Z_ReadByte(DS1302Z_DAY_ADDR);
    dataTime->day=(day_data>>4)*10+(day_data&0x0F);

    //4.week
    uint8_t week_data=DS1302Z_ReadByte(DS1302Z_WEEK_ADDR);
    dataTime->week=(week_data&0x0F);

    //5.时钟
    uint8_t hour_data=DS1302Z_ReadByte(DS1302Z_HOUR_ADDR);
    if(hour_data&0x80)
    {
        dataTime->flag_12=1;
        if(hour_data&0x20)
        {
            dataTime->flag_pm=1;
        }
        else
        {
            dataTime->flag_pm=0;
        }
        if(hour_data&0x10)
        {
            dataTime->hour=10+(hour_data&0x0F);
        }
        else
        {
            dataTime->hour=(hour_data&0x0F);
        }
    }
    else
    {
        dataTime->flag_12=0;
        if(hour_data&0x20)
        {
            dataTime->hour=20+(hour_data&0x0F);
        }
        else if(hour_data&0x10)
        {
            dataTime->hour=10+(hour_data&0x0F);
        }
        else
        {
            dataTime->hour=(hour_data&0x0F);
        }
    }
    //底层一直用24小时制度保存
    if(dataTime->flag_12)
    {
        if(dataTime->flag_pm)
        {
            dataTime->hour+=12;
        }
        dataTime->flag_12=0;
    }
    //6.分钟
    uint8_t minute_data=DS1302Z_ReadByte(DS1302Z_MINUTE_ADDR);
    dataTime->minute=(minute_data>>4)*10+(minute_data&0x0F);

}

/**
 * @brief 设置时间
*/
void app_dataTime_set(app_dataTime_t* dataTime)
{
   //0.讲写保护寄存器置为0
   DS1302Z_WriteByte(DS1302Z_WRITE_PROTECT_ADDR,0x00);
   //1.年份
   uint8_t year_s=(dataTime->year%100)/10;
   uint8_t year_g=dataTime->year%10;
   DS1302Z_WriteByte(DS1302Z_YEAR_ADDR,((year_s<<4)|year_g));
   //2.月份
   uint8_t month_s=dataTime->month/10;
   uint8_t month_g=dataTime->month%10;
   DS1302Z_WriteByte(DS1302Z_MONTH_ADDR,(((month_s&0x01)<<4)|month_g));
   //3.日期
   uint8_t day_s=dataTime->day/10;
   uint8_t day_g=dataTime->day%10;
   DS1302Z_WriteByte(DS1302Z_DAY_ADDR,(((day_s&0x03)<<4)|day_g));
   //4.小时 时间设置使用24小时制度,写入寄存器写12小时制
   if(dataTime->hour>12)
   {
        uint8_t hour_s=(dataTime->hour-12)/10;
        uint8_t hour_g=(dataTime->hour-12)%10;
        DS1302Z_WriteByte(DS1302Z_HOUR_ADDR,(0x80|(0x20)|(hour_s<<4)|(hour_g%10)));
   }
   else
   {
        uint8_t hour_s=dataTime->hour/10;
        uint8_t hour_g=dataTime->hour%10;
        DS1302Z_WriteByte(DS1302Z_HOUR_ADDR,(0x80|(hour_s<<4)|(hour_g%10)));
   }
   //5.分钟
   uint8_t minute_s=dataTime->minute/10;
   uint8_t minute_g=dataTime->minute%10;
   DS1302Z_WriteByte(DS1302Z_MINUTE_ADDR,((minute_s<<4)|minute_g));
   //6.秒
   DS1302Z_WriteByte(DS1302Z_SECOND_ADDR,0x00);
   //7.星期
   uint16_t y=dataTime->year;
   uint8_t c=y/100;
   uint8_t m=dataTime->month;
   if(m==1 || m==2)
   {
       y--;
       m+=12;
   }
   //蔡乐公式
   int16_t w=y+y/4+c/4-2*c+26*(m+1)/10+dataTime->day-1;
   while(w<0)
   {
       w+=7;
   }
   w%=7;
   if(w==0)
   {
       w=7;
   }
   DS1302Z_WriteByte(DS1302Z_WEEK_ADDR,w);
}
