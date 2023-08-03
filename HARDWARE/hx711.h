#ifndef __HX711_H
#define	__HX711_H

#include "stm32f10x.h"

void Hx711_Init(void);
int Hx711_Read(void);
char Hx710_Read(int *ReAD);
int Hx712_Read(void);
int Hx720_Read(void);

#endif
