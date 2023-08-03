#ifndef __GPS_H
#define __GPS_H	 

#define true	1
#define false	0

typedef struct{
	char UTC_Time[11];		//UTCʱ��(ʱ����.����)��ʽ	  ʱ����
	char status;	        //��λ״̬ A:��Ч V:��Ч   ' ':��ʱ��Ч��
	char latitude[13];		//γ��ddmm.mmmm (�ȷ�)��ʽ
	char lat_NS;	        //����
	char longitude[13];		//����dddmm.mmmm (�ȷ�)��ʽ
	char lon_WE;	        //����
	char UTC_Date[8];		//UTC���ڣ�ddmmyy(������)��ʽ  ������
    char RunState;          //����״̬ bit1����״̬ bit0  1���� 0�ر�״̬
    char cSaveFlag;         //��λ����
    unsigned short int usCountDown;       //��ʱ�����͹ر� 3K=1����
    
	char latitudeBuf[10];	//��ʾγ��,x.xxxxx��
	char longitudeBuf[10];	//��ʾ����
    
    double lat;             //γ��,x.xxxxx��
    double lon;             //����
    
}GPS_INFO_STR;

extern GPS_INFO_STR GPS_Info; // GPS_Info.cSaveFlag  longitudeBuf


void GPS_Init(void);	//Ĭ��9600
void GPS_Stop(void);	//
void GPS_Start(void);	//
void GPS_ManualStart(void);	//�ֶ���ʼ
void GPS_CountDown(void);	//ϵͳ20ms��ʱ�����ã��Զ���λ����ʱ����

void GPS_Location_Data(void);//�������ѯ����λ����

#endif

