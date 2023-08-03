/*******************************************************************************
创建者：张喜双
创建日期：2018
功能描述：
    1. 厚度检测、钢筋扫描模式，共用函数及变量
    
*******************************************************************************/
#include "ui_Dialog.h"
#include "Config_ui.h"
#include "pyinput.h"

const char KeyBoard[56] = "1234567890qwertyuiopasdfghjklzxcvbnm#-_.~,+(),;^&!";

//刷新键盘
static void BrushKeyBoard(char* keyValBuf,u8 language,u8 capsLock,u8 symble)
{
	u8 i;
	
	u16 ctlBkCol;
	u16 ctlTxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		ctlBkCol = DGRAY;
		ctlTxCol = WHITE;
	}else{
		ctlBkCol = LGRAY;
		ctlTxCol = BLACK;
	}
	
	//显示字符
	if(language == 0){
		for(i=0;i<10;i++){
			LCD_SetColor(WHITE,BLACK);
			LCD_ShowASC(ASC20x10,20+i*48,75,keyValBuf[i]);//数字1
		}
	}
	for(i=0;i<10;i++){
		LCD_SetColor(WHITE,BLACK);
		if(language == 0){
			if(capsLock != 0)LCD_ShowASC(ASC20x10,20+i*48,75+52,keyValBuf[i+10]);//字母
			else LCD_ShowASC(ASC20x10,20+i*48,75+52,keyValBuf[i+10]-32);//字母
		}else{
			LCD_ShowASC(ASC20x10,20+i*48,75+52,keyValBuf[i+10]-32);//字母
		}
	}
	for(i=0;i<9;i++){
		LCD_SetColor(WHITE,BLACK);
		if(language == 0){
			if(capsLock != 0)LCD_ShowASC(ASC20x10,44+i*48,75+52*2,keyValBuf[i+20]);//字母
			else LCD_ShowASC(ASC20x10,44+i*48,75+52*2,keyValBuf[i+20]-32);//字母
		}else{
			LCD_ShowASC(ASC20x10,44+i*48,75+52*2,keyValBuf[i+20]-32);//字母
		}
	}
	for(i=0;i<7;i++){
		LCD_SetColor(WHITE,BLACK);
		if(language == 0){
			if(capsLock != 0)LCD_ShowASC(ASC20x10,92+i*48,75+52*3,keyValBuf[i+29]);//字母
			else LCD_ShowASC(ASC20x10,92+i*48,75+52*3,keyValBuf[i+29]-32);//字母
		}else{
			LCD_ShowASC(ASC20x10,92+i*48,75+52*3,keyValBuf[i+29]-32);//字母
		}
	}
	for(i=0;i<7;i++){
		LCD_SetColor(WHITE,BLACK);
		if((language == 1)&&(symble == 1))
			LCD_ShowASC(ASC20x10,92+i*48,75+52*4,keyValBuf[i+36+7]);//字符
		else LCD_ShowASC(ASC20x10,92+i*48,75+52*4,keyValBuf[i+36]);//字符
	}
		
	if(language == 0){//英文
		LCD_FillRect(1,60+52*3,70,50,ctlBkCol);//大小写或符号
		if(capsLock != 0){//小写
			LCD_SetColor(ctlBkCol,GRAY);
			LCD_ShowChinese(Font32,20,225,0,"箭");
		}else {//大写
			LCD_SetColor(ctlBkCol,ctlTxCol);
			LCD_ShowChinese(Font32,20,225,0,"箭");
		}
		LCD_SetColor(ctlBkCol,GRAY);
		LCD_ShowChinese(Font20,32,281,0,"杠");
		LCD_ShowChinese(Font16,45,285,0,"中");
		LCD_SetColor(ctlBkCol,ctlTxCol);
		LCD_ShowChinese(Font20,12,281,0,"英");
	}else {//中文
		LCD_FillRect(1,60+52*3,70,50,ctlBkCol);//大小写或符号
		if(symble == 0){//符号1
			LCD_SetColor(ctlBkCol,ctlTxCol);
			LCD_ShowChinese(Font20,10,231,0,"符号");
			LCD_ShowASC(ASC20x10,50,231,'1');
		}else {//符号2
			LCD_SetColor(ctlBkCol,ctlTxCol);
			LCD_ShowChinese(Font20,10,231,0,"符号");
			LCD_ShowASC(ASC20x10,50,231,'2');
		}
		LCD_SetColor(ctlBkCol,GRAY);
		LCD_ShowChinese(Font20,32,281,0,"杠");
		LCD_ShowChinese(Font16,45,285,0,"英");
		LCD_SetColor(ctlBkCol,ctlTxCol);
		LCD_ShowChinese(Font20,12,281,0,"中");
		
		LCD_SetColor(ctlBkCol,GRAY);
		LCD_ShowChinese(Font32,8,74,0,"←");
		LCD_ShowChinese(Font32,441,74,0,"→");
	}
	LCD_SetColor(ctlBkCol,ctlTxCol);LCD_ShowChinese(Font32,428,225,0,"刪");		
	LCD_SetColor(ctlBkCol,ctlTxCol);LCD_ShowChinese(Font20,424,283,0,"清空");	
}
//构件名称设置对话框
void TextSetDialog(SYS_PARA_TABLE_STRUCT_P paraTable_p)
{
	u8 i;
	u8 language = 0;//0：英文，1：中文
	u8 capsLock = 0;//0：大写，1：小写
	u8 symble = 0;//0：符号1，1：符号2
	
	u8 cursor;
	u8 paraDispBuf[24];
	u8 pinyinBuf[48];
	u8 pinyinLen = 0;
	u8 matchNum = 0;
//	u8* matchMbP;
	u8 matchQnt = 0;
	u8 matchPageQnt;
	u8 matchPageNum;
//	u8 onePageHzCnt;
	u8 curHzNum;
	u8 oneHzBuf[4];
	
	u16 reBkCol;
	u16 diaBkCol;
	u16 tctlBkCol;
	u16 tctlTxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		reBkCol = BLACK;
		diaBkCol = BLACK;
		tctlBkCol = DGRAY;
		tctlTxCol = WHITE;
	}else{
		reBkCol = WHITE;
		diaBkCol = LLGRAY;
		tctlBkCol = LGRAY;
		tctlTxCol = BLACK;
	}
	
	LCD_FillRect(0,0,480,320,diaBkCol);//弹窗
	LCD_FillRect(100,10,294,40,WHITE);//输入框
	LCD_SetColor(diaBkCol,tctlTxCol);
	LCD_ShowChinese(Font20,10,20,0,paraTable_p->sChName);
	
	LCD_SetColor(tctlBkCol,tctlTxCol);
	LCD_FillRect(408,10,64,40,tctlBkCol);//确定
	LCD_ShowChinese(Font20,420,20,0,"确定");

	//显示按键
	if(language == 0){
		for(i=0;i<10;i++) LCD_FillRect( 1+i*48, 60,      46, 50,WHITE);//数字
	}else{
		LCD_FillRect(1,60,478,50,diaBkCol);//汉字显示区
		LCD_FillRect(1,70,478,40,tctlBkCol);//汉字显示区
		LCD_SetColor(tctlBkCol,GRAY);
		LCD_ShowChinese(Font32,8,74,0,"←");
		LCD_ShowChinese(Font32,441,74,0,"→");
	}
	for(i=0;i<10;i++) LCD_FillRect( 1+i*48, 60+52,   46, 50,WHITE);
	for(i=0;i<9; i++) LCD_FillRect(25+i*48, 60+52*2, 46, 50,WHITE);
	for(i=0;i<7; i++) LCD_FillRect(73+i*48, 60+52*3, 46, 50,WHITE);
	for(i=0;i<7; i++) LCD_FillRect(73+i*48, 60+52*4, 46, 50,WHITE);
	
	LCD_FillRect(1,60+52*3,70,50,tctlBkCol);//大小写或符号
	LCD_FillRect(1,60+52*4,70,50,tctlBkCol);//中英文切换
	LCD_FillRect(409,60+52*3,70,50,tctlBkCol);//退格
	LCD_FillRect(409,60+52*4,70,50,tctlBkCol);//清空
	
	BrushKeyBoard((char*)KeyBoard,language,capsLock,symble);
	
	//按钮初始化
	g_u8ButtonNums = 49;
	
	//字符
	for(i=0;i<10;i++){
		g_ButtonBuf[i].sX = 1+i*48; g_ButtonBuf[i].sY = 60; g_ButtonBuf[i].eX = g_ButtonBuf[i].sX+46-1; g_ButtonBuf[i].eY = g_ButtonBuf[i].sY+50-1;
		g_ButtonBuf[i].btSta = 0;
		g_ButtonBuf[i].btAbility = 0;
	}
	for(i=0;i<10;i++){
		g_ButtonBuf[i+10].sX = 1+i*48; g_ButtonBuf[i+10].sY = 60+52; g_ButtonBuf[i+10].eX = g_ButtonBuf[i+10].sX+46-1; g_ButtonBuf[i+10].eY = g_ButtonBuf[i+10].sY+50-1;
		g_ButtonBuf[i+10].btSta = 0;
		g_ButtonBuf[i+10].btAbility = 0;
	}
	for(i=0;i<9; i++){
		g_ButtonBuf[i+20].sX = 25+i*48; g_ButtonBuf[i+20].sY = 60+52*2; g_ButtonBuf[i+20].eX = g_ButtonBuf[i+20].sX+46-1; g_ButtonBuf[i+20].eY = g_ButtonBuf[i+20].sY+50-1;
		g_ButtonBuf[i+20].btSta = 0;
		g_ButtonBuf[i+20].btAbility = 0;
	}
	for(i=0;i<7; i++){
		g_ButtonBuf[i+29].sX = 73+i*48; g_ButtonBuf[i+29].sY = 60+52*3; g_ButtonBuf[i+29].eX = g_ButtonBuf[i+29].sX+46-1; g_ButtonBuf[i+29].eY = g_ButtonBuf[i+29].sY+50-1;
		g_ButtonBuf[i+29].btSta = 0;
		g_ButtonBuf[i+29].btAbility = 0;
	}
	for(i=0;i<7; i++){
		g_ButtonBuf[i+36].sX = 73+i*48; g_ButtonBuf[i+36].sY = 60+52*4; g_ButtonBuf[i+36].eX = g_ButtonBuf[i+36].sX+46-1; g_ButtonBuf[i+36].eY = g_ButtonBuf[i+36].sY+50-1;
		g_ButtonBuf[i+36].btSta = 0;
		g_ButtonBuf[i+36].btAbility = 0;
	}
	//大小写
	g_ButtonBuf[43].sX = 1;   g_ButtonBuf[43].sY = 216; g_ButtonBuf[43].eX = 70;  g_ButtonBuf[43].eY = 265;
	g_ButtonBuf[43].btSta = 0;
	g_ButtonBuf[43].btAbility = 0;
	//中英文                                                                                                
	g_ButtonBuf[44].sX = 1;   g_ButtonBuf[44].sY = 268; g_ButtonBuf[44].eX = 70;  g_ButtonBuf[44].eY = 317;
	g_ButtonBuf[44].btSta = 0;
	g_ButtonBuf[44].btAbility = 0;
	//退格                                                                                                  
	g_ButtonBuf[45].sX = 409; g_ButtonBuf[45].sY = 216; g_ButtonBuf[45].eX = 478; g_ButtonBuf[45].eY = 265;
	g_ButtonBuf[45].btSta = 0;
	g_ButtonBuf[45].btAbility = 0;
	//清空                                                                                                  
	g_ButtonBuf[46].sX = 409; g_ButtonBuf[46].sY = 268; g_ButtonBuf[46].eX = 478; g_ButtonBuf[46].eY = 317;
	g_ButtonBuf[46].btSta = 0;
	g_ButtonBuf[46].btAbility = 0;
	//确定                                                                                                  
	g_ButtonBuf[47].sX = 408; g_ButtonBuf[47].sY = 10;  g_ButtonBuf[47].eX = 471; g_ButtonBuf[47].eY = 49;
	g_ButtonBuf[47].btSta = 0;
	g_ButtonBuf[47].btAbility = 0;
	//返回                                                                                                  
	g_ButtonBuf[48].sX = 0;   g_ButtonBuf[48].sY = 0;   g_ButtonBuf[48].eX = 79;  g_ButtonBuf[48].eY = 49;
	g_ButtonBuf[48].btSta = 0;
	g_ButtonBuf[48].btAbility = 0;
	
	//参数显示
	memset(pinyinBuf,0,sizeof(pinyinBuf));
	memset(paraDispBuf,0,sizeof(paraDispBuf));
	if(strlen((const char*)paraTable_p->pList->listStr) > paraTable_p->iParaMaxVal){
		memcpy(paraDispBuf,paraTable_p->pList->listStr,paraTable_p->iParaMaxVal);
	}else{
		memcpy(paraDispBuf,paraTable_p->pList->listStr,strlen((const char*)paraTable_p->pList->listStr));
	}
	
