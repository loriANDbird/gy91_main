/*******************************************************************************
�����ߣ���ϲ˫
�������ڣ�2018
����������
    1. ��ȼ�⡢�ֽ�ɨ��ģʽ�����ú���������
    
*******************************************************************************/
#include "ui_Dialog.h"
#include "Config_ui.h"
#include "pyinput.h"

const char KeyBoard[56] = "1234567890qwertyuiopasdfghjklzxcvbnm#-_.~,+(),;^&!";

//ˢ�¼���
static void BrushKeyBoard(char* keyValBuf,u8 language,u8 capsLock,u8 symble)
{
	u8 i;
	
	u16 ctlBkCol;
	u16 ctlTxCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		ctlBkCol = DGRAY;
		ctlTxCol = WHITE;
	}else{
		ctlBkCol = LGRAY;
		ctlTxCol = BLACK;
	}
	
	//��ʾ�ַ�
	if(language == 0){
		for(i=0;i<10;i++){
			LCD_SetColor(WHITE,BLACK);
			LCD_ShowASC(ASC20x10,20+i*48,75,keyValBuf[i]);//����1
		}
	}
	for(i=0;i<10;i++){
		LCD_SetColor(WHITE,BLACK);
		if(language == 0){
			if(capsLock != 0)LCD_ShowASC(ASC20x10,20+i*48,75+52,keyValBuf[i+10]);//��ĸ
			else LCD_ShowASC(ASC20x10,20+i*48,75+52,keyValBuf[i+10]-32);//��ĸ
		}else{
			LCD_ShowASC(ASC20x10,20+i*48,75+52,keyValBuf[i+10]-32);//��ĸ
		}
	}
	for(i=0;i<9;i++){
		LCD_SetColor(WHITE,BLACK);
		if(language == 0){
			if(capsLock != 0)LCD_ShowASC(ASC20x10,44+i*48,75+52*2,keyValBuf[i+20]);//��ĸ
			else LCD_ShowASC(ASC20x10,44+i*48,75+52*2,keyValBuf[i+20]-32);//��ĸ
		}else{
			LCD_ShowASC(ASC20x10,44+i*48,75+52*2,keyValBuf[i+20]-32);//��ĸ
		}
	}
	for(i=0;i<7;i++){
		LCD_SetColor(WHITE,BLACK);
		if(language == 0){
			if(capsLock != 0)LCD_ShowASC(ASC20x10,92+i*48,75+52*3,keyValBuf[i+29]);//��ĸ
			else LCD_ShowASC(ASC20x10,92+i*48,75+52*3,keyValBuf[i+29]-32);//��ĸ
		}else{
			LCD_ShowASC(ASC20x10,92+i*48,75+52*3,keyValBuf[i+29]-32);//��ĸ
		}
	}
	for(i=0;i<7;i++){
		LCD_SetColor(WHITE,BLACK);
		if((language == 1)&&(symble == 1))
			LCD_ShowASC(ASC20x10,92+i*48,75+52*4,keyValBuf[i+36+7]);//�ַ�
		else LCD_ShowASC(ASC20x10,92+i*48,75+52*4,keyValBuf[i+36]);//�ַ�
	}
		
	if(language == 0){//Ӣ��
		LCD_FillRect(1,60+52*3,70,50,ctlBkCol);//��Сд�����
		if(capsLock != 0){//Сд
			LCD_SetColor(ctlBkCol,GRAY);
			LCD_ShowChinese(Font32,20,225,0,"��");
		}else {//��д
			LCD_SetColor(ctlBkCol,ctlTxCol);
			LCD_ShowChinese(Font32,20,225,0,"��");
		}
		LCD_SetColor(ctlBkCol,GRAY);
		LCD_ShowChinese(Font20,32,281,0,"��");
		LCD_ShowChinese(Font16,45,285,0,"��");
		LCD_SetColor(ctlBkCol,ctlTxCol);
		LCD_ShowChinese(Font20,12,281,0,"Ӣ");
	}else {//����
		LCD_FillRect(1,60+52*3,70,50,ctlBkCol);//��Сд�����
		if(symble == 0){//����1
			LCD_SetColor(ctlBkCol,ctlTxCol);
			LCD_ShowChinese(Font20,10,231,0,"����");
			LCD_ShowASC(ASC20x10,50,231,'1');
		}else {//����2
			LCD_SetColor(ctlBkCol,ctlTxCol);
			LCD_ShowChinese(Font20,10,231,0,"����");
			LCD_ShowASC(ASC20x10,50,231,'2');
		}
		LCD_SetColor(ctlBkCol,GRAY);
		LCD_ShowChinese(Font20,32,281,0,"��");
		LCD_ShowChinese(Font16,45,285,0,"Ӣ");
		LCD_SetColor(ctlBkCol,ctlTxCol);
		LCD_ShowChinese(Font20,12,281,0,"��");
		
		LCD_SetColor(ctlBkCol,GRAY);
		LCD_ShowChinese(Font32,8,74,0,"��");
		LCD_ShowChinese(Font32,441,74,0,"��");
	}
	LCD_SetColor(ctlBkCol,ctlTxCol);LCD_ShowChinese(Font32,428,225,0,"�h");		
	LCD_SetColor(ctlBkCol,ctlTxCol);LCD_ShowChinese(Font20,424,283,0,"���");	
}
//�����������öԻ���
void TextSetDialog(SYS_PARA_TABLE_STRUCT_P paraTable_p)
{
	u8 i;
	u8 language = 0;//0��Ӣ�ģ�1������
	u8 capsLock = 0;//0����д��1��Сд
	u8 symble = 0;//0������1��1������2
	
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
	//��ɫ����
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
	
	LCD_FillRect(0,0,480,320,diaBkCol);//����
	LCD_FillRect(100,10,294,40,WHITE);//�����
	LCD_SetColor(diaBkCol,tctlTxCol);
	LCD_ShowChinese(Font20,10,20,0,paraTable_p->sChName);
	
	LCD_SetColor(tctlBkCol,tctlTxCol);
	LCD_FillRect(408,10,64,40,tctlBkCol);//ȷ��
	LCD_ShowChinese(Font20,420,20,0,"ȷ��");

	//��ʾ����
	if(language == 0){
		for(i=0;i<10;i++) LCD_FillRect( 1+i*48, 60,      46, 50,WHITE);//����
	}else{
		LCD_FillRect(1,60,478,50,diaBkCol);//������ʾ��
		LCD_FillRect(1,70,478,40,tctlBkCol);//������ʾ��
		LCD_SetColor(tctlBkCol,GRAY);
		LCD_ShowChinese(Font32,8,74,0,"��");
		LCD_ShowChinese(Font32,441,74,0,"��");
	}
	for(i=0;i<10;i++) LCD_FillRect( 1+i*48, 60+52,   46, 50,WHITE);
	for(i=0;i<9; i++) LCD_FillRect(25+i*48, 60+52*2, 46, 50,WHITE);
	for(i=0;i<7; i++) LCD_FillRect(73+i*48, 60+52*3, 46, 50,WHITE);
	for(i=0;i<7; i++) LCD_FillRect(73+i*48, 60+52*4, 46, 50,WHITE);
	
	LCD_FillRect(1,60+52*3,70,50,tctlBkCol);//��Сд�����
	LCD_FillRect(1,60+52*4,70,50,tctlBkCol);//��Ӣ���л�
	LCD_FillRect(409,60+52*3,70,50,tctlBkCol);//�˸�
	LCD_FillRect(409,60+52*4,70,50,tctlBkCol);//���
	
	BrushKeyBoard((char*)KeyBoard,language,capsLock,symble);
	
	//��ť��ʼ��
	g_u8ButtonNums = 49;
	
	//�ַ�
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
	//��Сд
	g_ButtonBuf[43].sX = 1;   g_ButtonBuf[43].sY = 216; g_ButtonBuf[43].eX = 70;  g_ButtonBuf[43].eY = 265;
	g_ButtonBuf[43].btSta = 0;
	g_ButtonBuf[43].btAbility = 0;
	//��Ӣ��                                                                                                
	g_ButtonBuf[44].sX = 1;   g_ButtonBuf[44].sY = 268; g_ButtonBuf[44].eX = 70;  g_ButtonBuf[44].eY = 317;
	g_ButtonBuf[44].btSta = 0;
	g_ButtonBuf[44].btAbility = 0;
	//�˸�                                                                                                  
	g_ButtonBuf[45].sX = 409; g_ButtonBuf[45].sY = 216; g_ButtonBuf[45].eX = 478; g_ButtonBuf[45].eY = 265;
	g_ButtonBuf[45].btSta = 0;
	g_ButtonBuf[45].btAbility = 0;
	//���                                                                                                  
	g_ButtonBuf[46].sX = 409; g_ButtonBuf[46].sY = 268; g_ButtonBuf[46].eX = 478; g_ButtonBuf[46].eY = 317;
	g_ButtonBuf[46].btSta = 0;
	g_ButtonBuf[46].btAbility = 0;
	//ȷ��                                                                                                  
	g_ButtonBuf[47].sX = 408; g_ButtonBuf[47].sY = 10;  g_ButtonBuf[47].eX = 471; g_ButtonBuf[47].eY = 49;
	g_ButtonBuf[47].btSta = 0;
	g_ButtonBuf[47].btAbility = 0;
	//����                                                                                                  
	g_ButtonBuf[48].sX = 0;   g_ButtonBuf[48].sY = 0;   g_ButtonBuf[48].eX = 79;  g_ButtonBuf[48].eY = 49;
	g_ButtonBuf[48].btSta = 0;
	g_ButtonBuf[48].btAbility = 0;
	
	//������ʾ
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
		ui_AllButtonReaction(43,WHITE,GOLD);//�ַ���
		ui_OneButtonReaction(43,tctlBkCol,GOLD);//��Сд
		ui_OneButtonReaction(44,tctlBkCol,GOLD);//��Ӣ��
		ui_OneButtonReaction(45,tctlBkCol,GOLD);//�˸�
		ui_OneButtonReaction(46,tctlBkCol,GOLD);//���
		ui_OneButtonReaction(47,tctlBkCol,GOLD);//ȷ��
//		ui_OneButtonReaction(48,GOLD,ORANGE);//����
//		ui_OneButtonReactionXY(48,0,0,47,33,GOLD,ORANGE);//����
		
//		if(g_u16KeyValBuf == KEY_OK){

//		}
//		if(g_u16KeyValBuf == KEY_BACK){
//			
//		}
//		if(g_u16KeyValBuf == KEY_FN){

//		}
		
		//�ַ�
		if(g_u8TouchResult < 43){
			if(g_u8TouchResult<10){//����
				if(language == 0){
					if(cursor < paraTable_p->iParaMaxVal){
						paraDispBuf[cursor] = KeyBoard[g_u8TouchResult];
						paraDispBuf[cursor+1] = '\0';
						cursor += 1;
						LCD_SetColor(WHITE,BLACK);
						LCD_ShowASC(ASC24x12,120+(cursor-1)*12,18,KeyBoard[g_u8TouchResult]);
					}
				}else{//����
					if(g_u8TouchResult == 0){//��ҳ
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
							LCD_ShowChinese(Font32,441,74,0,"��");
							if(matchPageNum == 0){
								LCD_SetColor(tctlBkCol,GRAY);
								LCD_ShowChinese(Font32,8,74,0,"��");
							}else{
								LCD_SetColor(tctlBkCol,tctlTxCol);
								LCD_ShowChinese(Font32,8,74,0,"��");
							}
						}else{
							LCD_SetColor(tctlBkCol,GRAY);
							LCD_ShowChinese(Font32,8,74,0,"��");
						}
					}else if(g_u8TouchResult == 9){//�ҷ�ҳ
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
							LCD_ShowChinese(Font32,8,74,0,"��");
							if(matchPageNum == matchPageQnt-1){
								LCD_SetColor(tctlBkCol,GRAY);
								LCD_ShowChinese(Font32,441,74,0,"��");
							}else{
								LCD_SetColor(tctlBkCol,tctlTxCol);
								LCD_ShowChinese(Font32,441,74,0,"��");
							}
						}else{
							LCD_SetColor(tctlBkCol,GRAY);
							LCD_ShowChinese(Font32,441,74,0,"��");
						}
					}else{
						if(pinyinLen > 0){//������
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
								LCD_FillRect(1,52,478,16,diaBkCol);//������ʾ��
								LCD_FillRect(40,70,400,40,tctlBkCol);//������ʾ��
								
								LCD_SetColor(tctlBkCol,GRAY);
								LCD_ShowChinese(Font32,8,74,0,"��");
								LCD_ShowChinese(Font32,441,74,0,"��");
							}
						}
					}
				}
			}else if((g_u8TouchResult>=10)&&(g_u8TouchResult<36)){//��ĸ
				if(language == 0){//Ӣ��
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
				}else {//����
//					paraDispBuf[cursor] = KeyBoard[g_u8TouchResult];
//					cursor += 1;
//					LCD_SetColor(WHITE,BLACK);
//					LCD_ShowASC(ASC24x12,120+(cursor-1)*12,18,KeyBoard[g_u8TouchResult]);
					pinyinBuf[pinyinLen++] = KeyBoard[g_u8TouchResult];
					if(pinyinLen > 40)pinyinLen = 40;
					LCD_SetColor(tctlBkCol,tctlTxCol);
					LCD_ShowString(ASC16x8,1,52,0,(char*)pinyinBuf);
					
					matchNum = g_t26.getpymb(pinyinBuf);
					if((matchNum)||(matchNum&0x80)){//��ƥ��
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
							LCD_ShowChinese(Font32,441,74,0,"��");
						}
					}else{//��ƥ��
						
					}
				}
			}else {//�ַ�
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
					LCD_FillRect(1,52,478,16,diaBkCol);//������ʾ��
					LCD_FillRect(40,70,400,40,tctlBkCol);//������ʾ��
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
		
		//��Сд�����
		if(g_u8TouchResult == 43){
			if(language == 0)capsLock = !capsLock;
			else symble = !symble;
			BrushKeyBoard((char*)KeyBoard,language,capsLock,symble);
		}
		//��Ӣ��
		if(g_u8TouchResult == 44){
			language = !language;
			capsLock = 0;
			symble = 0;
			//��ʾ���ְ���
			if(language == 0){
				pinyinLen = 0;
				memset(pinyinBuf,0,sizeof(pinyinBuf));
				LCD_FillRect(1,50,478,60,diaBkCol);//������ʾ��
//				LCD_FillRect(1,70,478,40,WHITE);//������ʾ��
				for(i=0;i<10;i++) LCD_FillRect(1+i*48,60,46,50,WHITE);//����
			}else{
				pinyinLen = 0;
				memset(pinyinBuf,0,sizeof(pinyinBuf));
				LCD_FillRect(1,50,478,60,diaBkCol);//������ʾ��
				LCD_FillRect(1,70,478,40,tctlBkCol);//������ʾ��
			}
			BrushKeyBoard((char*)KeyBoard,language,capsLock,symble);
		}
		//�˸�
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
					LCD_FillRect(1,52,478,16,diaBkCol);//������ʾ��
					LCD_FillRect(40,70,400,40,tctlBkCol);//������ʾ��
					LCD_SetColor(tctlBkCol,GRAY);
					LCD_ShowChinese(Font32,8,74,0,"��");
					LCD_ShowChinese(Font32,441,74,0,"��");
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
		
		//���
		if((g_u8TouchResult == 46)||(g_u8TouchResult == IS_BTN_LONGPRESS(45))){
			LCD_FillRect(100,10,294,40,WHITE);//�����
			cursor = 0;
			memset(paraDispBuf,0x0,sizeof(paraDispBuf));
			LCD_FillRect(120+cursor*12,42,12,2,RED);
			
			pinyinLen = 0;
			memset(pinyinBuf,0,sizeof(pinyinBuf));
			if(language == 1){
				LCD_FillRect(1,52,478,16,diaBkCol);//������ʾ��
				LCD_FillRect(40,70,400,40,tctlBkCol);//������ʾ��
			}
			
		}
		//ȷ��
		if((g_u8TouchResult == 47)||(g_u16KeyValBuf == KEY_OK)){
			sprintf((char*)paraTable_p->pList->listStr,"%s",paraDispBuf);
			LCD_FillRect(0,0,480,320,reBkCol);//��յ���
			return;
		}
		//����
		if((g_u8TouchResult == 48)||(g_u16KeyValBuf == KEY_BACK)){			
			LCD_FillRect(0,0,480,320,reBkCol);//��յ���
			return;
		}
		
//		if(g_u8TouchResult == BTN_INVALID_AREA){//�ǰ�ť������
//			if(Is_DesignArea_Touch(0,0,479,319) == 0){
//				Sleep_ms(50);
//				LCD_FillRect(0,0,480,320,WHITE);//��յ���
//				return;
//			}
//		}
	}
}

