/*******************************************************************************
ʣ�๤����
1.�ɼ�DIF_MIJI_DIAM_XX_LW ΢�����ݱ�����΢���ܼ�������жϣ�����Ϊ���Ϸɲɼ����ݱ����������֤΢��
2.��Ȳ����㷨���ܼ�����ֵ�������ࣨ�ֶ����ú��Զ�������������ֱ����������
3.���������㷨����͹�油��
4.���⹦������ɣ����²ɼ���ȱ�������ȱ�󣬻�����֤���ܡ�����bug
5.�����⹦������ɣ������㷨�Ż��ϴ󣬻�����֤���ܡ�����bug
*******************************************************************************/
#include "AimLib.h"
#include "math.h"
#include "OpData.h"
#include "OpPara.h"

#define ZJC_TEST_GY91

#define SITE_OFFSET   0

//////#define SITE_OFFSET   12

//int SampleDataCnt = 0;

enum SOURCETYPE//enumö��
{
    CH1SUM, CH2SUM, CH3SUM,
    CH1SUB, CH2SUB, CH3SUB
};

//ԭʼ����
SOURCE_DATA SourceData;

//unsigned int SumDataBuf_Mark_CH1[30] = {0};
//unsigned int SumDataBuf_Mark_CH2[30] = {0};
//unsigned int SumDataBuf_Mark_CH3[30] = {0};

char g_cStrBuf[100];//�ַ��������ͣ�[]ָ��������
int g_iSumAimState = 0;
int iSumMaxVal = 0;

WORKPARA g_tWorkPara;//WORKPARAΪ�������ṹ��

//////COVER_BUFFER ResBuf_CH1;//���ֵ����
//////COVER_BUFFER ResBuf_CH2;//���ֵ����
//////COVER_BUFFER ResBuf_CH3;//���ֵ����

//��ͨ���������ṹ��
typedef struct {
	int iCh1;
    int iCh2;
    int iCh3;
    int iCh4;
    int iCh5;
    int iCh6;//ich���ڴ洢ģ��ת��������
    int iSub1;
    int iSub2;
    int iSub3;//�洢��ֵ
    int iSum1;
    int iSum2;
    int iSum3;//�洢�ۼӽ��
}ADC_RES_STR;

//�ֽ����������
typedef struct {
	  unsigned int iReBarsCnt;//cnt��
    unsigned int iDistance;
    unsigned int iHD;
    
    unsigned int iTWORebarVal;
    unsigned int iTHREERebarVal;
    unsigned int iFOURRebarVal;
}MIJI_RET;

typedef struct {
    int SumDataBuf_Mark[5];//buf-����
    int SumSiteBuf_Mark[5];
    short SumBoFengSite[8];
    int SumBoFengVal[8];
    short SumBoGuSite[8];
    short SumBoGuVal[8];
    
    short LastSumRebarSite;
    short LastSumRebarHD;
    int LastSumRebarVal;
    
    int SubDataBuf_Mark[5];//sub�����ֵ
    short SubBoFengSite[8];
    int SubBoFengVal[8];
    short SubBoGuSite[8];
    int SubBoGuVal[8];
    short LastSubRebarSite[7];//��¼��ֵ��λ���ĸֽ�
    short LastSubRebarHD[7];
    
    int iLastSubRebarCnt;//���һ�β�ֵ������ĸֽ����
    int iLastSubMaxVal;
    int iLastSubMinVal;
    short iLastSubMaxSite;
    short iLastSubMinSite;
    
    short DifWaveDirCnt[8];//��¼����Mark���������½��Ĵ�����dif-΢��
    int DifDataBuf_Mark[5];
    short DifBoFengSite[8];
    int DifBoFengVal[8];
    short DifBoGuSite[8];
    int DifBoGuVal[8];
    short LastDifRebarSite[7];
    short LastDifRebarHD[7];
    int LastDifRebarVal[7];

    int LastDifMaxVal;
    int LastDifMinVal;
    short LastDifMaxSite;
    short LastDifMinSite;
    
    short SecDifWaveDirCnt[8];//��¼����Mark���������½��Ĵ���
    int SecDifDataBuf_Mark[5];
    short SecDifBoFengSite[8];
    int SecDifBoFengVal[8];
    short SecDifBoGuSite[8];
    int SecDifBoGuVal[8];
    short LastSecDifRebarSite[7];
    short LastSecDifRebarHD[7];
    int LastSecDifRebarVal[7];

    int LastSecDifMaxVal;
    int LastSecDifMinVal;
    short LasSecDifMaxSite;
    short LastSecDifMinSite;

    int MZKDataBuf_Mark[5];//MZK��׼��
    short MZKZeroSite[8];
    int MZKZeroVal[8];

    short LastMZKRebarSite[7];//��¼��׼��λ���ĸֽ�
    short LastMZKRebarHD[7];
    
    int iLastMZKZeroVal;
    short iLastMZKZeroSite;
    
    //����ʹ�õĺ��ֵ
    double EstSumCh2Diam16;
    double EstSubCh2Diam16;
    
    short EstimateSumCh1;
    short EstimateSumCh2;
    short EstimateSumCh3;
    
    short EstimateSubCh1;
    short EstimateSubCh2;
    short EstimateSubCh3;
}AIMREBAR_BUF_STR;

//�ֽλ�����ṹ��
typedef struct {
    unsigned char ucSubAimStep;
    unsigned char ucDifAimStep;
    unsigned char ucSecDifAimStep;
    unsigned char ucMZKAimStep;

    int iSubMaxVal;
    int iSubMinVal;
    short iSubMaxSite;
    short iSubMinSite;
    short SubRebarSite;//���ڳ��μ���ֽ����
    short SubRebarHD;

    int iDifMaxVal;
    int iDifMinVal;
    int iDifMaxSite;
    int iDifMinSite;
    
    int iMZKZeroVal;
    int iMZKZeroSite;
    short MZKRebarSite;
    short MZKRebarHD;
}AIMREBAR_PARA_STR;

typedef struct {
    unsigned int TwoRebars[15];         //�����ֽ����
    unsigned int ThreeRebars[15];       //�����ֽ����
    unsigned int FourRebars[15];        //�ĸ��ֽ����
}MIJI_TAB;

typedef struct {
    short RebarSiteBuf[7];              //�����ֽ����
    short RebarCoverBuf[7];             //�����ֽ����
}MIJI_RES;



//ADC��ȥ����Ľ��ֵ
ADC_RES_STR AdcDataBuf;//AdcDataBuf������������ǰ��Ľṹ��

AIMREBAR_PARA_STR AimRebarPara_CH1;//�ֽλ����
AIMREBAR_PARA_STR AimRebarPara_CH2;//�ֽλ����
AIMREBAR_PARA_STR AimRebarPara_CH3;//�ֽλ����

AIMREBAR_BUF_STR AimRebarBuf_CH1;//�ֽλ����
AIMREBAR_BUF_STR AimRebarBuf_CH2;//�ֽλ����
AIMREBAR_BUF_STR AimRebarBuf_CH3;//�ֽλ����

//*********************************ADֵת��ȱ�*********************************//
char ListZhiJing[16] = {//ֱ���б�
    6, 8,10,12,14,16,18,20,22,25,28,32,40,50
};
//С���� ��ȱ� �����ں�ȱ궨С����TAB 
const unsigned short int COVER_VAL[272] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
    34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
    64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,
    94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,
    118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,
    140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
    
    160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,196,198,200//2mm���
};

//�����Ƹ�ֱ��16Ϊ��׼���þ�ֵ������ַ��ɼ�����
//******************��ֵ��******************//
//���Ƹ�
const unsigned int BIG_LWG_SUM_CH1[295] = {
    //20230310����
    190589,//0
    160720,149632,141376,129995,117984,109845,100808,94711,88249,81435,//1-10
    75774,69432,64510,60533,54656,51355,47926,44809,41835,38838,//11-20
    36101,33269,31268,29533,27365,25808,24005,22610,21335,20695,//21-30
    19484,18271,17178,16165,15265,14311,13529,12813,12018,11350,//31-40
    10812,10238,9717,9155,8601,8216,7797,7410,7015,6761,//41-50
    6394,6096,5787,5527,5209,4966,4720,4504,4294,4144,//51-60
    3957,3770,3614,3459,3240,3093,2954,2844,2699,2610,//61-70
    2526,2425,2336,2224,2119,2031,1942,1865,1803,1742,//71-80
    1675,1614,1551,1487,1421,1366,1313,1266,1223,1174,//81-90
    1134,1098,1058,1016,976,937,905,876,845,816,//91-100
    794,774,745,719,691,668,646,625,602,589,//101-110
    569,550,538,518,497,482,469,456,442,435,//111-120
    424,413,401,388,376,365,351,341,334,325,//121-130
    317,306,300,291,281,272,267,264,258,256,//131-140
    250,242,239,233,227,219,214,210,205,201,//141-150
    196,192,188,184,176,174,170,167,163,160,//151-160
    150,141,135,137,133,//162-170
    125,120,115,111,107,//172-180
    106,103,96,92,91,//182-190
    87,83,79,77,75,//192-200
    71,64,//205-210
    60,55,//215-220
    53,49,//225-230
};

const unsigned int BIG_LWG_SUM_CH2[295] = {
    //20230310����
    190589,//0
    161076,153298,147443,139118,129132,122005,113499,108287,102792,96439,//1-10
    90753,83786,78693,74207,68056,64270,59618,56353,52793,49540,//11-20
    46015,42689,39864,37899,35296,33147,30941,29035,27378,26416,//21-30
    24751,23297,21798,20460,19357,18121,17011,16147,15100,14212,//31-40
    13514,12739,12052,11330,10601,10157,9600,9084,8570,8255,//41-50
    7774,7396,6994,6684,6254,5956,5665,5369,5126,4934,//51-60
    4693,4456,4269,4071,3801,3621,3443,3297,3096,3032,//61-70
    2935,2810,2702,2572,2443,2338,2213,2123,2051,1983,//71-80
    1902,1832,1753,1677,1605,1536,1476,1418,1367,1311,//81-90
    1259,1217,1170,1123,1076,1032,991,957,925,891,//91-100
    866,842,810,777,748,723,685,660,648,633,//101-110
    614,595,577,558,536,515,496,485,470,459,//111-120
    449,436,426,413,399,387,368,358,350,340,//121-130
    325,316,312,300,292,283,279,273,266,263,//131-140
    257,251,247,240,233,226,222,214,211,206,//141-150
    202,198,195,189,183,180,173,170,165,163,//151-160
    151,144,139,136,133,//162-170
    126,120,116,111,110,//172-180
    106,103,96,92,89,//182-190
    87,83,81,80,77,//192-200
    69,64,//205-210
    60,58,//215-220
    51,46,//225-230
};

const unsigned int BIG_LWG_SUM_CH3[295] = {
    //20230310����
    187048,//0
    157534,146463,138082,127062,114014,105909,97193,91875,85480,79704,//1-10
    74090,67520,62729,58687,53672,50534,46598,43809,40788,38471,//11-20
    35514,33072,30848,29157,27249,25456,23841,22348,21076,20230,//21-30
    18896,17941,16809,15715,14950,14045,13242,12561,11772,11145,//31-40
    10591,9956,9436,8920,8382,8028,7596,7199,6827,6542,//41-50
    6188,5901,5612,5384,5069,4816,4599,4364,4183,4032,//51-60
    3842,3656,3511,3355,3141,3005,2870,2768,2616,2529,//61-70
    2447,2337,2260,2158,2052,1965,1871,1802,1744,1701,//71-80
    1635,1580,1512,1455,1389,1332,1282,1234,1184,1149,//81-90
    1101,1061,1024,986,945,908,874,842,815,782,//91-100
    765,744,715,686,662,642,618,593,584,560,//101-110
    547,527,516,497,482,454,443,434,423,414,//111-120
    405,392,379,367,358,350,337,327,322,310,//121-130
    300,295,293,280,276,260,254,251,243,241,//131-140
    240,237,232,222,219,214,206,200,195,191,//141-150
    185,182,179,175,173,170,167,160,157,155,//151-160
    148,139,135,131,126,//162-170
    121,109,108,101,99,//172-180
    98,94,92,87,84,//182-190
    83,77,78,75,73,//192-200
    68,61,//205-210
    54,53,//215-220
    47,46,//225-230
};

//******************��ֵ��******************//
//���Ƹ�
const unsigned int BIG_LWG_SUB_CH1[160] = {
    //20230310����
    193972,//0
    152608,143754,136722,121066,104435,89868,78020,73331,63507,58084,//1-10
    51412,44190,39311,35485,30571,28211,25266,22751,20821,18736,//11-20
    16665,14941,13657,12687,11506,10439,9555,8624,7959,7656,//21-30
    7025,6558,6005,5516,5091,4703,4347,4049,3709,3477,//31-40
    3243,3028,2828,2615,2401,2257,2096,1964,1844,1751,//41-50
    1636,1535,1443,1346,1254,1178,1107,1043,977,934,//51-60
    880,831,782,741,689,646,615,579,550,519,//61-70
    498,468,450,421,395,377,357,342,325,314,//71-80
    296,282,269,255,241,229,216,207,200,189,//81-90
    182,172,164,157,149,145,136,131,125,118,//91-100
    113,110,106,103,95,91,87,83,80,77,//101-110
    72,71,70,68,63,61,58,56,55,53,//111-120
};

const unsigned int BIG_LWG_SUB_CH2[160] = {
    //20230310����
    191841,//0
    142789,135770,128108,117670,105588,94836,83637,77903,70340,65626,//1-10
    59394,51075,46276,42530,37649,34846,31400,28460,26192,23815,//11-20
    21179,19251,17592,16380,14994,13630,12388,11364,10491,9968,//21-30
    9187,8549,7864,7168,6625,6142,5667,5265,4821,4513,//31-40
    4195,3915,3640,3369,3081,2891,2666,2504,2338,2213,//41-50
    2061,1935,1813,1683,1562,1462,1371,1290,1208,1155,//51-60
    1083,1020,963,906,839,785,742,698,659,627,//61-70
    599,563,534,500,468,443,423,403,383,367,//71-80
    347,330,316,296,281,266,251,241,228,216,//81-90
    207,196,187,180,172,162,153,149,138,135,//91-100
    132,128,119,114,107,104,99,95,90,85,//101-110
    82,80,76,72,71,68,66,61,61,57,//111-120
};

const unsigned int BIG_LWG_SUB_CH3[160] = {
    //20230310����
    193581,//0
    149809,140294,131496,117456,100277,86132,74718,69307,61177,56606,//1-10
    50077,42401,37883,34372,30219,27621,24697,22132,20218,18388,//11-20
    16250,14734,13312,12367,11358,10316,9272,8605,7924,7489,//21-30
    6845,6356,5876,5350,4938,4606,4222,3953,3618,3410,//31-40
    3151,2934,2747,2551,2345,2191,2033,1911,1785,1695,//41-50
    1583,1492,1391,1303,1208,1135,1070,1010,948,908,//51-60
    850,808,760,718,671,625,592,559,530,505,//61-70
    478,456,432,406,384,361,343,329,310,302,//71-80
    290,274,262,249,234,223,212,203,191,183,//81-90
    174,166,160,152,145,137,132,126,120,115,//91-100
    108,104,100,95,93,90,86,83,80,73,//101-110
    71,69,66,65,61,59,57,55,53,52,//111-120
};




//�������Ϲ��ĸֽ������������̼��㷽��
//******************��ֵ��******************//
//��ֵ�� ���� ���ȱ�&����
const short int SUM_BIG_FI_LW_CH1[16]={
	1000,
    812,//6
    859,//8
    904,//10
    938,//12
    980,//14
    1000,//16
    1020,//18
    1034,//20
    1070,//22
    1107,//25
    1148,//28
    1169,//32
    1206,//40

	1306//50 20220512CZF
};
const short int SUM_BIG_ZERO_LW_CH1[16]={
	 0,
    -83,//6
    -71,//8
    -50,//10
    -27,//12
    -11,//14
    0,//16
    19,//18
    31,//20
    33,//22
    57,//25
    57,//28
    83,//32
    78,//40

	128//50 20220512CZF
};

//��ֵ�� ���� ���ȱ�&����
const short int SUM_BIG_FI_LW_CH2[16]={
	1000,
    838,//6
    874,//8
    950,//10
    950,//12
    985,//14
    1000,//16
    1015,//18
    1027,//20
    1055,//22
    1089,//25
    1137,//28
    1139,//32
    1182,//40

	1264//50
};
const short int SUM_BIG_ZERO_LW_CH2[16]={
	 0,
    -98,//6
    -75,//8
    -53,//10
    -30,//12
    -13,//14
     0,//16
    21,//18
    32,//20
    38,//22
    62,//25
    51,//28
    85,//32
    78,//40

	127//50
};

//��ֵ�� ���� ���ȱ�&����
const short int SUM_BIG_FI_LW_CH3[16]={
	1000,
    814,//6
    856,//8
    909,//10
    945,//12
    986,//14
    1000,//16
    1020,//18
    1035,//20
    1067,//22
    1113,//25
    1146,//28
    1168,//32
    1207,//40

	1309//50
};
const short int SUM_BIG_ZERO_LW_CH3[16]={
	 0,
    -87,//6
    -63,//8
    -51,//10
    -28,//12
    -16,//14
     0,//16
    20,//18
    29,//20
    36,//22
    55,//25
    60,//28
    82,//32
    79,//40

	125//50 20190410MY
};

//******************��ֵ��******************//
//��ֵ�� ���� ���ȱ�&����
const short int SUB_BIG_FI_LW_CH1[16]={
	1000,
    809,//6
    848,//8
    894,//10
    943,//12
    986,//14
    1000,//16
    1008,//18
    1027,//20
    1059,//22
    1191,//25
    1127,//28
    1152,//32
    1176,//40

	1274//50
};

const short int SUB_BIG_ZERO_LW_CH1[16]={
	 0,
    -33,//6
    -28,//8
    -19,//10
    -12,//12
    -6,//14
    0,//16
    17,//18
    20,//20
    16,//22
    31,//25
    25,//28
    38,//32
    33,//40

	47//50
};

//��ֵ�� ���� ���ȱ�&����
const short int SUB_BIG_FI_LW_CH2[16]={
	1000,
    851,//6
    877,//8
    913,//10
    942,//12
    986,//14
    1000,//16
    1007,//18
    1021,//20
    1047,//22
    1083,//25
    1115,//28
    1138,//32
    1161,//40

	1246//50
};

const short int SUB_BIG_ZERO_LW_CH2[16]={
	 0,
    -57,//6
    -39,//8
    -26,//10
    -9,//12
    -6,//14
    0,//16
    17,//18
    23,//20
    21,//22
    32,//25
    27,//28
    37,//32
    32,//40

	47//50
};

//��ֵ�� ���� ���ȱ�&����
const short int SUB_BIG_FI_LW_CH3[16]={
	1000,
    817,//6
    846,//8
    908,//10
    935,//12
    988,//14
    1000,//16
    1013,//18
    1022,//20
    1060,//22
    1094,//25
    1126,//28
    1151,//32
    1180,//40

	1273//50
};

const short int SUB_BIG_ZERO_LW_CH3[16]={
	 0,
    -40,//6
    -22,//8
    -25,//10
    -7,//12
    -10,//14
    0,//16
    17,//18
    23,//20
    18,//22
    32,//25
    28,//28
    37,//32
    31,//40

	45//50 20190410MY
};

//******************��ֵ��******************//
//��ֵ�� Բ�� ���ȱ�&����
const short int SUM_BIG_FI_YG_CH1[16]={
	1000,
    785,//6
    827,//8
    874,//10
    930,//12
    962,//14
    1009,//16
    1021,//18
    1038,//20
    1065,//22
    1107,//25
    1148,//28
    1169,//32
    1206,//40

	1306//50 20220512CZF
};
const short int SUM_BIG_ZERO_YG_CH1[16]={
	 0,
    -84,//6
    -62,//8
    -43,//10
    -17,//12
    -9,//14
    -3,//16
    24,//18
    34,//20
    37,//22
    57,//25
    57,//28
    83,//32
    78,//40

	128//50 20220512CZF
};

//��ֵ�� Բ�� ���ȱ�&����
const short int SUM_BIG_FI_YG_CH2[16]={
	1000,
    816,//6
    850,//8
    893,//10
    939,//12
    967,//14
    1001,//16
    1015,//18
    1029,//20
    1049,//22
    1089,//25
    1118,//28
    1139,//32
    1182,//40

	1264//50
};
const short int SUM_BIG_ZERO_YG_CH2[16]={
	 0,
    -100,//6
    -71,//8
    -52,//10
    -19,//12
    -12,//14
     5,//16
    29,//18
    37,//20
    42,//22
    62,//25
    68,//28
    85,//32
    78,//40

	127//50
};

//��ֵ�� Բ�� ���ȱ�&����
const short int SUM_BIG_FI_YG_CH3[16]={
	1000,
    795,//6
    833,//8
    877,//10
    929,//12
    966,//14
    1005,//16
    1026,//18
    1038,//20
    1058,//22
    1113,//25
    1146,//28
    1168,//32
    1207,//40

	1309//50
};
const short int SUM_BIG_ZERO_YG_CH3[16]={
	 0,
    -88,//6
    -61,//8
    -45,//10
    -18,//12
    -14,//14
     1,//16
    22,//18
    34,//20
    42,//22
    55,//25
    60,//28
    82,//32
    79,//40

	125//50 20190410MY
};

//******************��ֵ��******************//
//��ֵ�� Բ�� ���ȱ�&����
const short int SUB_BIG_FI_YG_CH1[16]={
	1000,
    801,//6
    842,//8
    879,//10
    944,//12
    966,//14
    1006,//16
    1022,//18
    1036,//20
    1062,//22
    1191,//25
    1127,//28
    1152,//32
    1176,//40

	1274//50
};

const short int SUB_BIG_ZERO_YG_CH1[16]={
	 0,
    -42,//6
    -27,//8
    -15,//10
    -8,//12
    -2,//14
    -1,//16
    16,//18
    20,//20
    17,//22
    31,//25
    25,//28
    38,//32
    33,//40

	47//50
};

//��ֵ�� Բ�� ���ȱ�&����
const short int SUB_BIG_FI_YG_CH2[16]={
	1000,
    832,//6
    870,//8
    900,//10
    945,//12
    970,//14
    1005,//16
    1017,//18
    1028,//20
    1050,//22
    1083,//25
    1115,//28
    1138,//32
    1161,//40

	1246//50
};

const short int SUB_BIG_ZERO_YG_CH2[16]={
	 0,
    -59,//6
    -40,//8
    -25,//10
    -7,//12
    -4,//14
    2,//16
    20,//18
    25,//20
    22,//22
    32,//25
    27,//28
    37,//32
    32,//40

	47//50
};

//��ֵ�� Բ�� ���ȱ�&����
const short int SUB_BIG_FI_YG_CH3[16]={
	1000,
    794,//6
    838,//8
    873,//10
    936,//12
    967,//14
    1004,//16
    1022,//18
    1036,//20
    1055,//22
    1094,//25
    1126,//28
    1151,//32
    1180,//40

	1273//50
};

const short int SUB_BIG_ZERO_YG_CH3[16]={
	 0,
    -38,//6
    -23,//8
    -12,//10
    -6,//12
    -5,//14
    1,//16
    19,//18
    23,//20
    23,//22
    32,//25
    28,//28
    37,//32
    31,//40

	45//50 20190410MY
};

//*********************************��ֵ�ܼ�ģʽ���*********************************//
//******************���Ƹֱ�******************////////////2��////////////////////////////////////////3��///////////////////////////////////////////4��///////////////////////////////
///////////////////////////////////////10//20//30//40//50//60//70//80//90//100/110//120////10//20//30//40//50//60//70//80//90//100/110/120////10//20//30//40//50//60//70//80//90/100/110/120
const MIJI_TAB SUB_MIJI_DIAM_06_LW ={{ 30, 36, 39, 45, 51, 57, 63, 66, 72, 75,  81,  87},{ 60, 66, 72, 84,  90, 96,102,114,126,138,144,160},{ 84, 90,102,114,120,138,156,169,180,198,210,222}};
const MIJI_TAB SUB_MIJI_DIAM_08_LW ={{ 33, 39, 45, 45, 51, 57, 63, 69, 75, 81,  87, 102},{ 54, 66, 78, 84,  96,108,114,120,132,144,156,174},{ 84, 96,102,114,126,144,156,168,184,196,210,222}};
const MIJI_TAB SUB_MIJI_DIAM_10_LW ={{ 33, 39, 45, 51, 54, 60, 63, 69, 78, 84,  90,  99},{ 54, 66, 72, 84,  90, 96,120,126,132,144,168,174},{ 84, 96,114,120,132,144,162,174,192,204,216,230}};
const MIJI_TAB SUB_MIJI_DIAM_12_LW ={{ 33, 39, 45, 51, 57, 63, 69, 75, 81, 87,  93, 102},{ 60, 66, 78, 84,  90,102,114,126,138,144,156,168},{ 90, 96,108,120,138,150,168,180,186,204,210,222}};
const MIJI_TAB SUB_MIJI_DIAM_14_LW ={{ 36, 42, 48, 51, 57, 60, 66, 72, 81, 87,  93, 102},{ 60, 66, 72, 84,  96,108,114,126,138,150,156,174},{ 84, 96,114,126,138,150,162,180,198,210,216,228}};
const MIJI_TAB SUB_MIJI_DIAM_16_LW ={{ 39, 42, 48, 54, 60, 66, 72, 78, 84, 90,  96, 102},{ 60, 72, 78, 84,  96,108,114,132,138,150,162,168},{ 90, 96,114,126,138,156,168,186,204,216,222,240}};
const MIJI_TAB SUB_MIJI_DIAM_18_LW ={{ 45, 48, 54, 60, 63, 66, 72, 78, 84, 90,  96, 108},{ 60, 66, 78, 84,  96,108,120,132,144,156,168,180},{ 90, 96,108,126,144,156,168,186,198,216,228,240}};
const MIJI_TAB SUB_MIJI_DIAM_20_LW ={{ 42, 48, 54, 57, 63, 69, 75, 81, 87, 93,  96, 102},{ 66, 72, 78, 84,  96,108,120,132,144,150,168,174},{ 90,102,120,126,138,156,168,186,198,216,234,251}};
const MIJI_TAB SUB_MIJI_DIAM_22_LW ={{ 48, 48, 54, 60, 66, 69, 72, 78, 84, 90,  96, 102},{ 66, 72, 84, 96, 102,114,120,132,138,156,162,168},{ 90,102,114,132,144,156,168,180,198,210,222,234}};
const MIJI_TAB SUB_MIJI_DIAM_25_LW ={{ 48, 51, 57, 63, 69, 72, 78, 84, 90, 96, 102, 108},{ 66, 78, 84, 96, 108,114,126,138,150,156,168,180},{ 96,108,120,132,150,162,180,192,210,222,240,252}};
const MIJI_TAB SUB_MIJI_DIAM_28_LW ={{ 51, 54, 60, 66, 72, 78, 84, 90, 96,102, 108, 114},{ 72, 78, 84, 90, 102,114,125,138,150,162,174,186},{ 96,102,120,132,114,168,180,192,210,228,234,264}};
const MIJI_TAB SUB_MIJI_DIAM_32_LW ={{ 57, 57, 60, 66, 72, 78, 84, 90, 96,102, 108, 114},{ 78, 78, 84, 96, 102,114,126,138,150,162,174,186},{108,108,120,132,144,162,174,186,204,222,246,257}};


//******************Բ�ֱ�******************//////////////2��////////////////////////////3��///////////////////////////4��/////////////
const MIJI_TAB SUB_MIJI_DIAM_06_YG ={{ 60, 66, 72, 78, 90, 96,108},{ 96,114,126,138,156,174,198},{150,168,180,204,222,246,282}};
const MIJI_TAB SUB_MIJI_DIAM_08_YG ={{ 60, 66, 72, 78, 84, 96,114},{108,114,120,138,156,174,192},{144,156,174,198,222,252,282}};
const MIJI_TAB SUB_MIJI_DIAM_10_YG ={{ 60, 66, 72, 78, 90, 96,114},{108,114,126,144,162,186,210},{150,162,186,204,228,270,282}};
const MIJI_TAB SUB_MIJI_DIAM_12_YG ={{ 60, 66, 72, 78, 90, 96,114},{108,114,132,144,156,180,192},{150,162,174,198,228,264,282}};
const MIJI_TAB SUB_MIJI_DIAM_14_YG ={{ 66, 66, 72, 78, 90,102,114},{102,114,132,144,156,174,192},{150,168,180,204,228,252,270}};
const MIJI_TAB SUB_MIJI_DIAM_16_YG ={{ 66, 72, 78, 84, 90,102,108},{108,114,132,138,156,174,204},{144,162,180,198,222,252,282}};
const MIJI_TAB SUB_MIJI_DIAM_18_YG ={{ 66, 72, 78, 84, 96,102,114},{108,120,132,144,162,180,198},{156,168,174,204,228,258,282}};
const MIJI_TAB SUB_MIJI_DIAM_20_YG ={{ 66, 72, 78, 84, 96,102,114},{114,120,132,150,162,172,192},{150,168,180,204,228,258,282}};
const MIJI_TAB SUB_MIJI_DIAM_22_YG ={{ 66, 72, 78, 84, 96,102,114},{108,120,132,156,168,186,204},{162,162,192,210,234,264,294}};
const MIJI_TAB SUB_MIJI_DIAM_25_YG ={{ 72, 78, 90, 96,102,108,120},{108,120,132,150,168,192,210},{162,168,192,210,240,270,294}};
const MIJI_TAB SUB_MIJI_DIAM_28_YG ={{ 72, 78, 90, 96,102,108,120},{114,126,138,156,174,192,210},{162,168,192,210,240,270,294}};
const MIJI_TAB SUB_MIJI_DIAM_32_YG ={{ 72, 78, 90, 96,102,108,120},{120,126,144,156,174,192,210},{162,168,192,216,240,275,294}};

//******************��ֱ����ַ��ѯ��******************//
const MIJI_TAB *SUB_MIJI_DIAM_TABLE[25] ={
    &SUB_MIJI_DIAM_06_LW, &SUB_MIJI_DIAM_08_LW, &SUB_MIJI_DIAM_10_LW, &SUB_MIJI_DIAM_12_LW, &SUB_MIJI_DIAM_14_LW, &SUB_MIJI_DIAM_16_LW,
    &SUB_MIJI_DIAM_18_LW, &SUB_MIJI_DIAM_20_LW, &SUB_MIJI_DIAM_22_LW, &SUB_MIJI_DIAM_25_LW, &SUB_MIJI_DIAM_28_LW, &SUB_MIJI_DIAM_32_LW,
    &SUB_MIJI_DIAM_06_YG, &SUB_MIJI_DIAM_08_YG, &SUB_MIJI_DIAM_10_YG, &SUB_MIJI_DIAM_12_YG, &SUB_MIJI_DIAM_14_YG, &SUB_MIJI_DIAM_16_YG,
    &SUB_MIJI_DIAM_18_YG, &SUB_MIJI_DIAM_20_YG, &SUB_MIJI_DIAM_22_YG, &SUB_MIJI_DIAM_25_YG, &SUB_MIJI_DIAM_28_YG, &SUB_MIJI_DIAM_32_YG
};

//*********************************΢���ܼ�ģʽ���*********************************//
//******************���Ƹֱ�******************////////////2��/////////////////////////////////////////////////////3��///////////////////////////////////////////4��///////////////////////////////
///////////////////////////////////////10//20//30//40//50//60//70//80//90//100//110//120////10//20//30//40//50//60//70//80//90/100/110/120////10//20//30//40//50//60//70//80//90/100/110/120//////
const MIJI_TAB DIF_MIJI_DIAM_16_LW ={{ 45, 53, 60, 71, 78, 84, 90, 96,108, 114, 121, 128},{ 78, 84, 90, 96,102,113,125,132,143,159,171,186},{108,114,120,126,156,174,186,198,216,228,240,264}};

const MIJI_TAB DIF_MIJI_DIAM_06_LW ={{ 60, 66, 72, 78, 90, 96,108},{ 96,114,126,138,156,174,198},{150,168,180,204,222,246,282}};
const MIJI_TAB DIF_MIJI_DIAM_08_LW ={{ 60, 66, 72, 78, 90, 96,114},{102,114,126,138,156,174,198},{144,156,174,204,222,246,282}};
const MIJI_TAB DIF_MIJI_DIAM_10_LW ={{ 60, 66, 72, 78, 90, 96,108},{102,114,132,150,162,180,204},{144,162,180,198,222,246,282}};
const MIJI_TAB DIF_MIJI_DIAM_12_LW ={{ 66, 66, 72, 78, 90,102,108},{102,120,138,150,162,180,198},{150,168,186,204,222,252,282}};
const MIJI_TAB DIF_MIJI_DIAM_14_LW ={{ 66, 66, 72, 78, 90,102,114},{102,114,132,144,156,174,192},{150,168,180,204,228,252,270}};
//const MIJI_TAB SUB_MIJI_DIAM_16_LW ={{ 36, 42, 48, 54, 60, 66, 72, 78, 84, 96, 102, 108},{ 72, 78, 84, 96, 114,126,132,150,156,174,186,198},{108,114,120,126,156,174,186,198,216,228,240,264}};
const MIJI_TAB DIF_MIJI_DIAM_18_LW ={{ 72, 78, 84, 90, 96,102,108},{114,120,126,144,162,180,216},{156,168,186,216,232,258,294}};
const MIJI_TAB DIF_MIJI_DIAM_20_LW ={{ 66, 72, 78, 84, 90,102,114},{108,120,138,156,162,180,204},{150,162,192,204,234,258,294}};
const MIJI_TAB DIF_MIJI_DIAM_22_LW ={{ 72, 78, 90, 90,102,108,120},{108,120,132,156,168,186,204},{162,162,192,210,234,264,294}};
const MIJI_TAB DIF_MIJI_DIAM_25_LW ={{ 72, 78, 90, 96,102,108,120},{108,120,132,150,168,192,210},{162,168,192,210,240,270,294}};
const MIJI_TAB DIF_MIJI_DIAM_28_LW ={{ 72, 78, 90, 96,102,108,120},{114,126,138,156,174,192,210},{162,168,192,210,240,270,294}};
const MIJI_TAB DIF_MIJI_DIAM_32_LW ={{ 72, 78, 90, 96,102,108,120},{120,126,144,156,174,192,210},{162,168,192,216,240,275,294}};

