/******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：数据浏览
    1. ui_Browse10 界面初始化
    2. ui_Browse10_Run 函数实现
   
更新记录：
    1. 2018.03.01 初次创建
    
*******************************************************************************/
#include "ui_Browse10.h"
#include "Config_ui.h"

u8 g_ucTurnGouJianSta = 0;//0:无效,LAST:上一页,NEXT:下一页

//文件 参数
static short PageQnt;
static short PageCap = 6;
static short PageNum;//1,2...
static short PageLast;//1,2...
static short FileNum;//0,1...
//static short FileNumLast;//0,1...

static short HighLightNum;
static short HighLightNumLast;
FileListStruct HighLightFile;

static u8 SelectAllFlag;

typedef struct SelFileNode{
	char fileName[40];
	short filepage;
	short fileNum;//文件每页内序号
	struct SelFileNode *next;
}SelFileNode;

SelFileNode *SelectFileList;

extern SYS_PARA_TABLE_STRUCT* JGJParaTable;
extern u8 JGJData[12][9];//数据存储

extern SYS_PARA_TABLE_STRUCT* BlockParaTable;

void DispFileList(short page,short fnum);
void DispFileHighLight(short fpage,short fnum);
void ReadFileHead(u8 n,char* dir);
u8 ReadFileData(u8 ft,char* dir);
void JGJDataViewFun(void);
void BlockPreviewBro_Fun(void);

SelFileNode *CreateSelLink(void);
SelFileNode *DeleteSelLink(SelFileNode *link);
SelFileNode *DeleteSelFile(SelFileNode *link);
void SelectFileOpt(SelFileNode *link,char* fname,short fpage,short fnum);
void SelectFileRefresh(SelFileNode *link,short page);

void ui_Browse10_InitDisp(void)
{
	u8 i;
	char dbuf[8];
	
	u16 bkCol;
	u16 txCol;
	u16 barCol;
	u16 linCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
		txCol = WHITE;
		barCol = GRAY;
		linCol = DGRAY;
	}else{
		bkCol = WHITE;
		txCol = BLACK;
		barCol = DGRAY;
		linCol = LGRAY;
	}
	
	if(g_ucTurnGouJianSta == 0){
		
		//页面左侧菜单表
		MENU_TABLE_STRUCT Samp10Menu[5] = {
			{0,5,"徊",NULL},
			{1,5,"上","上页"},
			{2,5,"下","下页"},
			{3,5,"览","浏览"},
			{4,5,"除","删除"}
		};
		//菜单显示
		ui_DispMenuBtn(Samp10Menu,0);
		
		//状态栏
		LCD_DrawRect(56,0,424,32,bkCol);
		LCD_FillRect(57,1,422,31,barCol);
		
		LCD_SetColor(barCol,WHITE);
		LCD_ShowChinese(Font16,222,9,0,"数据浏览");
		sprintf(dbuf,"(%d)",g_GlobalPara.iSaveFileCnt);
		LCD_ShowString(Font16,286,9,0,dbuf);
		
		//分割线
		for(i=0;i<6;i++){
			LCD_FillRect(66,75+i*40,399,1,linCol);//横
		}
		
		LCD_SetColor(bkCol,txCol);
		LCD_DrawRect(70,50,11,11,barCol);//选中框
		LCD_ShowChinese(Font16,90,47,0,"构件名称");
		LCD_ShowChinese(Font16,184,47,0,"模式");
		LCD_ShowChinese(Font16,244,47,0,"直径");
		LCD_ShowChinese(Font16,300,47,0,"测点数");
		LCD_ShowChinese(Font16,380,47,0,"日期");
		
	//	ui_DispBattery(1);//显示电量
        
		//按钮初始化
		g_u8ButtonNums = 24;
		struct BUTTON SampBtn[24] = {
			//sX,  sY,  eX,  eY,
			{  0,   0,  59,  49, 0,0},//0, 返回
			{  0,  50,  59, 103, 0,0},//1, 上页
			{  0, 104,  59, 157, 0,0},//2, 下页
			{  0, 158,  59, 211, 0,0},//3, 浏览
			{  0, 212,  59, 265, 0,0},//4, 删除
			
			{ 60,  32, 109,  75, 0,0},//5, 全选
			{ 60,  76, 109, 115, 0,0},//6, 构件1
			{ 60, 116, 109, 155, 0,0},//7, 构件2
			{ 60, 156, 109, 195, 0,0},//8, 构件3
			{ 60, 196, 109, 235, 0,0},//9, 构件4
			{ 60, 236, 109, 275, 0,0},//10, 构件5
			{ 60, 276, 109, 319, 0,0},//11, 构件6
			
			{110,  76, 419, 115, 0,0},//12, 构件1
			{110, 116, 419, 155, 0,0},//13, 构件2
			{110, 156, 419, 195, 0,0},//14, 构件3
			{110, 196, 419, 235, 0,0},//15, 构件4
			{110, 236, 419, 275, 0,0},//16, 构件5
			{110, 276, 419, 319, 0,0},//17, 构件6
			
			{420,  76, 479, 115, 0,0},//18, 箭头1
			{420, 116, 479, 155, 0,0},//19, 箭头2
			{420, 156, 479, 195, 0,0},//20, 箭头3
			{420, 196, 479, 235, 0,0},//21, 箭头4
			{420, 236, 479, 275, 0,0},//22, 箭头5
			{420, 276, 479, 319, 0,0} //23, 箭头5
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
		
	//	//扫描文件
	//	g_GlobalPara.iSaveFileCnt = Scan_FileIndex(FileDirectory);
		
		SelectAllFlag = 0;
		
		SelectFileList = CreateSelLink();
		
		if(g_GlobalPara.ucFromPageNum == UI_MAIN){//从分析界面返回
			//扫描文件
			g_GlobalPara.iSaveFileCnt = Scan_FileIndex(FileDirectory);
			if(g_GlobalPara.iSaveFileCnt%PageCap == 0){
				PageQnt = g_GlobalPara.iSaveFileCnt/PageCap;
			}else{
				PageQnt = g_GlobalPara.iSaveFileCnt/PageCap+1;
			}
			PageNum = 1;
			PageLast = 1;
			HighLightNum = 0;
			HighLightNumLast = HighLightNum;
			
			//扫描文件
			Scan_OnePageFiles(FileDirectory,PageNum,g_GlobalPara.iSaveFileCnt);
		}
		DispFileList(PageNum,HighLightNum);
		
		DispScrollBar(PageQnt,PageNum,PageLast);
		
		//构件个数
		sprintf(dbuf,"(%d)",g_GlobalPara.iSaveFileCnt);
		LCD_SetColor(barCol,WHITE);
		LCD_ShowString(Font16,286,9,0,dbuf);
	}
	
	ClearTouchSta();//清除触摸
}