//�������öԻ���
int NumSetDialog(SYS_PARA_TABLE_STRUCT_P paraTable_p)
{
	u8 i,j,k;
	u8 temp;
	u8 cursor;
	int paraVal;
	char paraDispBuf[8];
	char numBuf[16] = "123456789-0.";
	
	u16 nBkCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		nBkCol = BLACK;
	}else{
		nBkCol = WHITE;
	}
	
	LCD_FillRect(90,40,360,260,LGRAY);//����
	LCD_FillRect(110,52,320,36,WHITE);//�����
	sprintf(paraDispBuf,"%s��",paraTable_p->sChName);
	LCD_SetColor(WHITE,GRAY);
	LCD_ShowText(Font20,120,60,0,(u8*)paraDispBuf);
//	LCD_ShowText(Font20,120,60,0,paraTable_p->sChName);
//	LCD_ShowChinese(Font20,200,60,0,"��");
	if(paraTable_p->Unit != NULL){
		LCD_ShowString(ASC20x10,420-(strlen(paraTable_p->Unit)*10),60,0,paraTable_p->Unit);
	}
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110+83*3,100,71,40,GRAY);LCD_ShowChinese(Font32,110+83*3+20,100+4,0,"�h");//ɾ��
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110+83*3,100+50,71,40,GRAY);LCD_ShowChinese(Font32,110+83*3+20,100+50+4,0,"ޒ");//����
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110,100+50*3,71,40,GRAY);LCD_ShowString(ASC20x10,140,110+50*3,0,"-");//����-
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110+83*2,100+50*3,71,40,GRAY);LCD_ShowString(ASC20x10,140+83*2+3,110+50*3,0,".");//����.
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
	//��ť��ʼ��
	g_u8ButtonNums = 15;
	struct BUTTON SampBtn[15] = {
		//sX,  sY,  eX,  eY,
		{193, 250, 263, 290, 0,0},//0, ����0
		{110, 100, 180, 140, 0,0},//1, ����1
		{193, 100, 263, 140, 0,0},//2, ����2
		{276, 100, 346, 140, 0,0},//3, ����3
		{110, 150, 180, 190, 0,0},//4, ����4
		{193, 150, 263, 190, 0,0},//5, ����5
		{276, 150, 346, 190, 0,0},//6, ����6
		{110, 200, 180, 240, 0,0},//7, ����7
		{193, 200, 263, 240, 0,0},//8, ����8
		{276, 200, 346, 240, 0,0},//9, ����9
		{110, 250, 180, 290, 0,0},//10, ����-
		{276, 250, 346, 290, 0,0},//11, ����.
		{359, 100, 429, 140, 0,0},//12, �˸�
		{359, 150, 429, 190, 0,0},//13, ����
		{359, 200, 429, 290, 0,0} //14, OK
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	//������ʾ
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
		ui_AllButtonReaction(10,WHITE,GOLD);//�ַ���
		ui_OneButtonReaction(10,GRAY,GOLD);//����
		ui_OneButtonReaction(11,GRAY,GOLD);//����
		ui_OneButtonReaction(12,GRAY,GOLD);//�˸�
		ui_OneButtonReaction(13,GRAY,GOLD);//����
		ui_OneButtonReaction(14,GRAY,GOLD);//OK
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			LCD_FillRect(90,40,360,260,nBkCol);//��յ���
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
			LCD_FillRect(90,40,360,260,nBkCol);//��յ���
			return paraVal;
		}
		//����
		if(g_u8TouchResult == 13){
			LCD_FillRect(90,40,360,260,nBkCol);//��յ���
			return 0;
		}
		//�˸�
		if(g_u8TouchResult == 12){
			if(cursor > 0)cursor -= 1;
			paraDispBuf[cursor] = ' ';
//			paraVal = atoi(paraDispBuf);
			LCD_ShowString(ASC24x12,220,58,0,paraDispBuf);
			LCD_FillRect(220+(cursor+1)*12,82,12,2,WHITE);
			LCD_FillRect(220+cursor*12,82,12,2,RED);
		}
		//�˸񳤰�
		if(g_u8TouchResult == IS_BTN_LONGPRESS(12)){
			LCD_FillRect(220,52,210,36,WHITE);//�����
			LCD_FillRect(220,82,12,2,RED);
			cursor = 0;
			memset(paraDispBuf,0,sizeof(paraDispBuf));
		}

		//����0-11
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
		
		if(g_u8TouchResult == BTN_INVALID_AREA){//�ǰ�ť������
			if(Is_DesignArea_Touch(90,40,449,299) == 0){
				LCD_FillRect(90,40,360,260,nBkCol);//��յ���
				return 0;
			}
		}
	}
}
//��������Ի���
int iNumInputDialog(int* iNumAddr)
{
	u8 i,j,k;
//	u8 temp;
	u8 cursor;
	int iParaVal;
	char paraDispBuf[8];
	char numBuf[16] = "123456789-0.";
	
	u16 inBkCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		inBkCol = BLACK;
	}else{
		inBkCol = WHITE;
	}
	
	LCD_FillRect(90,40,360,260,LGRAY);//����
	LCD_FillRect(110,52,320,36,WHITE);//�����
	LCD_SetColor(WHITE,GRAY);
	LCD_ShowChinese(Font20,120,60,0,"��������");
	LCD_ShowChinese(Font20,200,60,0,"��");
	
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110+83*3,100,71,40,GRAY);LCD_ShowChinese(Font32,110+83*3+20,100+4,0,"�h");//ɾ��
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110+83*3,100+50,71,40,GRAY);LCD_ShowChinese(Font32,110+83*3+20,100+50+4,0,"ޒ");//����
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110,100+50*3,71,40,GRAY);LCD_ShowString(ASC20x10,110,110+50*3,0,"-");//����-
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(110+83*2,100+50*3,71,40,GRAY);LCD_ShowString(ASC20x10,110+83*2+3,110+50*3,0,".");//����.
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
	//��ť��ʼ��
	g_u8ButtonNums = 15;
	struct BUTTON SampBtn[15] = {
		//sX,  sY,  eX,  eY,
		{193, 250, 263, 290, 0,0},//0, ����0
		{110, 100, 180, 140, 0,0},//1, ����1
		{193, 100, 263, 140, 0,0},//2, ����2
		{276, 100, 346, 140, 0,0},//3, ����3
		{110, 150, 180, 190, 0,0},//4, ����4
		{193, 150, 263, 190, 0,0},//5, ����5
		{276, 150, 346, 190, 0,0},//6, ����6
		{110, 200, 180, 240, 0,0},//7, ����7
		{193, 200, 263, 240, 0,0},//8, ����8
		{276, 200, 346, 240, 0,0},//9, ����9
		{110, 250, 180, 290, 0,0},//10, ����-
		{276, 250, 346, 290, 0,0},//11, ����.
		{359, 100, 429, 140, 0,0},//12, �˸�
		{359, 150, 429, 190, 0,0},//13, ����
		{359, 200, 429, 290, 0,0} //14, OK
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	//������ʾ
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
		ui_AllButtonReaction(10,WHITE,GOLD);//�ַ���
		ui_OneButtonReaction(10,GRAY,GOLD);//����
		ui_OneButtonReaction(11,GRAY,GOLD);//����
		ui_OneButtonReaction(12,GRAY,GOLD);//�˸�
		ui_OneButtonReaction(13,GRAY,GOLD);//����
		ui_OneButtonReaction(14,GRAY,GOLD);//OK
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			LCD_FillRect(90,40,360,260,inBkCol);//��յ���
			return 0;
		}
		if(g_u16KeyValBuf == KEY_FN){

		}
		
		//OK
		if(g_u8TouchResult == 14){
			iParaVal = atoi(paraDispBuf);
			*iNumAddr = iParaVal;
			LCD_FillRect(90,40,360,260,inBkCol);//��յ���
			return iParaVal;
		}
		//����
		if(g_u8TouchResult == 13){
			LCD_FillRect(90,40,360,260,inBkCol);//��յ���
			return 0;
		}
		//�˸�
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
		//�˸񳤰�
		if(g_u8TouchResult == IS_BTN_LONGPRESS(12)){
			LCD_FillRect(220,52,210,36,WHITE);//�����
			LCD_FillRect(220,82,12,2,RED);
			cursor = 0;
			memset(paraDispBuf,0,sizeof(paraDispBuf));
		}

		//����0-11
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
		
		if(g_u8TouchResult == BTN_INVALID_AREA){//�ǰ�ť������
			if(Is_DesignArea_Touch(90,40,449,299) == 0){
				LCD_FillRect(90,40,360,260,inBkCol);//��յ���
				ClearTouchSta();
				return 0;
			}
		}
	}
}
//��������Ի���
float fNumInputDialog(float fNum,u8 dicimal)
{
	u8 i,j,k;
//	u8 temp;
	u8 cursor;
	float fParaVal;
	char paraDispBuf[8];
	char numBuf[16] = "123456789-0.";
	
	u16 fnBkCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		fnBkCol = BLACK;
	}else{
		fnBkCol = WHITE;
	}
	
	LCD_FillRect(60,40,360,260,LGRAY);//����
	LCD_FillRect(80,52,320,36,WHITE);//�����
	LCD_SetColor(WHITE,GRAY);
	LCD_ShowChinese(Font20,90,60,0,"��������");
	LCD_ShowChinese(Font20,170,60,0,"��");
	
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(80+83*3,100,71,40,GRAY);LCD_ShowChinese(Font32,80+83*3+20,100+4,0,"�h");//ɾ��
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(80+83*3,100+50,71,40,GRAY);LCD_ShowChinese(Font32,80+83*3+20,100+50+4,0,"ޒ");//����
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(80,100+50*3,71,40,GRAY);LCD_ShowString(ASC20x10,110,110+50*3,0,"-");//����-
	LCD_SetColor(GRAY,BLACK);
	LCD_FillRect(80+83*2,100+50*3,71,40,GRAY);LCD_ShowString(ASC20x10,110+83*2+3,110+50*3,0,".");//����.
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
	//��ť��ʼ��
	g_u8ButtonNums = 15;
	struct BUTTON SampBtn[15] = {
		//sX,  sY,  eX,  eY,
		{163, 250, 233, 290, 0,0},//0, ����0
		{ 80, 100, 150, 140, 0,0},//1, ����1
		{163, 100, 233, 140, 0,0},//2, ����2
		{246, 100, 316, 140, 0,0},//3, ����3
		{ 80, 150, 150, 190, 0,0},//4, ����4
		{163, 150, 233, 190, 0,0},//5, ����5
		{246, 150, 316, 190, 0,0},//6, ����6
		{ 80, 200, 150, 240, 0,0},//7, ����7
		{163, 200, 233, 240, 0,0},//8, ����8
		{246, 200, 316, 240, 0,0},//9, ����9
		{ 80, 250, 150, 290, 0,0},//10, ����-
		{246, 250, 316, 290, 0,0},//11, ����.
		{329, 100, 399, 140, 0,0},//12, �˸�
		{329, 150, 399, 190, 0,0},//13, ����
		{329, 200, 399, 290, 0,0} //14, OK
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	//������ʾ
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
		ui_AllButtonReaction(10,WHITE,GOLD);//�ַ���
		ui_OneButtonReaction(10,GRAY,GOLD);//����
		ui_OneButtonReaction(11,GRAY,GOLD);//����
		ui_OneButtonReaction(12,GRAY,GOLD);//�˸�
		ui_OneButtonReaction(13,GRAY,GOLD);//����
		ui_OneButtonReaction(14,GRAY,GOLD);//OK
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			fParaVal = fNum;
			LCD_FillRect(60,40,360,260,fnBkCol);//��յ���
			return fParaVal;
		}
		if(g_u16KeyValBuf == KEY_FN){

		}
		
		//OK
		if(g_u8TouchResult == 14){
			fParaVal = atof(paraDispBuf);
			LCD_FillRect(60,40,360,260,fnBkCol);//��յ���
			return fParaVal;
		}
		//����
		if(g_u8TouchResult == 13){
			fParaVal = fNum;
			LCD_FillRect(60,40,360,260,fnBkCol);//��յ���
			return fParaVal;
		}
		//�˸�
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
		//�˸񳤰�
		if(g_u8TouchResult == IS_BTN_LONGPRESS(12)){
			LCD_FillRect(190,52,210,36,WHITE);//�����
			LCD_FillRect(190,82,12,2,RED);
			cursor = 0;
			memset(paraDispBuf,0,sizeof(paraDispBuf));
		}

		//����0-11
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
		
		if(g_u8TouchResult == BTN_INVALID_AREA){//�ǰ�ť������
			if(Is_DesignArea_Touch(60,40,419,299) == 0){
				fParaVal = fNum;
				LCD_FillRect(60,40,360,260,fnBkCol);//��յ���
				return fParaVal;
			}
		}
	}
}

