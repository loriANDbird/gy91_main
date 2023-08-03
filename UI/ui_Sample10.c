/*******************************************************************************
�����ߣ�
�������ڣ�2018.03.01
������������Ȳ���
    1. ui_Sample10 �����ʼ��
    2. ui_Sample10_Run ����ʵ��

���¼�¼��
    1. 2018.03.01 ���δ���
    
*******************************************************************************/
#include "ui_Sample10.h"
#include "Config_ui.h"
#include "AimLib.h"


DETECTPARA g_DetectPara;//������

SYS_PARA_TABLE_STRUCT* SampTable = &ParaTable[PARATABLEINDEX_SAMPLE];

//static FRESULT fres;



char *ChName[3] = {"��","��","��"};
char *Condition[5] = {"�����","�ߵͽ�","�ܹ���","��͹��","������"};

u16 weiy1[20] = {40,80,120,160,200,240,290,320,350,410,440,480,520,560,600,640,680,720,760,790};
u8 houd1[20] = {20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210};

u16 weiy2[20] = {42,82,122,162,202,242,280,320,360,412,442,482,522,562,602,642,682,722,762,792};
u8 houd2[20] = {20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210};

u16 weiy3[20] = {44,84,124,164,204,244,270,320,370,414,444,484,524,564,604,644,684,724,764,794};
u8 houd3[20] = {20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210};

//��������
int GetSampDataCnt(void);
void SaveSampFile(void);

