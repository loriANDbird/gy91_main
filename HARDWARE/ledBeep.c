/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：
    1. 
    2. 
    
更新记录：
    1. 2018.03.01 创建
    
*******************************************************************************/
#include "ledBeep.h"
#include "delay.h"

//LED IO初始化
void Led_Beep_Init(void)
{ 
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(BEEP_RCC|SHOCK_RCC|LaserRow_RCC|LED_RCC1|LED_RCC2, ENABLE);	 //使能PB,PE端口时钟
	
	//指示灯
	GPIO_InitStructure.GPIO_Pin = LEDRED1_PIN|BLUERED1_PIN;	 //LED 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(LED_PORT1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LEDRED2_PIN|LEDRED3_PIN|BLUERED2_PIN|BLUERED3_PIN;	//LED 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(LED_PORT2, &GPIO_InitStructure);
	
    //激光灯
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LaserRow_PIN | LaserCol_PIN;	 //LED1-->PB.3 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(LaserRow_PORT, &GPIO_InitStructure);					 //根据设定参数初始化
	
    //Beep
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;				 //蜂鸣器-->PC.6 端口配置
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(BEEP_PORT, &GPIO_InitStructure);					 //根据设定参数初始化
	
	//Shock
    GPIO_InitStructure.GPIO_Pin = SHOCK_PIN;				 //震动马达-->PD.3 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(SHOCK_PORT, &GPIO_InitStructure);
    
	LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
	LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
	
	LaserRowOff();
	LaserColOff();
	BeepOff();
	ShockOff();
}

void LED_Ctrl(u8 clr,u8 num,u8 sta)
{
	if(clr == LED_RED){
		if(num == LED_NUM_ALL){
			if(sta == LED_STA_ON){
				LedRed1On();
				LedRed2On();
				LedRed3On();
			}else{
				LedRed1Off();
				LedRed2Off();
				LedRed3Off();
			}
		}if(num == LED_NUM_1){
			if(sta == LED_STA_ON){
				LedRed1On();
			}else{
				LedRed1Off();
			}
		}else if(num == LED_NUM_2){
			if(sta == LED_STA_ON){
				LedRed2On();
			}else{
				LedRed2Off();
			}
		}else if(num == LED_NUM_3){
			if(sta == LED_STA_ON){
				LedRed3On();
			}else{
				LedRed3Off();
			}
		}else{
			return;
		}
	}else{//蓝色
		if(num == LED_NUM_ALL){
			if(sta == LED_STA_ON){
				LedBlue1On();
				LedBlue2On();
				LedBlue3On();
			}else{
				LedBlue1Off();
				LedBlue2Off();
				LedBlue3Off();
			}
		}if(num == LED_NUM_1){
			if(sta == LED_STA_ON){
				LedBlue1On();
			}else{
				LedBlue1Off();
			}
		}else if(num == LED_NUM_2){
			if(sta == LED_STA_ON){
				LedBlue2On();
			}else{
				LedBlue2Off();
			}
		}else if(num == LED_NUM_3){
			if(sta == LED_STA_ON){
				LedBlue3On();
			}else{
				LedBlue3Off();
			}
		}else{
			return;
		}
	}
}

//提示音，一次
void PromptTone_Once(short ms)
{
	BeepOn();
	ShockOn();
	Sleep_ms(ms);
	BeepOff();
	ShockOff();
}

//提示音，两次
void PromptTone_Twice(short ms)
{
	BeepOn();
	ShockOn();
	Sleep_ms(ms);
	BeepOff();
	ShockOff();
	Sleep_ms(ms);
	BeepOn();
	ShockOn();
	Sleep_ms(ms);
	BeepOff();
	ShockOff();
}


