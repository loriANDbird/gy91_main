/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：参数设置
    1. ui_Paraset10 界面初始化，
    2. ui_Paraset10_Run 函数实现
   
更新记录：
    1. 2018.03.01 初次创建
*******************************************************************************/
#include "ui_Paraset10.h"
#include "Config_ui.h"
#include "OpPara.h"

static int *paraFi_p;
static int *paraZero_p;

static int BKLight;
static int BKLightLast;

//static u8 HighLightID;
static SYS_PARA_TABLE_STRUCT* SysParaTable = &ParaTable[PARATABLEINDEX_SYSTEM];

void RadioButtonDisp(u16 x,u16 y,u8 value);
void DoubleButtonDisp(u16 x,u16 y,SYS_PARA_TABLE_STRUCT* ParaTableArr);
void BKLightDisp(u16 x,u16 y,u8 bkVal);
void DialogParaDisp(u16 x,u16 y,SYS_PARA_TABLE_STRUCT* ParaTableArr);
void SysParaOpt(u8 pageNum,u8 optNum,SYS_PARA_TABLE_STRUCT* ParaTableArr);
void SysParaOptRes(u8 pageNum,u8 optNum,SYS_PARA_TABLE_STRUCT* ParaTableArr);
void BrushPageInit(u8 page);
void SysParaSet_Init(SYS_PARA_TABLE_STRUCT* SysParaTable,u8 pageNum);
void ConfigFiZero(u8 mode);
void About_Fun(void);

void ui_Paraset10_InitDisp(void)
{
//	HighLightID = 0xff;//无高亮
	SysParaSet_Init(SysParaTable,1);
}

void ui_Paraset10_Run(void)
{
	u8 i;
	u8 hindex;
	u8 pageNum = 1;//第一页
	u8 pageLast = 1;//上一页
	u8 pageQnt;	   //页数
	
	u16 BKCol;
	u16 menuBtCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		BKCol = BLACK;
		menuBtCol = DGRAY;
	}else{
		BKCol = WHITE;
		menuBtCol = LGRAY;
	}
	pageQnt = 2;
	
