/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：
    1. ui_Start 界面初始化
    2. ui_Start_Run 函数实现

更新记录：
    1. 2018.03.01 创建
*******************************************************************************/
#include "ui_Heat.h"
#include "Config_ui.h"

extern SYS_PARA_TABLE_STRUCT* SampTable;
extern char *Condition[5];

void ui_Gain_InitDisp(void)
{
	u8 i;
	
	u16 gBkCol;
	u16 gTxCol;
	u16 gBarCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		gBkCol = BLACK;
		gTxCol = WHITE;
		gBarCol = GRAY;
	}else{
		gBkCol = WHITE;
		gTxCol = BLACK;
		gBarCol = DGRAY;
	}
	
	//状态栏
	LCD_FillRect(57,1,422,31,gBarCol);//横
	//分割线
	LCD_FillRect(57,192,422,2,gTxCol);//横
	for(i=0;i<42;i++){
		LCD_SetColor(gBkCol,gTxCol);
		LCD_ShowChinese(Font8,58+i*10,192,1,"杠");
	}
	
	if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){//浏览界面
		ui_DispGJName(ASC16x8,70,8,WHITE,(char*)FlieDataHead.GouJianName);
		
		LCD_SetColor(gBarCol,WHITE);
		//钢筋类型
		LCD_ShowChinese(Font16,164,8,0,SampTable[4].pList[FlieDataHead.ucRebarType].listStr);
		//主筋直径
		LCD_ShowChinese(Font20,198,6,0,"φ");
		LCD_ShowNums(ASC20x10,216,6,0,FlieDataHead.ucZhuJinDiam);
		//工况
		LCD_ShowChinese(Font16,248,8,0,(u8*)Condition[FlieDataHead.WorkCondition]);
		//存储方式
//		if(g_SysPara.g_SamplePara.iSaveType != 0){//手存
//			LCD_ShowString(ASC20x10,300,6,0,"M");
//		}
		//计数图标
		LCD_ShowChinese(Font24,410,5,0,"计");
		DispSaveDataCnt(FlieDataHead.iSaveDotCntCh1);
		
		g_DetectPara.PageQnt = FlieDataHead.iCarSite/g_GlobalPara.sDispScreenWide+1;
	}else{
		ui_DispGJName(ASC16x8,70,8,WHITE,(char*)SampTable[0].pList->listStr);
		
		LCD_SetColor(gBarCol,WHITE);
		//钢筋类型
		LCD_ShowChinese(Font16,164,8,0,SampTable[4].pList[g_SysPara.g_SamplePara.iRebarType].listStr);
		//主筋直径
		LCD_ShowChinese(Font20,198,6,0,"φ");
		LCD_ShowNums(ASC20x10,216,6,0,*SampTable[1].ParaAddr);
		//工况
		LCD_ShowChinese(Font16,248,8,0,(u8*)Condition[g_DetectPara.WorkCondition]);
		//存储方式
//		if(g_SysPara.g_SamplePara.iSaveType != 0){//手存
//			LCD_ShowString(ASC20x10,300,6,0,"M");
//		}
		//计数图标
		LCD_ShowChinese(Font24,410,5,0,"计");
		DispSaveDataCnt(g_GlobalPara.iSaveDataCnt);
		
		g_DetectPara.PageQnt = g_iCarDist/g_GlobalPara.sDispScreenWide+1;
	}
	
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
		ui_DispMenuBtn(AnalyzeMenu,2);
		
		//按钮初始化
		g_u8ButtonNums = 13;
		struct BUTTON SampBtn[13] = {
			//sX,  sY,  eX,  eY,
			{  0,   0,  59,  49, 0,0},//0, 返回
			{  0,  50,  59, 103, 0,0},//1, 显示
			{  0, 104,  59, 157, 0,0},//2, 增益
			{  0, 158,  59, 211, 0,0},//3, 热力
			{440,  32, 479,  85, 0,0},//4, 通道1
			{440,  86, 479, 138, 0,0},//5, 通道2
			{440, 139, 479, 191, 0,0},//6, 通道3
			{ 70,  32, 439,  85, 0,BTN_ABILITY_DRAG},//7, 通道1 拖动
			{ 70,  86, 439, 138, 0,BTN_ABILITY_DRAG},//8, 通道2 拖动
			{ 70, 139, 439, 191, 0,BTN_ABILITY_DRAG},//9, 通道3 拖动
			{160, 240, 219, 280, 0,0},//10, +
			{320, 240, 379, 280, 0,0},//11, -
			{  0, 212,  59, 265, 0,0} //12, 统计
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}else{
		//页面左侧菜单表
		MENU_TABLE_STRUCT AnalyzeMenu[4] = {
			{0,4,"徊",NULL},
			{1,4,"显","显示"},
			{2,4,"益","增益"},
			{3,4,"力","热力"}
		};
		//菜单显示
		ui_DispMenuBtn(AnalyzeMenu,2);
		
		//按钮初始化
		g_u8ButtonNums = 12;
		struct BUTTON SampBtn[12] = {
			//sX,  sY,  eX,  eY,
			{  0,   0,  59,  49, 0,0},//0, 返回
			{  0,  50,  59, 103, 0,0},//1, 显示
			{  0, 104,  59, 157, 0,0},//2, 增益
			{  0, 158,  59, 211, 0,0},//3, 热力
			{440,  32, 479,  85, 0,0},//4, 通道1
			{440,  86, 479, 138, 0,0},//5, 通道2
			{440, 139, 479, 191, 0,0},//6, 通道3
			{ 70,  32, 439,  85, 0,BTN_ABILITY_DRAG},//7, 通道1 拖动
			{ 70,  86, 439, 138, 0,BTN_ABILITY_DRAG},//8, 通道2 拖动
			{ 70, 139, 439, 191, 0,BTN_ABILITY_DRAG},//9, 通道3 拖动
			{160, 240, 219, 280, 0,0},//10, +
			{320, 240, 379, 280, 0,0} //11, -
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}
	
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		//增益
		LCD_FillRect(160,240,60,40,GRAY);
		LCD_FillRect(320,240,60,40,GRAY);
		LCD_DrawRect(159,239,62,42,LGRAY);
		LCD_DrawRect(319,239,62,42,LGRAY);
		LCD_SetColor(GRAY,LLGRAY);
		LCD_ShowChinese(Font32,174,244,0,"＋");
		LCD_ShowChinese(Font32,334,244,0,"﹣");
	}else{
		//增益
		LCD_FillRect(160,240,60,40,GRAY);
		LCD_FillRect(320,240,60,40,GRAY);
		LCD_DrawRect(159,239,62,42,DGRAY);
		LCD_DrawRect(319,239,62,42,DGRAY);
		LCD_SetColor(GRAY,DDGRAY);
		LCD_ShowChinese(Font32,174,244,0,"＋");
		LCD_ShowChinese(Font32,334,244,0,"﹣");
	}
	LCD_SetColor(gBkCol,gTxCol);
	LCD_ShowNums(ASC24x12,264,248,0,g_DetectPara.ucGain);
	//参数初始化
