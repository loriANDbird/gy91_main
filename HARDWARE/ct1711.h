#ifndef __CT1711_H
#define __CT1711_H 
#include "Mcu_io.h"   


//IO方向设置
#define SWire_IO_IN()  {GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=8<<16;}
#define SWire_IO_OUT() {GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=3<<16;}

////IO操作函数											   
#define	SWire_DQ_OUT PBout(12) //数据端口	PA0 
#define	SWire_DQ_IN  PBin(12)  //数据端口	PA0 

#define SWire_RCC		RCC_APB2Periph_GPIOB
#define SWire_PORT		GPIOB
#define SWire_PIN		GPIO_Pin_12


void CT1711_Init(void);//初始化CT1711
float CT1711_Get_Temp(void);//获取温度
void CT1711_CvrWait(void);//转换状态,20ms调用一次

#endif















