#ifndef __WTC040_H
#define __WTC040_H
#include "stm32f10x.h"		
#include "main.h"									  	  

//无效回弹
#define SpeakCeQuFullCMD	130

//电池警告
#define SpeakBatteryWarning	120 

//测区完成
#define CeQuEnd     	118 

void Speaker_GPIO_Config(void);

//语音输出，输出n个脉冲则发第n个语音
void SpeakerPulses(unsigned char number);

void SpeakerNum(unsigned char result);

unsigned char SpeakerScan(void);


#endif



































