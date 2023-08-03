/*******************************************************************************
创建者：张喜双
创建日期：2018
功能描述：
    1. 厚度检测、钢筋扫描模式，共用函数及变量
    
*******************************************************************************/
#include "ui_Public.h"
#include "Config_ui.h"

//标题栏置灰
//mode 0:正常显示，1:置灰
void ui_DispTitleBar(u8 *name,u8 mode)
{
//	u16 x;
	u16 bkCol;
	u16 barCol;
	u16 namCol;
//	x = 60 + (420-strlen((const char*)name)*16/2)/2;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
		if(mode == 0){
			barCol = GRAY;
			namCol = WHITE;
		}else{
			barCol = DGRAY;
			namCol = GRAY;
		}
	}else{
		bkCol = WHITE;
		if(mode == 0){
			barCol = DGRAY;
			namCol = WHITE;
		}else{
			barCol = LGRAY;
			namCol = WHITE;
		}
	}
	//状态栏
	LCD_DrawRect(56,0,424,32,bkCol);
	LCD_FillRect(57,1,422,31,barCol);
	LCD_SetColor(barCol,namCol);
//	LCD_ShowChinese(Font16,x,9,0,name);
	LCD_ShowText_Center(Font16,57,1,422,31,0,(char*)name);
}
//显示日期
void ui_DispDate(void)
{
	char dateBuf[12];
	sprintf(&dateBuf[0],"%4d",g_SysTime.year);
	sprintf(&dateBuf[5],"%02d",g_SysTime.month);
	sprintf(&dateBuf[8],"%02d",g_SysTime.day);
//	if(dateBuf[5] == ' ') dateBuf[5] = '0';
//	if(dateBuf[8] == ' ') dateBuf[8] = '0';
	dateBuf[4] = '-';
	dateBuf[7] = '-';
	LCD_SetColor(LCD_ReadPoint(0,0),WHITE);//GRAY
	LCD_ShowString(ASC16x8,10,12,0,dateBuf);
}
//显示时间
void ui_DispTime(u8 sta)
{
	static u8 lastMin = 0;
	char timeBuf[8];
	u16 bkCol;
	bkCol = LCD_ReadPoint(0,0);
	if(sta == 1){//立即更新
		sprintf(timeBuf,"%02d",g_SysTime.hour);
		sprintf(&timeBuf[3],"%02d",g_SysTime.min);
		timeBuf[2] = ':';
//		if(timeBuf[0] == ' ') timeBuf[0] = '0';
//		if(timeBuf[3] == ' ') timeBuf[3] = '0';
		LCD_SetColor(bkCol,WHITE);//GRAY
		LCD_ShowString(ASC16x8,100,12,0,timeBuf);
		lastMin = g_SysTime.min;
	}else {
		if(g_SysTime.min != lastMin){
			sprintf(timeBuf,"%02d",g_SysTime.hour);
			sprintf(&timeBuf[3],"%02d",g_SysTime.min);
			timeBuf[2] = ':';
//			if(timeBuf[0] == ' ') timeBuf[0] = '0';
//			if(timeBuf[3] == ' ') timeBuf[3] = '0';
			LCD_SetColor(bkCol,WHITE);//GRAY
			LCD_ShowString(ASC16x8,100,12,0,timeBuf);
			lastMin = g_SysTime.min;
		}
	}
}

//显示GPS
//sta:1:立即更新，0:正常
void ui_DispGPSIcon(u8 sta)
{
	static u8 locaSta = 0;
	static u8 locaStaLast = 0;
	if(GPS_Info.status == 'A'){
		locaSta = 1;
		if(sta == 1){
			//显示图标
			LCD_SetColor(LCD_ReadPoint(380,9),WHITE);//GRAY
			LCD_ShowText(Font20,380,10,0,"丅");
		}
	}
	if(GPS_Info.status == ' '){
		locaSta = 0;
		LCD_SetColor(LCD_ReadPoint(380,9),WHITE);//GRAY
		LCD_ShowText(Font20,380,10,0,"N");
	}
	if(GPS_Info.status == 'V'){
		locaSta = 0;
		LCD_SetColor(LCD_ReadPoint(380,9),WHITE);//GRAY
		LCD_ShowText(Font20,380,10,0,"V");
	}
	if(locaSta != locaStaLast){
		if(locaSta == 1){
			//显示图标
			LCD_SetColor(LCD_ReadPoint(380,9),WHITE);//GRAY
			LCD_ShowText(Font20,380,10,0,"丅");
		}
		locaStaLast = locaSta;
	}
}

