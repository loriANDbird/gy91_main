/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：数据传输
    1. ui_Upload 界面初始化，
    2. ui_Upload_Run 函数实现
    
更新记录：
    1. 2018.03.01 初次创建
    
*******************************************************************************/
#include "ui_SampJGJ.h"
#include "Config_ui.h"

#define JGJPAGECAP	6	//每页可显示钢筋根数

extern char *ChName[3];
static MENU_TABLE_STRUCT OneBtnSet[1];//修改按钮显示
//extern short MZJSite;
//extern short CarSite;
//extern u8 Channel;//通道号 0,1,2

SYS_PARA_TABLE_STRUCT* JGJParaTable = &ParaTable[PARATABLEINDEX_JGJ];

static u8 JGJPageQnt;//JGJ页数1-2
static u8 JGJPage;//页码1-2
static u8 JGJPageC;//页码1-2
static u8 JGJQnt;//JGJ钢筋根数
static u8 JGJNum;//JGJ钢筋编号1-12
static u8 JGJDot;//当前测点1-6
static u8 JGJDir;//扫描方向，0横向，1纵向
static u8 JGJSta;//测量状态，0未开始，1已开始

u8 JGJData[12][9];//数据存储

u8 JGJHegeCnt = 0;//JGJ合格计数
u8 JGJHegeFlagBefore = 0;//检测前是否合格标志 1：合格
u8 JGJHegeFlagAfter = 0;//检测后是否合格标志 1：合格

//内部函数
void SampleJGJ_Init(void);
void JGJParaSet_Init(SYS_PARA_TABLE_STRUCT* JGJParaTable,u8 pageNum);
void JGJParaSet_Fun(SYS_PARA_TABLE_STRUCT* JGJParaTable);
void SaveJGJFile(void);

void WriteJGJCurDot(u8 jgjNum,u8 dotNum,u8 houdu);
void DispJGJData(u8 jgjPageNum,u8 jgjNum,u8 dotNum);
void ClearJGJData(void);
void ClearJGJBuf(void);
void BrushJGJDot(u8 jgjPageNum,u8 jgjNum,u8 dotNum);
void BrushJGJRebar(u8 jgjPageNum,u8 jgjNum,u8 dotNum);
void BrushJGJSketch(u8 jgjPageNum,u8 jgjNum);
u8 GetJGJSaveCnt(void);
u8 IsRebarTested(u8 jgjNum);
u8 IsRebarHege(u8 jgjNum);
void GetJGJHegeFlagBefore(u8 jgjNum);
void GetJGJHegeFlagAfter(u8 jgjNum);
void SetJGJHegeCnt(u8 jgjNum);
u8 GetJGJAverage(u8 jgjNum);
u8 BankersRounding(u8 val1,u8 val2);

void ui_SampJGJ_InitDisp(void)
{
	//JGJ相关参数初始化
	if(g_GlobalPara.ucFromPageNum == UI_MAIN){
		if((g_SysPara.g_SamplePara.iRebarNumJGJ%JGJPAGECAP) == 0){
			JGJPageQnt = g_SysPara.g_SamplePara.iRebarNumJGJ/JGJPAGECAP;
		}else{
			JGJPageQnt = g_SysPara.g_SamplePara.iRebarNumJGJ/JGJPAGECAP+1;
		}
		JGJQnt = g_SysPara.g_SamplePara.iRebarNumJGJ;
		JGJPage = 1;
		JGJNum = 1;
		JGJDot = 1;
		JGJDir = g_SysPara.JGJScanDir;
		JGJSta = 0;
		
		JGJHegeCnt = 0;
		JGJHegeFlagBefore = 0;
		JGJHegeFlagAfter = 0;
		
		memset(JGJData,0x0,sizeof(JGJData));
	}
	if((g_SysPara.g_SamplePara.iRebarNumJGJ%JGJPAGECAP) == 0){
		JGJPageQnt = g_SysPara.g_SamplePara.iRebarNumJGJ/JGJPAGECAP;
	}else{
		JGJPageQnt = g_SysPara.g_SamplePara.iRebarNumJGJ/JGJPAGECAP+1;
	}
	JGJQnt = g_SysPara.g_SamplePara.iRebarNumJGJ;
	//存储容量
	g_GlobalPara.iStorageFreePrcnt = Get_FatFreePrcnt();
	
	SampleJGJ_Init();
}

