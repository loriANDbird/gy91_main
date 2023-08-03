#ifndef UI_SAMPDISP_H
#define UI_SAMPDISP_H

#include "stm32f10x.h"

//显示模式表
struct DISPOPTION{
	u8 id;
	u8* icon1;
	u8* Name1;
	u8* icon2;
	u8* Name2;
};

void ui_Sampdisp_InitDisp(void);
void ui_Sampdisp_Run(void);


#endif

