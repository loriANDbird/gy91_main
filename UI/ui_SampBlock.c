/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：数据删除
    1. ui_Delete 界面初始化
    2. ui_Delete_Run 函数实现
   
更新记录：
    1. 2018.03.01 初次创建
    
*******************************************************************************/
#include "ui_SampBlock.h"
#include "Config_ui.h"

#define BLOCKSAMPSTOP	0
#define BLOCKSAMPHANG	1
#define BLOCKSAMPLIE	2

#define GRIDWIDTH		120
#define mmPerPix4x4		(120/60)
#define mmPerPix8x8		(120/30)

static u8 BlockGridQnt;

static u8 BlockSampSta;
static short BlockCarDist;
static short BlockCarDistLast;
static short BlockCarDistMax;
static short BlockCarDistZero;

static u8 SelNumHangLie;//0-15,4x4:0-3行,8-11列;8x8:0-7行,8-15列;
static u8 SelNumHang;
static u8 SelNumLie;
//static u8 SelNumLastHang;
//static u8 SelNumLastLie;

//后退时当前行列待删除数据最大下标
static u16 DirMaxIndex[3];
//后退时当前行列待删除数据最大位移
static u16 DirMaxDist[3];
//后退时当前行列已存数据个数
static u16 DirSaveCnt[3];

static u8 ViewAbleFlag;

SYS_PARA_TABLE_STRUCT* BlockParaTable = &ParaTable[PARATABLEINDEX_BLOCK];

static u16 CarTrackBuf_Hang[8];//小车横向扫描轨迹记录
static u16 CarTrackBuf_Lie[8];//小车纵向扫描轨迹记录


//static BlOCKDATA BlockDataBuf[400];

//static u16 GrayScale[16] = {};

//页面左侧菜单表
static MENU_TABLE_STRUCT Samp10Menu[6] = {
	{0,6,"徊",NULL},
	{1,6,"参","参数"},
	{2,6,"始","开始"},
	{3,6,"览","预览"},
	{4,6,"存","保存"},
	{5,6,"建","新建"}
};

//函数声明
void BlockSample_Init(void);
void BlockParaSet_Init(SYS_PARA_TABLE_STRUCT* BlockParaTable,u8 pageNum);
void BlockParaSet_Fun(SYS_PARA_TABLE_STRUCT* BlockParaTable);

void SaveBlockFile(void);
u8 IsViewAbleFun(void);

void SaveBlockData(u8 ch,u8 dir,u8 dist,u8 houdu);
void GetBlockDataDirInfo(u8 ch,u8 dir);
void DeleteBlockDataOne(u8 ch,u8 dir,u8 dist);
void DeleteBlockDataOneDir(u8 ch,u8 dir);

void BlockPreview_Fun(void);
void BrushCarNumDisp(u8 hangNum,u8 lieNum);
void DispCarTrack(u8 hangNum,u8 lieNum);
void DispCarTrackCur(u8 hlnum);
void BrushCarTrackOne(u8 hlnum);
void BrushCarTrackAll(void);
int GetBlockDataCnt(void);
void DispBlockDist(short dist);

void ui_SampBlock_InitDisp(void)
{
	
	//参数初始化
	if(g_GlobalPara.ucFromPageNum == UI_MAIN){
		
		//参数初始化
		BlockSampSta = BLOCKSAMPSTOP;
		SelNumHangLie = 0;
		SelNumHang = 0;
		SelNumLie = 0xff;
		
		ClearCarDist();
		BlockCarDist = 0;
		BlockCarDistLast = 0;
		BlockCarDistZero = 0;
		
		g_GlobalPara.iSaveDataCnt = 0;
		g_GlobalPara.ucHouduAvg = 0;
		g_GlobalPara.ucHouduMin = 0;
		g_GlobalPara.ucHouduMax = 0;
		
		//清空缓存
		memset(&ResBuf_CH1,0x0,sizeof(ResBuf_CH1));
		memset(&ResBuf_CH2,0x0,sizeof(ResBuf_CH2));
		memset(&ResBuf_CH3,0x0,sizeof(ResBuf_CH3));
		
		memset(CarTrackBuf_Hang,0x0,sizeof(CarTrackBuf_Hang));
		memset(CarTrackBuf_Lie,0x0,sizeof(CarTrackBuf_Lie));
	}
	//存储容量
	g_GlobalPara.iStorageFreePrcnt = Get_FatFreePrcnt();
	
	BlockSample_Init();
	
	//测试数据
//	ResBuf_CH1.AllRebarCnt = 4;
//	ResBuf_CH2.AllRebarCnt = 4;
//	ResBuf_CH3.AllRebarCnt = 4;
//	
//	ResBuf_CH1.BlockRanks[0] = 0;
//	ResBuf_CH1.RebarSite[0] = 115;
//	ResBuf_CH1.RebarCover[0] = 60;
//	ResBuf_CH2.BlockRanks[0] = 0;
//	ResBuf_CH2.RebarSite[0] = 120;
//	ResBuf_CH2.RebarCover[0] = 80;
//	ResBuf_CH3.BlockRanks[0] = 0;
//	ResBuf_CH3.RebarSite[0] = 125;
//	ResBuf_CH3.RebarCover[0] = 100;
//	
//	ResBuf_CH1.BlockRanks[1] = 1;
//	ResBuf_CH1.RebarSite[1] = 130;
//	ResBuf_CH1.RebarCover[1] = 120;
//	ResBuf_CH2.BlockRanks[1] = 1;
//	ResBuf_CH2.RebarSite[1] = 135;
//	ResBuf_CH2.RebarCover[1] = 140;
//	ResBuf_CH3.BlockRanks[1] = 1;
//	ResBuf_CH3.RebarSite[1] = 140;
//	ResBuf_CH3.RebarCover[1] = 160;
//	
//	ResBuf_CH1.BlockRanks[2] = 8;
//	ResBuf_CH1.RebarSite[2] = 150;
//	ResBuf_CH1.RebarCover[2] = 120;
//	ResBuf_CH2.BlockRanks[2] = 8;
//	ResBuf_CH2.RebarSite[2] = 150;
//	ResBuf_CH2.RebarCover[2] = 100;
//	ResBuf_CH3.BlockRanks[2] = 8;
//	ResBuf_CH3.RebarSite[2] = 160;
//	ResBuf_CH3.RebarCover[2] = 80;
//	
//	ResBuf_CH1.BlockRanks[3] = 9;
//	ResBuf_CH1.RebarSite[3] = 160;
//	ResBuf_CH1.RebarCover[3] = 60;
//	ResBuf_CH2.BlockRanks[3] = 9;
//	ResBuf_CH2.RebarSite[3] = 170;
//	ResBuf_CH2.RebarCover[3] = 40;
//	ResBuf_CH3.BlockRanks[3] = 9;
//	ResBuf_CH3.RebarSite[3] = 170;
//	ResBuf_CH3.RebarCover[3] = 20;
}

