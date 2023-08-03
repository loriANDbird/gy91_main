/*******************************************************************************
�����ߣ�
�������ڣ�2018.03.01
����������
    1. ui_Start �����ʼ��
    2. ui_Start_Run ����ʵ��

    
���¼�¼��
    1. 2018.03.01 ����
    
*******************************************************************************/
#include "ui_Sampdisp.h"
#include "Config_ui.h"

//u8 SampleDispMode;//������4����ʾģʽ0-3
//u8 AnalyzeDispMode;//��������4����ʾģʽ0-3
static u8 dispModeLast = 0;

static u8 DispModeTemp;
struct DISPOPTION *DispModeTable;

//ɨ��ģʽ��
struct DISPOPTION SampDispModeTable[] = {
	{MIAO_POU, "��", "��׼��", "��", "����ͼ"},
	{MIAO_BO , "��", "��׼��", "��", "����ͼ"},
	{TIAO_POU, "��", "����ͼ", "��", "����ͼ"},
	{TIAO_BO , "��", "����ͼ", "��", "����ͼ"}
};

struct DISPOPTION AnalyzeDispModeTABLE[] = {
	{RELI_POU, "��", "����ͼ", "��", "����ͼ"},
	{RELI_BO , "��", "����ͼ", "��", "����ͼ"},
	{TIAO_POU, "��", "����ͼ", "��", "����ͼ"},
	{TIAO_BO , "��", "����ͼ", "��", "����ͼ"},
};

void ui_Sampdisp_InitDisp(void)
{
	u8 dispNum;
	u8 i,j;
	
	//״̬��
//	if(g_SysPara.iTheme == THEME_DARK){//��ɫ����
//		LCD_FillRect(57,1,422,31,GRAY);
//		LCD_SetColor(GRAY,WHITE);
//		LCD_ShowChinese(Font16,238,9,0,"��ʾ����");
//	}else{
//		LCD_FillRect(57,1,422,31,DGRAY);
//		LCD_SetColor(DGRAY,WHITE);
//		LCD_ShowChinese(Font16,238,9,0,"��ʾ����");
//	}
	ui_DispTitleBar("��ʾ����",0);
	
	if(g_GlobalPara.ucDispModepage == 0){//���ټ��
		DispModeTemp = g_SysPara.SampleDispMode;
		DispModeTable = SampDispModeTable;
	}else{//����
		DispModeTemp = g_SysPara.AnalyzeDispMode;
		DispModeTable = AnalyzeDispModeTABLE;
	}
	
	for(i=0;i<2;i++){
		for(j=0;j<2;j++){
			dispNum = i*2+j;
			if(dispNum == DispModeTemp){
//				LCD_DrawRect(120+180*j,68+126*i,120,90,GRAY);
				LCD_FillRect(120+180*j+1,68+126*i+1,118,88,GOLD);
				LCD_SetColor(GOLD,BLACK);
			}else{
//				LCD_DrawRect(120+180*j,68+126*i,120,90,GRAY);
				LCD_FillRect(120+180*j+1,68+126*i+1,118,88,LLGRAY);
				LCD_SetColor(LLGRAY,BLACK);
			}
			LCD_ShowChinese(Font32,135+180*j,78+126*i,0,DispModeTable[dispNum].icon1);
			LCD_ShowChinese(Font16,172+180*j,86+126*i,0,DispModeTable[dispNum].Name1);
			LCD_ShowChinese(Font32,135+180*j,116+126*i,0,DispModeTable[dispNum].icon2);
			LCD_ShowChinese(Font16,172+180*j,124+126*i,0,DispModeTable[dispNum].Name2);
		}
	}
	
	if(g_GlobalPara.ucDispModepage == 0){//���ټ��
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
		ui_DispMenuBtn(Samp10Menu,2);
		
		//��ť��ʼ��
		g_u8ButtonNums = 10;
		struct BUTTON SampBtn[10] = {
			//sX,  sY,  eX,  eY,
			{  0,   0,  59,  49, 0,0},//0, ����
			{  0,  50,  59, 103, 0,0},//1, ����
			{  0, 104,  59, 157, 0,0},//2, ��ʾ
			{  0, 158,  59, 211, 0,0},//3, ����
			{  0, 212,  59, 265, 0,0},//4, ����
			{  0, 266,  59, 319, 0,0},//5, �궨
			
			{120,  68, 239, 157, 0,0},//6, ��ʾ1
			{300,  68, 419, 157, 0,0},//7, ��ʾ2
			{120, 194, 239, 284, 0,0},//8, ��ʾ3
			{300, 194, 419, 283, 0,0} //9, ��ʾ4
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}else{//����
		if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){//�������
			//ҳ�����˵���
			MENU_TABLE_STRUCT AnalyzeMenu[5] = {
				{0,5,"��",NULL},
				{1,5,"��","��ʾ"},
				{2,5,"��","����"},
				{3,5,"��","����"},
				{4,5,"ͳ","ͳ��"}
			};
			//�˵���ʾ
			ui_DispMenuBtn(AnalyzeMenu,1);
		
			//��ť��ʼ��
			g_u8ButtonNums = 9;
			struct BUTTON SampBtn[9] = {
				//sX,  sY,  eX,  eY,
				{  0,   0,  59,  49, 0,0},//0, ����
				{  0,  50,  59, 103, 0,0},//1, ��ʾ
				{  0, 104,  59, 157, 0,0},//2, ����
				{  0, 158,  59, 211, 0,0},//3, ����
				{  0, 212,  59, 265, 0,0},//4, �궨 ��Ч
				{120,  68, 239, 157, 0,0},//5, ��ʾ1
				{300,  68, 419, 157, 0,0},//6, ��ʾ2
				{120, 194, 239, 284, 0,0},//7, ��ʾ3
				{300, 194, 419, 283, 0,0} //8, ��ʾ4
			};
			memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
		}else{
			//ҳ�����˵���
			MENU_TABLE_STRUCT Samp10Menu[4] = {
				{0,4,"��",NULL},
				{1,4,"��","��ʾ"},
				{2,4,"��","����"},
				{3,4,"��","����"}
			};
			//�˵���ʾ
			ui_DispMenuBtn(Samp10Menu,1);
			
			//��ť��ʼ��
			g_u8ButtonNums = 9;
			struct BUTTON SampBtn[9] = {
				//sX,  sY,  eX,  eY,
				{  0,   0,  59,  49, 0,0},//0, ����
				{  0,  50,  59, 103, 0,0},//1, ��ʾ
				{  0, 104,  59, 157, 0,0},//2, ����
				{  0, 158,  59, 211, 0,0},//3, ����
				{  0, 212,  59, 265, 0,0},//4, �궨 ��Ч
				{120,  68, 239, 157, 0,0},//5, ��ʾ1
				{300,  68, 419, 157, 0,0},//6, ��ʾ2
				{120, 194, 239, 284, 0,0},//7, ��ʾ3
				{300, 194, 419, 283, 0,0} //8, ��ʾ4
			};
			memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
		}
	}
	
	dispModeLast = DispModeTemp;
}