void ui_SampJGJ_Run(void)
{
    u8 Res;
    u8 ucAimState = 0;
    char cBuf[8];
//	u8 testVal = 1;
//	char tempBuf[16];
	
	u16 jbkCol;
	u16 jtxCol;
	u16 menuBtCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		jbkCol = BLACK;
		jtxCol = WHITE;
		menuBtCol = DGRAY;
	}else{
		jbkCol = WHITE;
		jtxCol = BLACK;
		menuBtCol = LGRAY;
	}
	
	//电量低提示
	if(g_GlobalPara.ucFromPageNum == UI_MAIN){
		Get_BatPrcnt();
		if(g_GlobalPara.iBatPercent < 20){
			PromptDialog(DIALOGBATLOW);
			g_GlobalPara.ucFromPageNum = UI_SAMPJGJ;//假装从主页进入
			LCD_FillRect(56,32,424,288,jbkCol);
			SampleJGJ_Init();
		}
		if(g_GlobalPara.iBatPercent < 5){
			PromptDialog(DIALOGBATNONE);
			SendUiNo(UI_MAIN);
			g_GlobalPara.ucFromPageNum = UI_SAMPJGJ;//假装从主页进入
			LCD_FillRect(56,32,424,288,jbkCol);
			return;
		}
	}
	
    while(1)
	{
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		//点击效果
		ui_AllButtonReaction(5,menuBtCol,GOLD);
		
//		ui_DispTime(0);//时间
		ui_GetTempera();//采集温度
		ui_DispBattery(2);//采集但不显示电量
		
		//蓝牙
		if(g_SysPara.iBlueToothSta == 1){
			BT_Lookup();
//			if(g_bBlueTooDispFlag == 1){
//				g_bBlueTooDispFlag = 0;
//				ui_DispBluetooth();
//			}
		}
		
		//数据采集
		if(Get_Adc_Data()){
			//g_DetectPara.HouduRes = 厚度计算
		}

        //更新瞄准镜位置
        ucAimState = UpdateMZK(g_DetectPara.Channel);
        
        //有位移变化再进行运算
        if(g_iCarDist > g_iCarLastDist){

            //位移变化标志位
//////            cDistChangeFlag = 1;
            
            //瞄准镜定位钢筋算法
//////            AimFuncRun_MZK(g_DetectPara.Channel, ucAimState, g_SysPara.SampleDispMode);
            Res = AimFuncRun_JGJ(g_DetectPara.Channel, ucAimState, g_SysPara.SampleDispMode);
            if(Res != 0){
                g_DetectPara.HouduRes = Res;
                sprintf(cBuf,"%3d",g_DetectPara.HouduRes);
                LCD_SetColor(jbkCol,jtxCol);
                LCD_ShowString(ASC24x12,380,164,0,cBuf);//厚度值
            }
            g_iCarLastDist = g_iCarDist;
        }

		//按键存储构件
		if(g_u16KeyValBuf == KEY_OK){
//			testVal += 1;
//			DispHouduRes(testVal);
			g_GlobalPara.iSaveDataCnt = GetJGJSaveCnt();
			if(g_GlobalPara.iSaveDataCnt > 0){
				
				//存储空间不足提示
				if(g_GlobalPara.iStorageFreePrcnt < 1){
					PromptDialog(DIALOGSTORNONE);
					ClearJGJBuf();//清空缓存
					g_GlobalPara.iSaveDataCnt = 0;
					
					SendUiNo(UI_SAMPJGJ);
					g_GlobalPara.ucFromPageNum = UI_MAIN;//假装从主页进入
					LCD_FillRect(56,32,424,288,jbkCol);
					return;
				}else if(g_GlobalPara.iStorageFreePrcnt < 3){
					PromptDialog(DIALOGSTORLOW);
				}else{
					
				}
				
				SaveJGJFile();
				ui_SetGJNameNext((char*)JGJParaTable[0].pList->listStr);//设置下一构建名
				SaveSysPara();
			
				ClearJGJBuf();//清空缓存
				g_GlobalPara.iSaveDataCnt = 0;
				
				SendUiNo(UI_SAMPJGJ);
				g_GlobalPara.ucFromPageNum = UI_MAIN;//假装从主页进入
				LCD_FillRect(56,32,424,288,jbkCol);
				return;
			}
		}
		
		//存点
		if((g_u16KeyValBuf == KEY_FN)||(g_u8TouchResult == 3)){//存储测点值
			if(JGJSta == 0){
				JGJSta = 1;
			}
			if(JGJDir == 0){//横向
//////				g_DetectPara.HouduRes += 1;//测试
				WriteJGJCurDot(JGJNum,JGJDot,g_DetectPara.HouduRes);//存点
				JGJDot += 1;
				if((JGJDot%2) == 1){
					JGJDot -= 2;
					JGJNum += 1;
					if(JGJNum > JGJQnt){
						JGJPage = 1;
						JGJNum = 1;
						JGJDot += 2;
						if(JGJDot > 6){
							JGJDot = 1;
						}
						BrushJGJSketch(JGJPage,JGJNum);
					}
					JGJPageC = (JGJNum-1)/JGJPAGECAP+1;
					if(JGJPageC != JGJPage){
						JGJPage = JGJPageC;
						BrushJGJSketch(JGJPage,JGJNum);
					}
					BrushJGJDot(JGJPage,JGJNum,JGJDot);
					Sleep_ms(200);
					BrushJGJRebar(JGJPage,JGJNum,JGJDot);
				}else{
					BrushJGJDot(JGJPage,JGJNum,JGJDot);
				}
			}
            else{//纵向
//////				g_DetectPara.HouduRes += 1;//测试
				WriteJGJCurDot(JGJNum,JGJDot,g_DetectPara.HouduRes);
				JGJDot += 1;
				BrushJGJDot(JGJPage,JGJNum,JGJDot);
				if(JGJDot > 6){
					JGJDot = 1;
					JGJNum += 1;
					if(JGJNum > JGJQnt){
						JGJPage = 1;
						JGJNum = 1;
						BrushJGJSketch(JGJPage,JGJNum);
					}
					JGJPageC = (JGJNum-1)/JGJPAGECAP+1;
					if(JGJPageC != JGJPage){
						JGJPage = JGJPageC;
						BrushJGJSketch(JGJPage,JGJNum);
					}
					Sleep_ms(200);
					BrushJGJDot(JGJPage,JGJNum,JGJDot);
					BrushJGJRebar(JGJPage,JGJNum,JGJDot);
				}
			}
			DispSaveDataCnt(GetJGJSaveCnt());//显示检测计数
		}
		//标定
		if(g_u16KeyValBuf == LONG_KEY_FN){
			SendUiNo(UI_BIAODING);
			g_GlobalPara.ucFromPageNum = UI_SAMPJGJ;
			LCD_FillRect(56,32,424,288,jbkCol);
			return;
		}
		//电量低标定
		if(g_GlobalPara.iBatPcntLowFlag == 1){
			g_GlobalPara.iBatPcntLowFlag = 0;
			if(PromptDialog(DIALOGBIAODINGBATLOW) == 1){
				SendUiNo(UI_BIAODING);
				g_GlobalPara.ucFromPageNum = UI_SAMPJGJ;
				LCD_FillRect(56,32,424,288,jbkCol);
				return;
			}else{
				LCD_FillRect(90,50,360,220,jbkCol);//清空弹窗
				SampleJGJ_Init();
			}
		}
		//电量变化大标定
		if(abs(g_GlobalPara.iBatPercent-g_SysPara.iBatPercentBDLast) > 20){
			if(g_GlobalPara.iChargeSta == CHARGE_NO){
				if(PromptDialog(DIALOGBIAODINGBATJUMP) == 1){
					SendUiNo(UI_BIAODING);
					g_GlobalPara.ucFromPageNum = UI_SAMPJGJ;
					LCD_FillRect(56,32,424,288,jbkCol);
					return;
				}else{
					g_SysPara.iBatPercentBDLast = g_GlobalPara.iBatPercent;
					LCD_FillRect(90,50,360,220,jbkCol);//清空弹窗
					SampleJGJ_Init();
					SaveSysPara();
				}
			}
		}
		//温度变化大标定
		if(fabs(g_GlobalPara.fTemperature-g_SysPara.fTemperatureBDLast) > 10.0f){
			if(PromptDialog(DIALOGBIAODINGTEMPER) == 1){
				SendUiNo(UI_BIAODING);
				g_GlobalPara.ucFromPageNum = UI_SAMPJGJ;
				LCD_FillRect(56,32,424,288,jbkCol);
				return;
			}else{
				g_SysPara.fTemperatureBDLast = g_GlobalPara.fTemperature;
				LCD_FillRect(90,50,360,220,jbkCol);//清空弹窗
				SampleJGJ_Init();
				SaveSysPara();
			}
		}
		
		//返回
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_BACK)){
			g_GlobalPara.iSaveDataCnt = GetJGJSaveCnt();
			if(g_GlobalPara.iSaveDataCnt > 0){
				if(PromptDialog(DIALOGSAVE) == CONFIRM){
					//存储空间不足提示
					if(g_GlobalPara.iStorageFreePrcnt < 1){
						PromptDialog(DIALOGSTORNONE);
						ClearJGJBuf();//清空缓存
						g_GlobalPara.iSaveDataCnt = 0;
						
						SendUiNo(UI_SAMPJGJ);
						g_GlobalPara.ucFromPageNum = UI_MAIN;//假装从主页进入
						LCD_FillRect(56,32,424,288,jbkCol);
						return;
					}else if(g_GlobalPara.iStorageFreePrcnt < 3){
						PromptDialog(DIALOGSTORLOW);
					}else{
						
					}
					
					SaveJGJFile();
					ui_SetGJNameNext((char*)JGJParaTable[0].pList->listStr);//设置下一构建名
					SaveSysPara();
					
					ClearJGJBuf();//清空缓存
				}else{
					//不保存
				}
				g_GlobalPara.iSaveDataCnt = 0;
			}
			SendUiNo(UI_MAIN);
			LCD_FillRect(56,32,424,288,jbkCol);
			return;
		}
		//参数
		if(g_u8TouchResult == 1){
			SendUiNo(UI_SAMPSET);
			g_GlobalPara.ucFromPageNum = UI_SAMPJGJ;
			LCD_FillRect(56,32,424,288,jbkCol);
			return;
//			LCD_FillRect(60,0,420,320,WHITE);
//			JGJParaSet_Fun(JGJParaTable);
//			ui_SampJGJ_InitDisp();
		}
		//横向
		if(g_u8TouchResult == 2){
			if(JGJSta == 0){
				JGJDir = !JGJDir;
				g_SysPara.JGJScanDir = JGJDir;
				if(JGJDir == 0){
					OneBtnSet[0].btnID = 2;
					OneBtnSet[0].btnQnt = 5;
					OneBtnSet[0].btnIcon = (u8*)"横";
					OneBtnSet[0].btnName = (u8*)"横向";
					ui_BrushMenuOneBtn(&OneBtnSet[0],NORMALLIGHT);
					BrushJGJDot(JGJPage,JGJNum,JGJDot);
				}else{
					OneBtnSet[0].btnID = 2;
					OneBtnSet[0].btnQnt = 5;
					OneBtnSet[0].btnIcon = (u8*)"纵";
					OneBtnSet[0].btnName = (u8*)"纵向";
					ui_BrushMenuOneBtn(&OneBtnSet[0],NORMALLIGHT);
					BrushJGJDot(JGJPage,JGJNum,JGJDot);
				}
				SaveSysPara();
			}
		}
