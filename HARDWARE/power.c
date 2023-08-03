#include "power.h"
#include "ledBeep.h"
#include "bkLed.h" 
#include "delay.h"
#include "Config_ui.h"

void Power_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;    
	
	RCC_APB2PeriphClockCmd(POWER_RCC|USBEN_RCC|CarPOWER_RCC|CHARGE_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = POWER_PIN;//主板电源
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(POWER_PORT, &GPIO_InitStructure);
	MainPowerOff();//电源关
	
	GPIO_InitStructure.GPIO_Pin = CarPOWER_PIN;//采集板电源
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CarPOWER_PORT, &GPIO_InitStructure);
	CarPowerOff();//电源关
	
	GPIO_InitStructure.GPIO_Pin = USBEN_PIN;//USB使能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USBEN_PORT, &GPIO_InitStructure);
	USB_Enable();//使能USB
	
	GPIO_InitStructure.GPIO_Pin = CHARGE_PIN|CHARGEDONE_PIN;//充电状态
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CHARGE_PORT, &GPIO_InitStructure);
	
	PwrOnFunc();//开机
	//while(ReadPwrKeySta() == 0){};  //等待电源按键抬起
}


//开总电源
void PwrOnFunc(void)
{
	MainPowerOn();
	CarPowerOn();
	PromptTone_Once(200);//提示音
}
//关总电源
void PwrOffFunc(void)
{
// 	__disable_irq(); //关闭总中断     
//	PromptTone_Twice(135);//提示音
//	
//	SetBkLedPercent(0);
//	CarPowerOff();
//	MainPowerOff();   //PowerCtrl   Off
//    while(1){
//		Sleep_ms(500);
//    }
}

//检测开机方式
void CheckOpenType(void)
{
	if((ReadPwrKeySta() == 0)&&(ReadChargeSta() == 0)&&(ReadChargeDoneSta() == 0)){//按键开机
		g_GlobalPara.iOpenType = OPEN_KEY;
		g_GlobalPara.iChargeSta = CHARGE_NO;
		while(ReadPwrKeySta() == 0){};  //等待电源按键抬起
	}
	if((ReadPwrKeySta() == 1)&&((ReadChargeSta() == 1)||(ReadChargeDoneSta() == 1))){//充电开机
		g_GlobalPara.iOpenType = OPEN_CHARGE;
		g_GlobalPara.iChargeSta = CHARGE_ING;
	}
}

//检测充电状态
void CheckChargeStep(void)
{
	if((ReadChargeSta() == 0)&&(ReadChargeDoneSta() == 0)){//未充电
		if((g_GlobalPara.iChargeSta != CHARGE_ING)&&(g_GlobalPara.iChargeSta != CHARGE_DONE)){
			g_GlobalPara.iChargeSta = CHARGE_NO;
		}
	}
	if((ReadChargeSta() == 1)&&(ReadChargeDoneSta() == 0)){//充电中
		g_GlobalPara.iChargeSta = CHARGE_ING;

	}
	if((ReadChargeSta() == 0)&&(ReadChargeDoneSta() == 1)){//充电完成
		g_GlobalPara.iChargeSta = CHARGE_DONE;
	}
	
	Sleep_ms(1);
}
//检测充电状态
//返回值：0:无状态变化，1:开始充电，2:充电完成，3:拔掉充电器
u8 CheckChargeSta(void)
{
	static int chargeSta_L = CHARGE_NO; 
//	if((ReadChargeSta() == 0)&&(ReadChargeDoneSta() == 0)){//未充电
//		g_GlobalPara.iChargeSta = CHARGE_NO;
//		LCD_SetColor(WHITE,BLACK);
//		LCD_ShowChinese(Font12,450,40,0,"○");
//	}
//	if((ReadChargeSta() == 1)&&(ReadChargeDoneSta() == 0)){//充电中
//		g_GlobalPara.iChargeSta = CHARGE_ING;
//		LCD_SetColor(WHITE,BLACK);
//		LCD_ShowChinese(Font12,450,40,0,"●");
//	}
//	if((ReadChargeSta() == 0)&&(ReadChargeDoneSta() == 1)){//充电完成
//		g_GlobalPara.iChargeSta = CHARGE_DONE;
//	}
	
	if((g_GlobalPara.iChargeStaRes == CHARGE_ING)&&(chargeSta_L == CHARGE_NO)){//开始充电
		chargeSta_L = g_GlobalPara.iChargeStaRes;
		return 1;
	}
	if((g_GlobalPara.iChargeStaRes == CHARGE_DONE)&&(chargeSta_L == CHARGE_ING)){//充电完成
		chargeSta_L = g_GlobalPara.iChargeStaRes;
		return 2;
	}
	if((g_GlobalPara.iChargeStaRes == CHARGE_NO)&&(chargeSta_L != CHARGE_NO)){//拔掉充电器
//		if(g_SamplePara.iOpenType == 1){//充电开机
//			PwrOffFunc();
//		}
		chargeSta_L = g_GlobalPara.iChargeStaRes;
		return 3;
	}
	chargeSta_L = g_GlobalPara.iChargeStaRes;
	return 0;
}
