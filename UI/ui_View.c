
#include "ui_View.h"
#include "Config_ui.h"

u8 bmpRes = 0;
u8 bmpNum;
u8 bmpQnt = 0;
char bmpNameBuf[8];

void ui_View_InitDisp(void)
{
	g_ButtonBuf[0].sX = 0;
	g_ButtonBuf[0].sY = 0;
	g_ButtonBuf[0].eX = 47;
	g_ButtonBuf[0].eY = 35;
	g_ButtonBuf[0].btSta = 0;
	g_ButtonBuf[0].btAbility = 0;
	
	bmpQnt = Scan_FileCount("0:/BMP");
	if(bmpQnt > 0){
		bmpNum = 1;
		sprintf(bmpNameBuf,"%d",bmpNum);
		bmpRes = LCD_ShowBMP(0,0,480,320,bmpNameBuf);
		if(bmpRes != 0){
			LCD_SetColor(WHITE,BLACK);
			LCD_ShowString(ASC24x12,0,0,0,bmpNameBuf);
		}
	}else{
		LCD_SetColor(WHITE,BLACK);
		LCD_ShowString(ASC24x12,0,0,0,"NO FILE!");
	}
}

void ui_View_Run(void)
{
	bmpNum = 1;
	while(1)
	{
		//按键
		ui_KeyTask();
		ui_ButtonTask(1);
//		ui_DispTime(0);//时间
//		ui_DispBattery(0);//电量

		//切换图片
		if(bmpQnt > 0){
			//切换图片
			if((g_u16KeyValBuf == KEY_UP)||(g_u8TouchResult == SLIDE_END_LEFT)){
				bmpNum++;		
				if(bmpNum > 20)bmpNum = 1;
				sprintf(bmpNameBuf,"%d",bmpNum);
				bmpRes = LCD_ShowBMP(0,0,480,320,bmpNameBuf);
				LCD_SetColor(WHITE,BLACK);
				sprintf(bmpNameBuf,"%2d",bmpNum);
				LCD_ShowString(ASC24x12,0,0,0,bmpNameBuf);
				while(bmpRes == 0){
					bmpNum++;
					if(bmpNum > 20)bmpNum = 1;
					sprintf(bmpNameBuf,"%d",bmpNum);
					bmpRes = LCD_ShowBMP(0,0,480,320,bmpNameBuf);
					LCD_SetColor(WHITE,BLACK);
					sprintf(bmpNameBuf,"%2d",bmpNum);
					LCD_ShowString(ASC24x12,0,0,0,bmpNameBuf);
				}
				if(bmpRes != 0){
					
					if(bmpNum == 2){
						LCD_ShowChinese(Font12,400,200,0,"开始检测");
						LCD_ShowChinese(Font14,400,220,0,"开始检测");
						LCD_ShowChinese(Font16,400,242,0,"开始检测");
					}
					if(bmpNum == 6){
						LCD_ShowString(ASC12x6,400,200,0,"1234567890");
						LCD_ShowString(ASC16x8,400,220,0,"1234567890");
						LCD_ShowString(ASC20x10,400,242,0,"1234567890");
					}
				}
			}
			if((g_u16KeyValBuf == KEY_POWER)||(g_u8TouchResult == SLIDE_END_RIGHT)){
				bmpNum--;
				
				if(bmpNum < 1)bmpNum = 20;
				sprintf(bmpNameBuf,"%d",bmpNum);
				bmpRes = LCD_ShowBMP(0,0,480,320,bmpNameBuf);
				LCD_SetColor(WHITE,BLACK);
				sprintf(bmpNameBuf,"%2d",bmpNum);
				LCD_ShowString(ASC24x12,0,0,0,bmpNameBuf);
				while(bmpRes == 0){
					bmpNum--;
					if(bmpNum < 1)bmpNum = 20;
					sprintf(bmpNameBuf,"%d",bmpNum);
					bmpRes = LCD_ShowBMP(0,0,480,320,bmpNameBuf);
					LCD_SetColor(WHITE,BLACK);
					sprintf(bmpNameBuf,"%2d",bmpNum);
					LCD_ShowString(ASC24x12,0,0,0,bmpNameBuf);
				}
				if(bmpRes != 0){
					
					if(bmpNum == 2){
						LCD_ShowChinese(Font12,400,200,0,"开始检测");
						LCD_ShowChinese(Font14,400,220,0,"开始检测");
						LCD_ShowChinese(Font16,400,242,0,"开始检测");
					}
					if(bmpNum == 6){
						LCD_ShowString(ASC12x6,400,200,0,"1234567890");
						LCD_ShowString(ASC16x8,400,220,0,"1234567890");
						LCD_ShowString(ASC20x10,400,242,0,"1234567890");
					}
				}
			}
		}
		//切换页面
		if(g_u16KeyValBuf == KEY_FN){
			Touch_Beep_On();
			SendUiNo(UI_MAIN);
			LCD_FillRect(0,0,480,320,WHITE);
			return;
		}
		if(g_u8TouchResult == 0){//返回主页
			SendUiNo(UI_MAIN);
			LCD_FillRect(0,0,480,320,WHITE);
			return;
		}
		Sleep_ms(10);
	}
}