//******************Բ�ֱ�******************//////////////2��////////////////////////////3��///////////////////////////4��/////////////
const MIJI_TAB DIF_MIJI_DIAM_06_YG ={{ 60, 66, 72, 78, 90, 96,108},{ 96,114,126,138,156,174,198},{150,168,180,204,222,246,282}};
const MIJI_TAB DIF_MIJI_DIAM_08_YG ={{ 60, 66, 72, 78, 84, 96,114},{108,114,120,138,156,174,192},{144,156,174,198,222,252,282}};
const MIJI_TAB DIF_MIJI_DIAM_10_YG ={{ 60, 66, 72, 78, 90, 96,114},{108,114,126,144,162,186,210},{150,162,186,204,228,270,282}};
const MIJI_TAB DIF_MIJI_DIAM_12_YG ={{ 60, 66, 72, 78, 90, 96,114},{108,114,132,144,156,180,192},{150,162,174,198,228,264,282}};
const MIJI_TAB DIF_MIJI_DIAM_14_YG ={{ 66, 66, 72, 78, 90,102,114},{102,114,132,144,156,174,192},{150,168,180,204,228,252,270}};
const MIJI_TAB DIF_MIJI_DIAM_16_YG ={{ 66, 72, 78, 84, 90,102,108},{108,114,132,138,156,174,204},{144,162,180,198,222,252,282}};
const MIJI_TAB DIF_MIJI_DIAM_18_YG ={{ 66, 72, 78, 84, 96,102,114},{108,120,132,144,162,180,198},{156,168,174,204,228,258,282}};
const MIJI_TAB DIF_MIJI_DIAM_20_YG ={{ 66, 72, 78, 84, 96,102,114},{114,120,132,150,162,172,192},{150,168,180,204,228,258,282}};
const MIJI_TAB DIF_MIJI_DIAM_22_YG ={{ 66, 72, 78, 84, 96,102,114},{108,120,132,156,168,186,204},{162,162,192,210,234,264,294}};
const MIJI_TAB DIF_MIJI_DIAM_25_YG ={{ 72, 78, 90, 96,102,108,120},{108,120,132,150,168,192,210},{162,168,192,210,240,270,294}};
const MIJI_TAB DIF_MIJI_DIAM_28_YG ={{ 72, 78, 90, 96,102,108,120},{114,126,138,156,174,192,210},{162,168,192,210,240,270,294}};
const MIJI_TAB DIF_MIJI_DIAM_32_YG ={{ 72, 78, 90, 96,102,108,120},{120,126,144,156,174,192,210},{162,168,192,216,240,275,294}};

//******************��ֱ����ַ��ѯ��******************//
const MIJI_TAB *DIF_MIJI_DIAM_TABLE[25] ={
    &DIF_MIJI_DIAM_06_LW, &DIF_MIJI_DIAM_08_LW, &DIF_MIJI_DIAM_10_LW, &DIF_MIJI_DIAM_12_LW, &DIF_MIJI_DIAM_14_LW, &DIF_MIJI_DIAM_16_LW,
    &DIF_MIJI_DIAM_18_LW, &DIF_MIJI_DIAM_20_LW, &DIF_MIJI_DIAM_22_LW, &DIF_MIJI_DIAM_25_LW, &DIF_MIJI_DIAM_28_LW, &DIF_MIJI_DIAM_32_LW,
    &DIF_MIJI_DIAM_06_YG, &DIF_MIJI_DIAM_08_YG, &DIF_MIJI_DIAM_10_YG, &DIF_MIJI_DIAM_12_YG, &DIF_MIJI_DIAM_14_YG, &DIF_MIJI_DIAM_16_YG,
    &DIF_MIJI_DIAM_18_YG, &DIF_MIJI_DIAM_20_YG, &DIF_MIJI_DIAM_22_YG, &DIF_MIJI_DIAM_25_YG, &DIF_MIJI_DIAM_28_YG, &DIF_MIJI_DIAM_32_YG
};

//��ʱ����

//*****************�ļ��ڲ�����*****************//
static int iSumMaxHD = 0;//����bugʹ�ã��������֤����Ҫ�ŵ��ṹ����

//================================================================//
//���ܣ�����5·ͨ���ļ��ֵ
//������NULL
//���أ�ADC_RES_STR
//================================================================//
static ADC_RES_STR GetSourceData(void)
{
    
    AdcDataBuf.iCh1 = g_iCh1AdcVal;
    AdcDataBuf.iCh2 = g_iCh2AdcVal;
    AdcDataBuf.iCh3 = g_iCh3AdcVal;
    AdcDataBuf.iCh4 = g_iCh4AdcVal;
    AdcDataBuf.iCh5 = g_iCh5AdcVal;
    AdcDataBuf.iCh6 = g_iCh6AdcVal;
    
//////    //��ֵ����
//////    AdcDataBuf.iSub1 = g_iCh2AdcVal - g_iCh1AdcVal;
//////    AdcDataBuf.iSub2 = g_iCh4AdcVal - g_iCh3AdcVal;
//////    AdcDataBuf.iSub3 = g_iCh6AdcVal - g_iCh5AdcVal;
//////    
//////    //��ֵ����
//////    AdcDataBuf.iSum1 = g_iCh1AdcVal + g_iCh2AdcVal;
//////    AdcDataBuf.iSum2 = g_iCh3AdcVal + g_iCh4AdcVal;
//////    AdcDataBuf.iSum3 = g_iCh5AdcVal + g_iCh6AdcVal;

    return AdcDataBuf;
}

//================================================================38//
//���ܣ�ͨ��ADCֵ��������׼��������
//������NULL
//���أ�NULL
//================================================================//
unsigned char AdcProduce_Aim(void)
{
//    unsigned int uibuf;
//    float fbuf1, fbuf2;
//    float mm;
    
    //ZJC_TEST��ʾ��ֵ�Ͳ�ֵ���ֵ
//    SetColor(CL_BLACK,CL_BLUE);
//    sprintf(g_cStrBuf,"%-8d,%-8d",g_uiCh1AdcVal,g_uiCh2AdcVal);
//    DispText(ASC10x16,150,62,g_cStrBuf);
    //END
    /*
       	SetColor(CL_BLACK,CL_WHITE);
        sprintf(g_cStrBuf,"%5d",g_uiCh1SmallZero); //
        DispText(ASC12x24,0,20,g_cStrBuf);
        sprintf(g_cStrBuf,"%5d",g_uiCh2SmallZero); //
        DispText(ASC12x24,0,80,g_cStrBuf);
    */
    
    //��ȡ��ͨ���ź���
    GetSourceData();
    
    //�˲�
//////    g_uiCh1Buf_AIM[0] = g_uiCh1Buf_AIM[1];
//////    g_uiCh1Buf_AIM[1] = g_uiCh1Buf_AIM[2];
//////    g_uiCh1Buf_AIM[2] = g_uiCh1Buf_AIM[3];
//////    g_uiCh1Buf_AIM[3] = g_uiCh1Buf_AIM[4];
//////    g_uiCh1Buf_AIM[4] = g_uiCh1Buf_AIM[5];
//////    g_uiCh1Buf_AIM[5] = g_uiCh1Buf_AIM[6];
//////    g_uiCh1Buf_AIM[6] = g_uiCh1AdcVal;

//////    g_uiCh2Buf_AIM[0] = g_uiCh2Buf_AIM[1];
//////    g_uiCh2Buf_AIM[1] = g_uiCh2Buf_AIM[2];
//////    g_uiCh2Buf_AIM[2] = g_uiCh2Buf_AIM[3];
//////    g_uiCh2Buf_AIM[3] = g_uiCh2Buf_AIM[4];
//////    g_uiCh2Buf_AIM[4] = g_uiCh2Buf_AIM[5];
//////    g_uiCh2Buf_AIM[5] = g_uiCh2Buf_AIM[6];
//////    g_uiCh2Buf_AIM[6] = g_uiCh2AdcVal;

//////    g_uiLvBoBuf1_AIM[0] = g_uiLvBoBuf1_AIM[1];
//////    g_uiLvBoBuf1_AIM[1] = g_uiLvBoBuf1_AIM[2];
//////    g_uiLvBoBuf1_AIM[2] = g_uiLvBoBuf1_AIM[3];
//////    g_uiLvBoBuf1_AIM[3] = g_uiLvBoBuf1_AIM[4];
//////    g_uiLvBoBuf1_AIM[4] = Lvbo(g_uiCh1Buf_AIM);
//////    g_uiCh1Avg_AIM =  PaiXu(g_uiLvBoBuf1_AIM);

//////    g_uiLvBoBuf2_AIM[0] = g_uiLvBoBuf2_AIM[1];
//////    g_uiLvBoBuf2_AIM[1] = g_uiLvBoBuf2_AIM[2];
//////    g_uiLvBoBuf2_AIM[2] = g_uiLvBoBuf2_AIM[3];
//////    g_uiLvBoBuf2_AIM[3] = g_uiLvBoBuf2_AIM[4];
//////    g_uiLvBoBuf2_AIM[4] = Lvbo(g_uiCh2Buf_AIM);
//////    g_uiCh2Avg_AIM =  PaiXu(g_uiLvBoBuf2_AIM);
//////    
//////    g_uiSumVal_AIM[0] = g_uiSumVal_AIM[1];
//////    g_uiSumVal_AIM[1] = g_uiSumVal_AIM[2];
//////    g_uiSumVal_AIM[2] = g_uiCh1Avg_AIM + g_uiCh2Avg_AIM;
 
//    //��ֵ�˲�
//    g_uiSubBuf[0] = g_uiSubBuf[1];
//    g_uiSubBuf[1] = g_uiSubBuf[2];
//    g_uiSubBuf[2] = g_uiSubBuf[3];
//    g_uiSubBuf[3] = g_uiSubBuf[4];
//    g_uiSubBuf[4] = g_uiSubBuf[5];
//    g_uiSubBuf[5] = g_uiSubBuf[6];
//    g_uiSubBuf[6] = g_uiCh2Avg - g_uiCh1Avg;
////    g_uiSubBuf[6] = (g_uiCh2Avg - g_uiCh1Avg)*0.5+g_uiSubAvg*0.5;
//   
//    if(g_ucSampDotCnt < 7) { //С��5�������㣬ֱ���˳��������к������
//        g_ucSampDotCnt += 1;
//        g_iMzjSite = 0;
//        return 0;
//    }
//    
//    g_uiSubLvBoBuf[0] = g_uiSubLvBoBuf[1];
//    g_uiSubLvBoBuf[1] = g_uiSubLvBoBuf[2];
//    g_uiSubLvBoBuf[2] = g_uiSubLvBoBuf[3];
//    g_uiSubLvBoBuf[3] = g_uiSubLvBoBuf[4];
//    g_uiSubLvBoBuf[4] = Lvbo(g_uiSubBuf);
//    g_uiSubAvg = PaiXu(g_uiSubLvBoBuf);
    
    return 0;
}

//================================================================//
//���ܣ���ȼ��㺯�������غ������
//������iAdcValue      						ʵʱADCֵ
// 		workPara							�ֽ����ѡ��
//		bGuCeZhiJing						�Ƿ����
//      cCeHouMode                          �Ƿ�Ϊ���ģʽ
//      bNewAdcCalcflag                     ��ֵ���ֵ��������ѡ��
//���أ����ֵ
//================================================================//
int FindCoverTab_Sum(enum CHANNELNUM Channel, unsigned int iAdcValue, WORKPARA WorkPara, AIMREBAR_BUF_STR *AimBuf)
{
    const unsigned int  *pAdcTab;//tab?
    const unsigned short int *pHDTab;
    const short int *pFIPara;
	const short int *pZeroPara;
//////    short int *pSCFIPara;
//////	short int *pSCZeroPara;
    int TabSize;
    
    int iDiamIndex_1 = 0;
    int iDiamIndex_2 = 0;

//	const double *JianJuPara_A;
//	const double *JianJuPara_B;
//	signed char  *UserTab; //�û���������

	int i;
	double dRatio;
	double dHouDu = 0;
	double dBuf_A = 0;
    double dBuf_B = 0;
	double dSpace=0.0;
	int iDiamMin = 0;

    //������ָ�븳ֵ
//////	if(WorkPara.cLingChengType == 1){//������
        if(WorkPara.cGangJinType == 0){//���Ƹ�
            if(Channel == CH1){
                pAdcTab = BIG_LWG_SUM_CH1;
                pFIPara = SUM_BIG_FI_LW_CH1;
                pZeroPara = SUM_BIG_ZERO_LW_CH1;//
            }
            else if(Channel == CH2){
                pAdcTab = BIG_LWG_SUM_CH2;
                pFIPara = SUM_BIG_FI_LW_CH2;
                pZeroPara = SUM_BIG_ZERO_LW_CH2;
            }
            else if(Channel == CH3){
                pAdcTab = BIG_LWG_SUM_CH3;
                pFIPara = SUM_BIG_FI_LW_CH3;
                pZeroPara = SUM_BIG_ZERO_LW_CH3;
            }
//////            pSCFIPara = &g_SysPara.iBigFi_LW[0];
//////            pSCZeroPara = &g_SysPara.iBigZero_LW[0];
            
//            UserTab = &g_SysPara.iBigUser_LWG[0];
//				UserTab = User_SMALL_LWG;
        }
        if(WorkPara.cGangJinType == 1){//Բ��
            if(Channel == CH1){
                pAdcTab = BIG_LWG_SUM_CH1;
                pFIPara = SUM_BIG_FI_YG_CH1;
                pZeroPara = SUM_BIG_ZERO_YG_CH1;
            }
            else if(Channel == CH2){
                pAdcTab = BIG_LWG_SUM_CH2;
                pFIPara = SUM_BIG_FI_YG_CH2;
                pZeroPara = SUM_BIG_ZERO_YG_CH2;
            }
            else if(Channel == CH3){
                pAdcTab = BIG_LWG_SUM_CH3;
                pFIPara = SUM_BIG_FI_YG_CH3;
                pZeroPara = SUM_BIG_ZERO_YG_CH3;
            }
//////            pSCFIPara = &g_SysPara.iBigFi_LW[0];
//////            pSCZeroPara = &g_SysPara.iBigZero_LW[0];
            
//            UserTab = &g_SysPara.iBigUser_LWG[0];
//				UserTab = User_SMALL_LWG;
        }
        TabSize = 268;//268��
        pHDTab = COVER_VAL;
//////    }
    
//    iAdcValue *= 2;

	//ͨ����������ֵ
	if(iAdcValue > pAdcTab[0]){
		dHouDu = 0;
	}
	else {
		for(i=0; i<TabSize; i++){
			if((iAdcValue <= pAdcTab[i]) && (iAdcValue > pAdcTab[i+1])){
                dRatio = (double)(pAdcTab[i]-iAdcValue)/(pAdcTab[i] - pAdcTab[i+1]);
                dHouDu = pHDTab[i] + dRatio;
				break;
			}
		}
	}
	dHouDu *= 10; //���ֵ�Ŵ�10��
    
    AimBuf->EstSumCh2Diam16 = dHouDu;
    
	//��ֱͬ������,���в�������ֱ����ʹ�����Բ�ֵ�������㲹��ֵ
    if(WorkPara.cZhujinDiam <= 22){
        if(WorkPara.cZhujinDiam%2 == 0){
            iDiamIndex_1 = WorkPara.cZhujinDiam/2 - 2;
            dHouDu = dHouDu * pFIPara[iDiamIndex_1]/1000 + pZeroPara[iDiamIndex_1];
        }
        else{
            iDiamIndex_1 = WorkPara.cZhujinDiam/2-2;
            iDiamIndex_2 = WorkPara.cZhujinDiam/2-1;
            dHouDu = dHouDu * (pFIPara[iDiamIndex_1] + pFIPara[iDiamIndex_2])/2000
                + (pZeroPara[iDiamIndex_1] + pZeroPara[iDiamIndex_2])/2;
        }
    }
    else{
        if(WorkPara.cZhujinDiam <= 25){
            iDiamIndex_1 = 9;
            iDiamIndex_2 = 10;
            iDiamMin = 22;
			dSpace=3.0;
        }
        else if(WorkPara.cZhujinDiam <= 28){
            iDiamIndex_1 = 10;
            iDiamIndex_2 = 11;
            iDiamMin = 25;
			dSpace=3.0;
        }
        else if(WorkPara.cZhujinDiam <= 32){
            iDiamIndex_1 = 11;
            iDiamIndex_2 = 12;
            iDiamMin = 28;
			dSpace=4.0;
        }
        else if(WorkPara.cZhujinDiam <= 40){
            iDiamIndex_1 = 12;
            iDiamIndex_2 = 13;
            iDiamMin = 32;
			dSpace=8.0;
        }
        else if(WorkPara.cZhujinDiam <= 50){
            iDiamIndex_1 = 13;
            iDiamIndex_2 = 14;
            iDiamMin = 40;
			dSpace=10.0;
        }
        dBuf_A = ((pFIPara[iDiamIndex_2]-pFIPara[iDiamIndex_1])*(WorkPara.cZhujinDiam - iDiamMin)/dSpace + pFIPara[iDiamIndex_1])/1000.0;
        dBuf_B = (pZeroPara[iDiamIndex_2]-pZeroPara[iDiamIndex_1])*(WorkPara.cZhujinDiam - iDiamMin)/dSpace + pZeroPara[iDiamIndex_1];
        dHouDu = dHouDu*dBuf_A + dBuf_B;
    }
//////    dHouDu += 5; //��������
    
    return (int)dHouDu;
}

//================================================================//
//����: ��ʼ����λ�ֽ����
//������NULL
//���أ�NULL
//================================================================//
void InitAimRebarParaCH1(void)
{
//////    memset(&AimRebarBuf_CH1, 0, sizeof(AimRebarBuf_CH1));
    memset(&AimRebarPara_CH1.ucSubAimStep, 0, sizeof(AimRebarPara_CH1));
    AimRebarPara_CH1.iSubMaxVal = -100000;
    AimRebarPara_CH1.iSubMinVal = 100000;
//    delay_ms(1000);
}

//================================================================//
//����: ��ʼ����λ�ֽ����
//������NULL
//���أ�NULL
//================================================================//
void InitAimRebarParaCH2(void)
{
//////    memset(&AimRebarBuf_CH2, 0, sizeof(AimRebarBuf_CH2));
    memset(&AimRebarPara_CH2.ucSubAimStep, 0, sizeof(AimRebarPara_CH2));
    AimRebarPara_CH2.iSubMaxVal = -100000;
    AimRebarPara_CH2.iSubMinVal = 100000;
//    delay_ms(1000);
}

//================================================================//
//����: ��ʼ����λ�ֽ����
//������NULL
//���أ�NULL
//================================================================//
void InitAimRebarParaCH3(void)
{
//////    memset(&AimRebarBuf_CH3, 0, sizeof(AimRebarBuf_CH3));
    memset(&AimRebarPara_CH3.ucSubAimStep, 0, sizeof(AimRebarPara_CH3));
    AimRebarPara_CH3.iSubMaxVal = -100000;
    AimRebarPara_CH3.iSubMinVal = 100000;
//    delay_ms(1000);
}

//================================================================//
//����: ��ʼ����λ�ֽ�����
//������NULL
//���أ�NULL
//================================================================//
void InitAimRebarResult(void)
{
    memset(&ResBuf_CH1, 0, sizeof(ResBuf_CH1));
    memset(&ResBuf_CH2, 0, sizeof(ResBuf_CH2));
    memset(&ResBuf_CH3, 0, sizeof(ResBuf_CH3));
}

//================================================================//
//����: ��ʼ����λ�ֽ��
//������NULL
//���أ�NULL
//================================================================//
void InitAimPara(void)
{
    memset(&SourceData.iCH1[0], 0, sizeof(SourceData));

    memset(&AimRebarBuf_CH2, 0, sizeof(AimRebarBuf_CH2));
    memset(&AimRebarBuf_CH1, 0, sizeof(AimRebarBuf_CH1));
    memset(&AimRebarBuf_CH3, 0, sizeof(AimRebarBuf_CH3));
    
    InitAimRebarParaCH1();
    InitAimRebarParaCH2();
    InitAimRebarParaCH3();
    
    InitAimRebarResult();//��ʼ���ֽ�����

//    delay_ms(1000);
    
    #ifdef TEST_ZJC
    //��ֵ��λ�ֽ������
    LCD_SetColor(WHITE,BLUE);//CL_BLUE
    LCD_ShowString(ASC16x8, 50, 40, 0,"CH1");
    LCD_ShowString(ASC16x8,200, 40, 0,"CH2");
    LCD_ShowString(ASC16x8,350, 40, 0,"CH3");
    #endif
}

//�������鸳ֵ
void SourceBufAddData(enum SOURCETYPE Type, int iData)
{
    switch(Type) {
        case CH1SUM:
            SourceData.cCH1SumCnt++;
            if(SourceData.cCH1SumCnt == 15){
                SourceData.cCH1SumCnt = 0;
            }
            SourceData.iCH1Sum[SourceData.cCH1SumCnt] = iData;
        break;
        
        case CH2SUM:
            SourceData.cCH2SumCnt++;
            if(SourceData.cCH2SumCnt == 15){
                SourceData.cCH2SumCnt = 0;
            }
            SourceData.iCH2Sum[SourceData.cCH2SumCnt] = iData;
        break;
        
        case CH3SUM:
            SourceData.cCH3SumCnt++;
            if(SourceData.cCH3SumCnt == 15){
                SourceData.cCH3SumCnt = 0;
            }
            SourceData.iCH3Sum[SourceData.cCH3SumCnt] = iData;
        break;
        
        case CH1SUB:
            SourceData.cCH1SubCnt++;
            if(SourceData.cCH1SubCnt == 15){
                SourceData.cCH1SubCnt = 0;
            }
            SourceData.iCH1Sub[SourceData.cCH1SubCnt] = iData;
        break;
        
        case CH2SUB:
            SourceData.cCH2SubCnt++;
            if(SourceData.cCH2SubCnt == 15){
                SourceData.cCH2SubCnt = 0;
            }
            SourceData.iCH2Sub[SourceData.cCH2SubCnt] = iData;
        break;
        
        case CH3SUB:
            SourceData.cCH3SubCnt++;
            if(SourceData.cCH3SubCnt == 15){
                SourceData.cCH3SubCnt = 0;
            }
            SourceData.iCH3Sub[SourceData.cCH3SubCnt] = iData;
        break;
        
        default:
        break;
    }
}

//��������ȡֵ
void SourceBufGetData(enum SOURCETYPE Type, int *GetBuf, int OffsetAddr, int iCnt)
{
    int *BufPoint;
    int i = 0;
    char cBufCnt;
    
    switch(Type){
        case CH1SUM:
            cBufCnt = SourceData.cCH1SumCnt;
            BufPoint = SourceData.iCH1Sum;
        break;
        
        case CH2SUM:
            cBufCnt = SourceData.cCH2SumCnt;
            BufPoint = SourceData.iCH2Sum;
        break;
        
        case CH3SUM:
            cBufCnt = SourceData.cCH3SumCnt;
            BufPoint = SourceData.iCH3Sum;
        break;
        
        case CH1SUB:
            cBufCnt = SourceData.cCH1SubCnt;
            BufPoint = SourceData.iCH1Sub;
        break;
        
        case CH2SUB:
            cBufCnt = SourceData.cCH2SubCnt;
            BufPoint = SourceData.iCH2Sub;
        break;
        
        case CH3SUB:
            cBufCnt = SourceData.cCH3SubCnt;
            BufPoint = SourceData.iCH3Sub;
        break;
        
        default:
        break;
    }
    
    for(i=0;i<iCnt;i++){
        if((cBufCnt-i-OffsetAddr) < 0){
            cBufCnt = 14+i+OffsetAddr;
        }
        GetBuf[i] = BufPoint[cBufCnt-i-OffsetAddr];
    }
}

