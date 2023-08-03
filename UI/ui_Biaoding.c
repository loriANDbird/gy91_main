/*******************************************************************************
�����ߣ�
�������ڣ�2018.03.01
����������
    1. ui_Start �����ʼ��
    2. ui_Start_Run ����ʵ��

    
���¼�¼��
    1. 2018.03.01 ����
    
*******************************************************************************/
#include "ui_Start.h"
#include "Config_ui.h"

u8 Biaoding_Fun(void);
void DispTempera(void);

void ui_Biaoding_InitDisp(void)
{
	u16 bdBkCol;
	u16 bdTxCol;
	u16 bdBarCol;
	u16 bdDioCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		bdBkCol = BLACK;
		bdTxCol = WHITE;
		bdBarCol = GRAY;
		bdDioCol = DGRAY;
	}else{
		bdBkCol = WHITE;
		bdTxCol = BLACK;
		bdBarCol = DGRAY;
		bdDioCol = LGRAY;
	}
	
	//״̬��
	ui_DispTitleBar("�����궨",1);
	
	LCD_FillRect(100,56,340,36,bdBarCol);//������
	LCD_SetColor(bdBarCol,WHITE);
	LCD_ShowChinese(Font16,238,66,0,"�����궨");
	
	LCD_FillRect(100,92,340,168,bdDioCol);//����
	LCD_FillRect(100,92,340,1,bdBkCol);//�ָ���
	
	LCD_FillRect(100,260,170,36,GOLD);//ȷ����ť
	LCD_FillRect(270,260,170,36,GRAY);//ȡ����ť
	LCD_FillRect(100,259,340,1,bdBkCol);//�ָ���
	LCD_FillRect(269,260,1,36,bdBkCol);//�ָ���
	
	LCD_SetColor(GOLD,BLACK);
	LCD_ShowChinese(Font16,168,270,0,"�궨");
	LCD_SetColor(GRAY,bdTxCol);
	LCD_ShowChinese(Font16,338,270,0,"ȡ��");
	
	LCD_SetColor(bdDioCol,GOLD);
	LCD_ShowText(Font20,220,144,0,"��Զ�����");
	LCD_SetColor(bdDioCol,bdTxCol);
	LCD_ShowText(Font16,206,168,0,"��    ����ʼ�궨");
	LCD_SetColor(bdDioCol,GOLD);
	LCD_ShowText(Font16,222,168,0,"�궨");
	
	
	//ҳ�����˵���
	if(g_GlobalPara.ucFromPageNum == UI_SAMPLE10){//���ټ��
		MENU_TABLE_STRUCT BdMenu[6] = {
			{0,6,"��",NULL},
			{1,6,"��","����"},
			{2,6,"��","��ʾ"},
			{3,6,"��","����"},
			{4,6,"��","����ͼ"},
			{5,6,"��","�궨"}
		};
		//�˵���ʾ
		ui_DispMenuBtn(BdMenu,5);
		
		//��ť��ʼ��
		g_u8ButtonNums = 8;
		struct BUTTON BdBtn[8] = {
			//sX,  sY,  eX,  eY,
			{100, 260, 269, 295, 0,0},//0, ȷ��
			{270, 260, 439, 295, 0,0},//1, ȡ��
			{  0,   0,  59,  49, 0,0},//2, ����
			{  0,  50,  59, 103, 0,0},//3, ����
			{  0, 104,  59, 157, 0,0},//4, ��ʾ
			{  0, 158,  59, 211, 0,0},//5, ����
			{  0, 212,  59, 265, 0,0},//6, ����
			{  0, 266,  59, 319, 0,0} //7, �궨
		};
		memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
	}
	//ҳ�����˵���
	else if(g_GlobalPara.ucFromPageNum == UI_SAMPJGJ){//JGJ���
		MENU_TABLE_STRUCT BdMenu[5] = {
			{0,5,"��",NULL},
			{1,5,"��","����"},
			{2,5,"��","����"},
			{3,5,"��","���"},
			{4,5,"��","�궨"}
		};
		
		if(g_SysPara.JGJScanDir == 0){
			BdMenu[2].btnIcon = "��";
			BdMenu[2].btnName = "����";
		}else{
			BdMenu[2].btnIcon = "��";
			BdMenu[2].btnName = "����";
		}
		//�˵���ʾ
		ui_DispMenuBtn(BdMenu,4);
		
		//��ť��ʼ��
		g_u8ButtonNums = 7;
		struct BUTTON BdBtn[7] = {
			//sX,  sY,  eX,  eY,
			{100, 260, 269, 295, 0,0},//0, ȷ��
			{270, 260, 439, 295, 0,0},//1, ȡ��
			{  0,   0,  59,  49, 0,0},//2, ����
			{  0,  50,  59, 103, 0,0},//3, ����
			{  0, 104,  59, 157, 0,0},//4, ����
			{  0, 158,  59, 211, 0,0},//5, ���
			{  0, 212,  59, 265, 0,0} //6, �궨
		};
		memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
	}
	//ҳ�����˵���
	else if(g_GlobalPara.ucFromPageNum == UI_SAMPBLOCK){//������
		MENU_TABLE_STRUCT BdMenu[3] = {
			{0,3,"��",NULL},
			{1,3,"Ĭ","Ĭ��ֵ"},
			{2,3,"��","�궨"}
		};
		//�˵���ʾ
		ui_DispMenuBtn(BdMenu,2);
		
		//��ť��ʼ��
		g_u8ButtonNums = 5;
		struct BUTTON BdBtn[5] = {
			//sX,  sY,  eX,  eY,
			{100, 260, 269, 295, 0,0},//0, ȷ��
			{270, 260, 439, 295, 0,0},//1, ȡ��
			{  0,   0,  59,  49, 0,0},//2, ����
			{  0,  50,  59, 103, 0,0},//3, Ĭ��ֵ
			{  0, 104,  59, 157, 0,0} //4, �궨
		};
		memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
	}
	//ҳ�����˵���
	else if(g_GlobalPara.ucFromPageNum == UI_VERIFY){//�����춨
		MENU_TABLE_STRUCT BdMenu[3] = {
			{0,3,"��",NULL},
			{1,3,"��","����"},
			{2,3,"��","�궨"}
		};
		//�˵���ʾ
		ui_DispMenuBtn(BdMenu,2);
		
		//��ť��ʼ��
		g_u8ButtonNums = 5;
		struct BUTTON BdBtn[5] = {
			//sX,  sY,  eX,  eY,
			{100, 260, 269, 295, 0,0},//0, ȷ��
			{270, 260, 439, 295, 0,0},//1, ȡ��
			{  0,   0,  59,  49, 0,0},//2, ����
			{  0,  50,  59, 103, 0,0},//3, ����
			{  0, 104,  59, 157, 0,0} //4, �궨
		};
		memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
	}
	//ҳ�����˵���
	else if(g_GlobalPara.ucFromPageNum == UI_CONDITION){//����
		MENU_TABLE_STRUCT BdMenu[2] = {
			{0,2,"��",NULL},
			{1,2,"��","����"}
		};
		//�˵���ʾ
		ui_DispMenuBtn(BdMenu,2);
		
		//��ť��ʼ��
		g_u8ButtonNums = 4;
		struct BUTTON BdBtn[4] = {
			//sX,  sY,  eX,  eY,
			{100, 260, 269, 295, 0,0},//0, ȷ��
			{270, 260, 439, 295, 0,0},//1, ȡ��
			{  0,   0,  59,  49, 0,0},//2, ����
			{  0,  50,  59, 103, 0,0} //3, ����
		};
		memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
	}
}