//		//存点
//		if(g_u8TouchResult == 3){
//			
//		}
		//标定
		if(g_u8TouchResult == 4){
			SendUiNo(UI_BIAODING);
			g_GlobalPara.ucFromPageNum = UI_SAMPJGJ;
			LCD_FillRect(56,32,424,288,jbkCol);
			return;
		}
		
//		//通道选择
//		if((g_u8TouchResult >= 5)&&(g_u8TouchResult <= 7)){
//			g_DetectPara.Channel = g_u8TouchResult - 5;
//			BrushChannelSel(g_DetectPara.Channel,0);
//		}
//		//拖动切换通道
//		for(i=0;i<3;i++){
//			if(g_ButtonBuf[17+i].btSta == BTN_STATUS_DRAG){
//				g_DetectPara.ChannelLast = g_DetectPara.Channel;
//				g_DetectPara.Channel = Get_DragChannel(&g_ButtonBuf[17],3,DRAG_ZONG);
//				if(g_DetectPara.Channel != g_DetectPara.ChannelLast){
//					BrushChannelSel(g_DetectPara.Channel,0);
//				}
//			}
//		}
		
		//选择钢筋
		if((g_u8TouchResult >= 5)&&(g_u8TouchResult <= 10)){
			//切换钢筋时设置上次钢筋是否合格
			SetJGJHegeCnt(JGJNum);
			JGJNum = (g_u8TouchResult-5+1)+(JGJPage-1)*JGJPAGECAP;
//			JGJDot = 1;
			if(JGJNum <= g_SysPara.g_SamplePara.iRebarNumJGJ){
//				LCD_SetColor(WHITE,BLACK);
//				LCD_ShowNnNums(ASC16x8,410,177,0,2,JGJNum);
				BrushJGJRebar(JGJPage,JGJNum,JGJDot);
				BrushJGJDot(JGJPage,JGJNum,JGJDot);
			}
		}
		
		//选择测点
		if((g_u8TouchResult >= 11)&&(g_u8TouchResult <= 13)){
			JGJDot = (g_u8TouchResult-11)*2+1;
//			DispJGJData(JGJPage,JGJNum);
			BrushJGJDot(JGJPage,JGJNum,JGJDot);
		}
		
		//翻页
		if(g_u8TouchResult == SLIDE_END_RIGHT){//+BTN_NUM_LONGPRESS_HOLD){
			if(Is_DesignArea_Slide(56,200,239,319)){
				JGJPage -= 1;
				if(JGJPage < 1)JGJPage = JGJPageQnt;
				//显示JGJ示意图
				if(JGJPageQnt > 1){
					BrushJGJSketch(JGJPage,JGJNum);
//					BrushJGJRebar(JGJPage,JGJNum,JGJDot);
					BrushJGJDot(JGJPage,JGJNum,JGJDot);
				}
			}
		}
		if(g_u8TouchResult == SLIDE_END_LEFT){//+BTN_NUM_LONGPRESS_HOLD){
			if(Is_DesignArea_Slide(56,200,239,319)){
				JGJPage += 1;
				if(JGJPage > JGJPageQnt)JGJPage = 1;
				//显示JGJ示意图
				if(JGJPageQnt > 1){
					BrushJGJSketch(JGJPage,JGJNum);
//					BrushJGJRebar(JGJPage,JGJNum,JGJDot);
					BrushJGJDot(JGJPage,JGJNum,JGJDot);
				}
			}
		}
		
		Sleep_ms(5);
	}
}

