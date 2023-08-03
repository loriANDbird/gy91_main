/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：厚度测量
    1. ui_Sample10 界面初始化，
    2. ui_Sample10_Run 函数实现

更新记录：
    1. 2018.03.01 初次创建
    
*******************************************************************************/
#include "ui_Analyze.h"
#include "Config_ui.h"

//static u8 PageNum = 1;
//static short CarSite = 0;
//extern u8 Channel;//通道号

extern SYS_PARA_TABLE_STRUCT* SampTable;
extern char *Condition[5];

void ui_Analyze_InitDisp(void)
{
	u8 i;
	
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		//状态栏
		LCD_FillRect(57,1,422,31,GRAY);
		//分割线
		LCD_FillRect(57,192,422,2,WHITE);//横
		for(i=0;i<42;i++){
			LCD_SetColor(BLACK,WHITE);
			LCD_ShowChinese(Font8,58+i*10,192,1,"杠");
		}
		LCD_FillRect(57,298,422,1,BLACK);//横
	}else{
		//状态栏
		LCD_FillRect(57,1,422,31,DGRAY);
		//分割线
		LCD_FillRect(57,192,422,2,BLACK);//横
		for(i=0;i<42;i++){
			LCD_SetColor(WHITE,BLACK);
			LCD_ShowChinese(Font8,58+i*10,192,1,"杠");
		}
		LCD_FillRect(57,298,422,1,WHITE);//横
	}
	
	
	if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){//浏览界面
		
		g_ucTurnGouJianSta = 0;//切换构件状态
		
		ui_DispGJName(ASC16x8,70,8,WHITE,(char*)FlieDataHead.GouJianName);
		//深色背景
		if(g_SysPara.iTheme == THEME_DARK){
			LCD_SetColor(GRAY,WHITE);
		}else{
			LCD_SetColor(DGRAY,WHITE);
		}
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
	}
    else{
		ui_DispGJName(ASC16x8,70,8,WHITE,(char*)SampTable[0].pList->listStr);
		
		//深色背景
		if(g_SysPara.iTheme == THEME_DARK){
			LCD_SetColor(GRAY,WHITE);
		}else{
			LCD_SetColor(DGRAY,WHITE);
		}
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
	
	//按钮初始化
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
		ui_DispMenuBtn(AnalyzeMenu,0);
	
		//按钮初始化
		g_u8ButtonNums = 11;
		
	}else{//
		//页面左侧菜单表
		MENU_TABLE_STRUCT AnalyzeMenu[4] = {
			{0,4,"徊",NULL},
			{1,4,"显","显示"},
			{2,4,"益","增益"},
			{3,4,"力","热力"}
		};
		//菜单显示
		ui_DispMenuBtn(AnalyzeMenu,0);
	
		//按钮初始化
		g_u8ButtonNums = 10;
	}
	struct BUTTON SampBtn[11] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0},//0, 返回
		{  0,  50,  59, 103, 0,0},//1, 显示
		{  0, 104,  59, 157, 0,0},//2, 增益
		{  0, 158,  59, 211, 0,0},//3, 热力
		{440,  32, 479,  85, 0,0},//6, 通道1
		{440,  86, 479, 138, 0,0},//7, 通道2
		{440, 139, 479, 191, 0,0},//8, 通道3
		{ 70,  32, 439,  85, 0,BTN_ABILITY_DRAG},//7, 通道1 拖动
		{ 70,  86, 439, 138, 0,BTN_ABILITY_DRAG},//8, 通道2 拖动
		{ 70, 139, 439, 191, 0,BTN_ABILITY_DRAG},//9, 通道3 拖动
		{  0, 212,  59, 265, 0,0} //10, 统计
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	//参数初始化
	g_DetectPara.PageView = 1;
	g_DetectPara.PageLast = 1;
	g_DetectPara.CarSite = 0;
	
	g_DetectPara.ucGain = 1;
	g_DetectPara.ucHeat = 4;
	
	
	//通道选择
	g_DetectPara.Channel = 0;
	g_DetectPara.ChannelLast = 1;
	
	//显示热力图
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
	//显示测点图形
	BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,g_SysPara.AnalyzeDispMode);

