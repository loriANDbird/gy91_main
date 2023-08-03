/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：钢筋扫描菜单
    1. ui_Sample20 界面初始化
    2. ui_Sample20_Run 函数实现

    
更新记录：
    1. 2018.03.01 初次创建
    
*******************************************************************************/
#include "ui_Sample20.h"


void ui_Sample20_InitDisp(void)
{
	LCD_FillRect(0,0,240,26,YELLOW);
	LCD_SetColor(YELLOW,BLACK);
	LCD_ShowString(ASC16x8,100,6,0,"12:30");
	
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
	
	LCD_SetColor(GRAY,BLACK);
	LCD_ShowString(ASC32x16,70,36,0,"sample2");
}

void ui_Sample20_Run(void)
{	
    while(1)
	{
		ui_KeyTask();

		Sleep_ms(5);
	}
}
