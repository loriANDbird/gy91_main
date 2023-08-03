/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：
    1. ui_Start 界面初始化
    2. ui_Start_Run 函数实现

    
更新记录：
    1. 2018.03.01 创建
    
*******************************************************************************/
#include "ui_Start.h"
#include "Config_ui.h"

u8 Biaoding_Fun(void);
void DispTempera(void);

void ui_Biaoding_InitDisp(void)
{
	u16 bdBkCol;
	u16 bdTxCol;
	u16 bdBarCol;
	u16 bdDioCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bdBkCol = BLACK;
		bdTxCol = WHITE;
		bdBarCol = GRAY;
		bdDioCol = DGRAY;
	}else{
		bdBkCol = WHITE;
		bdTxCol = BLACK;
		bdBarCol = DGRAY;
		bdDioCol = LGRAY;
	}
	
	//状态栏
	ui_DispTitleBar("仪器标定",1);
	
	LCD_FillRect(100,56,340,36,bdBarCol);//标题栏
	LCD_SetColor(bdBarCol,WHITE);
	LCD_ShowChinese(Font16,238,66,0,"仪器标定");
	
	LCD_FillRect(100,92,340,168,bdDioCol);//弹窗
	LCD_FillRect(100,92,340,1,bdBkCol);//分割线
	
	LCD_FillRect(100,260,170,36,GOLD);//确定按钮
	LCD_FillRect(270,260,170,36,GRAY);//取消按钮
	LCD_FillRect(100,259,340,1,bdBkCol);//分割线
	LCD_FillRect(269,260,1,36,bdBkCol);//分割线
	
	LCD_SetColor(GOLD,BLACK);
	LCD_ShowChinese(Font16,168,270,0,"标定");
	LCD_SetColor(GRAY,bdTxCol);
	LCD_ShowChinese(Font16,338,270,0,"取消");
	
	LCD_SetColor(bdDioCol,GOLD);
	LCD_ShowText(Font20,220,144,0,"请远离金属");
	LCD_SetColor(bdDioCol,bdTxCol);
	LCD_ShowText(Font16,206,168,0,"按    键开始标定");
	LCD_SetColor(bdDioCol,GOLD);
	LCD_ShowText(Font16,222,168,0,"标定");
	
	
	//页面左侧菜单表
	if(g_GlobalPara.ucFromPageNum == UI_SAMPLE10){//快速检测
		MENU_TABLE_STRUCT BdMenu[6] = {
			{0,6,"徊",NULL},
			{1,6,"参","参数"},
			{2,6,"显","显示"},
			{3,6,"况","工况"},
			{4,6,"析","热力图"},
			{5,6,"标","标定"}
		};
		//菜单显示
		ui_DispMenuBtn(BdMenu,5);
		
		//按钮初始化
		g_u8ButtonNums = 8;
		struct BUTTON BdBtn[8] = {
			//sX,  sY,  eX,  eY,
			{100, 260, 269, 295, 0,0},//0, 确定
			{270, 260, 439, 295, 0,0},//1, 取消
			{  0,   0,  59,  49, 0,0},//2, 返回
			{  0,  50,  59, 103, 0,0},//3, 参数
			{  0, 104,  59, 157, 0,0},//4, 显示
			{  0, 158,  59, 211, 0,0},//5, 工况
			{  0, 212,  59, 265, 0,0},//6, 分析
			{  0, 266,  59, 319, 0,0} //7, 标定
		};
		memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
	}
	//页面左侧菜单表
	else if(g_GlobalPara.ucFromPageNum == UI_SAMPJGJ){//JGJ检测
		MENU_TABLE_STRUCT BdMenu[5] = {
			{0,5,"徊",NULL},
			{1,5,"参","参数"},
			{2,5,"横","横向"},
			{3,5,"存","存点"},
			{4,5,"标","标定"}
		};
		
		if(g_SysPara.JGJScanDir == 0){
			BdMenu[2].btnIcon = "横";
			BdMenu[2].btnName = "横向";
		}else{
			BdMenu[2].btnIcon = "纵";
			BdMenu[2].btnName = "纵向";
		}
		//菜单显示
		ui_DispMenuBtn(BdMenu,4);
		
		//按钮初始化
		g_u8ButtonNums = 7;
		struct BUTTON BdBtn[7] = {
			//sX,  sY,  eX,  eY,
			{100, 260, 269, 295, 0,0},//0, 确定
			{270, 260, 439, 295, 0,0},//1, 取消
			{  0,   0,  59,  49, 0,0},//2, 返回
			{  0,  50,  59, 103, 0,0},//3, 参数
			{  0, 104,  59, 157, 0,0},//4, 横向
			{  0, 158,  59, 211, 0,0},//5, 存点
			{  0, 212,  59, 265, 0,0} //6, 标定
		};
		memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
	}
	//页面左侧菜单表
	else if(g_GlobalPara.ucFromPageNum == UI_SAMPBLOCK){//区块检测
		MENU_TABLE_STRUCT BdMenu[3] = {
			{0,3,"徊",NULL},
			{1,3,"默","默认值"},
			{2,3,"标","标定"}
		};
		//菜单显示
		ui_DispMenuBtn(BdMenu,2);
		
		//按钮初始化
		g_u8ButtonNums = 5;
		struct BUTTON BdBtn[5] = {
			//sX,  sY,  eX,  eY,
			{100, 260, 269, 295, 0,0},//0, 确定
			{270, 260, 439, 295, 0,0},//1, 取消
			{  0,   0,  59,  49, 0,0},//2, 返回
			{  0,  50,  59, 103, 0,0},//3, 默认值
			{  0, 104,  59, 157, 0,0} //4, 标定
		};
		memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
	}
	//页面左侧菜单表
	else if(g_GlobalPara.ucFromPageNum == UI_VERIFY){//仪器检定
		MENU_TABLE_STRUCT BdMenu[3] = {
			{0,3,"徊",NULL},
			{1,3,"参","参数"},
			{2,3,"标","标定"}
		};
		//菜单显示
		ui_DispMenuBtn(BdMenu,2);
		
		//按钮初始化
		g_u8ButtonNums = 5;
		struct BUTTON BdBtn[5] = {
			//sX,  sY,  eX,  eY,
			{100, 260, 269, 295, 0,0},//0, 确定
			{270, 260, 439, 295, 0,0},//1, 取消
			{  0,   0,  59,  49, 0,0},//2, 返回
			{  0,  50,  59, 103, 0,0},//3, 参数
			{  0, 104,  59, 157, 0,0} //4, 标定
		};
		memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
	}
	//页面左侧菜单表
	else if(g_GlobalPara.ucFromPageNum == UI_CONDITION){//工况
		MENU_TABLE_STRUCT BdMenu[2] = {
			{0,2,"徊",NULL},
			{1,2,"参","参数"}
		};
		//菜单显示
		ui_DispMenuBtn(BdMenu,2);
		
		//按钮初始化
		g_u8ButtonNums = 4;
		struct BUTTON BdBtn[4] = {
			//sX,  sY,  eX,  eY,
			{100, 260, 269, 295, 0,0},//0, 确定
			{270, 260, 439, 295, 0,0},//1, 取消
			{  0,   0,  59,  49, 0,0},//2, 返回
			{  0,  50,  59, 103, 0,0} //3, 参数
		};
		memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
	}
}

