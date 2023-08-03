/******************************************************************************
创建者：张喜双
创建日期：2018.07.05
功能描述：
    1. 数据文件读写相关函数
    2. 
    
更新记录：
    1. 2018.07.05 初次创建
*******************************************************************************/
#ifndef OPFILE_H
#define OPFILE_H

#include "Config_ui.h"
#include "rx8010.h"
#include "ff.h"

#define MaxFileCount 2400

typedef struct FileListStruct{
	u8 NameStr[32];
	u8 fileEn;//1:有效，0:无效
	u8 fileType;//文件类型
	u8 RebarDiam;//主筋直径
	u16 SaveDotCnt;//存储个数
	u16 FileDate;//文件日期
	u16 FileTime;//文件时间
}FileListStruct;


//文件头
typedef struct FileDataHeadStruct{
	short int       iVersion;         	//2		版本号
	char            YiQiBianHao[12];    //14	仪器编号
	char 			GouJianName[32];	//46	构件名称
	unsigned char   ucDetectType;		//47	检测类型：0快速检测，1JGJ检测，2区块检测
	unsigned char	WorkCondition;		//48	检测工况：0常规，1高低筋，2避箍筋，3凹凸面，4反向修正
	unsigned char 	ucGouJianType;      //49	构件类型：0梁，1板
	unsigned char 	ucRebarType;        //50	钢筋类型：0螺纹钢，1圆钢
	
	RX8010SJTIME 	FileTimeStruct;		//58	时间
	
	unsigned char   ucZhuJinDiam;       //59	主筋直径，区块检测时为X方向直径
	unsigned char   ucZhuJinJianju;     //60	主筋间距，区块检测时为X方向间距
	unsigned char   ucGuJinDiam;        //61	箍筋直径，区块检测时为Y方向直径
	unsigned char   ucGuJinJianju;      //62    箍筋间距，区块检测时为Y方向间距
    
	unsigned char   ucHouDuDesign;      //63	设计厚度
	unsigned char   ucHouDuAvg;         //64	平均厚度
	unsigned char   ucHouDuMin;         //65	最小厚度
	unsigned char   ucHouDuMax;         //66	最大厚度
	short int		iCarSite;			//68	小车位移
	short int		iSaveDotCntCh1;	    //70	通道1存储测点个数，JGJ检测时为存储钢筋根数
	short int		iSaveDotCntCh2;	    //72	通道2存储测点个数
	short int		iSaveDotCntCh3;	    //74	通道3存储测点个数
	short int 		iHeGeDotCntCh1;	    //76	通道1合格测点个数，JGJ检测时为存储合格钢筋根数
	short int 		iHeGeDotCntCh2;	    //78	通道2合格测点个数
	short int 		iHeGeDotCntCh3;	    //80	通道3合格测点个数
	unsigned char	ucJGJNumber;		//81	JGJ钢筋根数设置 0-12
	
	unsigned char   ucBlockAreaNum;     //82	区块扫描面积 0:4x4,1:8x8
    
    int iCH1Start;
    int iCH2Start;
    int iCH3Start;
    int iCH4Start;
    int iCH5Start;
    int iCH6Start;
// 	unsigned char 	ucNull[18];         //100
}FileDataHeadStruct;

extern FileListStruct FileList[6];//文件名列表
extern FileDataHeadStruct FlieDataHead;//文件头


int Scan_FileCount(char* path);
int Scan_EmptyFiles(char* path);
int Scan_FileIndex(char* path);
void Scan_OnePageFiles(char* path,unsigned short page,unsigned short fileQnt);
int DeleteFile(char* path);

int DeleteFilesss(char* path);

FRESULT f_deldir(char *path);
#endif
