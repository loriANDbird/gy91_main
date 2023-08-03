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

static u8 HeatLast;

void ui_Heat_InitDisp(void)
{
	u8 i;
	u16 cRed,r,g,b;
	
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		//状态栏
		LCD_FillRect(57,1,422,31,GRAY);//横
		//分割线
		LCD_FillRect(57,192,422,2,WHITE);//横
		for(i=0;i<42;i++){
			LCD_SetColor(BLACK,WHITE);
			LCD_ShowChinese(Font8,58+i*10,192,1,"杠");
		}
	}else{
		//状态栏
		LCD_FillRect(57,1,422,31,DGRAY);//横
		//分割线
		LCD_FillRect(57,192,422,2,BLACK);//横
		for(i=0;i<42;i++){
			LCD_SetColor(WHITE,BLACK);
			LCD_ShowChinese(Font8,58+i*10,192,1,"杠");
		}
	}
	
	if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){//浏览界面
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
	}else{
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
		ui_DispMenuBtn(AnalyzeMenu,3);
	
		//按钮初始化
		g_u8ButtonNums = 19;
		struct BUTTON SampBtn[19] = {
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
			
			{110, 210, 149, 310, 0,BTN_ABILITY_DRAG},//10, 颜色1
			{150, 210, 189, 310, 0,BTN_ABILITY_DRAG},//11, 颜色2
			{190, 210, 229, 310, 0,BTN_ABILITY_DRAG},//12, 颜色3
			{230, 210, 269, 310, 0,BTN_ABILITY_DRAG},//13, 颜色4
			{270, 210, 309, 310, 0,BTN_ABILITY_DRAG},//14, 颜色5
			{310, 210, 349, 310, 0,BTN_ABILITY_DRAG},//15, 颜色6
			{350, 210, 389, 310, 0,BTN_ABILITY_DRAG},//16, 颜色7
			{390, 210, 429, 310, 0,BTN_ABILITY_DRAG},//17, 颜色8
			{  0, 212,  59, 265, 0,0} //18, 统计
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
		ui_DispMenuBtn(AnalyzeMenu,3);
	
		//按钮初始化
		g_u8ButtonNums = 18;
		struct BUTTON SampBtn[18] = {
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
			
			{110, 210, 149, 310, 0,BTN_ABILITY_DRAG},//10, 颜色1
			{150, 210, 189, 310, 0,BTN_ABILITY_DRAG},//11, 颜色2
			{190, 210, 229, 310, 0,BTN_ABILITY_DRAG},//12, 颜色3
			{230, 210, 269, 310, 0,BTN_ABILITY_DRAG},//13, 颜色4
			{270, 210, 309, 310, 0,BTN_ABILITY_DRAG},//14, 颜色5
			{310, 210, 349, 310, 0,BTN_ABILITY_DRAG},//15, 颜色6
			{350, 210, 389, 310, 0,BTN_ABILITY_DRAG},//16, 颜色7
			{390, 210, 429, 310, 0,BTN_ABILITY_DRAG},//17, 颜色8
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}
	
	//颜色选择
	for(i=0;i<8;i++){
		r = 0x1f;
		g = 0x40/8*(7-i);
		b = 0x20/8*(7-i);
		cRed = (r<<11)+(g<<5)+b;
		LCD_FillRect(110+i*40,240,40,40,cRed);
	}
	//选择框
	LCD_FillRect(110+g_DetectPara.ucHeat*40-4,240-4,40+8,4,GOLD);
	LCD_FillRect(110+g_DetectPara.ucHeat*40-4,240+40,40+8,4,GOLD);
	LCD_FillRect(110+g_DetectPara.ucHeat*40-4,240,4,40,GOLD);
	LCD_FillRect(110+g_DetectPara.ucHeat*40+40,240,4,40,GOLD);
	HeatLast = g_DetectPara.ucHeat;
	
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
void ui_Heat_Run(void)
{
	u8 i;
	u16 cRedf,cRedn;
	u16 cRed[8] = {0xff1c,0xfe18,0xfd14,0xfc10,0xfb0c,0xfa08,0xf904,0xf800};
	
	u16 hbkCol;
	u16 menuBtCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		hbkCol = BLACK;
		menuBtCol = DGRAY;
	}else{
		hbkCol = WHITE;
		menuBtCol = LGRAY;
	}
	
	while(1){
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
			SendUiNo(UI_SAMPLE10);
			LCD_FillRect(56,32,424,288,hbkCol);
			return;
		}
		
		//切换页面
		if(g_u16KeyValBuf == KEY_FN){
			g_DetectPara.PageView += 1;
			if(g_DetectPara.PageView > 2)
				g_DetectPara.PageView = 1;
			BrushHeatMap(g_DetectPara.PageView,g_DetectPara.ucHeat);
		}
		
		//返回
		if(g_u8TouchResult == 0){
			SendUiNo(UI_ANALYZE);
			LCD_FillRect(56,32,424,288,hbkCol);
			return;
		}
		//显示
		if(g_u8TouchResult == 1){
			g_GlobalPara.ucDispModepage = 1;//分析显示
			SendUiNo(UI_SAMPDISP);
			LCD_FillRect(56,32,424,288,hbkCol);
			return;
		}
		//增益
		if(g_u8TouchResult == 2){
			SendUiNo(UI_GAIN);
			LCD_FillRect(56,200,424,120,hbkCol);
			return;
		}
		//热力
		if(g_u8TouchResult == 3){
//			SendUiNo(UI_HEAT);
//			LCD_FillRect(60,33,480,287,WHITE);
//			return;
		}
		//统计
		if(g_u8TouchResult == 18){
			if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){
				LCD_FillRect(56,32,424,288,hbkCol);
				SampleDataView_Fun();
				ui_Heat_InitDisp();
			}
		}
		
		//通道选择
		if((g_u8TouchResult >= 4)&&(g_u8TouchResult <= 6)){
			g_DetectPara.ChannelLast = g_DetectPara.Channel;
			g_DetectPara.Channel = g_u8TouchResult - 4;
			if(g_DetectPara.Channel != g_DetectPara.ChannelLast){
				BrushChannelSel(g_DetectPara.Channel,1);
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
		
		//热力选择
//		if((g_u8TouchResult >= 7)&&(g_u8TouchResult <= 14)){
//			HeatLast = g_DetectPara.ucHeat;
//			g_DetectPara.ucHeat = g_u8TouchResult - 7;
//			
//			if(g_DetectPara.ucHeat != HeatLast){
//				LCD_SetColor(WHITE,WHITE);
//				LCD_ShowChinese(Font40,110+HeatLast*40,260,0,"△");
//				
//				LCD_SetColor(WHITE,GRAY);
//				LCD_ShowChinese(Font40,110+g_DetectPara.ucHeat*40,260,0,"△");
//			}
//		}
		//拖动切换热力通道
		for(i=0;i<8;i++){
			if(g_ButtonBuf[10+i].btSta == BTN_STATUS_DRAG){
				HeatLast = g_DetectPara.ucHeat;
				g_DetectPara.ucHeat = Get_DragChannel(&g_ButtonBuf[10],8,DRAG_HENG);
				
				if(g_DetectPara.ucHeat != HeatLast){
					if(HeatLast == 0){
						cRedf = hbkCol;
						cRedn = cRed[HeatLast+1];
					}else if(HeatLast == 7){
						cRedf = cRed[HeatLast-1];
						cRedn = hbkCol;
					}else{
						cRedf = cRed[HeatLast-1];
						cRedn = cRed[HeatLast+1];
					}
					LCD_FillRect(110+HeatLast*40-4,240-4,40+8,4,hbkCol);
					LCD_FillRect(110+HeatLast*40-4,240+40,40+8,4,hbkCol);
					LCD_FillRect(110+HeatLast*40-4,240,4,40,cRedf);
					LCD_FillRect(110+HeatLast*40+40,240,4,40,cRedn);

					LCD_FillRect(110+g_DetectPara.ucHeat*40-4,240-4,40+8,4,GOLD);
					LCD_FillRect(110+g_DetectPara.ucHeat*40-4,240+40,40+8,4,GOLD);
					LCD_FillRect(110+g_DetectPara.ucHeat*40-4,240,4,40,GOLD);
					LCD_FillRect(110+g_DetectPara.ucHeat*40+40,240,4,40,GOLD);
					
					BrushHeatMap(g_DetectPara.PageView,g_DetectPara.ucHeat);
				}
			}
		}
	}
}