//	sprintf((char*)paraDispBuf,"HCGK%04d",*paraTable_p->ParaAddr);
	cursor = strlen((const char*)paraDispBuf);
	LCD_SetColor(WHITE,BLACK);
	Show_Text(Font24,120,18,0,paraDispBuf);
	if(cursor <= paraTable_p->iParaMaxVal)LCD_FillRect(120+cursor*12,42,12,2,RED);
	else LCD_FillRect(120+paraTable_p->iParaMaxVal*12,42,12,2,RED);
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_AllButtonReaction(43,WHITE,GOLD);//字符键
		ui_OneButtonReaction(43,tctlBkCol,GOLD);//大小写
		ui_OneButtonReaction(44,tctlBkCol,GOLD);//中英文
		ui_OneButtonReaction(45,tctlBkCol,GOLD);//退格
		ui_OneButtonReaction(46,tctlBkCol,GOLD);//清空
		ui_OneButtonReaction(47,tctlBkCol,GOLD);//确定
//		ui_OneButtonReaction(48,GOLD,ORANGE);//返回
//		ui_OneButtonReactionXY(48,0,0,47,33,GOLD,ORANGE);//返回
		
//		if(g_u16KeyValBuf == KEY_OK){

//		}
//		if(g_u16KeyValBuf == KEY_BACK){
//			
//		}
//		if(g_u16KeyValBuf == KEY_FN){

