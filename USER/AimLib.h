#ifndef AIMLIB_H
#define AIMLIB_H

//显示模式
#define	MIAO_POU	0
#define MIAO_BO		1
#define	TIAO_POU	2
#define	TIAO_BO		3

#define	RELI_POU	0
#define RELI_BO		1

#define SOURCEDATAMAXCNT 		2300

enum CHANNELNUM
{
    CH1, CH2, CH3
};

#ifdef TEST_SOURCEDATA
//原始数据缓存
typedef struct {
	int iCh1Sum[500];
    int iCh1Sub[500];
    int iCh2Sum[500];
    int iCh2Sub[500];
    int iCh3Sum[500];
    int iCh3Sub[500];
    
    int iCh1SumMark[500];
    int iCh1SumMark2[500];
    int iCh2SumMark[500];
    int iCh2SumMark2[500];
    int iCh3SumMark[500];
    int iCh3SumMark2[500];
   
////    int iCH1[500];
////    int iCH2[500];
////    int iCH3[500];
////    int iCH4[500];
////    int iCH5[500];
////    int iCH6[500];

    int iCh1DifMark[500];
    int iCh2DifMark[500];
    int iCh3DifMark[500];
    
    int iCh1SubMark[500];
    int iCh2SubMark[500];
    int iCh3SubMark[500];
    
    short iCh1DifWaveCnt[500];
    short iCh2DifWaveCnt[500];
    short iCh3DifWaveCnt[500];
}SOURCE_DATA;
#endif

//原始数据缓存
typedef struct {
    short iCH1[2300];
    short iCH2[2300];
    short iCH3[2300];
    short iCH4[2300];
    short iCH5[2300];
    short iCH6[2300];

//test    
//    int iCh1SubMark[500];
//    int iCh2SubMark[500];
//    int iCh3SubMark[500];
//    
//    short iCH1[1300];
//    short iCH2[1300];
//    short iCH3[1300];
//    short iCH4[1300];
//    short iCH5[1300];
//    short iCH6[1300];
//    
//    short iCh1[500];
//    short iCh2[500];
//    short iCh3[500];
//    short iCh4[500];
//    short iCh5[500];
//    short iCh6[500];
//end
    
    int iCH1Start;
    int iCH2Start;
    int iCH3Start;
    int iCH4Start;
    int iCH5Start;
    int iCH6Start;
    
    int iCH1Last;
    int iCH2Last;
    int iCH3Last;
    int iCH4Last;
    int iCH5Last;
    int iCH6Last;
    
    int iCH1Sum[15];
    int iCH2Sum[15];
    int iCH3Sum[15];
    int iCH1Sub[15];
    int iCH2Sub[15];
    int iCH3Sub[15];
    
    char cCH1SumCnt;
    char cCH2SumCnt;
    char cCH3SumCnt;
    char cCH1SubCnt;
    char cCH2SubCnt;
    char cCH3SubCnt;
    
//    int iCh1Sum[10];
//    int iCh1Sub[10];
//    int iCh2Sum[10];
//    int iCh2Sub[10];
//    int iCh3Sum[10];
//    int iCh3Sub[10];
}SOURCE_DATA;

//检测参数结构体
typedef struct {
	char cZhujinDiam;		//主筋直径
	char cZhujinDistance;	//主筋间距
	char cGujinDiam;		//箍筋直径
	char cGujinDistance;	//箍筋间距
	
	char cGangJinType;		//钢筋类型
	char cLingChengType;	//大小量程
	
	char cJiLiang;			//计量模式
    char cShejiHD;			//设计厚度
} WORKPARA;

typedef struct {
    unsigned char AllRebarCnt;          //总钢筋根数
    unsigned char AddRebatCnt;          //增加的钢筋根数
    unsigned short RebarSite[7];        //钢筋位置
    unsigned char RebarCover[7];        //保护层厚度
}ONECHREBAR_BUFFER;

typedef struct {
    char IsDisp;                        //是否存在需要显示的钢筋
    ONECHREBAR_BUFFER CH1;              //通道1
    ONECHREBAR_BUFFER CH2;              //通道2
    ONECHREBAR_BUFFER CH3;              //通道3
}DISPREBAR_BUFFER;

//估测结果结构体
typedef struct {
    char Diam;
    char Cover;
}ESTIMATEDIAM;

//外部调用参数
extern WORKPARA g_tWorkPara;
extern SOURCE_DATA SourceData;

//外部调用函数
void AimFuncRun_HD(char ucChannel, unsigned char DispMode);//厚度检测外部算法调用接口函数
unsigned char AimFuncRun_MZK(char ucChannel, unsigned char ucAimState, unsigned char DispMode);//厚度瞄准镜定位算法
unsigned char AimFuncRun_JGJ(unsigned char ucChannel, unsigned char ucAimState, unsigned char DispMode);//JGJ瞄准镜定位算法
DISPREBAR_BUFFER AimFuncRun_Block(void);//区块检测定位算法
ESTIMATEDIAM EstimateDiam(void);//估测功能函数
void InitAimPara(void);//初始化定位钢筋缓存
void DispRebarCnt(char Channel);//刷新钢筋根数

//测试用函数
int SaveSourceDataFile(void);
//int SaveTableDataFile(void);
#endif