void ui_Sample10_InitDisp(void)
{
	u8 i;
	u16 SampColor_Bk; 
	u16 SampColor_Txt;
	u16 SampBarColor_Bk;
	u16 SampBarColor_Txt;
    
//////	//��������
//////	ResBuf_CH1.AllRebarCnt = 20;
//////	ResBuf_CH2.AllRebarCnt = 20;
//////	ResBuf_CH3.AllRebarCnt = 20;
//////	memcpy(ResBuf_CH1.RebarSite,weiy1,sizeof(weiy1));
//////	memcpy(ResBuf_CH1.RebarCover,houd1,sizeof(houd1));
//////	memcpy(ResBuf_CH2.RebarSite,weiy2,sizeof(weiy2));
//////	memcpy(ResBuf_CH2.RebarCover,houd2,sizeof(houd2));
//////	memcpy(ResBuf_CH3.RebarSite,weiy3,sizeof(weiy3));
//////	memcpy(ResBuf_CH3.RebarCover,houd3,sizeof(houd3));
	
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		SampColor_Bk = BLACK;
		SampColor_Txt = WHITE;
		SampBarColor_Bk = GRAY;
		SampBarColor_Txt = WHITE;
	}else{
		SampColor_Bk = WHITE;
		SampColor_Txt = BLACK;
		SampBarColor_Bk = DGRAY;
		SampBarColor_Txt = WHITE;
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
	ui_DispMenuBtn(Samp10Menu,0);
	
	//״̬��
	LCD_DrawRect(56,0,424,32,SampColor_Bk);
	LCD_FillRect(57,1,422,31,SampBarColor_Bk);
	
	//�ָ���
	LCD_FillRect(57,192,422,2,SampColor_Txt);//��
	for(i=0;i<42;i++){
		LCD_SetColor(SampColor_Bk,SampColor_Txt);
		LCD_ShowChinese(Font8,58+i*10,192,1,"��");
	}
	LCD_FillRect(57,298,420,1,SampColor_Txt);//��
	
//	LCD_FillRect(57,32,422,160,SampColor_Bk);//ˢ����ɫ
	 
	LCD_SetColor(SampBarColor_Bk,SampBarColor_Txt);
	//�ֽ�����
	LCD_ShowChinese(Font16,164,8,0,SampTable[4].pList[g_SysPara.g_SamplePara.iRebarType].listStr);
	//����ֱ��
	LCD_ShowChinese(Font20,198,6,0,"��");
	LCD_ShowNums(ASC20x10,216,6,0,*SampTable[1].ParaAddr);
	//����
	LCD_ShowChinese(Font16,248,8,0,(u8*)Condition[g_DetectPara.WorkCondition]);
	//�洢��ʽ
	if(g_SysPara.g_SamplePara.iSaveType != 0){//�ִ�
		LCD_ShowString(ASC20x10,308,6,0,"M");
	}
	//����ͼ��
	LCD_ShowChinese(Font24,410,5,0,"��");
	DispSaveDataCnt(g_GlobalPara.iSaveDataCnt);
    
	//��ť��ʼ��
	g_u8ButtonNums = 12;
	struct BUTTON SampBtn[12] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0},//0, ����
		{  0,  50,  59, 103, 0,0},//1, ����
		{  0, 104,  59, 157, 0,0},//2, ��ʾ
		{  0, 158,  59, 211, 0,0},//3, ����
		{  0, 212,  59, 265, 0,0},//4, ����
		{  0, 266,  59, 319, 0,0},//5, �궨
		
		{440,  32, 479,  85, 0,0},//6, ͨ��1
		{440,  86, 479, 138, 0,0},//7, ͨ��2
		{440, 139, 479, 191, 0,0},//8, ͨ��3
		
		{ 70,  32, 439,  85, 0,BTN_ABILITY_DRAG},// 9, ͨ��1 �϶�
		{ 70,  86, 439, 138, 0,BTN_ABILITY_DRAG},//10, ͨ��2 �϶�
		{ 70, 139, 439, 191, 0,BTN_ABILITY_DRAG},//11, ͨ��3 �϶�
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	//��ʼ������
	if(g_GlobalPara.ucFromPageNum == UI_MAIN){
		//��ʼ������
		g_DetectPara.MZJSite = 0;
//		g_DetectPara.CarSite = 0;
		
		//ͨ��ѡ��
		g_DetectPara.Channel = 1;
		g_DetectPara.ChannelLast = 1;
		
		//��ջ���
		memset(&ResBuf_CH1,0x0,sizeof(ResBuf_CH1));
		memset(&ResBuf_CH2,0x0,sizeof(ResBuf_CH2));
		memset(&ResBuf_CH3,0x0,sizeof(ResBuf_CH3));
		
		g_GlobalPara.iSaveDataCnt = 0;
		g_GlobalPara.ucHouduAvg = 0;
		g_GlobalPara.ucHouduMin = 0;
		g_GlobalPara.ucHouduMax = 0;
	}
    
	//�洢����
	g_GlobalPara.iStorageFreePrcnt = Get_FatFreePrcnt();
	g_DetectPara.PageCurr = g_iCarDist/g_GlobalPara.sDispScreenWide+1;
	g_DetectPara.PageView = g_DetectPara.PageCurr;
	g_DetectPara.PageLast = g_DetectPara.PageCurr;
	
	//������
	sprintf((char*)SampTable[0].pList->listStr,"%s",g_SysPara.GouJianName);
	ui_DispGJName(ASC16x8,70,8,WHITE,(char*)SampTable[0].pList->listStr);
	
	if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){
		BrushChannelSel(g_DetectPara.Channel,1);
		BrushTiaoXingPage(g_DetectPara.PageCurr,0);
		BrushTiaoXingPage(g_DetectPara.PageCurr,1);
		BrushTiaoXingPage(g_DetectPara.PageCurr,2);
		DispChannelLine(g_DetectPara.Channel);
	}else{
        BrushZXX(FAR);
        BrushMZJ(0,FAR);
		BrushChannelSel(g_DetectPara.Channel,0);
	}
	
	//��ʾ���ͼ��
	BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,g_SysPara.SampleDispMode);

    //��������ֵ
    g_tWorkPara.cZhujinDiam = g_SysPara.g_SamplePara.iZhujinDiam;//����ֱ��
    g_tWorkPara.cZhujinDistance = 0;	//������
    g_tWorkPara.cGujinDiam = 0;		    //����ֱ��
    g_tWorkPara.cGujinDistance = 0;	    //������

    if(g_tWorkPara.cGangJinType == 0 ){
        g_tWorkPara.cGangJinType = 1;
        g_SysPara.g_SamplePara.iRebarType = 1;
    }
    else{
        g_tWorkPara.cGangJinType = 0;
        g_SysPara.g_SamplePara.iRebarType = 0;
    }
    LCD_ShowChinese(Font16,164,8,0,SampTable[4].pList[g_SysPara.g_SamplePara.iRebarType].listStr);//�ֽ�����
    g_tWorkPara.cLingChengType = 1;	    //��С����

    g_tWorkPara.cJiLiang = 0;			//����ģʽ
    g_tWorkPara.cShejiHD = 30;			//��ƺ��

    ClearTouchSta();//�������

//////    //��ʼ��ԭʼ���ݴ洢����
//////    InitSourceDataPara();//���ݲɼ����
//////	
//////    //��ʾԭʼ���ݴ洢����
//////    DispSourceDataPara();//���ݲɼ����
}

void ui_Sample10_Run(void)
{
	u8 i;
    u8 ucAimState = 0;
	u8 handSaveAbleCh1 = 0;
	u8 handSaveAbleCh2 = 0;
	u8 handSaveAbleCh3 = 0;
    u8 Res;
//    char cBuf[8];
	
//	u16 saveCnt;
    
    char cDistChangeFlag = 0;
	
	u16 bkCol;
	u16 txCol;
	u16 menuBtCol;
	//��ɫ����
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
		txCol = WHITE;
		menuBtCol = DGRAY;
	}else{
		bkCol = WHITE;
		txCol = BLACK;
		menuBtCol = LGRAY;
	}
	
//	u16 SampColor_Bk;
//	u16 SampColor_Txt;
	
	//��������ʾ
	if(g_GlobalPara.ucFromPageNum == UI_MAIN){
		Get_BatPrcnt();
		if(g_GlobalPara.iBatPercent < 20){
			PromptDialog(DIALOGBATLOW);
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
			LCD_FillRect(56,32,424,288,bkCol);
			ui_Sample10_InitDisp();
		}
		if(g_GlobalPara.iBatPercent < 5){
			PromptDialog(DIALOGBATNONE);
			SendUiNo(UI_MAIN);
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;//��װ����ҳ����
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
	}
	
	while(1)
	{
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		//���Ч��
		ui_AllButtonReaction(6,menuBtCol,GOLD);

//		ui_DispTime(0);//ʱ��
		ui_GetTempera();//�ɼ��¶�
		ui_DispBattery(2);//�ɼ�������ʾ����
		
		//����
		if(g_SysPara.iBlueToothSta == 1){
			BT_Lookup();
		}
		
        //ADC�ɼ�
		Get_Adc_Data();
        
        //������󻺴泤��
        if(g_iCarDist > SOURCEDATAMAXCNT){
            PromptTone_Once(200);//��ʾ��
            g_iCarDist = SOURCEDATAMAXCNT;
            return;
        }
        
        if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){
            //��λ�Ʊ仯�ٽ�������
            if(g_iCarDist != g_iCarLastDist){
                
                //λ�Ʊ仯��־λ
                cDistChangeFlag = 1;
                
                //���иֽλ�㷨
                AimFuncRun_HD(g_DetectPara.Channel, g_SysPara.SampleDispMode);
                
                
                
//////                EstimateDiam();
                
                
                g_iCarLastDist = g_iCarDist;
            }
        }
        else if((g_SysPara.SampleDispMode == MIAO_POU)||(g_SysPara.SampleDispMode == MIAO_BO)){
            //������׼��λ��
            ucAimState = UpdateMZK(g_DetectPara.Channel);
            
            //��λ�Ʊ仯�ٽ�������
            if(g_iCarDist != g_iCarLastDist){

                //λ�Ʊ仯��־λ
                cDistChangeFlag = 1;
                
                //��׼����λ�ֽ��㷨
                Res = AimFuncRun_MZK(g_DetectPara.Channel, ucAimState, g_SysPara.SampleDispMode);
                
//////                //��ʾ��׼�������
//////                if((Res != 0)&&(g_iCarDist >= g_iCarMaxDist)){
//////                    g_DetectPara.HouduRes = Res;
//////                    sprintf(cBuf,"%-2d",g_DetectPara.HouduRes);
//////                    LCD_SetColor(bkCol,txCol);
//////                    LCD_ShowString(ASC24x12,384,146,0,cBuf);//���ֵ
//////                }
                
                g_iCarLastDist = g_iCarDist;
            }
        }

		//����ҳ��
		if(cDistChangeFlag == 1){
            cDistChangeFlag = 0;
			g_DetectPara.PageCurr = g_iCarDist/g_GlobalPara.sDispScreenWide+1;
			if(g_DetectPara.PageCurr != g_DetectPara.PageLast){
				if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){
					ClearTiaoxingPage(0);//�����������
					ClearTiaoxingPage(1);//�����������
					ClearTiaoxingPage(2);//�����������
					BrushTiaoXingPage(g_DetectPara.PageCurr,0);
					BrushTiaoXingPage(g_DetectPara.PageCurr,1);
					BrushTiaoXingPage(g_DetectPara.PageCurr,2);
					DispChannelLine(g_DetectPara.Channel);
				}
                else if((g_SysPara.SampleDispMode == MIAO_POU)||(g_SysPara.SampleDispMode == MIAO_BO)){
//                    BrushMZJ(GetMZJSite(),u16 color);
//////                    UpdateMZK(g_DetectPara.Channel);
                }
//				ClearCurvePage();//�����������
				BrushCurvePage(g_DetectPara.PageCurr, g_DetectPara.Channel, g_SysPara.SampleDispMode);
				g_DetectPara.PageView = g_DetectPara.PageCurr;
				g_DetectPara.PageLast = g_DetectPara.PageCurr;
			}
			if(g_DetectPara.PageCurr != g_DetectPara.PageView){
				if(g_iCarDist != g_iCarDistLast){
					if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){
						ClearTiaoxingPage(0);//�����������
						ClearTiaoxingPage(1);//�����������
						ClearTiaoxingPage(2);//�����������
						BrushTiaoXingPage(g_DetectPara.PageCurr,0);
						BrushTiaoXingPage(g_DetectPara.PageCurr,1);
						BrushTiaoXingPage(g_DetectPara.PageCurr,2);
						DispChannelLine(g_DetectPara.Channel);
					}
	//				ClearCurvePage();//�����������
					BrushCurvePage(g_DetectPara.PageCurr,g_DetectPara.Channel,g_SysPara.SampleDispMode);
					g_DetectPara.PageView = g_DetectPara.PageCurr;
					g_DetectPara.PageLast = g_DetectPara.PageCurr;
				}
			}
			if(g_iCarDir < 0){//С������
//////				DeleteCurveOne(g_DetectPara.Channel, g_iCarDist, g_SysPara.SampleDispMode);
			}else{
				//���ֽ�
//				if(��⵽�ֽ�){
//					SaveDotData(g_DetectPara.Channel,g_iCarDist,g_DetectPara.HouduRes);
//					saveCnt = ResBuf[g_DetectPara.Channel]->AllRebarCnt;
//					if((g_SysPara.SampleDispMode == MIAO_POU)||(g_SysPara.SampleDispMode == TIAO_POU)){
//						if(saveCnt > 0){
//							BrushPouMian(g_DetectPara.PageCurr,g_iCarDist,ResBuf[g_DetectPara.Channel]->RebarSite[saveCnt-1],g_DetectPara.HouduRes);
//						}else{
//							BrushPouMian(g_DetectPara.PageCurr,g_iCarDist,0,g_DetectPara.HouduRes);
//						}
//					}
//					if((g_SysPara.SampleDispMode == MIAO_BO)||(g_SysPara.SampleDispMode == TIAO_BO)){
//						if(saveCnt > 0){
//							BrushBoXing(g_DetectPara.PageCurr,g_iCarDist,ResBuf[g_DetectPara.Channel]->RebarSite[saveCnt-1],g_DetectPara.HouduRes);
//						}else{
//							BrushBoXing(g_DetectPara.PageCurr,g_iCarDist,0,g_DetectPara.HouduRes);
//						}
//					}
//					if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){
//						BrushTiaoXing(g_DetectPara.Channel,g_iCarDist,g_DetectPara.HouduRes);
//					}
//				}
			}
			BrushCar(g_iCarDist);
			DispCarDist(g_iCarDist);
			
			g_iCarDistLast = g_iCarDist;
		}

		//�洢����
		if(g_u16KeyValBuf == KEY_OK){
            
            //����ԭʼ����CSV�ļ�
            SaveSourceDataFile();
            
            
			g_GlobalPara.iSaveDataCnt = GetSampDataCnt();
			if(g_GlobalPara.iSaveDataCnt > 0){
				//�洢�ռ䲻����ʾ
				if(g_GlobalPara.iStorageFreePrcnt < 1){
					PromptDialog(DIALOGSTORNONE);
					g_GlobalPara.iSaveDataCnt = 0;
					SendUiNo(UI_SAMPLE10);
					g_GlobalPara.ucFromPageNum = UI_MAIN;//��װ����ҳ����
					LCD_FillRect(56,32,424,288,bkCol);
					return;
				}else if(g_GlobalPara.iStorageFreePrcnt < 3){
					PromptDialog(DIALOGSTORLOW);
				}else{
					
				}
				
				SaveSampFile();
				ui_SetGJNameNext((char*)SampTable[0].pList->listStr);//������һ������
				SaveSysPara();
				g_GlobalPara.iSaveDataCnt = 0;
				
				SendUiNo(UI_SAMPLE10);
				g_GlobalPara.ucFromPageNum = UI_MAIN;//��װ����ҳ����
				LCD_FillRect(56,32,424,288,bkCol);
                g_iCarDist = 0;
                g_iCarLastDist = 0;
                
//                g_iCarCountZero = g_iCarCount;
                //            g_iCh1SumBuf = 0;
                //            g_iCh2SumBuf = 0;
                //            g_iCh3SumBuf = 0;

                //��ʼ����λ�ֽ��
                InitAimPara();
                
//                LCD_SetColor(WHITE,WHITE);
//                LCD_ShowString(ASC16x8,190,  0, 0,"                       ");
//                LCD_ShowString(ASC16x8,190, 20, 0,"                       ");
//                LCD_ShowString(ASC16x8,190, 40, 0,"                       ");

				return;
			}
		}