void ui_Browse10_Run(void)
{
	u8 i,tcNum;
	char filNamebuf[40];
	
	u16 bkCol;
	u16 highCol;
	u16 menuBtCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
		highCol = DGRAY;
		menuBtCol = DGRAY;
	}else{
		bkCol = WHITE;
		highCol = LGRAY;
		menuBtCol = LGRAY;
	}
	
	//
	if(g_ucTurnGouJianSta != 0){
		if(g_ucTurnGouJianSta == LAST){
			FileNum -= 1;
		}
		if(g_ucTurnGouJianSta == NEXT){
			FileNum += 1;
		}
		
		if(FileNum >= g_GlobalPara.iSaveFileCnt){
			FileNum = 0;
		}
		if(FileNum < 0){
			FileNum = g_GlobalPara.iSaveFileCnt-1;
		}  
		PageNum = FileNum/PageCap+1;
		tcNum = FileNum%PageCap;
		HighLightNum = FileNum;
		if(PageNum != PageLast){
			PageLast = PageNum;
			Scan_OnePageFiles(FileDirectory,PageNum,g_GlobalPara.iSaveFileCnt);
		}
		
		sprintf(filNamebuf,"0:/GY91/%s",FileList[tcNum].NameStr);
		memcpy(&HighLightFile,&FileList[tcNum],sizeof(HighLightFile));//记录当前构件信息
		//读文件头
		ReadFileHead(tcNum,filNamebuf);
		if(FileNum < g_GlobalPara.iSaveFileCnt){
			SelectFileList = DeleteSelLink(SelectFileList);
			if(FileList[tcNum].fileType == QUICK){//快速检测
				g_GlobalPara.iCurFileNum = FileNum;
				
				ReadFileData(QUICK,filNamebuf);
				
				SendUiNo(UI_ANALYZE);
				LCD_FillRect(56,32,424,288,bkCol);
				g_GlobalPara.ucFromPageNum = UI_BROWSE10;
				return;
			}else if(FileList[tcNum].fileType == JGJ){//JGJ检测
				ReadFileData(JGJ,filNamebuf);
				
				LCD_FillRect(56,32,424,288,bkCol);
				JGJDataViewFun();
				SendUiNo(UI_BROWSE10);
				LCD_FillRect(56,32,424,288,bkCol);
				g_GlobalPara.ucFromPageNum = UI_BROWSE10;
				return;
//				g_GlobalPara.ucFromPageNum = UI_BROWSE10;
//				ui_Browse10_InitDisp();
			}else if(FileList[tcNum].fileType == BLOCK){//区块检测
				ReadFileData(BLOCK,filNamebuf);
				
				LCD_FillRect(56,32,424,288,bkCol);
//				BlockPreview_Init();
				BlockPreviewBro_Fun();
				SendUiNo(UI_BROWSE10);
				LCD_FillRect(56,32,424,288,bkCol);
				g_GlobalPara.ucFromPageNum = UI_BROWSE10;
				return;
//				g_GlobalPara.ucFromPageNum = UI_BROWSE10;
//				ui_Browse10_InitDisp();
			}
		}
	}
	
	//电量低提示
	if(g_GlobalPara.ucFromPageNum == UI_MAIN){
		Get_BatPrcnt();
		if(g_GlobalPara.iBatPercent < 20){
			PromptDialog(DIALOGBATLOW);
			g_GlobalPara.ucFromPageNum = UI_BROWSE10;//假装从主页进入
			LCD_FillRect(56,32,424,288,bkCol);
			ui_Browse10_InitDisp();
		}
		if(g_GlobalPara.iBatPercent < 5){
			PromptDialog(DIALOGBATNONE);
			SendUiNo(UI_MAIN);
			g_GlobalPara.ucFromPageNum = UI_BROWSE10;//假装从主页进入
			LCD_FillRect(0,0,480,320,bkCol);
			return;
		}
	}
	
	while(1)
	{
		//按键
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_AllButtonReaction(5,menuBtCol,GOLD);
//		ui_DispTime(0);//时间
//		ui_DispBattery(0);//电量
		
		if(g_u16KeyValBuf == KEY_OK){

		}
		if(g_u16KeyValBuf == KEY_BACK){
			SelectFileList = DeleteSelLink(SelectFileList);
			SendUiNo(UI_MAIN);
			LCD_FillRect(0,0,480,320,bkCol);
			return;
		}
		//标定
		if(g_u16KeyValBuf == KEY_FN){
			
		}
		
		if(g_u16KeyValBuf == LONG_KEY_FN){
			
		}
		
		if(g_u8TouchResult == 0){//返回主页
			SelectFileList = DeleteSelLink(SelectFileList);
			SendUiNo(UI_MAIN);
			LCD_FillRect(0,0,480,320,bkCol);
			return;
		}
		//翻页
		if((g_u8TouchResult == 1)||(g_u8TouchResult == SLIDE_END_DOWN)){//上页
			PageLast = PageNum;
			if(PageQnt > 1){
				PageNum -= 1;
				if(PageNum < 1)PageNum = PageQnt;
				Scan_OnePageFiles(FileDirectory,PageNum,g_GlobalPara.iSaveFileCnt);
				DispFileList(PageNum,HighLightNum);
				DispScrollBar(PageQnt,PageNum,PageLast);
				
				SelectFileRefresh(SelectFileList,PageNum);
			}
		}
		if((g_u8TouchResult == 2)||(g_u8TouchResult == SLIDE_END_UP)){//下页
			PageLast = PageNum;
			if(PageQnt > 1){
				PageNum += 1;
				if(PageNum > PageQnt)PageNum = 1;
				Scan_OnePageFiles(FileDirectory,PageNum,g_GlobalPara.iSaveFileCnt);
				DispFileList(PageNum,HighLightNum);
				DispScrollBar(PageQnt,PageNum,PageLast);
				
				SelectFileRefresh(SelectFileList,PageNum);
			}
		}
		if(g_u8TouchResult == 3){//浏览
			tcNum = HighLightNum%PageCap;
			FileNum = HighLightNum;
//			sprintf(filNamebuf,"0:/GY91/%s",FileList[tcNum].NameStr);
			sprintf(filNamebuf,"0:/GY91/%s",HighLightFile.NameStr);//选中构件
			f_open(file,filNamebuf,FA_READ);
//			f_lseek(file, sizeof(FlieDataHead));
			f_read(file, &FlieDataHead, sizeof(FlieDataHead), &br);//读取文件头
			f_close(file);
			if(FileNum < g_GlobalPara.iSaveFileCnt){
				SelectFileList = DeleteSelLink(SelectFileList);
				if(HighLightFile.fileType == QUICK){//快速检测
					g_GlobalPara.iCurFileNum = FileNum;
					
					ReadFileData(QUICK,filNamebuf);
					SendUiNo(UI_ANALYZE);
					LCD_FillRect(56,32,424,288,bkCol);
					g_GlobalPara.ucFromPageNum = UI_BROWSE10;
					return;
				}else if(HighLightFile.fileType == JGJ){//JGJ检测
					ReadFileData(JGJ,filNamebuf);
					LCD_FillRect(56,32,424,288,bkCol);
					JGJDataViewFun();
					SendUiNo(UI_BROWSE10);
					LCD_FillRect(56,32,424,288,bkCol);
					g_GlobalPara.ucFromPageNum = UI_BROWSE10;
					return;
//					ui_Browse10_InitDisp();
				}else if(HighLightFile.fileType == BLOCK){//区块检测
					ReadFileData(BLOCK,filNamebuf);
					LCD_FillRect(56,32,424,288,bkCol);
//					BlockPreviewBro_Init();
					BlockPreviewBro_Fun();
					SendUiNo(UI_BROWSE10);
					LCD_FillRect(56,32,424,288,bkCol);
					g_GlobalPara.ucFromPageNum = UI_BROWSE10;
					return;
//					ui_Browse10_InitDisp();
				}
			}
		}
		if(g_u8TouchResult == 4){//删除
			if(SelectAllFlag == 1){//格式化
				if(PromptDialog(DIALOGDELEALL) == CONFIRM){
					f_mkfs("0:",1,4096);//格式化FLASH,0:盘符; 1,不需要引导区,8个扇区为1个簇
					f_mount(fs[0],"0:",1);//挂载FLASH
					f_mkdir("0:/GY91");
					f_mkdir("0:/BMP");
					f_mkdir("0:/FONT");
				}
				//扫描文件
				g_GlobalPara.iSaveFileCnt = Scan_FileIndex(FileDirectory);
				SendUiNo(UI_BROWSE10);
				LCD_FillRect(56,32,424,288,bkCol);
				g_GlobalPara.ucFromPageNum = UI_MAIN;
				SelectFileList = DeleteSelLink(SelectFileList);
				
				return;
			}else{
				if(SelectFileList->next != NULL){
					if(PromptDialog(DIALOGDELESEL) == CONFIRM){
						SelectFileList = DeleteSelFile(SelectFileList);
					}
					SendUiNo(UI_BROWSE10);
					LCD_FillRect(56,32,424,288,bkCol);
					g_GlobalPara.ucFromPageNum = UI_MAIN;
					SelectFileList = DeleteSelLink(SelectFileList);
					return;
				}
			}
		}
		//全选构件
		if(g_u8TouchResult == 5){
			if(SelectAllFlag == 0){ 
				SelectAllFlag = 1;
				SelectFileList = DeleteSelLink(SelectFileList);//全选后删除链表
				LCD_DrawRect(70,50,11,11,GRAY);//选中框
				LCD_FillRect(71,51,9,9,GOLD);
				for(i=0;i<6;i++){
					if((PageNum-1)*PageCap+i < g_GlobalPara.iSaveFileCnt){
//						if((HighLightNum/5+1 == PageNum)&&(HighLightNum%5 == i)){//高亮

						LCD_DrawRect(70,90+i*40,11,11,GRAY);//选中框
						LCD_FillRect(71,91+i*40,9,9,GOLD);
					}else{
						break;
					}
				}
			}else{
				SelectAllFlag = 0;
				SelectFileList = CreateSelLink();
				LCD_DrawRect(70,50,11,11,GRAY);//选中框
				LCD_FillRect(71,51,9,9,bkCol);
				for(i=0;i<6;i++){
					if((PageNum-1)*PageCap+i < g_GlobalPara.iSaveFileCnt){
						LCD_DrawRect(70,90+i*40,11,11,GRAY);//选中框
						if((HighLightNum/PageCap+1 == PageNum)&&(HighLightNum%PageCap == i)){//高亮
							LCD_FillRect(71,91+i*40,9,9,highCol);
						}else{
							LCD_FillRect(71,91+i*40,9,9,bkCol);
						}
					}else{
						break;
					}
				}
			}
		}
		//勾选构件
		if((g_u8TouchResult >= 6)&&(g_u8TouchResult <= 11)){
			tcNum = g_u8TouchResult-6;
			FileNum = (PageNum-1)*PageCap + tcNum;
			if(FileNum < g_GlobalPara.iSaveFileCnt){
				if(SelectAllFlag == 0){
					SelectFileOpt(SelectFileList,(char*)FileList[tcNum].NameStr,PageNum,tcNum);
				}
			}
		}
		//选中构件
		if((g_u8TouchResult >= 12)&&(g_u8TouchResult <= 17)){
			//选中构件
			HighLightNum = (g_u8TouchResult-12)+(PageNum-1)*PageCap;
			if(HighLightNum < g_GlobalPara.iSaveFileCnt){
				DispFileHighLight(PageNum,HighLightNum);
				if(SelectAllFlag == 0){
					SelectFileRefresh(SelectFileList,PageNum);
				}
			}else{
				HighLightNum = HighLightNumLast;
			}
			//打开构件
			tcNum = g_u8TouchResult-12;
			FileNum = (PageNum-1)*PageCap + tcNum;
			HighLightNum = FileNum;
			sprintf(filNamebuf,"0:/GY91/%s",FileList[tcNum].NameStr);
			memcpy(&HighLightFile,&FileList[tcNum],sizeof(HighLightFile));//记录当前构件信息
			if(FileNum < g_GlobalPara.iSaveFileCnt){
				SelectFileList = DeleteSelLink(SelectFileList);
				if(FileList[tcNum].fileType == QUICK){//快速检测
					g_GlobalPara.iCurFileNum = FileNum;
					
					ReadFileData(QUICK,filNamebuf);
					
					SendUiNo(UI_ANALYZE);
					LCD_FillRect(56,32,424,288,bkCol);
					g_GlobalPara.ucFromPageNum = UI_BROWSE10;
					return;
				}else if(FileList[tcNum].fileType == JGJ){//JGJ检测
					ReadFileData(JGJ,filNamebuf);
					
					LCD_FillRect(56,32,424,288,bkCol);
					JGJDataViewFun();
					SendUiNo(UI_BROWSE10);
					LCD_FillRect(56,32,424,288,bkCol);
					g_GlobalPara.ucFromPageNum = UI_BROWSE10;
					return;
//					g_GlobalPara.ucFromPageNum = UI_BROWSE10;
//					ui_Browse10_InitDisp();
				}else if(FileList[tcNum].fileType == BLOCK){//区块检测
					ReadFileData(BLOCK,filNamebuf);
					
					LCD_FillRect(56,32,424,288,bkCol);
//					BlockPreview_Init();
					BlockPreviewBro_Fun();
					SendUiNo(UI_BROWSE10);
					LCD_FillRect(56,32,424,288,bkCol);
					g_GlobalPara.ucFromPageNum = UI_BROWSE10;
					return;
//					g_GlobalPara.ucFromPageNum = UI_BROWSE10;
//					ui_Browse10_InitDisp();
				}
			}
		}
		//打开构件
		if((g_u8TouchResult >= 18)&&(g_u8TouchResult <= 23)){
			tcNum = g_u8TouchResult-18;
			FileNum = (PageNum-1)*PageCap + tcNum;
			HighLightNum = FileNum;
			sprintf(filNamebuf,"0:/GY91/%s",FileList[tcNum].NameStr);
			memcpy(&HighLightFile,&FileList[tcNum],sizeof(HighLightFile));//记录当前构件信息
			if(FileNum < g_GlobalPara.iSaveFileCnt){
				SelectFileList = DeleteSelLink(SelectFileList);
				if(FileList[tcNum].fileType == QUICK){//快速检测
					g_GlobalPara.iCurFileNum = FileNum;
					
					ReadFileData(QUICK,filNamebuf);
					
					SendUiNo(UI_ANALYZE);
					LCD_FillRect(56,32,424,288,bkCol);
					g_GlobalPara.ucFromPageNum = UI_BROWSE10;
					return;
				}else if(FileList[tcNum].fileType == JGJ){//JGJ检测
					ReadFileData(JGJ,filNamebuf);
					
					LCD_FillRect(56,32,424,288,bkCol);
					JGJDataViewFun();
					SendUiNo(UI_BROWSE10);
					LCD_FillRect(56,32,424,288,bkCol);
					g_GlobalPara.ucFromPageNum = UI_BROWSE10;
					return;
//					g_GlobalPara.ucFromPageNum = UI_BROWSE10;
//					ui_Browse10_InitDisp();
				}else if(FileList[tcNum].fileType == BLOCK){//区块检测
					ReadFileData(BLOCK,filNamebuf);
					
					LCD_FillRect(56,32,424,288,bkCol);
//					BlockPreview_Init();
					BlockPreviewBro_Fun();
					SendUiNo(UI_BROWSE10);
					LCD_FillRect(56,32,424,288,bkCol);
					g_GlobalPara.ucFromPageNum = UI_BROWSE10;
					return;
//					g_GlobalPara.ucFromPageNum = UI_BROWSE10;
//					ui_Browse10_InitDisp();
				}
			}
		}
		
		Sleep_ms(5);
	}
}

