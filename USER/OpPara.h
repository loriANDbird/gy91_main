#ifndef OPPARA_H
#define OPPARA_H

#include "Config_ui.h"
#include "OpFile.h"
#include "usb_type.h"

//��������
typedef struct{
	int iGoujianName;		//�������
	int iZhujinDiam;		//����ֱ��
	int iZhujinJianju;		//������
	int iDesignHoudu;		//��ƺ��
	int iRebarType;			//�ֽ�����
	int iRebarNumJGJ;		//�ֽ����
	//�߼�����
	int iGujinDiam;			//����ֱ��
	int iGujinJianju;		//������
	int iSaveType;			//�洢��ʽ
	int iGoujianType;		//��������
	int iFugaiHoudu;		//���Ǻ��
	int iAomianDiam;		//����ֱ��
	int iTumianDiam;		//͹��ֱ��
	int iScreenWide;		//�������
	
	int iBlockDiamX;		//����Xֱ��
	int iBlockJianjuX;		//����X���
	int iBlockDiamY;		//����Yֱ��
	int iBlockJianjuY;		//����Y���
	int iBlockDesignHoudu;	//������ƺ��
	int iBlockRebarType;	//����ֽ�����
	int iBlockAreaNum;		//����ɨ��������

}SAMPLE_PARA;

typedef struct  {
	unsigned short sID;				//ID���
	unsigned short sSoftVer;		//����汾��

	int iBlueToothSta;				//�������� 		0:�� 1:��
	int iTouchSoundSta;				//������Ч����	0:�� 1:��
	int iBKLedLight;				//��������		1:50% 2:75% 3:100%,Ĭ��2
	int iLanguage;					//����ѡ��		0:���� 1:Ӣ��
	int iLaserColSta;				//��ֱ����		0:�� 1:��
	int iLaserRowSta;				//ˮƽ����		0:�� 1:��
	int iTheme;						//����			0:�� 1:��

	int iBkLedOffMinCnt;			//�رձ�����Ӽ���
	int iPowerOffMinCnt;			//�ػ�ʱ����Ӽ���
	
	int	iSavedFileQuant;			//�洢���ݸ���
//	int iSavedFileNumMax;			//�Ѵ�����ļ�����ֵ
	int iBatFileCnt;				//�������Դ洢�ļ�����
    
    unsigned int uiCh1Zero;         //ͨ��1���
    unsigned int uiCh2Zero;         //ͨ��2���
    unsigned int uiCh3Zero;         //ͨ��3���
    unsigned int uiCh4Zero;         //ͨ��4���
    unsigned int uiCh5Zero;         //ͨ��5���
    unsigned int uiCh6Zero;         //ͨ��6���
	
	int iFact_LWSmall_Fi[16];		//�������Ƹ�С�������ȱ�
	int iFact_LWSmall_Zero[16];		//�������Ƹ�С��������
	int iUser_LWSmall_Zero[16];		//�û����Ƹ�С��������
	int iFact_LWBig_Fi[16];			//�������Ƹִ��������ȱ�
	int iFact_LWBig_Zero[16];		//�������Ƹִ���������
	int iUser_LWBig_Zero[16];		//�û����Ƹִ���������
	
	int iFact_YGSmall_Fi[16];		//����Բ��С�������ȱ�
	int iFact_YGSmall_Zero[16];		//����Բ��С��������
	int iUser_YGSmall_Zero[16];		//�û�Բ��С��������
	int iFact_YGBig_Fi[16];			//����Բ�ִ��������ȱ�
	int iFact_YGBig_Zero[16];		//����Բ�ִ���������
	int iUser_YGBig_Zero[16];		//�û�Բ�ִ���������
	
	char GouJianName[32];			//��������
	char EquipmentNum[16];			//�豸���
	char BlueToothNum[16];			//�������
	char HardVersion[16];			//Ӳ���汾
	char SoftVersion[16];			//����汾
	float fRangeMax;				//��������
	float fRangeMin;				//��������
	float fTemperatureBDLast;		//�ϴα궨�¶�
	int iBatPercentBDLast;		//�ϴα궨����

	u8 SampleDispMode;//������4����ʾģʽ0-3
	u8 AnalyzeDispMode;//��������4����ʾģʽ0-3
	
	SAMPLE_PARA g_SamplePara;		//������
	
	u8 JGJScanDir;					//JGJɨ�跽�� 0������1������
    
    short int iBigFi_LW[16];		//������  0��Ϊ  ����������   ���Ƹ�
	short int iBigZero_LW[16];		//������  0��Ϊ  ���������   ���Ƹ�

}STR_SYS_PARA;

//ȫ�ֱ���
typedef struct {
	u8 iOpenType;			//0:��������,1:��翪��
	u8 iChargeSta;			//���״̬��ʱ 0:δ��磬1:����У�2:������
	u8 iChargeStaRes;		//���״̬ 0:δ��磬1:����У�2:������
	u8 iBatPercent;		//�����ٷֱ�
	u8 iBatPcntMin;		//�����ٷֱ���Сֵ
	u8 iBatPcntMax;		//�����ٷֱ����ֵ
	u8 iBatPcntLowFlag;	//�����ͱ�־
	float fBatVoltage;		//��ص�ѹ
	float fTemperature;		//�¶�
	
	short sDispScreenWide;	//һҳ��Ļ��ʾ���룬400��800mm
	
	u8 iStorageFreePrcnt;	//ʣ�������ٷְ�
	
	u8 ucDispModepage;		//��ʾģʽҳ�� 0�����ټ�⣬1������
	u8 ucFromPageNum;		//ĸ������ţ����ڲ������úͱ궨����
	
	u8 ucHouduAvg;			//ƽ�����
	u8 ucHouduMax;			//�����
	u8 ucHouduMin;			//��С���
	
	int iSaveFileCnt;		//�洢�ļ�����
	int iSaveDataCnt;		//�������ݼ���
	
	int iCurFileNum;		//��ǰ�ļ����

}GLOBAL_PARA;

//�������ݴ洢�ṹ
typedef struct BatDataTest{
	int iMinutCount;		//ʱ�����
	int iSaveCount;			//�������
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
