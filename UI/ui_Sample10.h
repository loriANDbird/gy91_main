#ifndef UI_SAMPLE10_H
#define UI_SAMPLE10_H

#include "stm32f10x.h"
#include "OpTable.h"
#include "AimLib.h"
#include "OpData.h"

//检测模式
#define QUICK	0
#define JGJ		1
#define BLOCK	2

//工况
#define	CHANGGUI	0
#define GAODIJIN	1
#define	BIGUJIN		2
#define	AOTUMIAN	3
#define	FANXIANGXZ	4

#define COVERDATACNTMAX		500
#define SOURCEDATACNTMAX	2000

//反向修正参数
struct FANXIANGXZSTR{
	int CeLiangZhi1;
	int BiaoZhunZhi1;
	int CeLiangZhi2;
	int BiaoZhunZhi2;
};

typedef struct {
	short MZJSite;	//瞄准镜位置
	short CarSite;	//小车位置
	u8 HouduRes;	//测量厚度
	u8 Channel;		//线圈通道
	u8 ChannelLast;	//线圈通道
	u8 PageCurr;	//数据页码
	u8 PageView;	//数据页码
	u8 PageLast;	//数据页码
	u8 PageQnt;		//数据页码
	
	u8 JGJPageQnt;//JGJ页数1-2
	u8 JGJPage;//页码1-2
	u8 JGJPageC;//页码1-2
	u8 JGJQnt;//JGJ钢筋根数
	u8 JGJNum;//JGJ钢筋编号1-12
	u8 JGJDot;//当前测点1-6
	u8 JGJDir;//扫描方向，0横向，1纵向
	u8 JGJSta;//测量状态，0未开始，1已开始
//	u8 SampleDispMode;//检测界面4种显示模式0-3
//	u8 AnalyzeDispMode;//分析界面4种显示模式0-3
	u8 ucGain;				//增益 0-7
	u8 ucHeat;				//热力 0-7
	u8 WorkCondition;//工况设置0-4
	u8 AoTuMianSta;//0:凹面,1:凸面
	int AoTuMianDiam;//构件直径
	
	struct FANXIANGXZSTR FanXiangXZPara;
}DETECTPARA;

extern SOURCE_DATA SourceData;
extern COVER_BUFFER ResBuf_CH1;
extern COVER_BUFFER ResBuf_CH2;
extern COVER_BUFFER ResBuf_CH3;
extern COVER_BUFFER* ResBuf[3];

extern DETECTPARA g_DetectPara;

extern u8 JGJData[12][9];//数据存储

//**************外部调用函数*******************************************************
//厚度检测页面
void ui_Sample10_InitDisp(void);
void ui_Sample10_Run(void);
//显示参数设置列表
void SampParaOptions(SYS_PARA_TABLE_STRUCT* ParaTableArr,u8 pageCap,u8 pageNum);
//滚动条
void DispScrollBar(short pageQnt,short pageNum,short pageLast);

void GetHouDuAvg(void);
#endif