//	g_DetectPara.PageView = 1;
//	g_DetectPara.PageLast = 1;
//	g_DetectPara.CarSite = 0;
	
	//通道选择
//	g_DetectPara.Channel = 0;
//	g_DetectPara.ChannelLast = 1;
	
	//显示热力图
	g_SysPara.AnalyzeDispMode = RELI_POU;
	if((g_SysPara.AnalyzeDispMode == RELI_POU)||(g_SysPara.AnalyzeDispMode == RELI_BO)){
		
		BrushHeatMap(g_DetectPara.PageView,g_DetectPara.ucHeat);
	}
	//显示条形图
	if((g_SysPara.AnalyzeDispMode == TIAO_POU)||(g_SysPara.AnalyzeDispMode == TIAO_BO)){
		
		BrushChannelSel(g_DetectPara.Channel,1);
		BrushTiaoXingPage(g_DetectPara.PageView,0);
		BrushTiaoXingPage(g_DetectPara.PageView,1);
		BrushTiaoXingPage(g_DetectPara.PageView,2);
		DispChannelLine(g_DetectPara.Channel);
	}else{
		BrushChannelSel(g_DetectPara.Channel,1);
	}
}

//---------------------------------------------
//UI运行函数
void ui_Gain_Run(void)
{
	u8 i;
	
	u16 gbkCol;
	u16 gtxCol;
	u16 menuBtCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		gbkCol = BLACK;
		gtxCol = WHITE;
		menuBtCol = DGRAY;
	}else{
		gbkCol = WHITE;
		gtxCol = BLACK;
		menuBtCol = LGRAY;
	}
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		//点击效果
		ui_AllButtonReaction(2,menuBtCol,GOLD);
		ui_OneButtonReaction(10,GRAY,GOLD);//返回
		ui_OneButtonReaction(11,GRAY,GOLD);//返回
		
