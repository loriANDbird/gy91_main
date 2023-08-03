#ifndef _POWER_H
#define _POWER_H

#include "Mcu_io.h"


#define OPEN_KEY		0	//��������
#define OPEN_CHARGE		1	//��翪��

#define CHARGE_NO		0	//δ���
#define CHARGE_ING		1	//�����
#define CHARGE_DONE		2	//������

#define POWER_RCC       RCC_APB2Periph_GPIOC  
#define POWER_PORT      GPIOC 
#define POWER_PIN       GPIO_Pin_12

#define CarPOWER_RCC       RCC_APB2Periph_GPIOE  
#define CarPOWER_PORT      GPIOE 
#define CarPOWER_PIN       GPIO_Pin_1

#define USBEN_RCC       RCC_APB2Periph_GPIOC  
#define USBEN_PORT      GPIOC 
#define USBEN_PIN       GPIO_Pin_11

#define CHARGE_RCC       RCC_APB2Periph_GPIOC  
#define CHARGE_PORT      GPIOC 
#define CHARGE_PIN       GPIO_Pin_9
#define CHARGEDONE_PIN   GPIO_Pin_8

#define MainPowerOn()	PCout(12) = 1
#define MainPowerOff()	PCout(12) = 0

#define CarPowerOn()	PEout(1) = 1
#define CarPowerOff()	PEout(1) = 0

#define USB_Enable()	PCout(11) = 0
#define USB_Disable()	PCout(11) = 1

#define ReadPwrKeySta()		PDin(6)//����Դ��

#define ReadChargeSta()		PCin(9)//�����
#define ReadChargeDoneSta()		PCin(8)//������״̬


void Power_Init(void);
void PwrOnFunc(void);
void PwrOffFunc(void);

void CheckOpenType(void);
void CheckChargeStep(void);
u8 CheckChargeSta(void);

#endif


