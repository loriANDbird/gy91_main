#ifndef __WTC040_H
#define __WTC040_H
#include "stm32f10x.h"		
#include "main.h"									  	  

//��Ч�ص�
#define SpeakCeQuFullCMD	130

//��ؾ���
#define SpeakBatteryWarning	120 

//�������
#define CeQuEnd     	118 

void Speaker_GPIO_Config(void);

//������������n�������򷢵�n������
void SpeakerPulses(unsigned char number);

void SpeakerNum(unsigned char result);

unsigned char SpeakerScan(void);


#endif



































