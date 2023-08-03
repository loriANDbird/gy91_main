#ifndef MANAGE_UI_H
#define MANAGE_UI_H

#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ff.h"
#include "exfuns.h"   
#include "mass_mal.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "memory.h"	    
#include "usb_bot.h"

#include "main.h"
#include "mcu_io.h"

#include "KongJian.h"
#include "ui_Public.h"
#include "ui_Dialog.h"
#include "ui_View.h"
#include "ui_Start.h"
#include "ui_Main.h"
#include "ui_Sample10.h"
#include "ui_SampJGJ.h"
#include "ui_SampBlock.h"
#include "ui_Browse10.h"
#include "ui_Verify.h"
#include "ui_Paraset10.h"
#include "ui_Analyze.h"
#include "ui_Sampset.h"
#include "ui_Sampdisp.h"
#include "ui_Condition.h"
#include "ui_Biaoding.h"
#include "ui_Gain.h"
#include "ui_Heat.h"

#include "OpFile.h"
#include "OpData.h"
#include "OpPara.h"
#include "OpTable.h"

#include "delay.h"
#include "malloc.h"
#include "w25qxx.h" 
#include "ledBeep.h"
#include "timer.h"

#include "bkLed.h"
#include "power.h"
#include "systic.h"
#include "key.h"
#include "uart.h"
#include "batAdc.h"
#include "lcd.h"
#include "font.h"
#include "ft6336u.h"
#include "touch.h"
#include "rx8010.h" 
#include "ct1711.h"
#include "gps.h"
#include "hcBT20.h"
#include "iwdog.h"

#include "fontupd.h"
#include "pytext.h"


#ifndef NULL
	#define NULL 			0
#endif


#define UI_START         		1
#define UI_MAIN        			2		//ui界面 编号，对应 tab_ui_Func 表
#define UI_SAMPLE10     		3
#define UI_SAMPJGJ       		4
#define UI_SAMPBLOCK      		5
#define UI_BROWSE10    		 	6
#define UI_VERIFY         		7
#define UI_PARASET10    		8
#define UI_SAMPSET        		9
#define UI_ANALYZE        		10
#define UI_SAMPDISP        		11
#define UI_CONDITION        	12
#define UI_BIAODING        		13
#define UI_GAIN        			14
#define UI_HEAT        			15
#define UI_VIEW        			16

#define THEME_DARK		0
#define THEME_LIGHT		1

extern void (*g_pRunFunc)();  //运行函数

void UiInit(void);
void SendUiNo(signed char UiNo);
void UiSchedule(void);

extern signed char g_cLastUiNo;  	//上一任务号
extern signed char g_cUiNo;      	//有负数的情况

#endif

