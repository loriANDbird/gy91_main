/*******************************************************************************
�����ߣ���Ȫ
�������ڣ�2018.03.01
����������
    1. ��ʱ������
    2. SysTick�жϳ�ʼ������
    3. SysTick�жϷ������� 48MHzϵͳʱ���£���ʱԼ  5  uS
    
���¼�¼��
    1. 2018.03.01 ����
    
******************************************************************************/

#include "systic.h"
#include "key.h"
#include "OpPara.h"
#include "touch.h"
#include "iwdog.h"
#include "ct1711.h"


int	g_iSysTick20mSCnt;			//ϵͳ�δ������20mS
int	g_iSysTick500mSCnt;			//ϵͳ�δ������1S

int m_iPowerOff20msCnt;	//�ػ�ʱ���������
int m_iBKLedOff20msCnt;	//�رձ����������
int m_iPowerOffCnt = 0;		//�ػ�ʱ�����
int m_iBKLedOffCnt = 0;		//�ػ�ʱ�����
u32 m_iBatTest1SCnt = 0;		//��������ʱ�����
u8 g_iSysTick20mSFlag = 0;		//20ms��־
u8 g_bBkLedOnOffSta = 0;		//����ر�״̬	0:δ�ر� 1:�ر�
u8 g_bGetBatDataFlag = 0;		//��ȡ������־
u8 g_bBlueTooDispFlag = 0;		//����״̬��ʾ
u8 g_bTemperaGetFlag = 0;		//�¶ȶ�ȡ��־
u8 g_bTemperaDispFlag = 0;		//�¶���ʾ��־
u8 g_bTouchScanFlag = 0;		//����ɨ���־

u8 g_bDispAdcValFlag = 0;		//��ʾָ��λ�ñ�־

void SysTick_Init(void)  //20mS
{
	if (SysTick_Config(SystemCoreClock / 50)){  
		while (1);
	}
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
 
    g_iSysTick20mSCnt = 0;
	
	m_iPowerOffCnt = 0;
	g_bBkLedOnOffSta = 0;	
	m_iPowerOff20msCnt = g_SysPara.iPowerOffMinCnt*60*50;
	m_iBKLedOff20msCnt = g_SysPara.iBkLedOffMinCnt*60*50;
}

void SysTick_Disable(void)
{
//	SysTick->CTRL = 0;
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
	
	g_iSysTick20mSCnt += 1;
	g_iSysTick20mSFlag = 1;

	//�¶�
	CT1711_CvrWait();
	//GPS
	GPS_CountDown();
	
	if(g_iSysTick20mSCnt%5 == 0){//100ms
		g_bDispAdcValFlag = 1;
	}
	if(g_iSysTick20mSCnt%10 == 0){//200ms
		
		//����
		g_bBlueTooDispFlag = 1;
		//����
		g_bGetBatDataFlag = 1;
		//�¶�
		g_bTemperaGetFlag = 1;
		
	}
	if(g_iSysTick20mSCnt%25 == 0){//500ms
		//ι��
		if(g_ucIWDogFeed_Enable == 1){
			IWDG_Feed();
		}
		//
		g_iSysTick500mSCnt += 1;	
	}
	if(g_iSysTick20mSCnt%50 == 0){//1s
		//��������
		#ifdef BATTEST
		m_iBatTest1SCnt += 1;
		if(m_iBatTest1SCnt >= 60){//1����
			m_iBatTest1SCnt = 0;
			g_BatDataTest.iMinutCount += 1;
		}
		#endif
		//ʱ��
		RX8010_ReadTime(&g_SysTime);
		
		g_bTemperaDispFlag = 1;
	}
	
	//�Զ��ػ�
	if(g_GlobalPara.iChargeStaRes == 0){//δ���
		m_iPowerOffCnt += 1;
	}
	if(m_iPowerOffCnt > m_iPowerOff20msCnt){
		PwrOffFunc();//�ػ�
	}
	
	if(g_bBkLedOnOffSta == 0){
		if(g_GlobalPara.iChargeStaRes == 0){//δ���
			m_iBKLedOffCnt += 1;
		}
		if(m_iBKLedOffCnt > m_iBKLedOff20msCnt){
			SetBkLedPercent(10);//�ر���
			g_bBkLedOnOffSta = 1;
		}
	}
	
	//����ɨ��
	g_u16KeyVal = Key_Scan();
	g_bTouchScanFlag = 1;
}


//�ػ�ʱ�����ֵ����
void PowerOffCnt_Reset(void)
{
	m_iPowerOffCnt = 0;
}
//�رձ������ֵ����
void BKLedOffCnt_Reset(void)
{
	m_iBKLedOffCnt = 0;
}
