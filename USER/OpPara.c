/******************************************************************************
创建者：康泉
创建日期：2018.03.01
功能描述：
    1. 参数读写相关函数
    2. 全局参数按照不同分类，建议采用结构体形式存储，同时预留20%--30%字节
    
更新记录：
    1. 2018.03.01 初次创建
*******************************************************************************/

#include "OpPara.h"
#include "OpFile.h"
#include "Config_ui.h"

#define  MCU_F103VG         //1024K  FLASH
//#define  MCU_F103VE         //512K  FLASH
//#define  MCU_F103VC       //256K  FLASH

#ifdef MCU_F103VG
    #define  SYS_PARA_BaseAddr      0x08009000//0x080FF000    //4096字节大小,实际使用2K字节
#else
    #define  SYS_PARA_BaseAddr      0x08009000//0x0803F000    //4096字节大小,实际使用2K字节
#endif 

//#ifdef MCU_F103VE
//    #define  SYS_PARA_BaseAddr      0x0807F000    //4096字节大小,实际使用2K字节
//#else
//    #define  SYS_PARA_BaseAddr      0x0803F000    //4096字节大小,实际使用2K字节
//#endif 

u8 debug = 0;

//文件目录
char FileDirectory[16] = "0:/GY91";

//参数结构体变量
STR_SYS_PARA  g_SysPara;
GLOBAL_PARA	g_GlobalPara;
BatDataTest g_BatDataTest;//电量测试

