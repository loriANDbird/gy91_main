#ifndef __SYSTIC_H
#define __SYSTIC_H

#include "mcu_io.h"

extern int g_iSysTick20mSCnt;			//ϵͳ�δ������20mS
extern int g_iSysTick500mSCnt;			//ϵͳ�δ������1S

extern u8 g_iSysTick20mSFlag;		//20ms��־
extern u8 g_bBkLedOnOffSta;		//����ر�״̬	0:δ�ر� 1:�ر�
extern u8 g_bGetBatDataFlag;		//��ȡ������־
extern u8 g_bBlueTooDispFlag;		//����״̬��ʾ
extern u8 g_bTemperaGetFlag;		//�¶���ʾ��־
extern u8 g_bTemperaDispFlag;		//�¶���ʾ��־
extern u8 g_bDispAdcValFlag;		//��ʾָ��λ�ñ�־
extern u8 g_bTouchScanFlag;		//����ɨ���־

void SysTick_Init(void);
void SysTick_Disable(void);

void PowerOffCnt_Reset(void);
void BKLedOffCnt_Reset(void);

#endif

