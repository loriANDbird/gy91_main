/*******************************************************************************
�����ߣ�
�������ڣ�2018.03.01
����������
    1. ui_Main �����ʼ����
    2. ui_Main_Run ����ʵ��

���¼�¼��
    1. 2018.03.01 ���δ���
    
*******************************************************************************/
#include "ui_Main.h"
#include "Config_ui.h"

//static u8 HighLightID = 0xff;
//static u8 HighLightID_Last = 0xff;

struct ICON_STRUCT{
	unsigned char id;
	unsigned char* name;
	const unsigned short *addr;//ͼ���ַ
	const unsigned short *addrHL;//����ͼ���ַ
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
	unsigned short bkx;
	unsigned short bky;
	unsigned short bkWidth;
	unsigned short bkHeight;
};

struct ICON_STRUCT Icon_UiMain[6] = {
	{0,(u8*)"���ټ��",(const unsigned short*)gImage_detect,(const unsigned short*)gImage_detect_hl, 58, 76,60,60,  16, 56,144,120},
	{1,(u8*)"JGJ���", (const unsigned short*)gImage_jgj,   (const unsigned short*)gImage_jgj_hl,   210, 76,60,60, 168, 56,144,120},
	{2,(u8*)"������",(const unsigned short*)gImage_block, (const unsigned short*)gImage_block_hl, 362, 76,60,60, 320, 56,144,120},
	{3,(u8*)"�������",(const unsigned short*)gImage_browse,(const unsigned short*)gImage_browse_hl, 58,204,60,60,  16,184,144,120},
	{4,(u8*)"�����춨",(const unsigned short*)gImage_verify,(const unsigned short*)gImage_verify_hl,210,204,60,60, 168,184,144,120},
	{5,(u8*)"ϵͳ����",(const unsigned short*)gImage_sysset,(const unsigned short*)gImage_sysset_hl,362,204,60,60, 320,184,144,120}
};

void Water_Light(u8 sta);

//-------------------------------------------------------------------------------
//UI��ʼ������
void ui_Main_InitDisp(void)
{
	u8 i;
	
	u16 sBkCol;
	u16 sTxCol;
	u16 sBarCol;
	u16 sBtnCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		sBkCol = BLACK;
		sTxCol = WHITE;
		sBarCol = GRAY;
		sBtnCol = DGRAY;
	}else{
		sBkCol = LGRAY;
		sTxCol = BLACK;
		sBarCol = DGRAY;
		sBtnCol = WHITE;
	}
	//״̬��
	LCD_FillRect(0,0,480,40,sBarCol);
	//����
//	LCD_FillRect(0,40,480,280,sBkCol);
	//�����ť�ܱ�����
	LCD_FillRect(0,40,480,16,sBkCol);//����
	LCD_FillRect(16,176,448,8,sBkCol);//����
	LCD_FillRect(0,304,480,16,sBkCol);//����
	LCD_FillRect(0,56,16,248,sBkCol);//����
	LCD_FillRect(464,56,16,248,sBkCol);//����
	LCD_FillRect(160,56,8,248,sBkCol);//������
	LCD_FillRect(312,56,8,248,sBkCol);//������
	
	ui_DispBattery(1);//��ʾ����
	ui_DispDate();//��ʾ����
	ui_DispTime(1);//��ʾʱ��
	ui_DispGPSIcon(1);

	//��ť��ʼ��
	g_u8ButtonNums = 7;
	struct BUTTON SampBtn[7] = {
		//sX,  sY,  eX,  eY,
		{  0,  32, 159, 175, 0,0},//0,���ټ��
		{160,  32, 319, 175, 0,0},//1,JGJ���
		{320,  32, 479, 175, 0,0},//2,������
		{  0, 176, 159, 319, 0,0},//3,�������
		{160, 176, 319, 319, 0,0},//4,�����궨
		{320, 176, 479, 319, 0,0},//5,ϵͳ����
		{400, 0, 479, 100, 0,0},//5,ϵͳ����
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	if(g_SysPara.iTheme == THEME_DARK){
		Icon_UiMain[0].addr = (const unsigned short*)gImage_detect;
		Icon_UiMain[1].addr = (const unsigned short*)gImage_jgj;
		Icon_UiMain[2].addr = (const unsigned short*)gImage_block;
		Icon_UiMain[3].addr = (const unsigned short*)gImage_browse;
		Icon_UiMain[4].addr = (const unsigned short*)gImage_verify;
		Icon_UiMain[5].addr = (const unsigned short*)gImage_sysset;
	}else{
		Icon_UiMain[0].addr = (const unsigned short*)gImage_detect_light;
		Icon_UiMain[1].addr = (const unsigned short*)gImage_jgj_light;
		Icon_UiMain[2].addr = (const unsigned short*)gImage_block_light;
		Icon_UiMain[3].addr = (const unsigned short*)gImage_browse_light;
		Icon_UiMain[4].addr = (const unsigned short*)gImage_verify_light;
		Icon_UiMain[5].addr = (const unsigned short*)gImage_sysset_light;
	}
//	for(i=0;i<6;i++){
//		if(i == HighLightID){
//			HighLightID_Last = HighLightID;
//			LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].bky,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].bkHeight,LGRAY);
//			LCD_ShowPicture(Icon_UiMain[i].x,Icon_UiMain[i].y,Icon_UiMain[i].width,Icon_UiMain[i].height,Icon_UiMain[i].addrHL);
//			LCD_SetColor(LGRAY,BLACK);
//		}else{
//			LCD_ShowPicture(Icon_UiMain[i].x,Icon_UiMain[i].y,Icon_UiMain[i].width,Icon_UiMain[i].height,Icon_UiMain[i].addr);
//			LCD_SetColor(WHITE,BLACK);
//		}
//		
//		if(i == 1){//JGJ
//			LCD_ShowText(Font16,Icon_UiMain[i].x+4,Icon_UiMain[i].y+64+4,0,Icon_UiMain[i].name);
//		}else{
//			LCD_ShowText(Font16,Icon_UiMain[i].x,Icon_UiMain[i].y+64+4,0,Icon_UiMain[i].name);
//		}
//	}
//	if(HighLightID_Last > 5)HighLightID_Last = 0;
	
	for(i=0;i<6;i++){
		LCD_FillRect(Icon_UiMain[i].bkx,Icon_UiMain[i].bky,Icon_UiMain[i].bkWidth,Icon_UiMain[i].bkHeight,sBtnCol);
		LCD_SetColor(sBtnCol,sTxCol);
		LCD_ShowText_Center(Font16,Icon_UiMain[i].bkx,Icon_UiMain[i].y+Icon_UiMain[i].height,Icon_UiMain[i].bkWidth,Icon_UiMain[i].bky+Icon_UiMain[i].bkHeight-(Icon_UiMain[i].y+Icon_UiMain[i].height),0,(char*)Icon_UiMain[i].name);
		LCD_ShowPicture(Icon_UiMain[i].x,Icon_UiMain[i].y,Icon_UiMain[i].width,Icon_UiMain[i].height,Icon_UiMain[i].addr);

//		if(i == 1){//JGJ
//			LCD_ShowText(Font16,Icon_UiMain[i].x+4,Icon_UiMain[i].y+64+8,0,Icon_UiMain[i].name);
//		}else{
//			LCD_ShowText(Font16,Icon_UiMain[i].x,Icon_UiMain[i].y+64+8,0,Icon_UiMain[i].name);
//		}
	}
}