//		}
		
		//字符
		if(g_u8TouchResult < 43){
			if(g_u8TouchResult<10){//数字
				if(language == 0){
					if(cursor < paraTable_p->iParaMaxVal){
						paraDispBuf[cursor] = KeyBoard[g_u8TouchResult];
						paraDispBuf[cursor+1] = '\0';
						cursor += 1;
						LCD_SetColor(WHITE,BLACK);
						LCD_ShowASC(ASC24x12,120+(cursor-1)*12,18,KeyBoard[g_u8TouchResult]);
					}
				}else{//汉字
					if(g_u8TouchResult == 0){//左翻页
						if(matchPageNum > 0){
							matchPageNum -= 1;
							for(i=0;i<8;i++){
								curHzNum = matchPageNum*8+i;
								if(curHzNum < matchQnt){
									oneHzBuf[0] = g_t26.pymb[0]->pymb[curHzNum*2];
									oneHzBuf[1] = g_t26.pymb[0]->pymb[curHzNum*2+1];
									oneHzBuf[2] = '\0';
									LCD_SetColor(tctlBkCol,tctlTxCol);
									Show_Text(Font20,60+i*48,80,0,oneHzBuf);
								}else{
									LCD_SetColor(tctlBkCol,tctlTxCol);
									LCD_ShowString(Font20,60+i*48,80,0,"  ");
								}
							}
							
							LCD_SetColor(tctlBkCol,tctlTxCol);
							LCD_ShowChinese(Font32,441,74,0,"→");
							if(matchPageNum == 0){
								LCD_SetColor(tctlBkCol,GRAY);
								LCD_ShowChinese(Font32,8,74,0,"←");
							}else{
								LCD_SetColor(tctlBkCol,tctlTxCol);
								LCD_ShowChinese(Font32,8,74,0,"←");
							}
						}else{
							LCD_SetColor(tctlBkCol,GRAY);
							LCD_ShowChinese(Font32,8,74,0,"←");
						}
					}else if(g_u8TouchResult == 9){//右翻页
						if(matchPageNum < matchPageQnt-1){
							matchPageNum += 1;
							for(i=0;i<8;i++){
								curHzNum = matchPageNum*8+i;
								if(curHzNum < matchQnt){
									oneHzBuf[0] = g_t26.pymb[0]->pymb[curHzNum*2];
									oneHzBuf[1] = g_t26.pymb[0]->pymb[curHzNum*2+1];
									oneHzBuf[2] = '\0';
									LCD_SetColor(tctlBkCol,tctlTxCol);
									Show_Text(Font20,60+i*48,80,0,oneHzBuf);
								}else{
									LCD_SetColor(tctlBkCol,tctlTxCol);
									LCD_ShowString(Font20,60+i*48,80,0,"  ");
								}
							}
							LCD_SetColor(tctlBkCol,tctlTxCol);
							LCD_ShowChinese(Font32,8,74,0,"←");
							if(matchPageNum == matchPageQnt-1){
								LCD_SetColor(tctlBkCol,GRAY);
								LCD_ShowChinese(Font32,441,74,0,"→");
							}else{
								LCD_SetColor(tctlBkCol,tctlTxCol);
								LCD_ShowChinese(Font32,441,74,0,"→");
							}
						}else{
							LCD_SetColor(tctlBkCol,GRAY);
							LCD_ShowChinese(Font32,441,74,0,"→");
						}
					}else{
						if(pinyinLen > 0){//有输入
							if(cursor < paraTable_p->iParaMaxVal-1){
								curHzNum = matchPageNum*8+(g_u8TouchResult-1);
								paraDispBuf[cursor] = g_t26.pymb[0]->pymb[curHzNum*2];
								paraDispBuf[cursor+1] = g_t26.pymb[0]->pymb[curHzNum*2+1];
								paraDispBuf[cursor+2] = '\0';
								cursor += 2;
								
								oneHzBuf[0] = g_t26.pymb[0]->pymb[curHzNum*2];
								oneHzBuf[1] = g_t26.pymb[0]->pymb[curHzNum*2+1];
								oneHzBuf[2] = '\0';
								LCD_SetColor(WHITE,BLACK);
								Show_Text(Font24,120+(cursor-2)*12,18,0,oneHzBuf);
								
								pinyinLen = 0;
								memset(pinyinBuf,0,sizeof(pinyinBuf));
								LCD_FillRect(1,52,478,16,diaBkCol);//汉字显示区
								LCD_FillRect(40,70,400,40,tctlBkCol);//汉字显示区
								
								LCD_SetColor(tctlBkCol,GRAY);
								LCD_ShowChinese(Font32,8,74,0,"←");
								LCD_ShowChinese(Font32,441,74,0,"→");
							}
						}
					}
				}
			}else if((g_u8TouchResult>=10)&&(g_u8TouchResult<36)){//字母
				if(language == 0){//英文
					if(capsLock != 0){
						if(cursor < paraTable_p->iParaMaxVal){
							paraDispBuf[cursor] = KeyBoard[g_u8TouchResult];
							paraDispBuf[cursor+1] = '\0';
							cursor += 1;
							LCD_SetColor(WHITE,BLACK);
							LCD_ShowASC(ASC24x12,120+(cursor-1)*12,18,KeyBoard[g_u8TouchResult]);
						}
					}else {
						if(cursor < paraTable_p->iParaMaxVal){
							paraDispBuf[cursor] = KeyBoard[g_u8TouchResult]-32;
							paraDispBuf[cursor+1] = '\0';
							cursor += 1;
							LCD_SetColor(WHITE,BLACK);
							LCD_ShowASC(ASC24x12,120+(cursor-1)*12,18,KeyBoard[g_u8TouchResult]-32);
						}
					}
				}else {//中文
//					paraDispBuf[cursor] = KeyBoard[g_u8TouchResult];
//					cursor += 1;
//					LCD_SetColor(WHITE,BLACK);
//					LCD_ShowASC(ASC24x12,120+(cursor-1)*12,18,KeyBoard[g_u8TouchResult]);
					pinyinBuf[pinyinLen++] = KeyBoard[g_u8TouchResult];
					if(pinyinLen > 40)pinyinLen = 40;
					LCD_SetColor(tctlBkCol,tctlTxCol);
					LCD_ShowString(ASC16x8,1,52,0,(char*)pinyinBuf);
					
					matchNum = g_t26.getpymb(pinyinBuf);
					if((matchNum)||(matchNum&0x80)){//有匹配
//						matchMbP = g_t26.pymb[0]->pymb;
						matchQnt = strlen((char*)g_t26.pymb[0]->pymb)/2;
						if(matchQnt%8 == 0){
							matchPageQnt = matchQnt/8;
						}else{
							matchPageQnt = matchQnt/8+1;
						}
						matchPageNum = 0;
						
						for(i=0;i<8;i++){
							curHzNum = matchPageNum*8+i;
							if(curHzNum < matchQnt){
								oneHzBuf[0] = g_t26.pymb[0]->pymb[curHzNum*2];
								oneHzBuf[1] = g_t26.pymb[0]->pymb[curHzNum*2+1];
								oneHzBuf[2] = '\0';
								LCD_SetColor(tctlBkCol,tctlTxCol);
								Show_Text(Font20,60+i*48,80,0,oneHzBuf);
							}else{
								LCD_SetColor(tctlBkCol,tctlTxCol);
								LCD_ShowString(Font20,60+i*48,80,0,"  ");
							}
						}
						if(matchPageNum < matchPageQnt-1){
							LCD_SetColor(tctlBkCol,tctlTxCol);
							LCD_ShowChinese(Font32,441,74,0,"→");
						}
					}else{//无匹配
						
					}
				}
			}else {//字符
				if(language == 0){
					if(cursor < paraTable_p->iParaMaxVal){
						paraDispBuf[cursor] = KeyBoard[g_u8TouchResult];
						paraDispBuf[cursor+1] = '\0';
						cursor += 1;
						LCD_SetColor(WHITE,BLACK);
						LCD_ShowASC(ASC24x12,120+(cursor-1)*12,18,KeyBoard[g_u8TouchResult]);
					}
				}else{
					pinyinLen = 0;
					memset(pinyinBuf,0,sizeof(pinyinBuf));
					LCD_FillRect(1,52,478,16,diaBkCol);//汉字显示区
					LCD_FillRect(40,70,400,40,tctlBkCol);//汉字显示区
					if(symble == 1){
						if(cursor < paraTable_p->iParaMaxVal){
							paraDispBuf[cursor] = KeyBoard[g_u8TouchResult+7];
							paraDispBuf[cursor+1] = '\0';
							cursor += 1;
							LCD_SetColor(WHITE,BLACK);
							LCD_ShowASC(ASC24x12,120+(cursor-1)*12,18,KeyBoard[g_u8TouchResult+7]);
						}
					}else{
						if(cursor < paraTable_p->iParaMaxVal){
							paraDispBuf[cursor] = KeyBoard[g_u8TouchResult];
							paraDispBuf[cursor+1] = '\0';
							cursor += 1;
							LCD_SetColor(WHITE,BLACK);
							LCD_ShowASC(ASC24x12,120+(cursor-1)*12,18,KeyBoard[g_u8TouchResult]);
						}
					}
				}
			}
			
			if(cursor <= paraTable_p->iParaMaxVal){
				if(language == 0){
					LCD_FillRect(120+(cursor-1)*12,42,12,2,WHITE);
					LCD_FillRect(120+cursor*12,42,12,2,RED);
				}else{
					if((g_u8TouchResult > 0)&&(g_u8TouchResult < 9)){
						LCD_FillRect(120+(cursor-2)*12,42,12,2,WHITE);
						LCD_FillRect(120+cursor*12,42,12,2,RED);
					}else{
						LCD_FillRect(120+(cursor-1)*12,42,12,2,WHITE);
						LCD_FillRect(120+cursor*12,42,12,2,RED);
					}
				}
			}
		}
		
		//大小写或符号
		if(g_u8TouchResult == 43){
			if(language == 0)capsLock = !capsLock;
			else symble = !symble;
			BrushKeyBoard((char*)KeyBoard,language,capsLock,symble);
		}
		//中英文
		if(g_u8TouchResult == 44){
			language = !language;
			capsLock = 0;
			symble = 0;
			//显示数字按键
			if(language == 0){
				pinyinLen = 0;
				memset(pinyinBuf,0,sizeof(pinyinBuf));
				LCD_FillRect(1,50,478,60,diaBkCol);//汉字显示区
//				LCD_FillRect(1,70,478,40,WHITE);//汉字显示区
				for(i=0;i<10;i++) LCD_FillRect(1+i*48,60,46,50,WHITE);//数字
			}else{
				pinyinLen = 0;
				memset(pinyinBuf,0,sizeof(pinyinBuf));
				LCD_FillRect(1,50,478,60,diaBkCol);//汉字显示区
				LCD_FillRect(1,70,478,40,tctlBkCol);//汉字显示区
			}
			BrushKeyBoard((char*)KeyBoard,language,capsLock,symble);
		}
		//退格
		if(g_u8TouchResult == 45){
			if(pinyinLen > 0){
				pinyinLen -= 1;
				pinyinBuf[pinyinLen] = ' ';
				LCD_SetColor(tctlBkCol,tctlTxCol);
				LCD_ShowString(ASC16x8,1,52,0,(char*)pinyinBuf);
				if(pinyinLen == 0){
					matchPageQnt = 0;
					matchPageNum = 0;
					memset(pinyinBuf,0,sizeof(pinyinBuf));
					LCD_FillRect(1,52,478,16,diaBkCol);//汉字显示区
					LCD_FillRect(40,70,400,40,tctlBkCol);//汉字显示区
					LCD_SetColor(tctlBkCol,GRAY);
					LCD_ShowChinese(Font32,8,74,0,"←");
					LCD_ShowChinese(Font32,441,74,0,"→");
				}
			}else{
				if(cursor > 0)cursor -= 1;
				if((paraDispBuf[cursor]&0x80) == 0){
					paraDispBuf[cursor] = 0;
					LCD_SetColor(WHITE,BLACK);
					LCD_ShowString(ASC24x12,120+cursor*12,18,0," ");
					if(cursor < paraTable_p->iParaMaxVal){
						LCD_FillRect(120+(cursor+1)*12,42,12,2,WHITE);
						LCD_FillRect(120+cursor*12,42,12,2,RED);
					}
				}else{
					if(cursor > 0)cursor -= 1;
					paraDispBuf[cursor] = 0;
					paraDispBuf[cursor+1] = 0;
					LCD_SetColor(WHITE,BLACK);
					LCD_ShowString(ASC24x12,120+cursor*12,18,0,"  ");
					if(cursor < paraTable_p->iParaMaxVal){
						LCD_FillRect(120+(cursor+2)*12,42,12,2,WHITE);
						LCD_FillRect(120+cursor*12,42,12,2,RED);
					}
				}
			}
		}
		
		//清空
		if((g_u8TouchResult == 46)||(g_u8TouchResult == IS_BTN_LONGPRESS(45))){
			LCD_FillRect(100,10,294,40,WHITE);//输入框
			cursor = 0;
			memset(paraDispBuf,0x0,sizeof(paraDispBuf));
			LCD_FillRect(120+cursor*12,42,12,2,RED);
			
			pinyinLen = 0;
			memset(pinyinBuf,0,sizeof(pinyinBuf));
			if(language == 1){
				LCD_FillRect(1,52,478,16,diaBkCol);//汉字显示区
				LCD_FillRect(40,70,400,40,tctlBkCol);//汉字显示区
			}
			
		}
		//确定
		if((g_u8TouchResult == 47)||(g_u16KeyValBuf == KEY_OK)){
			sprintf((char*)paraTable_p->pList->listStr,"%s",paraDispBuf);
			LCD_FillRect(0,0,480,320,reBkCol);//清空弹窗
			return;
		}
		//返回
		if((g_u8TouchResult == 48)||(g_u16KeyValBuf == KEY_BACK)){			
			LCD_FillRect(0,0,480,320,reBkCol);//清空弹窗
			return;
		}
		