//---------------------------------------------
//UI运行函数
void ui_Biaoding_Run(void)
{
	u8 i;
	u8 res;
	u16 bkCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
	}else{
		bkCol = WHITE;
	}
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		
//		DispTempera();//显示温度
		
		if(g_u16KeyValBuf == KEY_OK){

		}
		if(g_u16KeyValBuf == KEY_BACK){
			SendUiNo(g_GlobalPara.ucFromPageNum);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_OK)){//确定
//			LCD_FillRect(56,32,424,288,bkCol);//清空弹窗
			
			res = Biaoding_Fun();
			
			if(res == 1){//标定完成
				
				//记录温度和电量
				g_SysPara.fTemperatureBDLast = g_GlobalPara.fTemperature;
				g_SysPara.iBatPercentBDLast = g_GlobalPara.iBatPercent;
				SaveSysPara();
				
				SendUiNo(g_GlobalPara.ucFromPageNum);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}else{//取消
				for(i=0;i<10;i++){//清除误触摸
					ui_ButtonTask(0);
					Sleep_ms(10);
				}
				ui_Biaoding_InitDisp();
				if(res == 2){
					//深色背景
					if(g_SysPara.iTheme == THEME_DARK){
						LCD_SetColor(DGRAY,WHITE);
						LCD_ShowText(Font16,206,168,0,"按    键开始标定");
						LCD_SetColor(DGRAY,RED);
						LCD_ShowText(Font16,222,168,0,"标定");
					}else{
						LCD_SetColor(LGRAY,BLACK);
						LCD_ShowText(Font16,206,168,0,"按    键开始标定");
						LCD_SetColor(LGRAY,RED);
						LCD_ShowText(Font16,222,168,0,"标定");
					}
				}
			}
		}
		if(g_u8TouchResult == 1){//取消
			SendUiNo(g_GlobalPara.ucFromPageNum);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		
		//返回
		if(g_u8TouchResult == 2){
			SendUiNo(g_GlobalPara.ucFromPageNum);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		if(g_GlobalPara.ucFromPageNum == UI_SAMPLE10){//快速检测
			//参数
			if(g_u8TouchResult == 3){
				SendUiNo(UI_SAMPSET);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//显示
			if(g_u8TouchResult == 4){
				g_GlobalPara.ucDispModepage = 0;//检测显示
				SendUiNo(UI_SAMPDISP);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//工况
			if(g_u8TouchResult == 5){
				SendUiNo(UI_CONDITION);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//分析
			if(g_u8TouchResult == 6){
				SendUiNo(UI_ANALYZE);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//标定
			if(g_u8TouchResult == 7){
				
			}
		}
		if(g_GlobalPara.ucFromPageNum == UI_SAMPJGJ){//JGJ检测
			//参数
			if(g_u8TouchResult == 3){
				SendUiNo(UI_SAMPSET);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//横向
			if(g_u8TouchResult == 4){
//				g_GlobalPara.ucDispModepage = 0;//检测显示
//				SendUiNo(UI_SAMPDISP);
//				LCD_FillRect(60,33,420,287,WHITE);
//				return;
			}
			//存点
			if(g_u8TouchResult == 5){
//				SendUiNo(UI_ANALYZE);
//				LCD_FillRect(60,0,420,320,WHITE);
//				return;
			}
			//标定
			if(g_u8TouchResult == 6){
				
			}
		}
		if(g_GlobalPara.ucFromPageNum == UI_SAMPBLOCK){//区块检测
			//默认值
			if(g_u8TouchResult == 3){
//				SendUiNo(UI_SAMPSET);
//				LCD_FillRect(60,33,480,287,WHITE);
//				return;
			}
			//标定
			if(g_u8TouchResult == 4){
				
			}
		}
		if(g_GlobalPara.ucFromPageNum == UI_VERIFY){//仪器检定
			//参数
			if(g_u8TouchResult == 3){
				SendUiNo(UI_SAMPSET);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//标定
			if(g_u8TouchResult == 4){
				
			}
		}
	}
}

u8 Biaoding_Fun(void)
{
	u8 biaodingSta = 0;//0：标定中，1：标定完成，2：标定失败
	int exitCnt = 0;
	int overTimeCnt = 0;
	int ellipsisCnt = 1;
	
	int sumCnt = 0;
	int ch1ZeroSum1 = 0;
	int ch2ZeroSum2 = 0;
	int ch3ZeroSum3 = 0;
	int ch4ZeroSum4 = 0;
	int ch5ZeroSum5 = 0;
	int ch6ZeroSum6 = 0;
	
	int ch1AdcValZero = 0;
	int ch2AdcValZero = 0;
	int ch3AdcValZero = 0;
	int ch4AdcValZero = 0;
	int ch5AdcValZero = 0;
	int ch6AdcValZero = 0;
	
	u16 bdzBkCol;
	u16 bdzTxCol;
//	u16 bdzBarCol;
	u16 bdzDioCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bdzBkCol = BLACK;
		bdzTxCol = WHITE;
//		bdzBarCol = GRAY;
		bdzDioCol = DGRAY;
	}else{
		bdzBkCol = WHITE;
		bdzTxCol = BLACK;
//		bdzBarCol = DGRAY;
		bdzDioCol = LGRAY;
	}
	
	LCD_SetColor(bdzDioCol,GOLD);
	LCD_ShowText(Font20,220,144,0,"  标定中  ");
	LCD_ShowText(Font16,206,168,0,"                ");
	
	//按钮
	LCD_FillRect(100,260,170,36,DGOLD);//确定按钮
	LCD_FillRect(270,260,170,36,GRAY);//取消按钮
	LCD_FillRect(269,260,1,36,bdzBkCol);//分割线
	
	LCD_SetColor(DGOLD,DGRAY);
	LCD_ShowChinese(Font16,168,270,0,"标定");
	LCD_SetColor(GRAY,bdzTxCol);
	LCD_ShowChinese(Font16,338,270,0,"取消");
	
	//按钮初始化
	g_u8ButtonNums = 1;
	struct BUTTON BdBtn[1] = {
		//sX,  sY,  eX,  eY,
		{270, 260, 439, 295, 0,0} //0, 取消
	};   
	memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
	
	
	//进度条
	LCD_FillRect(200,192,140,8,WHITE);
	
	while(1)
	{
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		
		
		//标定成功
		if(g_u16KeyValBuf == KEY_OK){
			if(biaodingSta == 0){//标定取消
				
			}else if(biaodingSta == 1){//标定完成
				LCD_FillRect(56,32,424,288,bdzBkCol);
				g_SysPara.uiCh1Zero = ch1AdcValZero;
				g_SysPara.uiCh2Zero = ch2AdcValZero;
				g_SysPara.uiCh3Zero = ch3AdcValZero;
				g_SysPara.uiCh4Zero = ch4AdcValZero;
				g_SysPara.uiCh5Zero = ch5AdcValZero;
				g_SysPara.uiCh6Zero = ch6AdcValZero;
				SaveSysPara();
				LCD_FillRect(56,32,424,288,bdzBkCol);
				return 1;
			}else{//标定失败
				
			}
		}
		//取消
		if(g_u16KeyValBuf == KEY_BACK){
			
			if(biaodingSta == 0){//标定取消
				return 0;
			}else if(biaodingSta == 1){//标定完成
				g_SysPara.uiCh1Zero = ch1AdcValZero;
				g_SysPara.uiCh2Zero = ch2AdcValZero;
				g_SysPara.uiCh3Zero = ch3AdcValZero;
				g_SysPara.uiCh4Zero = ch4AdcValZero;
				g_SysPara.uiCh5Zero = ch5AdcValZero;
				g_SysPara.uiCh6Zero = ch6AdcValZero;
				SaveSysPara();
				LCD_FillRect(56,32,424,288,bdzBkCol);
				return 1;
			}else{//标定失败
				return 2;
			}
		}
		//返回或确定
		if((g_u8TouchResult == 0)||(g_u8TouchResult == 1)){
			
			if(biaodingSta == 0){//标定取消
				return 0;
			}else if(biaodingSta == 1){//标定完成
				g_SysPara.uiCh1Zero = ch1AdcValZero;
				g_SysPara.uiCh2Zero = ch2AdcValZero;
				g_SysPara.uiCh3Zero = ch3AdcValZero;
				g_SysPara.uiCh4Zero = ch4AdcValZero;
				g_SysPara.uiCh5Zero = ch5AdcValZero;
				g_SysPara.uiCh6Zero = ch6AdcValZero;
				SaveSysPara();
				LCD_FillRect(56,32,424,288,bdzBkCol);
				return 1;
			}else{//标定失败
				return 2;
			}
		}
		//标定中
		if(biaodingSta == 0){
			//数据采集
			if(overTimeCnt%8 == 0){
				if(Get_Adc_DataOrig() == 1){//
					ch1ZeroSum1 += g_iCh1AdcValOrig;
					ch2ZeroSum2 += g_iCh2AdcValOrig;
					ch3ZeroSum3 += g_iCh3AdcValOrig;
					ch4ZeroSum4 += g_iCh4AdcValOrig;
					ch5ZeroSum5 += g_iCh5AdcValOrig;
					ch6ZeroSum6 += g_iCh6AdcValOrig;
					
					//进度条
					LCD_FillRect(200+sumCnt,192,1,8,GOLD);
//					LCD_SetColor(DGRAY,GOLD);
//					LCD_ShowNums(Font16,350,186,0,sumCnt);
					sumCnt += 1;
				}
			}
			overTimeCnt += 1;
			ellipsisCnt += 1;
			
			//采集完成
			if(sumCnt >= 140){
				//进度条
				LCD_FillRect(200+sumCnt,192,1,8,GOLD);
				Sleep_ms(200);
//				LCD_SetColor(DGRAY,GOLD);
//				LCD_ShowNums(Font16,350,186,0,sumCnt);
				
				ch1AdcValZero = ch1ZeroSum1/sumCnt;
				ch2AdcValZero = ch2ZeroSum2/sumCnt;
				ch3AdcValZero = ch3ZeroSum3/sumCnt;
				ch4AdcValZero = ch4ZeroSum4/sumCnt;
				ch5AdcValZero = ch5ZeroSum5/sumCnt;
				ch6AdcValZero = ch6ZeroSum6/sumCnt;
				
				LCD_SetColor(bdzDioCol,GREEN);
				LCD_ShowChinese(Font20,230,144,0,"标定成功");
				
				//刷新按钮
				LCD_FillRect(100,260,170,36,GOLD);//确定按钮
				LCD_FillRect(270,260,170,36,GRAY);//取消按钮
				LCD_FillRect(269,260,1,36,BLACK);//分割线
				
				LCD_SetColor(GOLD,BLACK);
				LCD_ShowChinese(Font16,168,270,0,"确定");
				LCD_SetColor(GRAY,bdzTxCol);
				LCD_ShowChinese(Font16,338,270,0,"取消");
				
				//按钮初始化
				g_u8ButtonNums = 2;
				struct BUTTON BdBtn[2] = {
					//sX,  sY,  eX,  eY,
					{100, 260, 269, 295, 0,0},//0, 标定
					{270, 260, 439, 295, 0,0} //0, 取消
				};
				memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
				
				biaodingSta = 1;
			}
			//标定失败
			if((overTimeCnt > 1000)&&(sumCnt < 10)){
				
				LCD_SetColor(bdzDioCol,GREEN);
				LCD_ShowChinese(Font20,230,144,0,"标定失败");
				
				//刷新按钮
				LCD_FillRect(100,260,170,36,GOLD);//确定按钮
				LCD_FillRect(270,260,170,36,GRAY);//取消按钮
				LCD_FillRect(269,260,1,36,bdzBkCol);//分割线
				
				LCD_SetColor(GOLD,BLACK);
				LCD_ShowChinese(Font16,168,270,0,"确定");
				LCD_SetColor(GRAY,bdzTxCol);
				LCD_ShowChinese(Font16,338,270,0,"取消");
					
				//按钮初始化
				g_u8ButtonNums = 2;
				struct BUTTON BdBtn[2] = {
					//sX,  sY,  eX,  eY,
					{100, 260, 269, 295, 0,0},//0, 标定
					{270, 260, 439, 295, 0,0} //0, 取消
				};
				memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
				
				biaodingSta = 2;
			}
			//进度条
			
//			if(ellipsisCnt > 138){
//				ellipsisCnt = 1;
//				LCD_SetColor(WHITE,BLACK);
//				LCD_ShowString(ASC20x10,220+64,100,0,"      ");
//			}
//			if(ellipsisCnt%20 == 0){
//				LCD_SetColor(WHITE,BLACK);
//				LCD_ShowString(ASC20x10,220+64+(ellipsisCnt/20-1)*10,100,0,".");
//			}
		}
		//标定成功计时退出
		if(biaodingSta == 1){
			exitCnt += 1;
			if(exitCnt > 500){
				g_SysPara.uiCh1Zero = ch1AdcValZero;
				g_SysPara.uiCh2Zero = ch2AdcValZero;
				g_SysPara.uiCh3Zero = ch3AdcValZero;
				g_SysPara.uiCh4Zero = ch4AdcValZero;
				g_SysPara.uiCh5Zero = ch5AdcValZero;
				g_SysPara.uiCh6Zero = ch6AdcValZero;
				SaveSysPara();
				
				LCD_FillRect(56,32,424,288,bdzBkCol); 
				return 1;
			}
		}
		//标定失败
		if(biaodingSta == 2){
			exitCnt += 1;
			if(exitCnt > 500){
				LCD_FillRect(56,32,424,288,bdzBkCol);
				return 2;
			}
		}
		Sleep_ms(10);
	}
}

//显示温度
void DispTempera(void)
{
	char buf[12];
	if(g_bTemperaDispFlag == 1){
		g_bTemperaDispFlag = 0;
		sprintf(buf,"%5.01f℃",g_GlobalPara.fTemperature);
		LCD_SetColor(LCD_ReadPoint(330,5),WHITE);
		LCD_ShowText(Font20,330,6,0,(u8*)buf);
//		LCD_ShowFloat(ASC20x10,340,6,0,5,1,g_GlobalPara.fTemperature);
	}
}
