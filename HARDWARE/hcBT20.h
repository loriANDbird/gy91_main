#ifndef _HCBT20_H_
#define _HCBT20_H_
#include "stm32f10x.h"

//***********************************************************
//蓝牙硬件接口宏定义
//修改蓝牙接口时只需修改下列宏定义
//修改串口时注意.c文件中串口时钟使能函数和中断服务函数
//***********************************************************
//发送间隔时间
#define BT_DELAY		50	//ms

//疫情型号
#define BT_DEVICE		"GY91"
#define BT_MODULE		"BT1"

//版本号
#define BT_SOFTVER		"V1.00"
#define BT_HARDVER		"V1.00"

//串口
#define BT_UART				USART1
#define BT_UART_RCC			RCC_APB2Periph_USART1//RCC_APB1Periph_USART2
#define BT_UART_IRQ			USART1_IRQn
#define BT_UART_BAUD		115200
//IO
#define BT_UART_PIN_RCC		RCC_APB2Periph_GPIOA
#define BT_UART_PIN_TX		GPIO_Pin_9
#define BT_UART_PIN_RX		GPIO_Pin_10
#define BT_UART_PORT		GPIOA
//DMA
#define BT_DMA_RCC			RCC_AHBPeriph_DMA1
#define BT_DMA_CH_TX		DMA1_Channel4
#define BT_DMA_CH_RX		DMA1_Channel5
#define BT_DMA_IT_TC_TX		DMA1_IT_TC4
#define BT_DMA_IT_TC_RX		DMA1_IT_TC5
//电源
#define BT_PWR_RCC			RCC_APB2Periph_GPIOE
#define BT_PWR_PIN			GPIO_Pin_2
#define BT_PWR_PORT			GPIOE
//片选
#define BT_CS_RCC			RCC_APB2Periph_GPIOC
#define BT_CS_PIN			GPIO_Pin_7
#define BT_CS_PORT			GPIOC
//模式
#define BT_MODE_RCC			RCC_APB2Periph_GPIOC
#define BT_MODE_PIN			GPIO_Pin_6
#define BT_MODE_PORT		GPIOC
//连接状态
#define BT_LINK_RCC			RCC_APB2Periph_GPIOA
#define BT_LINK_PIN			GPIO_Pin_8
#define BT_LINK_PORT		GPIOA

//蓝牙控制
#define BT_PowerOn()		GPIO_ResetBits(BT_PWR_PORT,BT_PWR_PIN)
#define BT_PowerOff()		GPIO_SetBits(BT_PWR_PORT,BT_PWR_PIN)

#define BT_Enable()			GPIO_ResetBits(BT_CS_PORT,BT_CS_PIN)
#define BT_Disable()		GPIO_SetBits(BT_CS_PORT,BT_CS_PIN)

#define BT_ModeATOn()		GPIO_SetBits(BT_MODE_PORT,BT_MODE_PIN)
#define BT_ModeATOff()		GPIO_ResetBits(BT_MODE_PORT,BT_MODE_PIN)

#define BT_LinkStaRead()	GPIO_ReadInputDataBit(BT_LINK_PORT,BT_LINK_PIN)


//外部调用函数
void BT_Init(void);
void BT_Lookup(void);

u8 BT_GetLinkSta(void);
u8 Bt_GetName(char* BtName);
u8 BT_SetName(char* BtName);
void BT_SetDefault(void);
	
void BT_RealSendFile(const char *pFileName);

u8 BT_IsRecvTXCmd(void);
#endif
