//UI���к���
void ui_Main_Run(void)
{
	u8 i;
	u8 id = 0xff;
	u8 ledSta = 0;
	u8 wlSta = 0;
	u8 HighLightID = 0xff;
	
	u16 sBkCol;
	u16 sTxCol;
	u16 sBtnCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		sBkCol = BLACK;
		sTxCol = WHITE;
		sBtnCol = DGRAY;
	}else{
		sBkCol = LGRAY;
		sTxCol = BLACK;
		sBtnCol = WHITE;
	}
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		
		ui_DispTime(0);//��ʾʱ��
		ui_DispBattery(0);//��ʾ����
		ui_GetTempera();//�ɼ��¶�
		ui_DispTempera();//��ʾ�¶�
		GPS_Location_Data();//GPS���
		ui_DispGPSIcon(0);//��ʾGPS
		ui_DispBluetooth();//��ʾ����ͼ��
		//����
		if(g_SysPara.iBlueToothSta == 1){
			BT_Lookup();
		}
		
		//�л�����
		if(g_u16KeyValBuf == KEY_OK){
			if(g_SysPara.iTheme == THEME_DARK){
				sBkCol = BLACK;
			}else{
				sBkCol = WHITE;
			}
			//���ټ��
			if(id == 0){
				SendUiNo(UI_SAMPLE10);
				LCD_FillRect(56,32,424,288,sBkCol);
				g_GlobalPara.ucFromPageNum = UI_MAIN;
				return;
			}
			//JGJ
			if(id == 1){
				SendUiNo(UI_SAMPJGJ);
				LCD_FillRect(56,32,424,288,sBkCol);
				g_GlobalPara.ucFromPageNum = UI_MAIN;
				return;
			}
			//������
			if(id == 2){
				SendUiNo(UI_SAMPBLOCK);
				LCD_FillRect(56,0,424,320,sBkCol);
				g_GlobalPara.ucFromPageNum = UI_MAIN;
				return;
			}
			//�������
			if(id == 3){
				SendUiNo(UI_BROWSE10);
				LCD_FillRect(56,32,424,288,sBkCol);
				g_GlobalPara.ucFromPageNum = UI_MAIN;
				return;
			}
			//�����춨
			if(id == 4){
				SendUiNo(UI_VERIFY);
				LCD_FillRect(56,32,424,288,sBkCol);
				g_GlobalPara.ucFromPageNum = UI_MAIN;
				return;
			}
			//ϵͳ����
			if(id == 5){
				SendUiNo(UI_PARASET10);
				LCD_FillRect(56,32,424,288,sBkCol);
				g_GlobalPara.ucFromPageNum = UI_MAIN;
				return;
			}
		}
		//����
		if(g_u16KeyValBuf == KEY_BACK){
			ledSta = 0;
			wlSta++;
			if(wlSta > 4){
				wlSta = 0;
			}
			LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
			LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
			GPS_ManualStart();//��ʼ�ֶ���λ
			
//			LCD_FillRect(0,0,480,320,sBkCol);
//			PromptDialog(id++);
//			if(id>12)id = 0;
		}
		Water_Light(wlSta);
		//�л�
		if(g_u16KeyValBuf == KEY_FN){
			USART_SendData(BT_UART,'A');
			//LED��ʾЧ��
			wlSta = 0;
			ledSta++;
			
			if((ledSta >= 1)&&(ledSta <= 3)){
				LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
				LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
				LED_Ctrl(LED_RED,ledSta,LED_STA_ON);
			}
			if((ledSta >= 4)&&(ledSta <= 6)){
				LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
				LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
				LED_Ctrl(LED_BLUE,ledSta-3,LED_STA_ON);
			}
			if(ledSta == 7){
				LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_ON);
				LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
			}
			if(ledSta == 8){
				LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
				LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_ON);
			}
			if(ledSta == 9){
				LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_ON);
				LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_ON);
			}
			if(ledSta >= 10){
				ledSta = 0;
				LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
				LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
			}
			
			//��ɫ����
			if(g_SysPara.iTheme == THEME_DARK){
				sBtnCol = DGRAY;
				sTxCol = WHITE;
			}else{
				sBtnCol = WHITE;
				sTxCol = BLACK;
			}
			//�������
			if(HighLightID < 6){
				LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].bky,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].y-Icon_UiMain[HighLightID].bky,sBtnCol);
//				LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].bky+Icon_UiMain[HighLightID].bkHeight-(Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height),WHITE);
				LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].y,Icon_UiMain[HighLightID].x-Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].height,sBtnCol);
				LCD_FillRect(Icon_UiMain[HighLightID].x+Icon_UiMain[HighLightID].width,Icon_UiMain[HighLightID].y,Icon_UiMain[HighLightID].bkx+Icon_UiMain[HighLightID].bkWidth-(Icon_UiMain[HighLightID].x+Icon_UiMain[HighLightID].width),Icon_UiMain[HighLightID].height,sBtnCol);