void SampleJGJ_Init(void)
{
	u8 i;
	u16 jgjBk;
	u16 jgjTx;
	u16 jgjBar;
	
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		jgjBk = BLACK;
		jgjTx = WHITE;
		jgjBar = GRAY;
	}else{
		jgjBk = WHITE;
		jgjTx = BLACK;
		jgjBar = DGRAY;
	}
	
	//页面左侧菜单表
	MENU_TABLE_STRUCT JGJMenu[5] = {
		{0,5,"徊",NULL},
		{1,5,"参","参数"},
		{2,5,"横","横向"},
		{3,5,"存","存点"},
		{4,5,"标","标定"}
	};
	
	if(JGJDir == 0){
		JGJMenu[2].btnIcon = "横";
		JGJMenu[2].btnName = "横向";
	}else{
		JGJMenu[2].btnIcon = "纵";
		JGJMenu[2].btnName = "纵向";
	}
	//菜单显示
	ui_DispMenuBtn(JGJMenu,0);
	
	//状态栏
	LCD_DrawRect(56,0,424,32,jgjBk);
	LCD_FillRect(57,1,422,31,jgjBar);
	//分割线
	LCD_FillRect(57,192,422,2,jgjTx);//横
	for(i=0;i<42;i++){
		LCD_SetColor(jgjBk,jgjTx);
		LCD_ShowChinese(Font8,58+i*10,192,1,"杠");
	}
	
	//JGJ
	LCD_FillRect(239,208,1,105,DGRAY);//竖
	LCD_FillRect(239+60,208,1,65,DGRAY);//竖
	LCD_FillRect(239+60+30,208,1,32,DGRAY);//竖
	LCD_FillRect(239+60+60,208,1,65,DGRAY);//竖
	LCD_FillRect(239+60+60+30,208,1,32,DGRAY);//竖
	LCD_FillRect(239+60+60+60,208,1,65,DGRAY);//竖
	LCD_FillRect(239+60+60+60+30,208,1,32,DGRAY);//竖
	LCD_FillRect(239,208+32,240,1,DGRAY);//横
	LCD_FillRect(239,208+32*2+1,240,1,DGRAY);//横
	
//	LCD_FillRect(240+60,176+18*3,60,1,DGRAY);//横
//	LCD_FillRect(240,176+18*4,180,1,DGRAY);//横
//	LCD_FillRect(240+60,176+18*5,60,1,DGRAY);//横
//	LCD_FillRect(240,176+18*6,180,1,DGRAY);//横
//	LCD_FillRect(240+60,176+18*7,60,1,DGRAY);//横
	
	LCD_SetColor(jgjBar,WHITE);
	//钢筋类型
	LCD_ShowChinese(Font16,164,8,0,JGJParaTable[7].pList[g_SysPara.g_SamplePara.iRebarType].listStr);
	LCD_ShowChinese(Font20,198,6,0,"φ");//主筋直径
	LCD_ShowNums(ASC20x10,216,6,0,*JGJParaTable[1].ParaAddr);
	
	LCD_ShowChinese(Font24,410,5,0,"计");//计数图标
	DispSaveDataCnt(g_GlobalPara.iSaveDataCnt);
	
	
	LCD_SetColor(jgjBk,jgjTx);
	LCD_ShowString(ASC24x12,380,164,0,"   ");//厚度值
	LCD_ShowString(ASC16x8,456,176,0,"m");
	LCD_ShowString(ASC16x8,466,176,0,"m");
	
	//JGJ数据显示
	LCD_SetColor(jgjBk,jgjTx);
	LCD_ShowChinese(Font14,249,216,0,"检测值");
	LCD_ShowChinese(Font14,249,216+32+2,0,"平均值");
	LCD_ShowChinese(Font14,249,216+32+42,0,"钢筋序号");
	LCD_ShowString(ASC16x8,363,216+32+42-1,0,"JGJ");
	LCD_ShowChinese(Font14,389,216+32+42,0,"均值");
	
	//钢筋序号
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		LCD_SetColor(jgjBk,GOLD);
	}else{
		LCD_SetColor(jgjBk,BLACK);
	}
	LCD_ShowNums_Center(ASC20x10,308,287,24,20,0,JGJNum);
	
	//瞄准镜
	BrushZXX(GRAY);
	BrushMZJ(g_DetectPara.MZJSite,FAR);
	
	
	//按钮初始化
	g_u8ButtonNums = 14;
	struct BUTTON SampBtn[14] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0},//0, 返回
		{  0,  50,  59, 103, 0,0},//1, 参数
		{  0, 104,  59, 157, 0,0},//2, 显示
		{  0, 158,  59, 211, 0,0},//3, 分析
		{  0, 212,  59, 265, 0,0},//4, 标定
		
		{ 68, 177,  95, 319, 0,0},//5, 钢筋1
		{ 96, 177, 123, 319, 0,0},//6, 钢筋2
		{124, 177, 151, 319, 0,0},//7, 钢筋3
		{152, 177, 179, 319, 0,0},//8, 钢筋4
		{180, 177, 207, 319, 0,0},//9, 钢筋5
		{208, 177, 235, 319, 0,0},//10, 钢筋6
		
		{300, 208, 359, 272, 0,0},//11, 测点1
		{360, 208, 419, 272, 0,0},//12, 测点2
		{420, 208, 479, 272, 0,0},//13, 测点3
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	if(g_GlobalPara.ucFromPageNum == UI_MAIN){
		//初始化参数
		g_DetectPara.MZJSite = 0;
//		g_DetectPara.CarSite = 0;
		
		//通道选择
		g_DetectPara.Channel = 1;
		g_DetectPara.ChannelLast = 1;
		
		g_GlobalPara.iSaveDataCnt = 0;
		g_GlobalPara.ucHouduAvg = 0;
		g_GlobalPara.ucHouduMin = 0;
		g_GlobalPara.ucHouduMax = 0;
        
		//清空缓存
		memset(&ResBuf_CH1,0x0,sizeof(ResBuf_CH1));
		memset(&ResBuf_CH2,0x0,sizeof(ResBuf_CH2));
		memset(&ResBuf_CH3,0x0,sizeof(ResBuf_CH3));
	}
	sprintf((char*)JGJParaTable[0].pList->listStr,"%s",g_SysPara.GouJianName);
	ui_DispGJName(ASC16x8,70,8,WHITE,(char*)JGJParaTable[0].pList->listStr);
    
    //检测参数赋值
    g_tWorkPara.cZhujinDiam = g_SysPara.g_SamplePara.iZhujinDiam;//主筋直径
    g_tWorkPara.cZhujinDistance = 0;	//主筋间距
    g_tWorkPara.cGujinDiam = 0;		    //箍筋直径
    g_tWorkPara.cGujinDistance = 0;	    //箍筋间距
	
    g_tWorkPara.cGangJinType = 0;		//钢筋类型
    g_tWorkPara.cLingChengType = 1;	    //大小量程
	
    g_tWorkPara.cJiLiang = 0;			//计量模式
    g_tWorkPara.cShejiHD = 30;			//设计厚度
	
    ClearTouchSta();//清除触摸
	
	//通道选择
//	BrushChannelSel(g_DetectPara.Channel,0);
	
	//显示JGJ示意图
	BrushJGJSketch(JGJPage,JGJNum);
	BrushJGJRebar(JGJPage,JGJNum,JGJDot);
	BrushJGJDot(JGJPage,JGJNum,JGJDot);
	
	ClearTouchSta();//清除触摸
}

