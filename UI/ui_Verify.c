/*******************************************************************************
�����ߣ�
�������ڣ�2018.03.01
�������������ݴ���
    1. ui_Upload �����ʼ����
    2. ui_Upload_Run ����ʵ��

    
���¼�¼��
    1. 2018.03.01 ���δ���
*******************************************************************************/
#include "ui_Verify.h"
#include "Config_ui.h"
#include "AimLib.h"

extern char *ChName[3];
//extern u8 DataPage;
//extern short MZJSite;
//extern short CarSite;
//extern u8 Channel;//ͨ���� 0,1,2

SYS_PARA_TABLE_STRUCT* VerifyParaTable = &ParaTable[PARATABLEINDEX_VERIFY];

extern int GetSampDataCnt(void);
extern void SaveSampFile(void);
void VerifyParaSet_Fun(SYS_PARA_TABLE_STRUCT* VerParaTable);

u8 SaveFlag;
u8 DispModeTemp;
FRESULT fres;

void ui_Verify_InitDisp(void)
{
	u8 i;
	
	u16 viBkCol;
	u16 viTxCol;
	u16 viBarCol;
	
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		viBkCol  = BLACK;
		viTxCol = WHITE;
		viBarCol = GRAY;
	}else{
		viBkCol  = WHITE;
		viTxCol = BLACK;;
		viBarCol = DGRAY;
	}
	
	//ҳ�����˵���
	MENU_TABLE_STRUCT Samp10Menu[3] = {
		{0,3,"��",NULL},
		{1,3,"��","����"},
		{2,3,"��","�궨"},
	};
	//�˵���ʾ
	ui_DispMenuBtn(Samp10Menu,0);
	
	//״̬��
	LCD_DrawRect(56,0,424,32,viBkCol);
	LCD_FillRect(57,1,422,31,viBarCol);
	
	//�ָ���
	LCD_FillRect(57,192,422,2,viBkCol);//��
	for(i=0;i<42;i++){
		LCD_SetColor(viBkCol,viTxCol);
		LCD_ShowChinese(Font8,58+i*10,192,1,"��");
	}
	LCD_FillRect(57,298,420,1,viTxCol);//��
	
	LCD_SetColor(viBarCol,WHITE);
	//�ֽ�����
	LCD_ShowChinese(Font16,164,8,0,VerifyParaTable[2].pList[g_SysPara.g_SamplePara.iRebarType].listStr);
	//����ֱ��
	LCD_ShowChinese(Font20,198,6,0,"��");
	LCD_ShowNums(ASC20x10,216,6,0,*VerifyParaTable[1].ParaAddr);

	//����ͼ��
	LCD_ShowChinese(Font24,410,5,0,"��");
	DispSaveDataCnt(g_GlobalPara.iSaveDataCnt);
	
	g_DetectPara.PageCurr = 1;
	g_DetectPara.MZJSite = 0;
	g_DetectPara.CarSite = 0;
	
	BrushZXX(FAR);
	BrushMZJ(g_DetectPara.MZJSite,FAR);

	//��ť��ʼ��
	g_u8ButtonNums = 9;
	struct BUTTON SampBtn[9] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0},//0, ����
		{  0,  50,  59, 103, 0,0},//1, ����
		{  0, 104,  59, 157, 0,0},//2, �궨
		{440,  32, 479,  85, 0,0},//3, ͨ��1
		{440,  86, 479, 138, 0,0},//4, ͨ��2
		{440, 139, 479, 191, 0,0},//5, ͨ��3
		
		{ 70,  32, 439,  85, 0,BTN_ABILITY_DRAG},//6, ͨ��1 �϶�
		{ 70,  86, 439, 138, 0,BTN_ABILITY_DRAG},//7, ͨ��2 �϶�
		{ 70, 139, 439, 191, 0,BTN_ABILITY_DRAG},//8, ͨ��3 �϶�
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	if(g_GlobalPara.ucFromPageNum == UI_MAIN){
		//��ʼ������
		g_DetectPara.MZJSite = 0;
//		g_DetectPara.CarSite = 0;
		
		//��ջ���
		memset(&ResBuf_CH1,0x0,sizeof(ResBuf_CH1));
		memset(&ResBuf_CH2,0x0,sizeof(ResBuf_CH2));
		memset(&ResBuf_CH3,0x0,sizeof(ResBuf_CH3));
		
		//ͨ��ѡ��
		g_DetectPara.Channel = 1;
		g_DetectPara.ChannelLast = 1;
		
		g_GlobalPara.iSaveDataCnt = 0;
		
//		g_SysPara.SampleDispMode = MIAO_POU;
		DispModeTemp = MIAO_POU;
	}
	sprintf((char*)VerifyParaTable[0].pList->listStr,"%s",g_SysPara.GouJianName);
	ui_DispGJName(ASC16x8,70,8,WHITE,(char*)VerifyParaTable[0].pList->listStr);

	//ͨ��ѡ��
	BrushChannelSel(g_DetectPara.Channel,0);
	//��ʾ���ͼ��
	BrushCurvePage(g_DetectPara.PageCurr,g_DetectPara.Channel,DispModeTemp);

	ClearTouchSta();//�������
}