//	ui_DispTime(1);
//	ui_DispBattery(1);//显示电量
	
	ClearTouchSta();//清除触摸
}

void ui_Analyze_Run(void)
{
	u8 i;
	
	u16 abkCol;
	u16 menuBtCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		abkCol = BLACK;
		menuBtCol = DGRAY;
	}else{
		abkCol = WHITE;
		menuBtCol = LGRAY;
	}
	
	while(1)
	{
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		//点击效果
		ui_AllButtonReaction(2,menuBtCol,GOLD);
//		ui_OneButtonReactionXY(0,0,0,44,33,GOLD,ORANGE);//返回
		
//		ui_DispTime(0);//时间
//		ui_DispBattery(0);//电量
		//
		if(g_u16KeyValBuf == KEY_OK){

		}
		if(g_u16KeyValBuf == KEY_BACK){
			if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){//浏览界面
				SendUiNo(UI_BROWSE10);
			}else{
				SendUiNo(UI_SAMPLE10);
			}
			LCD_FillRect(56,32,424,288,abkCol);
			g_GlobalPara.ucFromPageNum = UI_ANALYZE;
			return;
		}
		
		//返回
		if(g_u8TouchResult == 0){
			if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){//浏览界面
				SendUiNo(UI_BROWSE10);
				g_ucTurnGouJianSta = 0;//切换构件状态
			}else{
				SendUiNo(UI_SAMPLE10);
			}
			LCD_FillRect(56,32,424,288,abkCol);
			g_GlobalPara.ucFromPageNum = UI_ANALYZE;
			return;
		}
		//显示
		if(g_u8TouchResult == 1){
			g_GlobalPara.ucDispModepage = 1;//分析显示
			SendUiNo(UI_SAMPDISP);
			LCD_FillRect(56,32,424,288,abkCol);
			return;
		}
		//增益
		if(g_u8TouchResult == 2){
			if((g_SysPara.AnalyzeDispMode == TIAO_POU)||(g_SysPara.AnalyzeDispMode == TIAO_BO)){
				DeleteChannelLine(g_DetectPara.Channel);
			}
			SendUiNo(UI_GAIN);
			LCD_FillRect(56,200,424,120,abkCol);
			return;
		}
		//热力
		if(g_u8TouchResult == 3){
			if((g_SysPara.AnalyzeDispMode == TIAO_POU)||(g_SysPara.AnalyzeDispMode == TIAO_BO)){
				DeleteChannelLine(g_DetectPara.Channel);
			}
			SendUiNo(UI_HEAT);
			LCD_FillRect(56,200,424,120,abkCol);
			return;
		}
		//统计
		if(g_u8TouchResult == 10){
			if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){
				LCD_FillRect(56,32,424,288,abkCol);
				SampleDataView_Fun();
				ui_Analyze_InitDisp();
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
				BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,g_SysPara.SampleDispMode);