void ui_SampBlock_Run(void)
{
	u8 i;
	int m=0,n=0;
    DISPREBAR_BUFFER AimRes;
//	u8 fileDir[32];
	
	u16 bkCol;
	u16 menuBtCol;
	
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
		menuBtCol = DGRAY;
	}else{
		bkCol = WHITE;
		menuBtCol = LGRAY;
	}
	
	//电量低提示
	if(g_GlobalPara.ucFromPageNum == UI_MAIN){
		Get_BatPrcnt();
		if(g_GlobalPara.iBatPercent < 20){
			PromptDialog(DIALOGBATLOW);
			g_GlobalPara.ucFromPageNum = UI_SAMPBLOCK;//假装从主页进入
			LCD_FillRect(56,32,424,288,bkCol);
			BlockSample_Init();
		}
		if(g_GlobalPara.iBatPercent < 5){
			PromptDialog(DIALOGBATNONE);
			SendUiNo(UI_MAIN);
			g_GlobalPara.ucFromPageNum = UI_SAMPBLOCK;//假装从主页进入
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
	}
	
	MENU_TABLE_STRUCT OneBtnSet[1];//修改按钮显示
    while(1)
	{
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		//点击效果
		ui_AllButtonReaction(6,menuBtCol,GOLD);
//		ui_OneButtonReactionXY(0,0,0,47,33,GOLD,ORANGE);//返回

//		ui_DispTime(0);//时间
		ui_GetTempera();//采集温度
		ui_DispBattery(2);//采集但不显示电量

		//蓝牙
		if(g_SysPara.iBlueToothSta == 1){
			BT_Lookup();
//			if(g_bBlueTooDispFlag == 1){
//				g_bBlueTooDispFlag = 0;
//				ui_DispBluetooth();
//			}
		}
		
		//数据采集xl
		if(BlockSampSta != BLOCKSAMPSTOP){
			if(Get_Adc_Data()){
				
				if(BlockCarDistZero > g_iCarDist){
					BlockCarDistZero = g_iCarDist;
				}
				if(BlockSampSta != BLOCKSAMPSTOP){
					//画扫描轨迹
					BlockCarDist = g_iCarDist - BlockCarDistZero;
					if(BlockCarDist > BlockCarDistMax){
						BlockCarDist = BlockCarDistMax;
					}
					DispBlockDist(BlockCarDist);
//					DispCarTrack(SelNumHang,SelNumLie);
					DispCarTrackCur(SelNumHangLie);
					if(g_iCarDir < 0){//后退
						if(BlockCarDist > 0){
							GetBlockDataDirInfo(0,SelNumHangLie);
							GetBlockDataDirInfo(1,SelNumHangLie);
							GetBlockDataDirInfo(2,SelNumHangLie);
							for(i=0;i<3;i++){
								if(DirSaveCnt[i] > 0){
									if(BlockCarDist < DirMaxDist[i]){
										DeleteBlockDataOne(i,SelNumHangLie,BlockCarDist);
									}
								}
							}
						}
					}else{
						//检测钢筋
						//if(){
//							SaveBlockData(0,SelNumHangLie,u8 dist,u8 houdu)
//						}
                        
                        //有位移变化再进行运算
                        if(g_iCarDist != g_iCarLastDist){
                            
                            //运行钢筋定位算法
                            AimRes = AimFuncRun_Block();
                            
                            g_iCarLastDist = g_iCarDist;
                            
                            //保存定位到的钢筋
                            if(AimRes.IsDisp == 1){
                                if(AimRes.CH1.AddRebatCnt != 0){
                                    for(i=0;i<AimRes.CH1.AddRebatCnt;i++){
                                        SaveBlockData(0,SelNumHangLie,AimRes.CH1.RebarSite[i],AimRes.CH1.RebarCover[i]);
                                    }
                                }
                                if(AimRes.CH2.AddRebatCnt != 0){
                                    for(i=0;i<AimRes.CH2.AddRebatCnt;i++){
                                        SaveBlockData(0,SelNumHangLie,AimRes.CH2.RebarSite[i],AimRes.CH2.RebarCover[i]);
                                    }
                                }
                                if(AimRes.CH3.AddRebatCnt != 0){
                                    for(i=0;i<AimRes.CH3.AddRebatCnt;i++){
                                        SaveBlockData(0,SelNumHangLie,AimRes.CH3.RebarSite[i],AimRes.CH3.RebarCover[i]);
                                    }
                                }
                            }
                        }

//                        if(g_iCarDist == 50){
//							SaveBlockData(0,SelNumHangLie,30,20);
//						}
                        
						//测完一行/列
						if(BlockCarDist >= BlockCarDistMax){
							BlockCarDist = 0;
							BlockCarDistLast = 0;
							BlockCarDistZero = g_iCarDist;
							BlockSampSta = BLOCKSAMPSTOP;
							
							SelNumHangLie += 1;
							if(BlockGridQnt == 4){//4x4时
								if((SelNumHangLie >= 4)&&(SelNumHangLie < 8)){
									SelNumHangLie += 4;
								}
							}
							if(SelNumHangLie < BlockGridQnt){//行
								SelNumHang = SelNumHangLie;
								SelNumLie = 0xff;
							}else{//列
								SelNumLie = SelNumHangLie % BlockGridQnt;
								SelNumHang = 0xff;
							}
							if(SelNumHangLie > BlockGridQnt+8-1){//测完
								SelNumHangLie = 0;
								SelNumHang = 0;
								SelNumLie = 0xff;
							}
							
							OneBtnSet[0].btnID = 2;
							OneBtnSet[0].btnQnt = 6;
							OneBtnSet[0].btnIcon = "始";
							OneBtnSet[0].btnName = "开始";
							ui_BrushMenuOneBtn(&OneBtnSet[0],NORMALLIGHT);
							
							LCD_SetColor(bkCol,RED);
							LCD_ShowChinese(Font24,418,40,0,"停");
							LCD_SetColor(bkCol,GRAY);
							LCD_ShowText(Font16,406,70,0,"未采集");
							
							ui_BrushMenuOneBtn(&Samp10Menu[1],NORMALLIGHT);
							ViewAbleFlag = IsViewAbleFun();
							g_GlobalPara.iSaveDataCnt = GetBlockDataCnt();
							if(ViewAbleFlag == 1){
								ui_BrushMenuOneBtn(&Samp10Menu[3],NORMALLIGHT);
							}else{
								ui_BrushMenuOneBtn(&Samp10Menu[3],LOWLIGHT);
							}
							if(g_GlobalPara.iSaveDataCnt > 0){
								ui_BrushMenuOneBtn(&Samp10Menu[4],NORMALLIGHT);
							}else{
								ui_BrushMenuOneBtn(&Samp10Menu[4],LOWLIGHT);
							}
//							ui_BrushMenuOneBtn(&Samp10Menu[3],NORMALLIGHT);
//							ui_BrushMenuOneBtn(&Samp10Menu[4],NORMALLIGHT);
							ui_BrushMenuOneBtn(&Samp10Menu[5],NORMALLIGHT);
							
							PromptTone_Once(120);
							BrushCarNumDisp(SelNumHang,SelNumLie);
							BrushCarTrackOne(SelNumHangLie);
						}
					}
				}
			}
		}else{
			if(Get_Adc_Data()){
				BlockCarDistZero = g_iCarDist-BlockCarDist;
			}
		}
		
		if(g_u16KeyValBuf == KEY_OK){
			if(BlockSampSta == BLOCKSAMPSTOP){
				g_GlobalPara.iSaveDataCnt = GetBlockDataCnt();
				if(g_GlobalPara.iSaveDataCnt > 0){
					//存储空间不足提示
					if(g_GlobalPara.iStorageFreePrcnt < 3){
						PromptDialog(DIALOGSTORLOW);
					}
					if(g_GlobalPara.iStorageFreePrcnt < 1){
						PromptDialog(DIALOGSTORNONE);
						g_GlobalPara.iSaveDataCnt = 0;
						
						SendUiNo(UI_SAMPLE10);
						g_GlobalPara.ucFromPageNum = UI_SAMPBLOCK;//假装从主页进入
						LCD_FillRect(56,32,424,288,bkCol);
						return;
					}
					SaveBlockFile();//存储构件
					ui_SetGJNameNext((char*)BlockParaTable[0].pList->listStr);//设置下一构建名
					SaveSysPara();
					g_GlobalPara.iSaveDataCnt = 0;
					
					SendUiNo(UI_SAMPBLOCK);
					g_GlobalPara.ucFromPageNum = UI_MAIN;//假装从主页进入
					LCD_FillRect(56,32,424,288,bkCol);
					return;
				}
			}
		}
		
		//标定
		if(g_u16KeyValBuf == LONG_KEY_FN){
			SendUiNo(UI_BIAODING);
			g_GlobalPara.ucFromPageNum = UI_SAMPBLOCK;
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		//电量低标定
		if(g_GlobalPara.iBatPcntLowFlag == 1){
			g_GlobalPara.iBatPcntLowFlag = 0;
			if(PromptDialog(DIALOGBIAODINGBATLOW) == 1){
				SendUiNo(UI_BIAODING);
				g_GlobalPara.ucFromPageNum = UI_SAMPBLOCK;
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}else{
				LCD_FillRect(90,50,360,220,bkCol);//清空弹窗
				BlockSample_Init();
			}
		}
		//电量变化大标定
		if(abs(g_GlobalPara.iBatPercent-g_SysPara.iBatPercentBDLast) > 20){
			if(g_GlobalPara.iChargeSta == CHARGE_NO){
				if(PromptDialog(DIALOGBIAODINGBATJUMP) == 1){
					SendUiNo(UI_BIAODING);
					g_GlobalPara.ucFromPageNum = UI_SAMPBLOCK;
					LCD_FillRect(56,32,424,288,bkCol);
					return;
				}else{
					g_SysPara.iBatPercentBDLast = g_GlobalPara.iBatPercent;
					LCD_FillRect(90,50,360,220,bkCol);//清空弹窗
					BlockSample_Init();
					SaveSysPara();
				}
			}
		}
		//温度变化大标定
		if(fabs(g_GlobalPara.fTemperature-g_SysPara.fTemperatureBDLast) > 10.0f){
			if(PromptDialog(DIALOGBIAODINGTEMPER) == 1){
				SendUiNo(UI_BIAODING);
				g_GlobalPara.ucFromPageNum = UI_SAMPBLOCK;
				LCD_FillRect(56,32,424,288,bkCol);
				return;
			}else{
				g_SysPara.fTemperatureBDLast = g_GlobalPara.fTemperature;
				LCD_FillRect(90,50,360,220,bkCol);//清空弹窗
				BlockSample_Init();
				SaveSysPara();
			}
		}
		
		//返回
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_BACK)){
//			if(g_GlobalPara.iSaveDataCnt > 0){
				if(PromptDialog(DIALOGSAVE) == CONFIRM){
					//存储空间不足提示
					if(g_GlobalPara.iStorageFreePrcnt < 1){
						PromptDialog(DIALOGSTORNONE);
						
						g_GlobalPara.iSaveDataCnt = 0;
						SendUiNo(UI_MAIN);
						LCD_FillRect(56,32,424,288,bkCol);
						return;
					}else if(g_GlobalPara.iStorageFreePrcnt < 3){
						PromptDialog(DIALOGSTORLOW);
					}else{
						
					}
					
					SaveBlockFile();//存储构件
					ui_SetGJNameNext((char*)BlockParaTable[0].pList->listStr);//设置下一构建名
					SaveSysPara();
					
				}else{
					//不保存
				}
//			}
			SendUiNo(UI_MAIN);
			LCD_FillRect(56,32,424,288,bkCol);
			return;
		}
		
		//开始
		if((g_u8TouchResult == 2)||(g_u16KeyValBuf == KEY_FN)){
			
			if(BlockSampSta == BLOCKSAMPSTOP){
				
				if(SelNumHang != 0xff){
					BlockSampSta = BLOCKSAMPHANG;
					OneBtnSet[0].btnID = 2;
					OneBtnSet[0].btnQnt = 6;
					OneBtnSet[0].btnIcon = "束";
					OneBtnSet[0].btnName = "行结束";
					ui_BrushMenuOneBtn(&OneBtnSet[0],NORMALLIGHT);
					
					if(CarTrackBuf_Hang[SelNumHang] >= BlockCarDistMax){//已测完，清空
						ClearCarDist();
						BlockCarDist = 0;
						BlockCarDistLast = 0;
						DispBlockDist(g_iCarDist);
						CarTrackBuf_Hang[SelNumHang] = BlockCarDist;
					}
					
					BrushCarNumDisp(SelNumHang,SelNumLie);
					BrushCarTrackOne(SelNumHangLie);
				}
				if(SelNumLie != 0xff){
					BlockSampSta = BLOCKSAMPLIE;
					OneBtnSet[0].btnID = 2;
					OneBtnSet[0].btnQnt = 6;
					OneBtnSet[0].btnIcon = "束";
					OneBtnSet[0].btnName = "列结束";
					ui_BrushMenuOneBtn(&OneBtnSet[0],NORMALLIGHT);
					
					if(CarTrackBuf_Lie[SelNumLie] >= BlockCarDistMax){//已测完，清空
						ClearCarDist();
						BlockCarDist = 0;
						BlockCarDistLast = 0;
						DispBlockDist(g_iCarDist);
						CarTrackBuf_Lie[SelNumLie] = BlockCarDist;
					}
					
					BrushCarNumDisp(SelNumHang,SelNumLie);
					BrushCarTrackOne(SelNumHangLie);
				}
				LCD_SetColor(bkCol,GREEN);
				LCD_ShowChinese(Font24,418,40,0,"起");
				LCD_SetColor(bkCol,GRAY);
				LCD_ShowText(Font16,406,70,0,"采集中");
				
				ui_BrushMenuOneBtn(&Samp10Menu[1],LOWLIGHT);
				ui_BrushMenuOneBtn(&Samp10Menu[3],LOWLIGHT);
				ui_BrushMenuOneBtn(&Samp10Menu[4],LOWLIGHT);
				ui_BrushMenuOneBtn(&Samp10Menu[5],LOWLIGHT);
			}else{
				BlockSampSta = BLOCKSAMPSTOP;
				OneBtnSet[0].btnID = 2;
				OneBtnSet[0].btnQnt = 6;
				OneBtnSet[0].btnIcon = "始";
				OneBtnSet[0].btnName = "开始";
				ui_BrushMenuOneBtn(&OneBtnSet[0],NORMALLIGHT);
				
				LCD_SetColor(bkCol,RED);
				LCD_ShowChinese(Font24,418,40,0,"停");
				LCD_SetColor(bkCol,GRAY);
				LCD_ShowText(Font16,406,70,0,"未采集");
				
				ui_BrushMenuOneBtn(&Samp10Menu[1],NORMALLIGHT);
				ViewAbleFlag = IsViewAbleFun();
				g_GlobalPara.iSaveDataCnt = GetBlockDataCnt();
				if(ViewAbleFlag == 1){
					ui_BrushMenuOneBtn(&Samp10Menu[3],NORMALLIGHT);
				}else{
					ui_BrushMenuOneBtn(&Samp10Menu[3],LOWLIGHT);
				}
				if(g_GlobalPara.iSaveDataCnt > 0){
					ui_BrushMenuOneBtn(&Samp10Menu[4],NORMALLIGHT);
				}else{
					ui_BrushMenuOneBtn(&Samp10Menu[4],LOWLIGHT);
				}
//				ui_BrushMenuOneBtn(&Samp10Menu[3],NORMALLIGHT);
//				ui_BrushMenuOneBtn(&Samp10Menu[4],NORMALLIGHT);
				ui_BrushMenuOneBtn(&Samp10Menu[5],NORMALLIGHT);
				
				if(SelNumHang != 0xff){
					BrushCarNumDisp(SelNumHang,SelNumLie);
				}
				if(SelNumLie != 0xff){
					BrushCarNumDisp(SelNumHang,SelNumLie);
				}
			}
		}
		
		//行列切换
		if(BlockSampSta == BLOCKSAMPSTOP){
			//清除数据
//			if(g_u16KeyValBuf == KEY_FN){
				
//			}
			//参数
			if(g_u8TouchResult == 1){
				SendUiNo(UI_SAMPSET);
				g_GlobalPara.ucFromPageNum = UI_SAMPBLOCK;
				LCD_FillRect(56,32,424,288,bkCol);
				return;
//				LCD_FillRect(60,0,420,320,WHITE);
//				BlockParaSet_Fun(BlockParaTable);
//				ui_SampBlock_InitDisp();
			}
			//预览
			if(g_u8TouchResult == 3){
				ViewAbleFlag = IsViewAbleFun();
				if(ViewAbleFlag == 1){
					LCD_FillRect(56,32,424,288,bkCol);
					BlockPreview_Fun();
					BlockSample_Init();
					BrushCarTrackAll();
				}
			}
			//保存
			if(g_u8TouchResult == 4){
				g_GlobalPara.iSaveDataCnt = GetBlockDataCnt();
				if(g_GlobalPara.iSaveDataCnt > 0){
					//存储空间不足提示
					if(g_GlobalPara.iStorageFreePrcnt < 1){
						PromptDialog(DIALOGSTORNONE);
						g_GlobalPara.iSaveDataCnt = 0;
						
						SendUiNo(UI_SAMPBLOCK);
						g_GlobalPara.ucFromPageNum = UI_MAIN;//假装从主页进入
						LCD_FillRect(56,32,424,288,bkCol);
						return;
					}else if(g_GlobalPara.iStorageFreePrcnt < 3){
						PromptDialog(DIALOGSTORLOW);
					}else{
						
					}
					SaveBlockFile();
					ui_SetGJNameNext((char*)BlockParaTable[0].pList->listStr);//设置下一构建名
					SaveSysPara();
					g_GlobalPara.iSaveDataCnt = 0;
					
//					BlockSample_Init();
					
					SendUiNo(UI_SAMPBLOCK);
					g_GlobalPara.ucFromPageNum = UI_MAIN;//假装从主页进入
					LCD_FillRect(56,32,424,288,bkCol);
					return;
				}
			}
			//新建
			if(g_u8TouchResult == 5){
				g_GlobalPara.iSaveDataCnt = GetBlockDataCnt();
				if(g_GlobalPara.iSaveDataCnt > 0){
					if(PromptDialog(DIALOGSAVE) == CONFIRM){
						//存储空间不足提示
						if(g_GlobalPara.iStorageFreePrcnt < 1){
							PromptDialog(DIALOGSTORNONE);
							g_GlobalPara.iSaveDataCnt = 0;
						
							SendUiNo(UI_SAMPBLOCK);
							g_GlobalPara.ucFromPageNum = UI_MAIN;//假装从主页进入
							LCD_FillRect(56,32,424,288,bkCol);
							return;
						}else if(g_GlobalPara.iStorageFreePrcnt < 3){
							PromptDialog(DIALOGSTORLOW);
						}else{
							
						}
						
						SaveBlockFile();
						ui_SetGJNameNext((char*)BlockParaTable[0].pList->listStr);//设置下一构建名
						SaveSysPara();
						
						g_GlobalPara.iSaveDataCnt = 0;
						
						SendUiNo(UI_SAMPBLOCK);
						g_GlobalPara.ucFromPageNum = UI_MAIN;//假装从主页进入
						LCD_FillRect(56,32,424,288,bkCol);
						return;
					}else{
						ui_SetGJNameNext((char*)BlockParaTable[0].pList->listStr);//设置下一构建名
						SaveSysPara();
						SendUiNo(UI_SAMPBLOCK);
						g_GlobalPara.ucFromPageNum = UI_MAIN;//假装从主页进入
						LCD_FillRect(56,32,424,288,bkCol);
						return;
					}
				}else{
					ui_SetGJNameNext((char*)BlockParaTable[0].pList->listStr);//设置下一构建名
					SaveSysPara();
					SendUiNo(UI_SAMPBLOCK);
					g_GlobalPara.ucFromPageNum = UI_MAIN;//假装从主页进入
					LCD_FillRect(56,32,424,288,bkCol);
					return;
				}
			}
			//网格4x4
			if(g_SysPara.g_SamplePara.iBlockAreaNum == 0){
				//行选择
				if((g_u8TouchResult >= 6)&&(g_u8TouchResult <= 9)){
					if((g_u8TouchResult-6)!= SelNumHang){
						SelNumHang = g_u8TouchResult-6;
						SelNumHangLie = g_u8TouchResult-6;
//						ClearCarDist();
//						BlockCarDist = 0;
//						BlockCarDistLast = 0;
//						CarTrackBuf_Hang[SelNumHang] = BlockCarDist;
						BlockCarDist = CarTrackBuf_Hang[SelNumHang];
						BlockCarDistLast = BlockCarDist;
//						DispBlockDist(g_iCarDist);

						if(SelNumLie != 0xff){
							SelNumLie = 0xff;
						}
						BrushCarNumDisp(SelNumHang,SelNumLie);
						
						BrushCarTrackOne(SelNumHangLie);
					}
//					BrushCarTrackAll();
				}
				//列选择
				if((g_u8TouchResult >= 10)&&(g_u8TouchResult <= 13)){
					if((g_u8TouchResult-10)!= SelNumLie){
						SelNumLie = g_u8TouchResult-10;
						SelNumHangLie = g_u8TouchResult-10+8;
//						ClearCarDist();
//						BlockCarDist = 0;
//						BlockCarDistLast = 0;
//						CarTrackBuf_Lie[SelNumLie] = BlockCarDist;
						BlockCarDist = CarTrackBuf_Lie[SelNumLie];
						BlockCarDistLast = BlockCarDist;
//						DispBlockDist(g_iCarDist);

						if(SelNumHang != 0xff){
							SelNumHang = 0xff;
						}
						BrushCarNumDisp(SelNumHang,SelNumLie);
						
						BrushCarTrackOne(SelNumHangLie);
					}
//					BrushCarTrackAll();
				}
			}else{//网格8x8
				//行选择
				if((g_u8TouchResult >= 6)&&(g_u8TouchResult <= 13)){
					if((g_u8TouchResult-6)!= SelNumHang){
						SelNumHang = g_u8TouchResult-6;
						SelNumHangLie = g_u8TouchResult-6;
//						ClearCarDist();
//						BlockCarDist = 0;
//						BlockCarDistLast = 0;
//						CarTrackBuf_Hang[SelNumHang] = BlockCarDist;
						BlockCarDist = CarTrackBuf_Hang[SelNumHang];
						BlockCarDistLast = BlockCarDist;
//						DispBlockDist(g_iCarDist);

						if(SelNumLie != 0xff){
							SelNumLie = 0xff;
						}
						BrushCarNumDisp(SelNumHang,SelNumLie);
						
						BrushCarTrackOne(SelNumHangLie);
					}
//					BrushCarTrackAll();
				}
				//列选择
				if((g_u8TouchResult >= 14)&&(g_u8TouchResult <= 21)){
					if((g_u8TouchResult-10)!= SelNumLie){
						SelNumLie = g_u8TouchResult-14;
						SelNumHangLie = g_u8TouchResult-6;
//						ClearCarDist();
//						BlockCarDist = 0;
//						BlockCarDistLast = 0;
//						CarTrackBuf_Lie[SelNumLie] = BlockCarDist;
						BlockCarDist = CarTrackBuf_Lie[SelNumLie];
						BlockCarDistLast = BlockCarDist;
//						DispBlockDist(g_iCarDist);

						if(SelNumHang != 0xff){
							SelNumHang = 0xff;
						}
						BrushCarNumDisp(SelNumHang,SelNumLie);
						
						BrushCarTrackOne(SelNumHangLie);
					}
//					BrushCarTrackAll();
				}
			}
		}
		
		if(BlockSampSta != BLOCKSAMPSTOP){
			
			//小车动画
			n+=1;
			if(n%40 == 0){
				m+=1;
				if(m%2 == 0){
					LCD_DrawLine(416,136,424,128,GRAY);//清除
					LCD_DrawLine(435,136,443,128,GRAY);//清除
					
					LCD_DrawLine(416,128,424,136,BLACK);//显示
					LCD_DrawLine(435,128,443,136,BLACK);//显示
				}else{
					LCD_DrawLine(416,128,424,136,GRAY);//清除
					LCD_DrawLine(435,128,443,136,GRAY);//清除
					
					LCD_DrawLine(416,136,424,128,BLACK);//显示
					LCD_DrawLine(435,136,443,128,BLACK);//显示
				}
			}
		}
		Sleep_ms(5);
	}
}
//界面初始化
void BlockSample_Init(void)
{
	u8 i;
	u16 bkCol;
	u16 txCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
		txCol = WHITE;
	}else{
		bkCol = WHITE;
		txCol = BLACK;
	}
	
	//菜单显示
	ui_DispMenuBtn(Samp10Menu,0);
	ViewAbleFlag = IsViewAbleFun();
	g_GlobalPara.iSaveDataCnt = GetBlockDataCnt();
	if(ViewAbleFlag == 1){
		ui_BrushMenuOneBtn(&Samp10Menu[3],NORMALLIGHT);
	}else{
		ui_BrushMenuOneBtn(&Samp10Menu[3],LOWLIGHT);
	}
	if(g_GlobalPara.iSaveDataCnt > 0){
		ui_BrushMenuOneBtn(&Samp10Menu[4],NORMALLIGHT);
		ui_BrushMenuOneBtn(&Samp10Menu[5],NORMALLIGHT);
	}else{
		ui_BrushMenuOneBtn(&Samp10Menu[4],LOWLIGHT);
		ui_BrushMenuOneBtn(&Samp10Menu[5],LOWLIGHT);
	}
	
	//清除状态栏
	LCD_FillRect(56,0,424,320,bkCol);
	
	//分栏竖线
	LCD_FillRect(380,20,1,290,txCol);//竖
	
	LCD_SetColor(bkCol,BLACK);
	LCD_ShowChinese(Font16,70,8,0,"匚");
	LCD_ShowString(ASC16x8,88,2,0,"X");
	LCD_ShowString(ASC16x8,68,24,0,"Y");
	
	//地平线
	LCD_FillRect(395,140,70,2,txCol);//横
	for(i=0;i<7;i++){
		LCD_SetColor(bkCol,txCol);
		LCD_ShowChinese(Font8,396+i*10,140-2,1,"杠");
		LCD_FillRect(396+i*10,145,8,1,bkCol);//横
	}
	//小车
	LCD_SetColor(bkCol,GOLD);
	LCD_ShowChinese(Font32,414,108,0,"车");
	LCD_SetColor(bkCol,BLACK);
	LCD_ShowChinese(Font32,414,108,1,"牌");
	LCD_SetColor(bkCol,GRAY);
	LCD_ShowChinese(Font32,414,108,1,"轮");
	LCD_SetColor(bkCol,BLACK);
	LCD_ShowChinese(Font32,414,108,1,"圈");
	
	
	LCD_SetColor(bkCol,GRAY);
	LCD_ShowText(Font16,390,174,0,"构件:");