///////////////////////////////////////////////////////////////////////////////////////////////
//厚度平均值
void GetJGJHouDuAvg(void)
{
	u16 i,j;
	int sum,allSum;
	u8 avg,allAvg;
	u8 cnt,allCnt;
	
	g_GlobalPara.ucHouduMin = 255;
	g_GlobalPara.ucHouduMax = 0;
	
	allSum = 0;
	allAvg = 0;
	allCnt = 0;
	for(i=0;i<JGJQnt;i++){
		sum = 0;
		avg = 0;
		cnt = 0;
		for(j=0;j<3;j++){
			if(JGJData[i][2+j*3] > 0){
				sum += JGJData[i][2+j*3];
				cnt += 1;
				avg = sum/cnt;
			}
		}
		if(cnt > 0){
			allSum += avg;
			allCnt += 1;
			allAvg = allSum/allCnt;
			if(avg < g_GlobalPara.ucHouduMin){
				g_GlobalPara.ucHouduMin = avg;
			}
			if(avg > g_GlobalPara.ucHouduMax){
				g_GlobalPara.ucHouduMax = avg;
			}
			g_GlobalPara.ucHouduAvg = allAvg;
		}
	}
}

//存储构件
void SaveJGJFile(void)
{
	FRESULT fres;
	int offset = 0;
	u8 fileDir[32];
	
	//文件数据头初始化
	FlieDataHead.iVersion = 3;
	sprintf(FlieDataHead.YiQiBianHao,"%s",g_SysPara.EquipmentNum);
	sprintf(FlieDataHead.GouJianName,"%s",JGJParaTable[0].pList->listStr);
	
	memcpy(&FlieDataHead.FileTimeStruct,&g_SysTime,sizeof(g_SysTime));
	
	FlieDataHead.ucDetectType = JGJ;//检测类型
	FlieDataHead.ucGouJianType = *JGJParaTable[6].ParaAddr;;//构件类型
	FlieDataHead.ucRebarType = *JGJParaTable[7].ParaAddr;;//钢筋类型
	
	FlieDataHead.ucZhuJinDiam = *JGJParaTable[1].ParaAddr;//主筋直径
	FlieDataHead.ucZhuJinJianju = *JGJParaTable[2].ParaAddr;//主筋间距
	FlieDataHead.ucGuJinDiam = *JGJParaTable[3].ParaAddr;//箍筋直径
	FlieDataHead.ucGuJinJianju = *JGJParaTable[4].ParaAddr;//箍筋间距
	FlieDataHead.ucHouDuDesign = *JGJParaTable[5].ParaAddr;;//设计厚度
	
	GetJGJHouDuAvg();
	
	FlieDataHead.ucHouDuAvg = (u8)g_GlobalPara.ucHouduAvg;
	FlieDataHead.ucHouDuMin = (u8)g_GlobalPara.ucHouduMin;
	FlieDataHead.ucHouDuMax = (u8)g_GlobalPara.ucHouduMax;
	
	FlieDataHead.iSaveDotCntCh1 = GetJGJSaveCnt();//检测个数
//	FlieDataHead.iSaveDotCntCh2 = ResBuf_CH2.AllRebarCnt;
//	FlieDataHead.iSaveDotCntCh3 = ResBuf_CH3.AllRebarCnt;
	
	FlieDataHead.iHeGeDotCntCh1 = JGJHegeCnt;//合格数
//	FlieDataHead.iHeGeDotCntCh2 = g_GlobalPara.iSaveDataCnt;
//	FlieDataHead.iHeGeDotCntCh3 = g_GlobalPara.iSaveDataCnt;
	FlieDataHead.ucJGJNumber = JGJQnt;//构件中钢筋根数
	
	sprintf((char*)fileDir,"0:/GY91/%s.bin",FlieDataHead.GouJianName);
	fres = f_open(file,(const char*)fileDir,FA_WRITE|FA_READ|FA_CREATE_NEW);
	if(fres == FR_EXIST){
		if(PromptDialog(DIALOGGOUJIANEXIST)){
			fres = f_open(file,(const char*)fileDir,FA_WRITE|FA_READ|FA_CREATE_ALWAYS);
		}else{
			return;
		}
	}
	f_write(file,&FlieDataHead,sizeof(FlieDataHead),&bw);//写文件头
	offset = 128;//文件数据从128字节开始写入
	f_lseek(file,offset);
	f_write(file,JGJData,JGJQnt*9,&bw);//写数据ch1
	f_close(file);
	
	PromptTone_Once(120);//提示音
}

//清空JGJ缓存
void ClearJGJBuf(void)
{
	memset(JGJData,0x0,sizeof(JGJData));
}
//清除测点数据
void ClearJGJData(void)
{
	u8 i,j;
	u8 n;
	
	u16 cbkCol;
	u16 ctxCol;
	u16 avgCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		cbkCol = BLACK;
		ctxCol = WHITE;
		avgCol = GOLD;
	}else{
		cbkCol = WHITE;
		ctxCol = BLACK;
		avgCol = BLACK;
	}
	
	for(i=0;i<3;i++){
		for(j=0;j<2;j++){
			n = i*2+j;
			LCD_SetColor(cbkCol,ctxCol);
//			LCD_FillRect(300+30*n,208,29,32,WHITE);
			LCD_ShowText_Center(ASC16x8,300+30*n,208,29,32,0,"--");
		}
		n = i;
		LCD_SetColor(cbkCol,ctxCol);
//		LCD_FillRect(300+60*n,208,59,32,WHITE);
		LCD_ShowText_Center(ASC16x8,300+60*n,208+32+1,59,32,0,"--");
	}
	LCD_SetColor(cbkCol,avgCol);
	LCD_ShowText_Center(ASC32x16,420,276,56,40,0,"--");
}

//显示实时测点和数据
//jgjNum:1-12,dotNum:1-6
void WriteJGJCurDot(u8 jgjNum,u8 dotNum,u8 houdu)
{
	u8 i;
	u8 dotn,dotnz;
//	u8 rebarNum;
	u8 houduAvg;
	u8 avgFlag = 0;
	
	u16 wbkCol;
	u16 wtxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		wbkCol = BLACK;
		wtxCol = WHITE;
	}else{
		wbkCol = WHITE;
		wtxCol = BLACK;
	}
	
	dotn = (dotNum - 1)/2;//测点序号0-2
	dotnz = (dotNum - 1)%2;//
//	rebarNum = (jgjNum-1)%JGJPAGECAP;
	
	JGJData[jgjNum-1][dotn*3+dotnz] = houdu;
	if(dotnz != 0){//计算两数平均值
		JGJData[jgjNum-1][dotn*3+dotnz+1] = BankersRounding(JGJData[jgjNum-1][dotn*3+dotnz],JGJData[jgjNum-1][dotn*3+dotnz-1]);
	}
	//显示箭头
