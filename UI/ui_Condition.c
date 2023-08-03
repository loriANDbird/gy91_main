/*******************************************************************************
�����ߣ�
�������ڣ�2018.03.01
����������
    1. ui_Start �����ʼ��
    2. ui_Start_Run ����ʵ��

    
���¼�¼��
    1. 2018.03.01 ����
    
*******************************************************************************/
#include "ui_Condition.h"
#include "Config_ui.h"

//u8 WorkCondition;//��������0-4
//u8 AoTuMianSta;//0:����,1:͹��
//int AoTuMianDiam;//����ֱ��
static u8 WorkConditionLast = 0;
static u8 WorkConditionTemp;

extern SYS_PARA_TABLE_STRUCT* VerifyParaTable;

//ɨ�蹤����
struct DISPOPTION WorkConditionTable[] = {
	{CHANGGUI  , "��", "������", NULL, NULL},
	{GAODIJIN  , "��", "�ߵͽ���", NULL, NULL},
	{BIGUJIN   , "��", "�ܹ�����", NULL, NULL},
	{AOTUMIAN  , "��", "��͹����", NULL, NULL},
	{FANXIANGXZ, "��", "�����������", NULL, NULL}
};
extern char *Condition[5];

void AoTuMianSetFun(void);
void FanXiangXZSetFun(void);
int FXXZQuZhiFun(u8 num);
extern u8 Biaoding_Fun(void);
extern void DispTempera(void);

void ui_Condition_InitDisp(void)
{
	u8 dispNum;
	u8 i,j;
	
	u16 bxCol;
	u16 txCol;
	
	//״̬��
	if(g_SysPara.iTheme == THEME_DARK){//��ɫ����
//		LCD_FillRect(57,1,422,31,GRAY);
//		LCD_SetColor(GRAY,WHITE);
//		LCD_ShowChinese(Font16,236,9,0,"��������");
		
		bxCol = GRAY;
		txCol = WHITE;
	}else{//ǳɫ����
//		LCD_FillRect(57,1,422,31,DGRAY);
//		LCD_SetColor(DGRAY,WHITE);
//		LCD_ShowChinese(Font16,236,9,0,"��������");
		
		bxCol = LGRAY;
		txCol = BLACK;
	}
	
	ui_DispTitleBar("��������",0);
	
	WorkConditionTemp = g_DetectPara.WorkCondition;
	
	for(i=0;i<3;i++){
		for(j=0;j<2;j++){
			dispNum = i*2+j;
			if(dispNum >= 5)break;
			if(dispNum == WorkConditionTemp){
//				LCD_DrawRect(90+205*j,66+84*i,155,50,GRAY);
				LCD_FillRect(90+205*j+1,66+84*i+1,153,48,GOLD);
				LCD_SetColor(GOLD,BLACK);
			}else{
//				LCD_DrawRect(90+205*j,66+84*i,155,50,GRAY);
				LCD_FillRect(90+205*j+1,66+84*i+1,153,48,bxCol);
				LCD_SetColor(bxCol,txCol);
			}
			if(dispNum == 4){
				LCD_ShowChinese(Font32,100+205*j,75+84*i,0,WorkConditionTable[dispNum].icon1);
				LCD_ShowChinese(Font16,140+205*j,82+84*i,0,WorkConditionTable[dispNum].Name1);
			}else{
				LCD_ShowChinese(Font32,105+205*j,75+84*i,0,WorkConditionTable[dispNum].icon1);
				LCD_ShowChinese(Font16,145+205*j,82+84*i,0,WorkConditionTable[dispNum].Name1);
			}
		}
	}
	
	//ҳ�����˵���
	MENU_TABLE_STRUCT Samp10Menu[6] = {
		{0,6,"��",NULL},
		{1,6,"��","����"},
		{2,6,"��","��ʾ"},
		{3,6,"��","����"},
		{4,6,"��","����ͼ"},
		{5,6,"��","�궨"}
	};
	//�˵���ʾ
	ui_DispMenuBtn(Samp10Menu,3);
	
	//��ť��ʼ��
	g_u8ButtonNums = 11;
	struct BUTTON SampBtn[11] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0},//0, ����
		{  0,  50,  59, 103, 0,0},//1, ����
		{  0, 104,  59, 157, 0,0},//2, ��ʾ
		{  0, 158,  59, 211, 0,0},//3, ����
		{  0, 212,  59, 265, 0,0},//4, ����
		{  0, 266,  59, 319, 0,0},//5, �궨
		
		{ 90,  66, 239, 115, 0,0},//6, ��ʾ1
		{300,  66, 449, 115, 0,0},//7, ��ʾ2
		{ 90, 150, 239, 199, 0,0},//8, ��ʾ3
		{300, 150, 449, 199, 0,0},//9, ��ʾ4
		{ 90, 234, 239, 283, 0,0} //10, ��ʾ5
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	WorkConditionLast = WorkConditionTemp;
}

