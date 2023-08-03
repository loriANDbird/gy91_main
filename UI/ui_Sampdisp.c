/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：
    1. ui_Start 界面初始化
    2. ui_Start_Run 函数实现

    
更新记录：
    1. 2018.03.01 创建
    
*******************************************************************************/
#include "ui_Sampdisp.h"
#include "Config_ui.h"

//u8 SampleDispMode;//检测界面4种显示模式0-3
//u8 AnalyzeDispMode;//分析界面4种显示模式0-3
static u8 dispModeLast = 0;

static u8 DispModeTemp;
struct DISPOPTION *DispModeTable;

//扫描模式表
struct DISPOPTION SampDispModeTable[] = {
	{MIAO_POU, "瞄", "瞄准框", "剖", "剖面图"},
	{MIAO_BO , "瞄", "瞄准框", "波", "波形图"},
	{TIAO_POU, "条", "条形图", "剖", "剖面图"},
	{TIAO_BO , "条", "条形图", "波", "波形图"}
};

struct DISPOPTION AnalyzeDispModeTABLE[] = {
	{RELI_POU, "热", "热力图", "剖", "剖面图"},
	{RELI_BO , "热", "热力图", "波", "波形图"},
	{TIAO_POU, "条", "条形图", "剖", "剖面图"},
	{TIAO_BO , "条", "条形图", "波", "波形图"},
};

void ui_Sampdisp_InitDisp(void)
{
	u8 dispNum;
	u8 i,j;
	
	//状态栏
//	if(g_SysPara.iTheme == THEME_DARK){//深色背景
//		LCD_FillRect(57,1,422,31,GRAY);
//		LCD_SetColor(GRAY,WHITE);
//		LCD_ShowChinese(Font16,238,9,0,"显示设置");
//	}else{
//		LCD_FillRect(57,1,422,31,DGRAY);
//		LCD_SetColor(DGRAY,WHITE);
//		LCD_ShowChinese(Font16,238,9,0,"显示设置");
//	}
	ui_DispTitleBar("显示设置",0);
	
	if(g_GlobalPara.ucDispModepage == 0){//快速检测
		DispModeTemp = g_SysPara.SampleDispMode;
		DispModeTable = SampDispModeTable;
	}else{//分析
		DispModeTemp = g_SysPara.AnalyzeDispMode;
		DispModeTable = AnalyzeDispModeTABLE;
	}
	
	for(i=0;i<2;i++){
		for(j=0;j<2;j++){
			dispNum = i*2+j;
			if(dispNum == DispModeTemp){
//				LCD_DrawRect(120+180*j,68+126*i,120,90,GRAY);
				LCD_FillRect(120+180*j+1,68+126*i+1,118,88,GOLD);
				LCD_SetColor(GOLD,BLACK);
			}else{
//				LCD_DrawRect(120+180*j,68+126*i,120,90,GRAY);
				LCD_FillRect(120+180*j+1,68+126*i+1,118,88,LLGRAY);
				LCD_SetColor(LLGRAY,BLACK);
			}
			LCD_ShowChinese(Font32,135+180*j,78+126*i,0,DispModeTable[dispNum].icon1);
			LCD_ShowChinese(Font16,172+180*j,86+126*i,0,DispModeTable[dispNum].Name1);
			LCD_ShowChinese(Font32,135+180*j,116+126*i,0,DispModeTable[dispNum].icon2);
			LCD_ShowChinese(Font16,172+180*j,124+126*i,0,DispModeTable[dispNum].Name2);
		}
	}
	
	if(g_GlobalPara.ucDispModepage == 0){//快速检测
		//页面左侧菜单表
		MENU_TABLE_STRUCT Samp10Menu[6] = {
			{0,6,"徊",NULL},
			{1,6,"参","参数"},
			{2,6,"显","显示"},
			{3,6,"况","工况"},
			{4,6,"析","热力图"},
			{5,6,"标","标定"}
		};
		//菜单显示
		ui_DispMenuBtn(Samp10Menu,2);
		
		//按钮初始化
		g_u8ButtonNums = 10;
		struct BUTTON SampBtn[10] = {
			//sX,  sY,  eX,  eY,
			{  0,   0,  59,  49, 0,0},//0, 返回
			{  0,  50,  59, 103, 0,0},//1, 参数
			{  0, 104,  59, 157, 0,0},//2, 显示
			{  0, 158,  59, 211, 0,0},//3, 工况
			{  0, 212,  59, 265, 0,0},//4, 分析
			{  0, 266,  59, 319, 0,0},//5, 标定
			
			{120,  68, 239, 157, 0,0},//6, 显示1
			{300,  68, 419, 157, 0,0},//7, 显示2
			{120, 194, 239, 284, 0,0},//8, 显示3
			{300, 194, 419, 283, 0,0} //9, 显示4
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}else{//分析
		if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){//浏览界面
			//页面左侧菜单表
			MENU_TABLE_STRUCT AnalyzeMenu[5] = {
				{0,5,"徊",NULL},
				{1,5,"显","显示"},
				{2,5,"益","增益"},
				{3,5,"力","热力"},
				{4,5,"统","统计"}
			};
			//菜单显示
			ui_DispMenuBtn(AnalyzeMenu,1);
		
			//按钮初始化
			g_u8ButtonNums = 9;
			struct BUTTON SampBtn[9] = {
				//sX,  sY,  eX,  eY,
				{  0,   0,  59,  49, 0,0},//0, 返回
				{  0,  50,  59, 103, 0,0},//1, 显示
				{  0, 104,  59, 157, 0,0},//2, 增益
				{  0, 158,  59, 211, 0,0},//3, 热力
				{  0, 212,  59, 265, 0,0},//4, 标定 无效
				{120,  68, 239, 157, 0,0},//5, 显示1
				{300,  68, 419, 157, 0,0},//6, 显示2
				{120, 194, 239, 284, 0,0},//7, 显示3
				{300, 194, 419, 283, 0,0} //8, 显示4
			};
			memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
		}else{
			//页面左侧菜单表
			MENU_TABLE_STRUCT Samp10Menu[4] = {
				{0,4,"徊",NULL},
				{1,4,"显","显示"},
				{2,4,"益","增益"},
				{3,4,"力","热力"}
			};
			//菜单显示
			ui_DispMenuBtn(Samp10Menu,1);
			
			//按钮初始化
			g_u8ButtonNums = 9;
			struct BUTTON SampBtn[9] = {
				//sX,  sY,  eX,  eY,
				{  0,   0,  59,  49, 0,0},//0, 返回
				{  0,  50,  59, 103, 0,0},//1, 显示
				{  0, 104,  59, 157, 0,0},//2, 增益
				{  0, 158,  59, 211, 0,0},//3, 热力
				{  0, 212,  59, 265, 0,0},//4, 标定 无效
				{120,  68, 239, 157, 0,0},//5, 显示1
				{300,  68, 419, 157, 0,0},//6, 显示2
				{120, 194, 239, 284, 0,0},//7, 显示3
				{300, 194, 419, 283, 0,0} //8, 显示4
			};
			memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
		}
	}
	
	dispModeLast = DispModeTemp;
}

