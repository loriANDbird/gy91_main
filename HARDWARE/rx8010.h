#ifndef __RX8010_H
#define __RX8010_H

#include "main.h"


typedef struct RX8010SJTIME{
    u8     sec;
    u8     min;
    u8     hour;
    u8     week;
    u8     day;
    u8     month;
    u16    year;
}RX8010SJTIME;

extern RX8010SJTIME g_SysTime;


//对外接口函数
void RX8010_Init(void);
void RX8010_SetTime(RX8010SJTIME* Time);
void RX8010_ReadTime(RX8010SJTIME* Time);
void RX8010_TimeRst(void);
u8 MaxDay(u8 year,u8 month);

#endif