//	LCD_SetColor(WHITE,BLACK);
//	LCD_ShowString(ASC16x8,405,194,0,"HCGK0001");
	sprintf((char*)BlockParaTable[0].pList->listStr,"%s",g_SysPara.GouJianName);
	ui_DispGJName(ASC16x8,390,194,txCol,(char*)BlockParaTable[0].pList->listStr);
	
	LCD_SetColor(bkCol,GRAY);
	LCD_ShowText(Font16,390,220,0,"直径(mm)");
	LCD_SetColor(bkCol,txCol);
	LCD_ShowNums(ASC16x8,390,240,0,*BlockParaTable[1].ParaAddr);
	
	LCD_SetColor(bkCol,GRAY);
	LCD_ShowText(Font16,390,266,0,"位移(mm)");
	DispBlockDist(g_iCarDist);
	
	if(g_SysPara.g_SamplePara.iBlockAreaNum == 0){
		//显示网格4*4
		for(i=0;i<5;i++){
			LCD_FillRect(95,55+i*60,270,1,txCol);//横
			LCD_FillRect(110+i*60,40,1,270,txCol);//竖
		}
		for(i=0;i<4;i++){
			LCD_SetColor(bkCol,txCol);
			LCD_ShowNums(ASC12x6,70,79+i*60,0,i+1);
			LCD_ShowChinese(Font12,81,79+i*60,0,"→");
			
			LCD_SetColor(bkCol,txCol);
			LCD_ShowNums(ASC12x6,137+i*60,8,0,i+1);
			LCD_ShowChinese(Font12,134+i*60,23,0,"↓");
		}
	}else{
		//显示网格8*8
		for(i=0;i<9;i++){
			LCD_FillRect(95,55+i*30,270,1,txCol);//横
			LCD_FillRect(110+i*30,40,1,270,txCol);//竖
		}
		for(i=0;i<8;i++){
			LCD_SetColor(bkCol,txCol);
			LCD_ShowNums(ASC12x6,70,64+i*30,0,i+1);
			LCD_ShowChinese(Font12,81,64+i*30,0,"→");
			
			LCD_SetColor(bkCol,txCol);
			LCD_ShowNums(ASC12x6,122+i*30,8,0,i+1);
			LCD_ShowChinese(Font12,119+i*30,23,0,"↓");
		}
	}
	
	//采集状态
	LCD_SetColor(bkCol,RED);
	LCD_ShowChinese(Font24,418,40,0,"停");
	LCD_SetColor(bkCol,GRAY);
	LCD_ShowText(Font16,406,70,0,"未采集");
	
	
	
	if(g_SysPara.g_SamplePara.iBlockAreaNum == 0){
		//网格4x4按钮初始化
		g_u8ButtonNums = 14;
		struct BUTTON SampBtn[14] = {
			//sX,  sY,  eX,  eY,
			{  0,   0,  59,  49, 0,0},//0, 返回
			{  0,  50,  59, 103, 0,0},//1, 参数
			{  0, 104,  59, 157, 0,0},//2, 开始
			{  0, 158,  59, 211, 0,0},//3, 预览
			{  0, 212,  59, 265, 0,0},//4, 保存
			{  0, 266,  59, 319, 0,0},//5, 新建
			
			{ 65,  55, 105, 114, 0,0},//6, 行1
			{ 65, 115, 105, 164, 0,0},//7, 行2
			{ 65, 175, 105, 234, 0,0},//8, 行3
			{ 65, 235, 105, 294, 0,0},//9, 行4
			
			{110,   0, 169,  50, 0,0},//10, 列1
			{170,   0, 229,  50, 0,0},//11, 列2
			{230,   0, 289,  50, 0,0},//12, 列3
			{290,   0, 349,  50, 0,0},//13, 列4
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}
    else{
		//网格8x8按钮初始化
		g_u8ButtonNums = 22;
		struct BUTTON SampBtn[22] = {
			//sX,  sY,  eX,  eY,
			{  0,   0,  59,  49, 0,0},//0, 返回
			{  0,  50,  59, 103, 0,0},//1, 参数
			{  0, 104,  59, 157, 0,0},//2, 开始
			{  0, 158,  59, 211, 0,0},//3, 预览
			{  0, 212,  59, 265, 0,0},//4, 保存
			{  0, 266,  59, 319, 0,0},//5, 新建
			
			{ 65,  55, 105,  84, 0,0},//6, 行1
			{ 65,  85, 105, 114, 0,0},//7, 行2
			{ 65, 115, 105, 144, 0,0},//8, 行3
			{ 65, 145, 105, 174, 0,0},//9, 行4
			{ 65, 175, 105, 204, 0,0},//10, 行5
			{ 65, 205, 105, 234, 0,0},//11, 行6
			{ 65, 235, 105, 264, 0,0},//12, 行7
			{ 65, 265, 105, 294, 0,0},//13, 行8
			
			{110,   0, 139,  50, 0,0},//14, 列1
			{140,   0, 169,  50, 0,0},//15, 列2
			{170,   0, 199,  50, 0,0},//16, 列3
			{200,   0, 229,  50, 0,0},//17, 列4
			{230,   0, 259,  50, 0,0},//18, 列5
			{260,   0, 289,  50, 0,0},//19, 列6
			{290,   0, 319,  50, 0,0},//20, 列7
			{320,   0, 349,  50, 0,0},//21, 列8
		};
		memcpy(g_ButtonBuf,SampBtn,sizeof(SampBtn));
	}
	
	if(g_SysPara.g_SamplePara.iBlockAreaNum == 0){
		BlockGridQnt = 4;
		BlockCarDistMax = GRIDWIDTH*4;
		//显示网格4*4
		LCD_SetColor(bkCol,GOLD);
		LCD_ShowChinese(Font12,81,79+SelNumHang*60,0,"→");
		LCD_ShowNums(ASC12x6,70,79+SelNumHang*60,0,SelNumHang+1);
		
//		LCD_FillRect(95,55+i*60,270,1,GOLD);//横
//		LCD_FillRect(110+i*60,40,1,270,GOLD);//竖
	}
    else{
		BlockGridQnt = 8;
		BlockCarDistMax = BlockCarDistMax = GRIDWIDTH*8;
		//显示网格8*8
		LCD_SetColor(bkCol,GOLD);
		LCD_ShowChinese(Font12,81,64+SelNumHang*30,0,"→");
		LCD_ShowNums(ASC12x6,70,64+SelNumHang*30,0,SelNumHang+1);
	}
	
	BlockSampSta = BLOCKSAMPSTOP;
	if(BlockCarDist != 0){
		BrushCarTrackAll();//显示扫描轨迹
	}
	
//	ui_DispTime(1);
//	ui_DispBattery(1);//显示电量
	
    
    //检测参数赋值
    g_tWorkPara.cZhujinDiam = g_SysPara.g_SamplePara.iZhujinDiam;//主筋直径
    g_tWorkPara.cZhujinDistance = 0;	//主筋间距
    g_tWorkPara.cGujinDiam = 0;		    //箍筋直径
    g_tWorkPara.cGujinDistance = 0;	    //箍筋间距

    g_tWorkPara.cGangJinType = 0;		//钢筋类型
    g_tWorkPara.cLingChengType = 1;	    //大小量程

    g_tWorkPara.cJiLiang = 0;			//计量模式
    g_tWorkPara.cShejiHD = 30;			//设计厚度
    
	//清除误触摸
	ClearTouchSta();
}
//存储数据
void SaveBlockData(u8 ch,u8 dir,u8 dist,u8 houdu)
{
	unsigned short rebarCnt;
    
	rebarCnt = ResBuf[ch]->AllRebarCnt;
	
	ResBuf[ch]->BlockRanks[rebarCnt] = dir;
	ResBuf[ch]->RebarSite[rebarCnt] = dist;
	ResBuf[ch]->RebarCover[rebarCnt] = houdu;
	
	ResBuf[ch]->AllRebarCnt += 1;
}
//获取某行列已存数据个数和最大序号
void GetBlockDataDirInfo(u8 ch,u8 dir)
{
	u16 i;
	
	DirMaxIndex[ch] = 0;
	DirMaxDist[ch] = 0;
	DirSaveCnt[ch] = 0;
	for(i=0;i<ResBuf[ch]->AllRebarCnt;i++){
		if(ResBuf[ch]->BlockRanks[i] == dir){
			DirMaxIndex[ch] = i;
			DirMaxDist[ch] = ResBuf[ch]->RebarSite[i];
			DirSaveCnt[ch] += 1;
		}
	}
}
//小车后退删除数据
void DeleteBlockDataOne(u8 ch,u8 dir,u8 dist)
{
	u16 i;
	u16 allCnt = ResBuf[ch]->AllRebarCnt-1;
	if(dist < DirMaxDist[ch]){
		for(i=DirMaxIndex[ch];i<allCnt;i++){
			ResBuf[ch]->BlockRanks[i] = ResBuf[ch]->BlockRanks[i+1];
			ResBuf[ch]->RebarCover[i] = ResBuf[ch]->RebarCover[i+1];
			ResBuf[ch]->RebarSite[i] = ResBuf[ch]->RebarSite[i+1];
		}
		ResBuf[ch]->BlockRanks[allCnt] = 0;
		ResBuf[ch]->RebarCover[allCnt] = 0;
		ResBuf[ch]->RebarSite[allCnt]  = 0;
		ResBuf[ch]->AllRebarCnt -= 1;
	}
}
//删除某行列所有已存数据
void DeleteBlockDataOneDir(u8 ch,u8 dir)
{
	u16 i,j;
	u16 iStart = 0;
	u16 allCnt = ResBuf[ch]->AllRebarCnt;
	for(i=iStart;i<allCnt;i++){
		if(ResBuf[ch]->BlockRanks[i] == dir){
			allCnt -= 1;
			for(j=i;j<allCnt;j++){
				ResBuf[ch]->BlockRanks[j] = ResBuf[ch]->BlockRanks[j+1];
				ResBuf[ch]->RebarCover[j] = ResBuf[ch]->RebarCover[j+1];
				ResBuf[ch]->RebarSite[j] = ResBuf[ch]->RebarSite[j+1];
			}
			ResBuf[ch]->BlockRanks[allCnt] = 0;
			ResBuf[ch]->RebarCover[allCnt] = 0;
			ResBuf[ch]->RebarSite[allCnt]  = 0;
			ResBuf[ch]->AllRebarCnt -= 1;
			iStart -= 1;
		}
	}
}
//存储构件
void SaveBlockFile(void)
{
	FRESULT fres;
	int offset = 0;
	u8 fileDir[32];
	
	//文件数据头初始化
	FlieDataHead.iVersion = 3;
	sprintf(FlieDataHead.YiQiBianHao,"%s",g_SysPara.EquipmentNum);
	sprintf(FlieDataHead.GouJianName,"%s",BlockParaTable[0].pList->listStr);
	
	memcpy(&FlieDataHead.FileTimeStruct,&g_SysTime,sizeof(g_SysTime));
	
	FlieDataHead.ucDetectType = BLOCK;//检测类型
//	FlieDataHead.ucGouJianType = *BlockParaTable[6].ParaAddr;;//构件类型
	FlieDataHead.ucRebarType = *BlockParaTable[6].ParaAddr;;//钢筋类型
	
	FlieDataHead.ucZhuJinDiam = *BlockParaTable[1].ParaAddr;//主筋直径
	FlieDataHead.ucZhuJinJianju = *BlockParaTable[2].ParaAddr;//主筋间距
	FlieDataHead.ucGuJinDiam = *BlockParaTable[3].ParaAddr;//箍筋直径
	FlieDataHead.ucGuJinJianju = *BlockParaTable[4].ParaAddr;//箍筋间距
	FlieDataHead.ucHouDuDesign = *BlockParaTable[5].ParaAddr;;//设计厚度
	
	GetHouDuAvg();
	
	FlieDataHead.ucHouDuAvg = g_GlobalPara.ucHouduAvg;
	FlieDataHead.ucHouDuMin = g_GlobalPara.ucHouduMin;
	FlieDataHead.ucHouDuMax = g_GlobalPara.ucHouduMax;
	
	FlieDataHead.iCarSite = g_iCarDist;
	FlieDataHead.iSaveDotCntCh1 = ResBuf_CH1.AllRebarCnt;
	FlieDataHead.iSaveDotCntCh2 = ResBuf_CH2.AllRebarCnt;
	FlieDataHead.iSaveDotCntCh3 = ResBuf_CH3.AllRebarCnt;
	
	sprintf((char*)fileDir,"0:/GY91/%s.bin",FlieDataHead.GouJianName);
	
	fres = f_open(file,(const char*)fileDir,FA_WRITE|FA_READ|FA_CREATE_NEW);
	if(fres == FR_EXIST){
		if(PromptDialog(DIALOGGOUJIANEXIST)){
			fres = f_open(file,(const char*)fileDir,FA_WRITE|FA_READ|FA_CREATE_ALWAYS);
		}else{
			return;
		}
	}
	f_write(file,&FlieDataHead,sizeof(FlieDataHead),&bw);//写文件头
	offset = 128;//文件数据从128字节开始写入
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH1.RebarSite,ResBuf_CH1.AllRebarCnt*sizeof(unsigned short),&bw);//写数据ch1
	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH1.RebarCover,ResBuf_CH1.AllRebarCnt*sizeof(unsigned char),&bw);//写数据ch1
	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH1.BlockRanks,ResBuf_CH1.AllRebarCnt*sizeof(unsigned char),&bw);//写数据ch1
	offset += bw;
	f_lseek(file,offset);
