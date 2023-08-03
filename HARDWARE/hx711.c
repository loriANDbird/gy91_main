#include "stm32f10x.h"
#include "hx711.h"
#include "Mcu_io.h"


#define HX711_CLK_RCC       RCC_APB2Periph_GPIOB  
#define HX711_CLK_PORT      GPIOB 
#define HX711_CLK_PIN       GPIO_Pin_6  

#define HX711_SDA_RCC       RCC_APB2Periph_GPIOB  
#define HX711_SDA_PORT      GPIOB 
#define HX711_SDA_PIN       GPIO_Pin_7

#define HX711_CLK_1()       GPIO_SetBits(HX711_CLK_PORT, HX711_CLK_PIN)
#define HX711_CLK_0()       GPIO_ResetBits(HX711_CLK_PORT, HX711_CLK_PIN)

#define HX711_Read_SDA_IN() GPIO_ReadInputDataBit(HX711_SDA_PORT,HX711_SDA_PIN)

void Hx711_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;     
    RCC_APB2PeriphClockCmd(HX711_CLK_RCC, ENABLE); 	
    GPIO_InitStructure.GPIO_Pin = HX711_CLK_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(HX711_CLK_PORT, &GPIO_InitStructure);    

    RCC_APB2PeriphClockCmd(HX711_SDA_RCC, ENABLE); 
    GPIO_InitStructure.GPIO_Pin = HX711_SDA_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(HX711_SDA_PORT, &GPIO_InitStructure);    		 
}
/*
int Hx711_Read(void)
{
	unsigned  int Count;	
	unsigned char i;
	
	SCK711_L ;
	Count=0;

	if(DAT711 == 0){
	for (i=0;i<24;i++){
	SCK711_H;
	Count=Count<<1;
	SCK711_L;
	if(DAT711) Count++;
	}
	SCK711_H;
	Count=Count ^ 0x800000;
	SCK711_L;
 
	Count = Count >> 7;
	
  }
	return Count;
}
*/
char Hx710_Read(int *ReAD)
{
	unsigned  int Count;	
	unsigned char i;	
	char flag = 0;
	
	HX711_CLK_0() ;
	Count=0;
	if(HX711_Read_SDA_IN() == 0)
	{
		__disable_irq();
		for (i=0;i<24;i++)
		{
			HX711_CLK_1();
			Count=Count<<1;
			HX711_CLK_0();
			if(HX711_Read_SDA_IN()) Count++;
		 }
		HX711_CLK_1();
		Count=Count ^ 0x800000;
		HX711_CLK_0(); 
		Count = Count >> 7;	
		*ReAD = Count;
		flag = 1;
		 __enable_irq();  
  }
	return flag;
}
/*
int Hx712_Read(void)
{
	unsigned  int Count;	
	unsigned char i;

	
	SCK712_L ;
	Count=0;

	if(DAT712 == 0){
	for (i=0;i<24;i++){
	SCK712_H;
	Count=Count<<1;
	SCK712_L;
	if(DAT712) Count++;
	}
	SCK712_H;
	Count=Count ^ 0x800000;
	SCK712_L;
  
	Count = Count >> 7;	
  }
	return Count;
}

int Hx720_Read(void)
{
	unsigned  int Count;	
	unsigned char i;
	
	SCK720_L ;
	Count=0;
    
	if(DAT720 == 0){
	for (i=0;i<24;i++){
	SCK720_H;
	Count=Count<<1;
	SCK720_L;
	if(DAT720) Count++;
	}
	SCK720_H;
	Count=Count ^ 0x800000;
	SCK720_L;
	Count = Count >> 7;	
  }
	return Count;
}
*/

