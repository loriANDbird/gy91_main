
#include "ui_Start.h"
#include "Config_ui.h"

void ui_Start_InitDisp(void)
{
	unsigned char i;
	
	//按钮初始化
	g_u8ButtonNums = 1;
	struct BUTTON sBtn[1] = {
		//sX,  sY,  eX,  eY,
		{0, 0, 479, 319, 0,0} //0, 返回
	};   
	memcpy(g_ButtonBuf,sBtn,sizeof(sBtn));
	
	if(g_SysPara.iTheme == THEME_DARK){
		LCD_FillRect(0,0,480,320,BLACK);
		LCD_ShowPicture(20,20,160,36,(const unsigned short*)gImage_hcgk_dark);
		LCD_SetColor(BLACK,WHITE);
		for(i=0;i<7;i++){
			LCD_ShowNums(NUM96x48,72+i*48,72,0,i);
		}
		LCD_SetColor(BLACK,GOLD);
		LCD_ShowChinese(Font24,144,190,0,(u8*)"一体式钢筋扫描仪");
		LCD_SetColor(BLACK,LGRAY);
		LCD_ShowChinese(Font16,144,288,0,(u8*)"北京海创高科科技有限公司");
	}else{
		LCD_FillRect(0,0,480,320,WHITE);
		LCD_ShowPicture(20,20,160,36,(const unsigned short*)gImage_hcgk_light);
		LCD_SetColor(WHITE,BLACK);
		for(i=0;i<7;i++){
			LCD_ShowNums(NUM96x48,72+i*48,72,0,i);
		}
		LCD_SetColor(WHITE,DDGRAY);
		LCD_ShowChinese(Font24,144,190,0,(u8*)"一体式钢筋扫描仪");
		LCD_SetColor(WHITE,BLACK);
		LCD_ShowChinese(Font16,144,288,0,(u8*)"北京海创高科科技有限公司");
	}
	ClearTouchSta();
}

//---------------------------------------------

void ui_Start_Run(void)
{
	short barCnt = 0;
	short ledCnt = 0;
	short barCntLast = 0;
	while(1)
	{
		//按键
		ui_KeyTask();
		ui_ButtonTask(1);
//		ui_DispTime(0);//时间
//		ui_DispBattery(0);//电量
//		
		if(g_iSysTick20mSFlag == 1){
			g_iSysTick20mSFlag = 0;
			if(barCnt == 0){
				if(g_SysPara.iTheme == THEME_DARK){
					LCD_FillRect(120,248,240,4,LLGRAY);
				}else{
					LCD_FillRect(120,248,240,4,GRAY);
				}
				Sleep_ms(200);
			}
			barCnt += 2;
			if(barCnt%12 == 0){
				ledCnt += 1;
				
//				if(ledCnt == 4){
//					LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
//				}
//				if(ledCnt == 7){
//					LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
//				}
//				if(ledCnt <= 3){
//					LED_Ctrl(LED_RED,ledCnt,LED_STA_ON);
//				}else if(ledCnt <= 6){
//					LED_Ctrl(LED_BLUE,ledCnt-3,LED_STA_ON);
//				}else if(ledCnt <= 9){
//					LED_Ctrl(LED_BLUE,ledCnt-6,LED_STA_ON);
//					LED_Ctrl(LED_RED,ledCnt-6,LED_STA_ON);
//				}
//				if(ledCnt > 9){
//					ledCnt = 0;
//					LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
//					LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
//				}
				if(ledCnt == 4){
					LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
				}
				if(ledCnt <= 3){
					LED_Ctrl(LED_RED,ledCnt,LED_STA_ON);
				}else if(ledCnt <= 6){
					LED_Ctrl(LED_BLUE,ledCnt-3,LED_STA_ON);
				}
				if(ledCnt > 6){
					ledCnt = 0;
					LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
					LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
				}
			}
			LCD_FillRect(120+barCntLast,248,barCnt-barCntLast,4,GOLD);
			barCntLast = barCnt;
			if(barCnt >= 240){
				LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
				LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
				Sleep_ms(200);
				SendUiNo(UI_MAIN);
				if(g_SysPara.iTheme == THEME_DARK){
					LCD_FillRect(0,0,480,320,BLACK);
				}else{
					LCD_FillRect(0,0,480,320,WHITE);
				}
				return;
			}
			if(g_u16KeyValBuf != KEY_NONE){
				LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
				LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
				SendUiNo(UI_MAIN);
				if(g_SysPara.iTheme == THEME_DARK){
					LCD_FillRect(0,0,480,320,BLACK);
				}else{
					LCD_FillRect(0,0,480,320,WHITE);
				}
				return;
			}
		}

//		if(g_u16KeyValBuf != KEY_NONE){
//			LCD_FillRect(0,0,480,320,WHITE);
//		}
		Sleep_ms(5);
	}
}





//u8 bmpRes = 0;
//u8 bmpNum;
//u8 bmpQnt = 0;
//char bmpNameBuf[8];