void SaveAdcDataBuf(void)
{
    ADC_RES_STR AdcDataSource;
    int CH1Buf=0;
    int CH2Buf=0;
    int CH3Buf=0;
    int CH4Buf=0;
    int CH5Buf=0;
    int CH6Buf=0;
    int CH1_OffsetVal=0;
    int CH2_OffsetVal=0;
    int CH3_OffsetVal=0;
    int CH4_OffsetVal=0;
    int CH5_OffsetVal=0;
    int CH6_OffsetVal=0;
    int OffsetPartCnt = 0;

//    ADC_RES_STR AdcDataAvg;
    int i = 0;
    int iCurVal = 0;
    int iLastVal = 0;
    
    //���ݷ���λ�����
//    static unsigned char ucIndex = 0;
//    unsigned int uiSumAvg = 0;
    
    //�����ݷ���ԭʼ���ݻ�����
    if(g_iCarDist <= g_iCarLastDist){
        return;
    }

    //��ȡ����
    AdcDataSource = GetSourceData();

    //�������ݻ���
//    g_iCarLastDist = g_iCarDist;
    
    if((g_iCarDist > 0)&&(SourceData.iCH1Start == 0)&&(SourceData.iCH2Start == 0)&&(SourceData.iCH3Start == 0)
        &&(SourceData.iCH4Start == 0)&&(SourceData.iCH5Start == 0)&&(SourceData.iCH6Start == 0)){
        
        SourceData.iCH1[0] = AdcDataSource.iCh1;
        SourceData.iCH2[0] = AdcDataSource.iCh2;
        SourceData.iCH3[0] = AdcDataSource.iCh3;
        SourceData.iCH4[0] = AdcDataSource.iCh4;
        SourceData.iCH5[0] = AdcDataSource.iCh5;
        SourceData.iCH6[0] = AdcDataSource.iCh6;
            
        SourceData.iCH1Start = AdcDataSource.iCh1;
        SourceData.iCH2Start = AdcDataSource.iCh2;
        SourceData.iCH3Start = AdcDataSource.iCh3;
        SourceData.iCH4Start = AdcDataSource.iCh4;
        SourceData.iCH5Start = AdcDataSource.iCh5;
        SourceData.iCH6Start = AdcDataSource.iCh6;
            
        SourceData.iCH1Last = AdcDataSource.iCh1;
        SourceData.iCH2Last = AdcDataSource.iCh2;
        SourceData.iCH3Last = AdcDataSource.iCh3;
        SourceData.iCH4Last = AdcDataSource.iCh4;
        SourceData.iCH5Last = AdcDataSource.iCh5;
        SourceData.iCH6Last = AdcDataSource.iCh6;
            
        SourceData.iCH1[0] = AdcDataSource.iCh1;
        SourceData.iCH2[0] = AdcDataSource.iCh2;
        SourceData.iCH3[0] = AdcDataSource.iCh3;
        SourceData.iCH4[0] = AdcDataSource.iCh4;
        SourceData.iCH5[0] = AdcDataSource.iCh5;
        SourceData.iCH6[0] = AdcDataSource.iCh6;
            
        for(i=0;i<g_iCarDist;i++){
//            SourceData.iCh1[i+1] = 0;
//            SourceData.iCh2[i+1] = 0;
//            SourceData.iCh3[i+1] = 0;
//            SourceData.iCh4[i+1] = 0;
//            SourceData.iCh5[i+1] = 0;
//            SourceData.iCh6[i+1] = 0;
            
            SourceBufAddData(CH1SUM, AdcDataSource.iCh1 + AdcDataSource.iCh2);
            SourceBufAddData(CH2SUM, AdcDataSource.iCh3 + AdcDataSource.iCh4);
            SourceBufAddData(CH3SUM, AdcDataSource.iCh5 + AdcDataSource.iCh6);
            SourceBufAddData(CH1SUB, AdcDataSource.iCh2 - AdcDataSource.iCh1);
            SourceBufAddData(CH2SUB, AdcDataSource.iCh4 - AdcDataSource.iCh3);
            SourceBufAddData(CH3SUB, AdcDataSource.iCh6 - AdcDataSource.iCh5);
        }
        return;
    }
        
//    SourceData.iCH1[g_iCarDist] = AdcDataSource.iCh1;
//    SourceData.iCH2[g_iCarDist] = AdcDataSource.iCh2;
//    SourceData.iCH3[g_iCarDist] = AdcDataSource.iCh3;
//    SourceData.iCH4[g_iCarDist] = AdcDataSource.iCh4;
//    SourceData.iCH5[g_iCarDist] = AdcDataSource.iCh5;
//    SourceData.iCH6[g_iCarDist] = AdcDataSource.iCh6;

    CH1_OffsetVal = AdcDataSource.iCh1 - SourceData.iCH1Last;
    CH2_OffsetVal = AdcDataSource.iCh2 - SourceData.iCH2Last;
    CH3_OffsetVal = AdcDataSource.iCh3 - SourceData.iCH3Last;
    CH4_OffsetVal = AdcDataSource.iCh4 - SourceData.iCH4Last;
    CH5_OffsetVal = AdcDataSource.iCh5 - SourceData.iCH5Last;
    CH6_OffsetVal = AdcDataSource.iCh6 - SourceData.iCH6Last;
    
    //���հ�λ��
    if(g_iCarDist > 2){
        if(SourceData.iCH3[g_iCarDist-1] == 0){
            if(SourceData.iCH3[g_iCarDist-2] != 0){//����һ����λ
                
                CH1_OffsetVal /= 2;
                CH2_OffsetVal /= 2;
                CH3_OffsetVal /= 2;
                CH4_OffsetVal /= 2;
                CH5_OffsetVal /= 2;
                CH6_OffsetVal /= 2;
                OffsetPartCnt = 2;
                
//                SourceData.iCH1[g_iCarDist-1] = (SourceData.iCH1Last + AdcDataSource.iCh1)/2;
//                SourceData.iCH2[g_iCarDist-1] = (SourceData.iCH2Last + AdcDataSource.iCh2)/2;
//                SourceData.iCH3[g_iCarDist-1] = (SourceData.iCH3Last + AdcDataSource.iCh3)/2;
//                SourceData.iCH4[g_iCarDist-1] = (SourceData.iCH4Last + AdcDataSource.iCh4)/2;
//                SourceData.iCH5[g_iCarDist-1] = (SourceData.iCH5Last + AdcDataSource.iCh5)/2;
//                SourceData.iCH6[g_iCarDist-1] = (SourceData.iCH6Last + AdcDataSource.iCh6)/2;
                
//                SourceData.iCh1[g_iCarDist-1] = (SourceData.iCH1Last + AdcDataSource.iCh1)/2;
//                SourceData.iCh2[g_iCarDist-1] = (SourceData.iCH2Last + AdcDataSource.iCh2)/2;
//                SourceData.iCh3[g_iCarDist-1] = (SourceData.iCH3Last + AdcDataSource.iCh3)/2;
//                SourceData.iCh4[g_iCarDist-1] = (SourceData.iCH4Last + AdcDataSource.iCh4)/2;
//                SourceData.iCh5[g_iCarDist-1] = (SourceData.iCH5Last + AdcDataSource.iCh5)/2;
//                SourceData.iCh6[g_iCarDist-1] = (SourceData.iCH6Last + AdcDataSource.iCh6)/2;
                
                CH1Buf = (SourceData.iCH1Last + AdcDataSource.iCh1)/2;
                CH2Buf = (SourceData.iCH2Last + AdcDataSource.iCh2)/2;
                CH3Buf = (SourceData.iCH3Last + AdcDataSource.iCh3)/2;
                CH4Buf = (SourceData.iCH4Last + AdcDataSource.iCh4)/2;
                CH5Buf = (SourceData.iCH5Last + AdcDataSource.iCh5)/2;
                CH6Buf = (SourceData.iCH6Last + AdcDataSource.iCh6)/2;

                SourceBufAddData(CH1SUM, CH1Buf + CH2Buf);
                SourceBufAddData(CH2SUM, CH3Buf + CH4Buf);
                SourceBufAddData(CH3SUM, CH5Buf + CH6Buf);
                SourceBufAddData(CH1SUB, CH2Buf - CH1Buf);
                SourceBufAddData(CH2SUB, CH4Buf - CH3Buf);
                SourceBufAddData(CH3SUB, CH6Buf - CH5Buf);
            }
            else if(SourceData.iCH3[g_iCarDist-2] == 0){
                if(SourceData.iCH3[g_iCarDist-3] != 0){//����������λ
                    
                    CH1_OffsetVal /= 3;
                    CH2_OffsetVal /= 3;
                    CH3_OffsetVal /= 3;
                    CH4_OffsetVal /= 3;
                    CH5_OffsetVal /= 3;
                    CH6_OffsetVal /= 3;
                    OffsetPartCnt = 3;
                    
//                    SourceData.iCH1[g_iCarDist-2] = SourceData.iCH1[g_iCarDist-3] + CH1_OffsetVal;
//                    SourceData.iCH2[g_iCarDist-2] = SourceData.iCH2[g_iCarDist-3] + CH2_OffsetVal;
//                    SourceData.iCH3[g_iCarDist-2] = SourceData.iCH3[g_iCarDist-3] + CH3_OffsetVal;
//                    SourceData.iCH4[g_iCarDist-2] = SourceData.iCH4[g_iCarDist-3] + CH4_OffsetVal;
//                    SourceData.iCH5[g_iCarDist-2] = SourceData.iCH5[g_iCarDist-3] + CH5_OffsetVal;
//                    SourceData.iCH6[g_iCarDist-2] = SourceData.iCH6[g_iCarDist-3] + CH6_OffsetVal; 

//                    SourceData.iCh1[g_iCarDist-2] = SourceData.iCH1[g_iCarDist-3] + CH1_OffsetVal;
//                    SourceData.iCh2[g_iCarDist-2] = SourceData.iCH2[g_iCarDist-3] + CH2_OffsetVal;
//                    SourceData.iCh3[g_iCarDist-2] = SourceData.iCH3[g_iCarDist-3] + CH3_OffsetVal;
//                    SourceData.iCh4[g_iCarDist-2] = SourceData.iCH4[g_iCarDist-3] + CH4_OffsetVal;
//                    SourceData.iCh5[g_iCarDist-2] = SourceData.iCH5[g_iCarDist-3] + CH5_OffsetVal;
//                    SourceData.iCh6[g_iCarDist-2] = SourceData.iCH6[g_iCarDist-3] + CH6_OffsetVal;  

                    //g_iCarDist-2
                    CH1Buf = SourceData.iCH1Last + CH1_OffsetVal;
                    CH2Buf = SourceData.iCH2Last + CH2_OffsetVal;
                    CH3Buf = SourceData.iCH3Last + CH3_OffsetVal;
                    CH4Buf = SourceData.iCH4Last + CH4_OffsetVal;
                    CH5Buf = SourceData.iCH5Last + CH5_OffsetVal;
                    CH6Buf = SourceData.iCH6Last + CH6_OffsetVal;

                    SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                    SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                    SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                    SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                    SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                    SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                    
//                    SourceData.iCH1[g_iCarDist-1] = SourceData.iCH1[g_iCarDist-3] + 2*CH1_OffsetVal;
//                    SourceData.iCH2[g_iCarDist-1] = SourceData.iCH2[g_iCarDist-3] + 2*CH2_OffsetVal;
//                    SourceData.iCH3[g_iCarDist-1] = SourceData.iCH3[g_iCarDist-3] + 2*CH3_OffsetVal;
//                    SourceData.iCH4[g_iCarDist-1] = SourceData.iCH4[g_iCarDist-3] + 2*CH4_OffsetVal;
//                    SourceData.iCH5[g_iCarDist-1] = SourceData.iCH5[g_iCarDist-3] + 2*CH5_OffsetVal;
//                    SourceData.iCH6[g_iCarDist-1] = SourceData.iCH6[g_iCarDist-3] + 2*CH6_OffsetVal;
                    
//                    SourceData.iCh1[g_iCarDist-1] = SourceData.iCH1[g_iCarDist-3] + 2*CH1_OffsetVal;
//                    SourceData.iCh2[g_iCarDist-1] = SourceData.iCH2[g_iCarDist-3] + 2*CH2_OffsetVal;
//                    SourceData.iCh3[g_iCarDist-1] = SourceData.iCH3[g_iCarDist-3] + 2*CH3_OffsetVal;
//                    SourceData.iCh4[g_iCarDist-1] = SourceData.iCH4[g_iCarDist-3] + 2*CH4_OffsetVal;
//                    SourceData.iCh5[g_iCarDist-1] = SourceData.iCH5[g_iCarDist-3] + 2*CH5_OffsetVal;
//                    SourceData.iCh6[g_iCarDist-1] = SourceData.iCH6[g_iCarDist-3] + 2*CH6_OffsetVal;

                    //g_iCarDist-1
                    CH1Buf = SourceData.iCH1Last + CH1_OffsetVal*2;
                    CH2Buf = SourceData.iCH2Last + CH2_OffsetVal*2;
                    CH3Buf = SourceData.iCH3Last + CH3_OffsetVal*2;
                    CH4Buf = SourceData.iCH4Last + CH4_OffsetVal*2;
                    CH5Buf = SourceData.iCH5Last + CH5_OffsetVal*2;
                    CH6Buf = SourceData.iCH6Last + CH6_OffsetVal*2;

                    SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                    SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                    SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                    SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                    SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                    SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                }
                else if(SourceData.iCH3[g_iCarDist-3] == 0){
                    if(SourceData.iCH3[g_iCarDist-4] != 0){//����������λ
                        
                        CH1_OffsetVal /= 4;
                        CH2_OffsetVal /= 4;
                        CH3_OffsetVal /= 4;
                        CH4_OffsetVal /= 4;
                        CH5_OffsetVal /= 4;
                        CH6_OffsetVal /= 4;
                        OffsetPartCnt = 4;
                        
//                        SourceData.iCH1[g_iCarDist-3] = SourceData.iCH1[g_iCarDist-4] + CH1_OffsetVal;
//                        SourceData.iCH2[g_iCarDist-3] = SourceData.iCH2[g_iCarDist-4] + CH2_OffsetVal;
//                        SourceData.iCH3[g_iCarDist-3] = SourceData.iCH3[g_iCarDist-4] + CH3_OffsetVal;
//                        SourceData.iCH4[g_iCarDist-3] = SourceData.iCH4[g_iCarDist-4] + CH4_OffsetVal;
//                        SourceData.iCH5[g_iCarDist-3] = SourceData.iCH5[g_iCarDist-4] + CH5_OffsetVal;
//                        SourceData.iCH6[g_iCarDist-3] = SourceData.iCH6[g_iCarDist-4] + CH6_OffsetVal;
                        
//                        SourceData.iCh1[g_iCarDist-3] = SourceData.iCH1[g_iCarDist-4] + CH1_OffsetVal;
//                        SourceData.iCh2[g_iCarDist-3] = SourceData.iCH2[g_iCarDist-4] + CH2_OffsetVal;
//                        SourceData.iCh3[g_iCarDist-3] = SourceData.iCH3[g_iCarDist-4] + CH3_OffsetVal;
//                        SourceData.iCh4[g_iCarDist-3] = SourceData.iCH4[g_iCarDist-4] + CH4_OffsetVal;
//                        SourceData.iCh5[g_iCarDist-3] = SourceData.iCH5[g_iCarDist-4] + CH5_OffsetVal;
//                        SourceData.iCh6[g_iCarDist-3] = SourceData.iCH6[g_iCarDist-4] + CH6_OffsetVal;

                        //g_iCarDist-3
                        CH1Buf = SourceData.iCH1Last + CH1_OffsetVal;
                        CH2Buf = SourceData.iCH2Last + CH2_OffsetVal;
                        CH3Buf = SourceData.iCH3Last + CH3_OffsetVal;
                        CH4Buf = SourceData.iCH4Last + CH4_OffsetVal;
                        CH5Buf = SourceData.iCH5Last + CH5_OffsetVal;
                        CH6Buf = SourceData.iCH6Last + CH6_OffsetVal;

                        SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                        SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                        SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                        SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                        SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                        SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                        
//                        SourceData.iCH1[g_iCarDist-2] = SourceData.iCH1[g_iCarDist-4] + 2*CH1_OffsetVal;
//                        SourceData.iCH2[g_iCarDist-2] = SourceData.iCH2[g_iCarDist-4] + 2*CH2_OffsetVal;
//                        SourceData.iCH3[g_iCarDist-2] = SourceData.iCH3[g_iCarDist-4] + 2*CH3_OffsetVal;
//                        SourceData.iCH4[g_iCarDist-2] = SourceData.iCH4[g_iCarDist-4] + 2*CH4_OffsetVal;
//                        SourceData.iCH5[g_iCarDist-2] = SourceData.iCH5[g_iCarDist-4] + 2*CH5_OffsetVal;
//                        SourceData.iCH6[g_iCarDist-2] = SourceData.iCH6[g_iCarDist-4] + 2*CH6_OffsetVal;
                        
//                        SourceData.iCh1[g_iCarDist-2] = SourceData.iCH1[g_iCarDist-4] + 2*CH1_OffsetVal;
//                        SourceData.iCh2[g_iCarDist-2] = SourceData.iCH2[g_iCarDist-4] + 2*CH2_OffsetVal;
//                        SourceData.iCh3[g_iCarDist-2] = SourceData.iCH3[g_iCarDist-4] + 2*CH3_OffsetVal;
//                        SourceData.iCh4[g_iCarDist-2] = SourceData.iCH4[g_iCarDist-4] + 2*CH4_OffsetVal;
//                        SourceData.iCh5[g_iCarDist-2] = SourceData.iCH5[g_iCarDist-4] + 2*CH5_OffsetVal;
//                        SourceData.iCh6[g_iCarDist-2] = SourceData.iCH6[g_iCarDist-4] + 2*CH6_OffsetVal;
                        
                        //g_iCarDist-2
                        CH1Buf = SourceData.iCH1Last + CH1_OffsetVal*2;
                        CH2Buf = SourceData.iCH2Last + CH2_OffsetVal*2;
                        CH3Buf = SourceData.iCH3Last + CH3_OffsetVal*2;
                        CH4Buf = SourceData.iCH4Last + CH4_OffsetVal*2;
                        CH5Buf = SourceData.iCH5Last + CH5_OffsetVal*2;
                        CH6Buf = SourceData.iCH6Last + CH6_OffsetVal*2;

                        SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                        SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                        SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                        SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                        SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                        SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                        
//                        SourceData.iCH1[g_iCarDist-1] = SourceData.iCH1[g_iCarDist-4] + 3*CH1_OffsetVal;
//                        SourceData.iCH2[g_iCarDist-1] = SourceData.iCH2[g_iCarDist-4] + 3*CH2_OffsetVal;
//                        SourceData.iCH3[g_iCarDist-1] = SourceData.iCH3[g_iCarDist-4] + 3*CH3_OffsetVal;
//                        SourceData.iCH4[g_iCarDist-1] = SourceData.iCH4[g_iCarDist-4] + 3*CH4_OffsetVal;
//                        SourceData.iCH5[g_iCarDist-1] = SourceData.iCH5[g_iCarDist-4] + 3*CH5_OffsetVal;
//                        SourceData.iCH6[g_iCarDist-1] = SourceData.iCH6[g_iCarDist-4] + 3*CH6_OffsetVal;
                        
//                        SourceData.iCh1[g_iCarDist-1] = SourceData.iCH1[g_iCarDist-4] + 3*CH1_OffsetVal;
//                        SourceData.iCh2[g_iCarDist-1] = SourceData.iCH2[g_iCarDist-4] + 3*CH2_OffsetVal;
//                        SourceData.iCh3[g_iCarDist-1] = SourceData.iCH3[g_iCarDist-4] + 3*CH3_OffsetVal;
//                        SourceData.iCh4[g_iCarDist-1] = SourceData.iCH4[g_iCarDist-4] + 3*CH4_OffsetVal;
//                        SourceData.iCh5[g_iCarDist-1] = SourceData.iCH5[g_iCarDist-4] + 3*CH5_OffsetVal;
//                        SourceData.iCh6[g_iCarDist-1] = SourceData.iCH6[g_iCarDist-4] + 3*CH6_OffsetVal;
                        
                        //g_iCarDist-1
                        CH1Buf = SourceData.iCH1Last + CH1_OffsetVal*3;
                        CH2Buf = SourceData.iCH2Last + CH2_OffsetVal*3;
                        CH3Buf = SourceData.iCH3Last + CH3_OffsetVal*3;
                        CH4Buf = SourceData.iCH4Last + CH4_OffsetVal*3;
                        CH5Buf = SourceData.iCH5Last + CH5_OffsetVal*3;
                        CH6Buf = SourceData.iCH6Last + CH6_OffsetVal*3;

                        SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                        SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                        SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                        SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                        SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                        SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                    }
                    else if(SourceData.iCH3[g_iCarDist-4] == 0){
                        if(SourceData.iCH3[g_iCarDist-5] != 0){//�����ĸ���λ
                            
                            CH1_OffsetVal /= 5;
                            CH2_OffsetVal /= 5;
                            CH3_OffsetVal /= 5;
                            CH4_OffsetVal /= 5;
                            CH5_OffsetVal /= 5;
                            CH6_OffsetVal /= 5;
                            OffsetPartCnt = 5;
                            
//                            SourceData.iCH1[g_iCarDist-4] = SourceData.iCH1[g_iCarDist-5] + CH1_OffsetVal;
//                            SourceData.iCH2[g_iCarDist-4] = SourceData.iCH2[g_iCarDist-5] + CH2_OffsetVal;
//                            SourceData.iCH3[g_iCarDist-4] = SourceData.iCH3[g_iCarDist-5] + CH3_OffsetVal;
//                            SourceData.iCH4[g_iCarDist-4] = SourceData.iCH4[g_iCarDist-5] + CH4_OffsetVal;
//                            SourceData.iCH5[g_iCarDist-4] = SourceData.iCH5[g_iCarDist-5] + CH5_OffsetVal;
//                            SourceData.iCH6[g_iCarDist-4] = SourceData.iCH6[g_iCarDist-5] + CH6_OffsetVal;
                            
//                            SourceData.iCh1[g_iCarDist-4] = SourceData.iCH1[g_iCarDist-5] + CH1_OffsetVal;
//                            SourceData.iCh2[g_iCarDist-4] = SourceData.iCH2[g_iCarDist-5] + CH2_OffsetVal;
//                            SourceData.iCh3[g_iCarDist-4] = SourceData.iCH3[g_iCarDist-5] + CH3_OffsetVal;
//                            SourceData.iCh4[g_iCarDist-4] = SourceData.iCH4[g_iCarDist-5] + CH4_OffsetVal;
//                            SourceData.iCh5[g_iCarDist-4] = SourceData.iCH5[g_iCarDist-5] + CH5_OffsetVal;
//                            SourceData.iCh6[g_iCarDist-4] = SourceData.iCH6[g_iCarDist-5] + CH6_OffsetVal;
                            
                            //g_iCarDist-4
                            CH1Buf = SourceData.iCH1Last + CH1_OffsetVal;
                            CH2Buf = SourceData.iCH2Last + CH2_OffsetVal;
                            CH3Buf = SourceData.iCH3Last + CH3_OffsetVal;
                            CH4Buf = SourceData.iCH4Last + CH4_OffsetVal;
                            CH5Buf = SourceData.iCH5Last + CH5_OffsetVal;
                            CH6Buf = SourceData.iCH6Last + CH6_OffsetVal;

                            SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                            SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                            SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                            SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                            SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                            SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                            
//                            SourceData.iCH1[g_iCarDist-3] = SourceData.iCH1[g_iCarDist-5] + 2*CH1_OffsetVal;
//                            SourceData.iCH2[g_iCarDist-3] = SourceData.iCH2[g_iCarDist-5] + 2*CH2_OffsetVal;
//                            SourceData.iCH3[g_iCarDist-3] = SourceData.iCH3[g_iCarDist-5] + 2*CH3_OffsetVal;
//                            SourceData.iCH4[g_iCarDist-3] = SourceData.iCH4[g_iCarDist-5] + 2*CH4_OffsetVal;
//                            SourceData.iCH5[g_iCarDist-3] = SourceData.iCH5[g_iCarDist-5] + 2*CH5_OffsetVal;
//                            SourceData.iCH6[g_iCarDist-3] = SourceData.iCH6[g_iCarDist-5] + 2*CH6_OffsetVal;
                            
//                            SourceData.iCh1[g_iCarDist-3] = SourceData.iCH1[g_iCarDist-5] + 2*CH1_OffsetVal;
//                            SourceData.iCh2[g_iCarDist-3] = SourceData.iCH2[g_iCarDist-5] + 2*CH2_OffsetVal;
//                            SourceData.iCh3[g_iCarDist-3] = SourceData.iCH3[g_iCarDist-5] + 2*CH3_OffsetVal;
//                            SourceData.iCh4[g_iCarDist-3] = SourceData.iCH4[g_iCarDist-5] + 2*CH4_OffsetVal;
//                            SourceData.iCh5[g_iCarDist-3] = SourceData.iCH5[g_iCarDist-5] + 2*CH5_OffsetVal;
//                            SourceData.iCh6[g_iCarDist-3] = SourceData.iCH6[g_iCarDist-5] + 2*CH6_OffsetVal;
                            
                            //g_iCarDist-3
                            CH1Buf = SourceData.iCH1Last + CH1_OffsetVal*2;
                            CH2Buf = SourceData.iCH2Last + CH2_OffsetVal*2;
                            CH3Buf = SourceData.iCH3Last + CH3_OffsetVal*2;
                            CH4Buf = SourceData.iCH4Last + CH4_OffsetVal*2;
                            CH5Buf = SourceData.iCH5Last + CH5_OffsetVal*2;
                            CH6Buf = SourceData.iCH6Last + CH6_OffsetVal*2;

                            SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                            SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                            SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                            SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                            SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                            SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                            
//                            SourceData.iCH1[g_iCarDist-2] = SourceData.iCH1[g_iCarDist-5] + 3*CH1_OffsetVal;
//                            SourceData.iCH2[g_iCarDist-2] = SourceData.iCH2[g_iCarDist-5] + 3*CH2_OffsetVal;
//                            SourceData.iCH3[g_iCarDist-2] = SourceData.iCH3[g_iCarDist-5] + 3*CH3_OffsetVal;
//                            SourceData.iCH4[g_iCarDist-2] = SourceData.iCH4[g_iCarDist-5] + 3*CH4_OffsetVal;
//                            SourceData.iCH5[g_iCarDist-2] = SourceData.iCH5[g_iCarDist-5] + 3*CH5_OffsetVal;
//                            SourceData.iCH6[g_iCarDist-2] = SourceData.iCH6[g_iCarDist-5] + 3*CH6_OffsetVal;
                            
//                            SourceData.iCh1[g_iCarDist-2] = SourceData.iCH1[g_iCarDist-5] + 3*CH1_OffsetVal;
//                            SourceData.iCh2[g_iCarDist-2] = SourceData.iCH2[g_iCarDist-5] + 3*CH2_OffsetVal;
//                            SourceData.iCh3[g_iCarDist-2] = SourceData.iCH3[g_iCarDist-5] + 3*CH3_OffsetVal;
//                            SourceData.iCh4[g_iCarDist-2] = SourceData.iCH4[g_iCarDist-5] + 3*CH4_OffsetVal;
//                            SourceData.iCh5[g_iCarDist-2] = SourceData.iCH5[g_iCarDist-5] + 3*CH5_OffsetVal;
//                            SourceData.iCh6[g_iCarDist-2] = SourceData.iCH6[g_iCarDist-5] + 3*CH6_OffsetVal;
                            
                            //g_iCarDist-2
                            CH1Buf = SourceData.iCH1Last + CH1_OffsetVal*3;
                            CH2Buf = SourceData.iCH2Last + CH2_OffsetVal*3;
                            CH3Buf = SourceData.iCH3Last + CH3_OffsetVal*3;
                            CH4Buf = SourceData.iCH4Last + CH4_OffsetVal*3;
                            CH5Buf = SourceData.iCH5Last + CH5_OffsetVal*3;
                            CH6Buf = SourceData.iCH6Last + CH6_OffsetVal*3;

                            SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                            SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                            SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                            SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                            SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                            SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                            
//                            SourceData.iCH1[g_iCarDist-1] = SourceData.iCH1[g_iCarDist-5] + 4*CH1_OffsetVal;
//                            SourceData.iCH2[g_iCarDist-1] = SourceData.iCH2[g_iCarDist-5] + 4*CH2_OffsetVal;
//                            SourceData.iCH3[g_iCarDist-1] = SourceData.iCH3[g_iCarDist-5] + 4*CH3_OffsetVal;
//                            SourceData.iCH4[g_iCarDist-1] = SourceData.iCH4[g_iCarDist-5] + 4*CH4_OffsetVal;
//                            SourceData.iCH5[g_iCarDist-1] = SourceData.iCH5[g_iCarDist-5] + 4*CH5_OffsetVal;
//                            SourceData.iCH6[g_iCarDist-1] = SourceData.iCH6[g_iCarDist-5] + 4*CH6_OffsetVal;
                            
//                            SourceData.iCh1[g_iCarDist-1] = SourceData.iCH1[g_iCarDist-5] + 4*CH1_OffsetVal;
//                            SourceData.iCh2[g_iCarDist-1] = SourceData.iCH2[g_iCarDist-5] + 4*CH2_OffsetVal;
//                            SourceData.iCh3[g_iCarDist-1] = SourceData.iCH3[g_iCarDist-5] + 4*CH3_OffsetVal;
//                            SourceData.iCh4[g_iCarDist-1] = SourceData.iCH4[g_iCarDist-5] + 4*CH4_OffsetVal;
//                            SourceData.iCh5[g_iCarDist-1] = SourceData.iCH5[g_iCarDist-5] + 4*CH5_OffsetVal;
//                            SourceData.iCh6[g_iCarDist-1] = SourceData.iCH6[g_iCarDist-5] + 4*CH6_OffsetVal;
                            
                            //g_iCarDist-1
                            CH1Buf = SourceData.iCH1Last + CH1_OffsetVal*4;
                            CH2Buf = SourceData.iCH2Last + CH2_OffsetVal*4;
                            CH3Buf = SourceData.iCH3Last + CH3_OffsetVal*4;
                            CH4Buf = SourceData.iCH4Last + CH4_OffsetVal*4;
                            CH5Buf = SourceData.iCH5Last + CH5_OffsetVal*4;
                            CH6Buf = SourceData.iCH6Last + CH6_OffsetVal*4;

                            SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                            SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                            SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                            SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                            SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                            SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                        }
                        else if(SourceData.iCH3[g_iCarDist-5] == 0){
                            if(SourceData.iCH3[g_iCarDist-6] != 0){//����������λ
                                
                                CH1_OffsetVal /= 6;
                                CH2_OffsetVal /= 6;
                                CH3_OffsetVal /= 6;
                                CH4_OffsetVal /= 6;
                                CH5_OffsetVal /= 6;
                                CH6_OffsetVal /= 6;
                                OffsetPartCnt = 6;
                                
//                                SourceData.iCH1[g_iCarDist-5] = SourceData.iCH1[g_iCarDist-6] + CH1_OffsetVal;
//                                SourceData.iCH2[g_iCarDist-5] = SourceData.iCH2[g_iCarDist-6] + CH2_OffsetVal;
//                                SourceData.iCH3[g_iCarDist-5] = SourceData.iCH3[g_iCarDist-6] + CH3_OffsetVal;
//                                SourceData.iCH4[g_iCarDist-5] = SourceData.iCH4[g_iCarDist-6] + CH4_OffsetVal;
//                                SourceData.iCH5[g_iCarDist-5] = SourceData.iCH5[g_iCarDist-6] + CH5_OffsetVal;
//                                SourceData.iCH6[g_iCarDist-5] = SourceData.iCH6[g_iCarDist-6] + CH6_OffsetVal;
                                
                                //g_iCarDist-5
                                CH1Buf = SourceData.iCH1Last + CH1_OffsetVal;
                                CH2Buf = SourceData.iCH2Last + CH2_OffsetVal;
                                CH3Buf = SourceData.iCH3Last + CH3_OffsetVal;
                                CH4Buf = SourceData.iCH4Last + CH4_OffsetVal;
                                CH5Buf = SourceData.iCH5Last + CH5_OffsetVal;
                                CH6Buf = SourceData.iCH6Last + CH6_OffsetVal;

                                SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                                SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                                SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                                SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                                SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                                SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                                
//                                SourceData.iCH1[g_iCarDist-4] = SourceData.iCH1[g_iCarDist-6] + 2*CH1_OffsetVal;
//                                SourceData.iCH2[g_iCarDist-4] = SourceData.iCH2[g_iCarDist-6] + 2*CH2_OffsetVal;
//                                SourceData.iCH3[g_iCarDist-4] = SourceData.iCH3[g_iCarDist-6] + 2*CH3_OffsetVal;
//                                SourceData.iCH4[g_iCarDist-4] = SourceData.iCH4[g_iCarDist-6] + 2*CH4_OffsetVal;
//                                SourceData.iCH5[g_iCarDist-4] = SourceData.iCH5[g_iCarDist-6] + 2*CH5_OffsetVal;
//                                SourceData.iCH6[g_iCarDist-4] = SourceData.iCH6[g_iCarDist-6] + 2*CH6_OffsetVal;
                                
                                //g_iCarDist-4
                                CH1Buf = SourceData.iCH1Last + CH1_OffsetVal*2;
                                CH2Buf = SourceData.iCH2Last + CH2_OffsetVal*2;
                                CH3Buf = SourceData.iCH3Last + CH3_OffsetVal*2;
                                CH4Buf = SourceData.iCH4Last + CH4_OffsetVal*2;
                                CH5Buf = SourceData.iCH5Last + CH5_OffsetVal*2;
                                CH6Buf = SourceData.iCH6Last + CH6_OffsetVal*2;

                                SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                                SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                                SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                                SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                                SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                                SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                                
//                                SourceData.iCH1[g_iCarDist-3] = SourceData.iCH1[g_iCarDist-6] + 3*CH1_OffsetVal;
//                                SourceData.iCH2[g_iCarDist-3] = SourceData.iCH2[g_iCarDist-6] + 3*CH2_OffsetVal;
//                                SourceData.iCH3[g_iCarDist-3] = SourceData.iCH3[g_iCarDist-6] + 3*CH3_OffsetVal;
//                                SourceData.iCH4[g_iCarDist-3] = SourceData.iCH4[g_iCarDist-6] + 3*CH4_OffsetVal;
//                                SourceData.iCH5[g_iCarDist-3] = SourceData.iCH5[g_iCarDist-6] + 3*CH5_OffsetVal;
//                                SourceData.iCH6[g_iCarDist-3] = SourceData.iCH6[g_iCarDist-6] + 3*CH6_OffsetVal;
                                
                                //g_iCarDist-3
                                CH1Buf = SourceData.iCH1Last + CH1_OffsetVal*3;
                                CH2Buf = SourceData.iCH2Last + CH2_OffsetVal*3;
                                CH3Buf = SourceData.iCH3Last + CH3_OffsetVal*3;
                                CH4Buf = SourceData.iCH4Last + CH4_OffsetVal*3;
                                CH5Buf = SourceData.iCH5Last + CH5_OffsetVal*3;
                                CH6Buf = SourceData.iCH6Last + CH6_OffsetVal*3;

                                SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                                SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                                SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                                SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                                SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                                SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                                
//                                SourceData.iCH1[g_iCarDist-2] = SourceData.iCH1[g_iCarDist-6] + 4*CH1_OffsetVal;
//                                SourceData.iCH2[g_iCarDist-2] = SourceData.iCH2[g_iCarDist-6] + 4*CH2_OffsetVal;
//                                SourceData.iCH3[g_iCarDist-2] = SourceData.iCH3[g_iCarDist-6] + 4*CH3_OffsetVal;
//                                SourceData.iCH4[g_iCarDist-2] = SourceData.iCH4[g_iCarDist-6] + 4*CH4_OffsetVal;
//                                SourceData.iCH5[g_iCarDist-2] = SourceData.iCH5[g_iCarDist-6] + 4*CH5_OffsetVal;
//                                SourceData.iCH6[g_iCarDist-2] = SourceData.iCH6[g_iCarDist-6] + 4*CH6_OffsetVal;
                                
                                //g_iCarDist-2
                                CH1Buf = SourceData.iCH1Last + CH1_OffsetVal*4;
                                CH2Buf = SourceData.iCH2Last + CH2_OffsetVal*4;
                                CH3Buf = SourceData.iCH3Last + CH3_OffsetVal*4;
                                CH4Buf = SourceData.iCH4Last + CH4_OffsetVal*4;
                                CH5Buf = SourceData.iCH5Last + CH5_OffsetVal*4;
                                CH6Buf = SourceData.iCH6Last + CH6_OffsetVal*4;

                                SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                                SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                                SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                                SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                                SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                                SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                                
//                                SourceData.iCH1[g_iCarDist-1] = SourceData.iCH1[g_iCarDist-6] + 5*CH1_OffsetVal;
//                                SourceData.iCH2[g_iCarDist-1] = SourceData.iCH2[g_iCarDist-6] + 5*CH2_OffsetVal;
//                                SourceData.iCH3[g_iCarDist-1] = SourceData.iCH3[g_iCarDist-6] + 5*CH3_OffsetVal;
//                                SourceData.iCH4[g_iCarDist-1] = SourceData.iCH4[g_iCarDist-6] + 5*CH4_OffsetVal;
//                                SourceData.iCH5[g_iCarDist-1] = SourceData.iCH5[g_iCarDist-6] + 5*CH5_OffsetVal;
//                                SourceData.iCH6[g_iCarDist-1] = SourceData.iCH6[g_iCarDist-6] + 5*CH6_OffsetVal;
                                
                                //g_iCarDist-1
                                CH1Buf = SourceData.iCH1Last + CH1_OffsetVal*5;
                                CH2Buf = SourceData.iCH2Last + CH2_OffsetVal*5;
                                CH3Buf = SourceData.iCH3Last + CH3_OffsetVal*5;
                                CH4Buf = SourceData.iCH4Last + CH4_OffsetVal*5;
                                CH5Buf = SourceData.iCH5Last + CH5_OffsetVal*5;
                                CH6Buf = SourceData.iCH6Last + CH6_OffsetVal*5;

                                SourceBufAddData(CH1SUM,CH1Buf+CH2Buf);
                                SourceBufAddData(CH2SUM,CH3Buf+CH4Buf);
                                SourceBufAddData(CH3SUM,CH5Buf+CH6Buf);
                                SourceBufAddData(CH1SUB,CH2Buf-CH1Buf);
                                SourceBufAddData(CH2SUB,CH4Buf-CH3Buf);
                                SourceBufAddData(CH3SUB,CH6Buf-CH5Buf);
                            }
                        }
                    }
                }
            }
        }
    }
    
    SourceBufAddData(CH1SUM, AdcDataSource.iCh1 + AdcDataSource.iCh2);
    SourceBufAddData(CH2SUM, AdcDataSource.iCh3 + AdcDataSource.iCh4);
    SourceBufAddData(CH3SUM, AdcDataSource.iCh5 + AdcDataSource.iCh6);
    SourceBufAddData(CH1SUB, AdcDataSource.iCh2 - AdcDataSource.iCh1);
    SourceBufAddData(CH2SUB, AdcDataSource.iCh4 - AdcDataSource.iCh3);
    SourceBufAddData(CH3SUB, AdcDataSource.iCh6 - AdcDataSource.iCh5);
    
//    SourceData.iCh1[g_iCarDist] = AdcDataSource.iCh1;
//    SourceData.iCh2[g_iCarDist] = AdcDataSource.iCh2;
//    SourceData.iCh3[g_iCarDist] = AdcDataSource.iCh3;
//    SourceData.iCh4[g_iCarDist] = AdcDataSource.iCh4;
//    SourceData.iCh5[g_iCarDist] = AdcDataSource.iCh5;
//    SourceData.iCh6[g_iCarDist] = AdcDataSource.iCh6;

    if(OffsetPartCnt == 0){
        SourceData.iCH1[g_iCarDist] = AdcDataSource.iCh1 - SourceData.iCH1Last;
        SourceData.iCH2[g_iCarDist] = AdcDataSource.iCh2 - SourceData.iCH2Last;
        SourceData.iCH3[g_iCarDist] = AdcDataSource.iCh3 - SourceData.iCH3Last;
        SourceData.iCH4[g_iCarDist] = AdcDataSource.iCh4 - SourceData.iCH4Last;
        SourceData.iCH5[g_iCarDist] = AdcDataSource.iCh5 - SourceData.iCH5Last;
        SourceData.iCH6[g_iCarDist] = AdcDataSource.iCh6 - SourceData.iCH6Last;
    }
    else{
        for(i=0;i<OffsetPartCnt;i++){
            SourceData.iCH1[g_iCarDist-i] = CH1_OffsetVal;
            SourceData.iCH2[g_iCarDist-i] = CH2_OffsetVal;
            SourceData.iCH3[g_iCarDist-i] = CH3_OffsetVal;
            SourceData.iCH4[g_iCarDist-i] = CH4_OffsetVal;
            SourceData.iCH5[g_iCarDist-i] = CH5_OffsetVal;
            SourceData.iCH6[g_iCarDist-i] = CH6_OffsetVal;
        }
    }
    
    SourceData.iCH1Last = AdcDataSource.iCh1;
    SourceData.iCH2Last = AdcDataSource.iCh2;
    SourceData.iCH3Last = AdcDataSource.iCh3;
    SourceData.iCH4Last = AdcDataSource.iCh4;
    SourceData.iCH5Last = AdcDataSource.iCh5;
    SourceData.iCH6Last = AdcDataSource.iCh6;
}


void FilterHandler_Sub_Dif(short int index, short *SourceBufLeft, short *SourceBufRight, AIMREBAR_BUF_STR *AimBuf)
{
    int uiAvg = 0;
    int uiAvg1 = 0;
    char i = 0;
//    char j = 0;
//    char dataBuf[10];
    int iCurCnt = 0;
    int *SubAimBuf = &AimBuf->SubDataBuf_Mark[0];//��ֵ��λ���ݻ���ָ�븳ֵ
    int *DifAimBuf = &AimBuf->DifDataBuf_Mark[0];//΢�ֶ�λ���ݻ���ָ�븳ֵ
    int *SecDifAimBuf = &AimBuf->SecDifDataBuf_Mark[0];//����΢�ֶ�λ���ݻ���ָ�븳ֵ
    int SubBuf[15] = {0};
    
    if(index < 6){
        return;
    }
    

    iCurCnt = index/6*6;
    
    if(SourceBufLeft == SourceData.iCH1){
        SourceBufGetData(CH1SUB, SubBuf, index-iCurCnt+1, 6);
    }
    else if(SourceBufLeft == SourceData.iCH3){
        SourceBufGetData(CH2SUB, SubBuf, index-iCurCnt+1, 6);
    }
    else if(SourceBufLeft == SourceData.iCH5){
        SourceBufGetData(CH3SUB, SubBuf, index-iCurCnt+1, 6);
    }

    //�����ֵ��λ���桢΢�ֶ�λ����
    uiAvg = (int)((SubBuf[0]+SubBuf[1]+SubBuf[2]+SubBuf[3]+SubBuf[4]+SubBuf[5])/6);
    
    //��仺������
    for(i=0;i<4;i++){
        DifAimBuf[i] = DifAimBuf[i+1];
        SubAimBuf[i] = SubAimBuf[i+1];
        SecDifAimBuf[i] = SecDifAimBuf[i+1];
    }
    
    SubAimBuf[4] = uiAvg;//��ֵ����
    DifAimBuf[4] = uiAvg - SubAimBuf[3];//΢�ֻ���
    SecDifAimBuf[4] = DifAimBuf[4] - DifAimBuf[3];//����΢�ֻ���

    //����΢�ֲ��Σ������������½���mark���ݸ���
    if(DifAimBuf[4] >= DifAimBuf[3]){
        if(AimBuf->DifWaveDirCnt[7] >= 0){
            AimBuf->DifWaveDirCnt[7] ++;
        }
        else{
            for(i=0;i<7;i++){
                AimBuf->DifWaveDirCnt[i] = AimBuf->DifWaveDirCnt[i+1];//��ǰ��
            }
            AimBuf->DifWaveDirCnt[7] = 1;
        }
    }
    else{
        if(AimBuf->DifWaveDirCnt[7] <= 0){
            AimBuf->DifWaveDirCnt[7] --;
        }
        else{
            for(i=0;i<7;i++){
                AimBuf->DifWaveDirCnt[i] = AimBuf->DifWaveDirCnt[i+1];
            }
            AimBuf->DifWaveDirCnt[7] = -1;
        }
    }
    
    //�������΢�ֲ��Σ������������½���mark���ݸ���
    if(SecDifAimBuf[4] >= SecDifAimBuf[3]){
        if(AimBuf->SecDifWaveDirCnt[7] >= 0){
            AimBuf->SecDifWaveDirCnt[7] ++;
        }
        else{
            for(i=0;i<7;i++){
                AimBuf->SecDifWaveDirCnt[i] = AimBuf->SecDifWaveDirCnt[i+1];
            }
            AimBuf->SecDifWaveDirCnt[7] = 1;
        }
    }
    else{
        if(AimBuf->SecDifWaveDirCnt[7] <= 0){
            AimBuf->SecDifWaveDirCnt[7] --;
        }
        else{
            for(i=0;i<7;i++){
                AimBuf->SecDifWaveDirCnt[i] = AimBuf->SecDifWaveDirCnt[i+1];
            }
            AimBuf->SecDifWaveDirCnt[7] = -1;
        }
    }
    
    if(iCurCnt == 6){
        DifAimBuf[4] = 0;
        SecDifAimBuf[4] = 0;
    }
}