//---------------------------------------------
//UI运行函数
void ui_Sampdisp_Run(void)
{	
	u8 i,j;
	
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
		
		if(g_u16KeyValBuf == KEY_OK){

		}
		if(g_u16KeyValBuf == KEY_BACK){
			SendUiNo(UI_SAMPLE10);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		
		if(g_GlobalPara.ucDispModepage == 0){//快速检测
			if((g_u8TouchResult >= 6)&&(g_u8TouchResult <= 9)){
				DispModeTemp = g_u8TouchResult-6;
				if(DispModeTemp != dispModeLast){
					if(dispModeLast < 2){
						i = 0;
						j = dispModeLast%2;
					}else{
						i = 1;
						j = dispModeLast%2;
					}
					
					LCD_FillRect(120+180*j+1,68+126*i+1,118,88,LLGRAY);
					LCD_SetColor(LLGRAY,BLACK);
					LCD_ShowChinese(Font32,135+180*j,78+126*i,0,DispModeTable[dispModeLast].icon1);
					LCD_ShowChinese(Font16,172+180*j,86+126*i,0,DispModeTable[dispModeLast].Name1);
					LCD_ShowChinese(Font32,135+180*j,116+126*i,0,DispModeTable[dispModeLast].icon2);
					LCD_ShowChinese(Font16,172+180*j,124+126*i,0,DispModeTable[dispModeLast].Name2);
					
					if(DispModeTemp < 2){
						i = 0;
						j = DispModeTemp%2;
					}else{
						i = 1;
						j = DispModeTemp%2;
					}
					LCD_FillRect(120+180*j+1,68+126*i+1,118,88,GOLD);
					LCD_SetColor(GOLD,BLACK);
					LCD_ShowChinese(Font32,135+180*j,78+126*i,0,DispModeTable[DispModeTemp].icon1);
					LCD_ShowChinese(Font16,172+180*j,86+126*i,0,DispModeTable[DispModeTemp].Name1);
					LCD_ShowChinese(Font32,135+180*j,116+126*i,0,DispModeTable[DispModeTemp].icon2);
					LCD_ShowChinese(Font16,172+180*j,124+126*i,0,DispModeTable[DispModeTemp].Name2);
				}
				dispModeLast = DispModeTemp;
				if(g_GlobalPara.ucDispModepage == 0){//快速检测
					g_SysPara.SampleDispMode = DispModeTemp;
				}else{//分析显示
					g_SysPara.AnalyzeDispMode = DispModeTemp;
				}
				
				SaveSysPara();
				//直接返回
				SendUiNo(UI_SAMPLE10);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//返回
			if(g_u8TouchResult == 0){
				SendUiNo(UI_SAMPLE10);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//参数
			if(g_u8TouchResult == 1){
				SendUiNo(UI_SAMPSET);
				g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//显示
			if(g_u8TouchResult == 2){
//				SendUiNo(UI_ANALYZE);
//				LCD_FillRect(60,33,420,287,WHITE);
			}
			//工况
			if(g_u8TouchResult == 3){
				SendUiNo(UI_CONDITION);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//分析
			if(g_u8TouchResult == 4){
				SendUiNo(UI_ANALYZE);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//标定
			if(g_u8TouchResult == 5){
				SendUiNo(UI_BIAODING);
				g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
		}else{//分析
			if((g_u8TouchResult >= 5)&&(g_u8TouchResult <= 8)){
				DispModeTemp = g_u8TouchResult-5;
				if(DispModeTemp != dispModeLast){
					if(dispModeLast < 2){
						i = 0;
						j = dispModeLast%2;
					}else{
						i = 1;
						j = dispModeLast%2;
					}
					
					LCD_FillRect(120+180*j+1,68+126*i+1,118,88,LLGRAY);
					LCD_SetColor(LLGRAY,BLACK);
					LCD_ShowChinese(Font32,135+180*j,78+126*i,0,DispModeTable[dispModeLast].icon1);
					LCD_ShowChinese(Font16,172+180*j,86+126*i,0,DispModeTable[dispModeLast].Name1);
					LCD_ShowChinese(Font32,135+180*j,116+126*i,0,DispModeTable[dispModeLast].icon2);
					LCD_ShowChinese(Font16,172+180*j,124+126*i,0,DispModeTable[dispModeLast].Name2);
					
					if(DispModeTemp < 2){
						i = 0;
						j = DispModeTemp%2;
					}else{
						i = 1;
						j = DispModeTemp%2;
					}
					LCD_FillRect(120+180*j+1,68+126*i+1,118,88,GOLD);
					LCD_SetColor(GOLD,BLACK);
					LCD_ShowChinese(Font32,135+180*j,78+126*i,0,DispModeTable[DispModeTemp].icon1);
					LCD_ShowChinese(Font16,172+180*j,86+126*i,0,DispModeTable[DispModeTemp].Name1);
					LCD_ShowChinese(Font32,135+180*j,116+126*i,0,DispModeTable[DispModeTemp].icon2);
					LCD_ShowChinese(Font16,172+180*j,124+126*i,0,DispModeTable[DispModeTemp].Name2);
				}
				dispModeLast = DispModeTemp;
				if(g_GlobalPara.ucDispModepage == 0){//快速检测
					g_SysPara.SampleDispMode = DispModeTemp;
				}else{//分析显示
					g_SysPara.AnalyzeDispMode = DispModeTemp;
				}
				
				SaveSysPara();
				//直接返回
				SendUiNo(UI_ANALYZE);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//返回
			if(g_u8TouchResult == 0){
				SendUiNo(UI_ANALYZE);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//显示
			if(g_u8TouchResult == 1){
//				SendUiNo(UI_ANALYZE);
//				LCD_FillRect(60,33,420,287,WHITE);
			}
			//增益
			if(g_u8TouchResult == 2){
				SendUiNo(UI_GAIN);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//热力
			if(g_u8TouchResult == 3){
				SendUiNo(UI_HEAT);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//统计
			if(g_u8TouchResult == 4){
				if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){
					LCD_FillRect(56,32,424,288,bkCol);
					SampleDataView_Fun();
					ui_Sampdisp_InitDisp();
				}
			}
		}
	}
}