//				LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].bky,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].bkHeight,WHITE);
				LCD_SetColor(sBtnCol,sTxCol);
				LCD_ShowText_Center(Font16,Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].bky+Icon_UiMain[HighLightID].bkHeight-(Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height),0,(char*)Icon_UiMain[HighLightID].name);
				LCD_ShowPicture(Icon_UiMain[HighLightID].x,Icon_UiMain[HighLightID].y,Icon_UiMain[HighLightID].width,Icon_UiMain[HighLightID].height,Icon_UiMain[HighLightID].addr);
			}
			//
			if(id < 6){
				id++;
			}
			if(id > 5) id = 0;
			
			HighLightID = id;

			sBtnCol = GOLD;
			sTxCol = BLACK;
			//��ʾ����
			LCD_FillRect(Icon_UiMain[id].bkx,Icon_UiMain[id].bky,Icon_UiMain[id].bkWidth,Icon_UiMain[id].y-Icon_UiMain[id].bky,sBtnCol);
//			LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].bky+Icon_UiMain[HighLightID].bkHeight-(Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height),LGRAY);
			LCD_FillRect(Icon_UiMain[id].bkx,Icon_UiMain[id].y,Icon_UiMain[id].x-Icon_UiMain[id].bkx,Icon_UiMain[id].height,sBtnCol);
			LCD_FillRect(Icon_UiMain[id].x+Icon_UiMain[id].width,Icon_UiMain[id].y,Icon_UiMain[id].bkx+Icon_UiMain[id].bkWidth-(Icon_UiMain[id].x+Icon_UiMain[id].width),Icon_UiMain[id].height,sBtnCol);
			LCD_SetColor(sBtnCol,sTxCol);
			LCD_ShowText_Center(Font16,Icon_UiMain[id].bkx,Icon_UiMain[id].y+Icon_UiMain[id].height,Icon_UiMain[id].bkWidth,Icon_UiMain[id].bky+Icon_UiMain[id].bkHeight-(Icon_UiMain[id].y+Icon_UiMain[id].height),0,(char*)Icon_UiMain[id].name);
			LCD_ShowPicture(Icon_UiMain[id].x,Icon_UiMain[id].y,Icon_UiMain[id].width,Icon_UiMain[id].height,Icon_UiMain[id].addrHL);

		}
		//���ͼƬ
		if(g_u16KeyValBuf == LONG_KEY_FN){
			Touch_Beep_On();
			SendUiNo(UI_START);
			LCD_FillRect(0,0,480,320,WHITE);
			return;
		}
		
		//ͼ�������ʾ
		for(i=0;i<6;i++){
			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_DOWN){
				g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_HOLD;
				//��ɫ����
				if(g_SysPara.iTheme == THEME_DARK){
					sBtnCol = DGRAY;
					sTxCol = WHITE;
				}else{
					sBtnCol = WHITE;
					sTxCol = BLACK;
				}
				//�����������
				if(id < 6){
					LCD_FillRect(Icon_UiMain[id].bkx,Icon_UiMain[id].bky,Icon_UiMain[id].bkWidth,Icon_UiMain[id].y-Icon_UiMain[id].bky,sBtnCol);
//					LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].bky+Icon_UiMain[HighLightID].bkHeight-(Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height),WHITE);
					LCD_FillRect(Icon_UiMain[id].bkx,Icon_UiMain[id].y,Icon_UiMain[id].x-Icon_UiMain[id].bkx,Icon_UiMain[id].height,sBtnCol);
					LCD_FillRect(Icon_UiMain[id].x+Icon_UiMain[id].width,Icon_UiMain[id].y,Icon_UiMain[id].bkx+Icon_UiMain[id].bkWidth-(Icon_UiMain[id].x+Icon_UiMain[id].width),Icon_UiMain[id].height,sBtnCol);
//					LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].bky,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].bkHeight,WHITE);
					LCD_SetColor(sBtnCol,sTxCol);
					LCD_ShowText_Center(Font16,Icon_UiMain[id].bkx,Icon_UiMain[id].y+Icon_UiMain[id].height,Icon_UiMain[id].bkWidth,Icon_UiMain[id].bky+Icon_UiMain[id].bkHeight-(Icon_UiMain[id].y+Icon_UiMain[id].height),0,(char*)Icon_UiMain[id].name);
					LCD_ShowPicture(Icon_UiMain[id].x,Icon_UiMain[id].y,Icon_UiMain[id].width,Icon_UiMain[id].height,Icon_UiMain[id].addr);
				
					id = 0xff;
				}
				
				//
				HighLightID = i;
//				if(g_SysPara.iTheme == THEME_DARK){
					sBtnCol = GOLD;
					sTxCol = BLACK;
//				}else{
//					BkClr = GOLD;
//					TxClr = BLACK;
//				}
				//������ʾ
				LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].bky,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].y-Icon_UiMain[HighLightID].bky,sBtnCol);