//	f_write(file,"end1",4,&bw);//写数据校验ch1
//	offset += bw;
//	f_lseek(file,offset);
	f_write(file,&ResBuf_CH2.RebarSite,ResBuf_CH2.AllRebarCnt*sizeof(unsigned short),&bw);//写数据ch2
	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH2.RebarCover,ResBuf_CH2.AllRebarCnt*sizeof(unsigned char),&bw);//写数据ch2
	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH2.BlockRanks,ResBuf_CH2.AllRebarCnt*sizeof(unsigned char),&bw);//写数据ch2
	offset += bw;
	f_lseek(file,offset);
//	f_write(file,"end2",4,&bw);//写数据校验ch1
//	offset += bw;
//	f_lseek(file,offset);
	f_write(file,&ResBuf_CH3.RebarSite,ResBuf_CH3.AllRebarCnt*sizeof(unsigned short),&bw);//写数据ch3
	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH3.RebarCover,ResBuf_CH3.AllRebarCnt*sizeof(unsigned char),&bw);//写数据ch3
	offset += bw;
	f_lseek(file,offset);
	f_write(file,&ResBuf_CH3.BlockRanks,ResBuf_CH3.AllRebarCnt*sizeof(unsigned char),&bw);//写数据ch3
//	offset += bw;
//	f_lseek(file,offset);
//	f_write(file,"end3",4,&bw);//写数据校验ch1
	f_close(file);
	//好多bug改不完，好伤心，好难过，好悲催，奥利给不起来了
	//if i dont want to do，you'will die
	PromptTone_Once(120);//提示音
}
///////////////////////////////////////////////////////////////////////////////////////////////
//是否可以预览
u8 IsViewAbleFun(void)
{
	u8 i;
	u8 flag = 0;
	for(i=0;i<8;i++){
		if((CarTrackBuf_Hang[i] != 0)||(CarTrackBuf_Lie[i] != 0)){
			flag = 1;
			break;
		}
	}
	return flag;
}
//统计信息
void DataView_Fun(void)
{
	u8 i;
	char tempBuf[16];
	char timeBuf[20];
	
	u16 dvBkCol;
	u16 dvTxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		dvBkCol = BLACK;
		dvTxCol = WHITE;
		
		//状态栏
		LCD_FillRect(57,1,422,31,GRAY);//横
		LCD_SetColor(GRAY,WHITE);
		LCD_ShowChinese(Font16,238,9,0,"统计信息");
	}else{
		dvBkCol = WHITE;
		dvTxCol = BLACK;
		
		//状态栏
		LCD_FillRect(57,1,422,31,DGRAY);//横
		LCD_SetColor(DGRAY,WHITE);
		LCD_ShowChinese(Font16,238,9,0,"统计信息");
	}
	
	for(i=0;i<7;i++){
		LCD_FillRect(70,70+i*30,394,1,GRAY);//横
	}
	
