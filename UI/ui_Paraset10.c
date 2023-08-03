/*******************************************************************************
�����ߣ�
�������ڣ�2018.03.01
������������������
    1. ui_Paraset10 �����ʼ����
    2. ui_Paraset10_Run ����ʵ��
   
���¼�¼��
    1. 2018.03.01 ���δ���
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
//	HighLightID = 0xff;//�޸���
	SysParaSet_Init(SysParaTable,1);
}

void ui_Paraset10_Run(void)
{
	u8 i;
	u8 hindex;
	u8 pageNum = 1;//��һҳ
	u8 pageLast = 1;//��һҳ
	u8 pageQnt;	   //ҳ��
	
	u16 BKCol;
	u16 menuBtCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		BKCol = BLACK;
		menuBtCol = DGRAY;
	}else{
		BKCol = WHITE;
		menuBtCol = LGRAY;
	}
	pageQnt = 2;
	
//	HighLightID = 0xff;//�޸���
//	SysParaSet_Init(SysParaTable,pageNum);
	DispScrollBar(pageQnt,pageNum,pageLast);
	
	//��������ʾ
	if(g_GlobalPara.ucFromPageNum == UI_MAIN){
		Get_BatPrcnt();
		if(g_GlobalPara.iBatPercent < 20){
			PromptDialog(DIALOGBATLOW);
			g_GlobalPara.ucFromPageNum = UI_SAMPSET;//��װ����ҳ����
			LCD_FillRect(56,32,424,288,BKCol);
			SysParaSet_Init(SysParaTable,1);
		}
		if(g_GlobalPara.iBatPercent < 5){
			PromptDialog(DIALOGBATNONE);
			SendUiNo(UI_MAIN);
			g_GlobalPara.ucFromPageNum = UI_SAMPSET;//��װ����ҳ����
			LCD_FillRect(56,32,424,288,BKCol);
			return;
		}
	}
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_AllButtonReaction(6,menuBtCol,GOLD);
		
		//����
		if(g_SysPara.iBlueToothSta == 1){
			BT_Lookup();
//			if(g_bBlueTooDispFlag == 1){
//				g_bBlueTooDispFlag = 0;
//				ui_DispBluetooth();
//			}
		}
		
		//����
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			SendUiNo(UI_MAIN);
			LCD_FillRect(0,0,480,320,BKCol);
			return;
		}
		
		if(pageNum == 1){
			//����
			if(g_u8TouchResult == 7){
				SendUiNo(UI_MAIN);
				LCD_FillRect(0,0,480,320,BKCol);
				return;
			}
			//����
			if(g_u8TouchResult == 8){
				LCD_FillRect(56,32,424,288,BKCol);
				About_Fun();
				SysParaSet_Init(SysParaTable,pageNum);;
			}
		}else if(pageNum == 2){
			//����
			if(g_u8TouchResult == 4){
				SendUiNo(UI_MAIN);
				LCD_FillRect(0,0,480,320,BKCol);
				return;
			}
			//����
			if(g_u8TouchResult == 5){
				LCD_FillRect(56,32,424,288,BKCol);
				About_Fun();
				SysParaSet_Init(SysParaTable,pageNum);;
			}
		}
		
		
		//ѡ�ť
		if(g_u8TouchResult < g_u8ButtonNums-2){
			hindex = g_u8TouchResult;
			if(hindex > 6){//��������
				
			}else{
				hindex = g_u8TouchResult;
//				if(hindex < SysParaTable->ucTableLen){
					SysParaOpt(pageNum,hindex,SysParaTable);
//					HighLightID = hindex;//����ѡ��
					SaveSysPara();
					InitSysPara();
					if((hindex > 3)||(pageNum != 1)){//ǰ�ĸ���ť����ˢ����
						SysParaSet_Init(SysParaTable,pageNum);
					}
					SysParaOptRes(pageNum,hindex,SysParaTable);
					if((pageNum == 2)&&((hindex == 2)||(hindex == 3))){//�������ñ���ɫ
						//��ɫ����
						if(g_SysPara.iTheme == THEME_DARK){
							BKCol = BLACK;
						}else{
							BKCol = WHITE;
						}
					}
//				}else{
//					HighLightID = 0xff;//����ѡ��
//				}
			}
		}
		//�϶����ڱ�������
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
		if(g_u8TouchResult == SLIDE_END_UP){//�ϻ�
			
			if(pageQnt > 1){
				pageLast = pageNum;
				pageNum += 1;
				if(pageNum > pageQnt)pageNum = 1;
				//��ʾ�����б�
				LCD_FillRect(56,32,424,288,BKCol);
				BrushPageInit(pageNum);//ˢ��ҳ���б���ʾ
				DispScrollBar(pageQnt,pageNum,pageLast);
			}
		}
		if(g_u8TouchResult == SLIDE_END_DOWN){//�»�
			
			if(pageQnt > 1){
				pageLast = pageNum;
				pageNum -= 1;
				if(pageNum < 1) pageNum = pageQnt;
				//��ʾ�����б�
				LCD_FillRect(56,32,424,288,BKCol);
				BrushPageInit(pageNum);//ˢ��ҳ���б���ʾ
				DispScrollBar(pageQnt,pageNum,pageLast);
			}
		}
		if(g_u8TouchResult == SLIDE_END_LEFT){//��

		}
		if(g_u8TouchResult == SLIDE_END_RIGHT){//�һ�
			
		}
	}
}

//ϵͳ����ҳ���ʼ��
void SysParaSet_Init(SYS_PARA_TABLE_STRUCT* SysParaTable,u8 pageNum)
{
	u16 siBkCol;
	u16 siBarCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		siBkCol  = BLACK;
		siBarCol = GRAY;
	}else{
		siBkCol  = WHITE;
		siBarCol = DGRAY;
	}
	
	//ҳ�����˵���
	MENU_TABLE_STRUCT SyssetMenu[2] = {
		{0,2,"��",NULL},
		{1,2,"��","����"}
	};
	//�˵���ʾ
	ui_DispMenuBtn(SyssetMenu,0);
	
	//״̬��
	LCD_DrawRect(56,0,424,32,siBkCol);
	LCD_FillRect(57,1,422,31,siBarCol);
	
//	LCD_SetColor(siBarCol,WHITE);
//	LCD_ShowChinese(Font16,238,9,0,"ϵͳ����");
	ui_DispTitleBar("ϵͳ����",0);
	
	BrushPageInit(pageNum);//ˢ��ҳ���б���ʾ
}
//ˢ��ҳ���б���ʾ
void BrushPageInit(u8 page)
{
	u16 BkCol;
	u16 TxCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		BkCol = BLACK;
		TxCol = WHITE;
	}else{
		BkCol = WHITE;
		TxCol = BLACK;
	}
	
	if(page == 1){
		//��ʾ��������
		LCD_SetColor(BkCol,TxCol);
		LCD_ShowChinese(Font16,128,48, 0,"�����ϴ�");
		LCD_ShowChinese(Font16,282,48, 0,"������Ч");
		LCD_ShowChinese(Font16,128,88, 0,"ˮƽ����");
		LCD_ShowChinese(Font16,282,88, 0,"��ֱ����");
		LCD_ShowChinese(Font16,128,128,0,"��������");
		LCD_ShowChinese(Font16,128,176,0,"����ѡ��");
		LCD_ShowChinese(Font16,128,226,0,"ϵͳ����");
		LCD_ShowChinese(Font16,128,276,0,"ϵͳʱ��");
		//��ʾ����
		RadioButtonDisp(216,48,*(u8*)SysParaTable[0].ParaAddr);
		RadioButtonDisp(370,48,*(u8*)SysParaTable[1].ParaAddr);
		RadioButtonDisp(216,88,*(u8*)SysParaTable[2].ParaAddr);
		RadioButtonDisp(370,88,*(u8*)SysParaTable[3].ParaAddr);
		BKLightDisp(256,128,*(u8*)SysParaTable[4].ParaAddr);
		DialogParaDisp(256,166,&SysParaTable[5]);
		DialogParaDisp(256,216,&SysParaTable[6]);
		DialogParaDisp(256,266,&SysParaTable[7]);
		
		//��ť��ʼ��
		g_u8ButtonNums = 12;
		struct BUTTON SampBtn[12] = {
			//sX,  sY,  eX,  eY,
			{208,  40, 259,  71, 0,0},//0, �����ϴ�
			{362,  40, 413,  71, 0,0},//1, ������Ч
			{208,  80, 259, 111, 0,0},//2, ˮƽ����
			{362,  80, 413, 111, 0,0},//3, ��ֱ����
			{256, 166, 405, 201, 0,0},//4, ����ѡ��
			{256, 216, 405, 251, 0,0},//5, ϵͳ����
			{256, 266, 405, 301, 0,0},//6, ϵͳʱ��
			
			{  0,   0,  59,  49, 0,0},//7, ����
			{  0,  50,  59, 103, 0,0},//8, ����
			
			{256, 120, 305, 151, 0,BTN_ABILITY_DRAG},//9, ��������
			{306, 120, 355, 151, 0,BTN_ABILITY_DRAG},//10, ��������
			{356, 120, 405, 151, 0,BTN_ABILITY_DRAG} //11, ��������
			
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}else if(page == 2){
		//��ʾ��������
		LCD_SetColor(BkCol,TxCol);
		LCD_ShowChinese(Font16,128,56,0,"��������");
		LCD_ShowChinese(Font16,128,106,0,"�ػ�ʱ��");
		LCD_ShowChinese(Font16,128,156,0,"�������");
		//������ʾ
		DialogParaDisp(256,46,&SysParaTable[8]);
		DialogParaDisp(256,96,&SysParaTable[9]);
		DoubleButtonDisp(256,146,&SysParaTable[10]);
		
		//��ť��ʼ��
		g_u8ButtonNums = 6;
		struct BUTTON SampBtn[6] = {
			//sX,  sY,  eX,  eY,
			{256,  46, 405,  81, 0,0},//0, ����ѡ��1
			{256,  96, 405, 131, 0,0},//1, ����ѡ��2
			{256, 146, 325, 181, 0,0},//2, ����ѡ��3
			{336, 146, 405, 181, 0,0},//3, ����ѡ��4
			{  0,   0,  59,  49, 0,0},//4, ����
			{  0,  50,  59, 103, 0,0} //5, ����
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}
}

//��ѡ��ť
void RadioButtonDisp(u16 x,u16 y,u8 value)
{
	u16 barCol;
	u16 noCol;
	//��ɫ����
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

//˫��ť
void DoubleButtonDisp(u16 x,u16 y,SYS_PARA_TABLE_STRUCT* ParaTableArr)
{
	u16 dBkCol;
	u16 dTxCol;
	//��ɫ����
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

//����������ʾ
void BKLightDisp(u16 x,u16 y,u8 bkVal)
{
	u16 lBkCol;
	//��ɫ����
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
//��Ҫ�������õĲ�����ʾ
void DialogParaDisp(u16 x,u16 y,SYS_PARA_TABLE_STRUCT* ParaTableArr)
{
	u8 i;
	u16 paraX,paraY;
	char dispBuf[16];
	
	u16 opBkCol;
	u16 opTxCol;
	//��ɫ����
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
			if(ParaTableArr[0].Unit != NULL){//�ػ�ʱ��
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
	}else if(ParaTableArr[0].ucParaType == TYP_VALUE_DATE){//����
		sprintf(dispBuf,"%4d-%02d-%02d",g_SysTime.year,g_SysTime.month,g_SysTime.day);
		LCD_ShowText(Font16,paraX,paraY,0,(u8*)dispBuf);
	}else if(ParaTableArr[0].ucParaType == TYP_VALUE_TIME){//ʱ��
		sprintf(dispBuf,"%d:%02d:%02d",g_SysTime.hour,g_SysTime.min,g_SysTime.sec);
		LCD_ShowText(Font16,paraX,paraY,0,(u8*)dispBuf);
	}else if(ParaTableArr[0].ucParaType == TYP_VALUE_PASSWORD){			
		LCD_ShowString(Font16,paraX,paraY,0,dispBuf);
		for(i=0;i<4;i++){
			LCD_ShowString(Font16,paraX+i*12,paraY+2,0,"-");
		}
	}
}
//��������
//ParaTableArr:�������ַ
void SysParaOpt(u8 pageNum,u8 optNum,SYS_PARA_TABLE_STRUCT* ParaTableArr)
{
	int password;
	u16 tBkCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		tBkCol = BLACK;
	}else{
		tBkCol = WHITE;
	}
	
	if(pageNum < 1)pageNum = 1;
	if(pageNum > 2)pageNum = 2;
	if(pageNum == 1){//��һҳ
		if(optNum <= 3){//ǰ�ĸ�����
			*ParaTableArr[optNum].ParaAddr = !(*ParaTableArr[optNum].ParaAddr);
			if(optNum == 0)RadioButtonDisp(216,48,*ParaTableArr[0].ParaAddr);
			if(optNum == 1)RadioButtonDisp(370,48,*ParaTableArr[1].ParaAddr);
			if(optNum == 2)RadioButtonDisp(216,88,*ParaTableArr[2].ParaAddr);
			if(optNum == 3)RadioButtonDisp(370,88,*ParaTableArr[3].ParaAddr);
		}else if(optNum <= 6){
			optNum += 1;
			ui_DispTitleBar("ϵͳ����",1);
			if(optNum == 5){//����ѡ��
				SelectDialog(&ParaTableArr[optNum],ParaTableArr[optNum].iParaListLen);
			}
			if(optNum == 6){//ϵͳ����
				TimeSetDialog(0);
			}
			if(optNum == 7){//ϵͳʱ��
				TimeSetDialog(1);
			}
		}
	}else if(pageNum == 2){//�ڶ�ҳ
		if(optNum <= 3){//ǰ�ĸ�����
			optNum += 8;//��λ�����������
			if(optNum == 8){//��������
				ui_DispTitleBar("ϵͳ����",1);
				password = NumSetDialog(&ParaTableArr[optNum]);
				if(password == 6221){//���Ƹ�С����
					LCD_FillRect(0,0,480,320,tBkCol);
					ConfigFiZero(0);
				}else if(password == 6222){//���Ƹִ�����
					LCD_FillRect(0,0,480,320,tBkCol);
					ConfigFiZero(1);
				}else if(password == 6231){//Բ��С����
					ConfigFiZero(2);
				}else if(password == 6232){//Բ�ִ�����
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
				}else if(password == 3568){//�����ֿ�
					update_font(0,0,ASC16x8);
				}else if(password == 3569){//�����ֿ�
					update_unioem(0,0,ASC16x8);
				}else if(password == 2833){//�����ָ���������
					BT_SetDefault();
				}
			}
			if(optNum == 9){//�ػ�ʱ��
				ui_DispTitleBar("ϵͳ����",0);
				NumSetDialog(&ParaTableArr[optNum]);
			}
			if(optNum == 10){//��ɫ
				if(*ParaTableArr[10].ParaAddr != THEME_DARK){
					*ParaTableArr[10].ParaAddr = THEME_DARK;
					LCD_FillRect(0,0,480,320,BLACK);
				}
			}
			if(optNum == 11){//ǳɫ
				if(*ParaTableArr[10].ParaAddr != THEME_LIGHT){
					*ParaTableArr[10].ParaAddr = THEME_LIGHT;
					LCD_FillRect(0,0,480,320,WHITE);
				}
			}
		}
	}else return;
}
//��������
void SysParaOptRes(u8 pageNum,u8 optNum,SYS_PARA_TABLE_STRUCT* ParaTableArr)
{
	u16 orBkCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		orBkCol = GRAY;
	}else{
		orBkCol = DGRAY;
	}
	if(pageNum == 1){//��һҳ
		if(optNum <= 3){//ǰ�ĸ�����
			if(optNum == 0){
				if(*ParaTableArr[optNum].ParaAddr == 0){
					BT_PowerOff();//��������Դ
					LCD_SetColor(orBkCol,orBkCol);
					LCD_ShowChinese(Font20,414,6,0,"��");
				}else{
					BT_PowerOn();//��������Դ
					LCD_SetColor(orBkCol,WHITE);
					LCD_ShowChinese(Font20,414,6,0,"��");
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
			if(optNum == 4){//����ѡ��
				
			}
			if(optNum == 5){//ϵͳ����
				
			}
			if(optNum == 6){//ϵͳʱ��
				
			}
		}
	}else if(pageNum == 2){//�ڶ�ҳ
		
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
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		ziBkCol = BLACK;
		ziTxCol = WHITE;
	}else{
		ziBkCol = WHITE;
		ziTxCol = BLACK;
	}
	
	//�ָ���
	for(i=0;i<8;i++){
		if(i == 0){
			LCD_FillRect(0,40+i*35-1,480,2,GRAY);//��
		}else {
			LCD_FillRect(0,40+i*35,480,1,GRAY);//��
		}
	}
	for(i=0;i<5;i++){
		if(i == 2){
			LCD_FillRect(80+i*80-1,40,2,285,GRAY);//��
		}else {
			LCD_FillRect(80+i*80,40,1,285,GRAY);//��
		}
	}
	
	LCD_SetColor(ziBkCol,ziTxCol);
	LCD_ShowChinese(Font32,8,2,0,"ޒ");//����ͼ��
	
	if(mode == 0){//���Ƹ֡�С����
		LCD_SetColor(ziBkCol,ziTxCol);
		LCD_ShowChinese(Font20,70,10,0,"���Ƹ���С����");//��������
		paraFi_p = g_SysPara.iFact_LWSmall_Fi;
		paraZero_p = g_SysPara.iFact_LWSmall_Zero;
	}else if(mode == 1){//���Ƹ֡�������
		LCD_SetColor(ziBkCol,ziTxCol);
		LCD_ShowChinese(Font20,70,10,0,"���Ƹ��Ǵ�����");//��������
		paraFi_p = g_SysPara.iFact_LWBig_Fi;
		paraZero_p = g_SysPara.iFact_LWBig_Zero;
	}else if(mode == 2){//Բ�֡�С����
		LCD_SetColor(ziBkCol,ziTxCol);
		LCD_ShowChinese(Font20,70,10,0,"Բ����С����");//��������
		paraFi_p = g_SysPara.iFact_YGSmall_Fi;
		paraZero_p = g_SysPara.iFact_YGSmall_Zero;
	}else if(mode == 3){//Բ�֡�������
		LCD_SetColor(ziBkCol,ziTxCol);
		LCD_ShowChinese(Font20,70,10,0,"Բ���Ǵ�����");//��������
		paraFi_p = g_SysPara.iFact_YGBig_Fi;
		paraZero_p = g_SysPara.iFact_YGBig_Zero;
	}
	LCD_SetColor(ziBkCol,ziTxCol);
	LCD_ShowString(Font20,240,10,0,"Fi:");
	LCD_ShowString(Font20,360,10,0,"Zero:");
	LCD_SetColor(ziBkCol,ziTxCol);
	LCD_ShowChinese(Font20,20,48,0,"ֱ��");//��������
	LCD_ShowChinese(Font20,100,48,0,"����");//��������
	LCD_ShowChinese(Font20,180,48,0,"���");//��������
	LCD_ShowChinese(Font20,260,48,0,"ֱ��");//��������
	LCD_ShowChinese(Font20,340,48,0,"����");//��������
	LCD_ShowChinese(Font20,420,48,0,"���");//��������
	
	//����
	dateTemp = paraFi_p[0]/1000.0f;
	sprintf(dataBuf,"%.3f",dateTemp);
	LCD_SetColor(ziBkCol,ORANGE);
	LCD_ShowString(Font20,285,10,0,dataBuf);
	//���
	dateTemp = paraZero_p[0]/10.0f;
	sprintf(dataBuf,"%.1f",dateTemp);
	LCD_SetColor(ziBkCol,ORANGE);
	LCD_ShowString(Font20,425,10,0,dataBuf);
	
	for(i=0;i<2;i++){
		for(j=0;j<7;j++){
			sprintf(dataBuf,"D%02d",Rebar_Table_Diam[i*7+j]);
			LCD_SetColor(ziBkCol,ziTxCol);
			LCD_ShowString(Font20,24+i*240,83+j*35,0,dataBuf);
			//����
			dateTemp = paraFi_p[i*7+j+1]/1000.0f;
			sprintf(dataBuf,"%.3f",dateTemp);
			LCD_SetColor(ziBkCol,ORANGE);
			LCD_ShowString(Font20,95+i*240,83+j*35,0,dataBuf);
			//���
			dateTemp = paraZero_p[i*7+j+1]/10.0f;
			sprintf(dataBuf,"%.1f",dateTemp);
			LCD_SetColor(ziBkCol,ORANGE);
			LCD_ShowString(Font20,185+i*240,83+j*35,0,dataBuf);
		}
	}
		
	//��ť��ʼ��
	g_u8ButtonNums = 31;
	struct BUTTON SampBtn[31];
	//����
	SampBtn[0].sX = 270;
	SampBtn[0].sY = 0;
	SampBtn[0].eX = 349;
	SampBtn[0].eY = 39;
	SampBtn[0].btSta = 0;
	SampBtn[0].btAbility = 0;
	//���
	SampBtn[1].sX = 400;
	SampBtn[1].sY = 0;
	SampBtn[1].eX = 479;
	SampBtn[1].eY = 39;
	SampBtn[1].btSta = 0;
	SampBtn[1].btAbility = 0;
	for(i=0;i<2;i++){
		for(j=0;j<7;j++){
			btnNum = (i*7+j)*2+2;
			//����
			SampBtn[btnNum].sX = 80+i*240;
			SampBtn[btnNum].sY = 75+j*35;
			SampBtn[btnNum].eX = 80+i*240+79;
			SampBtn[btnNum].eY = 75+j*35+34;
			SampBtn[btnNum].btSta = 0;
			SampBtn[btnNum].btAbility = 0;
			//���
			SampBtn[btnNum+1].sX = 160+i*240;
			SampBtn[btnNum+1].sY = 75+j*35;
			SampBtn[btnNum+1].eX = 160+i*240+79;
			SampBtn[btnNum+1].eY = 75+j*35+34;
			SampBtn[btnNum+1].btSta = 0;
			SampBtn[btnNum+1].btAbility = 0;
		}
	}
	//����
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
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		zBkCol = BLACK;
	}else{
		zBkCol = WHITE;
	}
	
	ConfigFiZero_Init(mode);	
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_OneButtonReactionXY(30,0,0,47,36,zBkCol,GOLD);//����
		
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			LCD_FillRect(0,0,480,320,zBkCol);
			return;
		}
		//�л�ҳ��
		if(g_u16KeyValBuf == KEY_FN){

		}
		//����
		if(g_u8TouchResult < 30){
			index = g_u8TouchResult;
			if(index%2 == 0){//����
				dateTemp = paraFi_p[index/2]/1000.0f;
				dateTemp = fNumInputDialog(dateTemp,3);
				if(dateTemp > 9.999)dateTemp = 9.999;
				paraFi_p[index/2] = (int)(dateTemp*1000.0f);
			}else{//���
				dateTemp = paraZero_p[index/2]/10.0f;
				dateTemp = fNumInputDialog(dateTemp,1);
				if(dateTemp > 9.9)dateTemp = 9.9;
				paraZero_p[index/2] = (int)(dateTemp*10.0f);
			}		
			ConfigFiZero_Init(mode);
			
		}
		//����
		if(g_u8TouchResult == 30){
//			SendUiNo(UI_MAIN);
			LCD_FillRect(0,0,480,320,zBkCol);
			return;
		}
	}
}

//���ڱ���
void About_Fun(void)
{
	u8 i;
	u8 locaSta = 0;
	u32 locaCnt = 0;
	char dBuf[10];
	
	u16 aBkCol;
	u16 aTxCol;
	u16 aBarCol;
	//��ɫ����
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
	LCD_ShowChinese(Font16,237,8,0,"���ڱ���");
	
	//�ָ���
	for(i=0;i<6;i++){
		LCD_FillRect(70,75+i*40,395,1,GRAY);//��
	}
	//ʣ������
	g_GlobalPara.iStorageFreePrcnt = Get_FatFreePrcnt();
	
	LCD_SetColor(aBkCol,aTxCol);
	LCD_ShowText(Font16,100,48,0,"����汾:");
	LCD_ShowString(Font16,250,48,0,g_SysPara.SoftVersion);
	LCD_ShowText(Font16,100,88,0,"Ӳ���汾:");
	LCD_ShowString(Font16,250,88,0,g_SysPara.HardVersion);
	LCD_ShowText(Font16,100,128,0,"�������:");
	LCD_ShowString(Font16,250,128,0,g_SysPara.EquipmentNum);
	LCD_ShowText(Font16,100,168,0,"��������:");
	LCD_ShowString(Font16,250,168,0,g_SysPara.BlueToothNum);
	LCD_ShowText(Font16,100,208,0,"ʣ������:");
	sprintf(dBuf,"%d%%",g_GlobalPara.iStorageFreePrcnt);
	LCD_ShowString(Font16,250,208,0,dBuf);
	LCD_ShowText(Font16,100,248,0,"��λ:");
	if(GPS_Info.status == 'A'){
		sprintf(dBuf,"%8.4f",GPS_Info.lat);
		LCD_ShowString(Font16,250,248,0,dBuf);
		sprintf(dBuf,"%8.4f",GPS_Info.lon);
		LCD_ShowString(Font16,250+72,248,0,dBuf);
	}else{
		LCD_ShowText(Font16,250,248,0,"�����ʼ��λ");
	}
	LCD_ShowText(Font16,174,288,0,"���������߿ƿƼ����޹�˾");
	
	//ҳ�����˵���
	MENU_TABLE_STRUCT PreviewMenu[1] = {
		{0,1,"��",NULL}
	};
	//�˵���ʾ
	ui_DispMenuBtn(PreviewMenu,0);
	//��ť��ʼ��
	g_u8ButtonNums = 2;
	struct BUTTON abBtn[2] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0},//0, ����
		{250, 235, 450, 275, 0,0} //1, ��λ
	};
	memcpy(g_ButtonBuf,abBtn,sizeof(abBtn));
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		
		//����
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			SendUiNo(UI_PARASET10);
			LCD_FillRect(56,32,424,288,aBkCol);
			return;
		}
		
		//����
		if(g_u8TouchResult == 0){
			SendUiNo(UI_PARASET10);
			LCD_FillRect(56,32,424,288,aBkCol);
			ClearTouchSta();
			return;
		}
		//��λ
		if(g_u8TouchResult == 1){
			if(GPS_Info.status != 'A'){
				LCD_FillRect(250,248,200,16,aBkCol);//��ն�λ��ʾ
				LCD_SetColor(aBkCol,aTxCol);
				LCD_ShowText(Font16,100,248,0,"��λ��");
				GPS_ManualStart();//��ʼ�ֶ���λ
				locaSta = 1;
				i = 0;
			}
		}
		if(locaSta == 1){
			GPS_Location_Data();//GPS���
			if(GPS_Info.status == 'A'){//��λ�ɹ�
				LCD_SetColor(aBkCol,aTxCol);
				LCD_ShowText(Font16,100,248,0,"��λ�ɹ�");
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
