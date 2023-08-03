/*******************************************************************************
创建者：康泉
创建日期：2018.03.01
功能描述：
    1. 定时器操作
    2. SysTick中断初始化操作
    3. SysTick中断服务函数， 48MHz系统时钟下，用时约  5  uS
    
更新记录：
    1. 2018.03.01 创建
    
******************************************************************************/

#include "systic.h"
#include "key.h"
#include "OpPara.h"
#include "touch.h"
#include "iwdog.h"
#include "ct1711.h"


int	g_iSysTick20mSCnt;			//系统滴答计数器20mS
int	g_iSysTick500mSCnt;			//系统滴答计数器1S

int m_iPowerOff20msCnt;	//关机时间计数总数
int m_iBKLedOff20msCnt;	//关闭背光计数总数
int m_iPowerOffCnt = 0;		//关机时间计数
int m_iBKLedOffCnt = 0;		//关机时间计数
u32 m_iBatTest1SCnt = 0;		//电量测试时间计数
u8 g_iSysTick20mSFlag = 0;		//20ms标志
u8 g_bBkLedOnOffSta = 0;		//背光关闭状态	0:未关闭 1:关闭
u8 g_bGetBatDataFlag = 0;		//读取电量标志
u8 g_bBlueTooDispFlag = 0;		//蓝牙状态显示
u8 g_bTemperaGetFlag = 0;		//温度读取标志
u8 g_bTemperaDispFlag = 0;		//温度显示标志
u8 g_bTouchScanFlag = 0;		//触摸扫描标志

u8 g_bDispAdcValFlag = 0;		//显示指针位置标志

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

	//温度
	CT1711_CvrWait();
	//GPS
	GPS_CountDown();
	
	if(g_iSysTick20mSCnt%5 == 0){//100ms
		g_bDispAdcValFlag = 1;
	}
	if(g_iSysTick20mSCnt%10 == 0){//200ms
		
		//蓝牙
		g_bBlueTooDispFlag = 1;
		//电量
		g_bGetBatDataFlag = 1;
		//温度
		g_bTemperaGetFlag = 1;
		
	}
	if(g_iSysTick20mSCnt%25 == 0){//500ms
		//喂狗
		if(g_ucIWDogFeed_Enable == 1){
			IWDG_Feed();
		}
		//
		g_iSysTick500mSCnt += 1;	
	}
	if(g_iSysTick20mSCnt%50 == 0){//1s
		//电量测试
		#ifdef BATTEST
		m_iBatTest1SCnt += 1;
		if(m_iBatTest1SCnt >= 60){//1分钟
			m_iBatTest1SCnt = 0;
			g_BatDataTest.iMinutCount += 1;
		}
		#endif
		//时间
		RX8010_ReadTime(&g_SysTime);
		
		g_bTemperaDispFlag = 1;
	}
	
	//自动关机
	if(g_GlobalPara.iChargeStaRes == 0){//未充电
		m_iPowerOffCnt += 1;
	}
	if(m_iPowerOffCnt > m_iPowerOff20msCnt){
		PwrOffFunc();//关机
	}
	
	if(g_bBkLedOnOffSta == 0){
		if(g_GlobalPara.iChargeStaRes == 0){//未充电
			m_iBKLedOffCnt += 1;
		}
		if(m_iBKLedOffCnt > m_iBKLedOff20msCnt){
			SetBkLedPercent(10);//关背光
			g_bBkLedOnOffSta = 1;
		}
	}
	
	//按键扫描
	g_u16KeyVal = Key_Scan();
	g_bTouchScanFlag = 1;
}


//关机时间计数值清零
void PowerOffCnt_Reset(void)
{
	m_iPowerOffCnt = 0;
}
//关闭背光计数值清零
void BKLedOffCnt_Reset(void)
{
	m_iBKLedOffCnt = 0;
}