//				LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].bky+Icon_UiMain[HighLightID].bkHeight-(Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height),LGRAY);
				LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].y,Icon_UiMain[HighLightID].x-Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].height,sBtnCol);
				LCD_FillRect(Icon_UiMain[HighLightID].x+Icon_UiMain[HighLightID].width,Icon_UiMain[HighLightID].y,Icon_UiMain[HighLightID].bkx+Icon_UiMain[HighLightID].bkWidth-(Icon_UiMain[HighLightID].x+Icon_UiMain[HighLightID].width),Icon_UiMain[HighLightID].height,sBtnCol);
				LCD_SetColor(sBtnCol,sTxCol);
				LCD_ShowText_Center(Font16,Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].bky+Icon_UiMain[HighLightID].bkHeight-(Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height),0,(char*)Icon_UiMain[HighLightID].name);
				LCD_ShowPicture(Icon_UiMain[HighLightID].x,Icon_UiMain[HighLightID].y,Icon_UiMain[HighLightID].width,Icon_UiMain[HighLightID].height,Icon_UiMain[HighLightID].addrHL);

//				if(HighLightID == 1){//JGJ
//					LCD_ShowText(Font16,Icon_UiMain[HighLightID].x+4,Icon_UiMain[HighLightID].y+64+8,0,Icon_UiMain[HighLightID].name);
//				}else{
//					LCD_ShowText(Font16,Icon_UiMain[HighLightID].x,Icon_UiMain[HighLightID].y+64+8,0,Icon_UiMain[HighLightID].name);
//				}
			}
			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_UP){
				g_ButtonBuf[i].btSta = BTN_STATUS_NONE;
				HighLightID = i;
				if(g_SysPara.iTheme == THEME_DARK){
					sBtnCol = DGRAY;
					sTxCol = WHITE;
				}else{
					sBtnCol = WHITE;
					sTxCol = BLACK;
				}
				LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].bky,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].y-Icon_UiMain[HighLightID].bky,sBtnCol);
//				LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].bky+Icon_UiMain[HighLightID].bkHeight-(Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height),WHITE);
				LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].y,Icon_UiMain[HighLightID].x-Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].height,sBtnCol);
				LCD_FillRect(Icon_UiMain[HighLightID].x+Icon_UiMain[HighLightID].width,Icon_UiMain[HighLightID].y,Icon_UiMain[HighLightID].bkx+Icon_UiMain[HighLightID].bkWidth-(Icon_UiMain[HighLightID].x+Icon_UiMain[HighLightID].width),Icon_UiMain[HighLightID].height,sBtnCol);