//*******************************************************************
//显示文件列表
void DispFileList(short page,short fnum)
{
	u8 i;
	RX8010SJTIME dtime;
	short fNum;
	u16 bkColor;
	char timbuf[16];
	char nambuf[40];
	char dnambuf[16];
	char tempbuf[8];
	char* type[5] = {
		"快速",
		"JGJ",
		"区块",
		"快速",
		"快速"
	};
	
	
	u16 lbkCol;
	u16 ltxCol;
	u16 highCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		lbkCol = BLACK;
		ltxCol = WHITE;
		highCol = DGRAY;
	}else{
		lbkCol = WHITE;
		ltxCol = BLACK;
		highCol = LGRAY;
	}
	
	for(i=0;i<PageCap;i++){
		if(FileList[i].fileEn == 1){
			fNum = (page-1)*PageCap + i;
			if(fNum == fnum){//高亮
				bkColor = highCol;
			}else{
				bkColor = lbkCol;
			}
			
			LCD_FillRect(66,76+i*40,399,39,bkColor);
			
			sprintf(nambuf,"0:/GY91/%s",FileList[i].NameStr);
			f_open(file,nambuf,FA_READ);
			f_read(file, &FlieDataHead, sizeof(FlieDataHead), &br);//读取文件头
			f_close(file);
			FileList[i].RebarDiam = FlieDataHead.ucZhuJinDiam;
			FileList[i].SaveDotCnt = FlieDataHead.iSaveDotCntCh2;
			FileList[i].fileType = FlieDataHead.ucDetectType;
			
			dtime.year = ((FileList[i].FileDate & 0xFE00)>>9)+1980-2000;
			dtime.month = (FileList[i].FileDate & 0x1E0)>>5;
			dtime.day = FileList[i].FileDate & 0x1F;
//			dtime.hour = (FileList[i].FileTime & 0xF800)>>11;
//			dtime.min = (FileList[i].FileTime & 0x7E0)>>5;
//			dtime.sec = FileList[i].FileTime & 0x1F;
			
			sprintf(timbuf,"%02d-%02d-%02d",dtime.year,dtime.month,dtime.day);// %d:%d,dtime.hour,dtime.min
			
			LCD_DrawRect(70,90+i*40,11,11,GRAY);//选中框
			if(SelectAllFlag == 1){//全选
				LCD_FillRect(71,91+i*40,9,9,GOLD);
			}else{
				LCD_FillRect(71,91+i*40,9,9,lbkCol);
			}
			LCD_SetColor(bkColor,ltxCol);
			LCD_ShowChinese(Font16,448,87+i*40,0,"＞");
			GetNameShort(dnambuf,(char*)FileList[i].NameStr);
			Show_Text(Font16,90,87+i*40,0,(u8*)dnambuf);
			LCD_ShowText(Font16,184,87+i*40,0,(u8*)type[FileList[i].fileType]);
			sprintf(tempbuf,"%d",FileList[i].RebarDiam);
			LCD_ShowString(ASC16x8,252,87+i*40,0,tempbuf);
			sprintf(tempbuf,"%d",FileList[i].SaveDotCnt);
			LCD_ShowString(ASC16x8,312,87+i*40,0,tempbuf);
			LCD_ShowText(Font16,364,87+i*40,0,(u8*)timbuf);
		}else{
			LCD_FillRect(66,76+i*40,399,39,lbkCol);
		}
		HighLightNumLast = fnum;
	}
}
//显示高亮文件
//fpage:页码,fnum:文件序号
void DispFileHighLight(short fpage,short fnum)
{
//	FILINFO fno;
	RX8010SJTIME dtime;
	
	u8 high;
	u8 highLast;
	short fpageLast;
	
	char timbuf[16];
	char nambuf[40];
	char dnambuf[16];
	char tempbuf[8];
	char* type[5] = {
		"快速",
		"JGJ",
		"区块",
		"快速",
		"快速"
	};
	
	
	u16 hbkCol;
	u16 htxCol;
	u16 highCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		hbkCol = BLACK;
		htxCol = WHITE;
		highCol = DGRAY;
	}else{
		hbkCol = WHITE;
		htxCol = BLACK;
		highCol = LGRAY;
	}
	
	high = fnum%PageCap;
	highLast = HighLightNumLast%PageCap;
	fpageLast = HighLightNumLast/PageCap+1;
	
	if(fnum != HighLightNumLast){
		if(fpage == fpageLast){//清除上次高亮
			LCD_FillRect(66,76+highLast*40,399,39,hbkCol);//灰色背景
			
			sprintf(nambuf,"0:/GY91/%s",FileList[highLast].NameStr);
			f_open(file,nambuf,FA_READ);
			f_read(file, &FlieDataHead, sizeof(FlieDataHead), &br);//读取文件头
			f_close(file);
			FileList[highLast].SaveDotCnt = FlieDataHead.iSaveDotCntCh2;
			FileList[highLast].RebarDiam = FlieDataHead.ucZhuJinDiam;
			FileList[highLast].fileType = FlieDataHead.ucDetectType;
			
			dtime.year = ((FileList[highLast].FileDate & 0xFE00)>>9)+1980-2000;
			dtime.month = (FileList[highLast].FileDate & 0x1E0)>>5;
			dtime.day = FileList[highLast].FileDate & 0x1F;
//			dtime.hour = (FileList[highLast].FileTime & 0xF800)>>11;
//			dtime.min = (FileList[highLast].FileTime & 0x7E0)>>5;
//			dtime.sec = FileList[highLast].FileTime & 0x1F;
			
			sprintf(timbuf,"%02d-%02d-%02d",dtime.year,dtime.month,dtime.day);// %d:%d,dtime.hour,dtime.min
			
			LCD_DrawRect(70,90+highLast*40,11,11,GRAY);//选中框
			if(SelectAllFlag == 0){
				LCD_FillRect(71,91+highLast*40,9,9,hbkCol);
			}else{
				LCD_FillRect(71,91+highLast*40,9,9,GOLD);
			}
			LCD_SetColor(hbkCol,htxCol);
			LCD_ShowChinese(Font16,448,87+highLast*40,0,"＞");
			GetNameShort(dnambuf,(char*)FileList[highLast].NameStr);
			Show_Text(Font16,90,87+highLast*40,0,(u8*)dnambuf);
			LCD_ShowText(Font16,184,87+highLast*40,0,(u8*)type[FileList[highLast].fileType]);
			sprintf(tempbuf,"%d",FileList[highLast].RebarDiam);
			LCD_ShowString(ASC16x8,252,87+highLast*40,0,tempbuf);
			sprintf(tempbuf,"%d",FileList[highLast].SaveDotCnt);
			LCD_ShowString(ASC16x8,312,87+highLast*40,0,tempbuf);
			LCD_ShowText(Font16,364,87+highLast*40,0,(u8*)timbuf);
		}
		
		LCD_FillRect(66,76+high*40,399,39,highCol);//灰色背景
		
		sprintf(nambuf,"0:/GY91/%s",FileList[high].NameStr);
		f_open(file,nambuf,FA_READ);
		f_read(file, &FlieDataHead, sizeof(FlieDataHead), &br);//读取文件头
		f_close(file);
		FileList[highLast].SaveDotCnt = FlieDataHead.iSaveDotCntCh2;
		FileList[high].RebarDiam = FlieDataHead.ucZhuJinDiam;
		FileList[high].fileType = FlieDataHead.ucDetectType;
		
		dtime.year = ((FileList[high].FileDate & 0xFE00)>>9)+1980-2000;
		dtime.month = (FileList[high].FileDate & 0x1E0)>>5;
		dtime.day = FileList[high].FileDate & 0x1F;
//		dtime.hour = (FileList[high].FileTime & 0xF800)>>11;
//		dtime.min = (FileList[high].FileTime & 0x7E0)>>5;
//		dtime.sec = FileList[high].FileTime & 0x1F;
		
		sprintf(timbuf,"%02d-%02d-%02d",dtime.year,dtime.month,dtime.day);// %d:%d,dtime.hour,dtime.min
		
		LCD_DrawRect(70,90+high*40,11,11,GRAY);//选中框
		if(SelectAllFlag == 0){
			LCD_FillRect(71,91+high*40,9,9,highCol);
		}else{
			LCD_FillRect(71,91+high*40,9,9,GOLD);
		}
		LCD_SetColor(highCol,htxCol);
		LCD_ShowChinese(Font16,448,87+high*40,0,"＞");
		GetNameShort(dnambuf,(char*)FileList[high].NameStr);
		Show_Text(Font16,90,87+high*40,0,(u8*)dnambuf);
		LCD_ShowText(Font16,184,87+high*40,0,(u8*)type[FileList[high].fileType]);
		sprintf(tempbuf,"%d",FileList[high].RebarDiam);
		LCD_ShowString(ASC16x8,252,87+high*40,0,tempbuf);
		sprintf(tempbuf,"%d",FileList[high].SaveDotCnt);
		LCD_ShowString(ASC16x8,312,87+high*40,0,tempbuf);
		LCD_ShowText(Font16,364,87+high*40,0,(u8*)timbuf);
		
		HighLightNumLast = fnum;
	}
}
///////////////////////////////////////////////////////////////////////////////////
//读取文件数据
//ft:文件类型：QUICK/JGJ/BLOCK
void ReadFileHead(u8 n,char* dir)
{
	f_open(file,dir,FA_READ);
	f_read(file, &FlieDataHead, sizeof(FlieDataHead), &br);//读取文件头
	f_close(file);
	FileList[n].RebarDiam = FlieDataHead.ucZhuJinDiam;
	FileList[n].SaveDotCnt = FlieDataHead.iSaveDotCntCh2;
	FileList[n].fileType = FlieDataHead.ucDetectType;
}
//ft:文件类型：QUICK/JGJ/BLOCK
u8 ReadFileData(u8 ft,char* dir)
{
	int foffset = 0;
	char fileCheck[8];
	
	if(ft == QUICK){
		f_open(file,dir,FA_READ);
		f_read(file,&FlieDataHead,sizeof(FlieDataHead),&br);//读取文件头
		ResBuf_CH1.AllRebarCnt = FlieDataHead.iSaveDotCntCh1;
		ResBuf_CH2.AllRebarCnt = FlieDataHead.iSaveDotCntCh2;
		ResBuf_CH3.AllRebarCnt = FlieDataHead.iSaveDotCntCh3;
		foffset = 128;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH1.RebarSite,FlieDataHead.iSaveDotCntCh1*sizeof(unsigned short),&br);//读取通道1钢筋位置
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH1.RebarCover,FlieDataHead.iSaveDotCntCh1*sizeof(unsigned char),&br);//读取通道1钢筋厚度
		foffset += br;