//	LCD_SetColor(WHITE,GOLD);
//	LCD_ShowChinese(Font16,62+rebarNum*28,206+16-3+(dotNum-1)/2*32,0,"箭");
	//显示图形和数据
	if(dotNum%2 == 0){
		//更新测点图标
//		LCD_FillRect(74+6+rebarNum*28,206+16+dotn*32,8,10,GREEN);
		//显示厚度值
		LCD_SetColor(GOLD,BLACK);
		LCD_ShowNums_Center(ASC16x8,300+30*(dotNum-1),208,29,32,0,JGJData[jgjNum-1][dotn*3+dotnz]);
		LCD_SetColor(wbkCol,wtxCol);
		LCD_ShowNums_Center(ASC16x8,300+60*(dotn),208+32+1,59,32,0,JGJData[jgjNum-1][dotn*3+dotnz+1]);
		
		if(dotNum == 6){//测完
			avgFlag = 1;
		}else{
			avgFlag = 1;
			for(i=0;i<3;i++){
				if(JGJData[jgjNum-1][2+i*3] == 0){
					avgFlag = 0;
					break;
				}
			}
		}
		if(avgFlag == 1){
			houduAvg = (u8)((float)(JGJData[jgjNum-1][2]+JGJData[jgjNum-1][5]+JGJData[jgjNum-1][8])/3+0.5f);
			//深色背景
			if(g_SysPara.iTheme == THEME_DARK){
				LCD_SetColor(BLACK,GOLD);
			}else{
				LCD_SetColor(WHITE,BLACK);
			}
			if(houduAvg < 100){
				LCD_ShowNums_Center(ASC40x20,420,276,56,40,0,houduAvg);
			}else{
				LCD_ShowNums_Center(ASC32x16,420,276,56,40,0,houduAvg);
			}
			//检测是否合格并设置合格计数
			SetJGJHegeCnt(jgjNum);
		}
	}else{
		//更新测点图标
//		LCD_FillRect(74+6+rebarNum*28,206+16+dotn*32,8,10,GOLD);
		//显示厚度值
		LCD_SetColor(GOLD,BLACK);
		LCD_ShowNums_Center(ASC16x8,300+30*(dotNum-1),208,29,32,0,JGJData[jgjNum-1][dotn*3+dotnz]);
		LCD_SetColor(wbkCol,wtxCol);
		LCD_ShowText_Center(ASC16x8,300+60*(dotn),208+32+1,59,32,0,"--");
		JGJData[jgjNum-1][dotn*3+dotnz+1] = 0;
		JGJData[jgjNum-1][dotn*3+dotnz+2] = 0;
	}
}

//显示选中钢筋的全部测点数据
void DispJGJData(u8 jgjPageNum,u8 jgjNum,u8 dotNum)
{
	u8 i,j;
	u8 n;
	u16 data;
	u8 avgFlag = 1;
	u16 jgjAvg = 0;
	
	u16 sbkCol;
	u16 stxCol;
	u16 avgCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		sbkCol = BLACK;
		stxCol = WHITE;
		avgCol = GOLD;
	}else{
		sbkCol = WHITE;
		stxCol = BLACK;
		avgCol = BLACK;
	}
	
	for(i=0;i<3;i++){
		for(j=0;j<2;j++){
			n = i*2+j;
			data = JGJData[jgjNum-1][i*3+j];
			
			if(data > 0){
				if(n == dotNum-1){//高亮
					LCD_SetColor(GOLD,BLACK);
				}else{
					LCD_SetColor(sbkCol,stxCol);
				}
				LCD_ShowNums_Center(ASC16x8,300+30*n,208,29,32,0,data);
			}else{
				if(n == dotNum-1){//高亮
					LCD_SetColor(GOLD,BLACK);
					LCD_ShowText_Center(ASC16x8,300+30*n,208,29,32,0,"--");
				}else{
//					LCD_FillRect(301+30*n,208,29,32,WHITE);
					LCD_SetColor(sbkCol,stxCol);
					LCD_ShowText_Center(ASC16x8,300+30*n,208,29,32,0,"--");
				}
			}
		}
		n = i;
		data = JGJData[jgjNum-1][i*3+2];
		LCD_SetColor(sbkCol,stxCol);
		if(data > 0){
			jgjAvg += data;
			LCD_ShowNums_Center(ASC16x8,300+60*n,208+32+1,59,32,0,JGJData[jgjNum-1][i*3+2]);
		}else{
			avgFlag = 0;
//			LCD_FillRect(301+60*n,208+32+1,59,32,WHITE);
			LCD_SetColor(sbkCol,stxCol);
			LCD_ShowText_Center(ASC16x8,300+60*n,208+32+1,59,32,0,"--");
		}
	}
	if(avgFlag == 1){
		jgjAvg = (u16)((float)jgjAvg/3+0.5f);
		LCD_SetColor(sbkCol,avgCol);
		if(jgjAvg < 100){
			LCD_ShowNums_Center(ASC40x20,420,276,56,40,0,jgjAvg);
		}else{
			LCD_ShowNums_Center(ASC32x16,420,276,56,40,0,jgjAvg);
		}
	}else{
		LCD_SetColor(sbkCol,avgCol);
		LCD_ShowText_Center(ASC32x16,420,276,56,40,0,"--");
	}
}
//显示箭头，切换测点时调用
//jgjPageNum:1-2,jgjNum:1-12,dotNum:1-6
void BrushJGJDot(u8 jgjPageNum,u8 jgjNum,u8 dotNum)
{
	u8 i;
	u8 dotn,dotnz;
	u8 rebarNum;
	u8 rebarPage;
	u8 jgjNumQnt;
	static u8 dotNumLast = 1;
	
	u16 dbkCol;
	u16 dtxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		dbkCol = BLACK;
		dtxCol = WHITE;
	}else{
		dbkCol = WHITE;
		dtxCol = BLACK;
	}
	
	jgjNumQnt = g_SysPara.g_SamplePara.iRebarNumJGJ;
	
	dotn = (dotNum - 1)/2;//测点序号0-2
	dotnz = (dotNum - 1)%2;//
	rebarNum = (jgjNum-1)%JGJPAGECAP;
	rebarPage = (jgjNum-1)/JGJPAGECAP+1;
	if(dotNum > 6){//测完一根钢筋
		//清除测点显示
		if((rebarPage == jgjPageNum)&&(jgjNum <= jgjNumQnt)){
			for(i=0;i<3;i++){
				LCD_SetColor(dbkCol,dbkCol);//清除箭头
				if(JGJDir == 0){//横向
					LCD_ShowChinese(Font12,60+rebarNum*28,240+i*24,0,"剑");
				}else{//纵向
					LCD_ShowChinese(Font12,60+rebarNum*28,240+i*24,0,"箭");
				}
				//测点
				if((JGJData[jgjNum-1][i*3]>0)&&(JGJData[jgjNum-1][i*3+1])>0){
					LCD_FillRect(76+rebarNum*28,241+i*24,8,10,BLUE);
				}else{
					LCD_FillRect(76+rebarNum*28,241+i*24,8,10,GRAY);
				}
			}
		}
	}else{
		if((rebarPage == jgjPageNum)&&(jgjNum <= jgjNumQnt)){
			if(dotnz == 0){//当前测点
				for(i=0;i<3;i++){
					if(i == dotn){
						LCD_SetColor(dbkCol,GOLD);//画箭头
						if(JGJDir == 0){//横向
							LCD_ShowChinese(Font12,60+rebarNum*28,240+i*24,0,"剑");
						}else{//纵向
							LCD_ShowChinese(Font12,60+rebarNum*28,240+i*24,0,"箭");
						}
						//测点
						LCD_FillRect(76+rebarNum*28,241+i*24,8,10,GOLD);
						//序号
//						LCD_SetColor(GOLD,BLACK);
//						LCD_ShowNums_Center(ASC16x8,240,177+36*(i+1),60,35,0,i+1);
					}else{
						LCD_SetColor(dbkCol,dbkCol);//清除上次箭头
						if(JGJDir == 0){//横向
							LCD_ShowChinese(Font12,60+rebarNum*28,240+i*24,0,"剑");
						}else{//纵向
							LCD_ShowChinese(Font12,60+rebarNum*28,240+i*24,0,"箭");
						}
						//测点
						if((JGJData[jgjNum-1][i*3]>0)&&(JGJData[jgjNum-1][i*3+1])>0){
							LCD_FillRect(76+rebarNum*28,241+i*24,8,10,BLUE);
						}else{
							LCD_FillRect(76+rebarNum*28,241+i*24,8,10,GRAY);
						}
						//序号
//						LCD_SetColor(WHITE,BLACK);
//						LCD_ShowNums_Center(ASC16x8,240,177+36*(i+1),60,35,0,i+1);
					}
				}
			}else{
			}
		}
		//显示高亮测点数据
		if(dotNum != dotNumLast){
			LCD_SetColor(GOLD,BLACK);
			if(JGJData[jgjNum-1][dotn*3+dotnz] > 0){
				LCD_ShowNums_Center(ASC16x8,300+30*(dotNum-1),208,29,32,0,JGJData[jgjNum-1][dotn*3+dotnz]);
			}else{
				LCD_ShowText_Center(ASC16x8,300+30*(dotNum-1),208,29,32,0,"--");
			}
			//清除上次高亮测点数据
			dotn = (dotNumLast - 1)/2;//测点序号0-2
			dotnz = (dotNumLast - 1)%2;//
			LCD_SetColor(dbkCol,dtxCol);
			if(JGJData[jgjNum-1][dotn*3+dotnz] > 0){
				LCD_ShowNums_Center(ASC16x8,300+30*(dotNumLast-1),208,29,32,0,JGJData[jgjNum-1][dotn*3+dotnz]);
			}else{
				LCD_ShowText_Center(ASC16x8,300+30*(dotNumLast-1),208,29,32,0,"--");
			}
		}
		dotNumLast = dotNum;
	}
}