//	LCD_SetColor(WHITE,BLACK);
//	LCD_ShowText(Font20,320,50,0,BlockParaTable[6].pList[*BlockParaTable[6].ParaAddr].listStr);
	LCD_SetColor(dvBkCol,dvTxCol);
	LCD_ShowText(Font16,80,47,0,"构件编号:");
	ui_DispGJName(ASC16x8,180,47,dvTxCol,(char*)BlockParaTable[0].pList->listStr);
	
	sprintf(timeBuf,"%02d-%02d-%02d %02d:%02d:%02d",FlieDataHead.FileTimeStruct.year-2000,FlieDataHead.FileTimeStruct.month,
		FlieDataHead.FileTimeStruct.day,FlieDataHead.FileTimeStruct.hour,FlieDataHead.FileTimeStruct.min,FlieDataHead.FileTimeStruct.sec);
	LCD_SetColor(dvBkCol,dvTxCol);
	LCD_ShowString(ASC16x8,80,77,0,timeBuf);
	
	LCD_ShowText(Font16,80,107,0,"X方向直径:");
	sprintf(tempBuf,"%3d mm  ",*BlockParaTable[1].ParaAddr);
	LCD_ShowString(ASC20x10,180,105,0,tempBuf);
	LCD_ShowText(Font16,290,107,0,"X方向间距:");
	sprintf(tempBuf,"%3d mm  ",*BlockParaTable[2].ParaAddr);
	LCD_ShowString(ASC20x10,390,105,0,tempBuf);
	LCD_ShowText(Font16,80,137,0,"Y方向直径:");
	sprintf(tempBuf,"%3d mm  ",*BlockParaTable[3].ParaAddr);
	LCD_ShowString(ASC20x10,180,135,0,tempBuf);
	LCD_ShowText(Font16,290,137,0,"Y方向间距:");
	sprintf(tempBuf,"%3d mm  ",*BlockParaTable[4].ParaAddr);
	LCD_ShowString(ASC20x10,390,135,0,tempBuf);
	LCD_ShowText(Font16,80,167,0,"设计厚度:");
	sprintf(tempBuf,"%3d mm  ",*BlockParaTable[5].ParaAddr);
	LCD_ShowString(ASC20x10,180,165,0,tempBuf);
	LCD_ShowText(Font16,290,167,0,"平均厚度:");
	sprintf(tempBuf,"%3d mm  ",g_GlobalPara.ucHouduAvg);
	LCD_ShowString(ASC20x10,390,165,0,tempBuf);
	LCD_ShowText(Font16,80,197,0,"最小厚度:");
	sprintf(tempBuf,"%3d mm  ",g_GlobalPara.ucHouduMin);
	LCD_ShowString(ASC20x10,180,195,0,tempBuf);
	LCD_ShowText(Font16,290,197,0,"最大厚度:");
	sprintf(tempBuf,"%3d mm  ",g_GlobalPara.ucHouduMax);
	LCD_ShowString(ASC20x10,390,195,0,tempBuf);
	LCD_ShowText(Font16,80,227,0,"已存测点:");
	sprintf(tempBuf,"%3d  ",g_GlobalPara.iSaveDataCnt);
	LCD_ShowString(ASC20x10,180,225,0,tempBuf);
	
	//页面左侧菜单表
	MENU_TABLE_STRUCT PreviewMenu[1] = {
		{0,1,"徊",NULL}
	};
	//菜单显示
	ui_DispMenuBtn(PreviewMenu,0);
	//按钮初始化
	g_u8ButtonNums = 1;
	struct BUTTON vieBtn[1] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0} //0, 返回
	};
	memcpy(g_ButtonBuf,vieBtn,sizeof(vieBtn));
	
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
//		ui_OneButtonReactionXY(0,0,0,47,33,GOLD,ORANGE);//返回
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		//切换页面
		if(g_u16KeyValBuf == KEY_FN){
//			return;
		}
		
		//返回
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_BACK)){
			LCD_FillRect(56,32,424,288,dvBkCol);
			return;
		}
	}
}
//预览
void BlockPreview_Init(void)
{
	u8 i;
	u16 pBkCol;
	u16 pTxCol;
	u16 pBarCol;
	u16 pValCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		pBkCol = BLACK;
		pTxCol = WHITE;
		pBarCol = GRAY;
		pValCol = GOLD;
	}else{
		pBkCol = WHITE;
		pTxCol = BLACK;
		pBarCol = DGRAY;
		pValCol = BLACK;
	}
	
	
	//状态栏
	LCD_FillRect(57,1,422,31,pBarCol);
	//构件名
	ui_DispGJName(ASC16x8,70,8,WHITE,(char*)BlockParaTable[0].pList->listStr);
	
	//钢筋类型
	LCD_SetColor(pBarCol,WHITE);
	LCD_ShowChinese(Font16,180,8,0,BlockParaTable[6].pList[g_SysPara.g_SamplePara.iBlockRebarType].listStr);
	
	LCD_SetColor(pBarCol,WHITE);
	LCD_ShowChinese(Font20,220,6,0,"φ");
	LCD_ShowString(ASC20x10,264,6,0,"x");
	LCD_ShowNnNums(ASC20x10,244,6,0,2,*BlockParaTable[1].ParaAddr);//g_SysPara.g_SamplePara.iBlockDiamX
	LCD_ShowNnNums(ASC20x10,274,6,0,2,*BlockParaTable[3].ParaAddr);//g_SysPara.g_SamplePara.iBlockDiamY
	
	LCD_SetColor(pBkCol,pTxCol);
	LCD_ShowChinese(Font16,70,40,0,"匚");
	LCD_ShowString(ASC16x8,88,34,0,"X");
	LCD_ShowString(ASC16x8,68,56,0,"Y");
	
	//网格4x4
	if(g_SysPara.g_SamplePara.iBlockAreaNum == 0){
		LCD_FillRect(135,60,240,240,LLGRAY);//区域
		LCD_DrawRect(135,60,241,241,GRAY);//框
		LCD_SetColor(pBkCol,pTxCol);
		for(i=0;i<5;i++){
			if(i==0){
				LCD_ShowNnNums(ASC12x6,135+i*60-15,40,0,3,i*120);
			}else{
				LCD_ShowNnNums(ASC12x6,135+i*60-9,40,0,3,i*120);
			}
			LCD_ShowNnNums(ASC12x6,108,60+i*60-6,0,3,i*120);
			if((i==0)||(i==4))continue;
			LCD_FillRect(136,60+i*60,239,1,LGRAY);//横
			LCD_FillRect(135+i*60,61,1,239,LGRAY);//竖
		}
	}else{//网格8x8
		LCD_FillRect(135,60,240,240,LLGRAY);//区域
		LCD_DrawRect(135,60,241,241,GRAY);//框
		LCD_SetColor(pBkCol,pTxCol);
		for(i=0;i<9;i++){
			if(i==0){
				LCD_ShowNnNums(ASC12x6,135+i*30-15,40,0,3,i*120);
			}else{
				LCD_ShowNnNums(ASC12x6,135+i*30-9,40,0,3,i*120);
			}
			LCD_ShowNnNums(ASC12x6,108,60+i*30-6,0,3,i*120);
			if((i==0)||(i==8))continue;
			LCD_FillRect(136,60+i*30,239,1,LGRAY);//横
			LCD_FillRect(135+i*30,61,1,239,LGRAY);//竖
		}
	}
	
	LCD_SetColor(pBkCol,GRAY);
	LCD_ShowText(Font16,405,160,0,"位置(mm)");
	LCD_SetColor(pBkCol,pValCol);
	LCD_ShowString(ASC16x8,405,180,0,"X:");
	LCD_ShowString(ASC16x8,405,200,0,"Y:");
	
	LCD_SetColor(pBkCol,GRAY);
	LCD_ShowText(Font16,405,230,0,"厚度(mm)");
	LCD_SetColor(pBkCol,pValCol);
	LCD_ShowString(ASC24x12,405,260,0,"-- ");