//读取温度
void ui_GetTempera(void)
{
	if(g_bTemperaGetFlag == 1){
		g_bTemperaGetFlag = 0;
//		__disable_irq(); //关闭总中断
		g_GlobalPara.fTemperature = CT1711_Get_Temp();
//		__enable_irq(); //关闭总中断
	}
}
//显示温度
void ui_DispTempera(void)
{
	char buf[12];
	if(g_bTemperaDispFlag == 1){
		g_bTemperaDispFlag = 0;
		sprintf(buf,"%5.1f℃",g_GlobalPara.fTemperature);
		LCD_SetColor(LCD_ReadPoint(300,9),WHITE);//GRAY
		LCD_ShowText(Font20,300,10,0,(u8*)buf);
//		LCD_ShowFloat(ASC20x10,340,6,0,5,1,g_GlobalPara.fTemperature);
	}
}

//显示蓝牙图标
void ui_DispBluetooth(void)
{
	if(g_bBlueTooDispFlag == 1){//200ms刷新一次
		g_bBlueTooDispFlag = 0;
		if(g_SysPara.iBlueToothSta == 1){
			if(BT_GetLinkSta()){//已连接
				LCD_SetColor(LCD_ReadPoint(410,9),BLUE);//GRAY
				LCD_ShowChinese(Font20,410,10,0,"伢");
			}else{
				LCD_SetColor(LCD_ReadPoint(410,9),WHITE);//GRAY
				LCD_ShowChinese(Font20,410,10,0,"伢");
			}
		}else{
			LCD_FillRect(410,10,20,20,LCD_ReadPoint(410,9));//关闭GRAY
		}
	}
}

//构件名简写
void GetNameShort(char* nambuf,char* namSor)
{
	u8 i=0,sta=0;
	u8 nameLen;
	char nameBuf[16];
	char tempBuf[16];
	char SorTemp[32];
	char* sres;
	
	sprintf(SorTemp,"%s",namSor);
	sres = strstr(SorTemp,".bin");
	if(sres != NULL){
		*sres = '\0';
	}
	nameLen = strlen(SorTemp);
	
	if(nameLen > 9){
		while(i < nameLen){
			if((SorTemp[i]&0x80)!=0){//有汉字
				
				i += 2;
			}else{
				i += 1;
			}
			if(sta == 0){
				if(i == 4){
					memcpy(nameBuf,SorTemp,4);
					nameBuf[4] = '\0';
					sta = 1;
				}else if(i == 5){
					memcpy(nameBuf,SorTemp,5);
					nameBuf[5] = '\0';
					sta = 1;
				}
			}else if(sta == 1){
				
				if(i == nameLen-4){
					memcpy(tempBuf,&SorTemp[i],4);
					tempBuf[4] = '\0';
					sta = 2;
				}else if(i == nameLen-3){
					memcpy(tempBuf,&SorTemp[i],3);
					tempBuf[3] = '\0';
					sta = 2;
				}
				if(sta == 2){
					strcat(nameBuf,"..");
					strcat(nameBuf,tempBuf);

					break;
				}
			}
		}
	}else{
		sprintf(nameBuf,"%s","          ");
		for(i=0;i<nameLen;i++){
			nameBuf[i] = SorTemp[i];
		}
	}
	sprintf(nambuf,"%s",nameBuf);
}
//显示构件名称
void ui_DispGJName(u8 font,u16 x,u16 y,u16 nameCol,char* nameStr)
{
	char nameBuf[16];
	GetNameShort(nameBuf,nameStr);
	LCD_SetColor(LCD_ReadPoint(x,y-1),nameCol);
	Show_Text(font,x,y,0,(u8*)nameBuf);
}