//刷新钢筋图标显示，切换钢筋时调用
//jgjPageNum:1-2,jgjNum:1-12,dotNum:1-6
void BrushJGJRebar(u8 jgjPageNum,u8 jgjNum,u8 dotNum)
{
	u8 i;
	u8 dotn;
	u8 rebarNum;
	u8 rebarPage;
	u8 jgjNumQnt;
	static u8 jgjNumLast = 1;
	
	u16 rbkCol;
	u16 rtxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		rbkCol = BLACK;
		rtxCol = WHITE;
	}else{
		rbkCol = WHITE;
		rtxCol = BLACK;
	}
	
	jgjNumQnt = g_SysPara.g_SamplePara.iRebarNumJGJ;
	
	//清除上次显示
	rebarNum = (jgjNumLast-1)%JGJPAGECAP;
	rebarPage = (jgjNumLast-1)/JGJPAGECAP+1;
	for(i=0;i<3;i++){
		LCD_SetColor(rbkCol,rbkCol);//清除上次箭头
		if(JGJDir == 0){//横向
			LCD_ShowChinese(Font12,60+rebarNum*28,240+i*24,0,"剑");
		}else{//纵向
			LCD_ShowChinese(Font12,60+rebarNum*28,240+i*24,0,"箭");
		}
	}
	if((rebarPage == jgjPageNum)&&(jgjNum <= jgjNumQnt)){
		LCD_FillRect(72+rebarNum*28,210,16,16,GRAY);//序号
		LCD_SetColor(GRAY,rtxCol);
		LCD_ShowNums_Center(ASC12x6,72+rebarNum*28,210,16,16,1,jgjNumLast);
		
		for(i=0;i<3;i++){
//			LCD_SetColor(WHITE,WHITE);//清除上次箭头
//			LCD_ShowChinese(Font16,62+rebarNum*28,206+16-3+i*32,0,"箭");
			
			if((JGJData[jgjNumLast-1][i*3]>0)&&(JGJData[jgjNumLast-1][i*3+1])>0){
				LCD_FillRect(76+rebarNum*28,241+i*24,8,10,BLUE);
			}else{
				LCD_FillRect(76+rebarNum*28,241+i*24,8,10,GRAY);
			}
		}
	}
	//显示当前钢筋
	dotn = (dotNum - 1)/2;//测点序号0-2
	rebarNum = (jgjNum-1)%JGJPAGECAP;
	rebarPage = (jgjNum-1)/JGJPAGECAP+1;
	if((rebarPage == jgjPageNum)&&(jgjNum <= jgjNumQnt)){
		LCD_FillRect(72+rebarNum*28,210,16,16,GOLD);//序号
		LCD_SetColor(GOLD,BLACK);
		LCD_ShowNums_Center(ASC12x6,72+rebarNum*28,210,16,16,1,jgjNum);
		
		LCD_SetColor(rbkCol,GOLD);//显示当前箭头
		if(JGJDir == 0){//横向
			LCD_ShowChinese(Font12,60+rebarNum*28,240+dotn*24,0,"剑");
		}else{//纵向
			LCD_ShowChinese(Font12,60+rebarNum*28,240+dotn*24,0,"箭");
		}
		
		for(i=0;i<3;i++){
			if(i == dotn){
				LCD_FillRect(76+rebarNum*28,241+i*24,8,10,GOLD);
			}else{
				if((JGJData[jgjNum-1][i*3]>0)&&(JGJData[jgjNum-1][i*3+1])>0){
					LCD_FillRect(76+rebarNum*28,241+i*24,8,10,BLUE);
				}else{
					LCD_FillRect(76+rebarNum*28,241+i*24,8,10,GRAY);
				}
			}
		}
		//显示JGJ序号
		//深色背景
		if(g_SysPara.iTheme == THEME_DARK){
			LCD_SetColor(rbkCol,GOLD);
		}else{
			LCD_SetColor(rbkCol,BLACK);
		}
		LCD_ShowNums_Center(ASC20x10,308,287,24,20,0,jgjNum);
		
		//显示钢筋数据
		DispJGJData(jgjPageNum,jgjNum,dotNum);
		//设置检测前合格标志
		GetJGJHegeFlagBefore(jgjNum);
		//显示调试数据，合格数
		if(debug == 1){
			LCD_SetColor(rbkCol,RED);
			LCD_ShowString(ASC16x8,302,177,0,"HeGe");
			LCD_ShowNums(ASC16x8,335,177,0,JGJHegeCnt);
		}
	}
	jgjNumLast = jgjNum;
}
//显示JGJ示意图
void BrushJGJSketch(u8 jgjPageNum,u8 jgjNum)
{
	u8 i,j;
	u8 jgjNumQnt;
	u8 startNum;
	u8 curNum;
//	u8 rebarNum;
	
	u16 kbkCol;
	u16 ktxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		kbkCol = BLACK;
		ktxCol = WHITE;
	}else{
		kbkCol = WHITE;
		ktxCol = BLACK;
	}
	
	jgjNumQnt = g_SysPara.g_SamplePara.iRebarNumJGJ;
	startNum = (jgjPageNum-1)*JGJPAGECAP;
	
	//翻页清除箭头