//	LCD_ShowString(ASC16x8,405,250,0,"X:24");
//	LCD_ShowString(ASC16x8,405,280,0,"Y:36");
	
	//页面左侧菜单表
	MENU_TABLE_STRUCT PreviewMenu[2] = {
		{0,2,"徊",NULL},
		{1,2,"统","数据"}
	};
	//菜单显示
	ui_DispMenuBtn(PreviewMenu,0);
	//按钮初始化
	g_u8ButtonNums = 3;
	struct BUTTON viewBtn[3] = {
		//sX,  sY,  eX,  eY,
		{  0,   0,  59,  49, 0,0},//0, 返回
		{  0,  50,  59, 103, 0,0},//1, 数据
		{135,  60, 374, 300, 0,0} //2, 预览区域
	};
	memcpy(g_ButtonBuf,viewBtn,sizeof(viewBtn));
	
	BrushBlockRebar();
}
//预览
void BlockPreview_Fun(void)
{
	u16 *readAddr = NULL;
	u16 touchX,touchY,width,height;
//	u16 siteX,siteY;
	static u16 touchX_Last,touchY_Last;
//	int readLen;
	
	u16 prBkCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		prBkCol = BLACK;
	}else{
		prBkCol = WHITE;
	}
	
	width=16;
	height=16;
//	readLen = width*height;
	
	BlockPreview_Init();
	while(1){
		ui_KeyTask();
		ui_ButtonTask(g_u8ButtonNums);
		
		if(g_u16KeyValBuf == KEY_OK){
			
		}
		//切换页面
		if(g_u16KeyValBuf == KEY_FN){
//			return;
		}
		
		//返回
		if((g_u8TouchResult == 0)||(g_u16KeyValBuf == KEY_BACK)){
			LCD_FillRect(56,0,424,320,prBkCol);
			myfree(SRAMIN,readAddr);
			readAddr = NULL;
			return;
		}
		//数据
		if(g_u8TouchResult == 1){
			myfree(SRAMIN,readAddr);
			readAddr = NULL;
			LCD_FillRect(56,32,424,288,prBkCol);
			DataView_Fun();
			BlockPreview_Init();
		}
		//点击选择
		if(g_u8TouchResult == 2){
			if(readAddr != NULL){
				LCD_ShowPicture(touchX_Last,touchY_Last,width,height,readAddr);
				myfree(SRAMIN,readAddr);
				readAddr = NULL;
			}
			touchX = TPData.TpX[0]-8;
			touchY = TPData.TpY[0]-8;
			
			readAddr = LCD_ReadRect(touchX,touchY,width,height);
			if(readAddr != NULL){
				LCD_FillRect(touchX,touchY,width,height,GOLD);
				LCD_DrawRect(touchX,touchY,width,height,GRAY);
			}
			
			DispClickRebarInfo(TPData.TpX[0],TPData.TpY[0]);
			touchX_Last = touchX;
			touchY_Last = touchY;
		}
	}
}
//刷新小车序号和图标
void BrushCarNumDisp(u8 hangNum,u8 lieNum)
{
	u8 i;
	u16 cBkCol;
	u16 cTxCol;
	u16 hlCol = GOLD;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		cBkCol = BLACK;
		cTxCol = WHITE;
	}else{
		cBkCol = WHITE;
		cTxCol = BLACK;
	}
	//网格4x4
	if(g_SysPara.g_SamplePara.iBlockAreaNum == 0){
		if(SelNumHang != 0xff){
			for(i=0;i<4;i++){
				//行
				if(i == SelNumHang){
					if(BlockSampSta != BLOCKSAMPSTOP){
						LCD_SetColor(cBkCol,hlCol);
						LCD_ShowNums(ASC12x6,70,79+i*60,0,i+1);
						LCD_ShowChinese(Font12,81,79+i*60,0,"》");
					}else{
						LCD_SetColor(cBkCol,hlCol);
						LCD_ShowNums(ASC12x6,70,79+i*60,0,i+1);
						LCD_ShowChinese(Font12,81,79+i*60,0,"→");
					}
				}else{
					LCD_SetColor(cBkCol,cTxCol);
					LCD_ShowNums(ASC12x6,70,79+i*60,0,i+1);
					LCD_ShowChinese(Font12,81,79+i*60,0,"→");
				}
				//列
				LCD_SetColor(cBkCol,cTxCol);
				LCD_ShowNums(ASC12x6,137+i*60,8,0,i+1);
				LCD_ShowChinese(Font12,134+i*60,23,0,"↓");
			}
		}
		if(SelNumLie != 0xff){
			for(i=0;i<4;i++){
				//行
				if(i == SelNumLie){
					if(BlockSampSta != BLOCKSAMPSTOP){
						LCD_SetColor(cBkCol,hlCol);
						LCD_ShowNums(ASC12x6,137+i*60,8,0,i+1);
						LCD_ShowChinese(Font12,134+i*60,23,0,"《");
					}else{
						LCD_SetColor(cBkCol,hlCol);
						LCD_ShowNums(ASC12x6,137+i*60,8,0,i+1);
						LCD_ShowChinese(Font12,134+i*60,23,0,"↓");
					}
					
				}else{
					LCD_SetColor(cBkCol,cTxCol);
					LCD_ShowNums(ASC12x6,137+i*60,8,0,i+1);
					LCD_ShowChinese(Font12,134+i*60,23,0,"↓");
				}
				//行
				LCD_SetColor(cBkCol,cTxCol);
				LCD_ShowNums(ASC12x6,70,79+i*60,0,i+1);
				LCD_ShowChinese(Font12,81,79+i*60,0,"→");
			}
		}
	}else{//网格8x8
		if(SelNumHang != 0xff){
			for(i=0;i<8;i++){
				//行
				if(i == SelNumHang){
					if(BlockSampSta != BLOCKSAMPSTOP){
						LCD_SetColor(cBkCol,hlCol);
						LCD_ShowNums(ASC12x6,70,64+i*30,0,i+1);
						LCD_ShowChinese(Font12,81,64+i*30,0,"》");
					}else{
						LCD_SetColor(cBkCol,hlCol);
						LCD_ShowNums(ASC12x6,70,64+i*30,0,i+1);
						LCD_ShowChinese(Font12,81,64+i*30,0,"→");
					}
				}else{
					LCD_SetColor(cBkCol,cTxCol);
					LCD_ShowNums(ASC12x6,70,64+i*30,0,i+1);
					LCD_ShowChinese(Font12,81,64+i*30,0,"→");
				}
				
				//列
				LCD_SetColor(cBkCol,cTxCol);
				LCD_ShowNums(ASC12x6,122+i*30,8,0,i+1);
				LCD_ShowChinese(Font12,119+i*30,23,0,"↓");
			}
		}
		if(SelNumLie != 0xff){
			for(i=0;i<8;i++){
				//行
				if(i == SelNumLie){
					if(BlockSampSta != BLOCKSAMPSTOP){
						LCD_SetColor(cBkCol,hlCol);
						LCD_ShowNums(ASC12x6,122+i*30,8,0,i+1);
						LCD_ShowChinese(Font12,119+i*30,23,0,"《");
					}else{
						LCD_SetColor(cBkCol,hlCol);
						LCD_ShowNums(ASC12x6,122+i*30,8,0,i+1);
						LCD_ShowChinese(Font12,119+i*30,23,0,"↓");
					}
				}else{
					LCD_SetColor(cBkCol,cTxCol);
					LCD_ShowNums(ASC12x6,122+i*30,8,0,i+1);
					LCD_ShowChinese(Font12,119+i*30,23,0,"↓");
				}
				
				//行
				LCD_SetColor(cBkCol,cTxCol);
				LCD_ShowNums(ASC12x6,70,64+i*30,0,i+1);
				LCD_ShowChinese(Font12,81,64+i*30,0,"→");
			}
		}
	}
}