//ѡ���������
void SelectDialog(SYS_PARA_TABLE_STRUCT_P paraTable_p,u8 optNums)
{
	u8 i,j,k;
	u16 x,y;
	
	u16 slBkCol;
	u16 slTxCol;
	u16 slBarCol;
	u16 slDioCol;
	//��ɫ����
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
	
	//�������
	LCD_FillRect(60,56,40,296,slBkCol);//��
	LCD_FillRect(440,56,40-1,296,slBkCol);//��
	LCD_FillRect(60,32,420,24,slBkCol);//��
	LCD_FillRect(60,296,420,24-1,slBkCol);//��
	
	LCD_FillRect(100,56,340,36,slBarCol);//������
	LCD_SetColor(slBarCol,WHITE);
	LCD_ShowChinese(Font16,238,66,0,paraTable_p->sChName);
	
	LCD_FillRect(100,92,340,204,slDioCol);//����
	LCD_FillRect(100,92,340,1,slBkCol);//�ָ���
	
	if(optNums == 2){//2����ť
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
		//��ť��ʼ��
		g_u8ButtonNums = 2;
		struct BUTTON SampBtn[2] = {
			//sX,  sY,  eX,  eY,
			{130, 175, 249, 214, 0,0},//0,
			{290, 175, 409, 214, 0,0} //1,
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}else if(optNums == 3){//3����ť
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
		
		//��ť��ʼ��
		g_u8ButtonNums = 3;
		struct BUTTON SampBtn[3] = {
			//sX,  sY,  eX,  eY,
			{210, 115, 329, 154, 0,0},//0,
			{210, 175, 329, 214, 0,0},//1,
			{210, 235, 329, 274, 0,0} //2,
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
		
	}else if(optNums == 4){//4����ť
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

		//��ť��ʼ��
		g_u8ButtonNums = 4;
		struct BUTTON SampBtn[4] = {
			//sX,  sY,  eX,  eY,
			{130, 140, 249, 177, 0,0},//0,
			{290, 140, 409, 177, 0,0},//1,
			{130, 210, 249, 249, 0,0},//2,
			{290, 210, 409, 249, 0,0},//3,
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}else if(optNums == 5){//5����ť
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
		
		//��ť��ʼ��
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
		
	}else if(optNums == 6){//6����ť
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
		
		//��ť��ʼ��
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
		
		//��ť��ʼ��
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
		ui_OneButtonReaction(g_u8ButtonNums-1,BLACK,GOLD);//�ַ���
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			LCD_FillRect(100,56,340,240,slBkCol);//��յ���
			return;
		}
		if(g_u16KeyValBuf == KEY_FN){

		}
		
		if(g_u8TouchResult < optNums){//�����ť
			*paraTable_p->ParaAddr = g_u8TouchResult;
			LCD_FillRect(100,56,340,240,slBkCol);//��յ���
			return;
		}
		if(g_u8TouchResult == BTN_INVALID_AREA){//�ǰ�ť������
			if(Is_DesignArea_Touch(90,40,449,299) == 0){
				LCD_FillRect(100,56,340,240,slBkCol);//��յ���
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
	//��ɫ����
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
	
	//�������
	LCD_FillRect(60,56,40,296,tmBkCol);//��
	LCD_FillRect(440,56,40-1,296,tmBkCol);//��
	LCD_FillRect(60,32,420,24,tmBkCol);//��
	LCD_FillRect(60,296,420,24-1,tmBkCol);//��
	
	//������
	LCD_FillRect(100,56,340,36,tmBarCol);
	LCD_SetColor(tmBarCol,WHITE);
	if(mode == 0) LCD_ShowChinese(Font16,238,66,0,"ϵͳ����");
	else LCD_ShowChinese(Font16,238,66,0,"ϵͳʱ��");
	//����
	LCD_FillRect(100,92,340,168,tmDioCol);//����
	LCD_FillRect(100,92,340,1,tmBkCol);//�ָ���
	//��ť
	LCD_FillRect(100,260,170,36,GRAY);//ȷ����ť
	LCD_FillRect(270,260,170,36,GRAY);//ȡ����ť
	LCD_FillRect(100,259,340,1,tmBkCol);//�Ϸָ���
	LCD_FillRect(269,260,1,36,tmBkCol);//�зָ���
	
	LCD_SetColor(GRAY,tmTxCol);
	LCD_ShowChinese(Font16,168,270,0,"ȷ��");
	LCD_SetColor(GRAY,tmTxCol);
	LCD_ShowChinese(Font16,338,270,0,"ȡ��");
	
	
	for(i=0;i<3;i++){
		x = 120+i*105;
		y = 116;
		LCD_FillRect(x,y,90,36,GRAY);//+
		LCD_FillRect(x,y+36+1,90,48-2,WHITE);//����
		LCD_FillRect(x,y+84,90,36,GRAY);//��
//		LCD_DrawRect(x,100,90,120,GRAY);//�߿�
		LCD_SetColor(GRAY,tmTxCol);
		LCD_ShowString(ASC32x16,x+37,y+2,0,"+");
		LCD_ShowString(ASC32x16,x+37,y+86,0,"-");
	}
	
	if(mode == 0){
		LCD_SetColor(WHITE,GRAY);
		LCD_ShowChinese(Font20,180,167,0,"��");
		LCD_ShowChinese(Font20,285,167,0,"��");
		LCD_ShowChinese(Font20,390,167,0,"��");
		
		year = g_SysTime.year;
		month = g_SysTime.month;
		day = g_SysTime.day;
		
		LCD_SetColor(WHITE,BLACK);
		LCD_ShowNnNums(ASC20x10,130,166,0,4,year);
		LCD_ShowNnNums(ASC20x10,250,166,0,2,month);
		LCD_ShowNnNums(ASC20x10,355,166,0,2,day);
	}else{
		LCD_SetColor(WHITE,GRAY);
		LCD_ShowChinese(Font20,180,167,0,"ʱ");
		LCD_ShowChinese(Font20,285,167,0,"��");
		LCD_ShowChinese(Font20,390,167,0,"��");
		
		hour = g_SysTime.hour;
		min = g_SysTime.min;
		sec = g_SysTime.sec;
		
		LCD_SetColor(WHITE,BLACK);
		LCD_ShowNnNums(ASC20x10,145,166,0,2,hour);
		LCD_ShowNnNums(ASC20x10,250,166,0,2,min);
		LCD_ShowNnNums(ASC20x10,355,166,0,2,sec);
	}
	
	//��ť��ʼ��
	g_u8ButtonNums = 8;
	struct BUTTON SampBtn[8] = {
		//sX,  sY,  eX,  eY,
		{120, 116, 209, 151, 0,0},//0, +1
		{120, 200, 209, 235, 0,0},//1, -1
		{225, 116, 314, 151, 0,0},//2, +2
		{225, 200, 314, 235, 0,0},//3, -2
		{330, 116, 419, 151, 0,0},//4, +3
		{330, 200, 419, 235, 0,0},//5, -3
		{100, 260, 269, 295, 0,0},//6, ȷ��
		{270, 260, 439, 295, 0,0},//7, ȡ��
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_OneButtonReaction(6,GRAY,GOLD);//ȷ��
		ui_OneButtonReaction(7,GRAY,GOLD);//ȷ��
		
		if(g_u8TouchResult == 0){//+1
			if(mode == 0){//����
				year += 1;
				if(year > 2099)year = 2021;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,130,166,0,4,year);
			}else{//ʱ��
				hour += 1;
				if(hour > 23)hour = 0;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,145,166,0,2,hour);
			}
		}
		if(g_u8TouchResult == 1){//-1
			if(mode == 0){//����
				year -= 1;
				if(year < 2021)year = 2099;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,130,166,0,4,year);
			}else{//ʱ��
				hour -= 1;
				if(hour < 0)hour = 23;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,145,166,0,2,hour);
			}
		}
		////
		if(g_u8TouchResult == 2){//+2
			if(mode == 0){//����
				month += 1;
				if(month > 12)month = 1;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,250,166,0,2,month);
			}else{//ʱ��
				min += 1;
				if(min > 59)min = 0;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,250,166,0,2,min);
			}
		}
		if(g_u8TouchResult == 3){//-2
			if(mode == 0){//����
				month -= 1;
				if(month < 1)month = 12;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,250,166,0,2,month);
			}else{//ʱ��
				min -= 1;
				if(min < 0)min = 59;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,250,166,0,2,min);
			}
		}
		////
		if(g_u8TouchResult == 4){//+3
			if(mode == 0){//����
				day += 1;
				dayTemp = calendarTab[month];
				if(month == 2){
					if(year%4 == 0){//����
						dayTemp = 29;
					}else{
						dayTemp = 28;
					}
				}
				if(day > dayTemp)day = 1;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,355,166,0,2,day);
			}else{//ʱ��
				sec += 1;
				if(sec > 59)sec = 0;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,355,166,0,2,sec);
			}
		}
		if(g_u8TouchResult == 5){//-3
			if(mode == 0){//����
				day -= 1;
				dayTemp = calendarTab[month];
				if(month == 2){
					if(year%4 == 0){//����
						dayTemp = 29;
					}else{
						dayTemp = 28;
					}
				}
				if(day < 1)day = dayTemp;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,355,166,0,2,day);
			}else{//ʱ��
				sec -= 1;
				if(sec < 0)sec = 59;
				LCD_SetColor(WHITE,BLACK);
				LCD_ShowNnNums(ASC20x10,355,166,0,2,sec);
			}
		}
		////
		if(g_u8TouchResult == 6){//ȷ��
			LCD_FillRect(100,56,340,240,tmBkCol);//��յ���
			if(mode == 0){//����
				g_SysTime.year = year;
				g_SysTime.month = month;
				g_SysTime.day = day;
			}else{//ʱ��
				g_SysTime.hour = hour;
				g_SysTime.min = min;
				g_SysTime.sec = sec;
			}
			RX8010_SetTime(&g_SysTime);
			return;
		}
		if(g_u8TouchResult == 7){//ȡ�����˳�����
			LCD_FillRect(100,56,340,240,tmBkCol);//��յ���
			return;
		}
		
		if(g_u8TouchResult == BTN_INVALID_AREA){//�ǰ�ť������
			if(Is_DesignArea_Touch(90,50,449,299) == 0){
				LCD_FillRect(100,56,340,240,tmBkCol);//��յ���
				return;
			}
		}
	}
}