//		if(g_u8TouchResult == BTN_INVALID_AREA){//非按钮区域触摸
//			if(Is_DesignArea_Touch(0,0,479,319) == 0){
//				Sleep_ms(50);
//				LCD_FillRect(0,0,480,320,WHITE);//清空弹窗
//				return;
//			}
//		}
	}
}

//参数设置对话框
int NumSetDialog(SYS_PARA_TABLE_STRUCT_P paraTable_p)
{
	u8 i,j,k;
	u8 temp;
	u8 cursor;
	int paraVal;
	char paraDispBuf[8];
	char numBuf[16] = "123456789-0.";
	
	u16 nBkCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		nBkCol = BLACK;
	}else{
		nBkCol = WHITE;
	}
	
	LCD_FillRect(90,40,360,260,LGRAY);//弹窗
	LCD_FillRect(110,52,320,36,WHITE);//输入框
	sprintf(paraDispBuf,"%s：",paraTable_p->sChName);
	LCD_SetColor(WHITE,GRAY);
	LCD_ShowText(Font20,120,60,0,(u8*)paraDispBuf);
//	LCD_ShowText(Font20,120,60,0,paraTable_p->sChName);
//	LCD_ShowChinese(Font20,200,60,0,"：");
	if(paraTable_p->Unit != NULL){
		LCD_ShowString(ASC20x10,420-(strlen(paraTable_p->Unit)*10),60,0,paraTable_p->Unit);
	}
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110+83*3,100,71,40,GRAY);LCD_ShowChinese(Font32,110+83*3+20,100+4,0,"刪");//删除
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110+83*3,100+50,71,40,GRAY);LCD_ShowChinese(Font32,110+83*3+20,100+50+4,0,"迴");//返回
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110,100+50*3,71,40,GRAY);LCD_ShowString(ASC20x10,140,110+50*3,0,"-");//符号-
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110+83*2,100+50*3,71,40,GRAY);LCD_ShowString(ASC20x10,140+83*2+3,110+50*3,0,".");//符号.
	LCD_SetColor(GRAY,ENROD);
	LCD_FillRect(110+83*3,100+50*2,71,90,GRAY);LCD_ShowString(ASC32x16,110+83*3+20,100+50*2+29,0,"OK");//OK
	
	for(i=0;i<4;i++){
		for(j=0;j<3;j++){
			k = i*3+j;
			if((k == 9)||(k == 11)){
				LCD_FillRect(110+j*83,100+i*50,71,40,GRAY);
				LCD_SetColor(GRAY,BLACK);
			}else {
				LCD_FillRect(110+j*83,100+i*50,71,40,WHITE);
				LCD_SetColor(WHITE,BLACK);
			}
			LCD_ShowASC(ASC20x10,140+j*83,110+i*50,numBuf[k]);
		}
	}
	//按钮初始化
	g_u8ButtonNums = 15;
	struct BUTTON SampBtn[15] = {
		//sX,  sY,  eX,  eY,
		{193, 250, 263, 290, 0,0},//0, 数字0
		{110, 100, 180, 140, 0,0},//1, 数字1
		{193, 100, 263, 140, 0,0},//2, 数字2
		{276, 100, 346, 140, 0,0},//3, 数字3
		{110, 150, 180, 190, 0,0},//4, 数字4
		{193, 150, 263, 190, 0,0},//5, 数字5
		{276, 150, 346, 190, 0,0},//6, 数字6
		{110, 200, 180, 240, 0,0},//7, 数字7
		{193, 200, 263, 240, 0,0},//8, 数字8
		{276, 200, 346, 240, 0,0},//9, 数字9
		{110, 250, 180, 290, 0,0},//10, 符号-
		{276, 250, 346, 290, 0,0},//11, 符号.
		{359, 100, 429, 140, 0,0},//12, 退格
		{359, 150, 429, 190, 0,0},//13, 返回
		{359, 200, 429, 290, 0,0} //14, OK
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	//参数显示
	cursor = 0;
	memset(paraDispBuf,0,sizeof(paraDispBuf));
	if(paraTable_p->ucParaType == TYP_VALUE_PASSWORD){
		sprintf(paraDispBuf," ");	
	}else{
		if(*paraTable_p->ParaAddr == 0){
			sprintf(paraDispBuf,"  ");
		}else {
			sprintf(paraDispBuf,"%d",*paraTable_p->ParaAddr);
			cursor = strlen(paraDispBuf);
		}
	}
	LCD_SetColor(WHITE,BLACK);
	LCD_ShowString(ASC24x12,220,58,0,paraDispBuf);
//	LCD_FillRect(190,82,12,2,RED);
	LCD_FillRect(220+cursor*12,82,12,2,RED);

	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_AllButtonReaction(10,WHITE,GOLD);//字符键
		ui_OneButtonReaction(10,GRAY,GOLD);//符号
		ui_OneButtonReaction(11,GRAY,GOLD);//符号
		ui_OneButtonReaction(12,GRAY,GOLD);//退格
		ui_OneButtonReaction(13,GRAY,GOLD);//返回
		ui_OneButtonReaction(14,GRAY,GOLD);//OK
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			LCD_FillRect(90,40,360,260,nBkCol);//清空弹窗
			return 0;
		}
		if(g_u16KeyValBuf == KEY_FN){

		}
		
		//OK
		if(g_u8TouchResult == 14){
			paraVal = atoi(paraDispBuf);
			if(paraVal < paraTable_p->iParaMinVal)paraVal = paraTable_p->iParaMinVal;
			if(paraTable_p->ucParaType != TYP_VALUE_PASSWORD){
				*paraTable_p->ParaAddr = paraVal;
				SaveSysPara();
			}
			LCD_FillRect(90,40,360,260,nBkCol);//清空弹窗
			return paraVal;
		}
		//返回
		if(g_u8TouchResult == 13){
			LCD_FillRect(90,40,360,260,nBkCol);//清空弹窗
			return 0;
		}
		//退格
		if(g_u8TouchResult == 12){
			if(cursor > 0)cursor -= 1;
			paraDispBuf[cursor] = ' ';
//			paraVal = atoi(paraDispBuf);
			LCD_ShowString(ASC24x12,220,58,0,paraDispBuf);
			LCD_FillRect(220+(cursor+1)*12,82,12,2,WHITE);
			LCD_FillRect(220+cursor*12,82,12,2,RED);
		}
		//退格长按
		if(g_u8TouchResult == IS_BTN_LONGPRESS(12)){
			LCD_FillRect(220,52,210,36,WHITE);//输入框
			LCD_FillRect(220,82,12,2,RED);
			cursor = 0;
			memset(paraDispBuf,0,sizeof(paraDispBuf));
		}

		//数字0-11
		if(g_u8TouchResult <= 11){
			temp = paraDispBuf[cursor];
			if(g_u8TouchResult == 10)paraDispBuf[cursor] = '-';
			else if(g_u8TouchResult == 11)paraDispBuf[cursor] = '.';
			else paraDispBuf[cursor] = g_u8TouchResult+'0';
			paraVal = atoi(paraDispBuf);
			if(paraVal > paraTable_p->iParaMaxVal){
				paraDispBuf[cursor] = temp;
			}else{
				if(cursor < 6)cursor += 1;
				else paraDispBuf[cursor] = 0;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowString(ASC24x12,220,58,0,paraDispBuf);
				LCD_FillRect(220+(cursor-1)*12,82,12,2,WHITE);
				LCD_FillRect(220+cursor*12,82,12,2,RED);
			}
		}
		
		if(g_u8TouchResult == BTN_INVALID_AREA){//非按钮区域触摸
			if(Is_DesignArea_Touch(90,40,449,299) == 0){
				LCD_FillRect(90,40,360,260,nBkCol);//清空弹窗
				return 0;
			}
		}
	}
}
//数字输入对话框
int iNumInputDialog(int* iNumAddr)
{
	u8 i,j,k;
//	u8 temp;
	u8 cursor;
	int iParaVal;
	char paraDispBuf[8];
	char numBuf[16] = "123456789-0.";
	
	u16 inBkCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		inBkCol = BLACK;
	}else{
		inBkCol = WHITE;
	}
	
	LCD_FillRect(90,40,360,260,LGRAY);//弹窗
	LCD_FillRect(110,52,320,36,WHITE);//输入框
	LCD_SetColor(WHITE,GRAY);
	LCD_ShowChinese(Font20,120,60,0,"参数设置");
	LCD_ShowChinese(Font20,200,60,0,"：");
	
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110+83*3,100,71,40,GRAY);LCD_ShowChinese(Font32,110+83*3+20,100+4,0,"刪");//删除
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110+83*3,100+50,71,40,GRAY);LCD_ShowChinese(Font32,110+83*3+20,100+50+4,0,"迴");//返回
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110,100+50*3,71,40,GRAY);LCD_ShowString(ASC20x10,110,110+50*3,0,"-");//符号-
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110+83*2,100+50*3,71,40,GRAY);LCD_ShowString(ASC20x10,110+83*2+3,110+50*3,0,".");//符号.
	LCD_SetColor(GRAY,ENROD);
	LCD_FillRect(110+83*3,100+50*2,71,90,GRAY);LCD_ShowString(ASC32x16,110+83*3+20,100+50*2+29,0,"OK");//OK
	
	for(i=0;i<4;i++){
		for(j=0;j<3;j++){
			k = i*3+j;
			if((k == 9)||(k == 11)){
				LCD_FillRect(110+j*83,100+i*50,71,40,GRAY);
				LCD_SetColor(GRAY,BLACK);
			}else {
				LCD_FillRect(110+j*83,100+i*50,71,40,WHITE);
				LCD_SetColor(WHITE,BLACK);
			}
			LCD_ShowASC(ASC20x10,140+j*83,110+i*50,numBuf[k]);
		}
	}
	//按钮初始化
	g_u8ButtonNums = 15;
	struct BUTTON SampBtn[15] = {
		//sX,  sY,  eX,  eY,
		{193, 250, 263, 290, 0,0},//0, 数字0
		{110, 100, 180, 140, 0,0},//1, 数字1
		{193, 100, 263, 140, 0,0},//2, 数字2
		{276, 100, 346, 140, 0,0},//3, 数字3
		{110, 150, 180, 190, 0,0},//4, 数字4
		{193, 150, 263, 190, 0,0},//5, 数字5
		{276, 150, 346, 190, 0,0},//6, 数字6
		{110, 200, 180, 240, 0,0},//7, 数字7
		{193, 200, 263, 240, 0,0},//8, 数字8
		{276, 200, 346, 240, 0,0},//9, 数字9
		{110, 250, 180, 290, 0,0},//10, 符号-
		{276, 250, 346, 290, 0,0},//11, 符号.
		{359, 100, 429, 140, 0,0},//12, 退格
		{359, 150, 429, 190, 0,0},//13, 返回
		{359, 200, 429, 290, 0,0} //14, OK
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	//参数显示
	cursor = 0;
	memset(paraDispBuf,0,sizeof(paraDispBuf));

	if(*iNumAddr == 0){
		sprintf(paraDispBuf,"  ");
	}else {
		sprintf(paraDispBuf,"%d",*iNumAddr);
		cursor = strlen(paraDispBuf);
	}
	LCD_SetColor(WHITE,BLACK);
	LCD_ShowString(ASC24x12,220,58,0,paraDispBuf);
//	LCD_FillRect(190,82,12,2,RED);
	LCD_FillRect(220+cursor*12,82,12,2,RED);

	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_AllButtonReaction(10,WHITE,GOLD);//字符键
		ui_OneButtonReaction(10,GRAY,GOLD);//符号
		ui_OneButtonReaction(11,GRAY,GOLD);//符号
		ui_OneButtonReaction(12,GRAY,GOLD);//退格
		ui_OneButtonReaction(13,GRAY,GOLD);//返回
		ui_OneButtonReaction(14,GRAY,GOLD);//OK
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			LCD_FillRect(90,40,360,260,inBkCol);//清空弹窗
			return 0;
		}
		if(g_u16KeyValBuf == KEY_FN){

		}
		
		//OK
		if(g_u8TouchResult == 14){
			iParaVal = atoi(paraDispBuf);
			*iNumAddr = iParaVal;
			LCD_FillRect(90,40,360,260,inBkCol);//清空弹窗
			return iParaVal;
		}
		//返回
		if(g_u8TouchResult == 13){
			LCD_FillRect(90,40,360,260,inBkCol);//清空弹窗
			return 0;
		}
		//退格
		if(g_u8TouchResult == 12){
			if(cursor > 0)cursor -= 1;
			paraDispBuf[cursor] = ' ';
//			paraVal = atoi(paraDispBuf);
			LCD_ShowString(ASC24x12,220,58,0,paraDispBuf);
//			if(cursor < paraTable_p->iParaMaxVal-1){
				LCD_FillRect(220+(cursor+1)*12,82,12,2,WHITE);
				LCD_FillRect(220+cursor*12,82,12,2,RED);
//			}
		}
		//退格长按
		if(g_u8TouchResult == IS_BTN_LONGPRESS(12)){
			LCD_FillRect(220,52,210,36,WHITE);//输入框
			LCD_FillRect(220,82,12,2,RED);
			cursor = 0;
			memset(paraDispBuf,0,sizeof(paraDispBuf));
		}

		//数字0-11
		if(g_u8TouchResult <= 11){
//			temp = paraDispBuf[cursor];
			if(g_u8TouchResult == 10)paraDispBuf[cursor] = '-';
			else if(g_u8TouchResult == 11)paraDispBuf[cursor] = '.';
			else paraDispBuf[cursor] = g_u8TouchResult+'0';
			iParaVal = atoi(paraDispBuf);

			if(cursor < 6)cursor += 1;
			else paraDispBuf[cursor] = 0;
			LCD_SetColor(WHITE,BLACK);
			LCD_ShowString(ASC24x12,220,58,0,paraDispBuf);
			LCD_FillRect(220+(cursor-1)*12,82,12,2,WHITE);
			LCD_FillRect(220+cursor*12,82,12,2,RED);
		}
		
		if(g_u8TouchResult == BTN_INVALID_AREA){//非按钮区域触摸
			if(Is_DesignArea_Touch(90,40,449,299) == 0){
				LCD_FillRect(90,40,360,260,inBkCol);//清空弹窗
				ClearTouchSta();
				return 0;
			}
		}
	}
}
//数字输入对话框
float fNumInputDialog(float fNum,u8 dicimal)
{
	u8 i,j,k;
//	u8 temp;
	u8 cursor;
	float fParaVal;
	char paraDispBuf[8];
	char numBuf[16] = "123456789-0.";
	
	u16 fnBkCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		fnBkCol = BLACK;
	}else{
		fnBkCol = WHITE;
	}
	
	LCD_FillRect(60,40,360,260,LGRAY);//弹窗
	LCD_FillRect(80,52,320,36,WHITE);//输入框
	LCD_SetColor(WHITE,GRAY);
	LCD_ShowChinese(Font20,90,60,0,"参数设置");
	LCD_ShowChinese(Font20,170,60,0,"：");
	
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(80+83*3,100,71,40,GRAY);LCD_ShowChinese(Font32,80+83*3+20,100+4,0,"刪");//删除
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(80+83*3,100+50,71,40,GRAY);LCD_ShowChinese(Font32,80+83*3+20,100+50+4,0,"迴");//返回
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(80,100+50*3,71,40,GRAY);LCD_ShowString(ASC20x10,110,110+50*3,0,"-");//符号-
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(80+83*2,100+50*3,71,40,GRAY);LCD_ShowString(ASC20x10,110+83*2+3,110+50*3,0,".");//符号.
	LCD_SetColor(GRAY,ENROD);
	LCD_FillRect(80+83*3,100+50*2,71,90,GRAY);LCD_ShowString(ASC32x16,80+83*3+20,100+50*2+29,0,"OK");//OK
	
	for(i=0;i<4;i++){
		for(j=0;j<3;j++){
			k = i*3+j;
			if((k == 9)||(k == 11)){
				LCD_FillRect(80+j*83,100+i*50,71,40,GRAY);
				LCD_SetColor(GRAY,BLACK);
			}else {
				LCD_FillRect(80+j*83,100+i*50,71,40,WHITE);
				LCD_SetColor(WHITE,BLACK);
			}
			LCD_ShowASC(ASC20x10,110+j*83,110+i*50,numBuf[k]);
		}
	}
	//按钮初始化
	g_u8ButtonNums = 15;
	struct BUTTON SampBtn[15] = {
		//sX,  sY,  eX,  eY,
		{163, 250, 233, 290, 0,0},//0, 数字0
		{ 80, 100, 150, 140, 0,0},//1, 数字1
		{163, 100, 233, 140, 0,0},//2, 数字2
		{246, 100, 316, 140, 0,0},//3, 数字3
		{ 80, 150, 150, 190, 0,0},//4, 数字4
		{163, 150, 233, 190, 0,0},//5, 数字5
		{246, 150, 316, 190, 0,0},//6, 数字6
		{ 80, 200, 150, 240, 0,0},//7, 数字7
		{163, 200, 233, 240, 0,0},//8, 数字8
		{246, 200, 316, 240, 0,0},//9, 数字9
		{ 80, 250, 150, 290, 0,0},//10, 符号-
		{246, 250, 316, 290, 0,0},//11, 符号.
		{329, 100, 399, 140, 0,0},//12, 退格
		{329, 150, 399, 190, 0,0},//13, 返回
		{329, 200, 399, 290, 0,0} //14, OK
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	//参数显示
	cursor = 0;
	memset(paraDispBuf,0,sizeof(paraDispBuf));

//	if(fNumAddr == 0){
//		sprintf(paraDispBuf,"  ");
//	}else {
		sprintf(paraDispBuf,"%.*f",dicimal,fNum);
		cursor = strlen(paraDispBuf);
//	}
	LCD_SetColor(WHITE,BLACK);
	LCD_ShowString(ASC24x12,190,58,0,paraDispBuf);
//	LCD_FillRect(190,82,12,2,RED);
	LCD_FillRect(190+cursor*12,82,12,2,RED);

	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_AllButtonReaction(10,WHITE,GOLD);//字符键
		ui_OneButtonReaction(10,GRAY,GOLD);//符号
		ui_OneButtonReaction(11,GRAY,GOLD);//符号
		ui_OneButtonReaction(12,GRAY,GOLD);//退格
		ui_OneButtonReaction(13,GRAY,GOLD);//返回
		ui_OneButtonReaction(14,GRAY,GOLD);//OK
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			fParaVal = fNum;
			LCD_FillRect(60,40,360,260,fnBkCol);//清空弹窗
			return fParaVal;
		}
		if(g_u16KeyValBuf == KEY_FN){

		}
		
		//OK
		if(g_u8TouchResult == 14){
			fParaVal = atof(paraDispBuf);
			LCD_FillRect(60,40,360,260,fnBkCol);//清空弹窗
			return fParaVal;
		}
		//返回
		if(g_u8TouchResult == 13){
			fParaVal = fNum;
			LCD_FillRect(60,40,360,260,fnBkCol);//清空弹窗
			return fParaVal;
		}
		//退格
		if(g_u8TouchResult == 12){
			if(cursor > 0)cursor -= 1;
			paraDispBuf[cursor] = ' ';
//			paraVal = atoi(paraDispBuf);
			LCD_ShowString(ASC24x12,190,58,0,paraDispBuf);
//			if(cursor < paraTable_p->iParaMaxVal-1){
				LCD_FillRect(190+(cursor+1)*12,82,12,2,WHITE);
				LCD_FillRect(190+cursor*12,82,12,2,RED);
//			}
		}
		//退格长按
		if(g_u8TouchResult == IS_BTN_LONGPRESS(12)){
			LCD_FillRect(190,52,210,36,WHITE);//输入框
			LCD_FillRect(190,82,12,2,RED);
			cursor = 0;
			memset(paraDispBuf,0,sizeof(paraDispBuf));
		}

		//数字0-11
		if(g_u8TouchResult <= 11){
//			temp = paraDispBuf[cursor];
			if(g_u8TouchResult == 10)paraDispBuf[cursor] = '-';
			else if(g_u8TouchResult == 11)paraDispBuf[cursor] = '.';
			else paraDispBuf[cursor] = g_u8TouchResult+'0';
			fParaVal = atof(paraDispBuf);

			if(cursor < 6)cursor += 1;
			else paraDispBuf[cursor] = 0;
			LCD_SetColor(WHITE,BLACK);
			LCD_ShowString(ASC24x12,190,58,0,paraDispBuf);
			LCD_FillRect(190+(cursor-1)*12,82,12,2,WHITE);
			LCD_FillRect(190+cursor*12,82,12,2,RED);
		}
		
		if(g_u8TouchResult == BTN_INVALID_AREA){//非按钮区域触摸
			if(Is_DesignArea_Touch(60,40,419,299) == 0){
				fParaVal = fNum;
				LCD_FillRect(60,40,360,260,fnBkCol);//清空弹窗
				return fParaVal;
			}
		}
	}
}