//		if(g_u16KeyValBuf == KEY_BACK){
//			SendUiNo(UI_MAIN);
//			LCD_FillRect(0,0,480,320,WHITE);
//			return;
//		}
		//����
		if(g_u16KeyValBuf == KEY_FN){
			if(g_SysPara.g_SamplePara.iSaveType == 0){//�Զ��洢
				ClearCarDist();//����λ��
				memset(&ResBuf_CH1,0,sizeof(ResBuf_CH1));//������ݻ���
				memset(&ResBuf_CH2,0,sizeof(ResBuf_CH2));//������ݻ���
				memset(&ResBuf_CH3,0,sizeof(ResBuf_CH3));//������ݻ���
                
                
				if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){
					ClearTiaoxingPage(0);//�����������
					ClearTiaoxingPage(1);//�����������
					ClearTiaoxingPage(2);//�����������
					DispChannelLine(g_DetectPara.Channel);
				}
				ClearCurvePage();//��������
                
                BrushCar(g_iCarDist);
			
                g_iCarDist = 0;
                g_iCarLastDist = 0;
                g_iCarMaxDist = 0;
//////                g_iCarCountZero = g_iCarCount;

    //            g_iCh1SumBuf = 0;
    //            g_iCh2SumBuf = 0;
    //            g_iCh3SumBuf = 0;
                
                //��ʼ����λ�ֽ��
                InitAimPara();
                
                //��ʾ�ֽ����
                DispRebarCnt(g_DetectPara.Channel);
                
                
                
                //TEST_ZJC
                LCD_SetColor(WHITE,ORANGE);//CL_BLUE
                LCD_ShowString(ASC16x8,0, 180, 0,"          ");
                LCD_ShowString(ASC16x8,0, 200, 0,"          ");
                LCD_ShowString(ASC16x8,0, 160, 0,"          ");
                //END
                
                

//				BrushCar(g_iCarDist);
			}else{//�ֶ��洢
				if(handSaveAbleCh1 == 1){
					handSaveAbleCh1 = 0;
					SaveDotData(g_DetectPara.Channel,g_iCarDist,g_DetectPara.HouduRes);
				}
				if(handSaveAbleCh2 == 1){
					handSaveAbleCh2 = 0;
					SaveDotData(g_DetectPara.Channel,g_iCarDist,g_DetectPara.HouduRes);
				}
				if(handSaveAbleCh3 == 1){
					handSaveAbleCh3 = 0;
					SaveDotData(g_DetectPara.Channel,g_iCarDist,g_DetectPara.HouduRes);
				}
			}
		}
		//�궨
		if(g_u16KeyValBuf == LONG_KEY_FN){
			SendUiNo(UI_BIAODING);
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//�����ͱ궨
		if(g_GlobalPara.iBatPcntLowFlag == 1){
			g_GlobalPara.iBatPcntLowFlag = 0;
			if(PromptDialog(DIALOGBIAODINGBATLOW) == 1){
				SendUiNo(UI_BIAODING);
				g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}else{
				LCD_FillRect(90,50,360,220,bkCol);//��յ���
				ui_Sample10_InitDisp();
			}
		}
//////		//�����仯��궨
//////		if(abs(g_GlobalPara.iBatPercent-g_SysPara.iBatPercentBDLast) > 20){
//////			if(g_GlobalPara.iChargeSta == CHARGE_NO){
//////				if(PromptDialog(DIALOGBIAODINGBATJUMP) == 1){
//////					SendUiNo(UI_BIAODING);
//////					g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
//////					LCD_FillRect(56,32,424,288,bkCol);
//////					return;
//////				}else{
//////					g_SysPara.iBatPercentBDLast = g_GlobalPara.iBatPercent;
//////					LCD_FillRect(90,50,360,220,bkCol);//��յ���
//////					ui_Sample10_InitDisp();
//////					SaveSysPara();
//////				}
//////			}
//////		}
//////		//�¶ȱ仯��궨
//////		if(fabs(g_GlobalPara.fTemperature-g_SysPara.fTemperatureBDLast) > 10.0f){
//////			if(PromptDialog(DIALOGBIAODINGTEMPER) == 1){
//////				SendUiNo(UI_BIAODING);
//////				g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
//////				LCD_FillRect(56,32,424,288,bkCol);
//////				return;
//////			}else{
//////				g_SysPara.fTemperatureBDLast = g_GlobalPara.fTemperature;
//////				LCD_FillRect(90,50,360,220,bkCol);//��յ���
//////				ui_Sample10_InitDisp();
//////				SaveSysPara();
//////			}
//////		}
		
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
//						LCD_FillRect(0,0,480,320,WHITE);
						return;
					}else if(g_GlobalPara.iStorageFreePrcnt < 3){
						PromptDialog(DIALOGSTORLOW);
					}else{
                        
					}
					
					SaveSampFile();
					ui_SetGJNameNext((char*)SampTable[0].pList->listStr);//������һ������
					SaveSysPara();
				}else{
					//������
				}
				g_GlobalPara.iSaveDataCnt = 0;
			}
			SendUiNo(UI_MAIN);