//������Ϣ��
struct PROMPTStr promptStr[] = {
	{DIALOGSAVE, 	"���ݱ���", "�������ݲ��˳�������",		NULL,"ȷ��","ȡ��"},
	{DIALOGDELEALL, "����ɾ��", "ɾ��ȫ��������",			NULL,"ȷ��","ȡ��"},
	{DIALOGDELESEL, "����ɾ��", "ɾ��ѡ�й�����",			NULL,"ȷ��","ȡ��"},
	{DIALOGBATLOW,  "������ʾ", "�������㣬�뼰ʱ���",		NULL,"ȷ��",NULL},
	{DIALOGBATNONE, "������ʾ", "�������ͣ��޷�����",		NULL,"ȷ��",NULL},
	{DIALOGSTORLOW,	"�ڴ���ʾ", "�ڴ治�㣬�뼰ʱɾ������",	NULL,"ȷ��",NULL},
	{DIALOGSTORNONE,"�ڴ���ʾ", "�ڴ��������޷��洢����",	NULL,"ȷ��",NULL},
	{DIALOGUSBLINK,	 "USB��ʾ", "USB�����ӣ�׼����������",	NULL,NULL,NULL},
	{DIALOGBTLINK,  "������ʾ", "���������ӣ�׼����������",	NULL,NULL,NULL},
	