//	HighLightID = 0xff;//无高亮
//	SysParaSet_Init(SysParaTable,pageNum);
	DispScrollBar(pageQnt,pageNum,pageLast);
	
	//电量低提示
	if(g_GlobalPara.ucFromPageNum == UI_MAIN){
		Get_BatPrcnt();
		if(g_GlobalPara.iBatPercent < 20){
			PromptDialog(DIALOGBATLOW);
			g_GlobalPara.ucFromPageNum = UI_SAMPSET;//假装从主页进入
			LCD_FillRect(56,32,424,288,BKCol);
			SysParaSet_Init(SysParaTable,1);
		}
		if(g_GlobalPara.iBatPercent < 5){
			PromptDialog(DIALOGBATNONE);
			SendUiNo(UI_MAIN);
			g_GlobalPara.ucFromPageNum = UI_SAMPSET;//假装从主页进入
			LCD_FillRect(56,32,424,288,BKCol);
			return;
		}
	}
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_AllButtonReaction(6,menuBtCol,GOLD);
		
		//蓝牙
		if(g_SysPara.iBlueToothSta == 1){
			BT_Lookup();
//			if(g_bBlueTooDispFlag == 1){
//				g_bBlueTooDispFlag = 0;
//				ui_DispBluetooth();
//			}
		}
		
		//按键
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			SendUiNo(UI_MAIN);
			LCD_FillRect(0,0,480,320,BKCol);
			return;
		}
		
		if(pageNum == 1){
			//返回
			if(g_u8TouchResult == 7){
				SendUiNo(UI_MAIN);
				LCD_FillRect(0,0,480,320,BKCol);
				return;
			}
			//关于
			if(g_u8TouchResult == 8){
				LCD_FillRect(56,32,424,288,BKCol);
				About_Fun();
				SysParaSet_Init(SysParaTable,pageNum);;
			}
		}else if(pageNum == 2){
			//返回
			if(g_u8TouchResult == 4){
				SendUiNo(UI_MAIN);
				LCD_FillRect(0,0,480,320,BKCol);
				return;
			}
			//关于
			if(g_u8TouchResult == 5){
				LCD_FillRect(56,32,424,288,BKCol);
				About_Fun();
				SysParaSet_Init(SysParaTable,pageNum);;
			}
		}
		
		
		//选项按钮
		if(g_u8TouchResult < g_u8ButtonNums-2){
			hindex = g_u8TouchResult;
			if(hindex > 6){//背光亮度
				
			}else{
				hindex = g_u8TouchResult;
//				if(hindex < SysParaTable->ucTableLen){
					SysParaOpt(pageNum,hindex,SysParaTable);
//					HighLightID = hindex;//高亮选项
					SaveSysPara();
					InitSysPara();
					if((hindex > 3)||(pageNum != 1)){//前四个按钮不重刷界面
						SysParaSet_Init(SysParaTable,pageNum);
					}
					SysParaOptRes(pageNum,hindex,SysParaTable);
					if((pageNum == 2)&&((hindex == 2)||(hindex == 3))){//重新设置背景色
						//深色背景
						if(g_SysPara.iTheme == THEME_DARK){
							BKCol = BLACK;
						}else{
							BKCol = WHITE;
						}
					}
//				}else{
//					HighLightID = 0xff;//高亮选项
//				}
			}
		}
		//拖动调节背光亮度
		for(i=0;i<3;i++){
			if(g_ButtonBuf[9+i].btSta == BTN_STATUS_DRAG){
				BKLightLast = *SysParaTable[4].ParaAddr;
				BKLight = Get_DragChannel(&g_ButtonBuf[9],3,DRAG_HENG);
				if(BKLight != BKLightLast){
					BKLightDisp(256,128,(u8)BKLight);
					SetBkLedPercent(25+25*BKLight);
					*SysParaTable[4].ParaAddr = BKLight;
					SaveSysPara();
				}
			}
		}
		if(g_u8TouchResult == SLIDE_END_UP){//上划
			
			if(pageQnt > 1){
				pageLast = pageNum;
				pageNum += 1;
				if(pageNum > pageQnt)pageNum = 1;
				//显示参数列表
				LCD_FillRect(56,32,424,288,BKCol);
				BrushPageInit(pageNum);//刷新页面列表显示
				DispScrollBar(pageQnt,pageNum,pageLast);
			}
		}
		if(g_u8TouchResult == SLIDE_END_DOWN){//下划
			
			if(pageQnt > 1){
				pageLast = pageNum;
				pageNum -= 1;
				if(pageNum < 1) pageNum = pageQnt;
				//显示参数列表
				LCD_FillRect(56,32,424,288,BKCol);
				BrushPageInit(pageNum);//刷新页面列表显示
				DispScrollBar(pageQnt,pageNum,pageLast);
			}
		}
		if(g_u8TouchResult == SLIDE_END_LEFT){//左划

		}
		if(g_u8TouchResult == SLIDE_END_RIGHT){//右划
			
		}
	}
}

//系统参数页面初始化
void SysParaSet_Init(SYS_PARA_TABLE_STRUCT* SysParaTable,u8 pageNum)
{
	u16 siBkCol;
	u16 siBarCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		siBkCol  = BLACK;
		siBarCol = GRAY;
	}else{
		siBkCol  = WHITE;
		siBarCol = DGRAY;
	}
	
	//页面左侧菜单表
	MENU_TABLE_STRUCT SyssetMenu[2] = {
		{0,2,"徊",NULL},
		{1,2,"于","关于"}
	};
	//菜单显示
	ui_DispMenuBtn(SyssetMenu,0);
	
	//状态栏
	LCD_DrawRect(56,0,424,32,siBkCol);
	LCD_FillRect(57,1,422,31,siBarCol);
	