void ui_Verify_Run(void)
{
//	char tempBuf[16];
	char dataBuf[16];
	u8 i;
//	u16 saveCnt;
//	int offset = 0;
	u16 gbkCol;
	u16 menuBtCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		gbkCol = BLACK;
		menuBtCol = DGRAY;
	}else{
		gbkCol = WHITE;
		menuBtCol = LGRAY;
	}
	
	//��������ʾ
	if(g_GlobalPara.ucFromPageNum == UI_MAIN){
		Get_BatPrcnt();
		if(g_GlobalPara.iBatPercent < 20){
			PromptDialog(DIALOGBATLOW);
			g_GlobalPara.ucFromPageNum = UI_VERIFY;//��װ����ҳ����
			LCD_FillRect(56,32,424,288,gbkCol);
			ui_Verify_InitDisp();
		}
		if(g_GlobalPara.iBatPercent < 5){
			PromptDialog(DIALOGBATNONE);
			SendUiNo(UI_MAIN);
			g_GlobalPara.ucFromPageNum = UI_VERIFY;//��װ����ҳ����
			LCD_FillRect(56,32,424,288,gbkCol);
			return;
		}
	}
	
	while(1)
	{
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		//���Ч��
		ui_AllButtonReaction(5,menuBtCol,GOLD);
//		ui_OneButtonReactionXY(0,0,0,47,33,GOLD,ORANGE);//����

//		ui_DispTime(0);//ʱ��
		ui_GetTempera();//�ɼ��¶�
		ui_DispBattery(2);//�ɼ�������ʾ����

		//���ݲɼ�
		if(Get_Adc_Data()){
			g_DetectPara.PageCurr = g_iCarDist/g_GlobalPara.sDispScreenWide+1;
			if(g_DetectPara.PageCurr != g_DetectPara.PageLast){
				BrushCurvePage(g_DetectPara.PageCurr,g_DetectPara.Channel,DispModeTemp);
				g_DetectPara.PageView = g_DetectPara.PageCurr;
				g_DetectPara.PageLast = g_DetectPara.PageCurr;
			}
			if(g_DetectPara.PageCurr != g_DetectPara.PageView){
				if(g_iCarDist != g_iCarDistLast){
					BrushCurvePage(g_DetectPara.PageCurr,g_DetectPara.Channel,DispModeTemp);
					g_DetectPara.PageView = g_DetectPara.PageCurr;
					g_DetectPara.PageLast = g_DetectPara.PageCurr;
				}
			}
			if(g_iCarDir < 0){//С������
				DeleteCurveOne(g_DetectPara.Channel,g_iCarDist,DispModeTemp);
			}else{
				//���ֽ�
//				if(��⵽�ֽ�){
//					SaveDotData(g_DetectPara.Channel,g_iCarDist,g_DetectPara.HouduRes);
//					saveCnt = ResBuf[g_DetectPara.Channel]->AllRebarCnt;
//					if(saveCnt > 0){
//						BrushPouMian(g_DetectPara.PageCurr,g_iCarDist,ResBuf[g_DetectPara.Channel]->RebarSite[saveCnt-1],g_DetectPara.HouduRes);
//					}else{
//						BrushPouMian(g_DetectPara.PageCurr,g_iCarDist,0,g_DetectPara.HouduRes);
//					}
//				}
			}
			BrushCar(g_iCarDist);
			DispCarDist(g_iCarDist);
			
			//����
			LCD_SetColor(gbkCol,ENROD);
			sprintf(dataBuf,"1:%-6d",g_iCh1AdcVal);
			LCD_ShowString(ASC16x8,80, 200,0,dataBuf);
			sprintf(dataBuf,"2:%-6d",g_iCh2AdcVal);
			LCD_ShowString(ASC16x8,140,200,0,dataBuf);
			sprintf(dataBuf,"3:%-6d",g_iCh3AdcVal);
			LCD_ShowString(ASC16x8,80, 220,0,dataBuf);
			sprintf(dataBuf,"4:%-6d",g_iCh4AdcVal);
			LCD_ShowString(ASC16x8,140,220,0,dataBuf);
			sprintf(dataBuf,"5:%-6d",g_iCh5AdcVal);
			LCD_ShowString(ASC16x8,80, 240,0,dataBuf);
			sprintf(dataBuf,"6:%-6d",g_iCh6AdcVal);
			LCD_ShowString(ASC16x8,140,240,0,dataBuf);
			
			//����
			if(SaveFlag != 0){
				if(g_iCarDist != g_iCarDistLast){
//					SourceData.iCh2Sum[g_GlobalPara.iSaveDataCnt] = g_iCh3AdcVal+g_iCh4AdcVal;
					g_GlobalPara.iSaveDataCnt += 1;
					LCD_ShowNums(ASC20x10,280,6,0,g_GlobalPara.iSaveDataCnt);
				}
			}
			
			g_iCarDistLast = g_iCarDist;
		}
		
		if(g_u16KeyValBuf == KEY_OK){
			g_GlobalPara.iSaveDataCnt = GetSampDataCnt();
			if(g_GlobalPara.iSaveDataCnt > 0){
				//�洢�ռ䲻����ʾ
				if(g_GlobalPara.iStorageFreePrcnt < 1){
					PromptDialog(DIALOGSTORNONE);
					
					g_GlobalPara.iSaveDataCnt = 0;
				
					SendUiNo(UI_SAMPLE10);
					g_GlobalPara.ucFromPageNum = UI_MAIN;//��װ����ҳ����
					LCD_FillRect(56,32,424,288,gbkCol);
					return;
				}else if(g_GlobalPara.iStorageFreePrcnt < 3){
					PromptDialog(DIALOGSTORLOW);
				}else{
					
				}
				
				SaveSampFile();
				ui_SetGJNameNext((char*)VerifyParaTable[0].pList->listStr);//������һ������
				SaveSysPara();
				g_GlobalPara.iSaveDataCnt = 0;
				
				SendUiNo(UI_SAMPLE10);
				g_GlobalPara.ucFromPageNum = UI_MAIN;//��װ����ҳ����
				LCD_FillRect(56,32,424,288,gbkCol);
				return;
			}
		}
//		if(g_u16KeyValBuf == KEY_BACK){
//			SendUiNo(UI_MAIN);
//			LCD_FillRect(0,0,480,320,WHITE);
//			return;
//		}
		//����
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_BACK)){
			g_GlobalPara.iSaveDataCnt = GetSampDataCnt();
			if(g_GlobalPara.iSaveDataCnt > 0){
				if(PromptDialog(DIALOGSAVE) == CONFIRM){
					//�洢�ռ䲻����ʾ
					if(g_GlobalPara.iStorageFreePrcnt < 1){
						PromptDialog(DIALOGSTORNONE);
						g_GlobalPara.iSaveDataCnt = 0;
						SendUiNo(UI_MAIN);
						LCD_FillRect(56,32,424,288,gbkCol);
						return;
					}else if(g_GlobalPara.iStorageFreePrcnt < 3){
						PromptDialog(DIALOGSTORLOW);
					}else{
						
					}
					
					SaveSampFile();
					ui_SetGJNameNext((char*)VerifyParaTable[0].pList->listStr);//������һ������
					SaveSysPara();
				}else{
					//������
				}
				g_GlobalPara.iSaveDataCnt = 0;
			}
			SendUiNo(UI_MAIN);
			LCD_FillRect(56,32,424,288,gbkCol);
			return;
		}
		//���ԣ��洢����
//		if(g_u16KeyValBuf == KEY_FN){
//			SaveFlag = !SaveFlag;
//			if(SaveFlag == 0){
//				LCD_ShowString(ASC20x10,240,6,0,"Off");
//				fres = f_open(file,"0:/TEST/test.txt",FA_READ|FA_WRITE|FA_CREATE_ALWAYS);
//				for(i=0;i<g_GlobalPara.iSaveDataCnt;i++){
//					fres = f_printf(file,"%d,\r\n",SourceData.iCh2Sum[i]);
//				}
//				fres = f_close(file);
//				
//			}else{
//				g_GlobalPara.iSaveDataCnt = 0;
//				LCD_ShowNums(ASC20x10,280,6,0,g_GlobalPara.iSaveDataCnt);
//				memset(SourceData.iCh2Sum,0x0,sizeof(SourceData.iCh2Sum));
//				LCD_ShowString(ASC20x10,240,6,0,"On 0    ");
//			}
//		}
		//�궨
		if(g_u16KeyValBuf == LONG_KEY_FN){
			SendUiNo(UI_BIAODING);
			g_GlobalPara.ucFromPageNum = UI_VERIFY;
			LCD_FillRect(56,32,424,288,gbkCol);
			return;
		}
		//�����ͱ궨
		if(g_GlobalPara.iBatPcntLowFlag == 1){
			g_GlobalPara.iBatPcntLowFlag = 0;
			if(PromptDialog(DIALOGBIAODINGBATLOW) == 1){
				SendUiNo(UI_BIAODING);
				g_GlobalPara.ucFromPageNum = UI_VERIFY;
				LCD_FillRect(56,32,424,288,gbkCol);
				return;
			}else{
				LCD_FillRect(90,50,360,220,gbkCol);//��յ���
				ui_Verify_InitDisp();
			}
		}
		//�����仯��궨
		if(abs(g_GlobalPara.iBatPercent-g_SysPara.iBatPercentBDLast) > 20){
			if(g_GlobalPara.iChargeSta == CHARGE_NO){
				if(PromptDialog(DIALOGBIAODINGBATJUMP) == 1){
					SendUiNo(UI_BIAODING);
					g_GlobalPara.ucFromPageNum = UI_VERIFY;
					LCD_FillRect(56,32,424,288,gbkCol);
					return;
				}else{
					g_SysPara.iBatPercentBDLast = g_GlobalPara.iBatPercent;
					LCD_FillRect(90,50,360,220,gbkCol);//��յ���
					ui_Verify_InitDisp();
					SaveSysPara();
				}
			}
		}
		//�¶ȱ仯��궨
		if(fabs(g_GlobalPara.fTemperature-g_SysPara.fTemperatureBDLast) > 10.0f){
			if(PromptDialog(DIALOGBIAODINGTEMPER) == 1){
				SendUiNo(UI_BIAODING);
				g_GlobalPara.ucFromPageNum = UI_VERIFY;
				LCD_FillRect(56,32,424,288,gbkCol);
				return;
			}else{
				g_SysPara.fTemperatureBDLast = g_GlobalPara.fTemperature;
				LCD_FillRect(90,50,360,220,gbkCol);//��յ���
				ui_Verify_InitDisp();
				SaveSysPara();
			}
		}
		
		//����
		if(g_u8TouchResult == 0){
			SendUiNo(UI_MAIN);
			LCD_FillRect(56,32,424,288,gbkCol);
			return;
		}
		//����
		if(g_u8TouchResult == 1){
			SendUiNo(UI_SAMPSET);
			g_GlobalPara.ucFromPageNum = UI_VERIFY;
			LCD_FillRect(56,32,424,288,gbkCol);
			return;
//			LCD_FillRect(60,0,420,320,WHITE);
//			VerifyParaSet_Fun(VerifyParaTable);
//			ui_Verify_InitDisp();
		}
		//�궨
		if(g_u8TouchResult == 2){
			SendUiNo(UI_BIAODING);
			g_GlobalPara.ucFromPageNum = UI_VERIFY;
			LCD_FillRect(56,32,424,288,gbkCol);
			return;
		}
		//ͨ��ѡ��
		if((g_u8TouchResult >= 3)&&(g_u8TouchResult <= 5)){
			g_DetectPara.Channel = g_u8TouchResult - 3;
			BrushChannelSel(g_DetectPara.Channel,0);
			BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,DispModeTemp);
		}
		//�϶��л�ͨ��
		for(i=0;i<3;i++){
			if(g_ButtonBuf[6+i].btSta == BTN_STATUS_DRAG){
				g_DetectPara.ChannelLast = g_DetectPara.Channel;
				g_DetectPara.Channel = Get_DragChannel(&g_ButtonBuf[6],3,DRAG_ZONG);
				if(g_DetectPara.Channel != g_DetectPara.ChannelLast){
					BrushChannelSel(g_DetectPara.Channel,0);
					BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,DispModeTemp);
				}
			}
		}
		