	{DIALOGGOUJIANEXIST, "��ʾ", "�����Ѵ��ڣ��Ƿ񸲸ǣ�",	NULL,	"ȷ��","ȡ��"},
	{DIALOGBIAODINGBATLOW, "��ʾ", "��ص�������",	"�����±궨����",	"ȷ��","ȡ��"},
	{DIALOGBIAODINGBATJUMP,"��ʾ", "��ص����仯����",	"�����±궨����",	"ȷ��","ȡ��"},
	{DIALOGBIAODINGTEMPER, "��ʾ", "�����¶ȱ仯����",	"�����±궨����",	"ȷ��","ȡ��"},
};

//��ʾ����
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
	//��ɫ����
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
	
	//������
	LCD_FillRect(100,56,340,36,pdBarCol);
	//����
	LCD_FillRect(100,92,340,168,pdDioCol);//����
	LCD_FillRect(100,92,340,1,pdBkCol);//�ָ���
	//��ť
//	LCD_FillRect(100,260,170,36,GOLD);//ȷ����ť
//	LCD_FillRect(270,260,170,36,GRAY);//ȡ����ť
//	LCD_FillRect(100,259,340,1,fxBkCol);//�Ϸָ���
//	LCD_FillRect(269,260,1,36,fxBkCol);//�зָ���
	