//UI���к���
void ui_Condition_Run(void)
{	
	u8 i,j;
	u16 bkCol;
	u16 txCol;
	u16 bxCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
		txCol = WHITE;
		bxCol = GRAY;
	}else{
		bkCol = WHITE;
		txCol = BLACK;
		bxCol = LGRAY;
	}
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		if(g_u16KeyValBuf == KEY_BACK){
			SendUiNo(UI_SAMPLE10);
			LCD_FillRect(56,0,424,320,bkCol);
			return;
		}
		
		if((g_u8TouchResult >= 6)&&(g_u8TouchResult <= 10)){
			WorkConditionTemp = g_u8TouchResult-6;
			if((WorkConditionTemp != WorkConditionLast)||(WorkConditionTemp == 3)||(WorkConditionTemp == 4)){
				i = WorkConditionLast/2;
				j = WorkConditionLast%2;
				
				LCD_FillRect(90+205*j+1,66+84*i+1,153,48,bxCol);
				LCD_SetColor(bxCol,txCol);
				if(WorkConditionLast == 4){
					LCD_ShowChinese(Font32,100+205*j,75+84*i,0,WorkConditionTable[WorkConditionLast].icon1);
					LCD_ShowChinese(Font16,140+205*j,82+84*i,0,WorkConditionTable[WorkConditionLast].Name1);
				}else{
					LCD_ShowChinese(Font32,105+205*j,75+84*i,0,WorkConditionTable[WorkConditionLast].icon1);
					LCD_ShowChinese(Font16,145+205*j,82+84*i,0,WorkConditionTable[WorkConditionLast].Name1);
				}
				
				i = WorkConditionTemp/2;
				j = WorkConditionTemp%2;
				
				LCD_FillRect(90+205*j+1,66+84*i+1,153,48,GOLD);
				LCD_SetColor(GOLD,BLACK);
				if(WorkConditionTemp == 4){
					LCD_ShowChinese(Font32,100+205*j,75+84*i,0,WorkConditionTable[WorkConditionTemp].icon1);
					LCD_ShowChinese(Font16,140+205*j,82+84*i,0,WorkConditionTable[WorkConditionTemp].Name1);
				}else{
					LCD_ShowChinese(Font32,105+205*j,75+84*i,0,WorkConditionTable[WorkConditionTemp].icon1);
					LCD_ShowChinese(Font16,145+205*j,82+84*i,0,WorkConditionTable[WorkConditionTemp].Name1);
				}
				
				//����
				if(WorkConditionTemp == 3){
					g_DetectPara.WorkCondition = WorkConditionTemp;
					ui_DispTitleBar("��������",1);
					AoTuMianSetFun();
					Sleep_ms(10);
					SendUiNo(UI_SAMPLE10);
					LCD_FillRect(56,32,424,288,bkCol);
					return;
//					ui_Condition_InitDisp();
				}
				if(WorkConditionTemp == 4){
					g_DetectPara.WorkCondition = WorkConditionTemp;
					ui_DispTitleBar("��������",1);
					FanXiangXZSetFun();
					Sleep_ms(10);
					SendUiNo(UI_SAMPLE10);
					LCD_FillRect(56,32,424,288,bkCol);
					return;
//					ui_Condition_InitDisp();
				}
				LCD_SetColor(bkCol,txCol);
				LCD_ShowChinese(Font16,240,8,0,(u8*)Condition[WorkConditionTemp]);//��ʾ����
			}
			WorkConditionLast = WorkConditionTemp;
			g_DetectPara.WorkCondition = WorkConditionTemp;
			
			Sleep_ms(10);
			SendUiNo(UI_SAMPLE10);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		
		//����
		if(g_u8TouchResult == 0){
			SendUiNo(UI_SAMPLE10);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//����
		if(g_u8TouchResult == 1){
			SendUiNo(UI_SAMPSET);
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//��ʾ
		if(g_u8TouchResult == 2){
			SendUiNo(UI_SAMPDISP);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//����
		if(g_u8TouchResult == 3){
//			SendUiNo(UI_ANALYZE);
//			LCD_FillRect(60,0,420,320,WHITE);
//			return;
		}
		//����
		if(g_u8TouchResult == 4){
			SendUiNo(UI_ANALYZE);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//�궨
		if(g_u8TouchResult == 5){
			SendUiNo(UI_BIAODING);
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
	}
}

//***********************************************************************
//��͹�������ó�ʼ��
void AoTuMianSetInit(void)
{
	u16 atBkCol;
	u16 atTxCol;
	u16 atBarCol;
	u16 atDioCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		atBkCol = BLACK;
		atTxCol = WHITE;
		atBarCol = GRAY;
		atDioCol = DGRAY;
	}else{
		atBkCol = WHITE;
		atTxCol = BLACK;
		atBarCol = DGRAY;
		atDioCol = LGRAY;
	}
	
	//�������
	LCD_FillRect(60,56,40,296,atBkCol);//��
	LCD_FillRect(440,56,40-1,296,atBkCol);//��
	LCD_FillRect(60,32,420,24,atBkCol);//��
	LCD_FillRect(60,296,420,24-1,atBkCol);//��
	
	LCD_FillRect(100,56,340,36,atBarCol);//������
	LCD_SetColor(atBarCol,WHITE);
	LCD_ShowChinese(Font16,214,66,0,"��͹��������");
	
	LCD_FillRect(100,92,340,168,atDioCol);//����
	LCD_FillRect(100,92,340,1,atBkCol);//�ָ���
	
	LCD_FillRect(264,116,60,32,GRAY);//����
	LCD_FillRect(334,116,60,32,GRAY);//͹��
	LCD_FillRect(264,160,130,32,GRAY);//����ֱ��
	
	LCD_SetColor(atDioCol,atTxCol);
	LCD_ShowChinese(Font16,144,124,0,"�����");
	LCD_ShowChinese(Font16,144,168,0,"����ֱ��");
	
	LCD_SetColor(GRAY,atTxCol);
	LCD_ShowChinese(Font16,278,124,0,"����");
	LCD_ShowChinese(Font16,348,124,0,"͹��");
	
	LCD_ShowString(ASC20x10,348,168,0,"mm >");
	LCD_SetColor(GRAY,atTxCol);
	if(g_DetectPara.AoTuMianDiam == 0){
		LCD_ShowString(ASC20x10,284,168,0," -- ");
	}else{
		LCD_ShowNums(ASC20x10,284,168,0,g_DetectPara.AoTuMianDiam);
	}
	
	LCD_FillRect(100,260,170,36,GRAY);//ȷ����ť
	LCD_FillRect(270,260,170,36,GRAY);//ȡ����ť
	LCD_FillRect(100,259,340,1,atBkCol);//�ָ���
	LCD_FillRect(269,260,1,36,atBkCol);//�ָ���
	
	LCD_SetColor(GRAY,atTxCol);
	LCD_ShowChinese(Font16,168,270,0,"ȷ��");
	LCD_ShowChinese(Font16,338,270,0,"ȡ��");
	
	if(g_DetectPara.AoTuMianSta  == 0){
		LCD_DrawRect(264,116,60,32,GOLD);//����
		LCD_DrawRect(264+1,116+1,60-2,32-2,GOLD);//����
		LCD_DrawRect(334,116,60,32,GRAY);//͹��
		LCD_DrawRect(334+1,116+1,60-2,32-2,GRAY);//͹��
	}else{
		LCD_DrawRect(264,116,60,32,GRAY);//����
		LCD_DrawRect(264+1,116+1,60-2,32-2,GRAY);//����
		LCD_DrawRect(334,116,60,32,GOLD);//͹��
		LCD_DrawRect(334+1,116+1,60-2,32-2,GOLD);//͹��
	}
	
	//��ť��ʼ��
	g_u8ButtonNums = 5;
	struct BUTTON SampBtn[5] = {
		//sX,  sY,  eX,  eY,
		{100, 260, 269, 295, 0,0},//0, ȷ��
		{270, 260, 439, 295, 0,0},//1, ȡ��
		{264, 116, 323, 147, 0,0},//2, ����
		{334, 116, 393, 147, 0,0},//3, ͹��
		{264, 160, 393, 191, 0,0} //4, ����ֱ��
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
}
//��͹��������
void AoTuMianSetFun(void)
{
	u8 aotumainstaTemp;
	int aotumaindiamTemp;
	
	u16 atBkCol;
	u16 atTxCol;
	u16 atBarCol;
	u16 atDioCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		atBkCol = BLACK;
		atTxCol = WHITE;
		atBarCol = GRAY;
		atDioCol = DGRAY;
	}else{
		atBkCol = WHITE;
		atTxCol = BLACK;
		atBarCol = DGRAY;
		atDioCol = LGRAY;
	}
	
	aotumainstaTemp = g_DetectPara.AoTuMianSta;
	aotumaindiamTemp = g_DetectPara.AoTuMianDiam;
	
	//�������
	LCD_FillRect(60,56,40,296,atBkCol);//��
	LCD_FillRect(440,56,40-1,296,atBkCol);//��
	LCD_FillRect(60,32,420,24,atBkCol);//��
	LCD_FillRect(60,296,420,24-1,atBkCol);//��
	
	LCD_FillRect(100,56,340,36,atBarCol);//������
	LCD_SetColor(atBarCol,WHITE);
	LCD_ShowChinese(Font16,214,66,0,"��͹��������");
	
	LCD_FillRect(100,92,340,168,atDioCol);//����
	LCD_FillRect(100,92,340,1,atBkCol);//�ָ���
	
	LCD_FillRect(264,116,60,32,GRAY);//����
	LCD_FillRect(334,116,60,32,GRAY);//͹��
	LCD_FillRect(264,160,130,32,GRAY);//����ֱ��
	
	LCD_SetColor(atDioCol,atTxCol);
	LCD_ShowChinese(Font16,144,124,0,"�����");
	LCD_ShowChinese(Font16,144,168,0,"����ֱ��");
	
	LCD_SetColor(GRAY,atTxCol);
	LCD_ShowChinese(Font16,278,124,0,"����");
	LCD_ShowChinese(Font16,348,124,0,"͹��");
	
	LCD_ShowString(ASC20x10,348,168,0,"mm >");
	LCD_SetColor(GRAY,atTxCol);
	if(g_DetectPara.AoTuMianDiam == 0){
		LCD_ShowString(ASC20x10,284,168,0," -- ");
	}else{
		LCD_ShowNums(ASC20x10,284,168,0,g_DetectPara.AoTuMianDiam);
	}
	
	LCD_FillRect(100,260,170,36,GRAY);//ȷ����ť
	LCD_FillRect(270,260,170,36,GRAY);//ȡ����ť
	LCD_FillRect(100,259,340,1,atBkCol);//�ָ���
	LCD_FillRect(269,260,1,36,atBkCol);//�ָ���
	
	LCD_SetColor(GRAY,atTxCol);
	LCD_ShowChinese(Font16,168,270,0,"ȷ��");
	LCD_ShowChinese(Font16,338,270,0,"ȡ��");
	
	if(g_DetectPara.AoTuMianSta  == 0){
		LCD_DrawRect(264,116,60,32,GOLD);//����
		LCD_DrawRect(264+1,116+1,60-2,32-2,GOLD);//����
		LCD_DrawRect(334,116,60,32,GRAY);//͹��
		LCD_DrawRect(334+1,116+1,60-2,32-2,GRAY);//͹��
	}else{
		LCD_DrawRect(264,116,60,32,GRAY);//����
		LCD_DrawRect(264+1,116+1,60-2,32-2,GRAY);//����
		LCD_DrawRect(334,116,60,32,GOLD);//͹��
		LCD_DrawRect(334+1,116+1,60-2,32-2,GOLD);//͹��
	}
	
	//��ť��ʼ��
	g_u8ButtonNums = 5;
	struct BUTTON SampBtn[5] = {
		//sX,  sY,  eX,  eY,
		{100, 260, 269, 295, 0,0},//0, ȷ��
		{270, 260, 439, 295, 0,0},//1, ȡ��
		{264, 116, 323, 147, 0,0},//2, ����
		{334, 116, 393, 147, 0,0},//3, ͹��
		{264, 160, 393, 191, 0,0} //4, ����ֱ��
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_AllButtonReaction(5,GRAY,GOLD);
		
		//ȷ��
		if(g_u8TouchResult == 0){
			LCD_FillRect(100,56,340,240,atBkCol);//��յ���
			ClearTouchSta();
			return;
		}
		//ȡ��
		if(g_u8TouchResult == 1){
			LCD_FillRect(100,56,340,240,atBkCol);//��յ���
			g_DetectPara.AoTuMianSta = aotumainstaTemp;
			g_DetectPara.AoTuMianDiam = aotumaindiamTemp;
			ClearTouchSta();
			return;
		}
		
		//����
		if(g_u8TouchResult == 2){
			g_DetectPara.AoTuMianSta = 0;
			LCD_DrawRect(264,116,60,32,GOLD);//����
			LCD_DrawRect(264+1,116+1,60-2,32-2,GOLD);//����
			LCD_DrawRect(334,116,60,32,GRAY);//͹��
			LCD_DrawRect(334+1,116+1,60-2,32-2,GRAY);//͹��
		}
		//͹��
		if(g_u8TouchResult == 3){
			g_DetectPara.AoTuMianSta = 1;
			LCD_DrawRect(264,116,60,32,GRAY);//����
			LCD_DrawRect(264+1,116+1,60-2,32-2,GRAY);//����
			LCD_DrawRect(334,116,60,32,GOLD);//͹��
			LCD_DrawRect(334+1,116+1,60-2,32-2,GOLD);//͹��
		}
		//����ֱ��
		if(g_u8TouchResult == 4){
			iNumInputDialog(&g_DetectPara.AoTuMianDiam);
			AoTuMianSetInit();
		}
		
		if(g_u8TouchResult == BTN_INVALID_AREA){//�ǰ�ť������
			if(Is_DesignArea_Touch(100,56,439,295) == 0){
				LCD_FillRect(100,56,340,240,atBkCol);//��յ���
				g_DetectPara.AoTuMianSta = aotumainstaTemp;
				g_DetectPara.AoTuMianDiam = aotumaindiamTemp;
				return;
			}
		}
	}
}
//����������������
void FanXiangXZSetInit(void)
{
	u16 fxBkCol;
	u16 fxTxCol;
	u16 fxBarCol;
	u16 fxDioCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		fxBkCol = BLACK;
		fxTxCol = WHITE;
		fxBarCol = GRAY;
		fxDioCol = DGRAY;
	}else{
		fxBkCol = WHITE;
		fxTxCol = BLACK;
		fxBarCol = DGRAY;
		fxDioCol = LGRAY;
	}
	//����ˢ�¹������ý��汳��
	//״̬��
	ui_DispTitleBar("��������",1);
	//ҳ�����˵���
	MENU_TABLE_STRUCT ReBrMenu[6] = {
		{0,6,"��",NULL},
		{1,6,"��","����"},
		{2,6,"��","��ʾ"},
		{3,6,"��","����"},
		{4,6,"��","����ͼ"},
		{5,6,"��","�궨"}
	};
	//�˵���ʾ
	ui_DispMenuBtn(ReBrMenu,3);
	
	//////////////////////////////////////////////
	//����ܱ߱���
	LCD_FillRect(60,56,40,296,fxBkCol);//��
	LCD_FillRect(440,56,40-1,296,fxBkCol);//��
	LCD_FillRect(60,32,420,24,fxBkCol);//��
	LCD_FillRect(60,296,420,24-1,fxBkCol);//��
	
	//������
	LCD_FillRect(100,56,340,36,fxBarCol);
	LCD_SetColor(fxBarCol,WHITE);
	LCD_ShowChinese(Font16,206,66,0,"����������������");
	//����
	LCD_FillRect(100,92,340,168,fxDioCol);//����
	LCD_FillRect(100,92,340,1,fxBkCol);//�ָ���
	//��ť
	LCD_FillRect(100,260,170,36,GRAY);//ȷ����ť
	LCD_FillRect(270,260,170,36,GRAY);//ȡ����ť
	LCD_FillRect(100,259,340,1,fxBkCol);//�Ϸָ���
	LCD_FillRect(269,260,1,36,fxBkCol);//�зָ���
	
//	LCD_SetColor(GOLD,BLACK);
//	LCD_ShowChinese(Font16,168,270,0,"ȷ��");
//	LCD_SetColor(GRAY,fxTxCol);
//	LCD_ShowChinese(Font16,338,270,0,"ȡ��");
	LCD_SetColor(GRAY,fxTxCol);
	LCD_ShowChinese(Font16,168,270,0,"ȷ��");
	LCD_SetColor(GRAY,fxTxCol);
	LCD_ShowChinese(Font16,338,270,0,"ȡ��");
	//����
	LCD_SetColor(fxDioCol,fxTxCol);
	LCD_ShowText(Font16,110,106,0,"��׼ֵ1");
	LCD_ShowText(Font16,110,142,0,"��׼ֵ2");
	LCD_ShowText(Font16,250,106,0,"����ֵ1");
	LCD_ShowText(Font16,250,142,0,"����ֵ2");
	LCD_ShowString(ASC20x10,206+16-2,104,0,"mm");
	LCD_ShowString(ASC20x10,206+16-2,140,0,"mm");
	LCD_ShowString(ASC20x10,346+16-2,104,0,"mm");
	LCD_ShowString(ASC20x10,346+16-2,140,0,"mm");
	
	LCD_FillRect(166+8,102,40,24,GRAY);//��׼ֵ1
	LCD_FillRect(166+8,138,40,24,GRAY);//��׼ֵ2
	LCD_FillRect(306+8,102,40,24,GRAY);//����ֵ1
	LCD_FillRect(306+8,138,40,24,GRAY);//����ֵ2
	
	LCD_FillRect(390,102,40,24,GRAY);//ȡֵ1
	LCD_FillRect(390,138,40,24,GRAY);//ȡֵ2
	
	LCD_SetColor(GRAY,fxTxCol);
	LCD_ShowChinese(Font16,394,106,0,"ȡֵ");
	LCD_ShowChinese(Font16,394,142,0,"ȡֵ");
	
	//�ָ���
	LCD_FillRect(110,172,320,1,GRAY);
	
	LCD_SetColor(fxDioCol,BLUE);
	LCD_ShowText(Font16,110,176,0,"��ʾ:");
	LCD_ShowText(Font12,140,192,0,"1.�ģ�⹤��,ѡȡ���ֲ�ͬ�ĺ����Ϊ�궨��;");
	LCD_ShowText(Font12,140,208,0,"2.�ֶ������ı�������ֵ��Ϊ��׼ֵ;");
	LCD_ShowText(Font12,140,224,0,"3.ȡֵ��õ�������Ϊ����ֵ;");
	LCD_ShowText(Font12,140,240,0,"4.�����׼ֵ�Ͳ���ֵ��,���з�������������");

	LCD_SetColor(GRAY,fxTxCol);
	if(g_DetectPara.FanXiangXZPara.BiaoZhunZhi1 == 0){
		LCD_ShowText_Center(ASC16x8,166+8,102,40,24,0,"--");
	}else{
//		LCD_ShowNums(ASC16x8,188,86,0,FanXiangXZPara.BiaoZhunZhi1);
		LCD_ShowNums_Center(ASC16x8,166+8,102,40,24,0,g_DetectPara.FanXiangXZPara.BiaoZhunZhi1);
	}
	if(g_DetectPara.FanXiangXZPara.CeLiangZhi1 == 0){
		LCD_ShowText_Center(ASC16x8,166+8,138,40,24,0,"--");
	}else{
//		LCD_ShowNums(ASC16x8,188,114,0,FanXiangXZPara.CeLiangZhi1);
		LCD_ShowNums_Center(ASC16x8,166+8,138,40,24,0,g_DetectPara.FanXiangXZPara.CeLiangZhi1);
	}
	if(g_DetectPara.FanXiangXZPara.BiaoZhunZhi2 == 0){
		LCD_ShowText_Center(ASC16x8,306+8,102,40,24,0,"--");
	}else{
//		LCD_ShowNums(ASC16x8,368,86,0,FanXiangXZPara.BiaoZhunZhi2);
		LCD_ShowNums_Center(ASC16x8,306+8,102,40,24,0,g_DetectPara.FanXiangXZPara.BiaoZhunZhi2);
	}
	if(g_DetectPara.FanXiangXZPara.CeLiangZhi2 == 0){
		LCD_ShowText_Center(ASC16x8,306+8,138,40,24,0,"--");
	}else{
//		LCD_ShowNums(ASC16x8,368,114,0,FanXiangXZPara.CeLiangZhi2);
		LCD_ShowNums_Center(ASC16x8,306+8,138,40,24,0,g_DetectPara.FanXiangXZPara.CeLiangZhi2);
	}
	
	
	//��ť��ʼ��
	g_u8ButtonNums = 8;
	struct BUTTON SampBtn[8] = {
		//sX,  sY,  eX,  eY,
		{100, 260, 269, 295, 0,0},//0, ȷ��
		{270, 260, 439, 295, 0,0},//1, ȡ��
		{166,  96, 221, 131, 0,0},//2, ��׼ֵ1
		{306,  96, 371, 131, 0,0},//3, ����ֵ1
		{166, 132, 221, 167, 0,0},//4, ��׼ֵ2
		{306, 132, 371, 167, 0,0},//5, ����ֵ2
		{380,  96, 439, 131, 0,0},//6, ȡֵ1
		{380, 132, 439, 167, 0,0} //7, ȡֵ2
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
}
//����������������
void FanXiangXZSetFun(void)
{
	//��������:(100,56),width:340,height:240
//	u16 i;
	int biaozhun1,celiang1,biaozhun2,celiang2;
	
	u16 fxBkCol;
	u16 fxTxCol;
	u16 fxBarCol;
	u16 fxDioCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		fxBkCol = BLACK;
		fxTxCol = WHITE;
		fxBarCol = GRAY;
		fxDioCol = DGRAY;
	}else{
		fxBkCol = WHITE;
		fxTxCol = BLACK;
		fxBarCol = DGRAY;
		fxDioCol = LGRAY;
	}
	
	biaozhun1 = g_DetectPara.FanXiangXZPara.BiaoZhunZhi1;
	celiang1  = g_DetectPara.FanXiangXZPara.CeLiangZhi1;
	biaozhun2 = g_DetectPara.FanXiangXZPara.BiaoZhunZhi2;
	celiang2  = g_DetectPara.FanXiangXZPara.CeLiangZhi2;
	
	//״̬��
//	ui_DispTitleBar("��������",1);
	
	//�������
	LCD_FillRect(60,56,40,296,fxBkCol);//��
	LCD_FillRect(440,56,40-1,296,fxBkCol);//��
	LCD_FillRect(60,32,420,24,fxBkCol);//��
	LCD_FillRect(60,296,420,24-1,fxBkCol);//��
	
	//������
	LCD_FillRect(100,56,340,36,fxBarCol);
	LCD_SetColor(fxBarCol,WHITE);
	LCD_ShowChinese(Font16,206,66,0,"����������������");
	//����
	LCD_FillRect(100,92,340,168,fxDioCol);//����
	LCD_FillRect(100,92,340,1,fxBkCol);//�ָ���
	//��ť
	LCD_FillRect(100,260,170,36,GRAY);//ȷ����ť
	LCD_FillRect(270,260,170,36,GRAY);//ȡ����ť
	LCD_FillRect(100,259,340,1,fxBkCol);//�Ϸָ���
	LCD_FillRect(269,260,1,36,fxBkCol);//�зָ���
	
//	LCD_SetColor(GOLD,BLACK);
//	LCD_ShowChinese(Font16,168,270,0,"ȷ��");
//	LCD_SetColor(GRAY,fxTxCol);
//	LCD_ShowChinese(Font16,338,270,0,"ȡ��");
	LCD_SetColor(GRAY,fxTxCol);
	LCD_ShowChinese(Font16,168,270,0,"ȷ��");
	LCD_SetColor(GRAY,fxTxCol);
	LCD_ShowChinese(Font16,338,270,0,"ȡ��");
	//����
	LCD_SetColor(fxDioCol,fxTxCol);
	LCD_ShowText(Font16,110,106,0,"��׼ֵ1");
	LCD_ShowText(Font16,110,142,0,"��׼ֵ2");
	LCD_ShowText(Font16,250,106,0,"����ֵ1");
	LCD_ShowText(Font16,250,142,0,"����ֵ2");
	LCD_ShowString(ASC20x10,206+16-2,104,0,"mm");
	LCD_ShowString(ASC20x10,206+16-2,140,0,"mm");
	LCD_ShowString(ASC20x10,346+16-2,104,0,"mm");
	LCD_ShowString(ASC20x10,346+16-2,140,0,"mm");
	
	LCD_FillRect(166+8,102,40,24,GRAY);//��׼ֵ1
	LCD_FillRect(166+8,138,40,24,GRAY);//��׼ֵ2
	LCD_FillRect(306+8,102,40,24,GRAY);//����ֵ1
	LCD_FillRect(306+8,138,40,24,GRAY);//����ֵ2
	
	LCD_FillRect(390,102,40,24,GRAY);//ȡֵ1
	LCD_FillRect(390,138,40,24,GRAY);//ȡֵ2
	
	LCD_SetColor(GRAY,fxTxCol);
	LCD_ShowChinese(Font16,394,106,0,"ȡֵ");
	LCD_ShowChinese(Font16,394,142,0,"ȡֵ");
	
	//�ָ���
	LCD_FillRect(110,172,320,1,GRAY);
	
	LCD_SetColor(fxDioCol,BLUE);
	LCD_ShowText(Font16,110,176,0,"��ʾ:");
	LCD_ShowText(Font12,140,192,0,"1.�ģ�⹤��,ѡȡ���ֲ�ͬ�ĺ����Ϊ�궨��;");
	LCD_ShowText(Font12,140,208,0,"2.�ֶ������ı�������ֵ��Ϊ��׼ֵ;");
	LCD_ShowText(Font12,140,224,0,"3.ȡֵ��õ�������Ϊ����ֵ;");
	LCD_ShowText(Font12,140,240,0,"4.�����׼ֵ�Ͳ���ֵ��,���з�������������");

	LCD_SetColor(GRAY,fxTxCol);
	if(g_DetectPara.FanXiangXZPara.BiaoZhunZhi1 == 0){
		LCD_ShowText_Center(ASC16x8,166+8,102,40,24,0,"--");
	}else{
//		LCD_ShowNums(ASC16x8,188,86,0,FanXiangXZPara.BiaoZhunZhi1);
		LCD_ShowNums_Center(ASC16x8,166+8,102,40,24,0,g_DetectPara.FanXiangXZPara.BiaoZhunZhi1);
	}
	if(g_DetectPara.FanXiangXZPara.CeLiangZhi1 == 0){
		LCD_ShowText_Center(ASC16x8,166+8,138,40,24,0,"--");
	}else{
//		LCD_ShowNums(ASC16x8,188,114,0,FanXiangXZPara.CeLiangZhi1);
		LCD_ShowNums_Center(ASC16x8,166+8,138,40,24,0,g_DetectPara.FanXiangXZPara.CeLiangZhi1);
	}
	if(g_DetectPara.FanXiangXZPara.BiaoZhunZhi2 == 0){
		LCD_ShowText_Center(ASC16x8,306+8,102,40,24,0,"--");
	}else{
//		LCD_ShowNums(ASC16x8,368,86,0,FanXiangXZPara.BiaoZhunZhi2);
		LCD_ShowNums_Center(ASC16x8,306+8,102,40,24,0,g_DetectPara.FanXiangXZPara.BiaoZhunZhi2);
	}
	if(g_DetectPara.FanXiangXZPara.CeLiangZhi2 == 0){
		LCD_ShowText_Center(ASC16x8,306+8,138,40,24,0,"--");
	}else{
//		LCD_ShowNums(ASC16x8,368,114,0,FanXiangXZPara.CeLiangZhi2);
		LCD_ShowNums_Center(ASC16x8,306+8,138,40,24,0,g_DetectPara.FanXiangXZPara.CeLiangZhi2);
	}
	
	
	//��ť��ʼ��
	g_u8ButtonNums = 8;
	struct BUTTON SampBtn[8] = {
		//sX,  sY,  eX,  eY,
		{100, 260, 269, 295, 0,0},//0, ȷ��
		{270, 260, 439, 295, 0,0},//1, ȡ��
		{166,  96, 221, 131, 0,0},//2, ��׼ֵ1
		{306,  96, 371, 131, 0,0},//3, ����ֵ1
		{166, 132, 221, 167, 0,0},//4, ��׼ֵ2
		{306, 132, 371, 167, 0,0},//5, ����ֵ2
		{380,  96, 439, 131, 0,0},//6, ȡֵ1
		{380, 132, 439, 167, 0,0} //7, ȡֵ2
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		ui_AllButtonReaction(2,GRAY,GOLD);
		//ȷ��
		if(g_u8TouchResult == 0){
			LCD_FillRect(100,56,340,240,fxBkCol);//��յ���
			ClearTouchSta();
			return;
		}
		//ȡ��
		if(g_u8TouchResult == 1){
			LCD_FillRect(100,56,340,240,fxBkCol);//��յ���
			g_DetectPara.FanXiangXZPara.BiaoZhunZhi1 = biaozhun1;
			g_DetectPara.FanXiangXZPara.CeLiangZhi1  = celiang1;
			g_DetectPara.FanXiangXZPara.BiaoZhunZhi2 = biaozhun2;
			g_DetectPara.FanXiangXZPara.CeLiangZhi2  = celiang2;
			ClearTouchSta();
			return;
		}
		
		//��׼ֵ1
		if(g_u8TouchResult == 2){
			iNumInputDialog(&g_DetectPara.FanXiangXZPara.BiaoZhunZhi1);
			FanXiangXZSetInit();
		}
		//����ֵ1
		if(g_u8TouchResult == 3){
			iNumInputDialog(&g_DetectPara.FanXiangXZPara.CeLiangZhi1);
			FanXiangXZSetInit();
		}
		//��׼ֵ2
		if(g_u8TouchResult == 4){
			iNumInputDialog(&g_DetectPara.FanXiangXZPara.BiaoZhunZhi2);
			FanXiangXZSetInit();
		}
		//����ֵ2
		if(g_u8TouchResult == 5){
			iNumInputDialog(&g_DetectPara.FanXiangXZPara.CeLiangZhi2);
			FanXiangXZSetInit();
		}
		//ȡֵ1
		if(g_u8TouchResult == 6){
			LCD_FillRect(100,56,340,240,fxBkCol);//��յ���
			g_DetectPara.FanXiangXZPara.CeLiangZhi1 = FXXZQuZhiFun(1);
			FanXiangXZSetInit();
		}
		//ȡֵ2
		if(g_u8TouchResult == 7){
			LCD_FillRect(100,56,340,240,fxBkCol);//��յ���
			g_DetectPara.FanXiangXZPara.CeLiangZhi2 = FXXZQuZhiFun(2);
			FanXiangXZSetInit();
		}
		
		if(g_u8TouchResult == BTN_INVALID_AREA){//�ǰ�ť������
			if(Is_DesignArea_Touch(90,50,449,299) == 0){
				LCD_FillRect(100,56,340,240,fxBkCol);//��յ���
				g_DetectPara.FanXiangXZPara.BiaoZhunZhi1 = biaozhun1;
				g_DetectPara.FanXiangXZPara.CeLiangZhi1  = celiang1;
				g_DetectPara.FanXiangXZPara.BiaoZhunZhi2 = biaozhun2;
				g_DetectPara.FanXiangXZPara.CeLiangZhi2  = celiang2;
				ClearTouchSta();
				return;
			}
		}
	}
}
void FXXZBiaoDing(void)
{
	u8 i;
	u8 res;
	
	u16 fbdBkCol;
	u16 fbdTxCol;
	u16 fbdBarCol;
	u16 fbdDioCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		fbdBkCol = BLACK;
		fbdTxCol = WHITE;
		fbdBarCol = GRAY;
		fbdDioCol = DGRAY;
	}else{
		fbdBkCol = WHITE;
		fbdTxCol = BLACK;
		fbdBarCol = DGRAY;
		fbdDioCol = LGRAY;
	}
	
	//״̬��
	ui_DispTitleBar("�����궨",1);
	
	LCD_FillRect(100,56,340,36,fbdBarCol);//������
	LCD_SetColor(fbdBarCol,WHITE);
	LCD_ShowChinese(Font16,238,66,0,"�����궨");
	
	LCD_FillRect(100,92,340,168,fbdDioCol);//����
	LCD_FillRect(100,92,340,1,fbdBkCol);//�ָ���
	
	LCD_FillRect(100,260,170,36,GOLD);//ȷ����ť
	LCD_FillRect(270,260,170,36,GRAY);//ȡ����ť
	LCD_FillRect(100,259,340,1,fbdBkCol);//�ָ���
	LCD_FillRect(269,260,1,36,fbdBkCol);//�ָ���
	
	LCD_SetColor(GOLD,BLACK);
	LCD_ShowChinese(Font16,168,270,0,"�궨");
	LCD_SetColor(GRAY,fbdTxCol);
	LCD_ShowChinese(Font16,338,270,0,"ȡ��");
	
	LCD_SetColor(fbdDioCol,GOLD);
	LCD_ShowText(Font20,220,144,0,"��Զ�����");
	LCD_SetColor(fbdDioCol,fbdTxCol);
	LCD_ShowText(Font16,206,168,0,"��    ����ʼ�궨");
	LCD_SetColor(fbdDioCol,GOLD);
	LCD_ShowText(Font16,222,168,0,"�궨");
	
	//ҳ�����˵���
	MENU_TABLE_STRUCT FXXZBDMenu[2] = {
		{0,2,"��",NULL},
		{1,2,"��","�궨"}
	};
	//�˵���ʾ
	ui_DispMenuBtn(FXXZBDMenu,0);
	
	//��ť��ʼ��
	g_u8ButtonNums = 4;
	struct BUTTON SampBtn[4] = {
		//sX,  sY,  eX,  eY,
		{100, 260, 269, 295, 0,0},//0, ȷ��
		{270, 260, 439, 295, 0,0},//1, ȡ��
		{  0,   0,  59,  49, 0,0},//2, ����
		{  0,  50,  59, 103, 0,0} //3, �궨
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		
//		DispTempera();//��ʾ�¶�
		
		if(g_u16KeyValBuf == KEY_OK){

		}
		if(g_u16KeyValBuf == KEY_BACK){
			LCD_FillRect(56,32,424,288,fbdBkCol);
			return;
		}
		
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_OK)){//ȷ��
//			LCD_FillRect(56,32,424,288,bkCol);//��յ���
			
			res = Biaoding_Fun();
			
			if(res == 1){//�궨���
				LCD_FillRect(56,32,424,288,fbdBkCol);
				return;
			}else{//ȡ��
				for(i=0;i<10;i++){//�������
					ui_ButtonTask(0);
					Sleep_ms(10);
				}
				g_GlobalPara.ucFromPageNum == UI_CONDITION;
				ui_Biaoding_InitDisp();
				if(res == 2){
					//��ɫ����
					if(g_SysPara.iTheme == THEME_DARK){
						LCD_SetColor(fbdDioCol,fbdTxCol);
						LCD_ShowText(Font16,206,168,0,"��    ����ʼ�궨");
						LCD_SetColor(fbdDioCol,RED);
						LCD_ShowText(Font16,222,168,0,"�궨");
					}else{
						LCD_SetColor(fbdDioCol,fbdTxCol);
						LCD_ShowText(Font16,206,168,0,"��    ����ʼ�궨");
						LCD_SetColor(fbdDioCol,RED);
						LCD_ShowText(Font16,222,168,0,"�궨");
					}
				}
			}
		}
		if(g_u8TouchResult == 1){//ȡ��
			LCD_FillRect(56,32,424,288,fbdBkCol);
			return;
		}
		
		//����
		if(g_u8TouchResult == 2){
			LCD_FillRect(56,32,424,288,fbdBkCol);
			return;
		}
		//�궨
		if(g_u8TouchResult == 3){
			
		}
	}
}
//��������ȡֵ��num:1:ȡֵ1, 2:ȡֵ2
void FXXZQuZhiInit(u8 num)
{
	u8 i;
	u16 bkCol;
	u16 txCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		//״̬��
		LCD_FillRect(57,1,422,31,GRAY);
		//�ָ���
		LCD_FillRect(57,192,422,2,WHITE);//��
		for(i=0;i<42;i++){
			LCD_SetColor(BLACK,WHITE);
			LCD_ShowChinese(Font8,58+i*10,192,1,"��");
		}
		
		LCD_SetColor(GRAY,BLACK);
		LCD_ShowChinese(Font16,192,8,0,"��������ȡֵ");//�ֽ�����
		
		bkCol = BLACK;
		txCol = WHITE;
	}else{
		//״̬��
		LCD_FillRect(57,1,422,31,DGRAY);
		//�ָ���
		LCD_FillRect(57,192,422,2,BLACK);//��
		for(i=0;i<42;i++){
			LCD_SetColor(WHITE,BLACK);
			LCD_ShowChinese(Font8,58+i*10,192,1,"��");
		}
		
		LCD_SetColor(DGRAY,WHITE);
		LCD_ShowChinese(Font16,192,8,0,"��������ȡֵ");//�ֽ�����
		
		bkCol = WHITE;
		txCol = BLACK;
	}
	
	LCD_SetColor(bkCol,txCol);
	LCD_ShowString(ASC24x12,384,146,0,"  ");//���ֵ
	LCD_ShowString(ASC16x8,424,154,0,"m");
	LCD_ShowString(ASC16x8,434,154,0,"m");
	
	g_DetectPara.PageCurr = 1;
	g_DetectPara.MZJSite = 0;
	g_DetectPara.CarSite = 0;
	
	BrushZXX(GRAY);
	BrushMZJ(g_DetectPara.MZJSite,FAR);
	
	
	//ҳ�����˵���
	MENU_TABLE_STRUCT FXXZMenu[2] = {
		{0,2,"��",NULL},
		{1,2,"��","�궨"}
	};
	//�˵���ʾ
	ui_DispMenuBtn(FXXZMenu,0);
	
	//��ť��ʼ��
	g_u8ButtonNums = 5;
	struct BUTTON SampBtn[5] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0},//0, ����
		{  0,  50,  59, 103, 0,0},//1, �궨
		{440,  32, 479,  85, 0,0},//2, ͨ��1
		{440,  86, 479, 138, 0,0},//2, ͨ��2
		{440, 139, 479, 191, 0,0} //3, ͨ��3
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	//��ʼ������
	g_DetectPara.MZJSite = 0;