//		//��׼��
//		if(g_u8TouchResult == SLIDE_GAP_RIGHT){//+BTN_NUM_LONGPRESS_HOLD){
//			g_DetectPara.MZJSite += 2;
//			BrushMZJ(g_DetectPara.MZJSite,DGREEN);
//			g_DetectPara.CarSite += 2;
////			BrushCar(CarSite);
//		}
//		if(g_u8TouchResult == SLIDE_GAP_LEFT){//+BTN_NUM_LONGPRESS_HOLD){
//			g_DetectPara.MZJSite -= 2;
//			BrushMZJ(g_DetectPara.MZJSite,DGREEN);
//			g_DetectPara.CarSite -= 2;
////			BrushCar(CarSite);
//		}
		
		if(g_u8TouchResult == SLIDE_END_LEFT){//��
			
			g_DetectPara.PageCurr += 1;
			if(g_DetectPara.PageCurr > 25)g_DetectPara.PageCurr = 1;
		}
		if(g_u8TouchResult == SLIDE_END_RIGHT){//�һ�
			
			g_DetectPara.PageCurr -= 1;
			if(g_DetectPara.PageCurr < 1)g_DetectPara.PageCurr = 25;
		}
		
		Sleep_ms(5);
	}
	
//		//ADC�ɼ�
		Get_Adc_Data();
//		//��ʾ�¶�
//		if(g_bTemperaDispFlag == 1){
//			g_bTemperaDispFlag = 0;
//			LCD_SetColor(WHITE,BLACK);
//			LCD_ShowFloat(ASC24x12,420,296,0,5,1,g_GlobalPara.fTemperature);
//		}
}

///////////////////////////////////////////////////////////////////////////////////////////////