//	LCD_SetColor(siBarCol,WHITE);
//	LCD_ShowChinese(Font16,238,9,0,"系统设置");
	ui_DispTitleBar("系统设置",0);
	
	BrushPageInit(pageNum);//刷新页面列表显示
}
//刷新页面列表显示
void BrushPageInit(u8 page)
{
	u16 BkCol;
	u16 TxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		BkCol = BLACK;
		TxCol = WHITE;
	}else{
		BkCol = WHITE;
		TxCol = BLACK;
	}
	
	if(page == 1){
		//显示参数名称
		LCD_SetColor(BkCol,TxCol);
		LCD_ShowChinese(Font16,128,48, 0,"蓝牙上传");
		LCD_ShowChinese(Font16,282,48, 0,"触摸音效");
		LCD_ShowChinese(Font16,128,88, 0,"水平激光");
		LCD_ShowChinese(Font16,282,88, 0,"垂直激光");
		LCD_ShowChinese(Font16,128,128,0,"背光亮度");
		LCD_ShowChinese(Font16,128,176,0,"语言选择");
		LCD_ShowChinese(Font16,128,226,0,"系统日期");
		LCD_ShowChinese(Font16,128,276,0,"系统时间");
		//显示参数
		RadioButtonDisp(216,48,*(u8*)SysParaTable[0].ParaAddr);
		RadioButtonDisp(370,48,*(u8*)SysParaTable[1].ParaAddr);
		RadioButtonDisp(216,88,*(u8*)SysParaTable[2].ParaAddr);
		RadioButtonDisp(370,88,*(u8*)SysParaTable[3].ParaAddr);
		BKLightDisp(256,128,*(u8*)SysParaTable[4].ParaAddr);
		DialogParaDisp(256,166,&SysParaTable[5]);
		DialogParaDisp(256,216,&SysParaTable[6]);
		DialogParaDisp(256,266,&SysParaTable[7]);
		
		//按钮初始化
		g_u8ButtonNums = 12;
		struct BUTTON SampBtn[12] = {
			//sX,  sY,  eX,  eY,
			{208,  40, 259,  71, 0,0},//0, 蓝牙上传
			{362,  40, 413,  71, 0,0},//1, 触摸音效
			{208,  80, 259, 111, 0,0},//2, 水平激光
			{362,  80, 413, 111, 0,0},//3, 垂直激光
			{256, 166, 405, 201, 0,0},//4, 语言选择
			{256, 216, 405, 251, 0,0},//5, 系统日期
			{256, 266, 405, 301, 0,0},//6, 系统时间
			
			{  0,   0,  59,  49, 0,0},//7, 返回
			{  0,  50,  59, 103, 0,0},//8, 关于
			
			{256, 120, 305, 151, 0,BTN_ABILITY_DRAG},//9, 背光亮度
			{306, 120, 355, 151, 0,BTN_ABILITY_DRAG},//10, 背光亮度
			{356, 120, 405, 151, 0,BTN_ABILITY_DRAG} //11, 背光亮度
			
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}else if(page == 2){
		//显示参数名称
		LCD_SetColor(BkCol,TxCol);
		LCD_ShowChinese(Font16,128,56,0,"操作密码");
		LCD_ShowChinese(Font16,128,106,0,"关机时间");
		LCD_ShowChinese(Font16,128,156,0,"界面外观");
		//参数显示
		DialogParaDisp(256,46,&SysParaTable[8]);
		DialogParaDisp(256,96,&SysParaTable[9]);
		DoubleButtonDisp(256,146,&SysParaTable[10]);
		
		//按钮初始化
		g_u8ButtonNums = 6;
		struct BUTTON SampBtn[6] = {
			//sX,  sY,  eX,  eY,
			{256,  46, 405,  81, 0,0},//0, 参数选项1
			{256,  96, 405, 131, 0,0},//1, 参数选项2
			{256, 146, 325, 181, 0,0},//2, 参数选项3
			{336, 146, 405, 181, 0,0},//3, 参数选项4
			{  0,   0,  59,  49, 0,0},//4, 返回
			{  0,  50,  59, 103, 0,0} //5, 关于
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}
}

//单选按钮
void RadioButtonDisp(u16 x,u16 y,u8 value)
{
	u16 barCol;
	u16 noCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		barCol = WHITE;
		noCol = DGRAY;
	}else{
		barCol = DGRAY;
		noCol = LGRAY;
	}
	
	if(value == 0){
		LCD_DrawRect(x,y,36,16,GRAY);
		LCD_FillRect(x+1,y+1,17,14,barCol);
		LCD_FillRect(x+18,y+1,17,14,noCol);
	}else{
		LCD_DrawRect(x,y,36,16,GOLD);
		LCD_FillRect(x+1,y+1,17,14,GOLD);
		LCD_FillRect(x+18,y+1,17,14,barCol);
	}
}

