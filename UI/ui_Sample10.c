/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：厚度测量
    1. ui_Sample10 界面初始化
    2. ui_Sample10_Run 函数实现

更新记录：
    1. 2018.03.01 初次创建
    
*******************************************************************************/
#include "ui_Sample10.h"
#include "Config_ui.h"
#include "AimLib.h"


DETECTPARA g_DetectPara;//检测参数

SYS_PARA_TABLE_STRUCT* SampTable = &ParaTable[PARATABLEINDEX_SAMPLE];

//static FRESULT fres;



char *ChName[3] = {"上","中","下"};
char *Condition[5] = {"常规空","高低筋","避箍筋","凹凸面","反修正"};

u16 weiy1[20] = {40,80,120,160,200,240,290,320,350,410,440,480,520,560,600,640,680,720,760,790};
u8 houd1[20] = {20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210};

u16 weiy2[20] = {42,82,122,162,202,242,280,320,360,412,442,482,522,562,602,642,682,722,762,792};
u8 houd2[20] = {20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210};

u16 weiy3[20] = {44,84,124,164,204,244,270,320,370,414,444,484,524,564,604,644,684,724,764,794};
u8 houd3[20] = {20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210};

//函数声明
int GetSampDataCnt(void);
void SaveSampFile(void);

void ui_Sample10_InitDisp(void)
{
	u8 i;
	u16 SampColor_Bk; 
	u16 SampColor_Txt;
	u16 SampBarColor_Bk;
	u16 SampBarColor_Txt;
    
//////	//测试数据
//////	ResBuf_CH1.AllRebarCnt = 20;
//////	ResBuf_CH2.AllRebarCnt = 20;
//////	ResBuf_CH3.AllRebarCnt = 20;
//////	memcpy(ResBuf_CH1.RebarSite,weiy1,sizeof(weiy1));
//////	memcpy(ResBuf_CH1.RebarCover,houd1,sizeof(houd1));
//////	memcpy(ResBuf_CH2.RebarSite,weiy2,sizeof(weiy2));
//////	memcpy(ResBuf_CH2.RebarCover,houd2,sizeof(houd2));
//////	memcpy(ResBuf_CH3.RebarSite,weiy3,sizeof(weiy3));
//////	memcpy(ResBuf_CH3.RebarCover,houd3,sizeof(houd3));
	
	//深色背景
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
	
	//页面左侧菜单表
	MENU_TABLE_STRUCT Samp10Menu[6] = {
		{0,6,"徊",NULL},
		{1,6,"参","参数"},
		{2,6,"显","显示"},
		{3,6,"况","工况"},
		{4,6,"析","热力图"},
		{5,6,"标","标定"}
	};
	//菜单显示
	ui_DispMenuBtn(Samp10Menu,0);
	
	//状态栏
	LCD_DrawRect(56,0,424,32,SampColor_Bk);
	LCD_FillRect(57,1,422,31,SampBarColor_Bk);
	
	//分割线
	LCD_FillRect(57,192,422,2,SampColor_Txt);//横
	for(i=0;i<42;i++){
		LCD_SetColor(SampColor_Bk,SampColor_Txt);
		LCD_ShowChinese(Font8,58+i*10,192,1,"杠");
	}
	LCD_FillRect(57,298,420,1,SampColor_Txt);//横
	
//	LCD_FillRect(57,32,422,160,SampColor_Bk);//刷背景色
	 
	LCD_SetColor(SampBarColor_Bk,SampBarColor_Txt);
	//钢筋类型
	LCD_ShowChinese(Font16,164,8,0,SampTable[4].pList[g_SysPara.g_SamplePara.iRebarType].listStr);
	//主筋直径
	LCD_ShowChinese(Font20,198,6,0,"φ");
	LCD_ShowNums(ASC20x10,216,6,0,*SampTable[1].ParaAddr);
	//工况
	LCD_ShowChinese(Font16,248,8,0,(u8*)Condition[g_DetectPara.WorkCondition]);
	//存储方式
	if(g_SysPara.g_SamplePara.iSaveType != 0){//手存
		LCD_ShowString(ASC20x10,308,6,0,"M");
	}
	//计数图标
	LCD_ShowChinese(Font24,410,5,0,"计");
	DispSaveDataCnt(g_GlobalPara.iSaveDataCnt);
    
	//按钮初始化
	g_u8ButtonNums = 12;
	struct BUTTON SampBtn[12] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0},//0, 返回
		{  0,  50,  59, 103, 0,0},//1, 参数
		{  0, 104,  59, 157, 0,0},//2, 显示
		{  0, 158,  59, 211, 0,0},//3, 工况
		{  0, 212,  59, 265, 0,0},//4, 分析
		{  0, 266,  59, 319, 0,0},//5, 标定
		
		{440,  32, 479,  85, 0,0},//6, 通道1
		{440,  86, 479, 138, 0,0},//7, 通道2
		{440, 139, 479, 191, 0,0},//8, 通道3
		
		{ 70,  32, 439,  85, 0,BTN_ABILITY_DRAG},// 9, 通道1 拖动
		{ 70,  86, 439, 138, 0,BTN_ABILITY_DRAG},//10, 通道2 拖动
		{ 70, 139, 439, 191, 0,BTN_ABILITY_DRAG},//11, 通道3 拖动
	};
	memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	
	//初始化参数
	if(g_GlobalPara.ucFromPageNum == UI_MAIN){
		//初始化参数
		g_DetectPara.MZJSite = 0;
//		g_DetectPara.CarSite = 0;
		
		//通道选择
		g_DetectPara.Channel = 1;
		g_DetectPara.ChannelLast = 1;
		
		//清空缓存
		memset(&ResBuf_CH1,0x0,sizeof(ResBuf_CH1));
		memset(&ResBuf_CH2,0x0,sizeof(ResBuf_CH2));
		memset(&ResBuf_CH3,0x0,sizeof(ResBuf_CH3));
		
		g_GlobalPara.iSaveDataCnt = 0;
		g_GlobalPara.ucHouduAvg = 0;
		g_GlobalPara.ucHouduMin = 0;
		g_GlobalPara.ucHouduMax = 0;
	}
    
	//存储容量
	g_GlobalPara.iStorageFreePrcnt = Get_FatFreePrcnt();
	g_DetectPara.PageCurr = g_iCarDist/g_GlobalPara.sDispScreenWide+1;
	g_DetectPara.PageView = g_DetectPara.PageCurr;
	g_DetectPara.PageLast = g_DetectPara.PageCurr;
	
	//构建名
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
	
	//显示测点图形
	BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,g_SysPara.SampleDispMode);

    //检测参数赋值
    g_tWorkPara.cZhujinDiam = g_SysPara.g_SamplePara.iZhujinDiam;//主筋直径
    g_tWorkPara.cZhujinDistance = 0;	//主筋间距
    g_tWorkPara.cGujinDiam = 0;		    //箍筋直径
    g_tWorkPara.cGujinDistance = 0;	    //箍筋间距

    if(g_tWorkPara.cGangJinType == 0 ){
        g_tWorkPara.cGangJinType = 1;
        g_SysPara.g_SamplePara.iRebarType = 1;
    }
    else{
        g_tWorkPara.cGangJinType = 0;
        g_SysPara.g_SamplePara.iRebarType = 0;
    }
    LCD_ShowChinese(Font16,164,8,0,SampTable[4].pList[g_SysPara.g_SamplePara.iRebarType].listStr);//钢筋类型
    g_tWorkPara.cLingChengType = 1;	    //大小量程

    g_tWorkPara.cJiLiang = 0;			//计量模式
    g_tWorkPara.cShejiHD = 30;			//设计厚度

    ClearTouchSta();//清除触摸