//设置下一个构建名（存储构建后调用）
void ui_SetGJNameNext(char* src)
{
	u8 len;
	char nameTemp[32];
	sprintf(nameTemp,"%s",src);
	len = strlen(nameTemp);
	if((nameTemp[len-1]&0x80)!=0){//汉字
		if(len < 20){
			sprintf(&nameTemp[len],"1");
		}else{
			sprintf(&nameTemp[len-1],"1");
		}
	}else{
		if(((nameTemp[len-1]>='0')&&(nameTemp[len-1] <='9'))
			||((nameTemp[len-1]>='A')&&(nameTemp[len-1] <='Z'))
			||((nameTemp[len-1]>='a')&&(nameTemp[len-1] <='z'))){
			nameTemp[len-1] += 1;
			if((nameTemp[len-1]>'9')&&(nameTemp[len-1]<'A')){
				nameTemp[len-1] = 'A';
			}else if((nameTemp[len-1]>'Z')&&(nameTemp[len-1]<'a')){
				nameTemp[len-1] = 'a';
			}else if(nameTemp[len-1]>'z'){
				nameTemp[len-1] = '0';
			}
		}
	}
	sprintf(g_SysPara.GouJianName,"%s",nameTemp);
}
//采集电量
void Get_BatPrcnt(void)
{
	if(g_bGetBatDataFlag == 1){
		g_bGetBatDataFlag = 0;
		g_GlobalPara.iBatPercent = Get_Bat_Quantity();
	}
}
//显示电量
//sta=1,立即更新;sta=0,无效,sta=2,不显示
void ui_DispBattery(u8 sta)
{
	u8 trig = 0;
	u8 dispFlag = 0;
	char dispBuf[8];
	static int dispCnt = 0;
	static int batPcnt_L = 0;//上次电量
	static u8 chargeSta = 0;
	static u8 batLowFlag = 0;
//	static int batPcntMin = 101;//电量最小值
//	static int batPcntMax = 0;//电量最大值
	u16 x,y;
	u16 batBkColor,batColor,batLowColor,batFullColor;
	
	x = 440;
	y = 9+4;
	
	batBkColor = LCD_ReadPoint(0,0);//GRAY;
	batColor = WHITE;
	batLowColor = RED;
	batFullColor = GREEN;
	
	CheckChargeStep();
	if(g_bGetBatDataFlag == 1){
		g_bGetBatDataFlag = 0;
		
		chargeSta = g_GlobalPara.iChargeSta;
		g_GlobalPara.iChargeStaRes = g_GlobalPara.iChargeSta;
		g_GlobalPara.iChargeSta = CHARGE_NO;
		
		trig = CheckChargeSta();
		g_GlobalPara.iBatPercent = Get_Bat_Quantity();
		
		if(trig != 0)dispFlag = 1;//立即更新
		if(trig == 1){//开始充电清除电量格
			g_GlobalPara.iBatPcntMax = 0;//电量最大值
			if(sta == 0){
				LCD_FillRect(x+3,y+3,19,8,batBkColor);//
			}
			dispCnt = 0;
		}
		if(trig == 2){//充电完成清除充电标志
			g_GlobalPara.iBatPercent = 101;
			LCD_FillRect(x+3,y+3,19,8,batBkColor);//
		}
		if(trig == 3){//拔掉充电器清除充电标志
			g_GlobalPara.iBatPcntMin = 101;//电量最小值
			if(sta == 0){
				LCD_FillRect(x+8,y+3,8,8,batBkColor);//
			}
			dispCnt = 0;
		}
		
		if(chargeSta == CHARGE_ING){//充电中
			dispCnt += 1;
			if(dispCnt > 11)dispCnt = 0;
			if((dispCnt == 1)||(dispCnt == 7))dispFlag = 1;
		}
		//电量低标志
		if(batLowFlag == 0){
			if(g_GlobalPara.iBatPercent < 20){
				batLowFlag = 1;
				g_GlobalPara.iBatPcntLowFlag = 1;
			}
		}else{
			if(g_GlobalPara.iBatPercent > 30){
				batLowFlag = 0;
				g_GlobalPara.iBatPcntLowFlag = 0;
			}
		}
		//显示电量
//		LCD_SetColor(WHITE,BLACK);
//		LCD_ShowNnNums(ASC24x12,200,26,0,3,g_GlobalPara.iBatPercent);

//		LCD_ShowNnNums(ASC24x12,218,50,0,1,ReadChargeSta());
//		LCD_ShowNnNums(ASC24x12,218,74,0,1,ReadChargeDoneSta());
		if(debug == 1){
			sprintf(dispBuf,"%3d%%",(g_GlobalPara.iBatPcntMin>100?100:g_GlobalPara.iBatPcntMin));
			LCD_SetColor(WHITE,BLACK);
			LCD_ShowString(ASC16x8,x,40,0,dispBuf);
		}
	}
	
	if(sta == 1){
		dispFlag = 1;
//		batPcntMin = 101;//电量最小值
		trig = CheckChargeSta();
		g_GlobalPara.iBatPercent = Get_Bat_Quantity();
		g_GlobalPara.iBatPcntMin = g_GlobalPara.iBatPercent;
	}else if(sta == 0){
		if(chargeSta == CHARGE_NO){//放电
			g_GlobalPara.iBatPcntMax = g_GlobalPara.iBatPercent;
			if((g_GlobalPara.iBatPercent != batPcnt_L)&&(g_GlobalPara.iBatPercent < g_GlobalPara.iBatPcntMin)){
				g_GlobalPara.iBatPcntMin = g_GlobalPara.iBatPercent;
				dispFlag = 1;
			}
		}
        else if(chargeSta == CHARGE_ING){//充电
			g_GlobalPara.iBatPcntMin = g_GlobalPara.iBatPercent;
			if(g_GlobalPara.iBatPercent > g_GlobalPara.iBatPcntMax){
				g_GlobalPara.iBatPcntMax = g_GlobalPara.iBatPercent;
			}
		}
        else{
			if(g_GlobalPara.iBatPercent != batPcnt_L) dispFlag = 1;
		}
	}else{//不显示
		dispFlag = 0;
		g_GlobalPara.iBatPcntMin = g_GlobalPara.iBatPercent;
		g_GlobalPara.iBatPcntMax = g_GlobalPara.iBatPercent;
		batPcnt_L = g_GlobalPara.iBatPercent;
	}
	if(dispFlag == 1){
		#ifdef BATTEST
		ui_BatDataRecord(g_GlobalPara.iBatPercent);//电量测试
		#endif
		if(chargeSta == CHARGE_NO){//放电
//			batPcntMax = g_GlobalPara.iBatPercent;
//			if(g_GlobalPara.iBatPercent < batPcntMin){
//				batPcntMin = g_GlobalPara.iBatPercent;
				//电池框
				LCD_FillRect(x,y,25,2,batColor);
				LCD_FillRect(x,y+12,25,2,batColor);
				LCD_FillRect(x,y+2,2,10,batColor);
				LCD_FillRect(x+23,y+2,2,10,batColor);
				LCD_FillRect(x+25,y+4,2,6,batColor);
				//电量格
				if(g_GlobalPara.iBatPercent >= 75){
					LCD_FillRect(x+3,y+3,4,8,batColor);
					LCD_FillRect(x+8,y+3,4,8,batColor);
					LCD_FillRect(x+13,y+3,4,8,batColor);
					LCD_FillRect(x+18,y+3,4,8,batColor);
				}
                else if(g_GlobalPara.iBatPercent >= 50){
					LCD_FillRect(x+3,y+3,4,8,batColor);
					LCD_FillRect(x+8,y+3,4,8,batColor);
					LCD_FillRect(x+13,y+3,4,8,batColor);
					LCD_FillRect(x+18,y+3,4,8,batBkColor);
				}
				else if(g_GlobalPara.iBatPercent >= 30){
					LCD_FillRect(x+3,y+3,4,8,batColor);
					LCD_FillRect(x+8,y+3,4,8,batColor);
					LCD_FillRect(x+13,y+3,4,8,batBkColor);
					LCD_FillRect(x+18,y+3,4,8,batBkColor);
				}
                else if(g_GlobalPara.iBatPercent >= 10){
					LCD_FillRect(x+3,y+3,4,8,batColor);
					LCD_FillRect(x+8,y+3,4,8,batBkColor);
					LCD_FillRect(x+13,y+3,4,8,batBkColor);
					LCD_FillRect(x+18,y+3,4,8,batBkColor);
				}
                else if(g_GlobalPara.iBatPercent >= 5){
					//电池框
					LCD_FillRect(x,y,25,2,batLowColor);
					LCD_FillRect(x,y+12,25,2,batLowColor);
					LCD_FillRect(x,y+2,2,10,batLowColor);
					LCD_FillRect(x+23,y+2,2,10,batLowColor);
					LCD_FillRect(x+25,y+4,2,6,batLowColor);
					//电量格
					LCD_FillRect(x+3,y+3,4,8,batLowColor);
					LCD_FillRect(x+8,y+3,4,8,batBkColor);
					LCD_FillRect(x+13,y+3,4,8,batBkColor);
					LCD_FillRect(x+18,y+3,4,8,batBkColor);
				}
                else if(g_GlobalPara.iBatPercent >= 2){
					//电池框
					LCD_FillRect(x,y,25,2,batLowColor);
					LCD_FillRect(x,y+12,25,2,batLowColor);
					LCD_FillRect(x,y+2,2,10,batLowColor);
					LCD_FillRect(x+23,y+2,2,10,batLowColor);
					LCD_FillRect(x+25,y+4,2,6,batLowColor);
					//电量格
					LCD_FillRect(x+3,y+3,4,8,batBkColor);
					LCD_FillRect(x+8,y+3,4,8,batBkColor);
					LCD_FillRect(x+13,y+3,4,8,batBkColor);
					LCD_FillRect(x+18,y+3,4,8,batBkColor);
				}
                else{
					PwrOffFunc();
				}
//			}
		}
		if(chargeSta == CHARGE_ING){//充电中
//			batPcntMin = g_GlobalPara.iBatPercent;
//			if(g_GlobalPara.iBatPercent > batPcntMax) batPcntMax = g_GlobalPara.iBatPercent;

			if(dispCnt == 1){
				//电池框
				LCD_FillRect(x,y,25,2,batColor);
				LCD_FillRect(x,y+12,25,2,batColor);
				LCD_FillRect(x,y+2,2,10,batColor);
				LCD_FillRect(x+23,y+2,2,10,batColor);
				LCD_FillRect(x+25,y+4,2,6,batColor);
				
				LCD_FillRect(x+3,y+3,19,8,batBkColor);//充电完成清除充电标志
				
				LCD_SetColor(batBkColor,batColor);
				LCD_ShowChinese(Font8,448,y+3,0,"电");
			}
			if(dispCnt == 7){
				//电池框
				LCD_FillRect(x,y,25,2,batColor);
				LCD_FillRect(x,y+12,25,2,batColor);
				LCD_FillRect(x,y+2,2,10,batColor);
				LCD_FillRect(x+23,y+2,2,10,batColor);
				LCD_FillRect(x+25,y+4,2,6,batColor);
				
				LCD_FillRect(x+3,y+3,19,8,batBkColor);//充电完成清除充电标志
				
				//电量格
				if(g_GlobalPara.iBatPercent >= 75){
					LCD_FillRect(x+3,y+3,4,8,batColor);
					LCD_FillRect(x+8,y+3,4,8,batColor);
					LCD_FillRect(x+13,y+3,4,8,batColor);
					LCD_FillRect(x+18,y+3,4,8,batColor);
				}
                else if(g_GlobalPara.iBatPercent >= 50){
					LCD_FillRect(x+3,y+3,4,8,batColor);
					LCD_FillRect(x+8,y+3,4,8,batColor);
					LCD_FillRect(x+13,y+3,4,8,batColor);
					LCD_FillRect(x+18,y+3,4,8,batBkColor);
				}
				else if(g_GlobalPara.iBatPercent >= 30){
					LCD_FillRect(x+3,y+3,4,8,batColor);
					LCD_FillRect(x+8,y+3,4,8,batColor);
					LCD_FillRect(x+13,y+3,4,8,batBkColor);
					LCD_FillRect(x+18,y+3,4,8,batBkColor);
				}
                else{
					LCD_FillRect(x+3,y+3,4,8,batColor);
					LCD_FillRect(x+8,y+3,4,8,batBkColor);
					LCD_FillRect(x+13,y+3,4,8,batBkColor);
					LCD_FillRect(x+18,y+3,4,8,batBkColor);
				}
			}
		}
		if(chargeSta == CHARGE_DONE){//充电完成
			g_GlobalPara.iBatPcntMin = g_GlobalPara.iBatPercent;
			//电池框
			LCD_FillRect(x,y,25,2,batFullColor);
			LCD_FillRect(x,y+12,25,2,batFullColor);
			LCD_FillRect(x,y+2,2,10,batFullColor);
			LCD_FillRect(x+23,y+2,2,10,batFullColor);
			LCD_FillRect(x+25,y+4,2,6,batFullColor);
			
			LCD_FillRect(x+3,y+3,4,8,batFullColor);
			LCD_FillRect(x+8,y+3,4,8,batFullColor);
			LCD_FillRect(x+13,y+3,4,8,batFullColor);
			LCD_FillRect(x+18,y+3,4,8,batFullColor);
		}
		
		batPcnt_L = g_GlobalPara.iBatPercent;
	}
}
//获取存储容量
u8 Get_FatFreePrcnt(void)
{
	u32 total,free;
	u8 prcnt;
	exf_getfree((u8*)"0:",&total,&free);
	prcnt = (u8)((float)free/total*100);
	return prcnt;
}