//				LCD_FillRect(Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].bky,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].bkHeight,WHITE);
				LCD_SetColor(sBtnCol,sTxCol);
				LCD_ShowText_Center(Font16,Icon_UiMain[HighLightID].bkx,Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height,Icon_UiMain[HighLightID].bkWidth,Icon_UiMain[HighLightID].bky+Icon_UiMain[HighLightID].bkHeight-(Icon_UiMain[HighLightID].y+Icon_UiMain[HighLightID].height),0,(char*)Icon_UiMain[HighLightID].name);
				LCD_ShowPicture(Icon_UiMain[HighLightID].x,Icon_UiMain[HighLightID].y,Icon_UiMain[HighLightID].width,Icon_UiMain[HighLightID].height,Icon_UiMain[HighLightID].addr);

//				if(HighLightID == 1){//JGJ
//					LCD_ShowText(Font16,Icon_UiMain[HighLightID].x+4,Icon_UiMain[HighLightID].y+64+8,0,Icon_UiMain[HighLightID].name);
//				}else{
//					LCD_ShowText(Font16,Icon_UiMain[HighLightID].x,Icon_UiMain[HighLightID].y+64+8,0,Icon_UiMain[HighLightID].name);
//				}
			}
		}
		
		//���ͼ���������ת����
		if(g_u8TouchResult <= 5){
			if(g_SysPara.iTheme == THEME_DARK){
				sBkCol = BLACK;
			}else{
				sBkCol = WHITE;
			}
			//���ټ��
			if(g_u8TouchResult == 0){
				SendUiNo(UI_SAMPLE10);
				LCD_FillRect(56,32,424,288,sBkCol);
				g_GlobalPara.ucFromPageNum = UI_MAIN;
				return;
			}
			//JGJ
			if(g_u8TouchResult == 1){
				SendUiNo(UI_SAMPJGJ);
				LCD_FillRect(56,32,424,288,sBkCol);
				g_GlobalPara.ucFromPageNum = UI_MAIN;
				return;
			}
			//������
			if(g_u8TouchResult == 2){
				SendUiNo(UI_SAMPBLOCK);
				LCD_FillRect(56,0,424,320,sBkCol);
				g_GlobalPara.ucFromPageNum = UI_MAIN;
				return;
			}
			//�������
			if(g_u8TouchResult == 3){
				SendUiNo(UI_BROWSE10);
				LCD_FillRect(56,32,424,288,sBkCol);
				g_GlobalPara.ucFromPageNum = UI_MAIN;
				return;
			}
			//�����춨
			if(g_u8TouchResult == 4){
				SendUiNo(UI_VERIFY);
				LCD_FillRect(56,32,424,288,sBkCol);
				g_GlobalPara.ucFromPageNum = UI_MAIN;
				return;
			}
			//ϵͳ����
			if(g_u8TouchResult == 5){
				SendUiNo(UI_PARASET10);
				LCD_FillRect(56,32,424,288,sBkCol);
				g_GlobalPara.ucFromPageNum = UI_MAIN;
				return;
			}
		}
		Sleep_ms(5);
		     //ADC�ɼ�
		Get_Adc_Data();
	}
}

void Water_Light(u8 sta)
{
	static u16 timCnt = 0;
	static u16 ledCnt = 0;
	if(sta != 0){
		timCnt++;
		if(timCnt > 60000)timCnt = 0;
		if(timCnt%20 == 0){
			ledCnt += 1;
			
			if(sta == 1){
				if(ledCnt <= 3){
					LED_Ctrl(LED_RED,ledCnt,LED_STA_ON);
				}
				if(ledCnt > 3){
					ledCnt = 0;
					LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
					LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
				}
			}
			if(sta == 2){
				if(ledCnt <= 3){
					LED_Ctrl(LED_BLUE,ledCnt,LED_STA_ON);
				}
				if(ledCnt > 3){
					ledCnt = 0;
					LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
					LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
				}
			}
			if(sta == 3){
				if(ledCnt == 1){
					LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_ON);
				}
				if(ledCnt == 2){
					LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
				}
				if(ledCnt > 2){
					ledCnt = 0;
				}
			}
			if(sta == 4){
				if(ledCnt == 1){
					LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_ON);
				}
				if(ledCnt == 2){
					LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
				}
				if(ledCnt > 2){
					ledCnt = 0;
				}
			}
		}
	}else{
		timCnt = 0;
		ledCnt = 0;
	}
}
