/*******************************************************************************
创 建 者：张喜双
创建日期：2018.08.16
功能描述：
    1. ADC 采集电池电压.
        
更新记录：
    1. 2018.08.16 创建   
******************************************************************************/

#ifndef _BATADC_H_
#define _BATADC_H_

#include "stm32f10x.h"

#define FILTERLEN 40

void Bat_Adc_Init(void);
int Get_Bat_Quantity(void);
void Get_Hard_Version(void);
#endif 