//				BrushCar(g_iCarDist);
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
					BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,g_SysPara.AnalyzeDispMode);
				}
			}
		}
		
		if(g_u8TouchResult == SLIDE_END_LEFT){//左划
			g_DetectPara.PageLast = g_DetectPara.PageView;
			g_DetectPara.PageView += 1;
			if(g_DetectPara.PageView > g_DetectPara.PageQnt){
				g_DetectPara.PageView = g_DetectPara.PageQnt;
			}
			if(g_DetectPara.PageView != g_DetectPara.PageLast){
				//显示热力图
				if((g_SysPara.AnalyzeDispMode == RELI_POU)||(g_SysPara.AnalyzeDispMode == RELI_BO)){
					
					BrushHeatMap(g_DetectPara.PageView,g_DetectPara.ucHeat);
				}
				//显示条形图
				if((g_SysPara.AnalyzeDispMode == TIAO_POU)||(g_SysPara.AnalyzeDispMode == TIAO_BO)){
					ClearTiaoxingPage(0);
					ClearTiaoxingPage(1);
					ClearTiaoxingPage(2);
					BrushChannelSel(g_DetectPara.Channel,1);
					BrushTiaoXingPage(g_DetectPara.PageView,0);
					BrushTiaoXingPage(g_DetectPara.PageView,1);
					BrushTiaoXingPage(g_DetectPara.PageView,2);
					DispChannelLine(g_DetectPara.Channel);
				}else{
					BrushChannelSel(g_DetectPara.Channel,1);
				}
				//显示测点图形
				BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,g_SysPara.AnalyzeDispMode);
			}
		}
		if(g_u8TouchResult == SLIDE_END_RIGHT){//右划
			g_DetectPara.PageLast = g_DetectPara.PageView;
			g_DetectPara.PageView -= 1;
			if(g_DetectPara.PageView < 1){
				g_DetectPara.PageView = 1;
			}
			if(g_DetectPara.PageView != g_DetectPara.PageLast){
				//显示热力图
				if((g_SysPara.AnalyzeDispMode == RELI_POU)||(g_SysPara.AnalyzeDispMode == RELI_BO)){
					
					BrushHeatMap(g_DetectPara.PageView,g_DetectPara.ucHeat);
				}
				//显示条形图
				if((g_SysPara.AnalyzeDispMode == TIAO_POU)||(g_SysPara.AnalyzeDispMode == TIAO_BO)){
					ClearTiaoxingPage(0);
					ClearTiaoxingPage(1);
					ClearTiaoxingPage(2);
					BrushChannelSel(g_DetectPara.Channel,1);
					BrushTiaoXingPage(g_DetectPara.PageView,0);
					BrushTiaoXingPage(g_DetectPara.PageView,1);
					BrushTiaoXingPage(g_DetectPara.PageView,2);
					DispChannelLine(g_DetectPara.Channel);
				}else{
					BrushChannelSel(g_DetectPara.Channel,1);
				}
				//显示测点图形
				BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,g_SysPara.AnalyzeDispMode);
			}
		}
		if(g_u8TouchResult == SLIDE_END_UP){//上划
			if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){//浏览界面
				g_ucTurnGouJianSta = NEXT;//切换构件状态
				SendUiNo(UI_BROWSE10);
				LCD_FillRect(56,32,424,288,abkCol);
				g_GlobalPara.ucFromPageNum = UI_ANALYZE;
				return;
			}
		}
		if(g_u8TouchResult == SLIDE_END_DOWN){//下划
			if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){//浏览界面
				g_ucTurnGouJianSta = LAST;//切换构件状态
				SendUiNo(UI_BROWSE10);
				LCD_FillRect(56,32,424,288,abkCol);
				g_GlobalPara.ucFromPageNum = UI_ANALYZE;
				return;
			}
		}
		
		Sleep_ms(5);
	}
}