//选择参数弹窗
void SelectDialog(SYS_PARA_TABLE_STRUCT_P paraTable_p,u8 optNums)
{
	u8 i,j,k;
	u16 x,y;
	
	u16 slBkCol;
	u16 slTxCol;
	u16 slBarCol;
	u16 slDioCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		slBkCol = BLACK;
		slTxCol = WHITE;
		slBarCol = GRAY;
		slDioCol = DGRAY;
	}else{
		slBkCol = WHITE;
		slTxCol = BLACK;
		slBarCol = DGRAY;
		slDioCol = LGRAY;
	}
	
	//清除背景
	LCD_FillRect(60,56,40,296,slBkCol);//左
	LCD_FillRect(440,56,40-1,296,slBkCol);//右
	LCD_FillRect(60,32,420,24,slBkCol);//上
	LCD_FillRect(60,296,420,24-1,slBkCol);//下
	
	LCD_FillRect(100,56,340,36,slBarCol);//标题栏
	LCD_SetColor(slBarCol,WHITE);
	LCD_ShowChinese(Font16,238,66,0,paraTable_p->sChName);
	
	LCD_FillRect(100,92,340,204,slDioCol);//弹窗
	LCD_FillRect(100,92,340,1,slBkCol);//分割线
	
	if(optNums == 2){//2个按钮
		for(i=0;i<optNums;i++){
			if(i == *paraTable_p->ParaAddr){
				LCD_FillRect(130+160*i,175,120,40,GOLD);
				LCD_SetColor(GOLD,BLACK);
			}else{
				LCD_FillRect(130+160*i,175,120,40,GRAY);//
				LCD_SetColor(GRAY,slTxCol);
			}
//			if(paraTable_p->ucParaType == TYP_LIST_HANZI)
//				LCD_ShowText_Center(Font16,130+160*i,175,120,40,0,paraTable_p->pList[i].listStr);
//			if(paraTable_p->ucParaType == TYP_LIST_ASIC)
				LCD_ShowText_Center(Font16,130+160*i,175,120,40,0,(char*)paraTable_p->pList[i].listStr);
		}
//		LCD_SetColor(GOLD,BLACK);	
//		LCD_ShowChinese(Font20,x1,178,0,paraTable_p->pList[0].listStr);
//		LCD_SetColor(WHITE,BLACK);
//		LCD_ShowChinese(Font20,x2,178,0,paraTable_p->pList[1].listStr);
		//按钮初始化
		g_u8ButtonNums = 2;
		struct BUTTON SampBtn[2] = {
			//sX,  sY,  eX,  eY,
			{130, 175, 249, 214, 0,0},//0,
			{290, 175, 409, 214, 0,0} //1,
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}else if(optNums == 3){//3个按钮
		for(i=0;i<optNums;i++){
			if(i == *paraTable_p->ParaAddr){
				LCD_FillRect(210,115+60*i,120,40,GOLD);
				LCD_SetColor(GOLD,BLACK);
			}else{
				LCD_FillRect(210,115+60*i,120,40,GRAY);//
				LCD_SetColor(GRAY,slTxCol);
			}
//			if(paraTable_p->ucParaType == TYP_LIST_HANZI)
//				LCD_ShowText_Center(Font16,210,115+60*i,120,40,0,paraTable_p->pList[i].listStr);
//			if(paraTable_p->ucParaType == TYP_LIST_ASIC)
				LCD_ShowText_Center(Font16,210,115+60*i,120,40,0,(char*)paraTable_p->pList[i].listStr);
		}
//		LCD_SetColor(GOLD,BLACK);
//		LCD_ShowChinese(Font20,x3,109,0,paraTable_p->pList[0].listStr);
//		LCD_SetColor(WHITE,BLACK);
//		LCD_ShowChinese(Font20,x3,179,0,paraTable_p->pList[1].listStr);
//		LCD_ShowChinese(Font20,x3,249,0,paraTable_p->pList[2].listStr);
		
		//按钮初始化
		g_u8ButtonNums = 3;
		struct BUTTON SampBtn[3] = {
			//sX,  sY,  eX,  eY,
			{210, 115, 329, 154, 0,0},//0,
			{210, 175, 329, 214, 0,0},//1,
			{210, 235, 329, 274, 0,0} //2,
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
		
	}else if(optNums == 4){//4个按钮
		k=0;
		for(i=0;i<2;i++){
			y = 140+70*i;
			for(j=0;j<2;j++){
				x = 130+160*j;
				if(*paraTable_p->ParaAddr == k){
					LCD_FillRect(x,y,120,40,GOLD);
					LCD_SetColor(GOLD,BLACK);
				}else {
					LCD_FillRect(x,y,120,40,GRAY);
					LCD_SetColor(GRAY,slTxCol);
				}
//				if(paraTable_p->ucParaType == TYP_LIST_HANZI)
//					LCD_ShowText_Center(Font16,x,y,120,40,0,paraTable_p->pList[k].listStr);
//				if(paraTable_p->ucParaType == TYP_LIST_ASIC)
					LCD_ShowText_Center(Font16,x,y,120,40,0,(char*)paraTable_p->pList[k].listStr);
				k++;
			}
		}

		//按钮初始化
		g_u8ButtonNums = 4;
		struct BUTTON SampBtn[4] = {
			//sX,  sY,  eX,  eY,
			{130, 140, 249, 177, 0,0},//0,
			{290, 140, 409, 177, 0,0},//1,
			{130, 210, 249, 249, 0,0},//2,
			{290, 210, 409, 249, 0,0},//3,
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}else if(optNums == 5){//5个按钮
		k=0;
		for(i=0;i<3;i++){
			y = 115+60*i;
			for(j=0;j<2;j++){
				if(k == 5)break;
				x = 130+160*j;
				if(*paraTable_p->ParaAddr == k){
					LCD_FillRect(x,y,120,40,GOLD);
					LCD_SetColor(GOLD,BLACK);
				}else {
					LCD_FillRect(x,y,120,40,GRAY);
					LCD_SetColor(GRAY,slTxCol);
				}
//				if(paraTable_p->ucParaType == TYP_LIST_HANZI)
//					LCD_ShowText_Center(Font16,x,y,120,40,0,paraTable_p->pList[k].listStr);
//				if(paraTable_p->ucParaType == TYP_LIST_ASIC)
					LCD_ShowText_Center(Font16,x,y,120,40,0,(char*)paraTable_p->pList[k].listStr);
				k++;
			}
		}
		
		//按钮初始化
		g_u8ButtonNums = 5;
		struct BUTTON SampBtn[5] = {
			//sX,  sY,  eX,  eY,
			{130, 115, 249, 154, 0,0},//0,
			{290, 115, 409, 154, 0,0},//1,
			{130, 175, 249, 214, 0,0},//2,
			{290, 175, 409, 214, 0,0},//3,
			{130, 225, 249, 264, 0,0} //4,
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
		
	}else if(optNums == 6){//6个按钮
		k=0;
		for(i=0;i<3;i++){
			y = 115+60*i;
			for(j=0;j<2;j++){
//				if(k == 5)break;
				x = 130+160*j;
				if(*paraTable_p->ParaAddr == k){
					LCD_FillRect(x,y,120,40,GOLD);
					LCD_SetColor(GOLD,BLACK);
				}else {
					LCD_FillRect(x,y,120,40,GRAY);
					LCD_SetColor(GRAY,slTxCol);
				}
//				if(paraTable_p->ucParaType == TYP_LIST_HANZI)
//					LCD_ShowText_Center(Font16,x,y,120,40,0,paraTable_p->pList[k].listStr);
//				if(paraTable_p->ucParaType == TYP_LIST_ASIC)
					LCD_ShowText_Center(Font16,x,y,120,40,0,(char*)paraTable_p->pList[k].listStr);
				k++;
			}
		}
		
		//按钮初始化
		g_u8ButtonNums = 6;
		struct BUTTON SampBtn[6] = {
			//sX,  sY,  eX,  eY,
			{130, 115, 249, 154, 0,0},//0,
			{290, 115, 409, 154, 0,0},//1,
			{130, 175, 249, 214, 0,0},//2,
			{290, 175, 409, 214, 0,0},//3,
			{130, 225, 249, 264, 0,0},//4,
			{290, 225, 409, 264, 0,0} //5,
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
		
	}else if(optNums > 6){
		k=0;
		for(i=0;i<3;i++){
			y = 115+60*i;
			for(j=0;j<2;j++){
//				if(k == 5)break;
				x = 130+160*j;
				if(*paraTable_p->ParaAddr == k){
					LCD_FillRect(x,y,120,40,GOLD);
					LCD_SetColor(GOLD,BLACK);
				}else {
					LCD_FillRect(x,y,120,40,GRAY);
					LCD_SetColor(GRAY,slTxCol);
				}
//				if(paraTable_p->ucParaType == TYP_LIST_HANZI)
//					LCD_ShowText_Center(Font16,x,y,120,40,0,paraTable_p->pList[k].listStr);
//				if(paraTable_p->ucParaType == TYP_LIST_ASIC)
					LCD_ShowText_Center(Font16,x,y,120,40,0,(char*)paraTable_p->pList[k].listStr);
				k++;
			}
		}
		
		//按钮初始化
		g_u8ButtonNums = 6;
		struct BUTTON SampBtn[6] = {
			//sX,  sY,  eX,  eY,
			{130, 115, 249, 154, 0,0},//0,
			{290, 115, 409, 154, 0,0},//1,
			{130, 175, 249, 214, 0,0},//2,
			{290, 175, 409, 214, 0,0},//3,
			{130, 225, 249, 264, 0,0},//4,
			{290, 225, 409, 264, 0,0} //5,
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_OneButtonReaction(g_u8ButtonNums-1,BLACK,GOLD);//字符键
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			LCD_FillRect(100,56,340,240,slBkCol);//清空弹窗
			return;
		}
		if(g_u16KeyValBuf == KEY_FN){

		}
		
		if(g_u8TouchResult < optNums){//点击按钮
			*paraTable_p->ParaAddr = g_u8TouchResult;
			LCD_FillRect(100,56,340,240,slBkCol);//清空弹窗
			return;
		}
		if(g_u8TouchResult == BTN_INVALID_AREA){//非按钮区域触摸
			if(Is_DesignArea_Touch(90,40,449,299) == 0){
				LCD_FillRect(100,56,340,240,slBkCol);//清空弹窗
				return;
			}
		}
	}
}
//mode:0--date	1--time
void TimeSetDialog(u8 mode)//SYS_PARA_TABLE_STRUCT_P paraTable_p
{
	u8 i;
	u16 x,y;
	u16 year;
	short month,day,hour,min,sec;
	const int calendarTab[13] = {29,31,28,31,30,31,60,31,31,30,31,30,31};
	int dayTemp;
	
	u16 tmBkCol;
	u16 tmTxCol;
	u16 tmBarCol;
	u16 tmDioCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		tmBkCol = BLACK;
		tmTxCol = WHITE;
		tmBarCol = GRAY;
		tmDioCol = DGRAY;
	}else{
		tmBkCol = WHITE;
		tmTxCol = BLACK;
		tmBarCol = DGRAY;
		tmDioCol = LGRAY;
	}
	
	//清除背景
	LCD_FillRect(60,56,40,296,tmBkCol);//左
	LCD_FillRect(440,56,40-1,296,tmBkCol);//右
	LCD_FillRect(60,32,420,24,tmBkCol);//上
	LCD_FillRect(60,296,420,24-1,tmBkCol);//下
	
	//标题栏
	LCD_FillRect(100,56,340,36,tmBarCol);
	LCD_SetColor(tmBarCol,WHITE);
	if(mode == 0) LCD_ShowChinese(Font16,238,66,0,"系统日期");
	else LCD_ShowChinese(Font16,238,66,0,"系统时间");
	//弹窗
	LCD_FillRect(100,92,340,168,tmDioCol);//弹窗
	LCD_FillRect(100,92,340,1,tmBkCol);//分割线
	//按钮
	LCD_FillRect(100,260,170,36,GRAY);//确定按钮
	LCD_FillRect(270,260,170,36,GRAY);//取消按钮
	LCD_FillRect(100,259,340,1,tmBkCol);//上分割线
	LCD_FillRect(269,260,1,36,tmBkCol);//中分割线
	
	LCD_SetColor(GRAY,tmTxCol);
	LCD_ShowChinese(Font16,168,270,0,"确定");
	LCD_SetColor(GRAY,tmTxCol);
	LCD_ShowChinese(Font16,338,270,0,"取消");
	
	
	for(i=0;i<3;i++){
		x = 120+i*105;
		y = 116;
		LCD_FillRect(x,y,90,36,GRAY);//+
		LCD_FillRect(x,y+36+1,90,48-2,WHITE);//数据
		LCD_FillRect(x,y+84,90,36,GRAY);//—
//		LCD_DrawRect(x,100,90,120,GRAY);//边框
		LCD_SetColor(GRAY,tmTxCol);
		LCD_ShowString(ASC32x16,x+37,y+2,0,"+");
		LCD_ShowString(ASC32x16,x+37,y+86,0,"-");
	}
	
	if(mode == 0){
		LCD_SetColor(WHITE,GRAY);
		LCD_ShowChinese(Font20,180,167,0,"年");
		LCD_ShowChinese(Font20,285,167,0,"月");
		LCD_ShowChinese(Font20,390,167,0,"日");
		
		year = g_SysTime.year;
		month = g_SysTime.month;
		day = g_SysTime.day;
		
		LCD_SetColor(WHITE,BLACK);
		LCD_ShowNnNums(ASC20x10,130,166,0,4,year);
		LCD_ShowNnNums(ASC20x10,250,166,0,2,month);
		LCD_ShowNnNums(ASC20x10,355,166,0,2,day);
	}else{
		LCD_SetColor(WHITE,GRAY);
		LCD_ShowChinese(Font20,180,167,0,"时");
		LCD_ShowChinese(Font20,285,167,0,"分");
		LCD_ShowChinese(Font20,390,167,0,"秒");
		
		hour = g_SysTime.hour;
		min = g_SysTime.min;
		sec = g_SysTime.sec;
		
		LCD_SetColor(WHITE,BLACK);
		LCD_ShowNnNums(ASC20x10,145,166,0,2,hour);
		LCD_ShowNnNums(ASC20x10,250,166,0,2,min);
		LCD_ShowNnNums(ASC20x10,355,166,0,2,sec);
	}
	
	//按钮初始化
	g_u8ButtonNums = 8;
	struct BUTTON SampBtn[8] = {
		//sX,  sY,  eX,  eY,
		{120, 116, 209, 151, 0,0},//0, +1
		{120, 200, 209, 235, 0,0},//1, -1
		{225, 116, 314, 151, 0,0},//2, +2
		{225, 200, 314, 235, 0,0},//3, -2
		{330, 116, 419, 151, 0,0},//4, +3
		{330, 200, 419, 235, 0,0},//5, -3
		{100, 260, 269, 295, 0,0},//6, 确定
		{270, 260, 439, 295, 0,0},//7, 取消
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_OneButtonReaction(6,GRAY,GOLD);//确定
		ui_OneButtonReaction(7,GRAY,GOLD);//确定
		
		if(g_u8TouchResult == 0){//+1
			if(mode == 0){//日期
				year += 1;
				if(year > 2099)year = 2021;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,130,166,0,4,year);
			}else{//时间
				hour += 1;
				if(hour > 23)hour = 0;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,145,166,0,2,hour);
			}
		}
		if(g_u8TouchResult == 1){//-1
			if(mode == 0){//日期
				year -= 1;
				if(year < 2021)year = 2099;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,130,166,0,4,year);
			}else{//时间
				hour -= 1;
				if(hour < 0)hour = 23;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,145,166,0,2,hour);
			}
		}
		////
		if(g_u8TouchResult == 2){//+2
			if(mode == 0){//日期
				month += 1;
				if(month > 12)month = 1;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,250,166,0,2,month);
			}else{//时间
				min += 1;
				if(min > 59)min = 0;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,250,166,0,2,min);
			}
		}
		if(g_u8TouchResult == 3){//-2
			if(mode == 0){//日期
				month -= 1;
				if(month < 1)month = 12;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,250,166,0,2,month);
			}else{//时间
				min -= 1;
				if(min < 0)min = 59;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,250,166,0,2,min);
			}
		}
		////
		if(g_u8TouchResult == 4){//+3
			if(mode == 0){//日期
				day += 1;
				dayTemp = calendarTab[month];
				if(month == 2){
					if(year%4 == 0){//闰年
						dayTemp = 29;
					}else{
						dayTemp = 28;
					}
				}
				if(day > dayTemp)day = 1;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,355,166,0,2,day);
			}else{//时间
				sec += 1;
				if(sec > 59)sec = 0;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,355,166,0,2,sec);
			}
		}
		if(g_u8TouchResult == 5){//-3
			if(mode == 0){//日期
				day -= 1;
				dayTemp = calendarTab[month];
				if(month == 2){
					if(year%4 == 0){//闰年
						dayTemp = 29;
					}else{
						dayTemp = 28;
					}
				}
				if(day < 1)day = dayTemp;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,355,166,0,2,day);
			}else{//时间
				sec -= 1;
				if(sec < 0)sec = 59;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,355,166,0,2,sec);
			}
		}
		////
		if(g_u8TouchResult == 6){//确定
			LCD_FillRect(100,56,340,240,tmBkCol);//清空弹窗
			if(mode == 0){//日期
				g_SysTime.year = year;
				g_SysTime.month = month;
				g_SysTime.day = day;
			}else{//时间
				g_SysTime.hour = hour;
				g_SysTime.min = min;
				g_SysTime.sec = sec;
			}
			RX8010_SetTime(&g_SysTime);
			return;
		}
		if(g_u8TouchResult == 7){//取消，退出弹窗
			LCD_FillRect(100,56,340,240,tmBkCol);//清空弹窗
			return;
		}
		
		if(g_u8TouchResult == BTN_INVALID_AREA){//非按钮区域触摸
			if(Is_DesignArea_Touch(90,50,449,299) == 0){
				LCD_FillRect(100,56,340,240,tmBkCol);//清空弹窗
				return;
			}
		}
	}
}

