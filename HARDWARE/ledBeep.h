#ifndef __LED_H
#define __LED_H	 
#include "Mcu_io.h"

//////////////////////////////////////////////////////////////////////////////////	 
#define LED_RED		0
#define LED_BLUE	1

#define LED_NUM_ALL		0
#define LED_NUM_1		1
#define LED_NUM_2		2
#define LED_NUM_3		3

#define LED_STA_ON		1
#define LED_STA_OFF		0

#define LED_RCC1	RCC_APB2Periph_GPIOC  
#define LED_PORT1	GPIOC

#define LEDRED3_PIN		GPIO_Pin_0
#define BLUERED3_PIN	GPIO_Pin_13


#define LED_RCC2	RCC_APB2Periph_GPIOE  
#define LED_PORT2	GPIOE

#define LEDRED1_PIN		GPIO_Pin_4
#define LEDRED2_PIN		GPIO_Pin_5
#define BLUERED1_PIN	GPIO_Pin_3
#define BLUERED2_PIN	GPIO_Pin_6


#define LaserRow_RCC	RCC_APB2Periph_GPIOB  
#define LaserRow_PORT	GPIOB 
#define LaserRow_PIN	GPIO_Pin_4
#define LaserCol_PIN	GPIO_Pin_3
	
	
#define BEEP_RCC		RCC_APB2Periph_GPIOA  
#define BEEP_PORT		GPIOA 
#define BEEP_PIN		GPIO_Pin_15
	
	
#define SHOCK_RCC		RCC_APB2Periph_GPIOC  
#define SHOCK_PORT		GPIOC 
#define SHOCK_PIN		GPIO_Pin_10


#define LedRed1On()		PEout(4) = 0
#define LedRed1Off()	PEout(4) = 1

#define LedRed2On()		PEout(5) = 0
#define LedRed2Off()	PEout(5) = 1

#define LedRed3On()		PCout(0) = 0
#define LedRed3Off()	PCout(0) = 1

#define LedBlue1On()	PEout(3) = 0
#define LedBlue1Off()	PEout(3) = 1

#define LedBlue2On()	PEout(6) = 0
#define LedBlue2Off()	PEout(6) = 1

#define LedBlue3On()	PCout(13) = 0
#define LedBlue3Off()	PCout(13) = 1
	
	
#define LaserRowOn()	PBout(4) = 1
#define LaserRowOff()	PBout(4) = 0
#define LaserColOn()	PBout(3) = 1
#define LaserColOff()	PBout(3) = 0
	
	
#define BeepOn()		PAout(15) = 1
#define BeepOff()		PAout(15) = 0


#define ShockOn()		PCout(10) = 1
#define ShockOff()		PCout(10) = 0



void Led_Beep_Init(void);

void LED_Ctrl(u8 clr,u8 num,u8 sta);

void PromptTone_Once(short ms);
void PromptTone_Twice(short ms);

#endif