//*******************************************************************
//快速检测统计信息
void SampleDataView_Fun(void)
{
	u8 i;
	char tempBuf[16];
	char timeBuf[20];
	u16 bkCol;
	u16 txCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
		txCol = WHITE;
	}else{
		bkCol = WHITE;
		txCol = BLACK;
	}
	
	ui_DispTitleBar("统计信息",0);
	
	for(i=0;i<7;i++){
		LCD_FillRect(70,70+i*30,394,1,GRAY);//横
	}
	
	LCD_SetColor(bkCol,txCol);
	LCD_ShowText(Font16,80,47,0,"构件编号:");
	ui_DispGJName(ASC16x8,180,47,txCol,FlieDataHead.GouJianName);
	LCD_ShowText(Font16,390,47,0,SampTable[8].pList[FlieDataHead.ucGouJianType].listStr);
	
	sprintf(tempBuf,"(%d/%d)",g_GlobalPara.iCurFileNum,g_GlobalPara.iSaveFileCnt);
	LCD_ShowText(Font16,278,47,0,(u8*)tempBuf);
	
	sprintf(timeBuf,"%02d-%02d-%02d %02d:%02d:%02d",FlieDataHead.FileTimeStruct.year-2000,FlieDataHead.FileTimeStruct.month,
	FlieDataHead.FileTimeStruct.day,FlieDataHead.FileTimeStruct.hour,FlieDataHead.FileTimeStruct.min,FlieDataHead.FileTimeStruct.sec);
	LCD_ShowString(ASC16x8,80,77,0,timeBuf);
	
	LCD_SetColor(bkCol,txCol);
	LCD_ShowText(Font16,80,107,0,"主筋直径");
	sprintf(tempBuf,"%d mm  ",FlieDataHead.ucZhuJinDiam);
	LCD_ShowString(ASC20x10,180,105,0,tempBuf);
	LCD_ShowText(Font16,290,107,0,"主筋间距");
	sprintf(tempBuf,"%d mm  ",FlieDataHead.ucZhuJinJianju);
	LCD_ShowString(ASC20x10,390,105,0,tempBuf);
	LCD_ShowText(Font16,80,137,0,"箍筋直径");
	sprintf(tempBuf,"%d mm  ",FlieDataHead.ucGuJinDiam);
	LCD_ShowString(ASC20x10,180,135,0,tempBuf);
	LCD_ShowText(Font16,290,137,0,"箍筋间距");
	sprintf(tempBuf,"%d mm  ",FlieDataHead.ucGuJinJianju);
	LCD_ShowString(ASC20x10,390,135,0,tempBuf);
	LCD_ShowText(Font16,80,167,0,"设计厚度");
	sprintf(tempBuf,"%d mm  ",FlieDataHead.ucHouDuDesign);
	LCD_ShowString(ASC20x10,180,167,0,tempBuf);
	LCD_ShowText(Font16,290,167,0,"平均厚度");
	sprintf(tempBuf,"%d mm  ",FlieDataHead.ucHouDuAvg);
	LCD_ShowString(ASC20x10,390,167,0,tempBuf);
	LCD_ShowText(Font16,80,197,0,"最小厚度");
	sprintf(tempBuf,"%d mm  ",FlieDataHead.ucHouDuMin);
	LCD_ShowString(ASC20x10,180,197,0,tempBuf);
	LCD_ShowText(Font16,290,197,0,"最大厚度");
	sprintf(tempBuf,"%d mm  ",FlieDataHead.ucHouDuMax);
	LCD_ShowString(ASC20x10,390,197,0,tempBuf);
	LCD_ShowText(Font16,80,227,0,"已存测点");
	sprintf(tempBuf,"%d  ",FlieDataHead.iSaveDotCntCh1);
	LCD_ShowString(ASC20x10,180,225,0,tempBuf);
	
	//页面左侧菜单表
	MENU_TABLE_STRUCT PreviewMenu[1] = {
		{0,1,"徊",NULL}
	};
	//菜单显示
	ui_DispMenuBtn(PreviewMenu,0);
	//按钮初始化
	g_u8ButtonNums = 1;
	struct BUTTON avBtn[1] = {
		//sX,  sY,  eX,  eY,
		{0, 0, 59, 49, 0,0} //0, 返回
	};   
	memcpy(g_ButtonBuf,avBtn,sizeof(avBtn));
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
//		ui_OneButtonReactionXY(0,0,0,47,33,GOLD,ORANGE);//返回
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//切换页面
		if(g_u16KeyValBuf == KEY_FN){
//			return;
		}
		
		//返回
		if(g_u8TouchResult == 0){
			
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
	}
}