//螺纹钢
const int iFact_LWSmall_Fi[16] =   {1000,1001,1002,1003,1004,1005,1006,1007,1008,1009,1010};
const int iFact_LWSmall_Zero[16] = {   0,  11,  22,  33,  44,  55,  66,  77,  88,  99,  11};
const int iUser_LWSmall_Zero[16] = {0,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
const int iFact_LWBig_Fi[16] =     {0,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
const int iFact_LWBig_Zero[16] =   {0,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
const int iUser_LWBig_Zero[16] =   {0,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};

//圆钢
const int iFact_YGSmall_Fi[16] =   {0,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
const int iFact_YGSmall_Zero[16] = {0,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
const int iUser_YGSmall_Zero[16] = {0,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
const int iFact_YGBig_Fi[16] =     {0,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
const int iFact_YGBig_Zero[16] =   {0,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
const int iUser_YGBig_Zero[16] =   {0,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
#define SoftVer	100

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitSysPara(void)
{
	ReadSysPara();
	sprintf(g_SysPara.SoftVersion,"%s","V1.00.201015");
	sprintf(g_SysPara.HardVersion,"%s","V1.0");
	RX8010_ReadTime(&g_SysTime);
//	Get_Hard_Version();
	
	//全局参数初始化
	if(g_SysPara.g_SamplePara.iScreenWide == 0){
		g_GlobalPara.sDispScreenWide = 800;
	}else{
		g_GlobalPara.sDispScreenWide = 1200;
	}
	
	//系统参数设置
	if((g_SysPara.sID != 0x2020) || (g_SysPara.sSoftVer != SoftVer)){
		LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_ON);
		LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_ON);
//		W25QXX_Init();//初始化W25Q128
//		my_mem_init(SRAMIN);//初始化内部内存池
//		exfuns_init();//为fatfs相关变量申请内存
//		f_mount(fs[0],"0:",1);//挂载FLASH
//		f_mkfs("0:",1,4096);//格式化FLASH,0:盘符; 1,不需要引导区,8个扇区为1个簇
//		f_mkdir("0:/HCVNS");
		
		g_SysPara.sID = 0x2020;
		g_SysPara.sSoftVer = SoftVer;
		
		g_SysPara.iTouchSoundSta = 1;
		g_SysPara.iBlueToothSta = 1;
		g_SysPara.iLanguage = 0;
		g_SysPara.iBKLedLight = 2;	//背光亮度 2->75%
		g_SysPara.iLaserColSta = 0;
		g_SysPara.iLaserRowSta = 0;
		
		g_SysPara.iBkLedOffMinCnt = 20;	//20 分钟
		g_SysPara.iPowerOffMinCnt = 30;	//30 分钟
		
		memcpy(g_SysPara.iFact_LWSmall_Fi,iFact_LWSmall_Fi,16*sizeof(int));
		memcpy(g_SysPara.iFact_LWSmall_Zero,iFact_LWSmall_Zero,16*sizeof(int));
		memcpy(g_SysPara.iUser_LWSmall_Zero,iUser_LWSmall_Zero,16*sizeof(int));

		memcpy(g_SysPara.iFact_LWBig_Fi,iFact_LWBig_Fi,16*sizeof(int));
		memcpy(g_SysPara.iFact_LWBig_Zero,iFact_LWBig_Zero,16*sizeof(int));
		memcpy(g_SysPara.iUser_LWBig_Zero,iUser_LWBig_Zero,16*sizeof(int));
		
		memcpy(g_SysPara.iFact_YGSmall_Fi,iFact_YGSmall_Fi,16*sizeof(int));
		memcpy(g_SysPara.iFact_YGSmall_Zero,iFact_YGSmall_Zero,16*sizeof(int));
		memcpy(g_SysPara.iUser_YGSmall_Zero,iUser_YGSmall_Zero,16*sizeof(int));

		memcpy(g_SysPara.iFact_YGBig_Fi,iFact_YGBig_Fi,16*sizeof(int));
		memcpy(g_SysPara.iFact_YGBig_Zero,iFact_YGBig_Zero,16*sizeof(int));
		memcpy(g_SysPara.iUser_YGBig_Zero,iUser_YGBig_Zero,16*sizeof(int));
		
		g_SysPara.SampleDispMode = 0;
		g_SysPara.AnalyzeDispMode = 0;
		
//		g_SysPara.bEquipmentType = 5;
//		g_SysPara.bDecimalDigits = 3;
//		g_SysPara.fRangeMax = 200.0f;
//		g_SysPara.fRangeMin = 0.0f;
		
		sprintf(g_SysPara.EquipmentNum,"%s","10020091102");
		sprintf(g_SysPara.BlueToothNum,"HCGY91-%s",&g_SysPara.EquipmentNum[7]);
		sprintf(g_SysPara.GouJianName,"%s","HCGK0001");
		
		g_SysPara.iBatFileCnt = 0;
		g_SysPara.iSavedFileQuant = 0;
		
		//初始化参数列表
		ParaTableDefaultAll();
		
		//设置蓝牙
		BT_Init();
		BT_SetName(g_SysPara.BlueToothNum);
		
		SaveSysPara();
		Sleep_ms(200);
		SaveSysPara();
		
		LED_Ctrl(LED_RED,LED_NUM_ALL,LED_STA_OFF);
		LED_Ctrl(LED_BLUE,LED_NUM_ALL,LED_STA_OFF);
	}
}

void ReadSysPara(void)
{
	int ByteOffest;
	unsigned char *pData;
	pData = (unsigned char *)&g_SysPara;

	__disable_irq();
	for(ByteOffest = 0; ByteOffest < sizeof(STR_SYS_PARA); ByteOffest += 1){
		*pData = *(u8*)(SYS_PARA_BaseAddr + ByteOffest);
		pData += 1;
    }
	__enable_irq();
}

void SaveSysPara(void)
{
    int ByteOffest;
    unsigned char *pData;
    pData = (unsigned char *)&g_SysPara;
    
    __disable_irq();
 
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASH_ErasePage(SYS_PARA_BaseAddr);
    Sleep_us(50);
    for(ByteOffest = 0; ByteOffest < sizeof(STR_SYS_PARA); ByteOffest += 2)
	{
		FLASH_ProgramHalfWord(SYS_PARA_BaseAddr + ByteOffest, *(u16*)(pData + ByteOffest));
    }
    FLASH_Lock();
        
    __enable_irq();        

	SysTick_Init();  //写Flash后， 需要重新初始化系统定时器，NND	
}