//弹窗信息表
struct PROMPTStr promptStr[] = {
	{DIALOGSAVE, 	"数据保存", "保存数据并退出测量？",		NULL,"确定","取消"},
	{DIALOGDELEALL, "数据删除", "删除全部构件？",			NULL,"确定","取消"},
	{DIALOGDELESEL, "数据删除", "删除选中构件？",			NULL,"确定","取消"},
	{DIALOGBATLOW,  "电量提示", "电量不足，请及时充电",		NULL,"确定",NULL},
	{DIALOGBATNONE, "电量提示", "电量过低，无法测量",		NULL,"确定",NULL},
	{DIALOGSTORLOW,	"内存提示", "内存不足，请及时删除数据",	NULL,"确定",NULL},
	{DIALOGSTORNONE,"内存提示", "内存已满，无法存储数据",	NULL,"确定",NULL},
	{DIALOGUSBLINK,	 "USB提示", "USB已连接，准备传输数据",	NULL,NULL,NULL},
	{DIALOGBTLINK,  "蓝牙提示", "蓝牙已连接，准备传输数据",	NULL,NULL,NULL},
	
	{DIALOGGOUJIANEXIST, "提示", "构件已存在，是否覆盖？",	NULL,	"确定","取消"},
	{DIALOGBIAODINGBATLOW, "提示", "电池电量过低",	"请重新标定仪器",	"确定","取消"},
	{DIALOGBIAODINGBATJUMP,"提示", "电池电量变化过大",	"请重新标定仪器",	"确定","取消"},
	{DIALOGBIAODINGTEMPER, "提示", "环境温度变化过大",	"请重新标定仪器",	"确定","取消"},
};

