/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：厚度测量
    1. ui_Sample10 界面初始化，
    2. ui_Sample10_Run 函数实现

更新记录：
    1. 2018.03.01 初次创建
    
*******************************************************************************/
#include "ui_Sampset.h"
#include "Config_ui.h"

static u8 HighLightID;

static SYS_PARA_TABLE_STRUCT* SampSetTable;
static SYS_PARA_TABLE_STRUCT* HighTable;

void SampParaSet_Init(u8 pageNum);
void SampParaOptions(SYS_PARA_TABLE_STRUCT_P ParaTableArr,u8 pageCap,u8 pageNum);
void HighParaSet_Fun(void);

void ui_Sampset_InitDisp(void)
{
	HighLightID = 0xff;//无高亮
	SampParaSet_Init(1);
}

void ui_Sampset_Run(void)
{
	u8 index;
	u8 pageNum = 1;//第一页
	u8 pageLast = 1;//上一页
	u8 pageCap = 5;//页容量
	u8 pageQnt;	   //页数
	
	u16 bkCol;
	u16 menuBtCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
		menuBtCol = DGRAY;
	}else{
		bkCol = WHITE;
		menuBtCol = LGRAY;
	}
	
//	SYS_PARA_TABLE_STRUCT* sampTable = &ParaTable[PARATABLEINDEX_SAMPLE];
	
	if(SampSetTable[0].ucTableLen%pageCap == 0)pageQnt = SampSetTable[0].ucTableLen/pageCap;
	else pageQnt = SampSetTable[0].ucTableLen/pageCap + 1;
	
	DispScrollBar(pageQnt,pageNum,pageLast);
	
	while(1)
	{
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_OneButtonReaction(5,menuBtCol,GOLD);//返回
		ui_OneButtonReaction(6,menuBtCol,GOLD);//高级
		ui_OneButtonReaction(7,menuBtCol,GOLD);//默认值
		
//		ui_DispTime(0);//时间
//		ui_DispBattery(0);//电量
		
		if(g_u16KeyValBuf == KEY_OK){

		}
		if(g_u16KeyValBuf == KEY_BACK){
			SendUiNo(g_GlobalPara.ucFromPageNum);
			g_GlobalPara.ucFromPageNum = UI_SAMPSET;
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//切换页面
		if(g_u16KeyValBuf == KEY_FN){
			Touch_Beep_On();
			SendUiNo(UI_SAMPLE10);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		
		//返回
		if(g_u8TouchResult == 5){
			SendUiNo(g_GlobalPara.ucFromPageNum);
			g_GlobalPara.ucFromPageNum = UI_SAMPSET;
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		if(g_GlobalPara.ucFromPageNum == UI_SAMPLE10){
			//高级设置
			if(g_u8TouchResult == 6){
				Sleep_ms(50);
				LCD_FillRect(56,32,424,288,bkCol);
				HighParaSet_Fun();
				SampParaSet_Init(pageNum);
			}
			//默认值
			if(g_u8TouchResult == 7){
				ParaTableDefault(SampSetTable);
				SampParaSet_Init(pageNum);
			}
		}else if(g_GlobalPara.ucFromPageNum == UI_SAMPJGJ){
			//默认值
			if(g_u8TouchResult == 6){
				ParaTableDefault(SampSetTable);
				SampParaSet_Init(pageNum);
			}
		}
		else if(g_GlobalPara.ucFromPageNum == UI_SAMPBLOCK){
			//默认值
			if(g_u8TouchResult == 6){
				ParaTableDefault(SampSetTable);
				SampParaSet_Init(pageNum);
			}
			//标定
			if(g_u8TouchResult == 7){
				SendUiNo(UI_BIAODING);
				g_GlobalPara.ucFromPageNum = UI_SAMPBLOCK;
				LCD_FillRect(56,0,424,320,bkCol);
				return;
			}
		}
		else if(g_GlobalPara.ucFromPageNum == UI_VERIFY){
			
		}

		//选项按钮
		if(g_u8TouchResult < pageCap){
			index = (pageNum-1)*pageCap + g_u8TouchResult;
			if(index < SampSetTable[0].ucTableLen){
				ui_DispTitleBar("检测参数设置",1);
				HighLightID = index;//高亮选项
				if(SampSetTable[index].ucParaType == TYP_VALUE_DATA){//数据输入
					NumSetDialog(&SampSetTable[index]);
				}else if((SampSetTable[index].ucParaType == TYP_LIST_HANZI)||(SampSetTable[index].ucParaType == TYP_LIST_ASIC)){//数据选择
					SelectDialog(&SampSetTable[index],SampSetTable[index].iParaListLen);
				}else if(SampSetTable[index].ucParaType == TYP_VALUE_ASIC){//文本输入
					TextSetDialog(&SampSetTable[index]);
					//保存用户设置的构件名
					sprintf(g_SysPara.GouJianName,"%s",SampSetTable[index].pList->listStr);
				}
				SaveSysPara();
				SampParaSet_Init(pageNum);
			}
		}
		
		if(g_u8TouchResult == SLIDE_END_UP){//上划
			if(pageQnt > 1){
				pageLast = pageNum;
				pageNum += 1;
				if(pageNum > pageQnt)pageNum = 1;
				//显示参数列表
				SampParaOptions(SampSetTable,pageCap,pageNum);
				DispScrollBar(pageQnt,pageNum,pageLast);
			}
		}
		if(g_u8TouchResult == SLIDE_END_DOWN){//下划
			if(pageQnt > 1){
				pageLast = pageNum;
				pageNum -= 1;
				if(pageNum < 1) pageNum = pageQnt;
				//显示参数列表
				SampParaOptions(SampSetTable,pageCap,pageNum);
				DispScrollBar(pageQnt,pageNum,pageLast);
			}
		}
		
		Sleep_ms(5);
	}
}

//参数设置首页初始化
void SampParaSet_Init(u8 pageNum)
{
	//深色背景
//	if(g_SysPara.iTheme == THEME_DARK){
//		//状态栏
//		LCD_FillRect(57,1,422,31,GRAY);
//		LCD_SetColor(GRAY,WHITE);
//		LCD_ShowChinese(Font16,222,9,0,"检测参数设置");
//	}else{
//		//状态栏
//		LCD_FillRect(57,1,422,31,DGRAY);
//		LCD_SetColor(DGRAY,WHITE);
//		LCD_ShowChinese(Font16,222,9,0,"检测参数设置");
//	}
	ui_DispTitleBar("检测参数设置",0);
	
	//页面左侧菜单表
	if(g_GlobalPara.ucFromPageNum == UI_SAMPLE10){
		MENU_TABLE_STRUCT ParaMenu[3] = {
			{0,3,"徊",NULL},
			{1,3,"级","高级"},
			{2,3,"默","默认值"}
		};
		//菜单显示
		ui_DispMenuBtn(ParaMenu,0);
		
		//显示参数设置选项
		SampSetTable = &ParaTable[PARATABLEINDEX_SAMPLE];
		SampParaOptions(SampSetTable,5,pageNum);
		
		//按钮初始化
		g_u8ButtonNums = 8;
		struct BUTTON tempBtn[8] = {
			//sX,  sY,  eX,  eY,
			{256,  56, 405,  91, 0,0},//0, 参数选项1
			{256, 106, 405, 141, 0,0},//1, 参数选项2
			{256, 156, 405, 191, 0,0},//2, 参数选项3
			{256, 206, 405, 241, 0,0},//3, 参数选项4
			{256, 256, 405, 291, 0,0},//4, 参数选项5
			
			{  0,   0,  59,  49, 0,0},//5, 返回
			{  0,  50,  59, 103, 0,0},//6, 高级
			{  0, 104,  59, 157, 0,0} //7, 默认值
		};
		memcpy(g_ButtonBuf,tempBtn,sizeof(tempBtn));
	}else if(g_GlobalPara.ucFromPageNum == UI_SAMPJGJ){
		MENU_TABLE_STRUCT ParaMenu[2] = {
			{0,2,"徊",NULL},
			{1,2,"默","默认值"}
		};
		//菜单显示
		ui_DispMenuBtn(ParaMenu,0);
		
		//显示参数设置选项
		SampSetTable = &ParaTable[PARATABLEINDEX_JGJ];
		SampParaOptions(SampSetTable,5,pageNum);
	
		//按钮初始化
		g_u8ButtonNums = 7;
		struct BUTTON tempBtn[7] = {
			//sX,  sY,  eX,  eY,
			{256,  56, 405,  91, 0,0},//0, 参数选项1
			{256, 106, 405, 141, 0,0},//1, 参数选项2
			{256, 156, 405, 191, 0,0},//2, 参数选项3
			{256, 206, 405, 241, 0,0},//3, 参数选项4
			{256, 256, 405, 291, 0,0},//4, 参数选项5
			
			{  0,   0,  59,  49, 0,0},//5, 返回
			{  0,  50,  59, 103, 0,0} //6  默认值
		};
		memcpy(g_ButtonBuf,tempBtn,sizeof(tempBtn));
	}else if(g_GlobalPara.ucFromPageNum == UI_SAMPBLOCK){
		MENU_TABLE_STRUCT ParaMenu[3] = {
			{0,3,"徊",NULL},
			{1,3,"默","默认值"},
			{2,3,"标","标定"}
		};
		//菜单显示
		ui_DispMenuBtn(ParaMenu,0);
		
		//显示参数设置选项
		SampSetTable = &ParaTable[PARATABLEINDEX_BLOCK];
		SampParaOptions(SampSetTable,5,pageNum);
	
		//按钮初始化
		g_u8ButtonNums = 8;
		struct BUTTON tempBtn[8] = {
			//sX,  sY,  eX,  eY,
			{256,  56, 405,  91, 0,0},//0, 参数选项1
			{256, 106, 405, 141, 0,0},//1, 参数选项2
			{256, 156, 405, 191, 0,0},//2, 参数选项3
			{256, 206, 405, 241, 0,0},//3, 参数选项4
			{256, 256, 405, 291, 0,0},//4, 参数选项5
			
			{  0,   0,  59,  49, 0,0},//5, 返回
			{  0,  50,  59, 103, 0,0},//6  默认值
			{  0, 104,  59, 157, 0,0} //7  标定
		};
		memcpy(g_ButtonBuf,tempBtn,sizeof(tempBtn));
	}else if(g_GlobalPara.ucFromPageNum == UI_VERIFY){
		MENU_TABLE_STRUCT ParaMenu[1] = {
			{0,1,"徊",NULL}
		};
		//菜单显示
		ui_DispMenuBtn(ParaMenu,0);
		
		//显示参数设置选项
		SampSetTable = &ParaTable[PARATABLEINDEX_VERIFY];
		SampParaOptions(SampSetTable,3,pageNum);
		
		//按钮初始化
		g_u8ButtonNums = 6;
		struct BUTTON tempBtn[6] = {
			//sX,  sY,  eX,  eY,
			{256,  56, 405,  91, 0,0},//0, 参数选项1
			{256, 106, 405, 141, 0,0},//1, 参数选项2
			{256, 156, 405, 191, 0,0},//2, 参数选项3
			{256, 206, 405, 241, 0,0},//3, 参数选项4
			{256, 256, 405, 291, 0,0},//4, 参数选项5
			
			{  0,   0,  59,  49, 0,0},//5, 返回
		};
		memcpy(g_ButtonBuf,tempBtn,sizeof(tempBtn));
	}

//	ui_DispTime(1);
//	ui_DispBattery(1);//显示电量

//	NumSetDialog(&ParaTable[1]);
//	TimeSetDialog();
//	SelectDialog(2);
	
	ClearTouchSta();
}
//高级设置
void HighParaSet_Init(SYS_PARA_TABLE_STRUCT* highParaTable,u8 pageNum)
{
	//深色背景
//	if(g_SysPara.iTheme == THEME_DARK){
//		//状态栏
//		LCD_FillRect(57,1,422,31,GRAY);
//		LCD_SetColor(GRAY,WHITE);
//		LCD_ShowChinese(Font16,222,9,0,"高级参数设置");
//	}else{
//		//状态栏
//		LCD_FillRect(57,1,422,31,DGRAY);
//		LCD_SetColor(DGRAY,WHITE);
//		LCD_ShowChinese(Font16,222,9,0,"高级参数设置");
//	}
	ui_DispTitleBar("高级参数设置",0);
	
	//页面左侧菜单表
	MENU_TABLE_STRUCT HighParaMenu[2] = {
		{0,2,"徊",NULL},
		{1,2,"默","默认值"}
	};
	//菜单显示
	ui_DispMenuBtn(HighParaMenu,0);
	
	//显示参数列表
	SampParaOptions(highParaTable,5,pageNum);
	
	//按钮初始化
	g_u8ButtonNums = 8;
	struct BUTTON SampBtn[8] = {
		//sX,  sY,  eX,  eY,		
		{256,  56, 405,  91, 0,0},//0, 参数选项1
		{256, 106, 405, 141, 0,0},//1, 参数选项2
		{256, 156, 405, 191, 0,0},//2, 参数选项3
		{256, 206, 405, 241, 0,0},//3, 参数选项4
		{256, 256, 405, 291, 0,0},//4, 参数选项5
		
		{  0,   0,  59,  49, 0,0},//5, 返回
		{  0,  50,  59, 103, 0,0} //6  默认值
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));	
}

void HighParaSet_Fun(void)
{
//	u8 i;
	u8 hindex;
	u8 pageNum = 1;//第一页
	u8 pageLast = 1;//上一页
	u8 pageCap = 5;//页容量
	u8 pageQnt;	   //页数
	
	u16 bkCol;
	u16 menuBtCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
		menuBtCol = DGRAY;
	}else{
		bkCol = WHITE;
		menuBtCol = LGRAY;
	}
	
	HighTable = &ParaTable[PARATABLEINDEX_SAMPLEHIGH];
	
	if(HighTable[0].ucTableLen%pageCap == 0)pageQnt = HighTable[0].ucTableLen/pageCap;
	else pageQnt = HighTable[0].ucTableLen/pageCap + 1;
	
	HighLightID = 0xff;//无高亮
	HighParaSet_Init(HighTable,pageNum);
	DispScrollBar(pageQnt,pageNum,pageLast);
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_OneButtonReaction(5,menuBtCol,GOLD);//返回
		ui_OneButtonReaction(6,menuBtCol,GOLD);//默认值
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			if(g_SysPara.g_SamplePara.iScreenWide == 0){
				g_GlobalPara.sDispScreenWide = 800;
			}else{
				g_GlobalPara.sDispScreenWide = 1200;
			}
			
			SendUiNo(UI_SAMPSET);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
//		//切换页面
//		if(g_u16KeyValBuf == KEY_FN){
//			Touch_Beep_On();
//			SendUiNo(UI_SAMPLE10);
//			LCD_FillRect(0,0,480,320,WHITE);
//			return;
//		}
		
		//返回
		if(g_u8TouchResult == 5){
			if(g_SysPara.g_SamplePara.iScreenWide == 0){
				g_GlobalPara.sDispScreenWide = 800;
			}else{
				g_GlobalPara.sDispScreenWide = 1200;
			}
			
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//返回
		if(g_u8TouchResult == 6){//恢复默认值
			ParaTableDefault(HighTable);
			HighParaSet_Init(HighTable,pageNum);
		}
		
		//选项按钮
		if(g_u8TouchResult < pageCap){
			
			hindex = (pageNum-1)*pageCap + g_u8TouchResult;
			if(hindex < HighTable->ucTableLen){
				ui_DispTitleBar("高级参数设置",1);
				if(HighTable[hindex].ucParaType == TYP_VALUE_DATA){//数据输入
					NumSetDialog(&HighTable[hindex]);
				}else if((HighTable[hindex].ucParaType == TYP_LIST_HANZI)||(HighTable[hindex].ucParaType == TYP_LIST_ASIC)){//数据选择
					SelectDialog(&HighTable[hindex],HighTable[hindex].iParaListLen);
				}else if(HighTable[hindex].ucParaType == TYP_VALUE_ASIC){//文本输入
					TextSetDialog(&HighTable[hindex]);
				}
				HighLightID = hindex;//高亮选项
				HighParaSet_Init(HighTable,pageNum);
			}else{
				HighLightID = 0xff;//高亮选项
			}
		}		

		if(g_u8TouchResult == SLIDE_END_UP){//上划
			
			if(pageQnt > 1){
				pageLast = pageNum;
				pageNum += 1;
				if(pageNum > pageQnt)pageNum = 1;
				//显示参数列表
				SampParaOptions(HighTable,pageCap,pageNum);
				DispScrollBar(pageQnt,pageNum,pageLast);
			}
		}
		if(g_u8TouchResult == SLIDE_END_DOWN){//下划
			
			if(pageQnt > 1){
				pageLast = pageNum;
				pageNum -= 1;
				if(pageNum < 1) pageNum = pageQnt;
				//显示参数列表
				SampParaOptions(HighTable,pageCap,pageNum);
				DispScrollBar(pageQnt,pageNum,pageLast);
			}
		}
		if(g_u8TouchResult == SLIDE_END_LEFT){//左划
			
//			i+=1;
//			if(i>6)i=2;
//			LCD_FillRect(60,40,360,260,WHITE);//弹窗
//			SelectDialog(&ParaTable[4],i);
		}
		if(g_u8TouchResult == SLIDE_END_RIGHT){//右划
			
//			i-=1;
//			if(i<2)i=6;
//			LCD_FillRect(60,40,360,260,WHITE);//弹窗
//			SelectDialog(&ParaTable[4],i);
		}
	}
}

