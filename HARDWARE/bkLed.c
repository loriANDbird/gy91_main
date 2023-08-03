/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：
    1. 
    2. 
    
更新记录：
    1. 2019.01.02 创建
    
*******************************************************************************/
#include "bkLed.h" 
#include "delay.h"

#define BKLED_RCC       RCC_APB2Periph_GPIOB
#define BKLED_PORT      GPIOB 
#define BKLED_PIN       GPIO_Pin_0

#define BKLEDPWM_RCC    RCC_APB1Periph_TIM3
#define BKLEDPWM_TIM	TIM3

//背光IO初始化
void BkLed_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;    

	RCC_APB2PeriphClockCmd(BKLED_RCC, ENABLE); 	

	GPIO_InitStructure.GPIO_Pin = BKLED_PIN;	//LCD_BK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(BKLED_PORT, &GPIO_InitStructure);  
	
    BkLedOff();
	
	//BkLedPWM_Init();
}

//背光PWM初始化
void BkLedPWM_Init(void) 
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  	//定时器3通道3作为PWM输出通道(PB0)。
	GPIO_InitStructure.GPIO_Pin = BKLED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BKLED_PORT, &GPIO_InitStructure);

    RCC_APB1PeriphClockCmd(BKLED_RCC | BKLEDPWM_RCC, ENABLE);
	//配置时钟
    TIM_TimeBaseStructure.TIM_Period = 1000;     //pwm周期
    TIM_TimeBaseStructure.TIM_Prescaler = 3;	//48000000 /（psc+1）  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
    TIM_TimeBaseInit(BKLEDPWM_TIM, &TIM_TimeBaseStructure);

	//配置pwm通道 //
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC3Init(BKLEDPWM_TIM, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(BKLEDPWM_TIM, TIM_OCPreload_Enable);

    TIM_Cmd(BKLEDPWM_TIM, ENABLE);
	
//	SetBkLedPercent(0);
}

//设置背光亮度百分比
void SetBkLedPercent(u16 percent) //percent取值0~100
{
	if(percent > 100)percent = 100;
	TIM_SetCompare3(BKLEDPWM_TIM, percent*10);
}