//////    //初始化原始数据存储参数
//////    InitSourceDataPara();//数据采集相关
//////	
//////    //显示原始数据存储参数
//////    DispSourceDataPara();//数据采集相关
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
	//深色背景
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
	
	//电量低提示
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
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;//假装从主页进入
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
	}
	
	while(1)
	{
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		//点击效果
		ui_AllButtonReaction(6,menuBtCol,GOLD);

//		ui_DispTime(0);//时间
		ui_GetTempera();//采集温度
		ui_DispBattery(2);//采集但不显示电量
		
		//蓝牙
		if(g_SysPara.iBlueToothSta == 1){
			BT_Lookup();
		}
		
        //ADC采集
		Get_Adc_Data();
        
        //超过最大缓存长度
        if(g_iCarDist > SOURCEDATAMAXCNT){
            PromptTone_Once(200);//提示音
            g_iCarDist = SOURCEDATAMAXCNT;
            return;
        }
        
        if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){
            //有位移变化再进行运算
            if(g_iCarDist != g_iCarLastDist){
                
                //位移变化标志位
                cDistChangeFlag = 1;
                
                //运行钢筋定位算法
                AimFuncRun_HD(g_DetectPara.Channel, g_SysPara.SampleDispMode);
                
                
                
//////                EstimateDiam();
                
                
                g_iCarLastDist = g_iCarDist;
            }
        }
        else if((g_SysPara.SampleDispMode == MIAO_POU)||(g_SysPara.SampleDispMode == MIAO_BO)){
            //更新瞄准镜位置
            ucAimState = UpdateMZK(g_DetectPara.Channel);
            
            //有位移变化再进行运算
            if(g_iCarDist != g_iCarLastDist){

                //位移变化标志位
                cDistChangeFlag = 1;
                
                //瞄准镜定位钢筋算法
                Res = AimFuncRun_MZK(g_DetectPara.Channel, ucAimState, g_SysPara.SampleDispMode);
                
//////                //显示瞄准镜检测结果
//////                if((Res != 0)&&(g_iCarDist >= g_iCarMaxDist)){
//////                    g_DetectPara.HouduRes = Res;
//////                    sprintf(cBuf,"%-2d",g_DetectPara.HouduRes);
//////                    LCD_SetColor(bkCol,txCol);
//////                    LCD_ShowString(ASC24x12,384,146,0,cBuf);//厚度值
//////                }
                
                g_iCarLastDist = g_iCarDist;
            }
        }

		//绘制页面
		if(cDistChangeFlag == 1){
            cDistChangeFlag = 0;
			g_DetectPara.PageCurr = g_iCarDist/g_GlobalPara.sDispScreenWide+1;
			if(g_DetectPara.PageCurr != g_DetectPara.PageLast){
				if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){
					ClearTiaoxingPage(0);//清空条形区域
					ClearTiaoxingPage(1);//清空条形区域
					ClearTiaoxingPage(2);//清空条形区域
					BrushTiaoXingPage(g_DetectPara.PageCurr,0);
					BrushTiaoXingPage(g_DetectPara.PageCurr,1);
					BrushTiaoXingPage(g_DetectPara.PageCurr,2);
					DispChannelLine(g_DetectPara.Channel);
				}
                else if((g_SysPara.SampleDispMode == MIAO_POU)||(g_SysPara.SampleDispMode == MIAO_BO)){
//                    BrushMZJ(GetMZJSite(),u16 color);
//////                    UpdateMZK(g_DetectPara.Channel);
                }
//				ClearCurvePage();//清空曲线区域
				BrushCurvePage(g_DetectPara.PageCurr, g_DetectPara.Channel, g_SysPara.SampleDispMode);
				g_DetectPara.PageView = g_DetectPara.PageCurr;
				g_DetectPara.PageLast = g_DetectPara.PageCurr;
			}
			if(g_DetectPara.PageCurr != g_DetectPara.PageView){
				if(g_iCarDist != g_iCarDistLast){
					if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){
						ClearTiaoxingPage(0);//清空条形区域
						ClearTiaoxingPage(1);//清空条形区域
						ClearTiaoxingPage(2);//清空条形区域
						BrushTiaoXingPage(g_DetectPara.PageCurr,0);
						BrushTiaoXingPage(g_DetectPara.PageCurr,1);
						BrushTiaoXingPage(g_DetectPara.PageCurr,2);
						DispChannelLine(g_DetectPara.Channel);
					}
	//				ClearCurvePage();//清空曲线区域
					BrushCurvePage(g_DetectPara.PageCurr,g_DetectPara.Channel,g_SysPara.SampleDispMode);
					g_DetectPara.PageView = g_DetectPara.PageCurr;
					g_DetectPara.PageLast = g_DetectPara.PageCurr;
				}
			}
			if(g_iCarDir < 0){//小车后退
//////				DeleteCurveOne(g_DetectPara.Channel, g_iCarDist, g_SysPara.SampleDispMode);
			}else{
				//检测钢筋
//				if(检测到钢筋){
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

		//存储构件
		if(g_u16KeyValBuf == KEY_OK){
            
            //保存原始数据CSV文件
            SaveSourceDataFile();
            
            
			g_GlobalPara.iSaveDataCnt = GetSampDataCnt();
			if(g_GlobalPara.iSaveDataCnt > 0){
				//存储空间不足提示
				if(g_GlobalPara.iStorageFreePrcnt < 1){
					PromptDialog(DIALOGSTORNONE);
					g_GlobalPara.iSaveDataCnt = 0;
					SendUiNo(UI_SAMPLE10);
					g_GlobalPara.ucFromPageNum = UI_MAIN;//假装从主页进入
					LCD_FillRect(56,32,424,288,bkCol);
					return;
				}else if(g_GlobalPara.iStorageFreePrcnt < 3){
					PromptDialog(DIALOGSTORLOW);
				}else{
					
				}
				
				SaveSampFile();
				ui_SetGJNameNext((char*)SampTable[0].pList->listStr);//设置下一构建名
				SaveSysPara();
				g_GlobalPara.iSaveDataCnt = 0;
				
				SendUiNo(UI_SAMPLE10);
				g_GlobalPara.ucFromPageNum = UI_MAIN;//假装从主页进入
				LCD_FillRect(56,32,424,288,bkCol);
                g_iCarDist = 0;
                g_iCarLastDist = 0;
                
//                g_iCarCountZero = g_iCarCount;
                //            g_iCh1SumBuf = 0;
                //            g_iCh2SumBuf = 0;
                //            g_iCh3SumBuf = 0;

                //初始化定位钢筋缓存
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
		//清屏
		if(g_u16KeyValBuf == KEY_FN){
			if(g_SysPara.g_SamplePara.iSaveType == 0){//自动存储
				ClearCarDist();//清零位移
				memset(&ResBuf_CH1,0,sizeof(ResBuf_CH1));//清空数据缓存
				memset(&ResBuf_CH2,0,sizeof(ResBuf_CH2));//清空数据缓存
				memset(&ResBuf_CH3,0,sizeof(ResBuf_CH3));//清空数据缓存
                
                
				if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){
					ClearTiaoxingPage(0);//清空条形区域
					ClearTiaoxingPage(1);//清空条形区域
					ClearTiaoxingPage(2);//清空条形区域
					DispChannelLine(g_DetectPara.Channel);
				}
				ClearCurvePage();//清屏曲线
                
                BrushCar(g_iCarDist);
			
                g_iCarDist = 0;
                g_iCarLastDist = 0;
                g_iCarMaxDist = 0;
//////                g_iCarCountZero = g_iCarCount;

    //            g_iCh1SumBuf = 0;
    //            g_iCh2SumBuf = 0;
    //            g_iCh3SumBuf = 0;
                
                //初始化定位钢筋缓存
                InitAimPara();
                
                //显示钢筋根数
                DispRebarCnt(g_DetectPara.Channel);
                
                
                
                //TEST_ZJC
                LCD_SetColor(WHITE,ORANGE);//CL_BLUE
                LCD_ShowString(ASC16x8,0, 180, 0,"          ");
                LCD_ShowString(ASC16x8,0, 200, 0,"          ");
                LCD_ShowString(ASC16x8,0, 160, 0,"          ");
                //END
                
                

//				BrushCar(g_iCarDist);
			}else{//手动存储
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
		//标定
		if(g_u16KeyValBuf == LONG_KEY_FN){
			SendUiNo(UI_BIAODING);
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//电量低标定
		if(g_GlobalPara.iBatPcntLowFlag == 1){
			g_GlobalPara.iBatPcntLowFlag = 0;
			if(PromptDialog(DIALOGBIAODINGBATLOW) == 1){
				SendUiNo(UI_BIAODING);
				g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}else{
				LCD_FillRect(90,50,360,220,bkCol);//清空弹窗
				ui_Sample10_InitDisp();
			}
		}
//////		//电量变化大标定
//////		if(abs(g_GlobalPara.iBatPercent-g_SysPara.iBatPercentBDLast) > 20){
//////			if(g_GlobalPara.iChargeSta == CHARGE_NO){
//////				if(PromptDialog(DIALOGBIAODINGBATJUMP) == 1){
//////					SendUiNo(UI_BIAODING);
//////					g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
//////					LCD_FillRect(56,32,424,288,bkCol);
//////					return;
//////				}else{
//////					g_SysPara.iBatPercentBDLast = g_GlobalPara.iBatPercent;
//////					LCD_FillRect(90,50,360,220,bkCol);//清空弹窗
//////					ui_Sample10_InitDisp();
//////					SaveSysPara();
//////				}
//////			}
//////		}
//////		//温度变化大标定
//////		if(fabs(g_GlobalPara.fTemperature-g_SysPara.fTemperatureBDLast) > 10.0f){
//////			if(PromptDialog(DIALOGBIAODINGTEMPER) == 1){
//////				SendUiNo(UI_BIAODING);
//////				g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
//////				LCD_FillRect(56,32,424,288,bkCol);
//////				return;
//////			}else{
//////				g_SysPara.fTemperatureBDLast = g_GlobalPara.fTemperature;
//////				LCD_FillRect(90,50,360,220,bkCol);//清空弹窗
//////				ui_Sample10_InitDisp();
//////				SaveSysPara();
//////			}
//////		}
		
		//返回
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_BACK)){
			g_GlobalPara.iSaveDataCnt = GetSampDataCnt();
			if(g_GlobalPara.iSaveDataCnt > 0){
				if(PromptDialog(DIALOGSAVE) == CONFIRM){
					//存储空间不足提示
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
					ui_SetGJNameNext((char*)SampTable[0].pList->listStr);//设置下一构建名
					SaveSysPara();
				}else{
					//不保存
				}
				g_GlobalPara.iSaveDataCnt = 0;
			}
			SendUiNo(UI_MAIN);
//			LCD_FillRect(0,0,480,320,WHITE);
			return;
		}
		//参数
		if(g_u8TouchResult == 1){
			SendUiNo(UI_SAMPSET);
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//显示
		if(g_u8TouchResult == 2){
			g_GlobalPara.ucDispModepage = 0;//检测显示
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
			SendUiNo(UI_SAMPDISP);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//工况
		if(g_u8TouchResult == 3){
			SendUiNo(UI_CONDITION);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//分析
		if(g_u8TouchResult == 4){
//            SaveTableDataFile();
			SendUiNo(UI_ANALYZE);
			LCD_FillRect(56,32,424,288,bkCol);
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
			return;
		}
		//标定
		if(g_u8TouchResult == 5){
			SendUiNo(UI_BIAODING);
			g_GlobalPara.ucFromPageNum = UI_SAMPLE10;
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//通道选择
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
        
		//拖动切换通道
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
		
		//滑动翻页
		if(g_u8TouchResult == SLIDE_END_LEFT){//左划
			if(g_DetectPara.PageView < g_DetectPara.PageCurr){
				g_DetectPara.PageView += 1;
				BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,g_SysPara.SampleDispMode);
				if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){//条形
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
        
		if(g_u8TouchResult == SLIDE_END_RIGHT){//右划
			if(g_DetectPara.PageView > 1){
				g_DetectPara.PageView -= 1;
				BrushCurvePage(g_DetectPara.PageView,g_DetectPara.Channel,g_SysPara.SampleDispMode);
				if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){//条形
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
//存储构件
void SaveSampFile(void)
{
	FRESULT fres;
	int offset = 0;
	u8 fileDir[32];
	
	//文件数据头初始化
	FlieDataHead.iVersion = 3;
	sprintf(FlieDataHead.YiQiBianHao,"%s",g_SysPara.EquipmentNum);
	sprintf(FlieDataHead.GouJianName,"%s",SampTable[0].pList->listStr);
	
	memcpy(&FlieDataHead.FileTimeStruct,&g_SysTime,sizeof(g_SysTime));
	
	FlieDataHead.ucDetectType = QUICK;//检测类型
	FlieDataHead.ucGouJianType = *SampTable[8].ParaAddr;;//构件类型
	FlieDataHead.ucRebarType = *SampTable[4].ParaAddr;;//钢筋类型
	
	FlieDataHead.ucZhuJinDiam = *SampTable[1].ParaAddr;//主筋直径
	FlieDataHead.ucZhuJinJianju = *SampTable[2].ParaAddr;//主筋间距
	FlieDataHead.ucGuJinDiam = *SampTable[5].ParaAddr;//箍筋直径
	FlieDataHead.ucGuJinJianju = *SampTable[6].ParaAddr;//箍筋间距
	FlieDataHead.ucHouDuDesign = *SampTable[3].ParaAddr;;//设计厚度
	
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
	fres = f_write(file,&FlieDataHead,sizeof(FlieDataHead) ,&bw);//写文件头
	offset = 128;
	fres = f_lseek(file,offset);
	fres = f_write(file,&ResBuf_CH1.RebarSite,ResBuf_CH1.AllRebarCnt*sizeof(unsigned short),&bw);//写数据ch1
	offset += bw;
	fres = f_lseek(file,offset);
	f_write(file,&ResBuf_CH1.RebarCover,ResBuf_CH1.AllRebarCnt*sizeof(unsigned char),&bw);//写数据ch1
	offset += bw;
//	f_lseek(file,offset);
//	f_write(file,"end1",4,&bw);//写数据校验ch1
//	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH2.RebarSite,ResBuf_CH2.AllRebarCnt*sizeof(unsigned short),&bw);//写数据ch2
	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH2.RebarCover,ResBuf_CH2.AllRebarCnt*sizeof(unsigned char),&bw);//写数据ch2
	offset += bw;
//	f_lseek(file,offset);
//	f_write(file,"end2",4,&bw);//写数据校验ch1
//	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH3.RebarSite,ResBuf_CH3.AllRebarCnt*sizeof(unsigned short),&bw);//写数据ch3
	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH3.RebarCover,ResBuf_CH3.AllRebarCnt*sizeof(unsigned char),&bw);//写数据ch3
	offset += bw;
//	f_lseek(file,offset);
//	f_write(file,"end3",4,&bw);//写数据校验ch1

    f_lseek(file,offset);
	f_write(file, &SourceData.iCH1, g_iCarDist*sizeof(unsigned short), &bw);//写数据ch1
	offset += bw;
    
    f_lseek(file,offset);
	f_write(file, &SourceData.iCH2, g_iCarDist*sizeof(unsigned short), &bw);//写数据ch2
	offset += bw;
    
    f_lseek(file,offset);
	f_write(file, &SourceData.iCH3, g_iCarDist*sizeof(unsigned short), &bw);//写数据ch3
	offset += bw;
    
    f_lseek(file,offset);
	f_write(file, &SourceData.iCH4, g_iCarDist*sizeof(unsigned short), &bw);//写数据ch4
	offset += bw;
    
    f_lseek(file,offset);
	f_write(file, &SourceData.iCH5, g_iCarDist*sizeof(unsigned short), &bw);//写数据ch5
	offset += bw;
    
    f_lseek(file,offset);
	f_write(file, &SourceData.iCH6, g_iCarDist*sizeof(unsigned short), &bw);//写数据ch6
	offset += bw;

	f_close(file);
    
    Sleep_ms(50);
    
    BT_RealSendFile((char*)(fileDir+2));//实时上传功能
	
	PromptTone_Once(120);//提示音
}

//测点存储计数	
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

//厚度平均值
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