//---------------------------------------------
//UI���к���
void ui_Sampdisp_Run(void)
{	
	u8 i,j;
	
	u16 bkCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
	}else{
		bkCol = WHITE;
	}
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		
		if(g_u16KeyValBuf == KEY_OK){

		}
		if(g_u16KeyValBuf == KEY_BACK){
			SendUiNo(UI_SAMPLE10);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		
		if(g_GlobalPara.ucDispModepage == 0){//���ټ��
			if((g_u8TouchResult >= 6)&&(g_u8TouchResult <= 9)){
				DispModeTemp = g_u8TouchResult-6;
				if(DispModeTemp != dispModeLast){
					if(dispModeLast < 2){
						i = 0;
						j = dispModeLast%2;
					}else{
						i = 1;
						j = dispModeLast%2;
					}
					
					LCD_FillRect(120+180*j+1,68+126*i+1,118,88,LLGRAY);
					LCD_SetColor(LLGRAY,BLACK);
					LCD_ShowChinese(Font32,135+180*j,78+126*i,0,DispModeTable[dispModeLast].icon1);
					LCD_ShowChinese(Font16,172+180*j,86+126*i,0,DispModeTable[dispModeLast].Name1);
					LCD_ShowChinese(Font32,135+180*j,116+126*i,0,DispModeTable[dispModeLast].icon2);
					LCD_ShowChinese(Font16,172+180*j,124+126*i,0,DispModeTable[dispModeLast].Name2);
					
					if(DispModeTemp < 2){
						i = 0;
						j = DispModeTemp%2;
					}else{
						i = 1;
						j = DispModeTemp%2;
					}
					LCD_FillRect(120+180*j+1,68+126*i+1,118,88,GOLD);
					LCD_SetColor(GOLD,BLACK);
					LCD_ShowChinese(Font32,135+180*j,78+126*i,0,DispModeTable[DispModeTemp].icon1);
					LCD_ShowChinese(Font16,172+180*j,86+126*i,0,DispModeTable[DispModeTemp].Name1);
					LCD_ShowChinese(Font32,135+180*j,116+126*i,0,DispModeTable[DispModeTemp].icon2);
					LCD_ShowChinese(Font16,172+180*j,124+126*i,0,DispModeTable[DispModeTemp].Name2);
				}
				dispModeLast = DispModeTemp;
				if(g_GlobalPara.ucDispModepage == 0){//���ټ��
					g_SysPara.SampleDispMode = DispModeTemp;
				}else{//������ʾ
					g_SysPara.AnalyzeDispMode = DispModeTemp;
				}
				
				SaveSysPara();
				//ֱ�ӷ���
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
//				SendUiNo(UI_ANALYZE);
//				LCD_FillRect(60,33,420,287,WHITE);
			}
			//����
			if(g_u8TouchResult == 3){
				SendUiNo(UI_CONDITION);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
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
		}else{//����
			if((g_u8TouchResult >= 5)&&(g_u8TouchResult <= 8)){
				DispModeTemp = g_u8TouchResult-5;
				if(DispModeTemp != dispModeLast){
					if(dispModeLast < 2){
						i = 0;
						j = dispModeLast%2;
					}else{
						i = 1;
						j = dispModeLast%2;
					}
					
					LCD_FillRect(120+180*j+1,68+126*i+1,118,88,LLGRAY);
					LCD_SetColor(LLGRAY,BLACK);
					LCD_ShowChinese(Font32,135+180*j,78+126*i,0,DispModeTable[dispModeLast].icon1);
					LCD_ShowChinese(Font16,172+180*j,86+126*i,0,DispModeTable[dispModeLast].Name1);
					LCD_ShowChinese(Font32,135+180*j,116+126*i,0,DispModeTable[dispModeLast].icon2);
					LCD_ShowChinese(Font16,172+180*j,124+126*i,0,DispModeTable[dispModeLast].Name2);
					
					if(DispModeTemp < 2){
						i = 0;
						j = DispModeTemp%2;
					}else{
						i = 1;
						j = DispModeTemp%2;
					}
					LCD_FillRect(120+180*j+1,68+126*i+1,118,88,GOLD);
					LCD_SetColor(GOLD,BLACK);
					LCD_ShowChinese(Font32,135+180*j,78+126*i,0,DispModeTable[DispModeTemp].icon1);
					LCD_ShowChinese(Font16,172+180*j,86+126*i,0,DispModeTable[DispModeTemp].Name1);
					LCD_ShowChinese(Font32,135+180*j,116+126*i,0,DispModeTable[DispModeTemp].icon2);
					LCD_ShowChinese(Font16,172+180*j,124+126*i,0,DispModeTable[DispModeTemp].Name2);
				}
				dispModeLast = DispModeTemp;
				if(g_GlobalPara.ucDispModepage == 0){//���ټ��
					g_SysPara.SampleDispMode = DispModeTemp;
				}else{//������ʾ
					g_SysPara.AnalyzeDispMode = DispModeTemp;
				}
				
				SaveSysPara();
				//ֱ�ӷ���
				SendUiNo(UI_ANALYZE);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//����
			if(g_u8TouchResult == 0){
				SendUiNo(UI_ANALYZE);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//��ʾ
			if(g_u8TouchResult == 1){
//				SendUiNo(UI_ANALYZE);
//				LCD_FillRect(60,33,420,287,WHITE);
			}
			//����
			if(g_u8TouchResult == 2){
				SendUiNo(UI_GAIN);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//����
			if(g_u8TouchResult == 3){
				SendUiNo(UI_HEAT);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//ͳ��
			if(g_u8TouchResult == 4){
				if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){
					LCD_FillRect(56,32,424,288,bkCol);
					SampleDataView_Fun();
					ui_Sampdisp_InitDisp();
				}
			}
		}
	}
}