//提示弹窗
u8 PromptDialog(u8 id)
{
	u8 sLen;
	u8 dbuf[32];
	
	u16 pdBkCol;
	u16 pdTxCol;
	u16 pdBarCol;
	u16 pdDioCol;
	u16 pdTemVol;
	u16 pdBtxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		pdBkCol = BLACK;
		pdTxCol = GOLD;
		pdBarCol = GRAY;
		pdDioCol = DGRAY;
		pdTemVol = WHITE;
		pdBtxCol = WHITE;
	}else{
		pdBkCol = WHITE;
		pdTxCol = BLACK;
		pdBarCol = DGRAY;
		pdDioCol = LGRAY;
		pdTemVol = RED;
		pdBtxCol = BLACK;
	}
	
	//标题栏
	LCD_FillRect(100,56,340,36,pdBarCol);
	//弹窗
	LCD_FillRect(100,92,340,168,pdDioCol);//弹窗
	LCD_FillRect(100,92,340,1,pdBkCol);//分割线
	//按钮
//	LCD_FillRect(100,260,170,36,GOLD);//确定按钮
//	LCD_FillRect(270,260,170,36,GRAY);//取消按钮
//	LCD_FillRect(100,259,340,1,fxBkCol);//上分割线
//	LCD_FillRect(269,260,1,36,fxBkCol);//中分割线
	
