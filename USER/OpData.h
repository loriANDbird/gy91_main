#ifndef OP_DATA_H
#define OP_DATA_H

#include "mcu_io.h"
//#include "ui_Sample10.h"

#define FAR	            0
#define AIM             1
#define NEAR            2

#define HOUDUMAX	250

//检测结果缓存
typedef struct {
    unsigned short AllRebarCnt;        //总钢筋根数
    unsigned short AddRebatCnt;        //增加的钢筋根数
    unsigned short RebarSite[500];     //钢筋位置
    unsigned char RebarCover[500];     //保护层厚度
	unsigned char BlockRanks[500];     //区块行列号0-7:行，8-15:列
}COVER_BUFFER;

typedef struct {
	unsigned int iCh1;
    unsigned int iCh2;
    unsigned int iCh3;
    unsigned int iCh4;
    unsigned int iCh5;
    unsigned int iCh6;
}ADC_DATA_STR;

extern int g_iCh1AdcValOrig;
extern int g_iCh2AdcValOrig;
extern int g_iCh3AdcValOrig;
extern int g_iCh4AdcValOrig;
extern int g_iCh5AdcValOrig;
extern int g_iCh6AdcValOrig;

extern int g_iCh1AdcVal;
extern int g_iCh2AdcVal;
extern int g_iCh3AdcVal;
extern int g_iCh4AdcVal;
extern int g_iCh5AdcVal;
extern int g_iCh6AdcVal;

extern int g_iCh1AdcValMax;
extern int g_iCh2AdcValMax;
extern int g_iCh3AdcValMax;
extern int g_iCh4AdcValMax;
extern int g_iCh5AdcValMax;
extern int g_iCh6AdcValMax;

extern int g_iCh1AdcValMin;
extern int g_iCh2AdcValMin;
extern int g_iCh3AdcValMin;
extern int g_iCh4AdcValMin;
extern int g_iCh5AdcValMin;
extern int g_iCh6AdcValMin;

extern unsigned int g_iCarDist;
extern unsigned int g_iCarDistLast;
extern unsigned int g_iCarLastDist;
extern unsigned int g_iCarMaxDist;

extern unsigned int g_iCarDist;
extern short g_iCarDir;

extern COVER_BUFFER ResBuf_CH1;
extern COVER_BUFFER ResBuf_CH2;
extern COVER_BUFFER ResBuf_CH3;
extern COVER_BUFFER* ResBuf[3];

//typedef struct {
//	int AdcVal_Ch1;
//	int AdcVal_Ch2;
//	int AdcVal_Ch3;
//	int CarCnt;
//}SAMPLEVALADC;

//typedef struct {
//	int HouduVal_Ch1;
//	int HouduVal_Ch2;
//	int HouduVal_Ch3;
//	int CarSite;
//}SAMPLEVALHOUDU;

//******************外部调用**************************************
u8 Get_Adc_Data(void);
u8 Get_Adc_DataOrig(void);

void BrushZXX(int AimState);
void BrushMZJ(short iSite,char AimState);
char UpdateMZK(unsigned char ucChannel);
//void BrushKeDu(u8 page);
void DispHouduRes(short houdu);
void DispSaveDataCnt(short cnt);
void SaveDotData(u8 ch,u8 dist,u8 houdu);
void ClearCarDist(void);
void DispCarDist(short dist);
void BrushCar(short carSite);
void BrushChannelSel(u8 chNum,u8 mode);
void DispChannelLine(u8 chNum);
void DeleteChannelLine(u8 chNum);
void ClearCurvePage(void);
void ClearTiaoxingPage(u8 chNum);
void BrushPouMian(u8 carPage,short rebarSite,short rebarSiteLast,short houDu);
void BrushPouMianPage(short startSite,COVER_BUFFER* dataBuf);
void BrushBoXing(u8 carPage,short rebarSite,short rebarSiteLast,short houDu);
void BrushBoXingPage(short startSite,COVER_BUFFER* dataBuf);
void BrushTiaoXing(u8 chNum,short rebarSite,short houdu);
void BrushTiaoXingPage(u8 page,u8 chNum);
void BrushCurvePage(u8 page,u8 chNum,u8 dispMode);
void DeleteCurveOne(u8 chNum,short carSite,u8 dispMode);

void BrushHeatMap(u8 page,u8 gain);
#endif
