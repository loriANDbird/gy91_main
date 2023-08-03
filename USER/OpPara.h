#ifndef OPPARA_H
#define OPPARA_H

#include "Config_ui.h"
#include "OpFile.h"
#include "usb_type.h"

//采样参数
typedef struct{
	int iGoujianName;		//构件编号
	int iZhujinDiam;		//主筋直径
	int iZhujinJianju;		//主筋间距
	int iDesignHoudu;		//设计厚度
	int iRebarType;			//钢筋类型
	int iRebarNumJGJ;		//钢筋根数
	//高级设置
	int iGujinDiam;			//箍筋直径
	int iGujinJianju;		//箍筋间距
	int iSaveType;			//存储方式
	int iGoujianType;		//构件类型
	int iFugaiHoudu;		//覆盖厚度
	int iAomianDiam;		//凹面直径
	int iTumianDiam;		//凸面直径
	int iScreenWide;		//单屏宽度
	
	int iBlockDiamX;		//区块X直径
	int iBlockJianjuX;		//区块X间距
	int iBlockDiamY;		//区块Y直径
	int iBlockJianjuY;		//区块Y间距
	int iBlockDesignHoudu;	//区块设计厚度
	int iBlockRebarType;	//区块钢筋类型
	int iBlockAreaNum;		//区块扫描面积序号

}SAMPLE_PARA;

typedef struct  {
	unsigned short sID;				//ID编号
	unsigned short sSoftVer;		//软件版本号

	int iBlueToothSta;				//蓝牙开关 		0:关 1:开
	int iTouchSoundSta;				//触摸音效开关	0:关 1:开
	int iBKLedLight;				//背光亮度		1:50% 2:75% 3:100%,默认2
	int iLanguage;					//语言选择		0:中文 1:英文
	int iLaserColSta;				//垂直激光		0:关 1:开
	int iLaserRowSta;				//水平激光		0:关 1:开
	int iTheme;						//主题			0:黑 1:白

	int iBkLedOffMinCnt;			//关闭背光分钟计数
	int iPowerOffMinCnt;			//关机时间分钟计数
	
	int	iSavedFileQuant;			//存储数据个数
//	int iSavedFileNumMax;			//已存最大文件名数值
	int iBatFileCnt;				//电量测试存储文件计数
    
    unsigned int uiCh1Zero;         //通道1零点
    unsigned int uiCh2Zero;         //通道2零点
    unsigned int uiCh3Zero;         //通道3零点
    unsigned int uiCh4Zero;         //通道4零点
    unsigned int uiCh5Zero;         //通道5零点
    unsigned int uiCh6Zero;         //通道6零点
	
	int iFact_LWSmall_Fi[16];		//生产螺纹钢小量程满度表
	int iFact_LWSmall_Zero[16];		//生产螺纹钢小量程零点表
	int iUser_LWSmall_Zero[16];		//用户螺纹钢小量程零点表
	int iFact_LWBig_Fi[16];			//生产螺纹钢大量程满度表
	int iFact_LWBig_Zero[16];		//生产螺纹钢大量程零点表
	int iUser_LWBig_Zero[16];		//用户螺纹钢大量程零点表
	
	int iFact_YGSmall_Fi[16];		//生产圆钢小量程满度表
	int iFact_YGSmall_Zero[16];		//生产圆钢小量程零点表
	int iUser_YGSmall_Zero[16];		//用户圆钢小量程零点表
	int iFact_YGBig_Fi[16];			//生产圆钢大量程满度表
	int iFact_YGBig_Zero[16];		//生产圆钢大量程零点表
	int iUser_YGBig_Zero[16];		//用户圆钢大量程零点表
	
	char GouJianName[32];			//构件名称
	char EquipmentNum[16];			//设备编号
	char BlueToothNum[16];			//蓝牙编号
	char HardVersion[16];			//硬件版本
	char SoftVersion[16];			//软件版本
	float fRangeMax;				//量程上限
	float fRangeMin;				//量程下限
	float fTemperatureBDLast;		//上次标定温度
	int iBatPercentBDLast;		//上次标定电量

	u8 SampleDispMode;//检测界面4种显示模式0-3
	u8 AnalyzeDispMode;//分析界面4种显示模式0-3
	
	SAMPLE_PARA g_SamplePara;		//检测参数
	
	u8 JGJScanDir;					//JGJ扫描方向 0：横向，1：纵向
    
    short int iBigFi_LW[16];		//大量程  0号为  总量程满度   螺纹钢
	short int iBigZero_LW[16];		//大量程  0号为  总量程零点   螺纹钢

}STR_SYS_PARA;

//全局变量
typedef struct {
	u8 iOpenType;			//0:按键开机,1:充电开机
	u8 iChargeSta;			//充电状态临时 0:未充电，1:充电中，2:充电完成
	u8 iChargeStaRes;		//充电状态 0:未充电，1:充电中，2:充电完成
	u8 iBatPercent;		//电量百分比
	u8 iBatPcntMin;		//电量百分比最小值
	u8 iBatPcntMax;		//电量百分比最大值
	u8 iBatPcntLowFlag;	//电量低标志
	float fBatVoltage;		//电池电压
	float fTemperature;		//温度
	
	short sDispScreenWide;	//一页屏幕显示距离，400或800mm
	
	u8 iStorageFreePrcnt;	//剩余容量百分百
	
	u8 ucDispModepage;		//显示模式页面 0：快速检测，1：分析
	u8 ucFromPageNum;		//母界面序号，用于参数设置和标定界面
	
	u8 ucHouduAvg;			//平均厚度
	u8 ucHouduMax;			//最大厚度
	u8 ucHouduMin;			//最小厚度
	
	int iSaveFileCnt;		//存储文件计数
	int iSaveDataCnt;		//保存数据计数
	
	int iCurFileNum;		//当前文件序号

}GLOBAL_PARA;

//测量数据存储结构
typedef struct BatDataTest{
	int iMinutCount;		//时间计数
	int iSaveCount;			//保存计数
}BatDataTest;


extern u8 debug;
extern char FileDirectory[16];
extern STR_SYS_PARA  g_SysPara;
extern GLOBAL_PARA	g_GlobalPara;
extern BatDataTest g_BatDataTest;

extern void ReadSysPara(void);
extern void SaveSysPara(void);
extern void InitSysPara(void);


#endif
