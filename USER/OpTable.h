#ifndef OPTABLE_H
#define OPTABLE_H	

#include "mcu_io.h"

#define PARATABLELEN	45
#define PARATABLEINDEX_SAMPLE		0
#define PARATABLEINDEX_SAMPLEHIGH	5
#define PARATABLEINDEX_JGJ			13
#define PARATABLEINDEX_BLOCK		22
#define PARATABLEINDEX_VERIFY		30
#define PARATABLEINDEX_SYSTEM		33


//��������
#define TYP_VALUE_DATA            	0  		//�޷���8λ����
#define TYP_VALUE_ASIC              1       //�ַ�����
#define TYP_LIST_HANZI 				2  		//�����б�
#define TYP_LIST_ASIC 				3 		//�ַ��б�
#define TYP_VALUE_DATE              4       //��������
#define TYP_VALUE_TIME              5       //ʱ������
#define TYP_BOTTOM_STRING           6       //ʱ������
#define TYP_VALUE_PASSWORD          7       //��������
#define TYP_VALUE_INT               8       //��������

//������
typedef struct {
	unsigned char index;
	unsigned char listStr[27];
}PARA_LIST_STRUCT;

#define PARA_GROUP_SYSTEM	0				//ϵͳ��������
#define PARA_GROUP_SAMPLE	1				//�ɼ���������

//�������ñ�
typedef struct {//����30
	unsigned char group;					//��������
    unsigned char ParaID;                   //����ID
	u8 sChName[12];					        //���Ĳ�������
    char sEnName[12];				        //Ӣ�Ĳ�������
	unsigned char ucTableLen;				//��������
	unsigned char ucParaType;		        //������ʾ����      0=�ޣ�  1=�ַ�����  2=������  3=�б�
	char *Unit;								//��λ
	PARA_LIST_STRUCT *pList;  			    //�б����ָ��
	int iParaListLen;						//�����б���
 	int iParaMinVal;      		    		//������Сֵ
	int iParaMaxVal;      		    		//�������ֵ
	int iParaDftVal;						//����Ĭ��ֵ
	int* ParaAddr;						    //������ַ
}SYS_PARA_TABLE_STRUCT,*SYS_PARA_TABLE_STRUCT_P;

//ҳ��˵���ť��
typedef struct {
	unsigned char btnID;		//���
	unsigned char btnQnt;		//��ť����
	unsigned char* btnIcon;		//ͼ��
	unsigned char* btnName;		//����
}MENU_TABLE_STRUCT,*MENU_TABLE_STRUCT_P;

typedef struct {
	unsigned char pageNum;
	unsigned char btnNum;
	u16 sX;
	u16 sY;
	u16 eX;
	u16 eY;
}BUTTON_PARA_STRUCT;

extern PARA_LIST_STRUCT GoujianNameList[1];

extern SYS_PARA_TABLE_STRUCT ParaTable[PARATABLELEN];

extern const unsigned int Rebar_Table_Diam[16];

void ParaTableDefault(SYS_PARA_TABLE_STRUCT* tableAddr);
void ParaTableDefaultAll(void);

#endif
