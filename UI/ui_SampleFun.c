/*******************************************************************************
创建者：张喜双
创建日期：2018
功能描述：
    1. 厚度检测、钢筋扫描模式，共用函数及变量
    
*******************************************************************************/
#include "ui_Sample.h"
#include "Config_ui.h"


void ui_DispDate(void)
{
	char dateBuf[12];
	sprintf(&dateBuf[0],"%2d",20);
	sprintf(&dateBuf[2],"%2d",g_SysTime.year);
	sprintf(&dateBuf[5],"%2d",g_SysTime.month);
	sprintf(&dateBuf[8],"%2d",g_SysTime.day);
	if(dateBuf[5] == ' ') dateBuf[5] = '0';
	if(dateBuf[8] == ' ') dateBuf[8] = '0';
	dateBuf[4] = ':';
	dateBuf[7] = ':';
	LCD_SetColor(GOLD,BLACK);
	LCD_ShowString(ASC16x8,10,6,0,dateBuf);
}
void ui_DispTime(u8 sta)
{
	static u8 lastMin = 0;
	char timeBuf[8];
	if(sta == 1){//立即更新
		sprintf(timeBuf,"%2d",g_SysTime.hour);
		sprintf(&timeBuf[3],"%2d",g_SysTime.min);
		timeBuf[2] = ':';
		if(timeBuf[3] == ' ') timeBuf[3] = '0';
		LCD_SetColor(GOLD,BLACK);
		LCD_ShowString(ASC16x8,100,6,0,timeBuf);
		lastMin = g_SysTime.min;
	}else {
		if(g_SysTime.min != lastMin){
			sprintf(timeBuf,"%2d",g_SysTime.hour);
			sprintf(&timeBuf[3],"%2d",g_SysTime.min);
			timeBuf[2] = ':';
			if(timeBuf[3] == ' ') timeBuf[3] = '0';
			LCD_SetColor(GOLD,BLACK);
			LCD_ShowString(ASC16x8,100,6,0,timeBuf);
			lastMin = g_SysTime.min;
		}
	}
}

//显示电量
void ui_DispBattery(void)
{
	//电池框
	LCD_FillRect(200,5,25,2,BLACK);
	LCD_FillRect(200,17,25,2,BLACK);
	LCD_FillRect(200,7,2,10,BLACK);
	LCD_FillRect(223,7,2,10,BLACK);
	LCD_FillRect(225,9,2,6,BLACK);
	//电量格
	LCD_FillRect(203,8,4,8,BLACK);
	LCD_FillRect(208,8,4,8,BLACK);
	LCD_FillRect(213,8,4,8,BLACK);
	LCD_FillRect(218,8,4,8,BLACK);
}

void ui_DispBluetooth(void)
{
	if(g_SysPara.bBlueToothSta == 1){
		LCD_SetColor(GOLD ,BLACK);
		LCD_ShowChinese(Font24,170,1,0,"牙");
	}else{
		LCD_FillRect(170,1,24,24,GOLD);//关闭
	}
}