//void FilterHandler_Sum(short int index, int *SourceBuf, int *SumAimBuf, int *SumMakrBuf, int *SumMakrBuf2)
void FilterHandler_Sum(short int index, short *SourceBufLeft, short *SourceBufRight, int *SumAimBuf)
{
    int uiAvg = 0;
    int uiAvg1 = 0;
    char i = 0;
    int SumBuf[15] = {0};
//    char dataBuf[10];
    int iCurCnt = 0;
    
    if(index < 6){
        return;
    }
    
    //һ���ͺ��˲�
//    SourceBuf[index] = (unsigned int)(SourceBuf[index]*0.9+SourceBuf[index-1]*0.1);
    
    //��ֵ�����˲�
//    SourceBuf[index] = (SourceBuf[index]+SourceBuf[index-1]+SourceBuf[index-2]
//        +SourceBuf[index-3]+SourceBuf[index-4])/5;

    //���Ͳ���Ƶ��
    iCurCnt = index/6*6;
    
    if(SourceBufLeft == SourceData.iCH1){
        SourceBufGetData(CH1SUM, SumBuf, index-iCurCnt+1, 6);
    }
    else if(SourceBufLeft == SourceData.iCH3){
        SourceBufGetData(CH2SUM, SumBuf, index-iCurCnt+1, 6);
    }
    else if(SourceBufLeft == SourceData.iCH5){
        SourceBufGetData(CH3SUM, SumBuf, index-iCurCnt+1, 6);
    }

    //�����ֵ��λ���桢΢�ֶ�λ����
    uiAvg = (int)((SumBuf[0]+SumBuf[1]+SumBuf[2]+SumBuf[3]+SumBuf[4]+SumBuf[5])/6);

//    uiAvg1 = (int)((SourceBufLeft[iCurCnt] + SourceBufLeft[iCurCnt-1] + SourceBufLeft[iCurCnt-2] + SourceBufLeft[iCurCnt-3]
//        + SourceBufLeft[iCurCnt-4] + SourceBufLeft[iCurCnt-5]+SourceBufRight[iCurCnt] + SourceBufRight[iCurCnt-1] 
//        + SourceBufRight[iCurCnt-2]+ SourceBufRight[iCurCnt-3] + SourceBufRight[iCurCnt-4]+ SourceBufRight[iCurCnt-5])/6);
    
//        iCurCnt = index/6*6;
//        for(i=0;i<5;i++){
//            SumMakrBuf[iCurCnt-5+i] = SumAimBuf[i];
//        }

//        for(i=0;i<6;i++){
//            SumMakrBuf2[iCurCnt-5+i] = uiAvg;
//        }

    //Ӧ��������ͬ�ź��������������泤�ȵ����
    if((SumAimBuf[4]==SumAimBuf[3])&&(SumAimBuf[3]==SumAimBuf[2])&&(SumAimBuf[4]==uiAvg)){
        return;
    }

    //�ų����������г��ֵ�С����
//        if((SumAimBuf[4]<=SumAimBuf[3])&&(SumAimBuf[3]>=SumAimBuf[2])&&(SumAimBuf[2]>=SumAimBuf[1])
//            &&(SumAimBuf[1]>=SumAimBuf[0])&&(SumAimBuf[3] > uiAvg)&&((SumAimBuf[3] - uiAvg) < 5)){
//            return;
//        }
    
    for(i=0;i<4;i++){
        SumAimBuf[i] = SumAimBuf[i+1];
    }
    SumAimBuf[4] = uiAvg;
}

//================================================================//
//���ܣ��ź��˲�������ԭʼ���ݽ����˲�
//������NULL
//���أ�NULL
//================================================================//
void AdcFilter(void)
{
    if(g_iCarDist > 2){
//        FilterHandler_Sub_Dif(g_iCarDist-1, &SourceData.iCh2Sub[0], &AimRebarBuf_CH2.SubDataBuf_Mark[0], &AimRebarBuf_CH2.DifDataBuf_Mark[0],
//            &SourceData.iCh2SubMark[0], &SourceData.iCh2DifMark[0], &AimRebarBuf_CH2, &SourceData.iCh2DifWaveCnt[0]);//�����ֵ��������
//        FilterHandler_Sub_Dif(g_iCarDist-1, &SourceData.iCh1Sub[0], &AimRebarBuf_CH1.SubDataBuf_Mark[0], &AimRebarBuf_CH1.DifDataBuf_Mark[0],
//            &SourceData.iCh1SubMark[0], &SourceData.iCh1DifMark[0], &AimRebarBuf_CH1, &SourceData.iCh1DifWaveCnt[0]);//�����ֵ��������
//        FilterHandler_Sub_Dif(g_iCarDist-1, &SourceData.iCh3Sub[0], &AimRebarBuf_CH3.SubDataBuf_Mark[0], &AimRebarBuf_CH3.DifDataBuf_Mark[0], 
//            &SourceData.iCh3SubMark[0], &SourceData.iCh3DifMark[0], &AimRebarBuf_CH3, &SourceData.iCh3DifWaveCnt[0]);//�����ֵ��������
        
        FilterHandler_Sub_Dif(g_iCarDist-1, &SourceData.iCH3[0], &SourceData.iCH4[0], &AimRebarBuf_CH2);//�����ֵ��������
        FilterHandler_Sub_Dif(g_iCarDist-1, &SourceData.iCH1[0], &SourceData.iCH2[0], &AimRebarBuf_CH1);//�����ֵ��������
        FilterHandler_Sub_Dif(g_iCarDist-1, &SourceData.iCH5[0], &SourceData.iCH6[0], &AimRebarBuf_CH3);//�����ֵ��������
        
//        FilterHandler_Sum(g_iCarDist-1, &SourceData.iCh2Sum[0], &AimRebarBuf_CH2.SumDataBuf_Mark[0],&SourceData.iCh2SumMark[0], &SourceData.iCh2SumMark2[0]);//�����ֵ��������
//        FilterHandler_Sum(g_iCarDist-1, &SourceData.iCh1Sum[0], &AimRebarBuf_CH1.SumDataBuf_Mark[0],&SourceData.iCh1SumMark[0], &SourceData.iCh1SumMark2[0]);//�����ֵ��������
//        FilterHandler_Sum(g_iCarDist-1, &SourceData.iCh3Sum[0], &AimRebarBuf_CH3.SumDataBuf_Mark[0],&SourceData.iCh3SumMark[0], &SourceData.iCh3SumMark2[0]);//�����ֵ��������
        FilterHandler_Sum(g_iCarDist-1, &SourceData.iCH3[0], &SourceData.iCH4[0], &AimRebarBuf_CH2.SumDataBuf_Mark[0]);//�����ֵ��������
        FilterHandler_Sum(g_iCarDist-1, &SourceData.iCH1[0], &SourceData.iCH2[0], &AimRebarBuf_CH1.SumDataBuf_Mark[0]);//�����ֵ��������
        FilterHandler_Sum(g_iCarDist-1, &SourceData.iCH5[0], &SourceData.iCH6[0], &AimRebarBuf_CH3.SumDataBuf_Mark[0]);//�����ֵ��������
    }
}

//================================================================//
//���ܣ�ͨ����ֵ���ݶ�λ�ֽ��㷨
//������NULL
//���أ�NULL
//================================================================//
void CountSumBuffer(enum CHANNELNUM Channel, unsigned int iSearchIndex, AIMREBAR_PARA_STR *AimPara, COVER_BUFFER *Res, AIMREBAR_BUF_STR *AimBuf)
{
//    int i=0;
    int iIndex=0;
//    char dataBuf[10];
    int JudgeRange = 0;

    if(g_iCarDist <5){
        return;
    }
    
    int SumValCur = 0;
    
    if(Channel == CH1){
        SumValCur = SourceData.iCH1[iSearchIndex-15]+SourceData.iCH2[iSearchIndex-15];
    }
    else if(Channel == CH2){
        SumValCur = SourceData.iCH3[iSearchIndex-15]+SourceData.iCH4[iSearchIndex-15];
    }
    else if(Channel == CH3){
        SumValCur = SourceData.iCH5[iSearchIndex-15]+SourceData.iCH6[iSearchIndex-15];
    }

    //�ж����塢���Ȳ����뻺��
    if(iSearchIndex > 30){
        //�жϲ��岢���뻺��      ����ֵתλ�ƾ���66*120/400Լ20mm
        if((AimBuf->SumDataBuf_Mark[4] == 0)||((AimBuf->SumDataBuf_Mark[4] != 0)&&(iSearchIndex - 12 - AimBuf->SumDataBuf_Mark[4])>20)){
            //���η�ֵ�ж�
            if((AimBuf->SumDataBuf_Mark[4] <= AimBuf->SumDataBuf_Mark[3])&&(AimBuf->SumDataBuf_Mark[3] <= AimBuf->SumDataBuf_Mark[2])
                &&(AimBuf->SumDataBuf_Mark[2] >= AimBuf->SumDataBuf_Mark[1])&&(AimBuf->SumDataBuf_Mark[1] >= AimBuf->SumDataBuf_Mark[0])
                    &&(AimBuf->SumDataBuf_Mark[2] > (AimBuf->SumDataBuf_Mark[4]+JudgeRange))&&(AimBuf->SumDataBuf_Mark[2] > (AimBuf->SumDataBuf_Mark[0]+JudgeRange))){

                //���º�ֵ�ֽ�λ������
                AimBuf->SumBoFengSite[0] = AimBuf->SumBoFengSite[1];
                AimBuf->SumBoFengSite[1] = AimBuf->SumBoFengSite[2];
                AimBuf->SumBoFengSite[2] = AimBuf->SumBoFengSite[3];
                AimBuf->SumBoFengSite[3] = AimBuf->SumBoFengSite[4];
                AimBuf->SumBoFengSite[4] = iSearchIndex - 15 - SITE_OFFSET;

                //���º�ֵ�ֽ��ź�����
                AimBuf->SumBoFengVal[0] = AimBuf->SumBoFengVal[1];
                AimBuf->SumBoFengVal[1] = AimBuf->SumBoFengVal[2];
                AimBuf->SumBoFengVal[2] = AimBuf->SumBoFengVal[3];
                AimBuf->SumBoFengVal[3] = AimBuf->SumBoFengVal[4];
                AimBuf->SumBoFengVal[4] = AimBuf->SumDataBuf_Mark[2];//��

                //���㲢��ʾ���ֵ
                iSumMaxHD = FindCoverTab_Sum(Channel, AimBuf->SumDataBuf_Mark[2], g_tWorkPara, AimBuf);//��ֵ��������
                
                //����������
                AimBuf->LastSumRebarHD = (iSumMaxHD+5)/10;
                AimBuf->LastSumRebarSite = iSearchIndex - 15;
                AimBuf->LastSumRebarVal = AimBuf->SumDataBuf_Mark[2];

                //�ж������Ƿ���Ч
                if(((AimBuf->SumBoFengSite[4]-AimBuf->SumBoFengSite[3]>100)||(AimBuf->SumBoFengSite[3] == 0))
                    ||(((AimBuf->SumBoFengVal[4]-AimBuf->SumBoGuVal[4])>10)&&((AimBuf->SumBoFengVal[3]-AimBuf->SumBoGuVal[4]) > 10))){

                    if(Res->AllRebarCnt == 0){
                        iIndex = 0;
                    }
                    else{
                        iIndex = Res->AllRebarCnt-1;
                    }
                    
                    //Sum����ǰ��Sub����򲨷�ֵС��15ֱ�ӷ��뻺��  ���ƾ�ֵ�ֽ����1/3
//////                    if((AimBuf->SubBoFengSite[4] <= Res->RebarSite[iIndex])||(AimBuf->SubBoFengVal[4] < 20)){
                    if((AimBuf->SubBoFengSite[4] <= Res->RebarSite[iIndex])){
                       if((abs(AimBuf->LastSumRebarSite - Res->RebarSite[iIndex]) > 70)//�����ж�һ��λ���ڲ����������򽵵ĺ���
                           &&(SumValCur > 25)){
                            //����⵽�ĸֽ���뻺��
                            Res->RebarSite[Res->AllRebarCnt] = iSearchIndex - 15;
                            Res->RebarCover[Res->AllRebarCnt] = (iSumMaxHD+5)/10;

                            Res->AllRebarCnt += 1;
                            Res->AddRebatCnt = 1;
                            
                            //������ʷ����
                            AimBuf->iLastSubMinVal = AimPara->iSubMinVal;
                            AimBuf->iLastSubMinSite = AimPara->iSubMinSite;
                            AimBuf->iLastSubMaxVal = AimPara->iSubMaxVal;
                            AimBuf->iLastSubMaxSite = AimPara->iSubMaxSite;
                            
                            if(Channel == CH1){
                                InitAimRebarParaCH1();
                            }
                            else if(Channel == CH2){
                                InitAimRebarParaCH2();
                            }
                            else if(Channel == CH3){
                                InitAimRebarParaCH3();
                            }
//////                            #ifdef TEST_ZJC
//////                            if(Channel == CH1){
//////                                LCD_SetColor(WHITE,PURPLE);
//////                                sprintf(dataBuf,"SumCH1:%-6d, Sub:%-6d, LS:%-3d, S:%-3d   ", (iSumMaxHD+5)/10, AimBuf->SubBoFengSite[4], Res->RebarSite[iIndex],AimBuf->LastSumRebarSite);
//////                                LCD_ShowString(ASC16x8,0, 240, 0,dataBuf);
//////                            }
//////                            else if(Channel == CH2){
//////                                LCD_SetColor(WHITE,PURPLE);
//////                                sprintf(dataBuf,"SumCH2:%-6d, Sub:%-6d, LS:%-3d, S:%-3d   ", (iSumMaxHD+5)/10, AimBuf->SubBoFengSite[4], Res->RebarSite[iIndex],AimBuf->LastSumRebarSite);
//////                                LCD_ShowString(ASC16x8,0, 260, 0,dataBuf);
//////                            }
//////                            else if(Channel == CH3){
//////                                LCD_SetColor(WHITE,PURPLE);
//////                                sprintf(dataBuf,"SumCH3:%-6d, Sub:%-6d, LS:%-3d, S:%-3d   ", (iSumMaxHD+5)/10, AimBuf->SubBoFengSite[4], Res->RebarSite[iIndex],AimBuf->LastSumRebarSite);
//////                                LCD_ShowString(ASC16x8,0, 280, 0,dataBuf);
//////                            }
//////                            #endif
                        }
                    }
                }
                
//////                LCD_SetColor(WHITE,ENROD);
//////                sprintf(dataBuf,"Sum2:%-6d",iSumMaxHD);
//////                LCD_ShowString(ASC16x8,260, 60, 0,dataBuf);
//////                
//////                sprintf(dataBuf,"Val2:%-6d",SourceData.iCh2Sum[iSearchIndex-15]);
//////                LCD_ShowString(ASC16x8,260, 80, 0,dataBuf);
//////                
//////                sprintf(dataBuf,"%-6d",AimRebarPara_CH2.SumDataBuf_Mark[0]);
//////                LCD_ShowString(ASC16x8,260, 100, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimRebarPara_CH2.SumDataBuf_Mark[1]);
//////                LCD_ShowString(ASC16x8,260, 120, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimRebarPara_CH2.SumDataBuf_Mark[2]);
//////                LCD_ShowString(ASC16x8,260, 140, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimRebarPara_CH2.SumDataBuf_Mark[3]);
//////                LCD_ShowString(ASC16x8,260, 160, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimRebarPara_CH2.SumDataBuf_Mark[4]);
//////                LCD_ShowString(ASC16x8,260, 180, 0,dataBuf);
            }
        }
        
        //���β����ж�
        if((AimBuf->SumDataBuf_Mark[4] >= AimBuf->SumDataBuf_Mark[3])&&(AimBuf->SumDataBuf_Mark[3] >= AimBuf->SumDataBuf_Mark[2])
            &&(AimBuf->SumDataBuf_Mark[2] <= AimBuf->SumDataBuf_Mark[1])&&(AimBuf->SumDataBuf_Mark[1] <= AimBuf->SumDataBuf_Mark[0])
                &&(AimBuf->SumDataBuf_Mark[2] < AimBuf->SumDataBuf_Mark[4])&&(AimBuf->SumDataBuf_Mark[2] < AimBuf->SumDataBuf_Mark[0])){
            //���º�ֵ�ֽ�λ������
            AimBuf->SumBoGuSite[0] = AimBuf->SumBoGuSite[1];
            AimBuf->SumBoGuSite[1] = AimBuf->SumBoGuSite[2];
            AimBuf->SumBoGuSite[2] = AimBuf->SumBoGuSite[3];
            AimBuf->SumBoGuSite[3] = AimBuf->SumBoGuSite[4];
            AimBuf->SumBoGuSite[4] = iSearchIndex - 15 - SITE_OFFSET;
            
            //���º�ֵ�ֽ��ź�����
            AimBuf->SumBoGuVal[0] = AimBuf->SumBoGuVal[1];
            AimBuf->SumBoGuVal[1] = AimBuf->SumBoGuVal[2];
            AimBuf->SumBoGuVal[2] = AimBuf->SumBoGuVal[3];
            AimBuf->SumBoGuVal[3] = AimBuf->SumBoGuVal[4];
                    
            if(Channel == CH1){
                AimBuf->SumBoGuVal[4] = SumValCur;
            }
            else if(Channel == CH2){
                AimBuf->SumBoGuVal[4] = SumValCur;
            }
            else if(Channel == CH3){
                AimBuf->SumBoGuVal[4] = SumValCur;
            }
            
        }
    }
}

//================================================================//
//���ܣ�ͨ����ֵ���ݶ�λ�ֽ�
//������NULL
//���أ�NULL
//================================================================//
void AimRebarSum(void)
{
    CountSumBuffer(CH1, g_iCarDist-1, &AimRebarPara_CH1, &ResBuf_CH1, &AimRebarBuf_CH1);
    
    CountSumBuffer(CH2, g_iCarDist-1, &AimRebarPara_CH2, &ResBuf_CH2, &AimRebarBuf_CH2);
    
    CountSumBuffer(CH3, g_iCarDist-1, &AimRebarPara_CH3, &ResBuf_CH3, &AimRebarBuf_CH3);
}

//================================================================//
//���ܣ�ͨ����ֵ���ݶ�λ�ֽ�
//������NULL
//���أ�NULL
//================================================================//
void CountSubBuffer(enum CHANNELNUM Channel, unsigned int iSearchIndex, unsigned char *ucAimStep, int *AimBuf, 
    short *MarkBofengSite, int *MarkBofengVal, short *MarkBoguSite, int *MarkBoguVal)
{
//    int i = 0;
//    char dataBuf[10];
    int JudgeRange = 0;

    if(g_iCarDist <5){
        return;
    }

    //�ж����塢���Ȳ����뻺��
    if(iSearchIndex > 30){
        //�жϲ��岢���뻺��      ����ֵתλ�ƾ���66*120/400Լ20mm
        if((MarkBofengSite[4] == 0)||((MarkBofengSite[4] != 0)&&(iSearchIndex - 12 - MarkBofengSite[4])>20)){
            //���η�ֵ�ж�
            if((AimBuf[4] <= AimBuf[3])&&(AimBuf[3] <= AimBuf[2])&&(AimBuf[2] >= AimBuf[1])&&(AimBuf[1] >= AimBuf[0])
                &&(AimBuf[2] > (AimBuf[4]+JudgeRange))&&(AimBuf[2] > (AimBuf[0]+JudgeRange))){
                
                //���²�ֵ�ֽ�λ������
                MarkBofengSite[0] = MarkBofengSite[1];
                MarkBofengSite[1] = MarkBofengSite[2];
                MarkBofengSite[2] = MarkBofengSite[3];
                MarkBofengSite[3] = MarkBofengSite[4];
                MarkBofengSite[4] = iSearchIndex - 15 - SITE_OFFSET;
                
                //���²�ֵ�ֽ��ź�����
                MarkBofengVal[0] = MarkBofengVal[1];
                MarkBofengVal[1] = MarkBofengVal[2];
                MarkBofengVal[2] = MarkBofengVal[3];
                MarkBofengVal[3] = MarkBofengVal[4];
                MarkBofengVal[4] = AimBuf[2];
                
                //��λ������
                *ucAimStep = 1;
                
                //��ӡ��������
//////                if(Channel == CH2){
//////                    LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////                    sprintf(dataBuf,"%-6d",AimBuf[0]);
//////                    LCD_ShowString(ASC16x8,260, 100, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[1]);
//////                    LCD_ShowString(ASC16x8,260, 120, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[2]);
//////                    LCD_ShowString(ASC16x8,260, 140, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[3]);
//////                    LCD_ShowString(ASC16x8,260, 160, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[4]);
//////                    LCD_ShowString(ASC16x8,260, 180, 0,dataBuf);
//////                }
            }
        }
        
        //�ж�  ��ֵ   ���Ȳ����뻺��      ����ֵתλ�ƾ���66*120/400Լ20mm
        if((*ucAimStep == 1)&&((MarkBoguSite[4] == 0)||((MarkBoguSite[4] != 0)&&((iSearchIndex - 15 - MarkBoguSite[4])>20)))){//20  150//(g_iDataPutCnt - 12 - AimRebarDifPara.DifMarkSite[4])>45
            //���η�ֵ�ж�
            if((AimBuf[4] >= AimBuf[3])&&(AimBuf[3] >= AimBuf[2])&&(AimBuf[2] <= AimBuf[1])&&(AimBuf[1] <= AimBuf[0])
                &&(AimBuf[2] < (AimBuf[4]-JudgeRange))&&(AimBuf[2] < (AimBuf[0]-JudgeRange))){
                //����λ�ƶ���
                MarkBoguSite[0] = MarkBoguSite[1];
                MarkBoguSite[1] = MarkBoguSite[2];
                MarkBoguSite[2] = MarkBoguSite[3];
                MarkBoguSite[3] = MarkBoguSite[4];
                MarkBoguSite[4] = iSearchIndex - 15 - SITE_OFFSET;
                
                //����΢��ֵ����
                MarkBoguVal[0] = MarkBoguVal[1];
                MarkBoguVal[1] = MarkBoguVal[2];
                MarkBoguVal[2] = MarkBoguVal[3];
                MarkBoguVal[3] = MarkBoguVal[4];
                MarkBoguVal[4] = AimBuf[2];

                //��λ������
                *ucAimStep = 2;

                //��ӡ��������
//////                if(Channel == CH2){
//////                    LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////                    sprintf(dataBuf,"%-6d",AimBuf[0]);
//////                    LCD_ShowString(ASC16x8,360, 100, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[1]);
//////                    LCD_ShowString(ASC16x8,360, 120, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[2]);
//////                    LCD_ShowString(ASC16x8,360, 140, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[3]);
//////                    LCD_ShowString(ASC16x8,360, 160, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[4]);
//////                    LCD_ShowString(ASC16x8,360, 180, 0,dataBuf);
//////                }
            }
        }
    }
}


//================================================================//
//���ܣ��ܼ�ģʽ   �ֽ��������
//������NULL
//���أ�NULL
//================================================================//
MIJI_RET GetRebarCnt_Sub(WORKPARA WorkPara, short iWaveWidth, int iSubRet, int iSumRet)
{
    unsigned short iDisVal_Two = 0;//���õ��������ֽ�Ĳ��ο��ֵ
    unsigned short iDisVal_Three = 0;//���õ��������ֽ�Ĳ��ο��ֵ
    unsigned short iDisVal_Four = 0;//���õ����ĸ��ֽ�Ĳ��ο��ֵ
    unsigned char ucDiamIndex = 0;//�ֽ�ֱ����ѯ���
    unsigned char ucCoverIndex = 0;//�ֽ�ֱ����ѯ���
    MIJI_RET RET;

    if(iSubRet == 0){
        #ifdef ZJC_TEST //ZJC_TEST��ʾ�ֽ����
        SetColor(CL_BLACK,CL_RED);
        sprintf(g_cStrBuf,"iSubRet:0");
        DispText(ASC10x16,0,45,g_cStrBuf);
        #endif //END
        RET.iReBarsCnt = 1;
        RET.iDistance  = 0;
        return RET;
    }
    
    //����ֽ�ֱ��������
    if(WorkPara.cZhujinDiam <= 22){
        if(WorkPara.cZhujinDiam%2 == 0){
            ucDiamIndex = WorkPara.cZhujinDiam/2 - 3;
        }
        else{
            ucDiamIndex = WorkPara.cZhujinDiam/2 - 2;
        }
    }
    else if(WorkPara.cZhujinDiam <= 25){
        ucDiamIndex = 9;
    }
    else if(WorkPara.cZhujinDiam <= 28){
        ucDiamIndex = 10;
    }
    else if(WorkPara.cZhujinDiam <= 32){
        ucDiamIndex = 11;
    }

    if(WorkPara.cGangJinType == 1){//Բ��
        ucDiamIndex += 12;
    }

    //�����ȱ����
    if(iSubRet >= 10){
        ucCoverIndex = iSubRet/10 - 1;
        if(ucCoverIndex > 5){
            ucCoverIndex = 5;
        }
    }
    else{
        iSubRet = 10;
        ucCoverIndex = 0;
    }

    //��ֱͬ������,���в�������ֱ����ʹ�ýϴ��ֱ�����м���
    //�����ֽ�
    iDisVal_Two = (SUB_MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex+1] - SUB_MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex])\
        *(iSubRet-ucCoverIndex*10-10)/10 + SUB_MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex]+6;
    
    //�����ֽ�
    iDisVal_Three = (SUB_MIJI_DIAM_TABLE[ucDiamIndex]->ThreeRebars[ucCoverIndex+1] - SUB_MIJI_DIAM_TABLE[ucDiamIndex]->ThreeRebars[ucCoverIndex])\
        *(iSubRet-ucCoverIndex*10-10)/10 + SUB_MIJI_DIAM_TABLE[ucDiamIndex]->ThreeRebars[ucCoverIndex]+6;
    
    //�ĸ��ֽ�
    iDisVal_Four = (SUB_MIJI_DIAM_TABLE[ucDiamIndex]->FourRebars[ucCoverIndex+1] - SUB_MIJI_DIAM_TABLE[ucDiamIndex]->FourRebars[ucCoverIndex])\
        *(iSubRet-ucCoverIndex*10-10)/10 + SUB_MIJI_DIAM_TABLE[ucDiamIndex]->FourRebars[ucCoverIndex]+6;
    
    RET.iTWORebarVal = iDisVal_Two;
    RET.iTHREERebarVal = iDisVal_Three;
    RET.iFOURRebarVal = iDisVal_Four;
    
////////    #ifdef ZJC_TEST //ZJC_TEST��ʾ�ֽ����
    LCD_SetColor(BLACK,BLUE);
    sprintf(g_cStrBuf,"Val:%d  2:%d  3:%d  4:%d  %d", \
        iWaveWidth, iDisVal_Two, iDisVal_Three, iDisVal_Four,iSubRet);
    LCD_ShowString(ASC16x8,0,45, 0,g_cStrBuf);
////////    #endif //END

//    //ZJC_TEST��ʾ�ֽ����
//    SetColor(CL_BLACK,CL_BLUE);
//    sprintf(g_cStrBuf,"%d  %d  %d  %d  %d  %d  %d  %d", \
//        ucDiamIndex, ucCoverIndex, iSubRet-ucCoverIndex*10-10, \
//            (MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex+1] - MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex]),\
//                (MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex+1] - MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex])*(iSubRet-ucCoverIndex*10-10)/10,\
//                    MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex],MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex+1],iSubRet);
//    DispText(ASC10x16,0,85,g_cStrBuf);
//    //END

    //���ݲ��ο��  ����ֽ���
    if((iWaveWidth >= iDisVal_Two)&&(iWaveWidth < iDisVal_Three)){
        if(WorkPara.cZhujinDiam <= 12){
            RET.iDistance = WorkPara.cZhujinDiam + (iWaveWidth - iDisVal_Two)*(4*(iSubRet+5)/10+24 - WorkPara.cZhujinDiam)/(iDisVal_Three - iDisVal_Two);
        }
        else if(WorkPara.cZhujinDiam <= 22){
            RET.iDistance = WorkPara.cZhujinDiam + (iWaveWidth - iDisVal_Two)*(5*(iSubRet+5)/10+25 - WorkPara.cZhujinDiam)/(iDisVal_Three - iDisVal_Two);
        }
        else{
            RET.iDistance = WorkPara.cZhujinDiam + (iWaveWidth - iDisVal_Two)*(6*(iSubRet+5)/10+22 - WorkPara.cZhujinDiam)/(iDisVal_Three - iDisVal_Two);
        }
    }
    else if((iWaveWidth >= iDisVal_Three)&&(iWaveWidth < iDisVal_Four)){
        if(WorkPara.cZhujinDiam <= 12){
            RET.iDistance = (4*iSubRet/10+24)/2 + (iWaveWidth - iDisVal_Three)*(4*(iSubRet+5)/10+24 - (4*iSubRet/10+24)/2)/(iDisVal_Four - iDisVal_Three);
        }
        else if(WorkPara.cZhujinDiam <= 22){
            RET.iDistance = (5*iSubRet/10+25)/2 + (iWaveWidth - iDisVal_Three)*(5*(iSubRet+5)/10+25 - (5*iSubRet/10+25)/2)/(iDisVal_Four - iDisVal_Three);
        }
        else{
            RET.iDistance = (6*iSubRet/10+22)/2 + (iWaveWidth - iDisVal_Three)*(6*(iSubRet+5)/10+22 - (6*iSubRet/10+22)/2)/(iDisVal_Four - iDisVal_Three);
        }
    }
    else if(iWaveWidth >= iDisVal_Four){
        if(WorkPara.cZhujinDiam <= 12){
            RET.iDistance = (4*iSubRet/10+24)/2 + (iWaveWidth - iDisVal_Four)*(4*(iSubRet+5)/10+24 - (4*iSubRet/10+24)/2)/(iDisVal_Four - iDisVal_Three);
        }
        else if(WorkPara.cZhujinDiam <= 22){
            RET.iDistance = (5*iSubRet/10+25)/2 + (iWaveWidth - iDisVal_Four)*(5*(iSubRet+5)/10+25 - (5*iSubRet/10+25)/2)/(iDisVal_Four - iDisVal_Three);
        }
        else{
            RET.iDistance = (6*iSubRet/10+22)/2 + (iWaveWidth - iDisVal_Four)*(6*(iSubRet+5)/10+22 - (6*iSubRet/10+22)/2)/(iDisVal_Four - iDisVal_Three);
        }
    }
    else{
        RET.iDistance = 10;
    }
    
    //���ݲ��ο��  ����ֽ����
    if((iWaveWidth >= iDisVal_Two)&&(iWaveWidth < iDisVal_Three)){
        RET.iReBarsCnt = 2;
    }
    else if((iWaveWidth >= iDisVal_Three)&&(iWaveWidth < iDisVal_Four)){
        RET.iReBarsCnt = 3;
    }
    else if(iWaveWidth >= iDisVal_Four){
        RET.iReBarsCnt = 4;
    }
    else{
        RET.iReBarsCnt = 1;
    }
    
    //���ݲ��ο��  �������ֵ
    if((iWaveWidth >= iDisVal_Two)&&(iWaveWidth < iDisVal_Three)){
        RET.iHD = (iSubRet+iSumRet)/2/10/2 - (iSubRet+iSumRet)/2/10/2*(iWaveWidth - iDisVal_Two)/(iDisVal_Three - iDisVal_Two);
    }
    else if((iWaveWidth >= iDisVal_Three)&&(iWaveWidth < iDisVal_Four)){
        RET.iHD = (iSubRet+iSumRet)/2/10/2 - (iSubRet+iSumRet)/2/10/2*(iWaveWidth - iDisVal_Three)/(iDisVal_Four - iDisVal_Three);
    }
    else if(iWaveWidth >= iDisVal_Four){
        RET.iHD = 2;
    }
    else{
        RET.iHD = 2;
    }
    if(RET.iHD <= 2){
        RET.iHD = 2;
    }
    
    return RET;
}