//void ui_Start_InitDisp(void)
//{
//	g_ButtonBuf[0].sX = 0;
//	g_ButtonBuf[0].sY = 0;
//	g_ButtonBuf[0].eX = 47;
//	g_ButtonBuf[0].eY = 35;
//	g_ButtonBuf[0].btSta = 0;
//	g_ButtonBuf[0].btAbility = 0;
//	
//	bmpQnt = Scan_FileCount("0:/BMP");
//	if(bmpQnt > 0){
//		bmpNum = 1;
//		sprintf(bmpNameBuf,"%d",bmpNum);
//		bmpRes = LCD_ShowBMP(0,0,480,320,bmpNameBuf);
//		if(bmpRes != 0){
//			LCD_SetColor(WHITE,BLACK);
//			LCD_ShowString(ASC24x12,0,0,0,bmpNameBuf);
//		}
//	}else{
//		LCD_SetColor(WHITE,BLACK);
//		LCD_ShowString(ASC24x12,0,0,0,"NO FILE!");
//	}
//}

////---------------------------------------------

//void ui_Start_Run(void)
//{
//	bmpNum = 1;
//	while(1)
//	{
//		//按键
//		ui_KeyTask();
//		ui_ButtonTask(1);
////		ui_DispTime(0);//时间
////		ui_DispBattery(0);//电量

//		//切换图片
//		if(bmpQnt > 0){
//			//切换图片
//			if((g_u16KeyValBuf == KEY_UP)||(g_u8TouchResult == SLIDE_END_LEFT)){
//				bmpNum++;		
//				if(bmpNum > 20)bmpNum = 1;
//				sprintf(bmpNameBuf,"%d",bmpNum);
//				bmpRes = LCD_ShowBMP(0,0,480,320,bmpNameBuf);
//				LCD_SetColor(WHITE,BLACK);
//				sprintf(bmpNameBuf,"%2d",bmpNum);
//				LCD_ShowString(ASC24x12,0,0,0,bmpNameBuf);
//				while(bmpRes == 0){
//					bmpNum++;
//					if(bmpNum > 20)bmpNum = 1;
//					sprintf(bmpNameBuf,"%d",bmpNum);
//					bmpRes = LCD_ShowBMP(0,0,480,320,bmpNameBuf);
//					LCD_SetColor(WHITE,BLACK);
//					sprintf(bmpNameBuf,"%2d",bmpNum);
//					LCD_ShowString(ASC24x12,0,0,0,bmpNameBuf);
//				}
//				if(bmpRes != 0){
//					
//					if(bmpNum == 2){
//						LCD_ShowChinese(Font12,400,200,0,"开始检测");
//						LCD_ShowChinese(Font14,400,220,0,"开始检测");
//						LCD_ShowChinese(Font16,400,242,0,"开始检测");
//					}
//					if(bmpNum == 6){
//						LCD_ShowString(ASC12x6,400,200,0,"1234567890");
//						LCD_ShowString(ASC16x8,400,220,0,"1234567890");
//						LCD_ShowString(ASC20x10,400,242,0,"1234567890");
//					}
//				}
//			}
//			if((g_u16KeyValBuf == KEY_POWER)||(g_u8TouchResult == SLIDE_END_RIGHT)){
//				bmpNum--;
//				
//				if(bmpNum < 1)bmpNum = 20;
//				sprintf(bmpNameBuf,"%d",bmpNum);
//				bmpRes = LCD_ShowBMP(0,0,480,320,bmpNameBuf);
//				LCD_SetColor(WHITE,BLACK);
//				sprintf(bmpNameBuf,"%2d",bmpNum);
//				LCD_ShowString(ASC24x12,0,0,0,bmpNameBuf);
//				while(bmpRes == 0){
//					bmpNum--;
//					if(bmpNum < 1)bmpNum = 20;
//					sprintf(bmpNameBuf,"%d",bmpNum);
//					bmpRes = LCD_ShowBMP(0,0,480,320,bmpNameBuf);
//					LCD_SetColor(WHITE,BLACK);
//					sprintf(bmpNameBuf,"%2d",bmpNum);
//					LCD_ShowString(ASC24x12,0,0,0,bmpNameBuf);
//				}
//				if(bmpRes != 0){
//					
//					if(bmpNum == 2){
//						LCD_ShowChinese(Font12,400,200,0,"开始检测");
//						LCD_ShowChinese(Font14,400,220,0,"开始检测");
//						LCD_ShowChinese(Font16,400,242,0,"开始检测");
//					}
//					if(bmpNum == 6){
//						LCD_ShowString(ASC12x6,400,200,0,"1234567890");
//						LCD_ShowString(ASC16x8,400,220,0,"1234567890");
//						LCD_ShowString(ASC20x10,400,242,0,"1234567890");
//					}
//				}
//			}
//		}
//		//切换页面
//		if(g_u16KeyValBuf == KEY_FN){
//			Touch_Beep_On();
//			SendUiNo(UI_MAIN);
//			LCD_FillRect(0,0,480,320,WHITE);
//			return;
//		}
//		if(g_u8TouchResult == 0){//返回主页
//			SendUiNo(UI_MAIN);
//			LCD_FillRect(0,0,480,320,WHITE);
//			return;
//		}
//		Sleep_ms(10);
//	}
//}

