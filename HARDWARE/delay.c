/*******************************************************************************
功能描述：
    1. 硬件延时函数
    2. deley_us(),  delay_ms()  比较准确。
    
******************************************************************************/
#include "delay.h"

void Sleep_ms(int nms)
{
	int idly,i;
	for(i=0;i<nms;i++){
		for(idly = 0;idly < 10000;idly++);
	}
}

void Sleep_us(int nus)
{
	int idly,i;
	for(i=0;i<nus;i++){
		for(idly = 0;idly < 10;idly++);
	}
}

//void delay_ms(int nms)
//{
//	int idly,i;
//	for(i=0;i<nms;i++){
//		for(idly = 0;idly < 10000;idly++);
//	}
//}

//void delay_us(int nus)
//{
//	int idly,i;
//	for(i=0;i<nus;i++){
//		for(idly = 0;idly < 10;idly++);
//	}
//}