//		f_lseek(file,foffset);
//		f_read(file,&fileCheck,4,&br);//读取校验
//		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH2.RebarSite,FlieDataHead.iSaveDotCntCh2*sizeof(unsigned short),&br);//读取通道2钢筋位置
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH2.RebarCover,FlieDataHead.iSaveDotCntCh2*sizeof(unsigned char),&br);//读取通道2钢筋厚度
		foffset += br;
//		f_lseek(file,foffset);
//		f_read(file,&fileCheck,4,&br);//读取校验
//		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH3.RebarSite,FlieDataHead.iSaveDotCntCh3*sizeof(unsigned short),&br);//读取通道3钢筋位置
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH3.RebarCover,FlieDataHead.iSaveDotCntCh3*sizeof(unsigned char),&br);//读取通道3钢筋厚度
		foffset += br;
//		f_lseek(file,foffset);
//		f_read(file,&fileCheck,4,&br);//读取校验

        g_iCarDist = FlieDataHead.iCarSite;

        f_lseek(file,foffset);
        f_read(file, &SourceData.iCH1, g_iCarDist*sizeof(unsigned short), &bw);//写数据ch1
        foffset += bw;
        
        f_lseek(file,foffset);
        f_read(file, &SourceData.iCH2, g_iCarDist*sizeof(unsigned short), &bw);//写数据ch2
        foffset += bw;
        
        f_lseek(file,foffset);
        f_read(file, &SourceData.iCH3, g_iCarDist*sizeof(unsigned short), &bw);//写数据ch3
        foffset += bw;
        
        f_lseek(file,foffset);
        f_read(file, &SourceData.iCH4, g_iCarDist*sizeof(unsigned short), &bw);//写数据ch4
        foffset += bw;
        
        f_lseek(file,foffset);
        f_read(file, &SourceData.iCH5, g_iCarDist*sizeof(unsigned short), &bw);//写数据ch5
        foffset += bw;
        
        f_lseek(file,foffset);
        f_read(file, &SourceData.iCH6, g_iCarDist*sizeof(unsigned short), &bw);//写数据ch6
        foffset += bw;
    
		f_close(file);
		return 1;
	}
    else if(ft == JGJ){
		f_open(file,dir,FA_READ);
		f_read(file,&FlieDataHead,sizeof(FlieDataHead),&br);//读取文件头
		foffset = 128;
		f_lseek(file,foffset);
		f_read(file,JGJData,FlieDataHead.ucJGJNumber*9,&br);//读取JGJ数据
		f_close(file);
		return 1;
	}
    else if(ft == BLOCK){
		f_open(file,dir,FA_READ);
		f_read(file,&FlieDataHead,sizeof(FlieDataHead),&br);//读取文件头
		ResBuf_CH1.AllRebarCnt = FlieDataHead.iSaveDotCntCh1;
		ResBuf_CH2.AllRebarCnt = FlieDataHead.iSaveDotCntCh2;
		ResBuf_CH3.AllRebarCnt = FlieDataHead.iSaveDotCntCh3;
		foffset = 128;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH1.RebarSite,FlieDataHead.iSaveDotCntCh1*sizeof(unsigned short),&br);//读取通道1钢筋位置
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH1.RebarCover,FlieDataHead.iSaveDotCntCh1*sizeof(unsigned char),&br);//读取通道1钢筋厚度
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH1.BlockRanks,FlieDataHead.iSaveDotCntCh1*sizeof(unsigned char),&br);//读取通道1钢筋位置
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&fileCheck,4,&br);//读取校验
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH2.RebarSite,FlieDataHead.iSaveDotCntCh2*sizeof(unsigned short),&br);//读取通道2钢筋位置
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH2.RebarCover,FlieDataHead.iSaveDotCntCh2*sizeof(unsigned char),&br);//读取通道2钢筋厚度
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH2.BlockRanks,FlieDataHead.iSaveDotCntCh2*sizeof(unsigned char),&br);//读取通道2钢筋位置
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&fileCheck,4,&br);//读取校验
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH3.RebarSite,FlieDataHead.iSaveDotCntCh3*sizeof(unsigned short),&br);//读取通道3钢筋位置
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH3.RebarCover,FlieDataHead.iSaveDotCntCh3*sizeof(unsigned char),&br);//读取通道3钢筋厚度
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&ResBuf_CH3.BlockRanks,FlieDataHead.iSaveDotCntCh3*sizeof(unsigned char),&br);//读取通道3钢筋位置
		foffset += br;
		f_lseek(file,foffset);
		f_read(file,&fileCheck,4,&br);//读取校验
		f_close(file);
		return 1;
	}else return 0;
}
//JGJ/////////////////////////////////////////////////////////////////////////////////
//JGJ统计数据刷新
void JGJDataOrigRefresh(u8 pageQnt,u8 pageNum,u8 saveNum)
{
	u8 i;
	u16 nx,num,snum;//ny,
	u8 curNum;
	u16 color;
	char buf[8];
	
	u16 rBkCol;
	u16 rTxCol;
	u16 rhCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		rBkCol = BLACK;
		rTxCol = WHITE;
		rhCol = DDGRAY;
	}else{
		rBkCol = WHITE;
		rTxCol = BLACK;
		rhCol = LLGRAY;
	}
	
	snum = (pageNum-1)*18;
	for(i=0;i<6;i++){
		nx = i/3;
//		ny = i/2;
		num = i*3;
		curNum = (pageNum-1)*6+i;
		if((i==1)||(i==4)){
			color = rhCol;
			LCD_SetColor(rhCol,rTxCol);
		}else{
			color = rBkCol;
			LCD_SetColor(rBkCol,rTxCol);
		}
		if(curNum < saveNum){
			//序号
			LCD_ShowNums_Center(Font16,61+nx*208,65+((num+0)%9)*28,41,27,0,snum+num+1);
			LCD_ShowNums_Center(Font16,61+nx*208,65+((num+1)%9)*28,41,27,0,snum+num+2);
			LCD_ShowNums_Center(Font16,61+nx*208,65+((num+2)%9)*28,41,27,0,snum+num+3);
			
//			if((JGJData[curNum][2]!=0)||(JGJData[curNum][5]!=0)||(JGJData[curNum][8]!=0)){
				//平均厚度
				LCD_ShowNums_Center(Font16,61+nx*208+42,65+((num+0)%9)*28,41,27,0,JGJData[curNum][2]);
				LCD_ShowNums_Center(Font16,61+nx*208+42,65+((num+1)%9)*28,41,27,0,JGJData[curNum][5]);
				LCD_ShowNums_Center(Font16,61+nx*208+42,65+((num+2)%9)*28,41,27,0,JGJData[curNum][8]);
				//厚度/厚度
				sprintf(buf,"%d/%d",JGJData[curNum][0],JGJData[curNum][1]);
				LCD_ShowText_Center(Font16,61+nx*208+84,65+((num+0)%9)*28,81,27,0,buf);
				sprintf(buf,"%d/%d",JGJData[curNum][3],JGJData[curNum][4]);
				LCD_ShowText_Center(Font16,61+nx*208+84,65+((num+1)%9)*28,81,27,0,buf);
				sprintf(buf,"%d/%d",JGJData[curNum][6],JGJData[curNum][7]);
				LCD_ShowText_Center(Font16,61+nx*208+84,65+((num+2)%9)*28,81,27,0,buf);
				//△H
				LCD_ShowNums_Center(Font16,61+nx*208+166,65+((num+0)%9)*28,41,27,0,abs(JGJData[curNum][0]-JGJData[curNum][1]));
				LCD_ShowNums_Center(Font16,61+nx*208+166,65+((num+1)%9)*28,41,27,0,abs(JGJData[curNum][3]-JGJData[curNum][4]));
				LCD_ShowNums_Center(Font16,61+nx*208+166,65+((num+2)%9)*28,41,27,0,abs(JGJData[curNum][6]-JGJData[curNum][7]));
//			}else{
//				LCD_FillRect(61+nx*210,64+((num+0)%9)*28,40,28,color);
//				LCD_FillRect(61+nx*210,64+((num+1)%9)*28,40,28,color);
//				LCD_FillRect(61+nx*210,64+((num+2)%9)*28,40,28,color);
//				
//				LCD_FillRect(61+nx*210+42,64+((num+0)%9)*28,40,28,color);
//				LCD_FillRect(61+nx*210+42,64+((num+1)%9)*28,40,28,color);
//				LCD_FillRect(61+nx*210+42,64+((num+2)%9)*28,40,28,color);
//				
//				LCD_FillRect(61+nx*210+84,64+((num+0)%9)*28,80,28,color);
//				LCD_FillRect(61+nx*210+84,64+((num+1)%9)*28,80,28,color);
//				LCD_FillRect(61+nx*210+84,64+((num+2)%9)*28,80,28,color);
//				
//				LCD_FillRect(61+nx*210+168,64+((num+0)%9)*28,40,28,color);
//				LCD_FillRect(61+nx*210+168,64+((num+1)%9)*28,40,28,color);
//				LCD_FillRect(61+nx*210+168,64+((num+2)%9)*28,40,28,color);
//			}
		}else{
			LCD_FillRect(61+nx*208,65+((num+0)%9)*28,41,27,color);
			LCD_FillRect(61+nx*208,65+((num+1)%9)*28,41,27,color);
			LCD_FillRect(61+nx*208,65+((num+2)%9)*28,41,27,color);
			
			LCD_FillRect(61+nx*208+42,65+((num+0)%9)*28,41,27,color);
			LCD_FillRect(61+nx*208+42,65+((num+1)%9)*28,41,27,color);
			LCD_FillRect(61+nx*208+42,65+((num+2)%9)*28,41,27,color);
			
			LCD_FillRect(61+nx*208+84,65+((num+0)%9)*28,81,27,color);
			LCD_FillRect(61+nx*208+84,65+((num+1)%9)*28,81,27,color);
			LCD_FillRect(61+nx*208+84,65+((num+2)%9)*28,81,27,color);
			
			LCD_FillRect(61+nx*208+166,65+((num+0)%9)*28,41,27,color);
			LCD_FillRect(61+nx*208+166,65+((num+1)%9)*28,41,27,color);
			LCD_FillRect(61+nx*208+166,65+((num+2)%9)*28,41,27,color);
		}
//		LCD_ShowNnNums(Font16,77+42,70+i*28,1,1,22);
//		LCD_ShowString(Font16,166,70+i*28,1,"21/22");
//		LCD_ShowString(Font16,245,70+i*28,1,"1");
	}