//ParaTableArr:表地址，pageCap:一页显示数量，pageNum:页码
void SampParaOptions(SYS_PARA_TABLE_STRUCT* ParaTableArr,u8 pageCap,u8 pageNum)
{
	u8 i,n;
	u8 pageQuant;
	u8 optNum;
	u8 tIndex;
	u8 sLen;
	char dispBuf[16];
	
	u16 nameX = 128;
	u16 paraX = 256;
	
	u16 bkCol;
	u16 txCol;
	u16 paraBk;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
		txCol = WHITE;
		paraBk = GRAY;
	}else{
		bkCol = WHITE;
		txCol = BLACK;
		paraBk = LGRAY;
	}
	
	if(ParaTableArr[0].ucTableLen%pageCap == 0){
		pageQuant = ParaTableArr[0].ucTableLen/pageCap;
	}else pageQuant = ParaTableArr[0].ucTableLen/pageCap + 1;
	if(pageNum < 1)pageNum = 1;
	if(pageNum > pageQuant)return;
	optNum = (pageNum-1)*pageCap;
	//显示列表
	for(i=0; i<pageCap; i++){
		tIndex = i+optNum;
		if((i+optNum) > (ParaTableArr[optNum].ucTableLen-1)){
			LCD_FillRect(nameX,56+10+i*50,64+8,16,bkCol);//清除参数名
			LCD_FillRect(paraX,56+i*50,150,36,bkCol);//清除参数
			continue;
		}
		LCD_FillRect(paraX,56+i*50,150,36,paraBk);//清除当前参数
		sprintf(dispBuf,"%s ",ParaTableArr[i+optNum].sChName);
		LCD_SetColor(bkCol,txCol);
		LCD_ShowText(Font16,nameX,56+10+i*50,0,(u8*)dispBuf);//参数名称
		if(tIndex  == HighLightID)LCD_SetColor(paraBk,RED);
		else LCD_SetColor(paraBk,txCol);
		
		memset(dispBuf,0,sizeof(dispBuf));
		memset(dispBuf,' ',10);//参数区域10个字符
		dispBuf[10] = '\0';
		if(ParaTableArr[i+optNum].ucParaType == TYP_VALUE_ASIC){//构件名称
//			Show_Text(Font16,paraX+14,56+10+i*50,0,ParaTableArr[i+optNum].pList[0].listStr);
			ui_DispGJName(Font16,paraX+14,56+10+i*50,txCol,(char*)ParaTableArr[i+optNum].pList[0].listStr);
		}else if(ParaTableArr[i+optNum].ucParaType == TYP_VALUE_DATA){
			if(*ParaTableArr[i+optNum].ParaAddr == 0){
				if(ParaTableArr[i+optNum].Unit != NULL){
					sprintf(dispBuf,"-- %s",ParaTableArr[i+optNum].Unit);
				}else{
					sprintf(dispBuf,"-- ");
				}
			}else{
				if(ParaTableArr[i+optNum].Unit != NULL){
					sprintf(dispBuf,"%d %s",*ParaTableArr[i+optNum].ParaAddr,ParaTableArr[i+optNum].Unit);
				}else{
					sprintf(dispBuf,"%d ",*ParaTableArr[i+optNum].ParaAddr);
				}
			}
			sLen = strlen(dispBuf);
			for(n=sLen;n<10;n++){
				dispBuf[n] = ' ';
			}
			dispBuf[10] = '\0';
			LCD_ShowText(Font16,paraX+14,56+10+i*50,0,(u8*)dispBuf);
		}else if(ParaTableArr[i+optNum].ucParaType == TYP_LIST_HANZI){
			sLen = strlen((char*)ParaTableArr[i+optNum].pList[*ParaTableArr[i+optNum].ParaAddr].listStr);
			memcpy(dispBuf,(char*)ParaTableArr[i+optNum].pList[*ParaTableArr[i+optNum].ParaAddr].listStr,sLen);
			for(n=sLen;n<10;n++){
				dispBuf[n] = ' ';
			}
			dispBuf[10] = '\0';
			LCD_ShowText(Font16,paraX+14,56+10+i*50,0,(u8*)dispBuf);
		}
		else if(ParaTableArr[i+optNum].ucParaType == TYP_LIST_ASIC){
			sprintf(dispBuf,"%s ",ParaTableArr[i+optNum].pList[*ParaTableArr[i+optNum].ParaAddr].listStr);
			sLen = strlen(dispBuf);
			for(n=sLen;n<10;n++){
				dispBuf[n] = ' ';
			}
			dispBuf[10] = '\0';
			LCD_ShowText(Font16,paraX+14,56+10+i*50,0,(u8*)dispBuf);
		}
		LCD_ShowText(Font20,paraX+130,56+8+i*50,0,">");
	}
}
//显示滚动条:pageQnt:总页数，pageNum:页码1,2,3...
void DispScrollBar(short pageQnt,short pageNum,short pageLast)
{
	//滚动条区域：x=460~479,y=34~319;
	//滚动条长度：270
	//滚动条宽度：w=12
	unsigned short i;
	unsigned short wid = 8;
	unsigned short higt;
	float len = 280.0f/pageQnt;
	
	u16 barBkCol;
	u16 barCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		barBkCol = DDGRAY;
		barCol = GRAY;
	}else{
		barBkCol = LLGRAY;
		barCol = GRAY;
	}
	
	higt = (unsigned short)len+1;
	if(higt < 4)higt = 4;
	if(pageQnt > 1){
//		LCD_FillRect(470,36+(pageLast-1)*len,wid,higt,barBkCol);
//		LCD_FillRect(470,36+(pageNum-1)*len,wid,higt,barCol);
		for(i=0;i<pageQnt;i++){
			if(i == pageNum-1)
				LCD_FillRect(470,36+i*len,wid,higt,barCol);
			else LCD_FillRect(470,36+i*len,wid,higt,barBkCol);
		}
	}
}