//双按钮
void DoubleButtonDisp(u16 x,u16 y,SYS_PARA_TABLE_STRUCT* ParaTableArr)
{
	u16 dBkCol;
	u16 dTxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		dBkCol = DGRAY;
		dTxCol = WHITE;
	}else{
		dBkCol = LGRAY;
		dTxCol = BLACK;
	}
	u8 value;
	value = *ParaTableArr[0].ParaAddr;
	if(value == 0){
		LCD_SetColor(dBkCol,dTxCol);
		LCD_ShowText_Center(Font16,x,y,70,36,0,(char*)ParaTableArr[0].pList[0].listStr);
		LCD_DrawRect(x,y,70,36,GOLD);
		LCD_DrawRect(x+1,y+1,70-2,36-2,GOLD);
		LCD_SetColor(dBkCol,dTxCol);
		LCD_ShowText_Center(Font16,x+80,y,70,36,0,(char*)ParaTableArr[0].pList[1].listStr);
	}else{
		LCD_SetColor(dBkCol,dTxCol);
		LCD_ShowText_Center(Font16,x,y,70,36,0,(char*)ParaTableArr[0].pList[0].listStr);
		LCD_ShowText_Center(Font16,x+80,y,70,36,0,(char*)ParaTableArr[0].pList[1].listStr);
		LCD_DrawRect(x+80,y,70,36,GOLD);
		LCD_DrawRect(x+80+1,y+1,70-2,36-2,GOLD);
	}
}

//背光亮度显示
void BKLightDisp(u16 x,u16 y,u8 bkVal)
{
	u16 lBkCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		lBkCol = DGRAY;
	}else{
		lBkCol = LGRAY;
	}
	