//			LCD_FillRect(0,0,480,320,WHITE);
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
			g_GlobalPara.ucDispModepage = 0;//�����ʾ
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
			SendUiNo(UI_SAMPDISP);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//����
		if(g_u8TouchResult == 3){
			SendUiNo(UI_CONDITION);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//����
		if(g_u8TouchResult == 4){
//            SaveTableDataFile();
			SendUiNo(UI_ANALYZE);
			LCD_FillRect(56,32,424,288,bkCol);
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
			return;
		}
		//�궨
		if(g_u8TouchResult == 5){
			SendUiNo(UI_BIAODING);
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//ͨ��ѡ��
		if((g_u8TouchResult >= 6)&&(g_u8TouchResult <= 8)){
			g_DetectPara.ChannelLast = g_DetectPara.Channel;
			g_DetectPara.Channel = g_u8TouchResult - 6;
			if(g_DetectPara.Channel != g_DetectPara.ChannelLast){
				if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){
					BrushChannelSel(g_DetectPara.Channel,1);
					DeleteChannelLine(g_DetectPara.ChannelLast);
					BrushTiaoXingPage(g_DetectPara.PageView,g_DetectPara.ChannelLast);
					BrushTiaoXingPage(g_DetectPara.PageView,g_DetectPara.Channel);
					DispChannelLine(g_DetectPara.Channel);
				}else{
					BrushChannelSel(g_DetectPara.Channel,0);
				}
				BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,g_SysPara.SampleDispMode);
//				BrushCar(g_iCarDist);
			}
		}
        
		//�϶��л�ͨ��
		for(i=0;i<3;i++){
			if(g_ButtonBuf[9+i].btSta == BTN_STATUS_DRAG){
				g_DetectPara.ChannelLast = g_DetectPara.Channel;
				g_DetectPara.Channel = Get_DragChannel(&g_ButtonBuf[9],3,DRAG_ZONG);
				if(g_DetectPara.Channel != g_DetectPara.ChannelLast){
					if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){
						BrushChannelSel(g_DetectPara.Channel,1);
						
						DeleteChannelLine(g_DetectPara.ChannelLast);
						BrushTiaoXingPage(g_DetectPara.PageView,g_DetectPara.ChannelLast);
						BrushTiaoXingPage(g_DetectPara.PageView,g_DetectPara.Channel);
						DispChannelLine(g_DetectPara.Channel);
					}else{
						BrushChannelSel(g_DetectPara.Channel,0);
					}
					BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,g_SysPara.SampleDispMode);
//					BrushCar(g_iCarDist);
				}
			}
		}
		
		//������ҳ
		if(g_u8TouchResult == SLIDE_END_LEFT){//��
			if(g_DetectPara.PageView < g_DetectPara.PageCurr){
				g_DetectPara.PageView += 1;
				BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,g_SysPara.SampleDispMode);
				if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){//����
					ClearTiaoxingPage(0);
					ClearTiaoxingPage(1);
					ClearTiaoxingPage(2);
					BrushTiaoXingPage(g_DetectPara.PageView,0);
					BrushTiaoXingPage(g_DetectPara.PageView,1);
					BrushTiaoXingPage(g_DetectPara.PageView,2);
					DispChannelLine(g_DetectPara.Channel);
				}
			}
		}
        
		if(g_u8TouchResult == SLIDE_END_RIGHT){//�һ�
			if(g_DetectPara.PageView > 1){
				g_DetectPara.PageView -= 1;
				BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,g_SysPara.SampleDispMode);
				if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){//����
					ClearTiaoxingPage(0);
					ClearTiaoxingPage(1);
					ClearTiaoxingPage(2);
					BrushTiaoXingPage(g_DetectPara.PageView,0);
					BrushTiaoXingPage(g_DetectPara.PageView,1);
					BrushTiaoXingPage(g_DetectPara.PageView,2);
					DispChannelLine(g_DetectPara.Channel);
				}
			}
		}
		
		Sleep_ms(5);
	}
}
////////////////////////////////////////////////////////////////////
//�洢����
void SaveSampFile(void)
{
	FRESULT fres;
	int offset = 0;
	u8 fileDir[32];
	
	//�ļ�����ͷ��ʼ��
	FlieDataHead.iVersion = 3;
	sprintf(FlieDataHead.YiQiBianHao,"%s",g_SysPara.EquipmentNum);
	sprintf(FlieDataHead.GouJianName,"%s",SampTable[0].pList->listStr);
	
	memcpy(&FlieDataHead.FileTimeStruct,&g_SysTime,sizeof(g_SysTime));
	
	FlieDataHead.ucDetectType = QUICK;//�������
	FlieDataHead.ucGouJianType = *SampTable[8].ParaAddr;;//��������
	FlieDataHead.ucRebarType = *SampTable[4].ParaAddr;;//�ֽ�����
	
	FlieDataHead.ucZhuJinDiam = *SampTable[1].ParaAddr;//����ֱ��
	FlieDataHead.ucZhuJinJianju = *SampTable[2].ParaAddr;//������
	FlieDataHead.ucGuJinDiam = *SampTable[5].ParaAddr;//����ֱ��
	FlieDataHead.ucGuJinJianju = *SampTable[6].ParaAddr;//������
	FlieDataHead.ucHouDuDesign = *SampTable[3].ParaAddr;;//��ƺ��
	
	GetHouDuAvg();
	
	FlieDataHead.ucHouDuAvg = (u8)g_GlobalPara.ucHouduAvg;
	FlieDataHead.ucHouDuMin = (u8)g_GlobalPara.ucHouduMin;
	FlieDataHead.ucHouDuMax = (u8)g_GlobalPara.ucHouduMax;
	
	FlieDataHead.iCarSite = g_iCarDist;
	FlieDataHead.iSaveDotCntCh1 = ResBuf_CH1.AllRebarCnt;
	FlieDataHead.iSaveDotCntCh2 = ResBuf_CH2.AllRebarCnt;
	FlieDataHead.iSaveDotCntCh3 = ResBuf_CH3.AllRebarCnt;
    
    FlieDataHead.iCH1Start = SourceData.iCH1Start;
    FlieDataHead.iCH2Start = SourceData.iCH2Start;
    FlieDataHead.iCH3Start = SourceData.iCH3Start;
    FlieDataHead.iCH4Start = SourceData.iCH4Start;
    FlieDataHead.iCH5Start = SourceData.iCH5Start;
    FlieDataHead.iCH6Start = SourceData.iCH6Start;
	
//	FlieDataHead.iHeGeDotCntCh1 = g_GlobalPara.iSaveDataCnt;
//	FlieDataHead.iHeGeDotCntCh2 = g_GlobalPara.iSaveDataCnt;
//	FlieDataHead.iHeGeDotCntCh3 = g_GlobalPara.iSaveDataCnt;
	
	sprintf((char*)fileDir,"0:/GY91/%s.bin",FlieDataHead.GouJianName);
	fres = f_open(file,(const char*)fileDir,FA_WRITE|FA_READ|FA_CREATE_NEW);
	if(fres == FR_EXIST){
		if(PromptDialog(DIALOGGOUJIANEXIST)){
			fres = f_open(file,(const char*)fileDir,FA_WRITE|FA_READ|FA_CREATE_ALWAYS);
		}else{
			return;
		}
	}
	fres = f_write(file,&FlieDataHead,sizeof(FlieDataHead) ,&bw);//д�ļ�ͷ
	offset = 128;
	fres = f_lseek(file,offset);
	fres = f_write(file,&ResBuf_CH1.RebarSite,ResBuf_CH1.AllRebarCnt*sizeof(unsigned short),&bw);//д����ch1
	offset += bw;
	fres = f_lseek(file,offset);
	f_write(file,&ResBuf_CH1.RebarCover,ResBuf_CH1.AllRebarCnt*sizeof(unsigned char),&bw);//д����ch1
	offset += bw;
//	f_lseek(file,offset);
//	f_write(file,"end1",4,&bw);//д����У��ch1
//	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH2.RebarSite,ResBuf_CH2.AllRebarCnt*sizeof(unsigned short),&bw);//д����ch2
	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH2.RebarCover,ResBuf_CH2.AllRebarCnt*sizeof(unsigned char),&bw);//д����ch2
	offset += bw;
