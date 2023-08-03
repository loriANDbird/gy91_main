#ifndef __SYSTIC_H
#define __SYSTIC_H

#include "mcu_io.h"

extern int g_iSysTick20mSCnt;			//系统滴答计数器20mS
extern int g_iSysTick500mSCnt;			//系统滴答计数器1S

extern u8 g_iSysTick20mSFlag;		//20ms标志
extern u8 g_bBkLedOnOffSta;		//背光关闭状态	0:未关闭 1:关闭
extern u8 g_bGetBatDataFlag;		//读取电量标志
extern u8 g_bBlueTooDispFlag;		//蓝牙状态显示
extern u8 g_bTemperaGetFlag;		//温度显示标志
extern u8 g_bTemperaDispFlag;		//温度显示标志
extern u8 g_bDispAdcValFlag;		//显示指针位置标志
extern u8 g_bTouchScanFlag;		//触摸扫描标志

void SysTick_Init(void);
void SysTick_Disable(void);

void PowerOffCnt_Reset(void);
void BKLedOffCnt_Reset(void);

#endif

