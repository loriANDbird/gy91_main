#ifndef __TIMER_H
#define __TIMER_H

#include "Mcu_io.h"

void TIM2_Int_Init(u16 arr,u16 psc);
void Touch_Beep_On(void);
void Touch_Beep_Off(void);
 
void TIM4_Int_Init(u16 arr,u16 psc);
#endif