//	f_lseek(file,offset);
//	f_write(file,"end2",4,&bw);//д����У��ch1
//	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH3.RebarSite,ResBuf_CH3.AllRebarCnt*sizeof(unsigned short),&bw);//д����ch3
	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH3.RebarCover,ResBuf_CH3.AllRebarCnt*sizeof(unsigned char),&bw);//д����ch3
	offset += bw;
//	f_lseek(file,offset);
//	f_write(file,"end3",4,&bw);//д����У��ch1

    f_lseek(file,offset);
	f_write(file, &SourceData.iCH1, g_iCarDist*sizeof(unsigned short), &bw);//д����ch1
	offset += bw;
    
    f_lseek(file,offset);
	f_write(file, &SourceData.iCH2, g_iCarDist*sizeof(unsigned short), &bw);//д����ch2
	offset += bw;
    
    f_lseek(file,offset);
	f_write(file, &SourceData.iCH3, g_iCarDist*sizeof(unsigned short), &bw);//д����ch3
	offset += bw;
    
    f_lseek(file,offset);
	f_write(file, &SourceData.iCH4, g_iCarDist*sizeof(unsigned short), &bw);//д����ch4
	offset += bw;
    
    f_lseek(file,offset);
	f_write(file, &SourceData.iCH5, g_iCarDist*sizeof(unsigned short), &bw);//д����ch5
	offset += bw;
    
    f_lseek(file,offset);
	f_write(file, &SourceData.iCH6, g_iCarDist*sizeof(unsigned short), &bw);//д����ch6
	offset += bw;

	f_close(file);
    
    Sleep_ms(50);
    
    BT_RealSendFile((char*)(fileDir+2));//ʵʱ�ϴ�����
	
	PromptTone_Once(120);//��ʾ��
}