//	for(i=0;i<9;i++){
//		LCD_SetColor(WHITE,BLACK);
////		LCD_ShowNnNums(Font16,77,70+i*28,1,1,i+1);
//		
//		LCD_ShowNnNums(Font16,77+42,70+i*28,1,1,22);
//		LCD_ShowString(Font16,166,70+i*28,1,"21/22");
//		LCD_ShowString(Font16,245,70+i*28,1,"1");
//	}
//	
//	for(i=0;i<9;i++){
//		LCD_SetColor(WHITE,BLACK);
////		LCD_ShowNnNums(Font16,77+210,70+i*28,1,1,i+1);
//		
//		LCD_ShowNnNums(Font16,77+42+210,70+i*28,1,1,22);
//		LCD_ShowString(Font16,166+210,70+i*28,1,"21/22");
//		LCD_ShowString(Font16,245+210,70+i*28,1,"1");
//	}
}
//JGJ统计信息
void JGJDataOrig(void)
{
	u8 i;
	u8 jgjSaveNum;//数据存储个数
	u8 jgjPageQnt;
	u8 jgjPageNum;
	
	u16 oBkCol;
	u16 oTxCol;
	u16 oBarCol;
	u16 ohCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		oBkCol = BLACK;
		oTxCol = WHITE;
		oBarCol = GRAY;
		ohCol = DDGRAY;
	}else{
		oBkCol = WHITE;
		oTxCol = BLACK;
		oBarCol = DGRAY;
		ohCol = LLGRAY;
	}
	
	jgjSaveNum = FlieDataHead.ucJGJNumber;
	if(jgjSaveNum > 6){
		jgjPageQnt = 2;
	}else{
		jgjPageQnt = 1;
	}
	jgjPageNum = 1;
	
	//状态栏
	LCD_FillRect(57,1,422,31,oBarCol);//横
	
	//分割线
//	LCD_FillRect(60,60,420,1,GRAY);//横
	LCD_FillRect(60,148+1,416,84-1,ohCol);//灰色背景
	for(i=0;i<9;i++){
		LCD_FillRect(60,64+i*28,416,1,GRAY);//横
	}
	
	//竖线