//	if(bkVal < 0)bkVal = 0;
	if(bkVal > 2)bkVal = 2;
	if(bkVal == 0){
		LCD_FillRect(x,y,48,16,GOLD);
		LCD_FillRect(x+50,y,50,16,lBkCol);
		LCD_FillRect(x+100+2,y,48,16,lBkCol);
	}else if(bkVal == 1){
		LCD_FillRect(x,y,48,16,GOLD);
		LCD_FillRect(x+50,y,50,16,GOLD);
		LCD_FillRect(x+100+2,y,48,16,lBkCol);
	}else if(bkVal == 2){
		LCD_FillRect(x,y,48,16,GOLD);
		LCD_FillRect(x+50,y,50,16,GOLD);
		LCD_FillRect(x+100+2,y,48,16,GOLD);
	}
}
//需要弹窗设置的参数显示
void DialogParaDisp(u16 x,u16 y,SYS_PARA_TABLE_STRUCT* ParaTableArr)
{
	u8 i;
	u16 paraX,paraY;
	char dispBuf[16];
	
	u16 opBkCol;
	u16 opTxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		opBkCol = DGRAY;
		opTxCol = WHITE;
	}else{
		opBkCol = LGRAY;
		opTxCol = BLACK;
	}
	
	paraX = x + 14;
	paraY = y + 10;
	
	LCD_FillRect(x,y,150,36,opBkCol);
	LCD_SetColor(opBkCol,opTxCol);
	LCD_ShowText(Font16,388,paraY,0,">");
	
	memset(dispBuf,0,sizeof(dispBuf));
	memset(dispBuf,' ',10);
	dispBuf[10] = '\0';
	if(ParaTableArr[0].ucParaType == TYP_VALUE_DATA){
		if(*ParaTableArr[0].ParaAddr == 0)sprintf(dispBuf,"--  ");
		else {
			if(ParaTableArr[0].Unit != NULL){//关机时间
				sprintf(dispBuf,"%d min",*ParaTableArr[0].ParaAddr);
			}else sprintf(dispBuf,"%d",*ParaTableArr[0].ParaAddr);
		}
		LCD_ShowText(Font16,paraX,paraY,0,(u8*)dispBuf);
	}else if(ParaTableArr[0].ucParaType == TYP_LIST_HANZI){
		sprintf(dispBuf,"%s",ParaTableArr[0].pList[*ParaTableArr[0].ParaAddr].listStr);
		LCD_ShowText(Font16,paraX,paraY,0,(u8*)dispBuf);
	}else if(ParaTableArr[0].ucParaType == TYP_LIST_ASIC){
		sprintf(dispBuf,"%s",ParaTableArr[0].pList[*ParaTableArr[0].ParaAddr].listStr);
		LCD_ShowText(Font16,paraX,paraY,0,(u8*)dispBuf);
	}else if(ParaTableArr[0].ucParaType == TYP_VALUE_DATE){//日期
		sprintf(dispBuf,"%4d-%02d-%02d",g_SysTime.year,g_SysTime.month,g_SysTime.day);
		LCD_ShowText(Font16,paraX,paraY,0,(u8*)dispBuf);
	}else if(ParaTableArr[0].ucParaType == TYP_VALUE_TIME){//时间
		sprintf(dispBuf,"%d:%02d:%02d",g_SysTime.hour,g_SysTime.min,g_SysTime.sec);
		LCD_ShowText(Font16,paraX,paraY,0,(u8*)dispBuf);
	}else if(ParaTableArr[0].ucParaType == TYP_VALUE_PASSWORD){			
		LCD_ShowString(Font16,paraX,paraY,0,dispBuf);
		for(i=0;i<4;i++){
			LCD_ShowString(Font16,paraX+i*12,paraY+2,0,"-");
		}
	}
}
//参数设置
//ParaTableArr:参数表地址
void SysParaOpt(u8 pageNum,u8 optNum,SYS_PARA_TABLE_STRUCT* ParaTableArr)
{
	int password;
	u16 tBkCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		tBkCol = BLACK;
	}else{
		tBkCol = WHITE;
	}
	
	if(pageNum < 1)pageNum = 1;
	if(pageNum > 2)pageNum = 2;
	if(pageNum == 1){//第一页
		if(optNum <= 3){//前四个参数
			*ParaTableArr[optNum].ParaAddr = !(*ParaTableArr[optNum].ParaAddr);
			if(optNum == 0)RadioButtonDisp(216,48,*ParaTableArr[0].ParaAddr);
			if(optNum == 1)RadioButtonDisp(370,48,*ParaTableArr[1].ParaAddr);
			if(optNum == 2)RadioButtonDisp(216,88,*ParaTableArr[2].ParaAddr);
			if(optNum == 3)RadioButtonDisp(370,88,*ParaTableArr[3].ParaAddr);
		}else if(optNum <= 6){
			optNum += 1;
			ui_DispTitleBar("系统设置",1);
			if(optNum == 5){//语言选择
				SelectDialog(&ParaTableArr[optNum],ParaTableArr[optNum].iParaListLen);
			}
			if(optNum == 6){//系统日期
				TimeSetDialog(0);
			}
			if(optNum == 7){//系统时间
				TimeSetDialog(1);
			}
		}
	}else if(pageNum == 2){//第二页
		if(optNum <= 3){//前四个参数
			optNum += 8;//定位参数表内序号
			if(optNum == 8){//操作密码
				ui_DispTitleBar("系统设置",1);
				password = NumSetDialog(&ParaTableArr[optNum]);
				if(password == 6221){//螺纹钢小量程
					LCD_FillRect(0,0,480,320,tBkCol);
					ConfigFiZero(0);
				}else if(password == 6222){//螺纹钢大量程
					LCD_FillRect(0,0,480,320,tBkCol);
					ConfigFiZero(1);
				}else if(password == 6231){//圆钢小量程
					ConfigFiZero(2);
				}else if(password == 6232){//圆钢大量程
					ConfigFiZero(3);
				}else if(password == 6241){
					
				}else if(password == 6242){
					
				}else if(password == 6240){
					debug = 1;
				}else if(password == 9191){
					sprintf((char*)SysParaTable[11].pList->listStr,"%s",g_SysPara.EquipmentNum);
					TextSetDialog(&SysParaTable[11]);
					sprintf(g_SysPara.EquipmentNum,"%s",(char*)SysParaTable[11].pList->listStr);
				}else if(password == 1444){
					g_SysPara.sID = 0;
//					SaveSysPara();
//					InitSysPara();
				}else if(password == 3568){//更新字库
					update_font(0,0,ASC16x8);
				}else if(password == 3569){//更新字库
					update_unioem(0,0,ASC16x8);
				}else if(password == 2833){//蓝牙恢复出厂设置
					BT_SetDefault();
				}
			}
			if(optNum == 9){//关机时间
				ui_DispTitleBar("系统设置",0);
				NumSetDialog(&ParaTableArr[optNum]);
			}
			if(optNum == 10){//深色
				if(*ParaTableArr[10].ParaAddr != THEME_DARK){
					*ParaTableArr[10].ParaAddr = THEME_DARK;
					LCD_FillRect(0,0,480,320,BLACK);
				}
			}
			if(optNum == 11){//浅色
				if(*ParaTableArr[10].ParaAddr != THEME_LIGHT){
					*ParaTableArr[10].ParaAddr = THEME_LIGHT;
					LCD_FillRect(0,0,480,320,WHITE);
				}
			}
		}
	}else return;
}
//参数设置
void SysParaOptRes(u8 pageNum,u8 optNum,SYS_PARA_TABLE_STRUCT* ParaTableArr)
{
	u16 orBkCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		orBkCol = GRAY;
	}else{
		orBkCol = DGRAY;
	}
	if(pageNum == 1){//第一页
		if(optNum <= 3){//前四个参数
			if(optNum == 0){
				if(*ParaTableArr[optNum].ParaAddr == 0){
					BT_PowerOff();//关蓝牙电源
					LCD_SetColor(orBkCol,orBkCol);
					LCD_ShowChinese(Font20,414,6,0,"伢");
				}else{
					BT_PowerOn();//开蓝牙电源
					LCD_SetColor(orBkCol,WHITE);
					LCD_ShowChinese(Font20,414,6,0,"伢");
				}
			}else if(optNum == 1){
				 
			}else if(optNum == 2){
				if((*ParaTableArr[optNum].ParaAddr) == 0){
					LaserRowOff();
				}else{
					LaserRowOn();
				}
			}else if(optNum == 3){
				if((*ParaTableArr[optNum].ParaAddr) == 0){
					LaserColOff();
				}else{
					LaserColOn();
				}
			}
		}else if(optNum <= 6){
			optNum += 1;
			if(optNum == 4){//语言选择
				
			}
			if(optNum == 5){//系统日期
				
			}
			if(optNum == 6){//系统时间
				
			}
		}
	}else if(pageNum == 2){//第二页
		
	}
}