//================================================================//
//���ܣ��ܼ�ģʽ   �ֽ��������
//������NULL
//���أ�NULL
//================================================================//
MIJI_RET GetRebarCnt_Dif(WORKPARA WorkPara, short iWaveWidth, int iSubRet, int iSumRet)
{
    unsigned short iDisVal_Two = 0;//���õ��������ֽ�Ĳ��ο��ֵ
    unsigned short iDisVal_Three = 0;//���õ��������ֽ�Ĳ��ο��ֵ
    unsigned short iDisVal_Four = 0;//���õ����ĸ��ֽ�Ĳ��ο��ֵ
    unsigned char ucDiamIndex = 0;//�ֽ�ֱ����ѯ���
    unsigned char ucCoverIndex = 0;//�ֽ�ֱ����ѯ���
    MIJI_RET RET;

    if(iSubRet == 0){
        #ifdef ZJC_TEST //ZJC_TEST��ʾ�ֽ����
        SetColor(CL_BLACK,CL_RED);
        sprintf(g_cStrBuf,"iSubRet:0");
        DispText(ASC10x16,0,45,g_cStrBuf);
        #endif //END
        RET.iReBarsCnt = 1;
        RET.iDistance  = 0;
        return RET;
    }
    
    //����ֽ�ֱ��������
    if(WorkPara.cZhujinDiam <= 22){
        if(WorkPara.cZhujinDiam%2 == 0){
            ucDiamIndex = WorkPara.cZhujinDiam/2 - 3;
        }
        else{
            ucDiamIndex = WorkPara.cZhujinDiam/2 - 2;
        }
    }
    else if(WorkPara.cZhujinDiam <= 25){
        ucDiamIndex = 9;
    }
    else if(WorkPara.cZhujinDiam <= 28){
        ucDiamIndex = 10;
    }
    else if(WorkPara.cZhujinDiam <= 32){
        ucDiamIndex = 11;
    }

    if(WorkPara.cGangJinType == 1){//Բ��
        ucDiamIndex += 12;
    }

    //�����ȱ����
    if(iSubRet >= 10){
        ucCoverIndex = iSubRet/10 - 1;
        if(ucCoverIndex > 5){
            ucCoverIndex = 5;
        }
    }
    else{
        iSubRet = 10;
        ucCoverIndex = 0;
    }

    //��ֱͬ������,���в�������ֱ����ʹ�ýϴ��ֱ�����м���
    //�����ֽ�
    iDisVal_Two = (DIF_MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex+1] - DIF_MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex])\
        *(iSubRet-ucCoverIndex*10-10)/10 + DIF_MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex]+6;
    
    //�����ֽ�
    iDisVal_Three = (DIF_MIJI_DIAM_TABLE[ucDiamIndex]->ThreeRebars[ucCoverIndex+1] - DIF_MIJI_DIAM_TABLE[ucDiamIndex]->ThreeRebars[ucCoverIndex])\
        *(iSubRet-ucCoverIndex*10-10)/10 + DIF_MIJI_DIAM_TABLE[ucDiamIndex]->ThreeRebars[ucCoverIndex]+6;
    
    //�ĸ��ֽ�
    iDisVal_Four = (DIF_MIJI_DIAM_TABLE[ucDiamIndex]->FourRebars[ucCoverIndex+1] - DIF_MIJI_DIAM_TABLE[ucDiamIndex]->FourRebars[ucCoverIndex])\
        *(iSubRet-ucCoverIndex*10-10)/10 + DIF_MIJI_DIAM_TABLE[ucDiamIndex]->FourRebars[ucCoverIndex]+6;
    
    RET.iTWORebarVal = iDisVal_Two;
    RET.iTHREERebarVal = iDisVal_Three;
    RET.iFOURRebarVal = iDisVal_Four;
    
    #ifdef ZJC_TEST_GY91 //ZJC_TEST��ʾ�ֽ����
    LCD_SetColor(BLACK,BLUE);
    sprintf(g_cStrBuf,"Val:%d  2:%d  3:%d  4:%d  %d", \
        iWaveWidth, iDisVal_Two, iDisVal_Three, iDisVal_Four,iSubRet);
    LCD_ShowText(ASC16x8,0,45,0,(u8*)g_cStrBuf);
    #endif //END

//    //ZJC_TEST��ʾ�ֽ����
//    SetColor(CL_BLACK,CL_BLUE);
//    sprintf(g_cStrBuf,"%d  %d  %d  %d  %d  %d  %d  %d", \
//        ucDiamIndex, ucCoverIndex, iSubRet-ucCoverIndex*10-10, \
//            (MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex+1] - MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex]),\
//                (MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex+1] - MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex])*(iSubRet-ucCoverIndex*10-10)/10,\
//                    MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex],MIJI_DIAM_TABLE[ucDiamIndex]->TwoRebars[ucCoverIndex+1],iSubRet);
//    DispText(ASC10x16,0,85,g_cStrBuf);
//    //END

    //���ݲ��ο��  ����ֽ���
    if((iWaveWidth >= iDisVal_Two)&&(iWaveWidth < iDisVal_Three)){
        if(WorkPara.cZhujinDiam <= 12){
            RET.iDistance = WorkPara.cZhujinDiam + (iWaveWidth - iDisVal_Two)*(4*(iSubRet+5)/10+24 - WorkPara.cZhujinDiam)/(iDisVal_Three - iDisVal_Two);
        }
        else if(WorkPara.cZhujinDiam <= 22){
            RET.iDistance = WorkPara.cZhujinDiam + (iWaveWidth - iDisVal_Two)*(5*(iSubRet+5)/10+25 - WorkPara.cZhujinDiam)/(iDisVal_Three - iDisVal_Two);
        }
        else{
            RET.iDistance = WorkPara.cZhujinDiam + (iWaveWidth - iDisVal_Two)*(6*(iSubRet+5)/10+22 - WorkPara.cZhujinDiam)/(iDisVal_Three - iDisVal_Two);
        }
    }
    else if((iWaveWidth >= iDisVal_Three)&&(iWaveWidth < iDisVal_Four)){
        if(WorkPara.cZhujinDiam <= 12){
            RET.iDistance = (4*iSubRet/10+24)/2 + (iWaveWidth - iDisVal_Three)*(4*(iSubRet+5)/10+24 - (4*iSubRet/10+24)/2)/(iDisVal_Four - iDisVal_Three);
        }
        else if(WorkPara.cZhujinDiam <= 22){
            RET.iDistance = (5*iSubRet/10+25)/2 + (iWaveWidth - iDisVal_Three)*(5*(iSubRet+5)/10+25 - (5*iSubRet/10+25)/2)/(iDisVal_Four - iDisVal_Three);
        }
        else{
            RET.iDistance = (6*iSubRet/10+22)/2 + (iWaveWidth - iDisVal_Three)*(6*(iSubRet+5)/10+22 - (6*iSubRet/10+22)/2)/(iDisVal_Four - iDisVal_Three);
        }
    }
    else if(iWaveWidth >= iDisVal_Four){
        RET.iDistance = 30;
    }
    else{
        RET.iDistance = 30;
    }
    
    //���ݲ��ο��  ����ֽ����
    if((iWaveWidth >= iDisVal_Two)&&(iWaveWidth < iDisVal_Three)){
        RET.iReBarsCnt = 2;
    }
    else if((iWaveWidth >= iDisVal_Three)&&(iWaveWidth < iDisVal_Four)){
        RET.iReBarsCnt = 3;
    }
    else if(iWaveWidth >= iDisVal_Four){
        RET.iReBarsCnt = 4;
    }
    else{
        RET.iReBarsCnt = 1;
    }
    
    //���ݲ��ο��  �������ֵ
    if((iWaveWidth >= iDisVal_Two)&&(iWaveWidth < iDisVal_Three)){
        RET.iHD = (iSubRet+iSumRet)/2/10/2 - (iSubRet+iSumRet)/2/10/2*(iWaveWidth - iDisVal_Two)/(iDisVal_Three - iDisVal_Two);
    }
    else if((iWaveWidth >= iDisVal_Three)&&(iWaveWidth < iDisVal_Four)){
        RET.iHD = (iSubRet+iSumRet)/2/10/2 - (iSubRet+iSumRet)/2/10/2*(iWaveWidth - iDisVal_Three)/(iDisVal_Four - iDisVal_Three);
    }
    else if(iWaveWidth >= iDisVal_Four){
        RET.iHD = 2;
    }
    else{
        RET.iHD = 2;
    }
    if(RET.iHD <= 2){
        RET.iHD = 2;
    }
    
    return RET;
}

//================================================================//
//���ܣ���ȼ��㺯�������غ������
//������iAdcValue      						ʵʱADCֵ
// 		workPara							�ֽ����ѡ��
//		bGuCeZhiJing						�Ƿ����
//      cCeHouMode                          �Ƿ�Ϊ���ģʽ
//      bNewAdcCalcflag                     ��ֵ���ֵ��������ѡ��
//���أ����ֵ
//================================================================//
int FindCoverTab_Sub(enum CHANNELNUM Channel, unsigned int iAdcValue, WORKPARA WorkPara, AIMREBAR_BUF_STR *AimBuf)
{
    const unsigned int  *pAdcTab;
    const unsigned short int *pHDTab;
    const short int *pFIPara;
	const short int *pZeroPara;
//////    short int *pSCFIPara;
//////	short int *pSCZeroPara;
    int TabSize;
    
    int iDiamIndex_1 = 0;
    int iDiamIndex_2 = 0;

//	const double *JianJuPara_A;
//	const double *JianJuPara_B;
//	signed char  *UserTab; //�û���������

	int i;
	double dRatio;
	double dHouDu = 0;
	double dBuf_A = 0;
    double dBuf_B = 0;
	double dSpace=0.0;
	int iDiamMin = 0;

    //������ָ�븳ֵ
	if(WorkPara.cLingChengType == 1){//������
        if(WorkPara.cGangJinType == 0){//���Ƹ�
            if(Channel == CH1){
                pAdcTab = BIG_LWG_SUB_CH1;
                pFIPara = SUB_BIG_FI_LW_CH1;
                pZeroPara = SUB_BIG_ZERO_LW_CH1;
            }
            else if(Channel == CH2){
                pAdcTab = BIG_LWG_SUB_CH2;
                pFIPara = SUB_BIG_FI_LW_CH2;
                pZeroPara = SUB_BIG_ZERO_LW_CH2;
            }
            else if(Channel == CH3){
                pAdcTab = BIG_LWG_SUB_CH3;
                pFIPara = SUB_BIG_FI_LW_CH3;
                pZeroPara = SUB_BIG_ZERO_LW_CH3;
            }
            
//////            pSCFIPara = &g_SysPara.iBigFi_LW[0];
//////            pSCZeroPara = &g_SysPara.iBigZero_LW[0];
            
//            UserTab = &g_SysPara.iBigUser_LWG[0];
//				UserTab = User_SMALL_LWG;
        }
        if(WorkPara.cGangJinType == 1){//Բ��
            if(Channel == CH1){
                pAdcTab = BIG_LWG_SUB_CH1;
                pFIPara = SUB_BIG_FI_YG_CH1;
                pZeroPara = SUB_BIG_ZERO_YG_CH1;
            }
            else if(Channel == CH2){
                pAdcTab = BIG_LWG_SUB_CH2;
                pFIPara = SUB_BIG_FI_YG_CH2;
                pZeroPara = SUB_BIG_ZERO_YG_CH2;
            }
            else if(Channel == CH3){
                pAdcTab = BIG_LWG_SUB_CH3;
                pFIPara = SUB_BIG_FI_YG_CH3;
                pZeroPara = SUB_BIG_ZERO_YG_CH3;
            }
            
//////            pSCFIPara = &g_SysPara.iBigFi_LW[0];
//////            pSCZeroPara = &g_SysPara.iBigZero_LW[0];
            
//            UserTab = &g_SysPara.iBigUser_LWG[0];
//				UserTab = User_SMALL_LWG;
        }
        TabSize = 268;
    }

	//ͨ����������ֵ
	if(iAdcValue > pAdcTab[0]){
		dHouDu = 0;
	}
	else {
		for(i=0; i<TabSize; i++){
			if((iAdcValue <= pAdcTab[i]) && (iAdcValue > pAdcTab[i+1])){
                dRatio = (double)(pAdcTab[i]-iAdcValue)/(pAdcTab[i] - pAdcTab[i+1]);
                dHouDu = i + dRatio;
				break;
			}
		}
	}
	dHouDu *= 10; //���ֵ�Ŵ�10��
    
    AimBuf->EstSubCh2Diam16 = dHouDu;
    
	//��ֱͬ������,���в�������ֱ����ʹ�����Բ�ֵ�������㲹��ֵ
    if(WorkPara.cZhujinDiam <= 22){
        if(WorkPara.cZhujinDiam%2 == 0){
            iDiamIndex_1 = WorkPara.cZhujinDiam/2 - 2;
            dHouDu = dHouDu * pFIPara[iDiamIndex_1]/1000 + pZeroPara[iDiamIndex_1];
        }
        else{
            iDiamIndex_1 = WorkPara.cZhujinDiam/2-2;
            iDiamIndex_2 = WorkPara.cZhujinDiam/2-1;
            dHouDu = dHouDu * (pFIPara[iDiamIndex_1] + pFIPara[iDiamIndex_2])/2000
                + (pZeroPara[iDiamIndex_1] + pZeroPara[iDiamIndex_2])/2;
        }
    }
    else{
        if(WorkPara.cZhujinDiam <= 25){
            iDiamIndex_1 = 9;
            iDiamIndex_2 = 10;
            iDiamMin = 22;
			dSpace=3.0;
        }
        else if(WorkPara.cZhujinDiam <= 28){
            iDiamIndex_1 = 10;
            iDiamIndex_2 = 11;
            iDiamMin = 25;
			dSpace=3.0;
        }
        else if(WorkPara.cZhujinDiam <= 32){
            iDiamIndex_1 = 11;
            iDiamIndex_2 = 12;
            iDiamMin = 28;
			dSpace=4.0;
        }   
        else if(WorkPara.cZhujinDiam <= 40){
            iDiamIndex_1 = 12;
            iDiamIndex_2 = 13;
            iDiamMin = 32;
			dSpace=8.0;
        }
        else if(WorkPara.cZhujinDiam <= 50){
            iDiamIndex_1 = 13;
            iDiamIndex_2 = 14;
            iDiamMin = 40;
			dSpace=10.0;
        }
        dBuf_A = ((pFIPara[iDiamIndex_2]-pFIPara[iDiamIndex_1])*(WorkPara.cZhujinDiam - iDiamMin)/dSpace + pFIPara[iDiamIndex_1])/1000.0;
        dBuf_B = (pZeroPara[iDiamIndex_2]-pZeroPara[iDiamIndex_1])*(WorkPara.cZhujinDiam - iDiamMin)/dSpace + pZeroPara[iDiamIndex_1];
        dHouDu = dHouDu*dBuf_A + dBuf_B;
    }
//////    dHouDu += 5; //��������
    
    return (int)dHouDu;
}

//================================================================//
//���ܣ�����λ�Ʋ�����ֵ�������
//������NULL
//���أ�NULL
//================================================================//
int SubCoverModify(int iWaveWidth, int iSubRet, MIJI_RET MIJI_PARA)
{
    int res = 0;
    int iWidth1 = 0, iWidth2 = 0, iWidth3 = 0;
    
    iWidth1 = MIJI_PARA.iTWORebarVal + (MIJI_PARA.iTHREERebarVal - MIJI_PARA.iTWORebarVal)/3;
    iWidth2 = MIJI_PARA.iTWORebarVal + (MIJI_PARA.iTHREERebarVal - MIJI_PARA.iTWORebarVal)/3*2;

    if(iWaveWidth < iWidth1){
        res = iSubRet;
    }
    else if(iWaveWidth < iWidth2){
        res = iSubRet-1;
    }
    else{
        res = iSubRet-2;
    }
    return res;
}

//================================================================//
//���ܣ���λ�ֽ��ʾ��ֵ��λ�ֽ����
//������NULL
//���أ�NULL
//================================================================//
void SubAimFunc(enum CHANNELNUM Channel, AIMREBAR_PARA_STR *AimPara, AIMREBAR_BUF_STR *AimBuf, int DispSite)
{
    char dataBuf[10];
    int i = 0;
//    int DifBofengSite = 0;
    MIJI_RET MIJI_RET_SUB;
    MIJI_RET MIJI_RET_DIF;
    MIJI_RET MIJI_RET;
    
    int SecCnt = 0;
    int DifCnt = 0;
    int iDiffStart=0,iDiffBoGuVal=1000000,iDifMarkCnt=0;
    
    if(AimPara->ucSubAimStep == 2){
        
//////        //����ʱ����ӡ��ֵ����
//////        iCurCnt = index/6*6;
//////        for(i=0;i<5;i++){
//////            SourceData.iCh3DifMark[iCurCnt-5+i] = SubAimBuf[i];
//////        }
//////        SourceData.iCh3DifMark[iCurCnt-5+i] = 0;

        //��λ��ֵ����
        AimPara->iSubMinVal = AimBuf->SubBoGuVal[4];
        AimPara->iSubMinSite = AimBuf->SubBoGuSite[4];
        
        //��λ��ֵ����
        AimPara->iSubMaxVal = AimBuf->SubBoFengVal[4];
        AimPara->iSubMaxSite = AimBuf->SubBoFengSite[4];
        
        //�жϲ��岨�Ȳ�ֵ�Ƿ��������Ҫ��
        if((AimPara->iSubMaxVal - AimPara->iSubMinVal) <= 20){
            return;
        }

        //����ֽ�λ��
        AimPara->SubRebarSite = (AimPara->iSubMaxSite + AimPara->iSubMinSite)/2;
        
        //���㲢��ʾ���ֵ
        AimPara->SubRebarHD = FindCoverTab_Sub(Channel, AimPara->iSubMaxVal-AimPara->iSubMinVal, g_tWorkPara, AimBuf);//��ֵ��������

//////        LCD_SetColor(WHITE,PURPLE);
//////        if(Channel == CH1){
//////            AimBuf->EstimateSubCh1 = AimPara->SubRebarHD;
//////            sprintf(dataBuf,"SubCH1:%d,%d,%d,%d",AimPara->SubRebarHD,AimPara->iSubMaxVal-AimPara->iSubMinVal, AimPara->iSubMaxVal, AimPara->iSubMinVal);
//////            LCD_ShowString(ASC16x8,200, 180, 0,dataBuf);
//////        }
//////        else if(Channel == CH2){
//////            AimBuf->EstimateSubCh2 = AimPara->SubRebarHD;
//////            sprintf(dataBuf,"SubCH2:%d,%d,%d,%d",AimPara->SubRebarHD,AimPara->iSubMaxVal-AimPara->iSubMinVal, AimPara->iSubMaxVal, AimPara->iSubMinVal);
//////            LCD_ShowString(ASC16x8,200, 200, 0,dataBuf);
//////        }
//////        else if(Channel == CH3){
//////            AimBuf->EstimateSubCh3 = AimPara->SubRebarHD;
//////            sprintf(dataBuf,"SubCH3:%d,%d,%d,%d",AimPara->SubRebarHD,AimPara->iSubMaxVal-AimPara->iSubMinVal, AimPara->iSubMaxVal, AimPara->iSubMinVal);
//////            LCD_ShowString(ASC16x8,200, 220, 0,dataBuf);
//////        }

        AimPara->SubRebarHD += 5;
        
//////        LCD_SetColor(WHITE,BLUE);
//////        if(Channel == CH1){
//////            sprintf(dataBuf,"1:%d,%d,%d,%d,%d,%d,%d,%d,%d   ",AimPara->iSubMinSite-AimBuf->DifBoFengSite[4],AimPara->iSubMinSite,AimBuf->DifBoFengSite[3],AimBuf->DifBoFengSite[4]
//////                ,(AimPara->SubRebarHD + AimPara->SubRebarHD/10)/10,AimPara->iSubMaxVal,AimPara->iSubMinVal,AimBuf->SumBoFengVal[4],AimPara->iDifMinVal);
//////            LCD_ShowString(ASC16x8,10, 180, 0,dataBuf);
//////        }
//////        else if(Channel == CH2){
//////            sprintf(dataBuf,"2:%d,%d,%d,%d,%d,%d,%d,%d,%d   ",AimPara->iSubMinSite-AimBuf->DifBoFengSite[4],AimPara->iSubMinSite,AimBuf->DifBoFengSite[3],AimBuf->DifBoFengSite[4]
//////                ,(AimPara->SubRebarHD + AimPara->SubRebarHD/10)/10,AimPara->iSubMaxVal,AimPara->iSubMinVal,AimBuf->SumBoFengVal[4],AimPara->iDifMinVal);
//////            LCD_ShowString(ASC16x8,10, 200, 0,dataBuf);
//////        }
//////        else if(Channel == CH3){
//////            sprintf(dataBuf,"3:%d,%d,%d,%d,%d,%d,%d,%d,%d   ",AimPara->iSubMinSite-AimBuf->DifBoFengSite[4],AimPara->iSubMinSite,AimBuf->DifBoFengSite[3],AimBuf->DifBoFengSite[4]
//////                ,(AimPara->SubRebarHD + AimPara->SubRebarHD/10)/10,AimPara->iSubMaxVal,AimPara->iSubMinVal,AimBuf->SumBoFengVal[4],AimPara->iDifMinVal);
//////            LCD_ShowString(ASC16x8,10, 220, 0,dataBuf);
//////        }
        
//////        LCD_SetColor(WHITE,BLUE);
//////        if(Channel == CH1){
//////            sprintf(dataBuf,"1:%d,%d,%d,%d,%d,%d,%d   ",AimPara->iSubMinSite-AimBuf->DifBoFengSite[4],AimPara->iSubMinSite,AimBuf->DifBoFengSite[3],AimBuf->DifBoFengSite[4]
//////                ,(AimPara->SubRebarHD + AimPara->SubRebarHD/10)/10,AimBuf->SumBoFengVal[4],AimBuf->LastSumRebarHD);
//////            LCD_ShowString(ASC16x8,10, 180, 0,dataBuf);
//////        }
//////        else if(Channel == CH2){
//////            sprintf(dataBuf,"2:%d,%d,%d,%d,%d,%d,%d   ",AimPara->iSubMinSite-AimBuf->DifBoFengSite[4],AimPara->iSubMinSite,AimBuf->DifBoFengSite[3],AimBuf->DifBoFengSite[4]
//////                ,(AimPara->SubRebarHD + AimPara->SubRebarHD/10)/10,AimBuf->SumBoFengVal[4],AimBuf->LastSumRebarHD);
//////            LCD_ShowString(ASC16x8,10, 200, 0,dataBuf);
//////        }
//////        else if(Channel == CH3){
//////            sprintf(dataBuf,"3:%d,%d,%d,%d,%d,%d,%d   ",AimPara->iSubMinSite-AimBuf->DifBoFengSite[4],AimPara->iSubMinSite,AimBuf->DifBoFengSite[3],AimBuf->DifBoFengSite[4]
//////                ,(AimPara->SubRebarHD + AimPara->SubRebarHD/10)/10,AimBuf->SumBoFengVal[4],AimBuf->LastSumRebarHD);
//////            LCD_ShowString(ASC16x8,10, 220, 0,dataBuf);
//////        }
        
        //�����㷨1   ��ֵ�ܼ������
        MIJI_RET_SUB = GetRebarCnt_Sub(g_tWorkPara, AimPara->iSubMinSite-AimPara->iSubMaxSite, (AimPara->SubRebarHD + AimPara->SubRebarHD/10)/10, AimPara->SubRebarHD/10);
        
        //΢���ܼ������
//        for(i=4;i>=0;i--){//΢�ֲ����ڲ�ֵ����֮ǰ������һ����ֵ����֮��
//            if((AimPara->iSubMinSite-AimBuf->DifBoFengSite[i] < AimPara->iSubMaxSite)&&(AimPara->iSubMinSite-AimBuf->DifBoFengSite[i]>AimBuf->SubBoFengSite[3])){
//                DifBofengSite = AimPara->iSubMinSite-AimBuf->DifBoFengSite[i];
//                break;
//            }
//        }

//        if((MIJI_RET_SUB.iReBarsCnt == 2)&&(Channel == CH2)){
//            LCD_SetColor(WHITE,ORANGE);//CL_BLUE
//            sprintf(dataBuf,"Sub:2");
//            LCD_ShowString(ASC16x8,0, 160, 0,dataBuf);
//        }
            
            
        //�����㷨2
        
        //����  ΢�ֲ������
        for(i=4;i>=0;i--){
            if(AimBuf->DifBoFengSite[i] > AimBuf->iLastSubMinSite){
                iDifMarkCnt++;
            }
            else{
                break;
            }
        }
                    
        //�ж�΢�ֲ�����ʼ����λ��
        for(i=4;i>=0;i--){
            if(AimPara->iSubMaxSite >= AimBuf->DifBoFengSite[i]){
                iDiffStart = i;
                break;
            }
        }
        
        //�ж�����΢�ֲ���֮���Ƿ��в���
        for(i=4;i>=0;i--){
            if((AimBuf->DifBoGuSite[i] > AimBuf->DifBoFengSite[iDiffStart-1])&&(AimBuf->DifBoGuSite[i] < AimBuf->DifBoFengSite[iDiffStart])){
                iDiffBoGuVal = AimBuf->DifBoGuVal[i];
                break;
            }
        }
        
        //����΢�ֲ��岨�Ȳ����ֽ�    //�����벨��Ĳ�ֵ����20   ������С��100   ����ֵС��20  //���ڿɼ���λ�����ݵ��ж�����
        if((iDifMarkCnt > 1)&&(iDiffBoGuVal != 1000000)&&(((AimBuf->DifBoFengSite[3]-iDiffBoGuVal)>20)||((AimBuf->DifBoFengSite[4]-iDiffBoGuVal)>20))\
            &&((AimBuf->DifBoFengSite[iDiffStart]-AimBuf->DifBoFengSite[iDiffStart-1])<100)&&(iDiffBoGuVal<20)){
            iDifMarkCnt = 2;//�Ȳ���1���ֽ��������ֽ������Ժ������ٴ���
                
                
            MIJI_RET_DIF.iReBarsCnt = 2;
            
            DifCnt = 2;
//            if(Channel == CH2){
//                LCD_SetColor(WHITE,ORANGE);//CL_BLUE
//                sprintf(dataBuf,"Dif:2");
//                LCD_ShowString(ASC16x8,0, 180, 0,dataBuf);
//            }
        }
        else{
            iDifMarkCnt = 0;
        }

        
//////        if(MIJI_RET_DIF.iReBarsCnt == 1){
//////            MIJI_RET_DIF.iReBarsCnt = 2;
//////            
//////            DifCnt = 2;
//////            
//////            LCD_SetColor(WHITE,ORANGE);//CL_BLUE
//////            sprintf(dataBuf,"Dif:2");
//////            LCD_ShowString(ASC16x8,0, 180, 0,dataBuf);
//////        }
        
        //�����㷨3  ��һ�εĲ�ֵ���Ⱥͱ��εĲ�ֵ�����м䣬��������΢�ֲ���
        if(((AimBuf->DifBoFengSite[4] > AimBuf->SubBoGuSite[3])&&(AimBuf->DifBoFengSite[4] < AimBuf->SubBoFengSite[4]))
            &&((AimBuf->DifBoFengSite[3] > AimBuf->SubBoGuSite[3])&&(AimBuf->DifBoFengSite[3] < AimBuf->SubBoFengSite[4]))){
                MIJI_RET_DIF = GetRebarCnt_Dif(g_tWorkPara, AimPara->iSubMinSite-AimBuf->DifBoFengSite[4], (AimPara->SubRebarHD + AimPara->SubRebarHD/10)/10, AimPara->SubRebarHD/10);
        }
        else{
            MIJI_RET_DIF = MIJI_RET_SUB;
        }
        
        //�����㷨4  ����΢�ֲ����м䣬����2������΢�ֲ���
        if(((AimBuf->SecDifBoGuSite[4] > AimBuf->DifBoGuSite[3])&&(AimBuf->SecDifBoGuSite[4] < AimBuf->DifBoGuSite[4]))
            &&((AimBuf->SecDifBoGuSite[3] > AimBuf->DifBoGuSite[3])&&(AimBuf->SecDifBoGuSite[3] < AimBuf->DifBoGuSite[4]))){
            if((MIJI_RET_DIF.iReBarsCnt == 1)&&(((AimBuf->SecDifBoFengVal[4]-AimBuf->SecDifBoGuVal[3]) > 200)&&((AimBuf->SecDifBoFengVal[4]-AimBuf->SecDifBoGuVal[4]) > 200)
                &&(AimBuf->SecDifBoFengSite[4] < AimBuf->SecDifBoGuVal[4])&&(AimBuf->SecDifBoFengSite[4] > AimBuf->SecDifBoGuVal[3]))){
                MIJI_RET_DIF.iReBarsCnt = 2;//�ֽ����������΢�ָֽ��м�λ��
                    
                SecCnt = 2;
//                if(Channel == CH2){
//                    LCD_SetColor(WHITE,ORANGE);//CL_BLUE
//                    sprintf(dataBuf,"Sec:2   %d   %d",AimBuf->SecDifBoFengVal[3],AimBuf->SecDifBoGuVal[3]);
//                    LCD_ShowString(ASC16x8,0, 200, 0,dataBuf);
//                }
            }
        }
        
        //Test
//////        if(Channel == CH1){
//////            sprintf(dataBuf,"Sub:%d  Dif:%d      ",MIJI_RET_SUB.iReBarsCnt, MIJI_RET_DIF.iReBarsCnt);
//////            LCD_ShowString(ASC16x8,0, 180, 0,dataBuf);
//////        }
//////        else if(Channel == CH2){
//////            sprintf(dataBuf,"Sub:%d  Dif:%d  %d,%d,%d,%d,%d    ",MIJI_RET_SUB.iReBarsCnt, MIJI_RET_DIF.iReBarsCnt,AimBuf->DifBoFengSite[4],AimBuf->DifBoFengSite[3],AimBuf->DifBoFengSite[2],AimBuf->DifBoFengSite[1],AimBuf->DifBoFengSite[0]);
//////            LCD_ShowString(ASC16x8,0, 200, 0,dataBuf);
//////            sprintf(dataBuf,"Sub:%d  Dif:%d  %d,%d,%d,%d,%d    ",MIJI_RET_SUB.iReBarsCnt, MIJI_RET_DIF.iReBarsCnt,AimBuf->DifBoGuSite[4],AimBuf->DifBoGuSite[3],AimBuf->DifBoGuSite[2],AimBuf->DifBoGuSite[1],AimBuf->DifBoGuSite[0]);
//////            LCD_ShowString(ASC16x8,0, 220, 0,dataBuf);
//////        }
//////        else if(Channel == CH3){
//////            sprintf(dataBuf,"Sub:%d  Dif:%d      ",MIJI_RET_SUB.iReBarsCnt, MIJI_RET_DIF.iReBarsCnt);
//////            LCD_ShowString(ASC16x8,0, 250, 0,dataBuf);
//////        }
        //End
        
        if(MIJI_RET_SUB.iReBarsCnt >= MIJI_RET_DIF.iReBarsCnt){
            MIJI_RET = MIJI_RET_SUB;
        }
        else{
            MIJI_RET = MIJI_RET_DIF;
        }
        
        //ͨ�����������ֵ�������
        if(MIJI_RET.iReBarsCnt > 1){
            AimPara->SubRebarHD = SubCoverModify(AimPara->iSubMinSite-AimPara->iSubMaxSite, (AimPara->SubRebarHD + AimPara->SubRebarHD/10)/10, MIJI_RET_SUB);
        }
        else{
            AimPara->SubRebarHD = AimPara->SubRebarHD/10;
        }
        
        //��¼��ʷ�ֽ�λ�úͺ��
//////        for(i=6;i>=MIJI_RET.iReBarsCnt;i--){
//////            AimBuf->LastSubRebarSite[i-MIJI_RET.iReBarsCnt] = AimBuf->LastSubRebarSite[i];
//////            AimBuf->LastSubRebarHD[i-MIJI_RET.iReBarsCnt] = AimBuf->LastSubRebarHD[i];
//////            if(i < 0){
//////                break;
//////            }
//////        }
        
        //��ʷ���ݻ�����ǰ�ƶ��ֽ������λ��
        for(i=MIJI_RET.iReBarsCnt;i<=6;i++){
            AimBuf->LastSubRebarSite[i-MIJI_RET.iReBarsCnt] = AimBuf->LastSubRebarSite[i];
            AimBuf->LastSubRebarHD[i-MIJI_RET.iReBarsCnt] = AimBuf->LastSubRebarHD[i];
        }
        AimBuf->iLastSubRebarCnt = MIJI_RET.iReBarsCnt;
        
        //������������ʷ���ݻ�����
        if(MIJI_RET.iReBarsCnt == 1){
            AimBuf->LastSubRebarSite[6] = AimPara->SubRebarSite;
            AimBuf->LastSubRebarHD[6] = AimPara->SubRebarHD;
        }
        if(MIJI_RET.iReBarsCnt == 2){
            
            if(MIJI_RET_SUB.iReBarsCnt == 2){
                AimBuf->LastSubRebarSite[5] = AimPara->SubRebarSite - (AimPara->iSubMinSite - AimPara->iSubMaxSite)/4;
                AimBuf->LastSubRebarHD[5] = AimPara->SubRebarHD;
                
                AimBuf->LastSubRebarSite[6] = AimPara->SubRebarSite + (AimPara->iSubMinSite - AimPara->iSubMaxSite)/4;
                AimBuf->LastSubRebarHD[6] = AimPara->SubRebarHD;
            }
            else if(DifCnt == 2){
                AimBuf->LastSubRebarSite[5] = AimBuf->DifBoGuSite[3];
                AimBuf->LastSubRebarHD[5] = AimPara->SubRebarHD;
                
                AimBuf->LastSubRebarSite[6] = AimBuf->DifBoGuSite[4];
                AimBuf->LastSubRebarHD[6] = AimPara->SubRebarHD;
            }
            else if(SecCnt == 2){
                AimBuf->LastSubRebarSite[5] = (AimBuf->SecDifBoGuSite[3]+AimBuf->SecDifBoGuSite[4])/2;
                AimBuf->LastSubRebarHD[5] = AimPara->SubRebarHD;
                
                AimBuf->LastSubRebarSite[6] = AimBuf->DifBoGuSite[4];
                AimBuf->LastSubRebarHD[6] = AimPara->SubRebarHD;
            }
        }
        if(MIJI_RET.iReBarsCnt == 3){
            AimBuf->LastSubRebarSite[4] = AimPara->SubRebarSite - (AimPara->iSubMinSite - AimPara->iSubMaxSite)/3;
            AimBuf->LastSubRebarHD[4] = AimPara->SubRebarHD;
            
            AimBuf->LastSubRebarSite[5] = AimPara->SubRebarSite;
            AimBuf->LastSubRebarHD[5] = AimPara->SubRebarHD;
            
            AimBuf->LastSubRebarSite[6] = AimPara->SubRebarSite + (AimPara->iSubMinSite - AimPara->iSubMaxSite)/3;
            AimBuf->LastSubRebarHD[6] = AimPara->SubRebarHD;
        }
        if(MIJI_RET.iReBarsCnt == 4){
            AimBuf->LastSubRebarSite[3] = AimPara->SubRebarSite - (AimPara->iSubMinSite - AimPara->iSubMaxSite)/6*3;
            AimBuf->LastSubRebarHD[3] = AimPara->SubRebarHD;
            
            AimBuf->LastSubRebarSite[4] = AimPara->SubRebarSite - (AimPara->iSubMinSite - AimPara->iSubMaxSite)/6;
            AimBuf->LastSubRebarHD[4] = AimPara->SubRebarHD;
            
            AimBuf->LastSubRebarSite[5] = AimPara->SubRebarSite + (AimPara->iSubMinSite - AimPara->iSubMaxSite)/6;
            AimBuf->LastSubRebarHD[5] = AimPara->SubRebarHD;
            
            AimBuf->LastSubRebarSite[6] = AimPara->SubRebarSite + (AimPara->iSubMinSite - AimPara->iSubMaxSite)/6*3;
            AimBuf->LastSubRebarHD[6] = AimPara->SubRebarHD;
        }

//        #ifdef TEST_ZJC
        //��ʾ���ֵ���
//////        LCD_SetColor(WHITE,PURPLE);
//////        sprintf(dataBuf,"RES:%-d CNT:%-d  ", AimPara->SubRebarHD, MIJI_RET.iReBarsCnt);
//////        LCD_ShowString(ASC16x8,DispSite, 60, 0,dataBuf);
//////        
//////        //��ʾ��ֵ��λ�ֽ�����
//////        LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////        sprintf(dataBuf,"%-d,%-6d",AimPara->SubRebarSite, AimPara->iSubMaxVal-AimPara->iSubMinVal);
//////        LCD_ShowString(ASC16x8,DispSite, 80, 0, dataBuf);
//////        LCD_SetColor(WHITE,PURPLE);//CL_BLUE
//////        sprintf(dataBuf,"S:%-d,%-d,%-d,%-d", AimPara->iSubMinSite-AimPara->iSubMaxSite,MIJI_RET.iTWORebarVal,MIJI_RET.iTHREERebarVal,MIJI_RET.iFOURRebarVal);
//////        LCD_ShowString(ASC16x8,DispSite, 100, 0, dataBuf);
        
//////        LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////        sprintf(dataBuf,"%-6d",AimPara->iSubMaxVal);
//////        LCD_ShowString(ASC16x8, DispSite, 120, 0,dataBuf);
//////        sprintf(dataBuf,"%-6d",AimPara->iSubMaxSite);
//////        LCD_ShowString(ASC16x8, DispSite, 140, 0,dataBuf);
//////        sprintf(dataBuf,"%-6d",AimPara->iSubMinVal);
//////        LCD_ShowString(ASC16x8, DispSite, 160, 0,dataBuf);
//////        sprintf(dataBuf,"%-6d",AimPara->iSubMinSite);
//////        LCD_ShowString(ASC16x8, DispSite, 180, 0,dataBuf);
//        #endif
    }
}

