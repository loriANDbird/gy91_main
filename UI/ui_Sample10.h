#ifndef UI_SAMPLE10_H
#define UI_SAMPLE10_H

#include "stm32f10x.h"
#include "OpTable.h"
#include "AimLib.h"
#include "OpData.h"

//���ģʽ
#define QUICK	0
#define JGJ		1
#define BLOCK	2

//����
#define	CHANGGUI	0
#define GAODIJIN	1
#define	BIGUJIN		2
#define	AOTUMIAN	3
#define	FANXIANGXZ	4

#define COVERDATACNTMAX		500
#define SOURCEDATACNTMAX	2000

//������������
struct FANXIANGXZSTR{
	int CeLiangZhi1;
	int BiaoZhunZhi1;
	int CeLiangZhi2;
	int BiaoZhunZhi2;
};

typedef struct {
	short MZJSite;	//��׼��λ��
	short CarSite;	//С��λ��
	u8 HouduRes;	//�������
	u8 Channel;		//��Ȧͨ��
	u8 ChannelLast;	//��Ȧͨ��
	u8 PageCurr;	//����ҳ��
	u8 PageView;	//����ҳ��
	u8 PageLast;	//����ҳ��
	u8 PageQnt;		//����ҳ��
	
	u8 JGJPageQnt;//JGJҳ��1-2
	u8 JGJPage;//ҳ��1-2
	u8 JGJPageC;//ҳ��1-2
	u8 JGJQnt;//JGJ�ֽ����
	u8 JGJNum;//JGJ�ֽ���1-12
	u8 JGJDot;//��ǰ���1-6
	u8 JGJDir;//ɨ�跽��0����1����
	u8 JGJSta;//����״̬��0δ��ʼ��1�ѿ�ʼ
//	u8 SampleDispMode;//������4����ʾģʽ0-3
//	u8 AnalyzeDispMode;//��������4����ʾģʽ0-3
	u8 ucGain;				//���� 0-7
	u8 ucHeat;				//���� 0-7
	u8 WorkCondition;//��������0-4
	u8 AoTuMianSta;//0:����,1:͹��
	int AoTuMianDiam;//����ֱ��
	
	struct FANXIANGXZSTR FanXiangXZPara;
}DETECTPARA;

extern SOURCE_DATA SourceData;
extern COVER_BUFFER ResBuf_CH1;
extern COVER_BUFFER ResBuf_CH2;
extern COVER_BUFFER ResBuf_CH3;
extern COVER_BUFFER* ResBuf[3];

extern DETECTPARA g_DetectPara;

extern u8 JGJData[12][9];//���ݴ洢

//**************�ⲿ���ú���*******************************************************
//��ȼ��ҳ��
void ui_Sample10_InitDisp(void);
void ui_Sample10_Run(void);
//��ʾ���������б�
void SampParaOptions(SYS_PARA_TABLE_STRUCT* ParaTableArr,u8 pageCap,u8 pageNum);
//������
void DispScrollBar(short pageQnt,short pageNum,short pageLast);

void GetHouDuAvg(void);
#endif