//	if((jgjNum-1)/6+1 != jgjPageNum){
//		rebarNum = (jgjNum-1)%JGJPAGECAP;
		for(i=0;i<JGJPAGECAP;i++){
			for(j=0;j<3;j++){
				LCD_SetColor(kbkCol,kbkCol);//清除箭头
				if(JGJDir == 0){//横向
					LCD_ShowChinese(Font12,60+i*28,240+j*24,0,"剑");
				}else{//纵向
					LCD_ShowChinese(Font12,60+i*28,240+j*24,0,"箭");
				}
			}
		}
//	}
	//刷新钢筋
	for(i=0;i<JGJPAGECAP;i++){
		curNum = startNum + i;
		if(curNum > jgjNumQnt-1){
			LCD_FillRect(72+i*28,210,16,16,kbkCol);//清除序号
			LCD_FillRect(76+i*28,228,8,84,kbkCol);//清除钢筋
		}else{
			if(curNum == jgjNum-1){//当前钢筋
				LCD_FillRect(72+i*28,210,16,16,GOLD);//序号
			}else{
				LCD_FillRect(72+i*28,210,16,16,GRAY);//序号
			}
			LCD_SetColor(GRAY,ktxCol);
			LCD_ShowNums_Center(ASC12x6,72+i*28,210,16,16,1,curNum+1);//序号
			LCD_FillRect(78+i*28,228,4,84,GRAY);//钢筋
			for(j=0;j<3;j++){
				if((JGJData[curNum][j*3]>0)&&(JGJData[curNum][j*3+1]>0)){
					LCD_FillRect(76+i*28,241+j*24,8,10,BLUE);//测点
				}else{
					LCD_FillRect(76+i*28,241+j*24,8,10,GRAY);//清除测点
				}
			}
		}
	}
}

//检测钢筋存储计数
u8 GetJGJSaveCnt(void)
{
	u8 i,j;
	u8 cnt = 0;
	for(i=0;i<JGJQnt;i++){
		for(j=0;j<3;j++){
			if(JGJData[i][2+j*3] > 0){
				cnt += 1;
				break;
			}
		}
	}
	return cnt;
}
//判断钢筋是否已测过
u8 IsRebarTested(u8 jgjNum)
{
	u8 i,j;
	for(i=0;i<3;i++){
		for(j=0;j<2;j++){
			if(JGJData[jgjNum-1][i*3+j] > 0){
				return 1;
			}
		}
	}
	return 0;
}
//判断钢筋是否合格
u8 IsRebarHege(u8 jgjNum)
{
	u8 i;
	u8 houduAvg;
	u8 goujianType,designHoudu;
	
	goujianType = g_SysPara.g_SamplePara.iGoujianType;
	designHoudu = g_SysPara.g_SamplePara.iDesignHoudu;
	
	for(i=0;i<3;i++){
		if(JGJData[jgjNum-1][2+i*3] == 0){
			return 0;
		}
	}
	
	houduAvg = (u8)((float)(JGJData[jgjNum-1][2]+JGJData[jgjNum-1][5]+JGJData[jgjNum-1][8])/3+0.5f);

	if(goujianType == 0){//梁 正十负七
		if((houduAvg <= (designHoudu+10))&&(houduAvg >= (designHoudu-7))){
            return 1;
        }
        else{
            return 0;
        }
	}else{//板 正八负五
		if((houduAvg <= (designHoudu+8))&&(houduAvg >= (designHoudu-5))){
            return 1;
        }
        else{
            return 0;
        }
	}
}
//获取检测前钢筋合格标志
void GetJGJHegeFlagBefore(u8 jgjNum)
{
	//设置合格标志
	if(IsRebarTested(jgjNum)){//钢筋已测过
		JGJHegeFlagBefore = IsRebarHege(jgjNum);
	}else{
		JGJHegeFlagBefore = 0;
	}
}
//获取检测前钢筋合格标志
void GetJGJHegeFlagAfter(u8 jgjNum)
{
	//检测钢筋是否合格
	JGJHegeFlagAfter = IsRebarHege(jgjNum);
}

//设置钢筋合格计数
void SetJGJHegeCnt(u8 jgjNum)
{
	GetJGJHegeFlagAfter(jgjNum);
	if((JGJHegeFlagBefore == 0)&&(JGJHegeFlagAfter == 1)){
		//合格计数加1
		JGJHegeCnt += 1;
	}
	if((JGJHegeFlagBefore == 1)&&(JGJHegeFlagAfter == 0)){
		//合格计数减1
		JGJHegeCnt -= 1;
	}
}
//计算JGJ平均值
u8 GetJGJAverage(u8 jgjNum)
{
	u8 res;
	u16 sum;
	float avg;
	sum = JGJData[jgjNum-1][2]+JGJData[jgjNum-1][5]+JGJData[jgjNum-1][8];
	avg = (float)sum/3+0.5f;
	res = (u8)avg;
	return res;
}
//四舍六入五成双,不保留小数
u8 BankersRounding(u8 val1,u8 val2)
{
	u8 res;
	u16 sum;
	
	sum = val1+val2;
	res = sum/2;
	
	if((sum%2) != 0){
		if((res%2) != 0){
			res += 1;
		}
	}
	
	return res;
}