//	LCD_SetColor(GOLD,BLACK);
//	LCD_ShowChinese(Font16,168,270,0,"确定");
//	LCD_SetColor(GRAY,fxTxCol);
//	LCD_ShowChinese(Font16,338,270,0,"取消");
	
	
	sLen = strlen((char*)promptStr[id].name)/2;
	LCD_SetColor(pdBarCol,WHITE);
	LCD_ShowText(Font16,60+(420-sLen*Font16)/2,66,0,promptStr[id].name);
	
	if(promptStr[id].strWords2 == NULL){
		sLen = strlen((char*)promptStr[id].strWords1)/2;
		LCD_SetColor(pdDioCol,pdTxCol);
		LCD_ShowText(Font20,60+(420-sLen*Font20)/2,160,0,promptStr[id].strWords1);
	}else{
		sLen = strlen((char*)promptStr[id].strWords1)/2;
		LCD_SetColor(pdDioCol,pdTxCol);
		LCD_ShowText(Font20,60+(420-sLen*Font20)/2,140,0,promptStr[id].strWords1);
		
		sLen = strlen((char*)promptStr[id].strWords2)/2;
		LCD_SetColor(pdDioCol,pdTxCol);
		LCD_ShowText(Font20,60+(420-sLen*Font20)/2,170,0,promptStr[id].strWords2);
	}
//	LCD_ShowChinese(Font20,140,150,0,promptStr[id].strWords);
	if(promptStr[id].btnWords1 == NULL){//无按钮
		LCD_FillRect(100,260,340,36,pdDioCol);//补画按钮区域
	}else{
		if(promptStr[id].btnWords2 == NULL){//一个按钮
			//按钮
			LCD_FillRect(100,260,340,36,GRAY);//确定
			LCD_FillRect(100,259,340,1,pdBkCol);//上分割线
//			LCD_FillRect(269,260,1,36,fxBkCol);//中分割线
			
			LCD_SetColor(GRAY,pdBtxCol);
			LCD_ShowText(Font16,254,270,0,promptStr[id].btnWords1);
			
			//按钮初始化
			g_u8ButtonNums = 1;
			struct BUTTON SampBtn[1] = {
				//sX,  sY,  eX,  eY,
				{100, 260, 439, 295, 0,0} //0,
			};
			memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
			
		}else{//两个按钮
			//按钮
			LCD_FillRect(100,260,170,36,GRAY);//确定按钮
			LCD_FillRect(270,260,170,36,GRAY);//取消按钮
			LCD_FillRect(100,259,340,1,pdBkCol);//上分割线
			LCD_FillRect(269,260,1,36,pdBkCol);//中分割线
			
			LCD_SetColor(GRAY,pdBtxCol);
			LCD_ShowText(Font16,168,270,0,promptStr[id].btnWords1);
			LCD_ShowText(Font16,338,270,0,promptStr[id].btnWords2);
			
			if(id == DIALOGBIAODINGTEMPER){
				sprintf((char*)dbuf,"(上次%4.1f°，当前%4.1f°)",g_SysPara.fTemperatureBDLast,g_GlobalPara.fTemperature);
				sLen = strlen((char*)dbuf);
				LCD_SetColor(pdDioCol,pdTemVol);
				LCD_ShowText(Font16,60+(420-sLen*Font16/2)/2,196,0,dbuf);
			}
			
			//按钮初始化
			g_u8ButtonNums = 2;
			struct BUTTON SampBtn[2] = {
				//sX,  sY,  eX,  eY,
				{100, 260, 269, 295, 0,0},//0,
				{270, 260, 439, 295, 0,0} //1,
			};
			memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
		}
	}
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		if(g_u8ButtonNums > 1){
			ui_AllButtonReaction(g_u8ButtonNums,GRAY,GOLD);
		}
		if(promptStr[id].btnWords1 == NULL){//无按钮
			Sleep_ms(2000);
			LCD_FillRect(100,56,340,240,pdBkCol);//清空弹窗
			return 1;
		}else{
			if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_OK)){//确定
				LCD_FillRect(100,56,340,240,pdBkCol);//清空弹窗
				return 1;
			}		
			if((g_u8TouchResult == 1)||(g_u16KeyValBuf ==KEY_BACK)){//取消
				LCD_FillRect(100,56,340,240,pdBkCol);//清空弹窗
				return 0;
			}
		}
		if(g_u8TouchResult == BTN_INVALID_AREA){//非按钮区域触摸
//			if(Is_DesignArea_Touch(90,50,449,299) == 0){
//				LCD_FillRect(100,56,340,240,pdBkCol);//清空弹窗
//				return 0;
//			}
		}
		Sleep_ms(5);
	}
}