//---------------------------------------------
//UI���к���
void ui_Biaoding_Run(void)
{
	u8 i;
	u8 res;
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
		
//		DispTempera();//��ʾ�¶�
		
		if(g_u16KeyValBuf == KEY_OK){

		}
		if(g_u16KeyValBuf == KEY_BACK){
			SendUiNo(g_GlobalPara.ucFromPageNum);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_OK)){//ȷ��
//			LCD_FillRect(56,32,424,288,bkCol);//��յ���
			
			res = Biaoding_Fun();
			
			if(res == 1){//�궨���
				
				//��¼�¶Ⱥ͵���
				g_SysPara.fTemperatureBDLast = g_GlobalPara.fTemperature;
				g_SysPara.iBatPercentBDLast = g_GlobalPara.iBatPercent;
				SaveSysPara();
				
				SendUiNo(g_GlobalPara.ucFromPageNum);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}else{//ȡ��
				for(i=0;i<10;i++){//�������
					ui_ButtonTask(0);
					Sleep_ms(10);
				}
				ui_Biaoding_InitDisp();
				if(res == 2){
					//��ɫ����
					if(g_SysPara.iTheme == THEME_DARK){
						LCD_SetColor(DGRAY,WHITE);
						LCD_ShowText(Font16,206,168,0,"��    ����ʼ�궨");
						LCD_SetColor(DGRAY,RED);
						LCD_ShowText(Font16,222,168,0,"�궨");
					}else{
						LCD_SetColor(LGRAY,BLACK);
						LCD_ShowText(Font16,206,168,0,"��    ����ʼ�궨");
						LCD_SetColor(LGRAY,RED);
						LCD_ShowText(Font16,222,168,0,"�궨");
					}
				}
			}
		}
		if(g_u8TouchResult == 1){//ȡ��
			SendUiNo(g_GlobalPara.ucFromPageNum);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		
		//����
		if(g_u8TouchResult == 2){
			SendUiNo(g_GlobalPara.ucFromPageNum);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		if(g_GlobalPara.ucFromPageNum == UI_SAMPLE10){//���ټ��
			//����
			if(g_u8TouchResult == 3){
				SendUiNo(UI_SAMPSET);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//��ʾ
			if(g_u8TouchResult == 4){
				g_GlobalPara.ucDispModepage = 0;//�����ʾ
				SendUiNo(UI_SAMPDISP);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//����
			if(g_u8TouchResult == 5){
				SendUiNo(UI_CONDITION);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//����
			if(g_u8TouchResult == 6){
				SendUiNo(UI_ANALYZE);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//�궨
			if(g_u8TouchResult == 7){
				
			}
		}
		if(g_GlobalPara.ucFromPageNum == UI_SAMPJGJ){//JGJ���
			//����
			if(g_u8TouchResult == 3){
				SendUiNo(UI_SAMPSET);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//����
			if(g_u8TouchResult == 4){
//				g_GlobalPara.ucDispModepage = 0;//�����ʾ
//				SendUiNo(UI_SAMPDISP);
//				LCD_FillRect(60,33,420,287,WHITE);
//				return;
			}
			//���
			if(g_u8TouchResult == 5){
//				SendUiNo(UI_ANALYZE);
//				LCD_FillRect(60,0,420,320,WHITE);
//				return;
			}
			//�궨
			if(g_u8TouchResult == 6){
				
			}
		}
		if(g_GlobalPara.ucFromPageNum == UI_SAMPBLOCK){//������
			//Ĭ��ֵ
			if(g_u8TouchResult == 3){
//				SendUiNo(UI_SAMPSET);
//				LCD_FillRect(60,33,480,287,WHITE);
//				return;
			}
			//�궨
			if(g_u8TouchResult == 4){
				
			}
		}
		if(g_GlobalPara.ucFromPageNum == UI_VERIFY){//�����춨
			//����
			if(g_u8TouchResult == 3){
				SendUiNo(UI_SAMPSET);
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}
			//�궨
			if(g_u8TouchResult == 4){
				
			}
		}
	}
}

u8 Biaoding_Fun(void)
{
	u8 biaodingSta = 0;//0���궨�У�1���궨��ɣ�2���궨ʧ��
	int exitCnt = 0;
	int overTimeCnt = 0;
	int ellipsisCnt = 1;
	
	int sumCnt = 0;
	int ch1ZeroSum1 = 0;
	int ch2ZeroSum2 = 0;
	int ch3ZeroSum3 = 0;
	int ch4ZeroSum4 = 0;
	int ch5ZeroSum5 = 0;
	int ch6ZeroSum6 = 0;
	
	int ch1AdcValZero = 0;
	int ch2AdcValZero = 0;
	int ch3AdcValZero = 0;
	int ch4AdcValZero = 0;
	int ch5AdcValZero = 0;
	int ch6AdcValZero = 0;
	
	u16 bdzBkCol;
	u16 bdzTxCol;
//	u16 bdzBarCol;
	u16 bdzDioCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		bdzBkCol = BLACK;
		bdzTxCol = WHITE;
//		bdzBarCol = GRAY;
		bdzDioCol = DGRAY;
	}else{
		bdzBkCol = WHITE;
		bdzTxCol = BLACK;
//		bdzBarCol = DGRAY;
		bdzDioCol = LGRAY;
	}
	
	LCD_SetColor(bdzDioCol,GOLD);
	LCD_ShowText(Font20,220,144,0,"  �궨��  ");
	LCD_ShowText(Font16,206,168,0,"                ");
	
	//��ť
	LCD_FillRect(100,260,170,36,DGOLD);//ȷ����ť
	LCD_FillRect(270,260,170,36,GRAY);//ȡ����ť
	LCD_FillRect(269,260,1,36,bdzBkCol);//�ָ���
	
	LCD_SetColor(DGOLD,DGRAY);
	LCD_ShowChinese(Font16,168,270,0,"�궨");
	LCD_SetColor(GRAY,bdzTxCol);
	LCD_ShowChinese(Font16,338,270,0,"ȡ��");
	
	//��ť��ʼ��
	g_u8ButtonNums = 1;
	struct BUTTON BdBtn[1] = {
		//sX,  sY,  eX,  eY,
		{270, 260, 439, 295, 0,0} //0, ȡ��
	};   
	memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
	
	
	//������
	LCD_FillRect(200,192,140,8,WHITE);
	
	while(1)
	{
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		
		
		//�궨�ɹ�
		if(g_u16KeyValBuf == KEY_OK){
			if(biaodingSta == 0){//�궨ȡ��
				
			}else if(biaodingSta == 1){//�궨���
				LCD_FillRect(56,32,424,288,bdzBkCol);
				g_SysPara.uiCh1Zero = ch1AdcValZero;
				g_SysPara.uiCh2Zero = ch2AdcValZero;
				g_SysPara.uiCh3Zero = ch3AdcValZero;
				g_SysPara.uiCh4Zero = ch4AdcValZero;
				g_SysPara.uiCh5Zero = ch5AdcValZero;
				g_SysPara.uiCh6Zero = ch6AdcValZero;
				SaveSysPara();
				LCD_FillRect(56,32,424,288,bdzBkCol);
				return 1;
			}else{//�궨ʧ��
				
			}
		}
		//ȡ��
		if(g_u16KeyValBuf == KEY_BACK){
			
			if(biaodingSta == 0){//�궨ȡ��
				return 0;
			}else if(biaodingSta == 1){//�궨���
				g_SysPara.uiCh1Zero = ch1AdcValZero;
				g_SysPara.uiCh2Zero = ch2AdcValZero;
				g_SysPara.uiCh3Zero = ch3AdcValZero;
				g_SysPara.uiCh4Zero = ch4AdcValZero;
				g_SysPara.uiCh5Zero = ch5AdcValZero;
				g_SysPara.uiCh6Zero = ch6AdcValZero;
				SaveSysPara();
				LCD_FillRect(56,32,424,288,bdzBkCol);
				return 1;
			}else{//�궨ʧ��
				return 2;
			}
		}
		//���ػ�ȷ��
		if((g_u8TouchResult == 0)||(g_u8TouchResult == 1)){
			
			if(biaodingSta == 0){//�궨ȡ��
				return 0;
			}else if(biaodingSta == 1){//�궨���
				g_SysPara.uiCh1Zero = ch1AdcValZero;
				g_SysPara.uiCh2Zero = ch2AdcValZero;
				g_SysPara.uiCh3Zero = ch3AdcValZero;
				g_SysPara.uiCh4Zero = ch4AdcValZero;
				g_SysPara.uiCh5Zero = ch5AdcValZero;
				g_SysPara.uiCh6Zero = ch6AdcValZero;
				SaveSysPara();
				LCD_FillRect(56,32,424,288,bdzBkCol);
				return 1;
			}else{//�궨ʧ��
				return 2;
			}
		}
		//�궨��
		if(biaodingSta == 0){
			//���ݲɼ�
			if(overTimeCnt%8 == 0){
				if(Get_Adc_DataOrig() == 1){//
					ch1ZeroSum1 += g_iCh1AdcValOrig;
					ch2ZeroSum2 += g_iCh2AdcValOrig;
					ch3ZeroSum3 += g_iCh3AdcValOrig;
					ch4ZeroSum4 += g_iCh4AdcValOrig;
					ch5ZeroSum5 += g_iCh5AdcValOrig;
					ch6ZeroSum6 += g_iCh6AdcValOrig;
					
					//������
					LCD_FillRect(200+sumCnt,192,1,8,GOLD);
//					LCD_SetColor(DGRAY,GOLD);
//					LCD_ShowNums(Font16,350,186,0,sumCnt);
					sumCnt += 1;
				}
			}
			overTimeCnt += 1;
			ellipsisCnt += 1;
			
			//�ɼ����
			if(sumCnt >= 140){
				//������
				LCD_FillRect(200+sumCnt,192,1,8,GOLD);
				Sleep_ms(200);
//				LCD_SetColor(DGRAY,GOLD);
//				LCD_ShowNums(Font16,350,186,0,sumCnt);
				
				ch1AdcValZero = ch1ZeroSum1/sumCnt;
				ch2AdcValZero = ch2ZeroSum2/sumCnt;
				ch3AdcValZero = ch3ZeroSum3/sumCnt;
				ch4AdcValZero = ch4ZeroSum4/sumCnt;
				ch5AdcValZero = ch5ZeroSum5/sumCnt;
				ch6AdcValZero = ch6ZeroSum6/sumCnt;
				
				LCD_SetColor(bdzDioCol,GREEN);
				LCD_ShowChinese(Font20,230,144,0,"�궨�ɹ�");
				
				//ˢ�°�ť
				LCD_FillRect(100,260,170,36,GOLD);//ȷ����ť
				LCD_FillRect(270,260,170,36,GRAY);//ȡ����ť
				LCD_FillRect(269,260,1,36,BLACK);//�ָ���
				
				LCD_SetColor(GOLD,BLACK);
				LCD_ShowChinese(Font16,168,270,0,"ȷ��");
				LCD_SetColor(GRAY,bdzTxCol);
				LCD_ShowChinese(Font16,338,270,0,"ȡ��");
				
				//��ť��ʼ��
				g_u8ButtonNums = 2;
				struct BUTTON BdBtn[2] = {
					//sX,  sY,  eX,  eY,
					{100, 260, 269, 295, 0,0},//0, �궨
					{270, 260, 439, 295, 0,0} //0, ȡ��
				};
				memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
				
				biaodingSta = 1;
			}
			//�궨ʧ��
			if((overTimeCnt > 1000)&&(sumCnt < 10)){
				
				LCD_SetColor(bdzDioCol,GREEN);
				LCD_ShowChinese(Font20,230,144,0,"�궨ʧ��");
				
				//ˢ�°�ť
				LCD_FillRect(100,260,170,36,GOLD);//ȷ����ť
				LCD_FillRect(270,260,170,36,GRAY);//ȡ����ť
				LCD_FillRect(269,260,1,36,bdzBkCol);//�ָ���
				
				LCD_SetColor(GOLD,BLACK);
				LCD_ShowChinese(Font16,168,270,0,"ȷ��");
				LCD_SetColor(GRAY,bdzTxCol);
				LCD_ShowChinese(Font16,338,270,0,"ȡ��");
					
				//��ť��ʼ��
				g_u8ButtonNums = 2;
				struct BUTTON BdBtn[2] = {
					//sX,  sY,  eX,  eY,
					{100, 260, 269, 295, 0,0},//0, �궨
					{270, 260, 439, 295, 0,0} //0, ȡ��
				};
				memcpy(g_ButtonBuf,BdBtn,sizeof(BdBtn));
				
				biaodingSta = 2;
			}
			//������
			
//			if(ellipsisCnt > 138){
//				ellipsisCnt = 1;
//				LCD_SetColor(WHITE,BLACK);
//				LCD_ShowString(ASC20x10,220+64,100,0,"      ");
//			}
//			if(ellipsisCnt%20 == 0){
//				LCD_SetColor(WHITE,BLACK);
//				LCD_ShowString(ASC20x10,220+64+(ellipsisCnt/20-1)*10,100,0,".");
//			}
		}
		//�궨�ɹ���ʱ�˳�
		if(biaodingSta == 1){
			exitCnt += 1;
			if(exitCnt > 500){
				g_SysPara.uiCh1Zero = ch1AdcValZero;
				g_SysPara.uiCh2Zero = ch2AdcValZero;
				g_SysPara.uiCh3Zero = ch3AdcValZero;
				g_SysPara.uiCh4Zero = ch4AdcValZero;
				g_SysPara.uiCh5Zero = ch5AdcValZero;
				g_SysPara.uiCh6Zero = ch6AdcValZero;
				SaveSysPara();
				
				LCD_FillRect(56,32,424,288,bdzBkCol); 
				return 1;
			}
		}
		//�궨ʧ��
		if(biaodingSta == 2){
			exitCnt += 1;
			if(exitCnt > 500){
				LCD_FillRect(56,32,424,288,bdzBkCol);
				return 2;
			}
		}
		Sleep_ms(10);
	}
}

//��ʾ�¶�
void DispTempera(void)
{
	char buf[12];
	if(g_bTemperaDispFlag == 1){
		g_bTemperaDispFlag = 0;
		sprintf(buf,"%5.01f��",g_GlobalPara.fTemperature);
		LCD_SetColor(LCD_ReadPoint(330,5),WHITE);
		LCD_ShowText(Font20,330,6,0,(u8*)buf);
//		LCD_ShowFloat(ASC20x10,340,6,0,5,1,g_GlobalPara.fTemperature);
	}
}
