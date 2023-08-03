#ifndef AIMLIB_H
#define AIMLIB_H

//��ʾģʽ
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
//ԭʼ���ݻ���
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

//ԭʼ���ݻ���
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

//�������ṹ��
typedef struct {
	char cZhujinDiam;		//����ֱ��
	char cZhujinDistance;	//������
	char cGujinDiam;		//����ֱ��
	char cGujinDistance;	//������
	
	char cGangJinType;		//�ֽ�����
	char cLingChengType;	//��С����
	
	char cJiLiang;			//����ģʽ
    char cShejiHD;			//��ƺ��
} WORKPARA;

typedef struct {
    unsigned char AllRebarCnt;          //�ֽܸ����
    unsigned char AddRebatCnt;          //���ӵĸֽ����
    unsigned short RebarSite[7];        //�ֽ�λ��
    unsigned char RebarCover[7];        //��������
}ONECHREBAR_BUFFER;

typedef struct {
    char IsDisp;                        //�Ƿ������Ҫ��ʾ�ĸֽ�
    ONECHREBAR_BUFFER CH1;              //ͨ��1
    ONECHREBAR_BUFFER CH2;              //ͨ��2
    ONECHREBAR_BUFFER CH3;              //ͨ��3
}DISPREBAR_BUFFER;

//�������ṹ��
typedef struct {
    char Diam;
    char Cover;
}ESTIMATEDIAM;

//�ⲿ���ò���
extern WORKPARA g_tWorkPara;
extern SOURCE_DATA SourceData;

//�ⲿ���ú���
void AimFuncRun_HD(char ucChannel, unsigned char DispMode);//��ȼ���ⲿ�㷨���ýӿں���
unsigned char AimFuncRun_MZK(char ucChannel, unsigned char ucAimState, unsigned char DispMode);//�����׼����λ�㷨
unsigned char AimFuncRun_JGJ(unsigned char ucChannel, unsigned char ucAimState, unsigned char DispMode);//JGJ��׼����λ�㷨
DISPREBAR_BUFFER AimFuncRun_Block(void);//�����ⶨλ�㷨
ESTIMATEDIAM EstimateDiam(void);//���⹦�ܺ���
void InitAimPara(void);//��ʼ����λ�ֽ��
void DispRebarCnt(char Channel);//ˢ�¸ֽ����

//�����ú���
int SaveSourceDataFile(void);
//int SaveTableDataFile(void);
#endif