//显示菜单按钮
void ui_DispMenuBtn(MENU_TABLE_STRUCT* menuBtn,u8 highID)
{
	u8 i,iMax;
	u8 btnNum;
	u8 btnWide,strWide;
	u16 nameX,iconX,iconY,nameY,btnY;
	u16 startY;
	u16 btnHeight,btnHeightBack,iconFont;
	
	u16 bkCol;
	u16 hlCol;
	u16 btCol;
	u16 txCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
		hlCol = GOLD;
		btCol = DGRAY;
		txCol = WHITE;
	}else{
		bkCol = WHITE;
		hlCol = GOLD;
		btCol = LGRAY;
		txCol = BLACK;
	}
	
	//6个按钮，>6个不考虑
	iMax = 6;
	startY = 1;
	btnHeightBack = 48;
	btnHeight = 53;
	iconFont = Font32;
	btnWide = 56;
	
	iconX = (btnWide-iconFont)/2;
	
	//菜单栏边框
	LCD_DrawRect(0,0,btnWide,320,bkCol);
	
	//返回按钮
	LCD_FillRect(1,startY,btnWide-2,btnHeightBack,btCol);//按钮
	LCD_FillRect(1,startY+btnHeightBack,btnWide-2,1,bkCol);//按钮下边分割线
	LCD_SetColor(btCol,txCol);
	if(menuBtn[0].btnIcon != NULL){
		LCD_ShowChinese(iconFont,iconX,startY+(btnHeightBack-iconFont)/2,0,menuBtn[0].btnIcon);
	}
	//其他按钮
	for(i=0;i<iMax-1;i++){
		btnY = startY + btnHeightBack+1 + i*(btnHeight+1);
		iconY = btnY + 1;
		nameY = iconY + iconFont + 3;
		btnNum = i + 1;//按钮号
		if(btnNum < menuBtn[0].btnQnt){
			if(i < iMax-1){
				if((i > 0)&&(btnNum == highID)){//画高亮背景
					LCD_FillRect(1,btnY,btnWide-2,btnHeight,hlCol);
					LCD_SetColor(hlCol,BLACK);
				}
				else{
					LCD_FillRect(1,btnY,btnWide-2,btnHeight,btCol);
					LCD_SetColor(btCol,txCol);
				}
				LCD_FillRect(1,btnY+btnHeight,btnWide-2,1,bkCol);//按钮下边分割线
				if(menuBtn[btnNum].btnName != NULL){
					if(menuBtn[btnNum].btnIcon != NULL){
						LCD_ShowChinese(iconFont,iconX,iconY,0,menuBtn[btnNum].btnIcon);
					}
					strWide = strlen((char*)menuBtn[btnNum].btnName)*Font12/2;
					if(strWide > btnWide){
						nameX = 0;
					}
					else nameX = (btnWide-strWide)/2;
					LCD_ShowChinese(Font12,nameX,nameY,0,menuBtn[btnNum].btnName);
				}
			}
		}else{
			LCD_FillRect(1,btnY,btnWide-2,btnHeight,btCol);
			LCD_FillRect(1,btnY+btnHeight,btnWide-2,1,bkCol);//按钮下边分割线
		}
	}
}