//	for(i=1;i<10;i++){
		
		LCD_FillRect(102,36,1,280,GRAY);//竖
		LCD_FillRect(144,64,1,252,GRAY);//竖
		LCD_FillRect(226,36,1,280,GRAY);//竖
		LCD_FillRect(268,36,1,280,GRAY);//竖
		LCD_FillRect(310,36,1,280,GRAY);//竖
		LCD_FillRect(352,64,1,252,GRAY);//竖
		LCD_FillRect(434,36,1,280,GRAY);//竖
		
//		if((i==3)||(i==8)){
//			continue;
//		}
//		if((i==2)||(i==7)){
//			LCD_FillRect(60+i*42,64,1,252,GRAY);//竖
//		}else{
//			LCD_FillRect(60+i*42,36,1,280,GRAY);//竖
//		}
//	}
	
	LCD_SetColor(oBarCol,WHITE);
	LCD_ShowText(Font16,210,9,0,"JGJ检测原始数据");
	LCD_SetColor(oBkCol,oTxCol);
	LCD_ShowText(Font16,72,42,0,"No.");
	LCD_ShowText(Font16,159,42,0,"H");
	LCD_ShowText(Font16,231,42,0,"△H");
	LCD_ShowText(Font16,280 ,42,0,"No.");
	LCD_ShowText(Font16,367,42,0,"H");
	LCD_ShowText(Font16,439,42,0,"△H");
	
	JGJDataOrigRefresh(jgjPageQnt,jgjPageNum,jgjSaveNum);
	
	MENU_TABLE_STRUCT ParaMenu[1] = {
		{0,1,"徊",NULL}
	};
	//菜单显示
	ui_DispMenuBtn(ParaMenu,0);
	
	//按钮初始化
	g_u8ButtonNums = 1;
	struct BUTTON oBtn[1] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0} //0, 返回
	};
	memcpy(g_ButtonBuf,oBtn,sizeof(oBtn));
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		
		//返回
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_BACK)){
			
			LCD_FillRect(56,32,424,288,oBkCol);
			return;
		}
		
		if(jgjPageQnt > 1){
			if(g_u8TouchResult == SLIDE_END_LEFT){//左划
				if(jgjPageQnt > 1){
					jgjPageNum += 1;
					if(jgjPageNum > jgjPageQnt){
						jgjPageNum = 1;
					}
					JGJDataOrigRefresh(jgjPageQnt,jgjPageNum,jgjSaveNum);
				}
			}
			if(g_u8TouchResult == SLIDE_END_RIGHT){//右划
				if(jgjPageQnt > 1){
					jgjPageNum -= 1;
					if(jgjPageNum < 1){
						jgjPageNum = jgjPageQnt;
					}
					JGJDataOrigRefresh(jgjPageQnt,jgjPageNum,jgjSaveNum);
				}
			}
		}
	}
}
//JGJ预览图
void JGJPreView(void)
{
	u8 i,j;
	u8 gouJianType;
	u8 dotCnt;
	u8 dotn;
	short houduDesign;
	short houduSum;
	short houduAvg;
	short houduBig,houduSmall;
	u16 x,y,ypos,yneg,xc;
//	u8 pos,neg;//正负差值
	char tempBuf[16];
	
	u16 pvBkCol;
	u16 pvTxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		pvBkCol = BLACK;
		pvTxCol = WHITE;
	}else{
		pvBkCol = WHITE;
		pvTxCol = BLACK;
	}
	
	gouJianType = FlieDataHead.ucGouJianType;
	houduDesign = FlieDataHead.ucHouDuDesign;
	dotCnt = FlieDataHead.iSaveDotCntCh1;
	
	//设计厚度线
	LCD_FillRect(100,257,360,3,GOLD);//横,中心Y坐标248
	for(i=0;i<12;i++){
		LCD_FillRect(115+i*30-1,253,3,4,GOLD);
	}
	LCD_SetColor(pvBkCol,pvTxCol);
	LCD_ShowNnNums(Font20,65,258-10,0,3,houduDesign);
	if(gouJianType == 0){//梁 正十负七
//		neg = 7;
//		pos = 10;
		yneg = 258-4*7;
		ypos = 258+4*10;
		houduSmall = houduDesign-7;
		houduBig = houduDesign+10;
		LCD_DrawLineDotted(100,yneg,460,yneg,GRAY);//上虚线
		LCD_DrawLineDotted(100,ypos,460,ypos,GRAY);//下虚线
		LCD_SetColor(pvBkCol,GRAY);
		LCD_ShowNnNums(Font16,68,258-4*7-8,0,3,houduSmall);
		LCD_ShowNnNums(Font16,68,258+4*10-8,0,3,houduBig);
	}else{//板 正八负五
//		neg = 5;
//		pos = 8;
		yneg = 258-4*5;
		ypos = 258+4*8;
		houduSmall = houduDesign-5;
		houduBig = houduDesign+8;
		LCD_DrawLineDotted(100,yneg,460,yneg,GRAY);//上虚线
		LCD_DrawLineDotted(100,ypos,460,ypos,GRAY);//下虚线
		LCD_SetColor(pvBkCol,GRAY);
		LCD_ShowNnNums(Font16,68,258-4*5-8,0,3,houduSmall);
		LCD_ShowNnNums(Font16,68,258+4*8-8,0,3,houduBig);
	}
	for(i=0;i<dotCnt;i++){
		dotn = 0;
		houduSum = 0;
		for(j=0;j<3;j++){
			if(JGJData[i][2+j*3] > 0){
				dotn += 1;
				houduSum += JGJData[i][2+j*3];
			}
		}
		if(dotn > 0){
			houduAvg = (u8)((float)houduSum/dotn+0.5f);
			if((houduAvg >= houduSmall)&&(houduAvg <= houduBig)){
				x = 115+i*30-6;
				y = (houduAvg-houduDesign)*4+258-6;
				LCD_SetColor(pvBkCol,GREEN);
			}
			if(houduAvg < houduSmall){
				x = 115+i*30-6;
				y = yneg - (float)(houduSmall-houduAvg)/houduSmall*(yneg-210) - 6 - 1;
				LCD_SetColor(pvBkCol,RED);
			}
			if(houduAvg > houduBig){
				
				x = 115+i*30-6;
				if(houduAvg > houduBig+100){
					houduAvg = houduBig+100;
					y = ypos + (float)(houduAvg-houduBig)/100*(320-ypos) - 6 + 1;
				}else{
					y = ypos + (float)(houduAvg-houduBig)/100*(320-ypos) - 6 + 1;	
				}
				LCD_SetColor(pvBkCol,RED);
			}
			LCD_ShowChinese(Font14,x,y,1,(u8*)"點");
			
			//显示厚度值
			sprintf(tempBuf,"%d",houduAvg);
			xc = x-strlen(tempBuf)*8/2+6;//8是字体宽度16x8
			LCD_SetColor(pvBkCol,pvTxCol);
			LCD_ShowString(ASC16x8,xc,y-16,1,tempBuf);//厚度值
		}else{
			continue;
		}
	}
}
//JGJ数据预览
void JGJDataViewInit(void)
{
	u8 i;
	char dispBuf[16];
	char timeBuf[20];
	
	u16 viBkCol;
	u16 viTxCol;
	u16 viBarCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		viBkCol = BLACK;
		viTxCol = WHITE;
		viBarCol = GRAY;
	}else{
		viBkCol = WHITE;
		viTxCol = BLACK;
		viBarCol = DGRAY;
	}
	
	//状态栏
	LCD_FillRect(57,1,422,31,viBarCol);
	
	//分栏
	for(i=0;i<5;i++){
		LCD_FillRect(70,60+i*27,396,1,viTxCol);//横
	}
	
	//分割线
	LCD_FillRect(60,196,420,2,viTxCol);//横
	
	LCD_SetColor(viBarCol,WHITE);
	LCD_ShowText(Font16,226,9,0,"JGJ数据浏览");
	
	LCD_SetColor(viBkCol,viTxCol);
	sprintf(dispBuf,"(%d/%d)",FileNum,g_GlobalPara.iSaveFileCnt);
	LCD_ShowText(Font16,250,39,0,(u8*)dispBuf);
	
	LCD_ShowText(Font16,72,39,0,"构件编号:");
	ui_DispGJName(Font16,152,39,viTxCol,FlieDataHead.GouJianName);
	
	sprintf(timeBuf,"%02d-%02d-%02d %02d:%02d:%02d",FlieDataHead.FileTimeStruct.year-2000,FlieDataHead.FileTimeStruct.month,
	FlieDataHead.FileTimeStruct.day,FlieDataHead.FileTimeStruct.hour,FlieDataHead.FileTimeStruct.min,FlieDataHead.FileTimeStruct.sec);
	
	LCD_SetColor(viBkCol,viTxCol);
	LCD_ShowString(ASC16x8,322,39,0,timeBuf);
	
	LCD_ShowText(Font16,72,66,0,"主筋直径:");
	sprintf(dispBuf,"%3d mm",FlieDataHead.ucZhuJinDiam);
	LCD_ShowText(Font16,170,66,0,(u8*)dispBuf);
	LCD_ShowText(Font16,290,66,0,"主筋间距:");
	sprintf(dispBuf,"%3d mm",FlieDataHead.ucZhuJinJianju);
	LCD_ShowText(Font16,390,66,0,(u8*)dispBuf);
	
	LCD_ShowText(Font16,72,93,0,"箍筋直径:");
	sprintf(dispBuf,"%3d mm",FlieDataHead.ucGuJinDiam);
	LCD_ShowText(Font16,170,93,0,(u8*)dispBuf);
	LCD_ShowText(Font16,290,93,0,"箍筋间距:");
	sprintf(dispBuf,"%3d mm",FlieDataHead.ucGuJinJianju);
	LCD_ShowText(Font16,390,93,0,(u8*)dispBuf);
	
	LCD_ShowText(Font16,72,120,0,"设计厚度:");
	sprintf(dispBuf,"%3d mm",FlieDataHead.ucHouDuDesign);
	LCD_ShowText(Font16,170,120,0,(u8*)dispBuf);
	LCD_ShowText(Font16,290,120,0,"平均厚度:");
	sprintf(dispBuf,"%3d mm",FlieDataHead.ucHouDuAvg);
	LCD_ShowText(Font16,390,120,0,(u8*)dispBuf);
	
	LCD_ShowText(Font16,72,147,0,"最大厚度:");
	sprintf(dispBuf,"%3d mm",FlieDataHead.ucHouDuMax);
	LCD_ShowText(Font16,170,147,0,(u8*)dispBuf);
	LCD_ShowText(Font16,290,147,0,"最小厚度:");
	sprintf(dispBuf,"%3d mm",FlieDataHead.ucHouDuMin);
	LCD_ShowText(Font16,390,147,0,(u8*)dispBuf);
	
	LCD_ShowText(Font16,72,174,0,"已存测点:");
	sprintf(dispBuf,"%3d",FlieDataHead.iSaveDotCntCh1);
	LCD_ShowText(Font16,170,174,0,(u8*)dispBuf);
	LCD_ShowText(Font16,290,174,0,"合格率:");
	sprintf(dispBuf,"%3d%%",FlieDataHead.iHeGeDotCntCh1*100/FlieDataHead.iSaveDotCntCh1);
	LCD_ShowText(Font16,390,174,0,(u8*)dispBuf);
	
	LCD_ShowText(Font16,448,174,0,JGJParaTable[6].pList[FlieDataHead.ucGouJianType].listStr);
	
	JGJPreView();
	
	MENU_TABLE_STRUCT ParaMenu[2] = {
		{0,2,"徊",NULL},
		{1,2,"统","数据"}
	};
	//菜单显示
	ui_DispMenuBtn(ParaMenu,0);
	
	//按钮初始化
	g_u8ButtonNums = 2;
	struct BUTTON viBtn[2] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0},//0, 返回
		{  0,  50,  59, 103, 0,0},//1  数据
	};
	memcpy(g_ButtonBuf,viBtn,sizeof(viBtn));
	
	ClearTouchSta();//清除触摸
}
//JGJ数据预览
void JGJDataViewFun(void)
{
	u16 vfBkCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		vfBkCol = BLACK;
	}else{
		vfBkCol = WHITE;
	}
	JGJDataViewInit();
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		
		//返回
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_BACK)){
			g_ucTurnGouJianSta = 0;//切换构件状态
			LCD_FillRect(56,32,424,288,vfBkCol);
			return;
		}
		//统计
		if(g_u8TouchResult == 1){
			LCD_FillRect(56,32,424,288,vfBkCol);
			JGJDataOrig();
			JGJDataViewInit();
		}
		if(g_u8TouchResult == SLIDE_END_UP){//上划
			g_ucTurnGouJianSta = NEXT;//切换构件状态
			return;
		}
		if(g_u8TouchResult == SLIDE_END_DOWN){//下划
			g_ucTurnGouJianSta = LAST;//切换构件状态
			return;
		}
	}
}
//BLOCK//////////////////////////////////////////////////////////////////////////////////////
//统计信息
void BlockDataViewBro_Fun(void)
{
	u8 i;
	char tempBuf[16];
	char timeBuf[20];
	
	u16 broBkCol;
	u16 broTxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		broBkCol = BLACK;
		broTxCol = WHITE;
	}else{
		broBkCol = WHITE;
		broTxCol = BLACK;
	}
	
	ui_DispTitleBar("统计信息",0);
	
	for(i=0;i<7;i++){
		LCD_FillRect(70,70+i*30,394,1,GRAY);//横
	}
	LCD_SetColor(broBkCol,broTxCol);
	LCD_ShowText(Font16,80,47,0,"构件编号:");
	ui_DispGJName(ASC16x8,180,47,broTxCol,FlieDataHead.GouJianName);
	
	LCD_SetColor(broBkCol,broTxCol);
