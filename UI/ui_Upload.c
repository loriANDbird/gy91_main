/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：数据传输
    1. ui_Upload 界面初始化，
    2. ui_Upload_Run 函数实现

    
更新记录：
    1. 2018.03.01 初次创建
    
*******************************************************************************/
#include "ui_Upload.h"
#include "Config_ui.h"

#define BTN_RETURN	0
#define BTN_BROWSE	1
#define BTN_DELETE	2
#define BTN_SYSSET	3


void ui_Upload_InitDisp(void)
{
	//	LCD_FillRect(0,0,240,26,GOLD);
//	LCD_SetColor(WHITE,BLACK);
//	LCD_ShowString(ASC24x12,90,296,0,"Page1");
	
	LCD_FillRect(0,0,480,320,WHITE);
	
//	ui_DispTime(1);
	ui_DispBattery(1);//显示电量
	
	LCD_SetColor(WHITE,BLACK);
	LCD_ShowChinese(Font32,8,1,0,"迴");//返回图标
	
	g_ButtonBuf[0].sX = 0;
	g_ButtonBuf[0].sY = 0;
	g_ButtonBuf[0].eX = 47;
	g_ButtonBuf[0].eY = 35;
	g_ButtonBuf[0].btSta = 0;
	
//	LCD_FillRect(100,100,100,100,0x7bff);
//	LCD_FillRect(200,100,100,100,0x41ff);
//	LCD_FillRect(300,100,100,100,0x211f);
}

void ui_Upload_Run(void)
{
	
    while(1)
	{
		ui_KeyTask();
		ui_ButtonTask(1);
		
//		ui_DispTime(0);//时间
		ui_DispBattery(0);//电量
		
		//蓝牙
		if(g_SysPara.iBlueToothSta == 1){
			LookupBT();
			if(g_bBlueTooDispFlag == 1){
				g_bBlueTooDispFlag = 0;
				ui_DispBluetooth();
			}
		}
		
		//标定
		if(g_u8TouchResult == BTN_RETURN){
				Biaoding_Adc_Data();
		}
		//切换界面
		if(g_u16KeyValBuf == KEY_Fn){
			Touch_Beep_On();
			SendUiNo(UI_PARASET10);
			LCD_FillRect(0,0,480,320,WHITE);
			return;
		}
		//清除界面
		if(g_u16KeyValBuf == KEY_POWER){
			Touch_Beep_On();
			LCD_FillRect(0,0,480,320,WHITE);
			LCD_SetColor(WHITE,BLACK);
			LCD_ShowChinese(Font32,8,1,0,"迴");//返回图标
			ui_DispBattery(1);//显示电量
			TimeSetDialog(0);
			TimeSetDialog(1);
		}
		//跳转界面
		if((g_u16KeyValBuf == KEY_UP)||(g_u8TouchResult == SLIDE_END_LEFT)){
			Touch_Beep_On();
			SendUiNo(UI_PARASET10);
			LCD_FillRect(0,0,480,320,WHITE);
			return;
		}
		//ADC采集
		Get_Adc_Data();
		//显示温度
//		if(g_bTemperaDispFlag == 1){
//			g_bTemperaDispFlag = 0;
//			LCD_SetColor(WHITE,BLACK);
//			LCD_ShowFloat(ASC24x12,420,296,0,5,1,g_GlobalPara.fTemperature);
//		}
		Sleep_ms(5);
	}
}