//���洢����	
int GetSampDataCnt(void)
{
	u8 i;
	int max = 0;
	
	for(i=1;i<3;i++){
		if(ResBuf[i]->AllRebarCnt > max){
			max = ResBuf[i]->AllRebarCnt;
		}
	}
	return max;
}

//���ƽ��ֵ
void GetHouDuAvg(void)
{
	u16 i,j;
	int sum = 0;
	int cnt = 0;
	
	if(ResBuf[i]->AllRebarCnt == 0){
		g_GlobalPara.ucHouduMin = 0;
		g_GlobalPara.ucHouduMax = 0;
		g_GlobalPara.ucHouduAvg = 0;
	}else{
		g_GlobalPara.ucHouduMin = 255;
		g_GlobalPara.ucHouduMax = 0;
		for(i=0;i<3;i++){
			for(j=0;j<ResBuf[i]->AllRebarCnt;j++){
				sum += ResBuf[i]->RebarCover[j];
				cnt += 1;
				if(ResBuf[i]->RebarCover[j] < g_GlobalPara.ucHouduMin){
					g_GlobalPara.ucHouduMin = ResBuf[i]->RebarCover[j];
				}
				if(ResBuf[i]->RebarCover[j] > g_GlobalPara.ucHouduMax){
					g_GlobalPara.ucHouduMax = ResBuf[i]->RebarCover[j];
				}
			}
		}
		g_GlobalPara.ucHouduAvg = sum/cnt;
	}
}