//显示小车轨迹
void DispCarTrackCur(u8 hlnum)
{
	u16 i;
	u8 trackQnt;
	u8 hang,lie,hangCur,lieCur;
	u16 xStart,yStart,xEnd,yEnd;
	u16 gridWidth;
	u16 xCur,yCur;
	u16 ranS,ranE;//一行纵向边缘坐标或列横向边缘坐标
	u16 xLast,yLast;
	u16 mmPerPix;
	
	u16 tbkCol;
	u16 hangCol;
	u16 lieCol;
	u16 hlCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		tbkCol = BLACK;
		hangCol = DGRAY;
		lieCol = GRAY;
		hlCol = LGRAY;
	}else{
		tbkCol = WHITE;
		hangCol = LGRAY;
		lieCol = GRAY;
		hlCol = DGRAY;
	}
	
	xStart = 110;
	xEnd = 350;
	yStart = 55;
	yEnd = 295;
	
//	hangTop = 56;
//	lieLeft = 136;
	
	//4x4
	if(g_SysPara.g_SamplePara.iBlockAreaNum == 0){
		trackQnt = 4;
		mmPerPix = mmPerPix4x4;
		gridWidth = 60;
	}else{//8x8
		trackQnt = 8;
		mmPerPix = mmPerPix8x8;
		gridWidth = 30;
	}
	if(hlnum < trackQnt){//行
		hang = hlnum;
		lie = 0xff;
	}else{//列
		lie = hlnum % trackQnt;
		hang = 0xff;
	}
	if((hang != 0xff)&&(hang < 8)){
		CarTrackBuf_Hang[hang] = BlockCarDist;
		xCur = BlockCarDist/mmPerPix + xStart;
		xLast = BlockCarDistLast/mmPerPix + xStart;
		ranS = yStart+hang*gridWidth;
		ranE = ranS+gridWidth;
		if(xCur > xEnd)xCur = xEnd;
		if(xCur < xStart)xCur = xStart;
		if(xLast > xEnd)xLast = xEnd;
		if(xLast < xStart)xLast = xStart;
		if(xCur != xLast){
			if(xCur > xLast){
				for(i=xCur;i>xLast;i--){
					lieCur = (i-xStart)/gridWidth;
					yCur = CarTrackBuf_Lie[lieCur]/mmPerPix + yStart;
					if((i-xStart)%gridWidth != 0){
						if(yCur<=ranS){//无重叠
							LCD_FillRect(i,ranS+1,1,gridWidth-1,hangCol);
						}else if(yCur<ranE){//部分重叠
							LCD_FillRect(i,ranS+1,1,yCur-ranS,hlCol);
							LCD_FillRect(i,yCur+1,1,ranE-yCur-1,hangCol);
						}else{//全部重叠
							LCD_FillRect(i,ranS+1,1,gridWidth-1,hlCol);
						}
					}
				}
			}else{
				for(i=xLast;i>xCur;i--){
					lieCur = (i-xStart)/gridWidth;
					yCur = CarTrackBuf_Lie[lieCur]/mmPerPix + yStart;
					if((i-xStart)%gridWidth != 0){
						if(yCur<=ranS){//无重叠
							LCD_FillRect(i,ranS+1,1,gridWidth-1,tbkCol);
						}else if(yCur<ranE){//部分重叠
							LCD_FillRect(i,ranS+1,1,yCur-ranS,lieCol);
							LCD_FillRect(i,yCur+1,1,ranE-yCur-1,tbkCol);
						}else{//全部重叠
							LCD_FillRect(i,ranS+1,1,gridWidth-1,lieCol);
						}
					}
				}
			}
			BlockCarDistLast = BlockCarDist;
		}
	}
	if((lie != 0xff)&&(lie < 8)){
		CarTrackBuf_Lie[lie] = BlockCarDist;
		yCur = BlockCarDist/mmPerPix + yStart;
		yLast = BlockCarDistLast/mmPerPix + yStart;
		ranS = xStart+lie*gridWidth;
		ranE = ranS+gridWidth;
		if(yCur > yEnd)yCur = yEnd;
		if(yCur < yStart)yCur = yStart;
		if(yLast > yEnd)yLast = yEnd;
		if(yLast < yStart)yLast = yStart;
		if(yCur != yLast){
			if(yCur > yLast){
				for(i=yCur;i>yLast;i--){
					hangCur = (i-yStart)/gridWidth;
					xCur = CarTrackBuf_Hang[hangCur]/mmPerPix + xStart;
					if((i-yStart)%gridWidth != 0){
						if(xCur<=ranS){//无重叠
							LCD_FillRect(ranS+1,i,gridWidth-1,1,lieCol);
						}else if(xCur<ranE){//部分重叠
							LCD_FillRect(ranS+1,i,xCur-ranS,1,hlCol);
							LCD_FillRect(xCur+1,i,ranE-xCur-1,1,lieCol);
						}else{//全部重叠
							LCD_FillRect(ranS+1,i,gridWidth-1,1,hlCol);
						}
					}
				}
			}else{
				for(i=yLast;i>yCur;i--){
					hangCur = (i-yStart)/gridWidth;
					xCur = CarTrackBuf_Hang[hangCur]/mmPerPix + xStart;
					if((i-yStart)%gridWidth != 0){
						if(xCur<=ranS){//无重叠
							LCD_FillRect(ranS+1,i,gridWidth-1,1,tbkCol);
						}else if(xCur<ranE){//部分重叠
							LCD_FillRect(ranS+1,i,xCur-ranS,1,hangCol);
							LCD_FillRect(xCur+1,i,ranE-xCur-1,1,tbkCol);
						}else{//全部重叠
							LCD_FillRect(ranS+1,i,gridWidth-1,1,hangCol);
						}
					}
				}
			}
			BlockCarDistLast = BlockCarDist;
		}
	}
	
//	BlockCarDistLast = BlockCarDist;
}
//刷新小车轨迹单行或列
void BrushCarTrackOne(u8 hlnum)
{
	u16 i,j;
	u16 js,je;
	u8 trackQnt;
	u8 hang,lie;
	u16 gridWidth;
	u16 mmPerPix;
	u16 xCur,yCur;
	u16 ranS,ranE;//一行纵向边缘坐标或列横向边缘坐标
	u16 xStart,yStart;
//	u16 hangTop,lieLeft;
	
	u16 tbkCol;
	u16 hangCol;
	u16 lieCol;
	u16 hlCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		tbkCol = BLACK;
		hangCol = DGRAY;
		lieCol = GRAY;
		hlCol = LGRAY;
	}else{
		tbkCol = WHITE;
		hangCol = LGRAY;
		lieCol = GRAY;
		hlCol = DGRAY;
	}
	
	xStart = 110;
	yStart = 55;
//	xEnd = 375;
//	yEnd = 295;
	
//	hangTop = 56;
//	lieLeft = 136;
	
	//4x4
	if(g_SysPara.g_SamplePara.iBlockAreaNum == 0){
		trackQnt = 4;
		mmPerPix = mmPerPix4x4;
		gridWidth = 60;
	}else{//8x8
		trackQnt = 8;
		mmPerPix = mmPerPix8x8;
		gridWidth = 30;
	}
	if(hlnum < trackQnt){//行
		hang = hlnum;
		lie = 0xff;
	}else{//列
		lie = hlnum % trackQnt;
		hang = 0xff;
	}
	
	if(hang != 0xff){
		xCur = CarTrackBuf_Hang[hang]/mmPerPix + xStart;
		ranS = yStart+hang*gridWidth;
		ranE = ranS+gridWidth;
		for(i=0;i<trackQnt;i++){
			yCur = CarTrackBuf_Lie[i]/mmPerPix + yStart;
			js = xStart+i*gridWidth+1;
			je = js + gridWidth - 2;
			for(j=js;j<=je;j++){
				if(j<=xCur){
					if(yCur<=ranS){//无重叠
						LCD_FillRect(j,ranS+1,1,gridWidth-1,hangCol);
					}else if(yCur<ranE){//部分重叠
						LCD_FillRect(j,ranS+1,1,yCur-ranS,hlCol);
						LCD_FillRect(j,yCur+1,1,ranE-yCur-1,hangCol);
					}else{//全部重叠
						LCD_FillRect(j,ranS+1,1,gridWidth-1,hlCol);
					}
				}else{
					if(yCur<=ranS){//无重叠
						LCD_FillRect(j,ranS+1,1,gridWidth-1,tbkCol);
					}else if(yCur<ranE){//部分重叠
						LCD_FillRect(j,ranS+1,1,yCur-ranS,lieCol);
						LCD_FillRect(j,yCur+1,1,ranE-yCur-1,tbkCol);
					}else{//全部重叠
						LCD_FillRect(j,ranS+1,1,gridWidth-1,lieCol);
					}
				}
			}
		}
	}
	if(lie != 0xff){
		yCur = CarTrackBuf_Lie[lie]/mmPerPix + yStart;
		ranS = xStart+lie*gridWidth;
		ranE = ranS+gridWidth;
		for(i=0;i<trackQnt;i++){
			xCur = CarTrackBuf_Hang[i]/mmPerPix + xStart;
			js = yStart+i*gridWidth+1;
			je = js + gridWidth - 2;
			for(j=js;j<=je;j++){
				if(j<=yCur){
					if(xCur<=ranS){//无重叠
						LCD_FillRect(ranS+1,j,gridWidth-1,1,lieCol);
					}else if(xCur<ranE){//部分重叠
						LCD_FillRect(ranS+1,j,xCur-ranS,1,hlCol);
						LCD_FillRect(xCur+1,j,ranE-xCur-1,1,lieCol);
					}else{//全部重叠
						LCD_FillRect(ranS+1,j,gridWidth-1,1,hlCol);
					}
				}else{
					if(xCur<=ranS){//无重叠
						LCD_FillRect(ranS+1,j,gridWidth-1,1,tbkCol);
					}else if(xCur<ranE){//部分重叠
						LCD_FillRect(ranS+1,j,xCur-ranS,1,hangCol);
						LCD_FillRect(xCur+1,j,ranE-xCur-1,1,tbkCol);
					}else{//全部重叠
						LCD_FillRect(ranS+1,j,gridWidth-1,1,hangCol);
					}
				}
			}
		}
	}
}
//刷新小车轨迹全部
void BrushCarTrackAll(void)
{
	u16 i;
	u8 trackNum;
	
	//4x4
	if(g_SysPara.g_SamplePara.iBlockAreaNum == 0){
		trackNum = 4;
	}else{//8x8
		trackNum = 8;
	}
	for(i=0;i<trackNum;i++){
		BrushCarTrackOne(i);
	}
}