//	LCD_ShowText(Font20,320,50,0,BlockParaTable[6].pList[FlieDataHead.ucRebarType].listStr);
	sprintf(tempBuf,"(%d/%d)",FileNum,g_GlobalPara.iSaveFileCnt);
	LCD_ShowText(Font16,278,47,0,(u8*)tempBuf);
	
	sprintf(timeBuf,"%02d-%02d-%02d %02d:%02d:%02d",FlieDataHead.FileTimeStruct.year-2000,FlieDataHead.FileTimeStruct.month,
	FlieDataHead.FileTimeStruct.day,FlieDataHead.FileTimeStruct.hour,FlieDataHead.FileTimeStruct.min,FlieDataHead.FileTimeStruct.sec);
	LCD_ShowString(ASC16x8,80,77,0,timeBuf);
//	
	LCD_ShowText(Font16,80,107,0,"X方向直径:");
	sprintf(tempBuf,"%3d mm  ",FlieDataHead.ucZhuJinDiam);
	LCD_ShowString(ASC20x10,180,105,0,tempBuf);
	LCD_ShowText(Font16,290,107,0,"X方向间距:");
	sprintf(tempBuf,"%3d mm  ",FlieDataHead.ucZhuJinJianju);
	LCD_ShowString(ASC20x10,390,105,0,tempBuf);
	LCD_ShowText(Font16,80,137,0,"Y方向直径:");
	sprintf(tempBuf,"%3d mm  ",FlieDataHead.ucGuJinDiam);
	LCD_ShowString(ASC20x10,180,135,0,tempBuf);
	LCD_ShowText(Font16,290,137,0,"Y方向间距:");
	sprintf(tempBuf,"%3d mm  ",FlieDataHead.ucGuJinJianju);
	LCD_ShowString(ASC20x10,390,135,0,tempBuf);
	LCD_ShowText(Font16,80,167,0,"设计厚度:");
	sprintf(tempBuf,"%3d mm  ",FlieDataHead.ucHouDuDesign);
	LCD_ShowString(ASC20x10,180,165,0,tempBuf);
	LCD_ShowText(Font16,290,167,0,"平均厚度:");
	sprintf(tempBuf,"%3d mm  ",FlieDataHead.ucHouDuAvg);
	LCD_ShowString(ASC20x10,390,165,0,tempBuf);
	LCD_ShowText(Font16,80,197,0,"最小厚度:");
	sprintf(tempBuf,"%3d mm  ",FlieDataHead.ucHouDuMin);
	LCD_ShowString(ASC20x10,180,195,0,tempBuf);
	LCD_ShowText(Font16,290,197,0,"最大厚度:");
	sprintf(tempBuf,"%3d mm  ",FlieDataHead.ucHouDuMax);
	LCD_ShowString(ASC20x10,390,195,0,tempBuf);
	LCD_ShowText(Font16,80,227,0,"已存测点:");
	sprintf(tempBuf,"%3d  ",FlieDataHead.iSaveDotCntCh1);
	LCD_ShowString(ASC20x10,180,225,0,tempBuf);
	
	//页面左侧菜单表
	MENU_TABLE_STRUCT PreviewMenu[1] = {
		{0,1,"徊",NULL}
	};
	//菜单显示
	ui_DispMenuBtn(PreviewMenu,0);
	//按钮初始化
	g_u8ButtonNums = 1;
	struct BUTTON vbBtn[1] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0} //0, 返回
	};
	memcpy(g_ButtonBuf,vbBtn,sizeof(vbBtn));
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
//		ui_OneButtonReactionXY(0,0,0,47,33,GOLD,ORANGE);//返回
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		//切换页面
		if(g_u16KeyValBuf == KEY_FN){
//			return;
		}
		
		//返回
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_BACK)){
			LCD_FillRect(56,32,424,288,broBkCol);
			return;
		}
		Sleep_ms(5);
	}
}
//预览
void BlockPreviewBro_Init(void)
{
	u8 i;
	u16 prBkCol;
	u16 prTxCol;
	u16 prBarCol;
	u16 prValCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		prBkCol = BLACK;
		prTxCol = WHITE;
		prBarCol = GRAY;
		prValCol = GOLD;
	}else{
		prBkCol = WHITE;
		prTxCol = BLACK;
		prBarCol = DGRAY;
		prValCol = BLACK;
	}
	//状态栏
	LCD_FillRect(57,1,422,31,prBarCol);
	
	//构件名
	ui_DispGJName(ASC16x8,70,8,WHITE,FlieDataHead.GouJianName);
	
	//钢筋类型
	LCD_SetColor(prBarCol,WHITE);
	LCD_ShowChinese(Font16,180,8,0,BlockParaTable[6].pList[FlieDataHead.ucRebarType].listStr);
	
	LCD_SetColor(prBarCol,WHITE);
	LCD_ShowChinese(Font20,220,6,0,"φ");
	LCD_ShowString(ASC20x10,264,6,0,"x");
	LCD_ShowNnNums(ASC20x10,244,6,0,2,FlieDataHead.ucZhuJinDiam);//g_SysPara.g_SamplePara.iBlockDiamX
	LCD_ShowNnNums(ASC20x10,274,6,0,2,FlieDataHead.ucGuJinDiam);//g_SysPara.g_SamplePara.iBlockDiamY
	
	LCD_SetColor(prBkCol,prTxCol);
	LCD_ShowChinese(Font16,70,40,0,"匚");
	LCD_ShowString(ASC16x8,88,34,0,"X");
	LCD_ShowString(ASC16x8,68,56,0,"Y");
	
	//网格4x4
	if(g_SysPara.g_SamplePara.iBlockAreaNum == 0){
		LCD_FillRect(135,60,240,240,LLGRAY);//区域
		LCD_DrawRect(135,60,241,241,GRAY);//框
		LCD_SetColor(prBkCol,prTxCol);
		for(i=0;i<5;i++){
			if(i==0){
				LCD_ShowNnNums(ASC12x6,135+i*60-15,40,0,3,i*120);
			}else{
				LCD_ShowNnNums(ASC12x6,135+i*60-9,40,0,3,i*120);
			}
			LCD_ShowNnNums(ASC12x6,108,60+i*60-6,0,3,i*120);
			if((i==0)||(i==4))continue;
			LCD_FillRect(136,60+i*60,239,1,LGRAY);//横
			LCD_FillRect(135+i*60,61,1,239,LGRAY);//竖
		}
	}else{//网格8x8
		LCD_FillRect(135,60,240,240,LLGRAY);//区域
		LCD_DrawRect(135,60,241,241,GRAY);//框
		LCD_SetColor(prBkCol,prTxCol);
		for(i=0;i<9;i++){
			if(i==0){
				LCD_ShowNnNums(ASC12x6,135+i*30-15,40,0,3,i*120);
			}else{
				LCD_ShowNnNums(ASC12x6,135+i*30-9,40,0,3,i*120);
			}
			LCD_ShowNnNums(ASC12x6,108,60+i*30-6,0,3,i*120);
			if((i==0)||(i==8))continue;
			LCD_FillRect(136,60+i*30,239,1,LGRAY);//横
			LCD_FillRect(135+i*30,61,1,239,LGRAY);//竖
		}
	}
	
	LCD_SetColor(prBkCol,GRAY);
	LCD_ShowText(Font16,405,160,0,"位置(mm)");
	LCD_SetColor(prBkCol,prValCol);
	LCD_ShowString(ASC16x8,405,180,0,"X:");
	LCD_ShowString(ASC16x8,405,200,0,"Y:");
	
	LCD_SetColor(prBkCol,GRAY);
	LCD_ShowText(Font16,405,230,0,"厚度(mm)");
	LCD_SetColor(prBkCol,prValCol);
	LCD_ShowString(ASC24x12,405,260,0,"-- ");