//================================================================//
//���ܣ�ͨ����ֵ���ݶ�λ�ֽ�
//������NULL
//���أ�NULL
//================================================================//
void AimRebarSub(void)
{
    //���㲨�岨�ȵ��ź�����λ��
    CountSubBuffer(CH1, g_iCarDist-1, &AimRebarPara_CH1.ucSubAimStep, &AimRebarBuf_CH1.SubDataBuf_Mark[0], &AimRebarBuf_CH1.SubBoFengSite[0], 
        &AimRebarBuf_CH1.SubBoFengVal[0], &AimRebarBuf_CH1.SubBoGuSite[0], &AimRebarBuf_CH1.SubBoGuVal[0]);
    
    CountSubBuffer(CH2, g_iCarDist-1, &AimRebarPara_CH2.ucSubAimStep, &AimRebarBuf_CH2.SubDataBuf_Mark[0], &AimRebarBuf_CH2.SubBoFengSite[0],
        &AimRebarBuf_CH2.SubBoFengVal[0], &AimRebarBuf_CH2.SubBoGuSite[0], &AimRebarBuf_CH2.SubBoGuVal[0]);

    CountSubBuffer(CH3, g_iCarDist-1, &AimRebarPara_CH3.ucSubAimStep, &AimRebarBuf_CH3.SubDataBuf_Mark[0], &AimRebarBuf_CH3.SubBoFengSite[0],
        &AimRebarBuf_CH3.SubBoFengVal[0], &AimRebarBuf_CH3.SubBoGuSite[0], &AimRebarBuf_CH3.SubBoGuVal[0]);

    //���ݲ��岨����Ϣ��λ�ֽ������ֵ
    SubAimFunc(CH1, &AimRebarPara_CH1, &AimRebarBuf_CH1,  50);
    SubAimFunc(CH2, &AimRebarPara_CH2, &AimRebarBuf_CH2, 200);
    SubAimFunc(CH3, &AimRebarPara_CH3, &AimRebarBuf_CH3, 350);
}

//================================================================//
//���ܣ�����΢�����ݵĶ�λ����
//������NULL
//���أ�NULL
//================================================================//
void CountDifBuffer(enum CHANNELNUM Channel, unsigned int iSearchIndex, unsigned char *ucAimStep, int *AimBuf, 
    short *MarkBofengSite, int *MarkBofengVal, short *MarkBoguSite, int *MarkBoguVal, short *DifWaveCntBuf)
{
    char dataBuf[10];
    int JudgeRange = 0;

    if(g_iCarDist <5){
        return;
    }

    //�ж����塢���Ȳ����뻺��
    if(iSearchIndex > 30){
        //�жϲ��岢���뻺��      ����ֵתλ�ƾ���66*120/400Լ20mm
        if((MarkBofengSite[4] == 0)||((MarkBofengSite[4] != 0)&&(iSearchIndex - 12 - MarkBofengSite[4])>20)){
            //���η�ֵ�ж�
            if(((AimBuf[4] <= AimBuf[3])&&(AimBuf[3] <= AimBuf[2])&&(AimBuf[2] >= AimBuf[1])&&(AimBuf[1] >= AimBuf[0])
///���ô���///                &&(AimBuf[2] > AimBuf[4])&&(AimBuf[2] > AimBuf[0])&&(AimBuf[2] > 10)){//����10ȥ�����ɸ�Ϊʵʱ��ֵ����100
              &&(AimBuf[2] > (AimBuf[4]+JudgeRange))&&(AimBuf[2] > (AimBuf[0]+JudgeRange)))
                ||((DifWaveCntBuf[7] == -2)&&(DifWaveCntBuf[6]==1)&&(DifWaveCntBuf[5]==-1)&&(DifWaveCntBuf[4]>=3))){
                
                //���²�ֵ�ֽ�λ������
                MarkBofengSite[0] = MarkBofengSite[1];
                MarkBofengSite[1] = MarkBofengSite[2];
                MarkBofengSite[2] = MarkBofengSite[3];
                MarkBofengSite[3] = MarkBofengSite[4];
                MarkBofengSite[4] = iSearchIndex - 15 - SITE_OFFSET;
                
                //���²�ֵ�ֽ��ź�����
                MarkBofengVal[0] = MarkBofengVal[1];
                MarkBofengVal[1] = MarkBofengVal[2];
                MarkBofengVal[2] = MarkBofengVal[3];
                MarkBofengVal[3] = MarkBofengVal[4];
                MarkBofengVal[4] = AimBuf[2];
                
                //��λ������
                *ucAimStep = 1;
                
                //������������м�û�ж�λ�����ȣ�����Խ����������λ����Ϊ����
                if(MarkBofengSite[3] > MarkBoguSite[4]){
                    //����λ�ƶ���
                    MarkBoguSite[0] = MarkBoguSite[1];
                    MarkBoguSite[1] = MarkBoguSite[2];
                    MarkBoguSite[2] = MarkBoguSite[3];
                    MarkBoguSite[3] = MarkBoguSite[4];
                    MarkBoguSite[4] = (MarkBofengSite[3]+MarkBofengSite[4])/2 - SITE_OFFSET;
                    
                    //����΢��ֵ����
                    MarkBoguVal[0] = MarkBoguVal[1];
                    MarkBoguVal[1] = MarkBoguVal[2];
                    MarkBoguVal[2] = MarkBoguVal[3];
                    MarkBoguVal[3] = MarkBoguVal[4];
                    MarkBoguVal[4] = MarkBoguVal[3];

                    //��λ������
                    *ucAimStep = 2;
                }
                
//////                if((DifWaveCntBuf[7] == -2)&&(DifWaveCntBuf[6]==1)&&(DifWaveCntBuf[5]==-1)&&(DifWaveCntBuf[4]>=3)){
//////                    LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////                    if(Channel == CH1){
//////                        sprintf(dataBuf,"DifCntCH1:%-6d", iSearchIndex - 15 - SITE_OFFSET);
//////                        LCD_ShowString(ASC16x8, 0, 0, 0,dataBuf);
//////                    }
//////                    else if(Channel == CH2){
//////                        sprintf(dataBuf,"DifCntCH2:%-6d", iSearchIndex - 15 - SITE_OFFSET);
//////                        LCD_ShowString(ASC16x8, 0, 20, 0,dataBuf);
//////                    }
//////                    else if(Channel == CH3){
//////                        sprintf(dataBuf,"DifCntCH3:%-6d", iSearchIndex - 15 - SITE_OFFSET);
//////                        LCD_ShowString(ASC16x8, 0, 40, 0,dataBuf);
//////                    }
//////                }

                //��ӡ��������
//////                LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////                sprintf(dataBuf,"%-6d",AimBuf[0]);
//////                LCD_ShowString(ASC16x8,260, 100, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimBuf[1]);
//////                LCD_ShowString(ASC16x8,260, 120, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimBuf[2]);
//////                LCD_ShowString(ASC16x8,260, 140, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimBuf[3]);
//////                LCD_ShowString(ASC16x8,260, 160, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimBuf[4]);
//////                LCD_ShowString(ASC16x8,260, 180, 0,dataBuf);
            }
        }
        
        //�ж�  ��ֵ   ���Ȳ����뻺��      ����ֵתλ�ƾ���66*120/400Լ20mm
        if((MarkBoguSite[4] == 0)||((MarkBoguSite[4] != 0)&&((iSearchIndex - 15 - MarkBoguSite[4])>20))){//20  150//(g_iDataPutCnt - 12 - AimRebarDifPara.DifMarkSite[4])>45
            //���η�ֵ�ж�
            if((AimBuf[4] >= AimBuf[3])&&(AimBuf[3] >= AimBuf[2])&&(AimBuf[2] <= AimBuf[1])&&(AimBuf[1] <= AimBuf[0])
//                &&(AimBuf[2] < AimBuf[4])&&(AimBuf[2] < AimBuf[0])&&(AimBuf[2] < -10)){
///���ô���///                    &&(AimBuf[2] < AimBuf[4])&&(AimBuf[2] < AimBuf[0])&&(((MarkBofengVal[4] - AimBuf[2]) > 5))){//������MarkBofengVal[4]����һ����ֵ���Ⱥ�
               &&(AimBuf[2] < (AimBuf[4]-JudgeRange))&&(AimBuf[2] < (AimBuf[0]-JudgeRange))){//������MarkBofengVal[4]����һ����ֵ���Ⱥ� 
                //����λ�ƶ���
                MarkBoguSite[0] = MarkBoguSite[1];
                MarkBoguSite[1] = MarkBoguSite[2];
                MarkBoguSite[2] = MarkBoguSite[3];
                MarkBoguSite[3] = MarkBoguSite[4];
                MarkBoguSite[4] = iSearchIndex - 15 - SITE_OFFSET;
                
                //����΢��ֵ����
                MarkBoguVal[0] = MarkBoguVal[1];
                MarkBoguVal[1] = MarkBoguVal[2];
                MarkBoguVal[2] = MarkBoguVal[3];
                MarkBoguVal[3] = MarkBoguVal[4];
                MarkBoguVal[4] = AimBuf[2];

                //��λ������
                *ucAimStep = 2;
                
                //������������м�û�ж�λ�����ȣ�����Խ����������λ����Ϊ����
                if(MarkBofengSite[3] > MarkBoguSite[4]){
                    //���²�ֵ�ֽ�λ������
                    MarkBofengSite[0] = MarkBofengSite[1];
                    MarkBofengSite[1] = MarkBofengSite[2];
                    MarkBofengSite[2] = MarkBofengSite[3];
                    MarkBofengSite[3] = MarkBofengSite[4];
                    MarkBofengSite[4] = (MarkBoguSite[3]+MarkBoguSite[4])/2 - SITE_OFFSET;
                    
                    //���²�ֵ�ֽ��ź�����
                    MarkBofengVal[0] = MarkBofengVal[1];
                    MarkBofengVal[1] = MarkBofengVal[2];
                    MarkBofengVal[2] = MarkBofengVal[3];
                    MarkBofengVal[3] = MarkBofengVal[4];
                    MarkBofengVal[4] = MarkBofengVal[3];
                }
                
//////                //���Թ����������
//////                SourceData.iCh1DifMark[g_iCarDist] = 555;

//////                if(Channel == CH1){
//////                    //��ӡ��������
//////                    LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////                    sprintf(dataBuf,"%-6d",AimBuf[0]);
//////                    LCD_ShowString(ASC16x8,300, 100, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[1]);
//////                    LCD_ShowString(ASC16x8,300, 120, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[2]);
//////                    LCD_ShowString(ASC16x8,300, 140, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[3]);
//////                    LCD_ShowString(ASC16x8,300, 160, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[4]);
//////                    LCD_ShowString(ASC16x8,300, 180, 0,dataBuf);
//////                }
//////                if(Channel == CH2){
//////                    //��ӡ��������
//////                    LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////                    sprintf(dataBuf,"%-6d",AimBuf[0]);
//////                    LCD_ShowString(ASC16x8,360, 100, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[1]);
//////                    LCD_ShowString(ASC16x8,360, 120, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[2]);
//////                    LCD_ShowString(ASC16x8,360, 140, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[3]);
//////                    LCD_ShowString(ASC16x8,360, 160, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[4]);
//////                    LCD_ShowString(ASC16x8,360, 180, 0,dataBuf);
//////                }
//////                if(Channel == CH3){
//////                    //��ӡ��������
//////                    LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////                    sprintf(dataBuf,"%-6d",AimBuf[0]);
//////                    LCD_ShowString(ASC16x8,420, 100, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[1]);
//////                    LCD_ShowString(ASC16x8,420, 120, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[2]);
//////                    LCD_ShowString(ASC16x8,420, 140, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[3]);
//////                    LCD_ShowString(ASC16x8,420, 160, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[4]);
//////                    LCD_ShowString(ASC16x8,420, 180, 0,dataBuf);
//////                }
            }
        }
    }
}

//================================================================//
//���ܣ��������΢�����ݵĶ�λ����
//������NULL
//���أ�NULL
//================================================================//
void CountSecDifBuffer(enum CHANNELNUM Channel, unsigned int iSearchIndex, unsigned char *ucAimStep, int *AimBuf, 
    short *MarkBofengSite, int *MarkBofengVal, short *MarkBoguSite, int *MarkBoguVal, short *DifWaveCntBuf)
{
    char dataBuf[10];
    int JudgeRange = 0;

    if(g_iCarDist <5){
        return;
    }

    //�ж����塢���Ȳ����뻺��
    if(iSearchIndex > 30){
        //�жϲ��岢���뻺��      ����ֵתλ�ƾ���66*120/400Լ20mm
        if((MarkBofengSite[4] == 0)||((MarkBofengSite[4] != 0)&&(iSearchIndex - 12 - MarkBofengSite[4])>20)){
            //���η�ֵ�ж�
            if(((AimBuf[4] <= AimBuf[3])&&(AimBuf[3] <= AimBuf[2])&&(AimBuf[2] >= AimBuf[1])&&(AimBuf[1] >= AimBuf[0])
              &&(AimBuf[2] > (AimBuf[4]+JudgeRange))&&(AimBuf[2] > (AimBuf[0]+JudgeRange)))
                ||((DifWaveCntBuf[7] == -2)&&(DifWaveCntBuf[6]==1)&&(DifWaveCntBuf[5]==-1)&&(DifWaveCntBuf[4]>=3))){
                
                //���²�ֵ�ֽ�λ������
                MarkBofengSite[0] = MarkBofengSite[1];
                MarkBofengSite[1] = MarkBofengSite[2];
                MarkBofengSite[2] = MarkBofengSite[3];
                MarkBofengSite[3] = MarkBofengSite[4];
                MarkBofengSite[4] = iSearchIndex - 15 - SITE_OFFSET;
                
                //���²�ֵ�ֽ��ź�����
                MarkBofengVal[0] = MarkBofengVal[1];
                MarkBofengVal[1] = MarkBofengVal[2];
                MarkBofengVal[2] = MarkBofengVal[3];
                MarkBofengVal[3] = MarkBofengVal[4];
                MarkBofengVal[4] = AimBuf[2];
                
                //��λ������
                *ucAimStep = 1;
                
                //������������м�û�ж�λ�����ȣ�����Խ����������λ����Ϊ����
                if(MarkBofengSite[3] > MarkBoguSite[4]){
                    //����λ�ƶ���
                    MarkBoguSite[0] = MarkBoguSite[1];
                    MarkBoguSite[1] = MarkBoguSite[2];
                    MarkBoguSite[2] = MarkBoguSite[3];
                    MarkBoguSite[3] = MarkBoguSite[4];
                    MarkBoguSite[4] = (MarkBofengSite[3]+MarkBofengSite[4])/2 - SITE_OFFSET;
                    
                    //����΢��ֵ����
                    MarkBoguVal[0] = MarkBoguVal[1];
                    MarkBoguVal[1] = MarkBoguVal[2];
                    MarkBoguVal[2] = MarkBoguVal[3];
                    MarkBoguVal[3] = MarkBoguVal[4];
                    MarkBoguVal[4] = MarkBoguVal[3];

                    //��λ������
                    *ucAimStep = 2;
                }
                
//////                if((DifWaveCntBuf[7] == -2)&&(DifWaveCntBuf[6]==1)&&(DifWaveCntBuf[5]==-1)&&(DifWaveCntBuf[4]>=3)){
//////                    LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////                    if(Channel == CH1){
//////                        sprintf(dataBuf,"DifCntCH1:%-6d", iSearchIndex - 15 - SITE_OFFSET);
//////                        LCD_ShowString(ASC16x8, 0, 0, 0,dataBuf);
//////                    }
//////                    else if(Channel == CH2){
//////                        sprintf(dataBuf,"DifCntCH2:%-6d", iSearchIndex - 15 - SITE_OFFSET);
//////                        LCD_ShowString(ASC16x8, 0, 20, 0,dataBuf);
//////                    }
//////                    else if(Channel == CH3){
//////                        sprintf(dataBuf,"DifCntCH3:%-6d", iSearchIndex - 15 - SITE_OFFSET);
//////                        LCD_ShowString(ASC16x8, 0, 40, 0,dataBuf);
//////                    }
//////                }

                //��ӡ��������
//////                LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////                sprintf(dataBuf,"%-6d",AimBuf[0]);
//////                LCD_ShowString(ASC16x8,260, 100, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimBuf[1]);
//////                LCD_ShowString(ASC16x8,260, 120, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimBuf[2]);
//////                LCD_ShowString(ASC16x8,260, 140, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimBuf[3]);
//////                LCD_ShowString(ASC16x8,260, 160, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimBuf[4]);
//////                LCD_ShowString(ASC16x8,260, 180, 0,dataBuf);
            }
        }
        
        //�ж�  ��ֵ   ���Ȳ����뻺��      ����ֵתλ�ƾ���66*120/400Լ20mm
        if((MarkBoguSite[4] == 0)||((MarkBoguSite[4] != 0)&&((iSearchIndex - 15 - MarkBoguSite[4])>20))){//20  150//(g_iDataPutCnt - 12 - AimRebarDifPara.DifMarkSite[4])>45
            //���η�ֵ�ж�
            if(((AimBuf[4] >= AimBuf[3])&&(AimBuf[3] >= AimBuf[2])&&(AimBuf[2] <= AimBuf[1])&&(AimBuf[1] <= AimBuf[0])
               &&(AimBuf[2] < (AimBuf[4]-JudgeRange))&&(AimBuf[2] < (AimBuf[0]-JudgeRange)))){
                //||((DifWaveCntBuf[7] == -2)&&(DifWaveCntBuf[6]==1)&&(DifWaveCntBuf[5]==-1)&&(DifWaveCntBuf[4]>=3))){
                //����λ�ƶ���
                MarkBoguSite[0] = MarkBoguSite[1];
                MarkBoguSite[1] = MarkBoguSite[2];
                MarkBoguSite[2] = MarkBoguSite[3];
                MarkBoguSite[3] = MarkBoguSite[4];
                MarkBoguSite[4] = iSearchIndex - 15 - SITE_OFFSET;
                
                //����΢��ֵ����
                MarkBoguVal[0] = MarkBoguVal[1];
                MarkBoguVal[1] = MarkBoguVal[2];
                MarkBoguVal[2] = MarkBoguVal[3];
                MarkBoguVal[3] = MarkBoguVal[4];
                MarkBoguVal[4] = AimBuf[2];

                //��λ������
                *ucAimStep = 2;
                
                //������������м�û�ж�λ�����ȣ�����Խ����������λ����Ϊ����
                if(MarkBofengSite[3] > MarkBoguSite[4]){
                    //���²�ֵ�ֽ�λ������
                    MarkBofengSite[0] = MarkBofengSite[1];
                    MarkBofengSite[1] = MarkBofengSite[2];
                    MarkBofengSite[2] = MarkBofengSite[3];
                    MarkBofengSite[3] = MarkBofengSite[4];
                    MarkBofengSite[4] = (MarkBoguSite[3]+MarkBoguSite[4])/2 - SITE_OFFSET;
                    
                    //���²�ֵ�ֽ��ź�����
                    MarkBofengVal[0] = MarkBofengVal[1];
                    MarkBofengVal[1] = MarkBofengVal[2];
                    MarkBofengVal[2] = MarkBofengVal[3];
                    MarkBofengVal[3] = MarkBofengVal[4];
                    MarkBofengVal[4] = MarkBofengVal[3];
                }
                
//////                //���Թ����������
//////                SourceData.iCh1DifMark[g_iCarDist] = 555;

//////                if(Channel == CH1){
//////                    //��ӡ��������
//////                    LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////                    sprintf(dataBuf,"%-6d",AimBuf[0]);
//////                    LCD_ShowString(ASC16x8,300, 100, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[1]);
//////                    LCD_ShowString(ASC16x8,300, 120, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[2]);
//////                    LCD_ShowString(ASC16x8,300, 140, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[3]);
//////                    LCD_ShowString(ASC16x8,300, 160, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[4]);
//////                    LCD_ShowString(ASC16x8,300, 180, 0,dataBuf);
//////                }
//////                if(Channel == CH2){
//////                    //��ӡ��������
//////                    LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////                    sprintf(dataBuf,"%-6d",AimBuf[0]);
//////                    LCD_ShowString(ASC16x8,360, 100, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[1]);
//////                    LCD_ShowString(ASC16x8,360, 120, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[2]);
//////                    LCD_ShowString(ASC16x8,360, 140, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[3]);
//////                    LCD_ShowString(ASC16x8,360, 160, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[4]);
//////                    LCD_ShowString(ASC16x8,360, 180, 0,dataBuf);
//////                }
//////                if(Channel == CH3){
//////                    //��ӡ��������
//////                    LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////                    sprintf(dataBuf,"%-6d",AimBuf[0]);
//////                    LCD_ShowString(ASC16x8,420, 100, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[1]);
//////                    LCD_ShowString(ASC16x8,420, 120, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[2]);
//////                    LCD_ShowString(ASC16x8,420, 140, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[3]);
//////                    LCD_ShowString(ASC16x8,420, 160, 0,dataBuf);
//////                    sprintf(dataBuf,"%-6d",AimBuf[4]);
//////                    LCD_ShowString(ASC16x8,420, 180, 0,dataBuf);
//////                }
            }
        }
    }
}

//================================================================//
//���ܣ���λ�ֽ��ʾ΢�ֶ�λ�ֽ����
//������NULL
//���أ�NULL
//================================================================//
void DifAimFunc(enum CHANNELNUM Channel, AIMREBAR_PARA_STR *AimPara, AIMREBAR_BUF_STR *AimBuf, int DispSite)
{
//    char dataBuf[10];
    int i = 0;
    char dataBuf[10];

    if(AimPara->ucDifAimStep == 2){
        
        AimPara->ucDifAimStep = 3;
        
        AimPara->iDifMinVal = AimBuf->DifBoGuVal[4];
        AimPara->iDifMinSite = AimBuf->DifBoGuSite[4];

        //��λ΢�ֲ���
        for(i=4;i>=0;i--){
            if(AimBuf->DifBoFengVal[i] > AimPara->iDifMaxVal){
                AimPara->iDifMaxVal = AimBuf->DifBoFengVal[i];
                AimPara->iDifMaxSite = AimBuf->DifBoFengSite[i];
            }
            if(AimBuf->DifBoFengSite[i] <= AimBuf->DifBoGuSite[3]){
                break;
            }
        }

        //����ֽ�λ��
//        AimPara->LastDifRebarSite = AimPara->CurRebarSite;
//        AimPara->DifRebarSite = AimPara->iSubMinSite;

        //����΢�ֶ�λ��ʷ����
        AimBuf->LastDifMaxVal = AimPara->iDifMaxVal;
        AimBuf->LastDifMaxSite = AimPara->iDifMaxSite;
        AimBuf->LastDifMinVal = AimPara->iDifMinVal;
        AimBuf->LastDifMinSite = AimPara->iDifMinSite;
        
        
        if((abs(AimBuf->DifBoFengVal[4]-AimBuf->DifBoGuVal[3])/abs(AimBuf->DifBoFengVal[4]-AimBuf->DifBoFengVal[3])<5)
            &&(AimBuf->DifBoFengVal[4]-AimBuf->DifBoGuVal[4]>5)){
                
            //���㲢��ʾ���ֵ
            iSumMaxHD = FindCoverTab_Sum(Channel, AimBuf->LastSumRebarVal, g_tWorkPara, AimBuf);//��ֵ��������

            //����ֽ�λ�ü��������
            iSumMaxHD = iSumMaxHD+5;
            for(i=0;i<6;i++){
                AimBuf->LastDifRebarHD[i] = AimBuf->LastDifRebarHD[i+1];
            }
            AimBuf->LastDifRebarHD[6] = iSumMaxHD;
            
            for(i=0;i<6;i++){
                AimBuf->LastDifRebarSite[i] = AimBuf->LastDifRebarSite[i+1];
            }
            AimBuf->LastDifRebarSite[6] = AimBuf->LastDifMinSite;
            
            for(i=0;i<6;i++){
                AimBuf->LastDifRebarVal[i] = AimBuf->LastDifRebarVal[i+1];
            }
            AimBuf->LastDifRebarVal[6] = AimBuf->LastDifMinVal;
            
            
            //TEST_ZJC
            if(Channel == CH2){
                LCD_SetColor(WHITE,ORANGE);//CL_BLUE
                sprintf(dataBuf,"DIF:1  %d  %d",AimBuf->LastDifMaxVal,AimBuf->LastDifMinVal);//sprintf�����ǽ��ַ�����ʽ��Ϊ������
                LCD_ShowString(ASC16x8,0, 180, 0,dataBuf);
                i=0;
            }
            //END
            
        }
        else{
            //�����־λ
            AimPara->ucDifAimStep = 0;
        }
        
        
        
        

        //��ʾ��ֵ��λ�ֽ�����
//        LCD_SetColor(WHITE,ORANGE);//CL_BLUE
//        sprintf(dataBuf,"%-6d",AimPara->LastSubMaxVal-AimPara->LastSubMinVal);
//        LCD_ShowString(ASC16x8,DispSite, 80, 0,dataBuf);
//        sprintf(dataBuf,"%-d,%-d",AimPara->SubRebarSite, AimPara->LastSubMinSite-AimPara->LastSubMaxSite);
//        LCD_ShowString(ASC16x8,DispSite, 100, 0,dataBuf);

//        sprintf(dataBuf,"%-6d",AimPara->LastSubMaxVal);
//        LCD_ShowString(ASC16x8, DispSite, 120, 0,dataBuf);
//        sprintf(dataBuf,"%-6d",AimPara->LastSubMaxSite);
//        LCD_ShowString(ASC16x8, DispSite, 140, 0,dataBuf);
        
        #ifdef TEST_ZJC
        LCD_SetColor(WHITE,ORANGE);
        sprintf(dataBuf,"%-6d,%-6d", AimBuf->LastDifMinVal, AimBuf->LastDifMinSite);
        LCD_ShowString(ASC16x8, DispSite, 220, 0,dataBuf);
        sprintf(dataBuf,"%-6d",AimBuf->LastDifRebarHD[6]);
        LCD_ShowString(ASC16x8, DispSite, 240, 0,dataBuf);
        #endif
    }
}

//================================================================//
//���ܣ�ͨ��΢�����ݶ�λ�ֽ�
//������NULL
//���أ�NULL
//================================================================//
void AimRebarDif(void)
{
    //����΢�ֲ��岨�ȵ��ź�����λ��
    CountDifBuffer(CH2, g_iCarDist-1, &AimRebarPara_CH2.ucDifAimStep, &AimRebarBuf_CH2.DifDataBuf_Mark[0], &AimRebarBuf_CH2.DifBoFengSite[0], 
        &AimRebarBuf_CH2.DifBoFengVal[0], &AimRebarBuf_CH2.DifBoGuSite[0], &AimRebarBuf_CH2.DifBoGuVal[0], &AimRebarBuf_CH2.DifWaveDirCnt[0]);
    
    CountDifBuffer(CH1, g_iCarDist-1, &AimRebarPara_CH1.ucDifAimStep, &AimRebarBuf_CH1.DifDataBuf_Mark[0], &AimRebarBuf_CH1.DifBoFengSite[0], 
        &AimRebarBuf_CH1.DifBoFengVal[0], &AimRebarBuf_CH1.DifBoGuSite[0], &AimRebarBuf_CH1.DifBoGuVal[0], &AimRebarBuf_CH1.DifWaveDirCnt[0]);
    
    CountDifBuffer(CH3, g_iCarDist-1, &AimRebarPara_CH3.ucDifAimStep, &AimRebarBuf_CH3.DifDataBuf_Mark[0], &AimRebarBuf_CH3.DifBoFengSite[0], 
        &AimRebarBuf_CH3.DifBoFengVal[0], &AimRebarBuf_CH3.DifBoGuSite[0], &AimRebarBuf_CH3.DifBoGuVal[0], &AimRebarBuf_CH3.DifWaveDirCnt[0]);
    
    //�������΢�ֲ��岨�ȵ��ź�����λ��
    CountSecDifBuffer(CH2, g_iCarDist-1, &AimRebarPara_CH2.ucSecDifAimStep, &AimRebarBuf_CH2.SecDifDataBuf_Mark[0], &AimRebarBuf_CH2.SecDifBoFengSite[0], 
        &AimRebarBuf_CH2.SecDifBoFengVal[0], &AimRebarBuf_CH2.SecDifBoGuSite[0], &AimRebarBuf_CH2.SecDifBoGuVal[0], &AimRebarBuf_CH2.SecDifWaveDirCnt[0]);
    
    CountSecDifBuffer(CH1, g_iCarDist-1, &AimRebarPara_CH1.ucSecDifAimStep, &AimRebarBuf_CH1.SecDifDataBuf_Mark[0], &AimRebarBuf_CH1.SecDifBoFengSite[0], 
        &AimRebarBuf_CH1.SecDifBoFengVal[0], &AimRebarBuf_CH1.SecDifBoGuSite[0], &AimRebarBuf_CH1.SecDifBoGuVal[0], &AimRebarBuf_CH1.SecDifWaveDirCnt[0]);
    
    CountSecDifBuffer(CH3, g_iCarDist-1, &AimRebarPara_CH3.ucSecDifAimStep, &AimRebarBuf_CH3.SecDifDataBuf_Mark[0], &AimRebarBuf_CH3.SecDifBoFengSite[0], 
        &AimRebarBuf_CH3.SecDifBoFengVal[0], &AimRebarBuf_CH3.SecDifBoGuSite[0], &AimRebarBuf_CH3.SecDifBoGuVal[0], &AimRebarBuf_CH3.SecDifWaveDirCnt[0]);

    //���ݲ��岨����Ϣ��λ�ֽ������ֵ
    DifAimFunc(CH1, &AimRebarPara_CH1, &AimRebarBuf_CH1,  50);
    DifAimFunc(CH2, &AimRebarPara_CH2, &AimRebarBuf_CH2, 200);
    DifAimFunc(CH3, &AimRebarPara_CH3, &AimRebarBuf_CH3, 350);
}