//刷新单个按钮显示，高亮/低亮/正常
void ui_BrushMenuOneBtn(MENU_TABLE_STRUCT* menuBtn,u8 btnSta)
{
//	u8 iMax;
	u8 btnID;
	u8 btnWide,strWide;
	u16 nameX,iconX,iconY,nameY,btnY;
	u16 startY;
	u16 btnHeight,btnHeightBack,iconFont;
	
	u16 hlCol;
	u16 loCol;
	u16 btCol;
	u16 txCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		hlCol = GOLD;
		loCol = GRAY;
		btCol = DGRAY;
		txCol = WHITE;
	}else{
		hlCol = GOLD;
		loCol = GRAY;
		btCol = LGRAY;
		txCol = BLACK;
	}
	
	btnID = menuBtn[0].btnID;
	
//	iMax = 6;
	startY = 1;
	btnHeightBack = 48;
	btnHeight = 53;
	iconFont = Font32;
	btnWide = 56;
	
	if(btnID > 0){
		iconX = (btnWide-iconFont)/2;
		btnY = startY + btnHeightBack+1 + (btnID-1)*(btnHeight+1);
		iconY = btnY + 1;
		nameY = iconY + iconFont + 3;
		
		if(btnSta == HIGHLIGHT){
			LCD_FillRect(1,btnY,btnWide-2,btnHeight,hlCol);
			LCD_SetColor(hlCol,BLACK);
		}else if(btnSta == LOWLIGHT){
			LCD_FillRect(1,btnY,btnWide-2,btnHeight,btCol);
			LCD_SetColor(btCol,loCol);
		}else{
			LCD_FillRect(1,btnY,btnWide-2,btnHeight,btCol);
			LCD_SetColor(btCol,txCol);
		}
		
		if(menuBtn[0].btnName != NULL){
			if(menuBtn[0].btnIcon != NULL){
				LCD_ShowChinese(iconFont,iconX,iconY,0,menuBtn[0].btnIcon);
			}
			strWide = strlen((char*)menuBtn[0].btnName)*Font12/2;
			if(strWide > btnWide){
				nameX = 0;
			}else nameX = (btnWide-strWide)/2;
			LCD_ShowChinese(Font12,nameX,nameY,0,menuBtn[0].btnName);
		}
	}else{
		return;
	}
}

//电量测试，记录电量和时间
void ui_BatDataRecord(int pcnt)
{	
	char batPcntBuf[24];
	int byteOffset = 0;
	
	memset(batPcntBuf, 0, sizeof(batPcntBuf));
	sprintf(batPcntBuf,"%4.2f,%3d%%,%4d,%4d;\r\n",g_GlobalPara.fBatVoltage,pcnt,g_BatDataTest.iMinutCount,g_BatDataTest.iSaveCount);
	byteOffset = g_BatDataTest.iSaveCount * strlen(batPcntBuf);
	
	if(g_BatDataTest.iSaveCount == 0){
		f_open(file,g_batFileBuf, FA_WRITE | FA_READ);
		f_write(file,&batPcntBuf,strlen(batPcntBuf),&bw);
		f_close(file);
		g_BatDataTest.iSaveCount += 1;
	}else{
		f_open(file,g_batFileBuf, FA_WRITE | FA_READ);
		f_lseek(file,byteOffset);
		f_write(file,&batPcntBuf,strlen(batPcntBuf),&bw);
		f_close(file);
		g_BatDataTest.iSaveCount += 1;	
	}
}