//	LCD_ShowString(ASC16x8,405,250,0,"X:24");
//	LCD_ShowString(ASC16x8,405,280,0,"Y:36");
	
	//页面左侧菜单表
	MENU_TABLE_STRUCT PreviewMenu[2] = {
		{0,2,"徊",NULL},
		{1,2,"统","数据"}
	};
	//菜单显示
	ui_DispMenuBtn(PreviewMenu,0);
	//按钮初始化
	g_u8ButtonNums = 3;
	struct BUTTON viewBtn[3] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0},//0, 返回
		{  0,  50,  59, 103, 0,0},//1, 数据
		{135,  60, 374, 300, 0,0} //2, 预览区域
	};
	memcpy(g_ButtonBuf,viewBtn,sizeof(viewBtn));
	
	BrushBlockRebar();
	
	ClearTouchSta();//清除触摸
}
//预览
void BlockPreviewBro_Fun(void)
{
	u16 *readAddr = NULL;
	u16 touchX,touchY,width,height;
//	u16 siteX,siteY;
	static u16 touchX_Last,touchY_Last;
//	int readLen;
	
	u16 prfBkCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		prfBkCol = BLACK;
	}else{
		prfBkCol = WHITE;
	}
	
	width=16;
	height=16;
//	readLen = width*height;
	
	BlockPreviewBro_Init();
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		//切换页面
		if(g_u16KeyValBuf == KEY_FN){
//			return;
		}
		
		//返回
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_BACK)){
			g_ucTurnGouJianSta = 0;//切换构件状态
			LCD_FillRect(56,32,424,288,prfBkCol);
			myfree(SRAMIN,readAddr);
			readAddr = NULL;
			return;
		}
		//数据
		if(g_u8TouchResult == 1){
			myfree(SRAMIN,readAddr);
			readAddr = NULL;
			LCD_FillRect(56,32,424,288,prfBkCol);
			BlockDataViewBro_Fun();
			BlockPreviewBro_Init();
		}
		//点击选择
		if(g_u8TouchResult == 2){
			if(readAddr != NULL){
				LCD_ShowPicture(touchX_Last,touchY_Last,width,height,readAddr);
				myfree(SRAMIN,readAddr);
				readAddr = NULL;
			}
			touchX = TPData.TpX[0]-8;
			touchY = TPData.TpY[0]-8;
			
			readAddr = LCD_ReadRect(touchX,touchY,width,height);
			if(readAddr != NULL){
				LCD_FillRect(touchX,touchY,width,height,GOLD);
				LCD_DrawRect(touchX,touchY,width,height,GRAY);
			}
			
			DispClickRebarInfo(TPData.TpX[0],TPData.TpY[0]);
			
			touchX_Last = touchX;
			touchY_Last = touchY;
		}
		if(g_u8TouchResult == SLIDE_END_UP){//上划
			g_ucTurnGouJianSta = NEXT;//切换构件状态
			myfree(SRAMIN,readAddr);
			readAddr = NULL;
			return;
		}
		if(g_u8TouchResult == SLIDE_END_DOWN){//下划
			g_ucTurnGouJianSta = LAST;//切换构件状态
			myfree(SRAMIN,readAddr);
			readAddr = NULL;
			return;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//创建选中文件链表
SelFileNode *CreateSelLink(void)
{
	SelFileNode *head = (SelFileNode*)mymalloc(SRAMIN,sizeof(SelFileNode));
	if(head != NULL){
		head->next = NULL;
		return head;
	}else{
		return NULL;
	}
}

SelFileNode *DeleteSelLink(SelFileNode *link)
{
	SelFileNode *p = link;
	SelFileNode *temp = NULL;
	while(p != NULL){
		temp = p;
		p = p->next;
		myfree(SRAMIN,temp);
	}
	return NULL;
}

SelFileNode *DeleteSelFile(SelFileNode *link)
{
	char fpath[40];
	SelFileNode *p = link->next;
	SelFileNode *temp = NULL;
	while(p != NULL){
		temp = p;
		p = p->next;
		sprintf(fpath,"0:/GY91/%s",temp->fileName);
		f_unlink(fpath);
		myfree(SRAMIN,temp);
	}
	myfree(SRAMIN,link);
	return NULL;
}
//勾选或取消勾选构件
void SelectFileOpt(SelFileNode *link,char* fname,short fpage,short fnum)
{
	short curfilNum;
	SelFileNode *p = link;
	SelFileNode *del = NULL;
	
	u16 bkCol;
	u16 hCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
		hCol = DGRAY;
	}else{
		bkCol = WHITE;
		hCol = LGRAY;
	}
	
	curfilNum = (fpage-1)*PageCap + fnum;
	
	while(p->next != NULL){
		if(p->next->filepage == fpage){
			if(p->next->fileNum == fnum){//已勾选构件，取消勾选
				del = p->next;
				p->next = p->next->next;
				myfree(SRAMIN,del);
				
				LCD_DrawRect(70,90+fnum*40,11,11,GRAY);//选中框
				if(curfilNum == HighLightNum){
					LCD_FillRect(71,91+fnum*40,9,9,hCol);
				}else{
					LCD_FillRect(71,91+fnum*40,9,9,bkCol);
				}
				return;
			}
		}
		p = p->next;
	}
	//创建新节点
	SelFileNode *temp = mymalloc(SRAMIN,sizeof(SelFileNode));
	if(temp != NULL){
		sprintf(temp->fileName,"%s",fname);
		temp->filepage = fpage;
		temp->fileNum = fnum;
		temp->next = NULL;
		p->next = temp;
		
		LCD_DrawRect(70,90+p->next->fileNum*40,11,11,GRAY);//选中框
		LCD_FillRect(71,91+p->next->fileNum*40,9,9,GOLD);
	}
}

//翻页时刷新选中显示
void SelectFileRefresh(SelFileNode *link,short page)
{
//	short curfilNum;
	SelFileNode *p = link->next;
	
	if(link == NULL)return;
	while(p != NULL){
		if(p->filepage == page){
//			curfilNum = (p->filepage-1)*PageCap + p->fileNum;
			LCD_DrawRect(70,90+p->fileNum*40,11,11,GRAY);//选中框
			LCD_FillRect(71,91+p->fileNum*40,9,9,GOLD);
		}
		p = p->next;
	}
}