//显示钢筋图形
void BrushBlockRebar(void)
{
	u16 i,j,ii,jj;
	u16 rebarX,rebarY;
	u16 rebarCX,rebarCY;
	u16 rebarLen,rebarWid;
	u16 gridWidth;
	u16 mmPerPix;
	u16 xStart,yStart;//,xEnd,yEnd;
	u16 hangTop,lieLeft;
	u16 color,r,g,b;
	u16 houduMax = HOUDUMAX;
	u16 dataCnt;
//	u8 crossNum,crossCh;
	
	xStart = 135;
//	xEnd = 375;
	yStart = 60;
//	yEnd = 300;
	
	hangTop = 61;
	lieLeft = 136;
	
	//4x4
	if(g_SysPara.g_SamplePara.iBlockAreaNum == 0){
		mmPerPix = mmPerPix4x4;
		gridWidth = 60;
		rebarLen = 20;
		rebarWid = 9;
	}else{//8x8
		mmPerPix = mmPerPix8x8;
		gridWidth = 30;
		rebarLen = 10;
		rebarWid = 7;
	}
	
	for(i=0;i<3;i++){
		dataCnt = ResBuf[i]->AllRebarCnt;
		for(j=0;j<dataCnt;j++){
			if(ResBuf[i]->BlockRanks[j] < 8){//行，有数据
				if(ResBuf[i]->RebarSite[j] > 0){
					
					rebarY = ResBuf[i]->BlockRanks[j]*gridWidth + hangTop;
					rebarX = ResBuf[i]->RebarSite[j]/mmPerPix + xStart;
					
					r = (float)ResBuf[i]->RebarCover[j]/houduMax*31;
					g = r*2;
					b = r;
					color = (r<<11)+(g<<5)+b;
					if(i == 2){
						LCD_FillRect(rebarX-4,rebarY+rebarLen*i,rebarWid,rebarLen-1,color);
					}else{
						LCD_FillRect(rebarX-4,rebarY+rebarLen*i,rebarWid,rebarLen,color);
					}
				}
			}
		}
	}
	for(i=0;i<3;i++){
		dataCnt = ResBuf[i]->AllRebarCnt;
		for(j=0;j<dataCnt;j++){
			if((ResBuf[i]->BlockRanks[j] >= 8)&&(ResBuf[i]->BlockRanks[j] < 16)){//列，有数据
				if(ResBuf[i]->RebarSite[j] > 0){
					rebarX = (ResBuf[i]->BlockRanks[j]-8)*gridWidth + lieLeft;
					rebarY = ResBuf[i]->RebarSite[j]/mmPerPix + yStart;
					
					//先不管交叉直接画
					r = (float)ResBuf[i]->RebarCover[j]/houduMax*31;
					g = r*2;
					b = r;
					color = (r<<11)+(g<<5)+b;
					if(i == 2){
						LCD_FillRect(rebarX+rebarLen*i,rebarY-4,rebarLen-1,rebarWid,color);
					}else{
						LCD_FillRect(rebarX+rebarLen*i,rebarY-4,rebarLen,rebarWid,color);
					}
					//查找交叉钢筋
					for(ii=0;ii<3;ii++){
						for(jj=0;jj<dataCnt;jj++){
							if(ResBuf[ii]->BlockRanks[jj] < 8){//行，有数据
								if(ResBuf[ii]->RebarSite[jj] > 0){
									rebarCY = ResBuf[ii]->BlockRanks[jj]*gridWidth + hangTop;
									rebarCX = ResBuf[ii]->RebarSite[jj]/mmPerPix + xStart;
									r = (float)ResBuf[ii]->RebarCover[jj]/houduMax*31;
									g = r*2;
									b = r;
									color = (r<<11)+(g<<5)+b;
									//重叠情况重新画行钢筋
									
									if((rebarCX >= rebarX+rebarLen)&&(rebarCX+rebarWid <= rebarX)){//无交叉
										
									}else{//有交叉
										if(ResBuf[ii]->RebarCover[jj] < ResBuf[i]->RebarCover[j]){
											if(ii == 2){
												LCD_FillRect(rebarCX-4,rebarCY+rebarLen*ii,rebarWid,rebarLen-1,color);
											}else{
												LCD_FillRect(rebarCX-4,rebarCY+rebarLen*ii,rebarWid,rebarLen,color);
											}
										}
									}
									
//									if((rebarCX < rebarX+rebarLen)&&(rebarCX+9 > rebarCX+rebarLen)){
//										
//									}else if((rebarCX < rebarX+rebarLen)&&(rebarCX+9 > rebarCX+rebarLen)){
//										
//									}else if((rebarCX < rebarX+rebarLen)&&(rebarCX+9 > rebarCX+rebarLen)){
//										
//									}else{
//										
//									}
								}
							}
						}
					}
				}
			}
		}
	}
}
//扫描点中钢筋
void DispClickRebarInfo(u16 clickX,u16 clickY)
{
	//选中距离：X<8,y<10;
	u16 i,j;
	u8 resFlag = 0;
	u8 selX,selY;
	u16 juliX,juliY,juliMinX = 8,juliMinY = 8;
	u8 hanghao,liehao,hanghaoSub,liehaoSub;
	u16 rebarLen;
	u16 gridWidth;
	u16 mmPerPix;
	u16 xStart,yStart;//xEnd,yEnd;
	u16 siteX,siteY;
	short houdu;
	char tempBuf[8];
	u8 houduX,houduY;
	
	u16 ibkCol;
	u16 iTxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		ibkCol = BLACK;
		iTxCol = GOLD;
	}else{
		ibkCol = WHITE;
		iTxCol = BLACK;
	}
	
	xStart = 135;
//	xEnd = 375;
	yStart = 60;
//	yEnd = 300;
	
	//4x4
	if(g_SysPara.g_SamplePara.iBlockAreaNum == 0){
		mmPerPix = mmPerPix4x4;
		gridWidth = 60;
		rebarLen = 20;
	}else{//8x8
		mmPerPix = mmPerPix8x8;
		gridWidth = 30;
		rebarLen = 10;
	}
	
	hanghao = (clickY-yStart)/gridWidth;
	liehao = (clickX-xStart)/gridWidth+8;
	hanghaoSub = ((clickY-yStart)%gridWidth)/rebarLen;
	liehaoSub = ((clickX-xStart)%gridWidth)/rebarLen;
	for(i=0;i<3;i++){
		if(hanghaoSub == i){//通道1
			for(j=0;j<ResBuf[i]->AllRebarCnt;j++){
				if(ResBuf[i]->BlockRanks[j] == hanghao){
					juliX = abs(ResBuf[i]->RebarSite[j]/mmPerPix+xStart-clickX);
					if(juliX < juliMinX){
						juliMinX = juliX;
						selX = j;
						resFlag = 1;
					}
				}
			}
			break;
		}
	}
	for(i=0;i<3;i++){
		if(liehaoSub == i){
			for(j=0;j<ResBuf[i]->AllRebarCnt;j++){
				if(ResBuf[i]->BlockRanks[j] == liehao){
					juliY = abs(ResBuf[i]->RebarSite[j]/mmPerPix+yStart-clickY);
					if(juliY < juliMinY){
						juliMinY = juliY;
						selY = j;
						resFlag = 1;
					}
				}
			}
			break;
		}
	}
	
	if(resFlag == 1){
		if(juliMinX < juliMinY){
			houdu = ResBuf[hanghaoSub]->RebarCover[selX];
		}else if(juliMinX > juliMinY){
			houdu = ResBuf[liehaoSub]->RebarCover[selY];
		}else {
			houduX = ResBuf[hanghaoSub]->RebarCover[selX];
			houduY = ResBuf[liehaoSub]->RebarCover[selY];
			if(houduX < houduY){
				houdu = houduX;
			}else{
				houdu = houduY;
			}
		}
		LCD_SetColor(ibkCol,iTxCol);
		sprintf(tempBuf,"%-3d",houdu);
		LCD_ShowString(ASC24x12,405,260,0,tempBuf);
	}else{
		LCD_SetColor(ibkCol,iTxCol);
		LCD_ShowString(ASC24x12,405,260,0,"-- ");
	}
	siteX = (clickX-xStart)*mmPerPix;
	siteY = (clickY-yStart)*mmPerPix;
	LCD_SetColor(ibkCol,iTxCol);
	sprintf(tempBuf,"%-3d",siteX);
	LCD_ShowString(ASC16x8,421,180,0,tempBuf);
	sprintf(tempBuf,"%-3d",siteY);
	LCD_ShowString(ASC16x8,421,200,0,tempBuf);
}

//获取检测计数
int GetBlockDataCnt(void)
{
	return (ResBuf_CH1.AllRebarCnt+ResBuf_CH2.AllRebarCnt+ResBuf_CH3.AllRebarCnt);
}
//显示
void DispBlockDist(short dist)
{
	char dBuf[8];
	
	u16 dbkCol;
	u16 dTxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		dbkCol = BLACK;
		dTxCol = WHITE;
	}else{
		dbkCol = WHITE;
		dTxCol = BLACK;
	}
	
	sprintf(dBuf,"%-5d",dist);
	LCD_SetColor(dbkCol,dTxCol);
	LCD_ShowString(ASC16x8,390,286,0,dBuf);
}
