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


//参数类型
#define TYP_VALUE_DATA            	0  		//无符号8位类型
#define TYP_VALUE_ASIC              1       //字符类型
#define TYP_LIST_HANZI 				2  		//汉字列表
#define TYP_LIST_ASIC 				3 		//字符列表
#define TYP_VALUE_DATE              4       //日期类型
#define TYP_VALUE_TIME              5       //时间类型
#define TYP_BOTTOM_STRING           6       //时间类型
#define TYP_VALUE_PASSWORD          7       //密码类型
#define TYP_VALUE_INT               8       //整型类型

//参数表
typedef struct {
	unsigned char index;
	unsigned char listStr[27];
}PARA_LIST_STRUCT;

#define PARA_GROUP_SYSTEM	0				//系统参数分组
#define PARA_GROUP_SAMPLE	1				//采集参数分组

//参数设置表
typedef struct {//长度30
	unsigned char group;					//参数分组
    unsigned char ParaID;                   //参数ID
	u8 sChName[12];					        //中文参数名称
    char sEnName[12];				        //英文参数名称
	unsigned char ucTableLen;				//单个表长度
	unsigned char ucParaType;		        //参数显示类型      0=无，  1=字符串，  2=整数，  3=列表
	char *Unit;								//单位
	PARA_LIST_STRUCT *pList;  			    //列表参数指针
	int iParaListLen;						//参数列表长度
 	int iParaMinVal;      		    		//参数最小值
	int iParaMaxVal;      		    		//参数最大值
	int iParaDftVal;						//参数默认值
	int* ParaAddr;						    //参数地址
}SYS_PARA_TABLE_STRUCT,*SYS_PARA_TABLE_STRUCT_P;

//页面菜单按钮表
typedef struct {
	unsigned char btnID;		//编号
	unsigned char btnQnt;		//按钮数量
	unsigned char* btnIcon;		//图标
	unsigned char* btnName;		//名称
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