//================================================================//
//���ܣ���׼��λ�ֽ����ݼ���
//������NULL
//���أ�NULL
//================================================================//
//void CountMZKBuffer(unsigned int iSearchIndex, int *SubSourceData, int *SumSourceData, unsigned char *ucAimStep, int *AimBuf, 
//    short *ZeroSite, int *ZeroVal)
void CountMZKBuffer(unsigned int iSearchIndex, short *SourceDataLeft, short *SourceDataRight, unsigned char *ucAimStep, int *AimBuf, 
    short *ZeroSite, int *ZeroVal)
{
//    char dataBuf[10];
    int i = 0;
    int SubSourceData[15] = {0};
    if(g_iCarDist <5){
        return;
    }
    
    for(i=0;i<=6;i++){
        SubSourceData[i] = SourceDataRight[iSearchIndex-i*2] - SourceDataLeft[iSearchIndex-i*2];
    }

    //��׼��λ�ֽ��о�
    if(iSearchIndex > 10){
        //�жϲ��岢���뻺��      ����ֵתλ�ƾ���66*120/400Լ20mm
        if((ZeroSite[4] == 0)||((ZeroSite[4] != 0)&&(iSearchIndex - ZeroSite[4])>20)){
            //���η�ֵ�ж�
//            if((SubSourceData[iSearchIndex] <= SubSourceData[iSearchIndex-2])&&(SubSourceData[iSearchIndex-2] <= SubSourceData[iSearchIndex-4])
//                &&(SubSourceData[iSearchIndex-4] <= SubSourceData[iSearchIndex-6])&&(SubSourceData[iSearchIndex-6] <= SubSourceData[iSearchIndex-8])
//                    &&(SubSourceData[iSearchIndex-8] <= SubSourceData[iSearchIndex-10])&&(SubSourceData[iSearchIndex-10] <= SubSourceData[iSearchIndex-12])
//                        &&(SubSourceData[iSearchIndex] < 0)&&((SubSourceData[iSearchIndex-2] >= 0))){
            if((SubSourceData[0] <= SubSourceData[1])&&(SubSourceData[1] <= SubSourceData[2])
                &&(SubSourceData[2] <= SubSourceData[3])&&(SubSourceData[3] <= SubSourceData[4])
                    &&(SubSourceData[4] <= SubSourceData[5])&&(SubSourceData[5] <= SubSourceData[6])
                        &&(SubSourceData[0] < 0)&&((SubSourceData[1] >= 0))){
               
                //���²�ֵ�ֽ�λ������
                ZeroSite[0] = ZeroSite[1];
                ZeroSite[1] = ZeroSite[2];
                ZeroSite[2] = ZeroSite[3];
                ZeroSite[3] = ZeroSite[4];
                ZeroSite[4] = iSearchIndex;
                
                //���²�ֵ�ֽ��ź�����
                ZeroVal[0] = ZeroVal[1];
                ZeroVal[1] = ZeroVal[2];
                ZeroVal[2] = ZeroVal[3];
                ZeroVal[3] = ZeroVal[4];
                ZeroVal[4] = SourceDataLeft[iSearchIndex] + SourceDataRight[iSearchIndex];
                
                //��λ���ֽ�
                *ucAimStep = 1;
                
                //��ӡ��������
//////                LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////                sprintf(dataBuf,"%-6d",AimBuf[0]);
//////                LCD_ShowString(ASC16x8,260, 100, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimBuf[1]);
//////                LCD_ShowString(ASC16x8,260, 120, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimBuf[2]);
//////                LCD_ShowString(ASC16x8,260, 140, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimBuf[3]);
//////                LCD_ShowString(ASC16x8,260, 160, 0,dataBuf);
//////                sprintf(dataBuf,"%-6d",AimBuf[4]);
//////                LCD_ShowString(ASC16x8,260, 180, 0,dataBuf);
            }
        }
    }
}

//================================================================//
//���ܣ���׼��λ�ֽ�
//������NULL
//���أ�NULL
//================================================================//
void MZKAimFunc(enum CHANNELNUM Channel, AIMREBAR_PARA_STR *AimPara, AIMREBAR_BUF_STR *AimBuf, int DispSite)
{
//    char dataBuf[10];
    int i = 0;
//    MIJI_RET MIJI_RET;
    
    if(AimPara->ucMZKAimStep == 1){
        
//////        //����ʱ����ӡ��ֵ����
//////        iCurCnt = index/6*6;
//////        for(i=0;i<5;i++){
//////            SourceData.iCh3DifMark[iCurCnt-5+i] = SubAimBuf[i];
//////        }
//////        SourceData.iCh3DifMark[iCurCnt-5+i] = 0;

        //��¼��λ���ĸֽ�λ��
        AimPara->iMZKZeroVal = AimBuf->MZKZeroVal[4];
        AimPara->iMZKZeroSite = AimBuf->MZKZeroSite[4];
        
        //�жϲ��岨�Ȳ�ֵ�Ƿ��������Ҫ��
//////        if((AimPara->iSubMaxVal - AimPara->iSubMinVal) <= 20){
//////            return;
//////        }

        //����ֽ�λ��
        AimPara->MZKRebarSite = AimPara->iMZKZeroSite;
        
        //���㲢��ʾ���ֵ
        AimPara->MZKRebarHD = FindCoverTab_Sum(Channel, AimPara->iMZKZeroVal, g_tWorkPara, AimBuf);//��ֵ��������
//        AimPara->MZKRebarHD = FindCoverTab_Sub(Channel, AimPara->iSubMaxVal-AimPara->iSubMinVal, g_tWorkPara)+10;//��ֵ��������
        AimPara->MZKRebarHD = (AimPara->MZKRebarHD+5)/10;
        
        //��¼��ʷ�ֽ�λ�úͺ��
        for(i=1;i<=6;i++){
            AimBuf->LastMZKRebarSite[i-1] = AimBuf->LastMZKRebarSite[i];
            AimBuf->LastMZKRebarHD[i-1] = AimBuf->LastMZKRebarHD[i];
        }

        AimBuf->LastMZKRebarSite[6] = AimPara->MZKRebarSite;
        AimBuf->LastMZKRebarHD[6] = AimPara->MZKRebarHD;

        #ifdef TEST_ZJC
        //��ʾ���ֵ���
        LCD_SetColor(WHITE,PURPLE);
        sprintf(dataBuf,"RES:%-d CNT:%-d  ", AimPara->SubRebarHD, MIJI_RET.iReBarsCnt);
        LCD_ShowString(ASC16x8,DispSite, 60, 0,dataBuf);
        
        //��ʾ��ֵ��λ�ֽ�����
        LCD_SetColor(WHITE,BLUE);//CL_BLUE
        sprintf(dataBuf,"%-d,%-6d",AimPara->SubRebarSite, AimPara->iSubMaxVal-AimPara->iSubMinVal);
        LCD_ShowString(ASC16x8,DispSite, 80, 0, dataBuf);
        LCD_SetColor(WHITE,PURPLE);//CL_BLUE
        sprintf(dataBuf,"S:%-d,%-d,%-d,%-d", AimPara->iSubMinSite-AimPara->iSubMaxSite,MIJI_RET.iTWORebarVal,MIJI_RET.iTHREERebarVal,MIJI_RET.iFOURRebarVal);
        LCD_ShowString(ASC16x8,DispSite, 100, 0, dataBuf);
        
        LCD_SetColor(WHITE,BLUE);//CL_BLUE
        sprintf(dataBuf,"%-6d",AimPara->iSubMaxVal);
        LCD_ShowString(ASC16x8, DispSite, 120, 0,dataBuf);
        sprintf(dataBuf,"%-6d",AimPara->iSubMaxSite);
        LCD_ShowString(ASC16x8, DispSite, 140, 0,dataBuf);
        sprintf(dataBuf,"%-6d",AimPara->iSubMinVal);
        LCD_ShowString(ASC16x8, DispSite, 160, 0,dataBuf);
        sprintf(dataBuf,"%-6d",AimPara->iSubMinSite);
        LCD_ShowString(ASC16x8, DispSite, 180, 0,dataBuf);
        #endif
    }
}
//================================================================//
//���ܣ�ͨ����׼�����ݶ�λ�ֽ�
//������NULL
//���أ�NULL
//================================================================//
void AimRebarMZK(void)
{
    //��ֵ  ��λ���ݵĻ�����㼰���̱�־λ��λ
//////    CountSubBuffer(g_iCarDist-1, &SourceData.iCh1Sub[0], &AimRebarPara_CH1.ucSubAimStep, &AimRebarBuf_CH1.SubDataBuf_Mark[0],
//////        &AimRebarBuf_CH1.SubBoFengSite[0], &AimRebarBuf_CH1.SubBoFengVal[0], &AimRebarBuf_CH1.SubBoGuSite[0], &AimRebarBuf_CH1.SubBoGuVal[0]);
//////    
//////    CountSubBuffer(g_iCarDist-1, &SourceData.iCh2Sub[0], &AimRebarPara_CH2.ucSubAimStep, &AimRebarBuf_CH2.SubDataBuf_Mark[0],
//////        &AimRebarBuf_CH2.SubBoFengSite[0], &AimRebarBuf_CH2.SubBoFengVal[0], &AimRebarBuf_CH2.SubBoGuSite[0], &AimRebarBuf_CH2.SubBoGuVal[0]);

//////    CountSubBuffer(g_iCarDist-1, &SourceData.iCh3Sub[0], &AimRebarPara_CH3.ucSubAimStep, &AimRebarBuf_CH3.SubDataBuf_Mark[0],
//////        &AimRebarBuf_CH3.SubBoFengSite[0], &AimRebarBuf_CH3.SubBoFengVal[0], &AimRebarBuf_CH3.SubBoGuSite[0], &AimRebarBuf_CH3.SubBoGuVal[0]);
    
    //��׼��  ��λ���ݵĻ�����㼰���̱�־λ��λ
    CountMZKBuffer(g_iCarDist-1, &SourceData.iCH1[0], &SourceData.iCH2[0], &AimRebarPara_CH1.ucMZKAimStep, &AimRebarBuf_CH1.MZKDataBuf_Mark[0],
        &AimRebarBuf_CH1.MZKZeroSite[0], &AimRebarBuf_CH1.MZKZeroVal[0]);
    
    CountMZKBuffer(g_iCarDist-1, &SourceData.iCH3[0], &SourceData.iCH4[0], &AimRebarPara_CH2.ucMZKAimStep, &AimRebarBuf_CH2.MZKDataBuf_Mark[0],
        &AimRebarBuf_CH2.MZKZeroSite[0], &AimRebarBuf_CH2.MZKZeroVal[0]);

    CountMZKBuffer(g_iCarDist-1, &SourceData.iCH5[0], &SourceData.iCH6[0], &AimRebarPara_CH3.ucMZKAimStep, &AimRebarBuf_CH3.MZKDataBuf_Mark[0],
        &AimRebarBuf_CH3.MZKZeroSite[0], &AimRebarBuf_CH3.MZKZeroVal[0]);

    //��׼��  ���ݲ�ֵ���岨����Ϣ��λ�ֽ������ֵ
//    MZKAimFunc(CH1, &AimRebarPara_CH1, &AimRebarBuf_CH1,  50);
    MZKAimFunc(CH2, &AimRebarPara_CH2, &AimRebarBuf_CH2, 200);
//    MZKAimFunc(CH3, &AimRebarPara_CH3, &AimRebarBuf_CH3, 350);
    
    //��ֵ  ���ݲ��岨����Ϣ��λ�ֽ������ֵ
//////    SubAimFunc(CH1, &AimRebarPara_CH1, &AimRebarBuf_CH1,  50);
//////    SubAimFunc(CH2, &AimRebarPara_CH2, &AimRebarBuf_CH2, 200);
//////    SubAimFunc(CH3, &AimRebarPara_CH3, &AimRebarBuf_CH3, 350);
}

//================================================================//
//���ܣ��ۺ������㷨�ĺ�����ݣ��õ��������ݽ��
//������NULL
//���أ�NULL
//================================================================//
void AnalyzeCoverData_Sub(enum CHANNELNUM Channel, AIMREBAR_PARA_STR *AimPara, COVER_BUFFER *Res, AIMREBAR_BUF_STR *AimBuf)
{
    int i=0;
    
    short RebarSiteBuf[7] = {0};
    short RebarCoverBuf[7] = {0};
    char cAllBufCnt = 0;
    char cDifBufCnt = 0;
    char dataBuf[10];
    int SubPercent = 0;

    //��ֵ�㷨��λ���ֽ�
    if(AimPara->ucSubAimStep == 2){
        
        //�����־λ
        AimPara->ucSubAimStep = 0;
        AimPara->ucDifAimStep = 0;
        
        //ɸѡ��ֵ�㷨��λ���ĸֽ�
//////        for(i=0;i<=6;i++){//ɸѡ��ֵ���岨��λ���ڵĸֽ�
//////            if(((AimBuf->LastSubRebarSite[i] != 0)&&(AimBuf->LastSubRebarSite[i] < AimPara->iSubMaxSite))
//////                ||(AimBuf->LastSubRebarSite[i]==0)){
//////                continue;
//////            }
//////            RebarSiteBuf[cAllBufCnt] = AimBuf->LastSubRebarSite[i];
//////            RebarCoverBuf[cAllBufCnt] = AimBuf->LastSubRebarHD[i];
//////            cAllBufCnt ++;
//////        }
        for(i=6;i>(6-AimBuf->iLastSubRebarCnt);i--){//��ֵ�ֽ��������ʱ�õ��ĸֽ����
            RebarSiteBuf[cAllBufCnt] = AimBuf->LastSubRebarSite[i];
            RebarCoverBuf[cAllBufCnt] = AimBuf->LastSubRebarHD[i];
            cAllBufCnt ++;
        }
        
        //����΢�ֶ�λ���ĸֽ�ĸ���
        cDifBufCnt ++;//�ֽ�AimBuf->LastDifRebarSite[6]�����ж�ֱ���㵽�ֽ���
        for(i=5;i>=5;i--){
            if((AimBuf->LastDifRebarVal[i] >= 0)&&(AimBuf->LastDifRebarSite[i] > 0)){
                cDifBufCnt ++;
            }
        }
//////        if(Channel == CH1){
//////            sprintf(dataBuf,"1:%d",AimPara->iSubMaxSite);
//////            LCD_ShowString(ASC16x8,190, 0, 0,dataBuf);
//////        }
//////        else if(Channel == CH2){
//////            sprintf(dataBuf,"2:%d",AimPara->iSubMaxSite);
//////            LCD_ShowString(ASC16x8,190, 20, 0,dataBuf);
//////        }
//////        else if(Channel == CH3){
//////            sprintf(dataBuf,"3:%d",AimPara->iSubMaxSite);
//////            LCD_ShowString(ASC16x8,190, 40, 0,dataBuf);
//////        }
        //ɸѡ΢���㷨��λ���ĸֽ������ʱ����,΢�ֶ�λ�������ֽ��ֵһ����ȷ���ֽ����λ��
        if((cDifBufCnt == 2)&&(cAllBufCnt == 1)&&(AimBuf->LastDifRebarSite[5] != Res->RebarSite[Res->AllRebarCnt-1])
            &&(AimBuf->LastDifRebarSite[5] > AimPara->iSubMaxSite)){
            if(abs(AimBuf->LastDifRebarSite[5] - RebarSiteBuf[0]) < abs(AimBuf->LastDifRebarSite[6] - RebarSiteBuf[0])){
                RebarSiteBuf[1] = AimBuf->LastDifRebarSite[6];
                RebarCoverBuf[1] = AimBuf->LastDifRebarHD[6];
            }
            else{
                RebarSiteBuf[1] = RebarSiteBuf[0];
                RebarCoverBuf[1] = RebarCoverBuf[0];
                RebarSiteBuf[0] = AimBuf->LastDifRebarSite[5];
                RebarCoverBuf[0] = AimBuf->LastDifRebarHD[5];
            }
            cAllBufCnt = 2;
//////            LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////            sprintf(dataBuf,"CH:%d,SCnt:1,DCnt:2",Channel);
//////            LCD_ShowString(ASC16x8, 0, 0, 0, dataBuf);
        }

        //����⵽�ĸֽ���뻺��
        for(i=0;i<cAllBufCnt;i++){
            Res->RebarSite[Res->AllRebarCnt+i] = RebarSiteBuf[i];
            Res->RebarCover[Res->AllRebarCnt+i] = RebarCoverBuf[i];
        }
        Res->AllRebarCnt += cAllBufCnt;
        Res->AddRebatCnt = cAllBufCnt;
        
        //������ʷ����
        AimBuf->iLastSubMinVal = AimPara->iSubMinVal;
        AimBuf->iLastSubMinSite = AimPara->iSubMinSite;
        AimBuf->iLastSubMaxVal = AimPara->iSubMaxVal;
        AimBuf->iLastSubMaxSite = AimPara->iSubMaxSite;
        
//        #ifdef TEST_ZJC
        if(cAllBufCnt == 2){
            if(cDifBufCnt == 2){
                sprintf(dataBuf,"DCnt:2");
            }
            else{
                sprintf(dataBuf,"SCnt:2");
            }
        }
        else{
            sprintf(dataBuf,"SCnt:1");
        }
        
        LCD_SetColor(WHITE,BLUE);//CL_BLUE
        if(Channel == CH1){
            LCD_ShowString(ASC16x8, 200+(Res->AllRebarCnt-cAllBufCnt)*60, 180, 0, dataBuf);
        }
        else if(Channel == CH2){
            LCD_ShowString(ASC16x8, 200+(Res->AllRebarCnt-cAllBufCnt)*60, 200, 0, dataBuf);
        }
        else if(Channel == CH3){
            LCD_ShowString(ASC16x8, 200+(Res->AllRebarCnt-cAllBufCnt)*60, 220, 0, dataBuf);
        }
//        #endif
        
        if(Channel == CH1){
            InitAimRebarParaCH1();
        }
        else if(Channel == CH2){
            InitAimRebarParaCH2();
        }
        else if(Channel == CH3){
            InitAimRebarParaCH3();
        }
    }
    //΢���㷨��λ���ֽ�
    else if((AimPara->ucSubAimStep == 0)&&(AimPara->ucDifAimStep == 3)){
        
        //�����־λ
        AimPara->ucSubAimStep = 0;
        AimPara->ucDifAimStep = 0;
        
        //����΢�ֶ�λ���ĸֽ�ĸ���
//////        for(i=0;i<=6;i++){
//////            if(AimBuf->LastDifRebarSite[i] > AimBuf->SubBoGuSite[4]){
//////                cDifBufCnt ++;
//////            }
//////        }
        
        //ɸѡ��ֵ�㷨��λ���ĸֽ�   
        //�ж�ǰһ����ֵ���岨�ȵı�ֵ��ȷ���Ƿ�Ϊ�ֽ��΢�ֲ��ȣ������Ͻ��Ĳ�ֵ�����м�Ҳ����΢�ֲ���
//////        if(abs(AimBuf->iLastSubMaxVal) > abs(AimBuf->iLastSubMinVal)){
//////            SubPercent = 1000/(AimBuf->iLastSubMaxVal/(AimBuf->iLastSubMaxVal+AimBuf->iLastSubMinVal));
//////        }
//////        else{
//////            SubPercent = 1000/(AimBuf->iLastSubMinVal/(AimBuf->iLastSubMaxVal+AimBuf->iLastSubMinVal));
//////        }

//////        LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////        if(Channel == CH1){
//////            sprintf(dataBuf,"1:%-6d,%-6d,%-6d",SubPercent,AimBuf->iLastSubMaxVal,AimBuf->iLastSubMinVal);
//////            LCD_ShowString(ASC16x8,260, 30, 0,dataBuf);
//////        }
//////        else if(Channel == CH2){
//////            sprintf(dataBuf,"2:%-6d%-6d,%-6d",SubPercent,AimBuf->iLastSubMaxVal,AimBuf->iLastSubMinVal);
//////            LCD_ShowString(ASC16x8,260, 50, 0,dataBuf);
//////        }
//////        else if(Channel == CH3){
//////            sprintf(dataBuf,"3:%-6d%-6d,%-6d",SubPercent,AimBuf->iLastSubMaxVal,AimBuf->iLastSubMinVal);
//////            LCD_ShowString(ASC16x8,260, 70, 0,dataBuf);
//////        }

//////        if(((AimBuf->LastDifMaxVal-AimBuf->LastDifMinVal) < 30)||(AimBuf->LastDifMaxSite == 0)||(abs(SubPercent) < 150)){
        //�Ʋ⣺��SubPercent����������������ڣ�����ȥ�����ֵ��ֵ���о�
//////        if((AimBuf->LastDifMaxSite == 0)||(abs(SubPercent) < 150)){
//////            if(Channel == CH1){
//////                InitAimRebarParaCH1();
////////////                sprintf(dataBuf,"1:clear");
////////////                LCD_ShowString(ASC16x8,190, 0, 0,dataBuf);
//////            }
//////            else if(Channel == CH2){
//////                InitAimRebarParaCH2();
////////////                sprintf(dataBuf,"2:clear");
////////////                LCD_ShowString(ASC16x8,190, 20, 0,dataBuf);
//////            }
//////            else if(Channel == CH3){
//////                InitAimRebarParaCH3();
////////////                sprintf(dataBuf,"3:clear");
////////////                LCD_ShowString(ASC16x8,190, 40, 0,dataBuf);
//////            }
//////            return;
//////        }
        
//////        if(Channel == CH2){
//////            LCD_SetColor(WHITE,BLUE);//CL_BLUE
//////            sprintf(dataBuf,"%-6d",AimBuf->LastDifMaxVal);
//////            LCD_ShowString(ASC16x8,260, 100, 0,dataBuf);
//////            sprintf(dataBuf,"%-6d",AimBuf->LastDifMinVal);
//////            LCD_ShowString(ASC16x8,260, 120, 0,dataBuf);
//////            sprintf(dataBuf,"%-6d",AimBuf->LastDifMaxSite);
//////            LCD_ShowString(ASC16x8,260, 140, 0,dataBuf);
//////            sprintf(dataBuf,"%-6d",AimBuf->LastDifMinSite);
//////            LCD_ShowString(ASC16x8,260, 160, 0,dataBuf);
//////        }
        if((AimBuf->LastDifRebarSite[6]-AimBuf->LastSubRebarSite[6]) > 30){
            RebarSiteBuf[cAllBufCnt] = AimBuf->LastDifRebarSite[6];
            RebarCoverBuf[cAllBufCnt] = AimBuf->LastDifRebarHD[6];
            cAllBufCnt ++;

            //����⵽�ĸֽ���뻺��
            for(i=0;i<cAllBufCnt;i++){
                Res->RebarSite[Res->AllRebarCnt+i] = RebarSiteBuf[i];
                Res->RebarCover[Res->AllRebarCnt+i] = RebarCoverBuf[i];
            }
            Res->AllRebarCnt += cAllBufCnt;
            Res->AddRebatCnt = cAllBufCnt;
            
            //������ʷ����
            AimBuf->iLastSubMinVal = AimPara->iSubMinVal;
            AimBuf->iLastSubMinSite = AimPara->iSubMinSite;
            AimBuf->iLastSubMaxVal = AimPara->iSubMaxVal;
            AimBuf->iLastSubMaxSite = AimPara->iSubMaxSite;
            
    //        #ifdef TEST_ZJC
            sprintf(dataBuf,"DCnt:1");
            
            LCD_SetColor(WHITE,RED);//CL_BLUE
            if(Channel == CH1){
                LCD_ShowString(ASC16x8, 200+(Res->AllRebarCnt-cAllBufCnt)*60, 180, 0, dataBuf);
            }
            else if(Channel == CH2){
                LCD_ShowString(ASC16x8, 200+(Res->AllRebarCnt-cAllBufCnt)*60, 200, 0, dataBuf);
            }
            else if(Channel == CH3){
                LCD_ShowString(ASC16x8, 200+(Res->AllRebarCnt-cAllBufCnt)*60, 220, 0, dataBuf);
            }
    //        #endif
            
            if(Channel == CH1){
                InitAimRebarParaCH1();
            }
            else if(Channel == CH2){
                InitAimRebarParaCH2();
            }
            else if(Channel == CH3){
                InitAimRebarParaCH3();
            }
        }
    }
}

//================================================================//
//���ܣ��ۺϷ��������㷨�Ĵ�����
//������NULL
//���أ�NULL
//================================================================//
void AimRebarAnanlze_Sub(void)
{
    AnalyzeCoverData_Sub(CH2, &AimRebarPara_CH2, &ResBuf_CH2, &AimRebarBuf_CH2);
    AnalyzeCoverData_Sub(CH1, &AimRebarPara_CH1, &ResBuf_CH1, &AimRebarBuf_CH1);
    AnalyzeCoverData_Sub(CH3, &AimRebarPara_CH3, &ResBuf_CH3, &AimRebarBuf_CH3);
}

//================================================================//
//���ܣ��ۺ������㷨�ĺ�����ݣ��õ��������ݽ��
//������NULL
//���أ�NULL
//================================================================//
void AnalyzeCoverData_MZK(enum CHANNELNUM Channel, AIMREBAR_PARA_STR *AimPara, COVER_BUFFER *Res, AIMREBAR_BUF_STR *AimBuf)
{

    char cAllBufCnt = 0;
    if(AimPara->ucMZKAimStep != 1){
        return;
    }

    //�����־λ
    AimPara->ucMZKAimStep = 0;
    
    //����⵽�ĸֽ���뻺��
    Res->RebarSite[Res->AllRebarCnt] = AimBuf->LastMZKRebarSite[6];
    Res->RebarCover[Res->AllRebarCnt] = AimBuf->LastMZKRebarHD[6];

    cAllBufCnt = 1;
    Res->AllRebarCnt += cAllBufCnt;
    Res->AddRebatCnt = cAllBufCnt;
    
    //������ʷ����
    AimBuf->iLastMZKZeroVal = AimPara->iMZKZeroVal;
    AimBuf->iLastMZKZeroSite = AimPara->iMZKZeroSite;
    
    if(Channel == CH1){
        InitAimRebarParaCH1();
    }
    else if(Channel == CH2){
        InitAimRebarParaCH2();
    }
    else if(Channel == CH3){
        InitAimRebarParaCH3();
    }
}

//================================================================//
//���ܣ��ۺϷ��������㷨�Ĵ�����
//������NULL
//���أ�NULL
//================================================================//
void AimRebarAnanlze_MZK(void)
{
    AnalyzeCoverData_MZK(CH1, &AimRebarPara_CH1, &ResBuf_CH1, &AimRebarBuf_CH1);
    AnalyzeCoverData_MZK(CH2, &AimRebarPara_CH2, &ResBuf_CH2, &AimRebarBuf_CH2);
    AnalyzeCoverData_MZK(CH3, &AimRebarPara_CH3, &ResBuf_CH3, &AimRebarBuf_CH3);
}

//================================================================//
//���ܣ���ȡһ��ͨ�����ֽ�����
//������NULL
//���أ�NULL
//================================================================//
ONECHREBAR_BUFFER GetOneChRebarResult(COVER_BUFFER *ResBuf)
{
    ONECHREBAR_BUFFER Res;
    memset(&Res, 0, sizeof(Res));
    
    int i=0,j=0;
    
    if(ResBuf->AddRebatCnt == 0){
        return Res;
    }
    Res.AllRebarCnt = ResBuf->AllRebarCnt;
    Res.AddRebatCnt = ResBuf->AddRebatCnt;
    
    for(i=ResBuf->AllRebarCnt-ResBuf->AddRebatCnt;i<=ResBuf->AllRebarCnt-1;i++){
        Res.RebarCover[j] = ResBuf->RebarCover[i];
        Res.RebarSite[j] = ResBuf->RebarSite[i];
        j++;
    }
    
    ResBuf->AddRebatCnt = 0;
    return Res;
}

//================================================================//
//���ܣ���ȡ���ֽ�����
//������NULL
//���أ�NULL
//================================================================//
DISPREBAR_BUFFER GetRebarResult(void)
{
    int i=0;
    DISPREBAR_BUFFER Res;
    ONECHREBAR_BUFFER ResCh1;
    ONECHREBAR_BUFFER ResCh2;
    ONECHREBAR_BUFFER ResCh3;
    
    memset(&Res, 0, sizeof(Res));
    memset(&ResCh1, 0, sizeof(ResCh1));
    memset(&ResCh2, 0, sizeof(ResCh2));
    memset(&ResCh3, 0, sizeof(ResCh3));

    //��ȡ�����ֽ���
    ResCh1 = GetOneChRebarResult(&ResBuf_CH1);
    ResCh2 = GetOneChRebarResult(&ResBuf_CH2);
    ResCh3 = GetOneChRebarResult(&ResBuf_CH3);
    Res.IsDisp = 0;
    
    if(ResCh1.AddRebatCnt != 0){
        Res.CH1.AddRebatCnt = ResCh1.AddRebatCnt;
        Res.CH1.AllRebarCnt = ResCh1.AllRebarCnt;
        for(i=0;i<Res.CH1.AddRebatCnt;i++){
            Res.CH1.RebarCover[i] =  ResCh1.RebarCover[i];
            Res.CH1.RebarSite[i] = ResCh1.RebarSite[i];
        }
        Res.IsDisp = 1;
    }
    if(ResCh2.AddRebatCnt != 0){
        Res.CH2.AddRebatCnt = ResCh2.AddRebatCnt;
        Res.CH2.AllRebarCnt = ResCh2.AllRebarCnt;
        for(i=0;i<Res.CH2.AddRebatCnt;i++){
            Res.CH2.RebarCover[i] =  ResCh2.RebarCover[i];
            Res.CH2.RebarSite[i] = ResCh2.RebarSite[i];
        }
        Res.IsDisp = 1;
    }
    if(ResCh3.AddRebatCnt != 0){
        Res.CH3.AddRebatCnt = ResCh3.AddRebatCnt;
        Res.CH3.AllRebarCnt = ResCh3.AllRebarCnt;
        for(i=0;i<Res.CH3.AddRebatCnt;i++){
            Res.CH3.RebarCover[i] =  ResCh3.RebarCover[i];
            Res.CH3.RebarSite[i] = ResCh3.RebarSite[i];
        }
        Res.IsDisp = 1;
    }
    return Res;
}


//================================================================//
//���ܣ����º�㼰�̶�
//������NULL
//���أ�NULL
//================================================================//
void ReflashRedDot(unsigned int iCarSite)
{
    BrushCar(iCarSite);
}

//================================================================//
//���ܣ������㷨
//������NULL
//���أ�NULL
//================================================================//
ESTIMATEDIAM EstimateDiam(void)
{
    double fSAvg,fBAvg,fbuf0,fbuf1,fbuf2;
    int fMin = 1000;
    int i=0;
    int ValueBuf[20] = {0};
    int SumBuf[20] = {0};
    int SubBuf[20] = {0};
    float DotValue = 0;
    float fDiamVal = 0;
    float ihdBuf;
    unsigned char ucIndex;
    unsigned short int uiBuf;
    char dataBuf[200];
    ESTIMATEDIAM Res;
    
    memset(&Res, 0, sizeof(Res));

    //ˢ�º��
    ReflashRedDot(g_iCarDist);
    
    if(g_iCarDist > g_iCarLastDist){
        //�˲�����,�����ֵ��΢���㷨��������
        SaveAdcDataBuf();
        
        if((((g_iCarDist-1)/6 > (g_iCarLastDist-1)/6))&&(g_iCarDist>5)){
            //�����ݽ��н�Ƶ�˲�
            AdcFilter();

            //��ֵ��λ�ֽ�
            AimRebarSum();
            
            //��ֵ��λ�ֽ�
            AimRebarSub();
        }
    }
    
    //��ֵ�㷨��λ���ֽ��ʼ�����㷨
    if(AimRebarPara_CH2.ucSubAimStep == 2){
        for(i=1;i<14;i++){
            
            fSAvg = AimRebarBuf_CH2.EstSumCh2Diam16;
            fbuf1 = fSAvg * (float)SUM_BIG_FI_LW_CH2[i];
            fbuf1 /= 1000;
            fbuf1 += SUM_BIG_ZERO_LW_CH2[i];//10����ϵ
            fbuf1 = fbuf1 * (float)SUM_BIG_FI_LW_CH2[0];
            fbuf1 /= 1000;
            fbuf1 += SUM_BIG_ZERO_LW_CH2[0];
            
            fBAvg = AimRebarBuf_CH2.EstSubCh2Diam16;
            fbuf2 = fBAvg * (float)SUB_BIG_FI_LW_CH2[i];
            fbuf2 /= 1000;
            fbuf2 += SUB_BIG_ZERO_LW_CH2[i];//10����ϵ
            fbuf2 = fbuf2 * (float)SUB_BIG_FI_LW_CH2[0];
            fbuf2 /= 1000;
            fbuf2 += SUB_BIG_ZERO_LW_CH2[0];
            
            if(fbuf1 > fbuf2){
                fbuf0 = (int)(fbuf1 - fbuf2);
            }
            else{
                fbuf0 = (int)(fbuf2 - fbuf1);
            }
            
            ValueBuf[i-1] = (int)(fbuf2 - fbuf1);
            SumBuf[i-1] = (int)fbuf1;
            SubBuf[i-1] = (int)fbuf2;

            if(fbuf0 < fMin){
                //ihdBuf = (fbuf1 + fbuf2 + 10)/20;
                ihdBuf = (int)(fbuf1 + 5)/10;//ȡС��������
                fMin = fbuf0;
                ucIndex = i-1;
            }
        }
        
        //����ֱ��
        for(i=0;i<=12;i++){
            if((ValueBuf[i] >= 0)&&(ValueBuf[i+1] < 0)){
                ucIndex = i;
            }
        }
        
        if(ucIndex>13){
            ucIndex = 13;
        }
        
        uiBuf = ListZhiJing[ucIndex];
        if(ValueBuf[ucIndex] == 0){
            DotValue = 0;
        }
        else{
            DotValue = (ListZhiJing[ucIndex+1]-ListZhiJing[ucIndex])*abs(ValueBuf[ucIndex]*10/(ValueBuf[ucIndex]-ValueBuf[ucIndex+1]));
        }
        fDiamVal = (uiBuf*10+DotValue)/10;
        
        LCD_SetColor(WHITE,PURPLE);
        sprintf(dataBuf,"Diam:%.1f    Cover:%.1f   %d   %.1f   %.1f",fDiamVal, ihdBuf, fMin, fSAvg, fBAvg);
        LCD_ShowString(ASC16x8,0, 240, 0,dataBuf);
        
        LCD_SetColor(WHITE,PURPLE);
        sprintf(dataBuf,"%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d",ValueBuf[0],ValueBuf[1],ValueBuf[2],ValueBuf[3],ValueBuf[4],ValueBuf[5],ValueBuf[6],ValueBuf[7],ValueBuf[8],ValueBuf[9],ValueBuf[10],ValueBuf[11],ValueBuf[12]);
        LCD_ShowString(ASC16x8,0, 50, 0,dataBuf);
        
        LCD_SetColor(WHITE,PURPLE);
        sprintf(dataBuf,"%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d",SumBuf[0],SumBuf[1],SumBuf[2],SumBuf[3],SumBuf[4],SumBuf[5],SumBuf[6],SumBuf[7],SumBuf[8],SumBuf[9],SumBuf[10],SumBuf[11],SumBuf[12]);
        LCD_ShowString(ASC16x8,0, 70, 0,dataBuf);
        
        LCD_SetColor(WHITE,PURPLE);
        sprintf(dataBuf,"%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d",SubBuf[0],SubBuf[1],SubBuf[2],SubBuf[3],SubBuf[4],SubBuf[5],SubBuf[6],SubBuf[7],SubBuf[8],SubBuf[9],SubBuf[10],SubBuf[11],SubBuf[12]);
        LCD_ShowString(ASC16x8,0, 90, 0,dataBuf);
        
        Res.Cover = ihdBuf;
        Res.Diam = fDiamVal;
    }
    
    return Res;
}