void ConfigFiZero_Init(u8 mode)
{
	u8 i,j;
	u8 btnNum;
	
	float dateTemp;
	char dataBuf[8];
	
	u16 ziBkCol;
	u16 ziTxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		ziBkCol = BLACK;
		ziTxCol = WHITE;
	}else{
		ziBkCol = WHITE;
		ziTxCol = BLACK;
	}
	
	//分割线
	for(i=0;i<8;i++){
		if(i == 0){
			LCD_FillRect(0,40+i*35-1,480,2,GRAY);//横
		}else {
			LCD_FillRect(0,40+i*35,480,1,GRAY);//横
		}
	}
	for(i=0;i<5;i++){
		if(i == 2){
			LCD_FillRect(80+i*80-1,40,2,285,GRAY);//纵
		}else {
			LCD_FillRect(80+i*80,40,1,285,GRAY);//纵
		}
	}
	
	LCD_SetColor(ziBkCol,ziTxCol);
	LCD_ShowChinese(Font32,8,2,0,"迴");//返回图标
	
	if(mode == 0){//螺纹钢、小量程
		LCD_SetColor(ziBkCol,ziTxCol);
		LCD_ShowChinese(Font20,70,10,0,"螺纹钢崆小量程");//参数名称
		paraFi_p = g_SysPara.iFact_LWSmall_Fi;
		paraZero_p = g_SysPara.iFact_LWSmall_Zero;
	}else if(mode == 1){//螺纹钢、大量程
		LCD_SetColor(ziBkCol,ziTxCol);
		LCD_ShowChinese(Font20,70,10,0,"螺纹钢崆大量程");//参数名称
		paraFi_p = g_SysPara.iFact_LWBig_Fi;
		paraZero_p = g_SysPara.iFact_LWBig_Zero;
	}else if(mode == 2){//圆钢、小量程
		LCD_SetColor(ziBkCol,ziTxCol);
		LCD_ShowChinese(Font20,70,10,0,"圆钢崆小量程");//参数名称
		paraFi_p = g_SysPara.iFact_YGSmall_Fi;
		paraZero_p = g_SysPara.iFact_YGSmall_Zero;
	}else if(mode == 3){//圆钢、大量程
		LCD_SetColor(ziBkCol,ziTxCol);
		LCD_ShowChinese(Font20,70,10,0,"圆钢崆大量程");//参数名称
		paraFi_p = g_SysPara.iFact_YGBig_Fi;
		paraZero_p = g_SysPara.iFact_YGBig_Zero;
	}
	LCD_SetColor(ziBkCol,ziTxCol);
	LCD_ShowString(Font20,240,10,0,"Fi:");
	LCD_ShowString(Font20,360,10,0,"Zero:");
	LCD_SetColor(ziBkCol,ziTxCol);
	LCD_ShowChinese(Font20,20,48,0,"直径");//参数名称
	LCD_ShowChinese(Font20,100,48,0,"满度");//参数名称
	LCD_ShowChinese(Font20,180,48,0,"零点");//参数名称
	LCD_ShowChinese(Font20,260,48,0,"直径");//参数名称
	LCD_ShowChinese(Font20,340,48,0,"满度");//参数名称
	LCD_ShowChinese(Font20,420,48,0,"零点");//参数名称
	
	//满度
	dateTemp = paraFi_p[0]/1000.0f;
	sprintf(dataBuf,"%.3f",dateTemp);
	LCD_SetColor(ziBkCol,ORANGE);
	LCD_ShowString(Font20,285,10,0,dataBuf);
	//零点
	dateTemp = paraZero_p[0]/10.0f;
	sprintf(dataBuf,"%.1f",dateTemp);
	LCD_SetColor(ziBkCol,ORANGE);
	LCD_ShowString(Font20,425,10,0,dataBuf);
	
	for(i=0;i<2;i++){
		for(j=0;j<7;j++){
			sprintf(dataBuf,"D%02d",Rebar_Table_Diam[i*7+j]);
			LCD_SetColor(ziBkCol,ziTxCol);
			LCD_ShowString(Font20,24+i*240,83+j*35,0,dataBuf);
			//满度
			dateTemp = paraFi_p[i*7+j+1]/1000.0f;
			sprintf(dataBuf,"%.3f",dateTemp);
			LCD_SetColor(ziBkCol,ORANGE);
			LCD_ShowString(Font20,95+i*240,83+j*35,0,dataBuf);
			//零点
			dateTemp = paraZero_p[i*7+j+1]/10.0f;
			sprintf(dataBuf,"%.1f",dateTemp);
			LCD_SetColor(ziBkCol,ORANGE);
			LCD_ShowString(Font20,185+i*240,83+j*35,0,dataBuf);
		}
	}
		
	//按钮初始化
	g_u8ButtonNums = 31;
	struct BUTTON SampBtn[31];
	//满度
	SampBtn[0].sX = 270;
	SampBtn[0].sY = 0;
	SampBtn[0].eX = 349;
	SampBtn[0].eY = 39;
	SampBtn[0].btSta = 0;
	SampBtn[0].btAbility = 0;
	//零点
	SampBtn[1].sX = 400;
	SampBtn[1].sY = 0;
	SampBtn[1].eX = 479;
	SampBtn[1].eY = 39;
	SampBtn[1].btSta = 0;
	SampBtn[1].btAbility = 0;
	for(i=0;i<2;i++){
		for(j=0;j<7;j++){
			btnNum = (i*7+j)*2+2;
			//满度
			SampBtn[btnNum].sX = 80+i*240;
			SampBtn[btnNum].sY = 75+j*35;
			SampBtn[btnNum].eX = 80+i*240+79;
			SampBtn[btnNum].eY = 75+j*35+34;
			SampBtn[btnNum].btSta = 0;
			SampBtn[btnNum].btAbility = 0;
			//零点
			SampBtn[btnNum+1].sX = 160+i*240;
			SampBtn[btnNum+1].sY = 75+j*35;
			SampBtn[btnNum+1].eX = 160+i*240+79;
			SampBtn[btnNum+1].eY = 75+j*35+34;
			SampBtn[btnNum+1].btSta = 0;
			SampBtn[btnNum+1].btAbility = 0;
		}
	}
	//返回
	SampBtn[30].sX = 0;
	SampBtn[30].sY = 0;
	SampBtn[30].eX = 79;
	SampBtn[30].eY = 59;
	SampBtn[30].btSta = 0;
	SampBtn[30].btAbility = 0;
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
}
void ConfigFiZero(u8 mode)
{
	float dateTemp;
	u8 index;
	
	u16 zBkCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		zBkCol = BLACK;
	}else{
		zBkCol = WHITE;
	}
	
	ConfigFiZero_Init(mode);	
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_OneButtonReactionXY(30,0,0,47,36,zBkCol,GOLD);//返回
		
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			LCD_FillRect(0,0,480,320,zBkCol);
			return;
		}
		//切换页面
		if(g_u16KeyValBuf == KEY_FN){

		}
		//参数
		if(g_u8TouchResult < 30){
			index = g_u8TouchResult;
			if(index%2 == 0){//满度
				dateTemp = paraFi_p[index/2]/1000.0f;
				dateTemp = fNumInputDialog(dateTemp,3);
				if(dateTemp > 9.999)dateTemp = 9.999;
				paraFi_p[index/2] = (int)(dateTemp*1000.0f);
			}else{//零点
				dateTemp = paraZero_p[index/2]/10.0f;
				dateTemp = fNumInputDialog(dateTemp,1);
				if(dateTemp > 9.9)dateTemp = 9.9;
				paraZero_p[index/2] = (int)(dateTemp*10.0f);
			}		
			ConfigFiZero_Init(mode);
			
		}
		//返回
		if(g_u8TouchResult == 30){
//			SendUiNo(UI_MAIN);
			LCD_FillRect(0,0,480,320,zBkCol);
			return;
		}
	}
}