//		ui_DispTime(0);//时间
//		ui_DispBattery(0);//电量
		//
		if(g_u16KeyValBuf == KEY_OK){

		}
		if(g_u16KeyValBuf == KEY_BACK){
			SendUiNo(UI_SAMPLE10);
			LCD_FillRect(56,32,424,288,gbkCol);
			return;
		}

		//切换页面
		if(g_u16KeyValBuf == KEY_FN){
			Touch_Beep_On();
			SendUiNo(UI_SAMPLE10);
			LCD_FillRect(56,32,424,288,gbkCol);
			return;
		}
		
		//返回
		if(g_u8TouchResult == 0){
			SendUiNo(UI_ANALYZE);
			LCD_FillRect(56,32,424,288,gbkCol);
			return;
		}
		//显示
		if(g_u8TouchResult == 1){
			g_GlobalPara.ucDispModepage = 1;//分析显示
			SendUiNo(UI_SAMPDISP);
			LCD_FillRect(56,32,424,288,gbkCol);
			return;
		}
		//增益
		if(g_u8TouchResult == 2){
//			SendUiNo(UI_GAIN);
//			LCD_FillRect(60,33,420,287,WHITE);
//			return;
		}
		//热力
		if(g_u8TouchResult == 3){
			SendUiNo(UI_HEAT);
			LCD_FillRect(56,200,424,120,gbkCol);
			return;
		}
		//统计
		if(g_u8TouchResult == 12){
			if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){
				LCD_FillRect(56,32,424,288,gbkCol);
				SampleDataView_Fun();
				ui_Gain_InitDisp();
			}
		}
		
		//通道选择
		if((g_u8TouchResult >= 4)&&(g_u8TouchResult <= 6)){
			g_DetectPara.ChannelLast = g_DetectPara.Channel;
			g_DetectPara.Channel = g_u8TouchResult - 4;
			if(g_DetectPara.Channel != g_DetectPara.ChannelLast){
				if((g_SysPara.AnalyzeDispMode == TIAO_POU)||(g_SysPara.AnalyzeDispMode == TIAO_BO)){
					BrushChannelSel(g_DetectPara.Channel,1);
					
					DeleteChannelLine(g_DetectPara.ChannelLast);
					BrushTiaoXingPage(g_DetectPara.PageView,g_DetectPara.ChannelLast);
					BrushTiaoXingPage(g_DetectPara.PageView,g_DetectPara.Channel);
					DispChannelLine(g_DetectPara.Channel);
				}else{
					BrushChannelSel(g_DetectPara.Channel,1);
				}
			}
		}
		//拖动切换通道
		for(i=0;i<3;i++){
			if(g_ButtonBuf[7+i].btSta == BTN_STATUS_DRAG){
				g_DetectPara.ChannelLast = g_DetectPara.Channel;
				g_DetectPara.Channel = Get_DragChannel(&g_ButtonBuf[7],3,DRAG_ZONG);
				if(g_DetectPara.Channel != g_DetectPara.ChannelLast){
					if((g_SysPara.AnalyzeDispMode == TIAO_POU)||(g_SysPara.AnalyzeDispMode == TIAO_BO)){
						BrushChannelSel(g_DetectPara.Channel,1);
						
						DeleteChannelLine(g_DetectPara.ChannelLast);
						BrushTiaoXingPage(g_DetectPara.PageView,g_DetectPara.ChannelLast);
						BrushTiaoXingPage(g_DetectPara.PageView,g_DetectPara.Channel);
						DispChannelLine(g_DetectPara.Channel);
					}else{
						BrushChannelSel(g_DetectPara.Channel,1);
					}
				}
			}
		}
		//增益+
		if(g_u8TouchResult == 10){
			g_DetectPara.ucGain += 1;
			if(g_DetectPara.ucGain > 8)g_DetectPara.ucGain = 1;
			LCD_SetColor(gbkCol,gtxCol);
			LCD_ShowNums(ASC24x12,264,248,0,g_DetectPara.ucGain);
		}
		//增益-
		if(g_u8TouchResult == 11){
			g_DetectPara.ucGain -= 1;
			if(g_DetectPara.ucGain < 1)g_DetectPara.ucGain = 8;
			if(g_DetectPara.ucGain > 8)g_DetectPara.ucGain = 8;
			LCD_SetColor(gbkCol,gtxCol);
			LCD_ShowNums(ASC24x12,264,248,0,g_DetectPara.ucGain);
		}
	}
}