//================================================================//
//���ܣ��ֽ������ʾ����
//������NULL
//���أ�NULLx
//================================================================//
void DispRebarCnt(char Channel) 
{
    static unsigned short int LastCntBuf = 0;
    unsigned short int CntBuf = 0;

    switch(Channel){
        case CH1:
            CntBuf = ResBuf_CH1.AllRebarCnt;
            break;
        
        case CH2:
            CntBuf = ResBuf_CH2.AllRebarCnt;
            break;
        
        case CH3:
            CntBuf = ResBuf_CH3.AllRebarCnt;
            break;
        
        default:
            break;
    }
    
    if(CntBuf == LastCntBuf){
        return;
    }
    
    DispSaveDataCnt(CntBuf);
    LastCntBuf = CntBuf;
}

//================================================================//
//���ܣ��ֽ���ʾ����
//������NULL
//���أ�NULL
//================================================================//
void DispRebarTiaoXing(unsigned int iCarDist, enum CHANNELNUM Channel, DISPREBAR_BUFFER DispBuf)
{
    int i=0;
    
    if(DispBuf.IsDisp == 0){
        return;
    }
    
    if(DispBuf.CH1.AddRebatCnt != 0){
        for(i=0;i<DispBuf.CH1.AddRebatCnt;i++){
            if((iCarDist/400+1) == (DispBuf.CH1.RebarSite[i]/400+1)){
                BrushTiaoXing(0, DispBuf.CH1.RebarSite[i], DispBuf. CH1.RebarCover[i]);
            }
        }
    }
    
    if(DispBuf.CH2.AddRebatCnt != 0){
        for(i=0;i<DispBuf.CH2.AddRebatCnt;i++){
            if((iCarDist/400+1) == (DispBuf.CH2.RebarSite[i]/400+1)){
                BrushTiaoXing(1, DispBuf.CH2.RebarSite[i], DispBuf.CH2.RebarCover[i]);
            }
        }
    }
    
    if(DispBuf.CH3.AddRebatCnt != 0){
        for(i=0;i<DispBuf.CH3.AddRebatCnt;i++){
            if((iCarDist/400+1) == (DispBuf.CH3.RebarSite[i]/400+1)){
                BrushTiaoXing(2, DispBuf.CH3.RebarSite[i], DispBuf.CH3.RebarCover[i]);
            }
        }
    }
}

//================================================================//
//���ܣ��ֽ�ʾ��ͼ��ʾ����
//������NULL
//���أ�NULLaa
//================================================================//
void DispRebarBoXing(unsigned int iCarDist, char Channel, DISPREBAR_BUFFER DispBuf)
{
    int i=0;
    
    if(DispBuf.IsDisp == 0){
        return;
    }
    
    if(Channel == 0){
        for(i=0;i<DispBuf.CH1.AddRebatCnt;i++){
            if(i == 0){
                BrushBoXing(iCarDist/400+1, DispBuf.CH1.RebarSite[i], ResBuf_CH1.RebarSite[ResBuf_CH1.AllRebarCnt-DispBuf.CH1.AddRebatCnt-1], DispBuf.CH1.RebarCover[i]);
            }
            else{
                BrushBoXing(iCarDist/400+1, DispBuf.CH1.RebarSite[i], DispBuf.CH1.RebarSite[i-1], DispBuf.CH1.RebarCover[i]);
            }
        }
    }
    
    if(Channel == 1){
        for(i=0;i<DispBuf.CH2.AddRebatCnt;i++){
            if(i == 0){
                BrushBoXing(iCarDist/400+1, DispBuf.CH2.RebarSite[i], ResBuf_CH2.RebarSite[ResBuf_CH2.AllRebarCnt-DispBuf.CH2.AddRebatCnt-1], DispBuf.CH2.RebarCover[i]);
            }
            else{
                BrushBoXing(iCarDist/400+1, DispBuf.CH2.RebarSite[i], DispBuf.CH2.RebarSite[i-1], DispBuf.CH2.RebarCover[i]);
            }
        }
    }
    
    if(Channel == 2){
        for(i=0;i<DispBuf.CH3.AddRebatCnt;i++){
            if(i == 0){
                BrushBoXing(iCarDist/400+1, DispBuf.CH3.RebarSite[i], ResBuf_CH3.RebarSite[ResBuf_CH3.AllRebarCnt-DispBuf.CH3.AddRebatCnt-1], DispBuf.CH3.RebarCover[i]);
            }
            else{
                BrushBoXing(iCarDist/400+1, DispBuf.CH3.RebarSite[i], DispBuf.CH3.RebarSite[i-1], DispBuf.CH3.RebarCover[i]);
            }
        }
    }
}

//================================================================//
//���ܣ��ֽ�ʾ��ͼ��ʾ����
//������NULL
//���أ�NULLaa
//================================================================//
void DispRebarPouMian(unsigned int iCarDist, char Channel, DISPREBAR_BUFFER DispBuf)
{
    int i=0;
    
    if(DispBuf.IsDisp == 0){
        return;
    }
    
    if(Channel == 0){
        for(i=0;i<DispBuf.CH1.AddRebatCnt;i++){
            if(i == 0){
                BrushPouMian(iCarDist/400+1, DispBuf.CH1.RebarSite[i], ResBuf_CH1.RebarSite[ResBuf_CH1.AllRebarCnt-DispBuf.CH1.AddRebatCnt-1], DispBuf.CH1.RebarCover[i]);
            }
            else{
                BrushPouMian(iCarDist/400+1, DispBuf.CH1.RebarSite[i], DispBuf.CH1.RebarSite[i-1], DispBuf.CH1.RebarCover[i]);
            }
        }
    }
    
    if(Channel == 1){
        for(i=0;i<DispBuf.CH2.AddRebatCnt;i++){
            if(i == 0){
                BrushPouMian(iCarDist/400+1, DispBuf.CH2.RebarSite[i], ResBuf_CH2.RebarSite[ResBuf_CH2.AllRebarCnt-DispBuf.CH2.AddRebatCnt-1], DispBuf.CH2.RebarCover[i]);
            }
            else{
                BrushPouMian(iCarDist/400+1, DispBuf.CH2.RebarSite[i], DispBuf.CH2.RebarSite[i-1], DispBuf.CH2.RebarCover[i]);
            }
        }
    }
    
    if(Channel == 2){
        for(i=0;i<DispBuf.CH3.AddRebatCnt;i++){
            if(i == 0){
                BrushPouMian(iCarDist/400+1, DispBuf.CH3.RebarSite[i], ResBuf_CH3.RebarSite[ResBuf_CH3.AllRebarCnt-DispBuf.CH3.AddRebatCnt-1], DispBuf.CH3.RebarCover[i]);
            }
            else{
                BrushPouMian(iCarDist/400+1, DispBuf.CH3.RebarSite[i], DispBuf.CH3.RebarSite[i-1], DispBuf.CH3.RebarCover[i]);
            }
        }
    }
}
//================================================================//
//���ܣ���ȼ���ⲿ�����㷨�����ӿ�
//������NULL
//���أ�NULL
//================================================================//
void AimFuncRun_HD(char ucChannel, unsigned char DispMode)
{
    DISPREBAR_BUFFER DispRes;
    memset(&DispRes, 0, sizeof(DispRes));
    
    //ˢ�º��
    ReflashRedDot(g_iCarDist);
    
    if(g_iCarDist > g_iCarLastDist){
        //�˲�����,�����ֵ��΢���㷨��������
        SaveAdcDataBuf();
        
        if((((g_iCarDist-1)/6 > (g_iCarLastDist-1)/6))&&(g_iCarDist>5)){
            //�����ݽ��н�Ƶ�˲�
            AdcFilter();

            //��ֵ��λ�ֽ�
            AimRebarSum();
            
            //��ֵ��λ�ֽ�
            AimRebarSub();
            
            //΢�ֶ�λ�ֽ�
            AimRebarDif();
        }
        
        //�ۺϷ���������
        AimRebarAnanlze_Sub();
        
        //��ȡ���¶�λ���ĸֽ�
        DispRes = GetRebarResult();
        
        //���ƶ�λ���ĸֽ�
        DispRebarTiaoXing(g_iCarDist, CH2, DispRes);
        
        //���ƶ�λ���ĸֽ�
        if(DispMode == TIAO_BO){
            DispRebarBoXing(g_iCarDist, ucChannel, DispRes);
        }
        else if(DispMode == TIAO_POU){
            DispRebarPouMian(g_iCarDist, ucChannel, DispRes);
        }
        
        //��ʾ�ֽ����
        DispRebarCnt(g_DetectPara.Channel);
    }
}

//================================================================//
//���ܣ���׼����λ�ֽ��
//������NULL`   
//���أ�NULL
//================================================================//
unsigned char AimFuncRun_MZK(char ucChannel, unsigned char ucAimState, unsigned char DispMode)
{
    DISPREBAR_BUFFER DispRes;
    memset(&DispRes, 0, sizeof(DispRes));
    
    if(g_iCarDist > g_iCarMaxDist){
//    if(g_iCarDist > g_iCarLastDist){
        g_iCarMaxDist = g_iCarDist;

        //�˲�����,�����ֵ��΢���㷨��������
        SaveAdcDataBuf();

        //�����ݽ��н�Ƶ�˲�
        AdcFilter();

        //��׼��λ�ֽ�
        AimRebarMZK();

//            //��ֵ��λ�ֽ�
//            AimRebarSum();
//
//            //��ֵ��λ�ֽ�
//            AimRebarSub();
//
//            //΢�ֶ�λ�ֽ�
//            AimRebarDif();

        //�ۺϷ���������
        AimRebarAnanlze_MZK();
        
        //��ȡ���¶�λ���ĸֽ�
        DispRes = GetRebarResult();
        
        //���ƶ�λ���ĸֽ�
        if(DispMode == MIAO_BO){
            DispRebarBoXing(g_iCarDist, ucChannel, DispRes);
        }
        else if(DispMode == MIAO_POU){
            DispRebarPouMian(g_iCarDist, ucChannel, DispRes);
        }

        //��ʾ�ֽ����
        DispRebarCnt(g_DetectPara.Channel);
        
        if(DispRes.IsDisp != 0){
            if(ucChannel == CH1){
                return DispRes.CH1.RebarCover[0];
            }
            if(ucChannel == CH2){
                return DispRes.CH2.RebarCover[0];
            }
            if(ucChannel == CH3){
                return DispRes.CH3.RebarCover[0];
            }
        }
    }
    return 0;
}


//================================================================//
//���ܣ���׼����λ�ֽ��
//������NULL
//���أ�NULL
//================================================================//
unsigned char AimFuncRun_JGJ(unsigned char ucChannel, unsigned char ucAimState, unsigned char DispMode)
{
    DISPREBAR_BUFFER DispRes;
    memset(&DispRes, 0, sizeof(DispRes));
    
//    if(g_iCarDist > g_iCarMaxDist){
    if(g_iCarDist > g_iCarLastDist){
        g_iCarMaxDist = g_iCarDist;

        //�˲�����,�����ֵ��΢���㷨��������
        SaveAdcDataBuf();

        //�����ݽ��н�Ƶ�˲�
        AdcFilter();

        //��׼��λ�ֽ�
        AimRebarMZK();

//            //��ֵ��λ�ֽ�
//            AimRebarSum();
//            
//            //��ֵ��λ�ֽ�
//            AimRebarSub();
//
//            //΢�ֶ�λ�ֽ�
//            AimRebarDif();

        //�ۺϷ���������
        AimRebarAnanlze_MZK();
        
        //��ȡ���¶�λ���ĸֽ�
        DispRes = GetRebarResult();
        
        if(DispRes.IsDisp != 0){
            if(ucChannel == CH1){
                return DispRes.CH1.RebarCover[0];
            }
            if(ucChannel == CH2){
                return DispRes.CH2.RebarCover[0];
            }
            if(ucChannel == CH3){
                return DispRes.CH3.RebarCover[0];
            }
        }
    }
    return 0;
}


//================================================================//
//���ܣ���������ú���
//������NULL
//���أ�NULL
//================================================================//
DISPREBAR_BUFFER AimFuncRun_Block(void)
{
    DISPREBAR_BUFFER DispRes;
    memset(&DispRes, 0, sizeof(DispRes));
    
    //ˢ�º��
    ReflashRedDot(g_iCarDist);
    
    if(g_iCarDist > g_iCarMaxDist){
//    if(g_iCarDist > g_iCarLastDist){
        g_iCarMaxDist = g_iCarDist;
        
        //�˲�����,�����ֵ��΢���㷨��������
        SaveAdcDataBuf();
        
        if((((g_iCarDist-1)/6 > (g_iCarLastDist-1)/6))&&(g_iCarDist>5)){
            //�����ݽ��н�Ƶ�˲�
            AdcFilter();

            //��ֵ��λ�ֽ�
            AimRebarSum();
            
            //��ֵ��λ�ֽ�
            AimRebarSub();
            
            //΢�ֶ�λ�ֽ�
            AimRebarDif();
        }
        
        //�ۺϷ���������
        AimRebarAnanlze_Sub();
        
        //��ȡ���¶�λ���ĸֽ�
        DispRes = GetRebarResult();
        
////        //���ƶ�λ���ĸֽ�
////        DispRebarTiaoXing(g_iCarDist, CH2, DispRes);
////        
////        DispRebarBoXing(g_iCarDist, CH2, DispRes);
//////        
//////        //��ʾ�ֽ����
//////        DispRebarCnt(g_DetectPara.Channel);
    }
    return DispRes;
}





//================================================================//
//���ܣ��洢ԭʼ���ݣ��ļ���������������������PC�˻��Ʋ���
//������NULL
//���أ�NULL
//================================================================//
int SaveSourceDataFile(void)
{
    int i;
    u8 result;
//////    u8 uPerCnt;
//////    char filename[50];
    char filesNum = 0;
    int CH1=0,CH2=0,CH3=0,CH4=0,CH5=0,CH6=0;
    int SUM1=0,SUM2=0,SUM3=0,SUM4=0,SUM5=0,SUM6=0;
    int SUB1=0,SUB2=0,SUB3=0,SUB4=0,SUB5=0,SUB6=0;
    
//    int Ch1[100] = {0};
//    int Ch2[100] = {0};
//    int Sub[100] = {0};
//    int Sum[100] = {0};
    
    //��ʾԭʼ���ݴ洢����
    sprintf(g_cStrBuf,"0:/%02d%02d",g_SysTime.month, g_SysTime.day);
    f_mkdir(g_cStrBuf);//�����ļ���
    sprintf(g_cStrBuf,"0:/%02d%02d/GY91Disp.csv",g_SysTime.month, g_SysTime.day);
    result = f_open(file, g_cStrBuf, FA_OPEN_EXISTING);//�ļ����ڣ��޸��ļ���
    if (result == FR_OK){//���ļ����ڣ�д��������Ϣ
        f_close(file);
		//����ѭ�������ɴ�����ŵ��ļ�
        do{
            filesNum ++;
            sprintf(g_cStrBuf,"0:/%02d%02d/GY91Disp_%02d.csv",g_SysTime.month, g_SysTime.day, filesNum);
            result = f_open(file, g_cStrBuf, FA_OPEN_EXISTING);//���Ѿ����ڵ��ĵ����ж��ļ��Ƿ����
            if(result == FR_OK){
                f_close(file);
            }
            else{
                break;
            }
        }
        while(1);
    }
        
    result = f_open(file, g_cStrBuf, FA_WRITE | FA_CREATE_ALWAYS);//���ڵĸ���֮ǰ�ļ�
    if (result == FR_OK){//д��������Ϣ
        
        sprintf(g_cStrBuf,"CH1,CH2,SUM,SUB\r\n");//\r return������
        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf),&bw);
        
        for(i=1;i<g_iCarDist;i++){
            
            if(i==1){
                CH3 = SourceData.iCH3Start + SourceData.iCH3[i];
                CH4 = SourceData.iCH4Start + SourceData.iCH4[i];
            }
            else{
                CH3 +=SourceData.iCH3[i];//+=��ʾ�Ҳ��ֵ�ӵ����ı����ϣ�Ȼ�󽫽������������
                CH4 +=SourceData.iCH4[i];
            }
            sprintf(g_cStrBuf,"%5d,%5d,%5d,%5d\r\n", CH3, CH4, CH3+CH4, CH4-CH3);
            
//            Ch1[i]=CH3;
//            Ch2[i]=CH4;
//            Sub[i]=CH4-CH3;
//            Sum[i]=CH4+CH3;
            
            result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
            if(result != FR_OK){
                break;
            }
        }
        
        f_close(file);
    }
    return result;
}



//////int SaveSourceDataFile(void)
//////{
//////    int i;
//////    u8 result;
////////////    u8 uPerCnt;
////////////    char filename[50];
//////    char filesNum = 0;
//////    int CH1=0,CH2=0,CH3=0,CH4=0,CH5=0,CH6=0;
//////    int SUM1=0,SUM2=0,SUM3=0,SUM4=0,SUM5=0,SUM6=0;
//////    int SUB1=0,SUB2=0,SUB3=0,SUB4=0,SUB5=0,SUB6=0;
//////    
//////    int Ch1[100] = {0};
//////    int Ch2[100] = {0};
//////    int Sub[100] = {0};
//////    int Sum[100] = {0};
//////    
////////////    u8 uPer[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

////////    for(i=1;i<100;i++){
////////        sprintf(g_filname,"0:/HD/test%02d.txt",i);
////////        result = f_open(file,g_filname, FA_READ); //�ж��ļ��Ƿ����
////////        if (result == FR_OK){//д��������Ϣ
////////            f_close(file);
////////        }
////////        else{
////////            break;
////////        }
////////    }
////////    sprintf(filename,"0:/HD/TestA%02d%02d%02d%02d-%02d.txt",Rx8010sjTime.year,Rx8010sjTime.month,
////////        Rx8010sjTime.day,Rx8010sjTime.hour,g_iSaveFileNum);
////////    sprintf(filename,"0:/HD/%d.txt",g_iSaveFileNum);
//////        
//////    //��ʾԭʼ���ݴ洢����
//////    sprintf(g_cStrBuf,"0:/%02d%02d",g_SysTime.month, g_SysTime.day);
//////    f_mkdir(g_cStrBuf);
//////    sprintf(g_cStrBuf,"0:/%02d%02d/GY91Disp.csv",g_SysTime.month, g_SysTime.day);
//////    result = f_open(file, g_cStrBuf, FA_OPEN_EXISTING);//�ļ����ڣ��޸��ļ���
//////    if (result == FR_OK){//д��������Ϣ
//////        f_close(file);
//////        do{
//////            filesNum ++;
//////            sprintf(g_cStrBuf,"0:/%02d%02d/GY91Disp_%02d.csv",g_SysTime.month, g_SysTime.day, filesNum);
//////            result = f_open(file, g_cStrBuf, FA_OPEN_EXISTING);//���Ѿ����ڵ��ĵ����ж��ļ��Ƿ����
//////            if(result == FR_OK){
//////                f_close(file);
//////            }
//////            else{
//////                break;
//////            }
//////        }
//////        while(1);
//////    }
//////    
//////    result = f_open(file, g_cStrBuf, FA_WRITE | FA_CREATE_ALWAYS);//���ڵĸ���֮ǰ�ļ�
//////    if (result == FR_OK){//д��������Ϣ
//////        
//////        sprintf(g_cStrBuf,"CH1,CH2,SUM,SUB\r\n");
//////        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf),&bw);
//////        
//////        for(i=1;i<g_iCarDist;i++){
//////            
//////            if(i==1){
//////                CH3 = SourceData.iCH3Start + SourceData.iCH3[i];
//////                CH4 = SourceData.iCH4Start + SourceData.iCH4[i];
//////            }
//////            else{
//////                CH3 +=SourceData.iCH3[i];
//////                CH4 +=SourceData.iCH4[i];
//////            }
//////            sprintf(g_cStrBuf,"%5d,%5d,%5d,%5d\r\n", CH3, CH4, CH3+CH4, CH4-CH3);
//////            
//////            Ch1[i]=CH3;
//////            Ch2[i]=CH4;
//////            Sub[i]=CH4-CH3;
//////            Sum[i]=CH4+CH3;
//////            
//////            result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
//////            if(result != FR_OK){
//////                break;
//////            }
//////        }
//////        
////////////		if(g_iCarDist>20){
////////////			uPerCnt=g_iCarDist/20;
////////////		}
////////////		else{
////////////			uPerCnt = 0;
////////////		}
//////        
////////////        sprintf(g_cStrBuf,"CH1,CH2,CH3,CH4,CH5,CH6,SUB1,SUB2,SUB3,SUBMARK1,SUBMARK2,SUBMARK3,DIF1,DIF2,DIF3\r\n");
////////////        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf),&bw);
////////////        
////////////        for(i=1;i<g_iCarDist;i++){
////////////            sprintf(g_cStrBuf,"%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d\r\n",
//////////////            SourceData.iCh1Sum[i],SourceData.iCh1Sub[i],SourceData.iCh2Sum[i],SourceData.iCh2Sub[i],SourceData.iCh3Sum[i],SourceData.iCh3Sub[i]);
////////////                SourceData.iCH1[i],SourceData.iCH2[i],SourceData.iCH3[i],SourceData.iCH4[i],SourceData.iCH5[i],SourceData.iCH6[i]
////////////                ,SourceData.iCh1Sub[i],SourceData.iCh2Sub[i],SourceData.iCh3Sub[i]
////////////                ,SourceData.iCh1SubMark[i],SourceData.iCh2SubMark[i],SourceData.iCh3SubMark[i]
////////////                ,SourceData.iCh1DifMark[i],SourceData.iCh2DifMark[i],SourceData.iCh3DifMark[i]);
////////////            result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
////////////            if(result != FR_OK){
////////////                break;
////////////            }
////////////        }
//////        
////////0311ע�͵�        
////////////        sprintf(g_cStrBuf,"CH1,CH2,CH3,CH4,CH5,CH6,SUMMARK1,SUMMARK2,SUMMARK3,SUMMARK12,SUMMARK22,SUMMARK32,SUBMARK1,SUBMARK2,SUBMARK3,DIFMARK1,DIFMARK2,DIFMARK3\r\n");
////////////        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf),&bw);
////////////        
////////////        for(i=1;i<g_iCarDist;i++){
////////////            sprintf(g_cStrBuf,"%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d\r\n",
////////////                SourceData.iCH1[i],SourceData.iCH2[i],SourceData.iCH3[i]
////////////                ,SourceData.iCH4[i],SourceData.iCH5[i],SourceData.iCH6[i]
////////////                ,SourceData.iCh1SumMark[i],SourceData.iCh2SumMark[i],SourceData.iCh3SumMark[i]
////////////                ,SourceData.iCh1SumMark2[i],SourceData.iCh2SumMark2[i],SourceData.iCh3SumMark2[i]
////////////                ,SourceData.iCh1SubMark[i],SourceData.iCh2SubMark[i],SourceData.iCh3SubMark[i]
////////////                ,SourceData.iCh1DifMark[i],SourceData.iCh2DifMark[i],SourceData.iCh3DifMark[i]
////////////            );
////////////            result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
////////////            if(result != FR_OK){
////////////                break;
////////////            }
////////////        }
////////END 

////////20221201ע�͵�
////////////        sprintf(g_cStrBuf,"Sub1,Sub2,Sub3,Sum1,Sum2,Sum3,SUMMARK1,SUMMARK2,SUMMARK3,SUMMARK12,SUMMARK22,SUMMARK32,SUBMARK1,SUBMARK2,SUBMARK3,DIFMARK1,DIFMARK2,DIFMARK3,DIFMARKCNT1,DIFMARKCNT2,DIFMARKCNT3\r\n");
////////////        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf),&bw);
////////////        
////////////        for(i=1;i<g_iCarDist;i++){
////////////            sprintf(g_cStrBuf,"%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d\r\n",
////////////                SourceData.iCh1Sub[i],SourceData.iCh2Sub[i],SourceData.iCh3Sub[i]
////////////                ,SourceData.iCh1Sum[i],SourceData.iCh2Sum[i],SourceData.iCh3Sum[i]
////////////                ,SourceData.iCh1SumMark[i],SourceData.iCh2SumMark[i],SourceData.iCh3SumMark[i]
////////////                ,SourceData.iCh1SumMark2[i],SourceData.iCh2SumMark2[i],SourceData.iCh3SumMark2[i]
////////////                ,SourceData.iCh1SubMark[i],SourceData.iCh2SubMark[i],SourceData.iCh3SubMark[i]
////////////                ,SourceData.iCh1DifMark[i],SourceData.iCh2DifMark[i],SourceData.iCh3DifMark[i]
////////////                ,SourceData.iCh1DifWaveCnt[i],SourceData.iCh2DifWaveCnt[i],SourceData.iCh3DifWaveCnt[i]
////////////            );
////////////            result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
////////////            if(result != FR_OK){
////////////                break;
////////////            }
////////////        }
////////END 
//////        
//////        f_close(file);
//////    }
//////    return result;
//////}




//�洢���ݱ�
//////int SaveTableDataFile(void)
//////{
////////    char btDataBuf[16];
//////    char g_cStrBuf[64];
//////    char filename[50];
//////    char dataBuf[10];
//////    u8 result;
//////    
//////    //�ɼ����ݱ�
//////    sprintf(filename,"0:/HD/Sample1.txt");
//////    result = f_open(file, filename, FA_WRITE | FA_OPEN_ALWAYS);//���Բɼ������ļ�
//////    if (result == FR_OK){//д��������Ϣ
//////        if(SampleDataCnt%10 == 9){
//////            sprintf(g_cStrBuf, "%d,//%d-%d\r\n", AimRebarPara_CH1.iSubMaxVal-AimRebarPara_CH1.iSubMinVal, SampleDataCnt/10*10, SampleDataCnt/10*10+9);
//////        }
//////        else{
//////            sprintf(g_cStrBuf,"%d,",AimRebarPara_CH1.iSubMaxVal-AimRebarPara_CH1.iSubMinVal);
//////        }
//////        f_lseek(file,file->fptr+file->fsize);
//////        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
//////        f_close(file);
//////    }
//////    Sleep_ms(200);
//////    
//////    sprintf(filename,"0:/HD/Sample2.txt");
//////    result = f_open(file, filename, FA_WRITE | FA_OPEN_ALWAYS);//���Բɼ������ļ�
//////    if (result == FR_OK){//д��������Ϣ
//////        if(SampleDataCnt%10 == 9){
//////            sprintf(g_cStrBuf, "%d,//%d-%d\r\n", AimRebarPara_CH2.iSubMaxVal-AimRebarPara_CH2.iSubMinVal, SampleDataCnt/10*10, SampleDataCnt/10*10+9);
//////        }
//////        else{
//////            sprintf(g_cStrBuf,"%d,",AimRebarPara_CH2.iSubMaxVal-AimRebarPara_CH2.iSubMinVal);
//////        }
//////        
//////        f_lseek(file,file->fptr+file->fsize);
//////        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
//////        f_close(file);
//////    }
//////    Sleep_ms(200);
//////    
//////    sprintf(filename,"0:/HD/Sample3.txt");
//////    result = f_open(file, filename, FA_WRITE | FA_OPEN_ALWAYS);//���Բɼ������ļ�
//////    if (result == FR_OK){//д��������Ϣ
//////        if(SampleDataCnt%10 == 9){
//////            sprintf(g_cStrBuf, "%d,//%d-%d\r\n", AimRebarPara_CH3.iSubMaxVal-AimRebarPara_CH3.iSubMinVal, SampleDataCnt/10*10, SampleDataCnt/10*10+9);
//////        }
//////        else{
//////            sprintf(g_cStrBuf, "%d,", AimRebarPara_CH3.iSubMaxVal-AimRebarPara_CH3.iSubMinVal);
//////        }
//////        f_lseek(file,file->fptr+file->fsize);
//////        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
//////        f_close(file);
//////    }
//////    
//////    sprintf(filename,"0:/HD/Sample4.txt");
//////    result = f_open(file, filename, FA_WRITE | FA_OPEN_ALWAYS);//���Բɼ������ļ�
//////    if (result == FR_OK){//д��������Ϣ
//////        if(SampleDataCnt%10 == 9){
//////            sprintf(g_cStrBuf, "%d,//%d-%d\r\n", AimRebarBuf_CH1.SumBoFengVal[4], SampleDataCnt/10*10, SampleDataCnt/10*10+9);
//////        }
//////        else{
//////            sprintf(g_cStrBuf,"%d,",AimRebarBuf_CH1.SumBoFengVal[4]);
//////        }
//////        f_lseek(file,file->fptr+file->fsize);
//////        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
//////        f_close(file);
//////    }
//////    Sleep_ms(200);
//////    
//////    sprintf(filename,"0:/HD/Sample5.txt");
//////    result = f_open(file, filename, FA_WRITE | FA_OPEN_ALWAYS);//���Բɼ������ļ�
//////    if (result == FR_OK){//д��������Ϣ
//////        if(SampleDataCnt%10 == 9){
//////            sprintf(g_cStrBuf, "%d,//%d-%d\r\n", AimRebarBuf_CH2.SumBoFengVal[4], SampleDataCnt/10*10, SampleDataCnt/10*10+9);
//////        }
//////        else{
//////            sprintf(g_cStrBuf,"%d,",AimRebarBuf_CH2.SumBoFengVal[4]);
//////        }
//////        f_lseek(file,file->fptr+file->fsize);
//////        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
//////        f_close(file);
//////    }
//////    Sleep_ms(200);
//////    
//////    sprintf(filename,"0:/HD/Sample6.txt");
//////    result = f_open(file, filename, FA_WRITE | FA_OPEN_ALWAYS);//���Բɼ������ļ�
//////    if (result == FR_OK){//д��������Ϣ
//////        if(SampleDataCnt%10 == 9){
//////            sprintf(g_cStrBuf, "%d,//%d-%d\r\n", AimRebarBuf_CH3.SumBoFengVal[4], SampleDataCnt/10*10, SampleDataCnt/10*10+9);
//////        }
//////        else{
//////            sprintf(g_cStrBuf,"%d,",AimRebarBuf_CH3.SumBoFengVal[4]);
//////        }
//////        f_lseek(file,file->fptr+file->fsize);
//////        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
//////        f_close(file);
//////    }
//////    
//////    SampleDataCnt ++;
//////    LCD_SetColor(WHITE,BLACK);
//////    sprintf(dataBuf,"Cnt:%-5d",SampleDataCnt);
//////    LCD_ShowString(ASC16x8,400,300,0,dataBuf);
//////    
//////    return 0;
//////}


//////void SaveTableData(unsigned char ucChannel)
//////{
//////    char filename[50];
//////    u8 result;
//////    static int SampleDataCnt=0;
//////    int data;
//////    int i = 0;
//////    char dataBuf[10];
//////    
//////    if(ucChannel == 0xFF){
//////        SampleDataCnt=0;
//////        
//////        LCD_SetColor(WHITE,BLUE);
//////        sprintf(dataBuf,"Cnt:0     ");
//////        LCD_ShowString(ASC16x8,0,0,0,dataBuf);
//////    }
//////    SampleDataCnt += 1;
//////    
//////    LCD_SetColor(WHITE,BLUE);
//////    sprintf(dataBuf,"Cnt:%-5d",SampleDataCnt);
//////    LCD_ShowString(ASC16x8,0,0,0,dataBuf);
//////    
//////    LCD_SetColor(WHITE,WHITE);
//////    sprintf(dataBuf,"OK");
//////    LCD_ShowString(ASC16x8,0,20,0,dataBuf);
//////    
//////    for(i=0;i<6;i++){
//////        if(i==0){
//////            sprintf(filename,"0:/HD/SumCH1.txt");
//////            data = AimRebarBuf_CH1.LastSumRebarVal;
//////        }
//////        else if(i==1){
//////            sprintf(filename,"0:/HD/SumCH2.txt");
//////            data = AimRebarBuf_CH2.LastSumRebarVal;
//////        }
//////        else if(i==2){
//////            sprintf(filename,"0:/HD/SumCH3.txt");
//////            data = AimRebarBuf_CH3.LastSumRebarVal;
//////        }
//////        else if(i==3){
//////            sprintf(filename,"0:/HD/SubCH1.txt");
//////            data = AimRebarBuf_CH1.iLastSubMaxVal - AimRebarBuf_CH1.iLastSubMinVal;
//////        }
//////        else if(i==4){
//////            sprintf(filename,"0:/HD/SubCH2.txt");
//////            data = AimRebarBuf_CH2.iLastSubMaxVal - AimRebarBuf_CH2.iLastSubMinVal;
//////        }
//////        else if(i==5){
//////            sprintf(filename,"0:/HD/SubCH3.txt");
//////            data = AimRebarBuf_CH3.iLastSubMaxVal - AimRebarBuf_CH3.iLastSubMinVal;
//////        }
//////        
//////        result = f_open(file, filename, FA_WRITE | FA_OPEN_ALWAYS);//���Բɼ������ļ�
//////        if(result == FR_OK){//д��������Ϣ
//////            if(SampleDataCnt%10 == 0){
//////                sprintf(g_cStrBuf, "%d,//%d-%d\r\n", data, (SampleDataCnt-1)/10*10, (SampleDataCnt-1)/10*10+9);
//////            }
//////            else{
//////                sprintf(g_cStrBuf, "%d,", data);
//////            }
//////            f_lseek(file,file->fptr+file->fsize);
//////            result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
//////            f_close(file);
//////        }
//////        Sleep_ms(200);
//////    }
//////    
//////    LCD_SetColor(WHITE,BLUE);
//////    sprintf(dataBuf,"OK");
//////    LCD_ShowString(ASC16x8,0,20,0,dataBuf);
//////}