//	g_DetectPara.CarSite = 0;
	
	//ͨ��ѡ��
	g_DetectPara.Channel = 1;
	g_DetectPara.ChannelLast = 1;
	
	g_GlobalPara.iSaveDataCnt = 0;
	
//	g_SysPara.SampleDispMode = MIAO_POU;
	
	//ͨ��ѡ��
	BrushChannelSel(g_DetectPara.Channel,0);
}
//��������ȡֵ��num:1:ȡֵ1, 2:ȡֵ2
int FXXZQuZhiFun(u8 num)
{
	int houduVal;
	
	u16 fbkCol;
	u16 ftxCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		fbkCol = BLACK;
		ftxCol = WHITE;
	}else{
		fbkCol = WHITE;
		ftxCol = BLACK;
	}
	
	FXXZQuZhiInit(num);
	while(1)
	{
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		//���Ч��
//		ui_AllButtonReaction(5,LGRAY,GOLD);
//		ui_OneButtonReactionXY(0,0,0,47,33,GOLD,ORANGE);//����

//		ui_DispTime(0);//ʱ��
//		ui_DispBattery(0);//����

		//���ݲɼ�
		if(Get_Adc_Data()){
			
//			BrushCar(g_iCarDist);
//			DispCarDist(g_iCarDist);
//			
//			g_iCarDistLast = g_iCarDist;
			
			if(1){//�⵽�ֽ�����ʾ
				LCD_SetColor(fbkCol,ftxCol);
				if(num == 1){
					LCD_ShowText(Font16,126,240,0,"����ȡֵ�������ɣ�����ֵ1 =     mm");
				}else{
					LCD_ShowText(Font16,126,240,0,"����ȡֵ�������ɣ�����ֵ2 =     mm");
				}
				
				LCD_ShowNums_Center(ASC24x12,366,232,32,24,0,g_DetectPara.HouduRes);
			}
		}
		
		//����
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_OK)||(g_u16KeyValBuf == KEY_BACK)||(g_u16KeyValBuf == KEY_FN)){
			LCD_FillRect(56,32,424,288,fbkCol);
			return houduVal;
		}
		//�궨
		if((g_u8TouchResult == 1)||(g_u16KeyValBuf == LONG_KEY_FN)){
			LCD_FillRect(56,32,424,288,fbkCol);
			FXXZBiaoDing();
			FXXZQuZhiInit(num);
		}
		//ͨ��ѡ��
		if((g_u8TouchResult >= 2)&&(g_u8TouchResult <= 4)){
			g_DetectPara.Channel = g_u8TouchResult - 2;
			BrushChannelSel(g_DetectPara.Channel,0);
		}
		
		//��׼��
		if(g_u8TouchResult == SLIDE_GAP_RIGHT){//+BTN_NUM_LONGPRESS_HOLD){
			g_DetectPara.MZJSite += 2;
			BrushMZJ(g_DetectPara.MZJSite,FAR);
			g_DetectPara.CarSite += 2;
//			BrushCar(CarSite);
		}
		if(g_u8TouchResult == SLIDE_GAP_LEFT){//+BTN_NUM_LONGPRESS_HOLD){
			g_DetectPara.MZJSite -= 2;
			BrushMZJ(g_DetectPara.MZJSite,FAR);
			g_DetectPara.CarSite -= 2;
//			BrushCar(CarSite);
		}
		
		Sleep_ms(5);
	}
}
