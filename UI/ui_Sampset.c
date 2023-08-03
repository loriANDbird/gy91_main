/*******************************************************************************
�����ߣ�
�������ڣ�2018.03.01
������������Ȳ���
    1. ui_Sample10 �����ʼ����
    2. ui_Sample10_Run ����ʵ��

���¼�¼��
    1. 2018.03.01 ���δ���
    
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
	HighLightID = 0xff;//�޸���
	SampParaSet_Init(1);
}

void ui_Sampset_Run(void)
{
	u8 index;
	u8 pageNum = 1;//��һҳ
	u8 pageLast = 1;//��һҳ
	u8 pageCap = 5;//ҳ����
	u8 pageQnt;	   //ҳ��
	
	u16 bkCol;
	u16 menuBtCol;
	//��ɫ����
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
		ui_OneButtonReaction(5,menuBtCol,GOLD);//����
		ui_OneButtonReaction(6,menuBtCol,GOLD);//�߼�
		ui_OneButtonReaction(7,menuBtCol,GOLD);//Ĭ��ֵ
		
//		ui_DispTime(0);//ʱ��
//		ui_DispBattery(0);//����
		
		if(g_u16KeyValBuf == KEY_OK){

		}
		if(g_u16KeyValBuf == KEY_BACK){
			SendUiNo(g_GlobalPara.ucFromPageNum);
			g_GlobalPara.ucFromPageNum = UI_SAMPSET;
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//�л�ҳ��
		if(g_u16KeyValBuf == KEY_FN){
			Touch_Beep_On();
			SendUiNo(UI_SAMPLE10);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		
		//����
		if(g_u8TouchResult == 5){
			SendUiNo(g_GlobalPara.ucFromPageNum);
			g_GlobalPara.ucFromPageNum = UI_SAMPSET;
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		if(g_GlobalPara.ucFromPageNum == UI_SAMPLE10){
			//�߼�����
			if(g_u8TouchResult == 6){
				Sleep_ms(50);
				LCD_FillRect(56,32,424,288,bkCol);
				HighParaSet_Fun();
				SampParaSet_Init(pageNum);
			}
			//Ĭ��ֵ
			if(g_u8TouchResult == 7){
				ParaTableDefault(SampSetTable);
				SampParaSet_Init(pageNum);
			}
		}else if(g_GlobalPara.ucFromPageNum == UI_SAMPJGJ){
			//Ĭ��ֵ
			if(g_u8TouchResult == 6){
				ParaTableDefault(SampSetTable);
				SampParaSet_Init(pageNum);
			}
		}
		else if(g_GlobalPara.ucFromPageNum == UI_SAMPBLOCK){
			//Ĭ��ֵ
			if(g_u8TouchResult == 6){
				ParaTableDefault(SampSetTable);
				SampParaSet_Init(pageNum);
			}
			//�궨
			if(g_u8TouchResult == 7){
				SendUiNo(UI_BIAODING);
				g_GlobalPara.ucFromPageNum = UI_SAMPBLOCK;
				LCD_FillRect(56,0,424,320,bkCol);
				return;
			}
		}
		else if(g_GlobalPara.ucFromPageNum == UI_VERIFY){
			
		}

		//ѡ�ť
		if(g_u8TouchResult < pageCap){
			index = (pageNum-1)*pageCap + g_u8TouchResult;
			if(index < SampSetTable[0].ucTableLen){
				ui_DispTitleBar("����������",1);
				HighLightID = index;//����ѡ��
				if(SampSetTable[index].ucParaType == TYP_VALUE_DATA){//��������
					NumSetDialog(&SampSetTable[index]);
				}else if((SampSetTable[index].ucParaType == TYP_LIST_HANZI)||(SampSetTable[index].ucParaType == TYP_LIST_ASIC)){//����ѡ��
					SelectDialog(&SampSetTable[index],SampSetTable[index].iParaListLen);
				}else if(SampSetTable[index].ucParaType == TYP_VALUE_ASIC){//�ı�����
					TextSetDialog(&SampSetTable[index]);
					//�����û����õĹ�����
					sprintf(g_SysPara.GouJianName,"%s",SampSetTable[index].pList->listStr);
				}
				SaveSysPara();
				SampParaSet_Init(pageNum);
			}
		}
		
		if(g_u8TouchResult == SLIDE_END_UP){//�ϻ�
			if(pageQnt > 1){
				pageLast = pageNum;
				pageNum += 1;
				if(pageNum > pageQnt)pageNum = 1;
				//��ʾ�����б�
				SampParaOptions(SampSetTable,pageCap,pageNum);
				DispScrollBar(pageQnt,pageNum,pageLast);
			}
		}
		if(g_u8TouchResult == SLIDE_END_DOWN){//�»�
			if(pageQnt > 1){
				pageLast = pageNum;
				pageNum -= 1;
				if(pageNum < 1) pageNum = pageQnt;
				//��ʾ�����б�
				SampParaOptions(SampSetTable,pageCap,pageNum);
				DispScrollBar(pageQnt,pageNum,pageLast);
			}
		}
		
		Sleep_ms(5);
	}
}

//����������ҳ��ʼ��
void SampParaSet_Init(u8 pageNum)
{
	//��ɫ����
//	if(g_SysPara.iTheme == THEME_DARK){
//		//״̬��
//		LCD_FillRect(57,1,422,31,GRAY);
//		LCD_SetColor(GRAY,WHITE);
//		LCD_ShowChinese(Font16,222,9,0,"����������");
//	}else{
//		//״̬��
//		LCD_FillRect(57,1,422,31,DGRAY);
//		LCD_SetColor(DGRAY,WHITE);
//		LCD_ShowChinese(Font16,222,9,0,"����������");
//	}
	ui_DispTitleBar("����������",0);
	
	//ҳ�����˵���
	if(g_GlobalPara.ucFromPageNum == UI_SAMPLE10){
		MENU_TABLE_STRUCT ParaMenu[3] = {
			{0,3,"��",NULL},
			{1,3,"��","�߼�"},
			{2,3,"Ĭ","Ĭ��ֵ"}
		};
		//�˵���ʾ
		ui_DispMenuBtn(ParaMenu,0);
		
		//��ʾ��������ѡ��
		SampSetTable = &ParaTable[PARATABLEINDEX_SAMPLE];
		SampParaOptions(SampSetTable,5,pageNum);
		
		//��ť��ʼ��
		g_u8ButtonNums = 8;
		struct BUTTON tempBtn[8] = {
			//sX,  sY,  eX,  eY,
			{256,  56, 405,  91, 0,0},//0, ����ѡ��1
			{256, 106, 405, 141, 0,0},//1, ����ѡ��2
			{256, 156, 405, 191, 0,0},//2, ����ѡ��3
			{256, 206, 405, 241, 0,0},//3, ����ѡ��4
			{256, 256, 405, 291, 0,0},//4, ����ѡ��5
			
			{  0,   0,  59,  49, 0,0},//5, ����
			{  0,  50,  59, 103, 0,0},//6, �߼�
			{  0, 104,  59, 157, 0,0} //7, Ĭ��ֵ
		};
		memcpy(g_ButtonBuf,tempBtn,sizeof(tempBtn));
	}else if(g_GlobalPara.ucFromPageNum == UI_SAMPJGJ){
		MENU_TABLE_STRUCT ParaMenu[2] = {
			{0,2,"��",NULL},
			{1,2,"Ĭ","Ĭ��ֵ"}
		};
		//�˵���ʾ
		ui_DispMenuBtn(ParaMenu,0);
		
		//��ʾ��������ѡ��
		SampSetTable = &ParaTable[PARATABLEINDEX_JGJ];
		SampParaOptions(SampSetTable,5,pageNum);
	
		//��ť��ʼ��
		g_u8ButtonNums = 7;
		struct BUTTON tempBtn[7] = {
			//sX,  sY,  eX,  eY,
			{256,  56, 405,  91, 0,0},//0, ����ѡ��1
			{256, 106, 405, 141, 0,0},//1, ����ѡ��2
			{256, 156, 405, 191, 0,0},//2, ����ѡ��3
			{256, 206, 405, 241, 0,0},//3, ����ѡ��4
			{256, 256, 405, 291, 0,0},//4, ����ѡ��5
			
			{  0,   0,  59,  49, 0,0},//5, ����
			{  0,  50,  59, 103, 0,0} //6  Ĭ��ֵ
		};
		memcpy(g_ButtonBuf,tempBtn,sizeof(tempBtn));
	}else if(g_GlobalPara.ucFromPageNum == UI_SAMPBLOCK){
		MENU_TABLE_STRUCT ParaMenu[3] = {
			{0,3,"��",NULL},
			{1,3,"Ĭ","Ĭ��ֵ"},
			{2,3,"��","�궨"}
		};
		//�˵���ʾ
		ui_DispMenuBtn(ParaMenu,0);
		
		//��ʾ��������ѡ��
		SampSetTable = &ParaTable[PARATABLEINDEX_BLOCK];
		SampParaOptions(SampSetTable,5,pageNum);
	
		//��ť��ʼ��
		g_u8ButtonNums = 8;
		struct BUTTON tempBtn[8] = {
			//sX,  sY,  eX,  eY,
			{256,  56, 405,  91, 0,0},//0, ����ѡ��1
			{256, 106, 405, 141, 0,0},//1, ����ѡ��2
			{256, 156, 405, 191, 0,0},//2, ����ѡ��3
			{256, 206, 405, 241, 0,0},//3, ����ѡ��4
			{256, 256, 405, 291, 0,0},//4, ����ѡ��5
			
			{  0,   0,  59,  49, 0,0},//5, ����
			{  0,  50,  59, 103, 0,0},//6  Ĭ��ֵ
			{  0, 104,  59, 157, 0,0} //7  �궨
		};
		memcpy(g_ButtonBuf,tempBtn,sizeof(tempBtn));
	}else if(g_GlobalPara.ucFromPageNum == UI_VERIFY){
		MENU_TABLE_STRUCT ParaMenu[1] = {
			{0,1,"��",NULL}
		};
		//�˵���ʾ
		ui_DispMenuBtn(ParaMenu,0);
		
		//��ʾ��������ѡ��
		SampSetTable = &ParaTable[PARATABLEINDEX_VERIFY];
		SampParaOptions(SampSetTable,3,pageNum);
		
		//��ť��ʼ��
		g_u8ButtonNums = 6;
		struct BUTTON tempBtn[6] = {
			//sX,  sY,  eX,  eY,
			{256,  56, 405,  91, 0,0},//0, ����ѡ��1
			{256, 106, 405, 141, 0,0},//1, ����ѡ��2
			{256, 156, 405, 191, 0,0},//2, ����ѡ��3
			{256, 206, 405, 241, 0,0},//3, ����ѡ��4
			{256, 256, 405, 291, 0,0},//4, ����ѡ��5
			
			{  0,   0,  59,  49, 0,0},//5, ����
		};
		memcpy(g_ButtonBuf,tempBtn,sizeof(tempBtn));
	}

//	ui_DispTime(1);
//	ui_DispBattery(1);//��ʾ����

//	NumSetDialog(&ParaTable[1]);
//	TimeSetDialog();
//	SelectDialog(2);
	
	ClearTouchSta();
}
//�߼�����
void HighParaSet_Init(SYS_PARA_TABLE_STRUCT* highParaTable,u8 pageNum)
{
	//��ɫ����
//	if(g_SysPara.iTheme == THEME_DARK){
//		//״̬��
//		LCD_FillRect(57,1,422,31,GRAY);
//		LCD_SetColor(GRAY,WHITE);
//		LCD_ShowChinese(Font16,222,9,0,"�߼���������");
//	}else{
//		//״̬��
//		LCD_FillRect(57,1,422,31,DGRAY);
//		LCD_SetColor(DGRAY,WHITE);
//		LCD_ShowChinese(Font16,222,9,0,"�߼���������");
//	}
	ui_DispTitleBar("�߼���������",0);
	
	//ҳ�����˵���
	MENU_TABLE_STRUCT HighParaMenu[2] = {
		{0,2,"��",NULL},
		{1,2,"Ĭ","Ĭ��ֵ"}
	};
	//�˵���ʾ
	ui_DispMenuBtn(HighParaMenu,0);
	
	//��ʾ�����б�
	SampParaOptions(highParaTable,5,pageNum);
	
	//��ť��ʼ��
	g_u8ButtonNums = 8;
	struct BUTTON SampBtn[8] = {
		//sX,  sY,  eX,  eY,		
		{256,  56, 405,  91, 0,0},//0, ����ѡ��1
		{256, 106, 405, 141, 0,0},//1, ����ѡ��2
		{256, 156, 405, 191, 0,0},//2, ����ѡ��3
		{256, 206, 405, 241, 0,0},//3, ����ѡ��4
		{256, 256, 405, 291, 0,0},//4, ����ѡ��5
		
		{  0,   0,  59,  49, 0,0},//5, ����
		{  0,  50,  59, 103, 0,0} //6  Ĭ��ֵ
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));	
}

void HighParaSet_Fun(void)
{
//	u8 i;
	u8 hindex;
	u8 pageNum = 1;//��һҳ
	u8 pageLast = 1;//��һҳ
	u8 pageCap = 5;//ҳ����
	u8 pageQnt;	   //ҳ��
	
	u16 bkCol;
	u16 menuBtCol;
	//��ɫ����
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
	
	HighLightID = 0xff;//�޸���
	HighParaSet_Init(HighTable,pageNum);
	DispScrollBar(pageQnt,pageNum,pageLast);
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_OneButtonReaction(5,menuBtCol,GOLD);//����
		ui_OneButtonReaction(6,menuBtCol,GOLD);//Ĭ��ֵ
		
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
//		//�л�ҳ��
//		if(g_u16KeyValBuf == KEY_FN){
//			Touch_Beep_On();
//			SendUiNo(UI_SAMPLE10);
//			LCD_FillRect(0,0,480,320,WHITE);
//			return;
//		}
		
		//����
		if(g_u8TouchResult == 5){
			if(g_SysPara.g_SamplePara.iScreenWide == 0){
				g_GlobalPara.sDispScreenWide = 800;
			}else{
				g_GlobalPara.sDispScreenWide = 1200;
			}
			
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//����
		if(g_u8TouchResult == 6){//�ָ�Ĭ��ֵ
			ParaTableDefault(HighTable);
			HighParaSet_Init(HighTable,pageNum);
		}
		
		//ѡ�ť
		if(g_u8TouchResult < pageCap){
			
			hindex = (pageNum-1)*pageCap + g_u8TouchResult;
			if(hindex < HighTable->ucTableLen){
				ui_DispTitleBar("�߼���������",1);
				if(HighTable[hindex].ucParaType == TYP_VALUE_DATA){//��������
					NumSetDialog(&HighTable[hindex]);
				}else if((HighTable[hindex].ucParaType == TYP_LIST_HANZI)||(HighTable[hindex].ucParaType == TYP_LIST_ASIC)){//����ѡ��
					SelectDialog(&HighTable[hindex],HighTable[hindex].iParaListLen);
				}else if(HighTable[hindex].ucParaType == TYP_VALUE_ASIC){//�ı�����
					TextSetDialog(&HighTable[hindex]);
				}
				HighLightID = hindex;//����ѡ��
				HighParaSet_Init(HighTable,pageNum);
			}else{
				HighLightID = 0xff;//����ѡ��
			}
		}		

		if(g_u8TouchResult == SLIDE_END_UP){//�ϻ�
			
			if(pageQnt > 1){
				pageLast = pageNum;
				pageNum += 1;
				if(pageNum > pageQnt)pageNum = 1;
				//��ʾ�����б�
				SampParaOptions(HighTable,pageCap,pageNum);
				DispScrollBar(pageQnt,pageNum,pageLast);
			}
		}
		if(g_u8TouchResult == SLIDE_END_DOWN){//�»�
			
			if(pageQnt > 1){
				pageLast = pageNum;
				pageNum -= 1;
				if(pageNum < 1) pageNum = pageQnt;
				//��ʾ�����б�
				SampParaOptions(HighTable,pageCap,pageNum);
				DispScrollBar(pageQnt,pageNum,pageLast);
			}
		}
		if(g_u8TouchResult == SLIDE_END_LEFT){//��
			
//			i+=1;
//			if(i>6)i=2;
//			LCD_FillRect(60,40,360,260,WHITE);//����
//			SelectDialog(&ParaTable[4],i);
		}
		if(g_u8TouchResult == SLIDE_END_RIGHT){//�һ�
			
//			i-=1;
//			if(i<2)i=6;
//			LCD_FillRect(60,40,360,260,WHITE);//����
//			SelectDialog(&ParaTable[4],i);
		}
	}
}

//ParaTableArr:���ַ��pageCap:һҳ��ʾ������pageNum:ҳ��
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
	//��ɫ����
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
	//��ʾ�б�
	for(i=0; i<pageCap; i++){
		tIndex = i+optNum;
		if((i+optNum) > (ParaTableArr[optNum].ucTableLen-1)){
			LCD_FillRect(nameX,56+10+i*50,64+8,16,bkCol);//���������
			LCD_FillRect(paraX,56+i*50,150,36,bkCol);//�������
			continue;
		}
		LCD_FillRect(paraX,56+i*50,150,36,paraBk);//�����ǰ����
		sprintf(dispBuf,"%s ",ParaTableArr[i+optNum].sChName);
		LCD_SetColor(bkCol,txCol);
		LCD_ShowText(Font16,nameX,56+10+i*50,0,(u8*)dispBuf);//��������
		if(tIndex  == HighLightID)LCD_SetColor(paraBk,RED);
		else LCD_SetColor(paraBk,txCol);
		
		memset(dispBuf,0,sizeof(dispBuf));
		memset(dispBuf,' ',10);//��������10���ַ�
		dispBuf[10] = '\0';
		if(ParaTableArr[i+optNum].ucParaType == TYP_VALUE_ASIC){//��������
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
//��ʾ������:pageQnt:��ҳ����pageNum:ҳ��1,2,3...
void DispScrollBar(short pageQnt,short pageNum,short pageLast)
{
	//����������x=460~479,y=34~319;
	//���������ȣ�270
	//��������ȣ�w=12
	unsigned short i;
	unsigned short wid = 8;
	unsigned short higt;
	float len = 280.0f/pageQnt;
	
	u16 barBkCol;
	u16 barCol;
	//��ɫ����
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