//	LCD_SetColor(GOLD,BLACK);
//	LCD_ShowChinese(Font16,168,270,0,"ȷ��");
//	LCD_SetColor(GRAY,fxTxCol);
//	LCD_ShowChinese(Font16,338,270,0,"ȡ��");
	
	
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
	if(promptStr[id].btnWords1 == NULL){//�ް�ť
		LCD_FillRect(100,260,340,36,pdDioCol);//������ť����
	}else{
		if(promptStr[id].btnWords2 == NULL){//һ����ť
			//��ť
			LCD_FillRect(100,260,340,36,GRAY);//ȷ��
			LCD_FillRect(100,259,340,1,pdBkCol);//�Ϸָ���
//			LCD_FillRect(269,260,1,36,fxBkCol);//�зָ���
			
			LCD_SetColor(GRAY,pdBtxCol);
			LCD_ShowText(Font16,254,270,0,promptStr[id].btnWords1);
			
			//��ť��ʼ��
			g_u8ButtonNums = 1;
			struct BUTTON SampBtn[1] = {
				//sX,  sY,  eX,  eY,
				{100, 260, 439, 295, 0,0} //0,
			};
			memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
			
		}else{//������ť
			//��ť
			LCD_FillRect(100,260,170,36,GRAY);//ȷ����ť
			LCD_FillRect(270,260,170,36,GRAY);//ȡ����ť
			LCD_FillRect(100,259,340,1,pdBkCol);//�Ϸָ���
			LCD_FillRect(269,260,1,36,pdBkCol);//�зָ���
			
			LCD_SetColor(GRAY,pdBtxCol);
			LCD_ShowText(Font16,168,270,0,promptStr[id].btnWords1);
			LCD_ShowText(Font16,338,270,0,promptStr[id].btnWords2);
			
			if(id == DIALOGBIAODINGTEMPER){
				sprintf((char*)dbuf,"(�ϴ�%4.1f�㣬��ǰ%4.1f��)",g_SysPara.fTemperatureBDLast,g_GlobalPara.fTemperature);
				sLen = strlen((char*)dbuf);
				LCD_SetColor(pdDioCol,pdTemVol);
				LCD_ShowText(Font16,60+(420-sLen*Font16/2)/2,196,0,dbuf);
			}
			
			//��ť��ʼ��
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
		if(promptStr[id].btnWords1 == NULL){//�ް�ť
			Sleep_ms(2000);
			LCD_FillRect(100,56,340,240,pdBkCol);//��յ���
			return 1;
		}else{
			if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_OK)){//ȷ��
				LCD_FillRect(100,56,340,240,pdBkCol);//��յ���
				return 1;
			}		
			if((g_u8TouchResult == 1)||(g_u16KeyValBuf ==KEY_BACK)){//ȡ��
				LCD_FillRect(100,56,340,240,pdBkCol);//��յ���
				return 0;
			}
		}
		if(g_u8TouchResult == BTN_INVALID_AREA){//�ǰ�ť������
//			if(Is_DesignArea_Touch(90,50,449,299) == 0){
//				LCD_FillRect(100,56,340,240,pdBkCol);//��յ���
//				return 0;
//			}
		}
		Sleep_ms(5);
	}
}