//关于本机
void About_Fun(void)
{
	u8 i;
	u8 locaSta = 0;
	u32 locaCnt = 0;
	char dBuf[10];
	
	u16 aBkCol;
	u16 aTxCol;
	u16 aBarCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		aBkCol = BLACK;
		aTxCol = WHITE;
		aBarCol = GRAY;
	}else{
		aBkCol = WHITE;
		aTxCol = BLACK;
		aBarCol = DGRAY;
	}
	
	LCD_SetColor(aBarCol,WHITE);
	LCD_ShowChinese(Font16,237,8,0,"关于本机");
	
	//分割线
	for(i=0;i<6;i++){
		LCD_FillRect(70,75+i*40,395,1,GRAY);//横
	}
	//剩余容量
	g_GlobalPara.iStorageFreePrcnt = Get_FatFreePrcnt();
	
	LCD_SetColor(aBkCol,aTxCol);
	LCD_ShowText(Font16,100,48,0,"软件版本:");
	LCD_ShowString(Font16,250,48,0,g_SysPara.SoftVersion);
	LCD_ShowText(Font16,100,88,0,"硬件版本:");
	LCD_ShowString(Font16,250,88,0,g_SysPara.HardVersion);
	LCD_ShowText(Font16,100,128,0,"仪器编号:");
	LCD_ShowString(Font16,250,128,0,g_SysPara.EquipmentNum);
	LCD_ShowText(Font16,100,168,0,"蓝牙名称:");
	LCD_ShowString(Font16,250,168,0,g_SysPara.BlueToothNum);
	LCD_ShowText(Font16,100,208,0,"剩余容量:");
	sprintf(dBuf,"%d%%",g_GlobalPara.iStorageFreePrcnt);
	LCD_ShowString(Font16,250,208,0,dBuf);
	LCD_ShowText(Font16,100,248,0,"定位:");
	if(GPS_Info.status == 'A'){
		sprintf(dBuf,"%8.4f",GPS_Info.lat);
		LCD_ShowString(Font16,250,248,0,dBuf);
		sprintf(dBuf,"%8.4f",GPS_Info.lon);
		LCD_ShowString(Font16,250+72,248,0,dBuf);
	}else{
		LCD_ShowText(Font16,250,248,0,"点击开始定位");
	}
	LCD_ShowText(Font16,174,288,0,"北京海创高科科技有限公司");
	
	//页面左侧菜单表
	MENU_TABLE_STRUCT PreviewMenu[1] = {
		{0,1,"徊",NULL}
	};
	//菜单显示
	ui_DispMenuBtn(PreviewMenu,0);
	//按钮初始化
	g_u8ButtonNums = 2;
	struct BUTTON abBtn[2] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0},//0, 返回
		{250, 235, 450, 275, 0,0} //1, 定位
	};
	memcpy(g_ButtonBuf,abBtn,sizeof(abBtn));
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		
		//按键
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			SendUiNo(UI_PARASET10);
			LCD_FillRect(56,32,424,288,aBkCol);
			return;
		}
		
		//返回
		if(g_u8TouchResult == 0){
			SendUiNo(UI_PARASET10);
			LCD_FillRect(56,32,424,288,aBkCol);
			ClearTouchSta();
			return;
		}
		//定位
		if(g_u8TouchResult == 1){
			if(GPS_Info.status != 'A'){
				LCD_FillRect(250,248,200,16,aBkCol);//清空定位显示
				LCD_SetColor(aBkCol,aTxCol);
				LCD_ShowText(Font16,100,248,0,"定位中");
				GPS_ManualStart();//开始手动定位
				locaSta = 1;
				i = 0;
			}
		}
		if(locaSta == 1){
			GPS_Location_Data();//GPS检测
			if(GPS_Info.status == 'A'){//定位成功
				LCD_SetColor(aBkCol,aTxCol);
				LCD_ShowText(Font16,100,248,0,"定位成功");
				locaSta = 2;
			}
			
			locaCnt++;
			if(locaCnt%100 == 0){
				if(i >= 6) {
					LCD_SetColor(aBkCol,aTxCol);
					LCD_ShowString(Font16,250,248,0,"      ");
					i = 0;
				} else {
					LCD_SetColor(aBkCol,aTxCol);
					LCD_ShowString(Font16,250,248,0,".");
					i++;
				}
			}
		}
		Sleep_ms(5);
	}
}
