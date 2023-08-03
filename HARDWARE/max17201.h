///////////////////////////////////////////////////////////////////////////////
//模块型号： max17201
///////////////////////////////////////////////////////////////////////////////

	
#ifndef __MAX17201_H_
#define __MAX17201_H_
#include "stm32f10x.h"
/*  SLAVE ADDRESS          PROTOCOL        ADDRESS BYTE RANGE          INTERNAL MEMORY RANGE ACCESSED
        6Ch                  I2C                 00h–FFh                       000h–0FFh
        16h
                             SMBUS               00h–7Fh                       100h–17Fh
                             I2C                 80h–FFh                       180h–1FFh */
                             
/* . All commands are written to the Command register at memory address 060h to perform the desired operation.*/
#define Command_register_addr   0x60

/* The CommStat register can be used to track the status of the request */
#define CommStat_register_addr  0x61

//1:NVBusy  NVBusy defaults to 0 after reset indicating nonvolatile memory is idle. This bit is set after a nonvolatile related command is sent to the command register 
//and clears automatically after the operation completes

//2:NVError This bit is set if there was an error executing the command. Once set, the bit must be cleared by software to detect the next error

#define command_COPY_NV_BLOCK  0xE904
/* This command copies the entire block from shadow RAM
   to nonvolatile memory addresses 180h to 1DFh excluding
   the unique ID locations of 1BCh to 1BFh. After issuing this
   command, the host must wait tBLOCK for the operation
   to complete. The configuration memory can be copied a
   maximum of 7 times. Note that the supply voltage must be
   above VNVM for the operation to complete successfully */
   
#define command_NV_RECALL  0xE001
/* This command recalls the entire block from nonvolatile
   memory to shadow RAM addresses 180h to 1DFh. This
   is an low power operation that will take up to tRECALL to
   complete. Note that the supply voltage must be above
   VNVM for the operation to complete successfully. */

/* HISTORY RECALL [E2XXh]
   This command copies history data into page 1Eh of
   memory. After issuing this command, the host must wait
   tRECALL for the operation to complete before reading
   page 1Eh. Table 20 shows what history information can
   be recalled. See the SHA-256 Authentication, Battery
   Life Logging, and Memory sections for details on how to
   decode this information. */
#define command_Hardware_Reset  0x000F  

 

#define nRsense0_005R  0X01F4
#define nRsense0_010R  0X03E8
#define nRsense0_005R  0X01F4

                             
#define MAX17201_I2C_ADDRESS1       0x6c       //  00h–FFh

#define MAX17201_I2C_ADDRESS2       0x16       //SMBUS 00h-7Fh   IIC  80h-FFh

#define     DevName_register_addr               0x021
#define     nPackCfg_register_addr              0xB5        //Nonvolatile Restore: PackCfg (0BDh)
#define     PackCfg_register_addr               0xBD 
#define     nNVCfg0_register_addr               0xB8        //1B8h
#define     nNVCfg1_register_addr               0xB9        //1B9h
#define     nNVCfg2_register_addr               0xBA       //1BAh

#define     nRSense_register_addr               0xCF 
#define     Batt_register_addr                  0X0DA       //1.25mv 
#define     AvgCurrent_register_addr            0X00B       //5.625s 更新一次
#define     Current_register_addr               0X00A       //5.625s 更新一次
#define     nCGain_register_addr                0Xc8        //
#define     CGain_register_addr                 0X2e        //
//#define     FullCapRep_register_addr            0X035       //
#define     nFullSOCThr_register_addr           0Xc6        //1C6h The nFullSOCThr register gates detection of end-of charge
#define     FullSOCThr_register_addr            0X13        //FullSOCThr register gates detection of end-of charge
#define     nDesignVoltage_register_addr        0XD3        //1D3h  LSB 1MV
#define     Status_register_addr                0X00        //Contains alert status and chip status
#define     Status2_register_addr               0XB0        //Contains alert status and chip status

#define     VCell_register_addr                 0X09        //Lowest cell voltage of a pack, or the cell voltage for a single cell

#define     AvgVCell_register_addr              0X019      //FilterCfg register description for details on setting the time filter

/* Note that when a cell relaxation event is detected, the averaging period changes to the period defined by the RelaxCfg.dt setting */

#define     RepSOC_register_addr                0X06        //Reported state of charge
#define     RepCap_register_addr                0X05        //Reported remaining capacity
#define     Temp_register_addr                  0X08        //
//#define     Current_register_addr               0X0a        //
#define     TTE_register_addr                   0X11        //Time to empty
#define     TTF_register_addr                   0X20         //Time to full
#define     FullCap_register_addr               0X10         //
#define     MixCap_register_addr                0X0f         //The MixCap output register tracks the charge state of the cell
//FullCapRep Register (035h)
#define     FullCapRep_register_addr            0X35         //A new full-capacity value is calculated at the end of every charge cycle in the application
/* FullCapRep Register (035h)
   Register Type: Capacity
   Nonvolatile Backup and Restore From: nFullCapRep
   (1A9h) or nFullCapNom (1A5h). See nNVCfg2.EnFC.
   This register reports the full capacity that goes with
   RepCap, generally used for reporting to the user. A new
   full-capacity value is calculated at the end of every charge
   cycle in the application */



#define     FStat_register_addr                 0X3D     //The FStat register is a read-only register that monitors the status of the ModelGauge algorithm


#define     FilterCfg_register_addr            0x29 //029   
#define     nFilterCfg_register_addr            0x9D //19Dh 


// VEmpty (03Ah)    nVEmpty Register (19Eh)  VEmpty (03Ah) if nNVCfg0.enVE is set    Alternate Initial Value: 0xA561 (VE=3.3V, VR=3.88V) 

#define     VEmpty_register_addr            0x3A //03Ah   
#define     nVEmpty_register_addr           0x9E //19Eh 

/* This register stores the typical current experienced by
   the fuelgauge when empty has occurred. If enabled, this
   register is periodically backed up to nonvolatile memory
   as part of the learning function. */
   
#define     nIAvgEmpty_register_addr           0xA8 //(1A8h
#define     IAvgEmpty_register_addr            0x36 //(036

//满电量相关

#define     VFSOC_register_addr                0xff //VFSOC Register (0FFh)
#define     nIChgTerm_register_addr            0x9C //nIChgTerm Register (19Ch)
#define     IChgTerm_register_addr             0x1E  //IChgTerm (01Eh) 

//上下限设置
//nIAlrtTh Register (1C3h)

//Register Type: Special
//Nonvolatile Restore: IAlrtTh (0B4h) if nNVCfg1.enAT is set.
//Alternate Initial Value: 0x7F80 (Disabled)

#define     nIAlrtTh_register_addr           0xC3 //(1A8h
#define     IAlrtTh_register_addr            0xB4 //(036


typedef struct NPACKCFG_REGISTER_TYPE
{
    unsigned short NCELLS:4;
    unsigned short ZERO1:1;
    unsigned short BALCFG:3;
    unsigned short CxEn:1;
    unsigned short BtEn:1;
    unsigned short ChEn:1;
    unsigned short TdEn:1;
    unsigned short A1En:1;
    unsigned short A2En:1;
    unsigned short ZERO2:1;
    unsigned short FGT:1;
}NPACKCFG_REGISTER_TYPE_STRU;

typedef struct 
{
    unsigned short enSBS:1;     //0
    unsigned short enHCfg:1;    //1
    unsigned short enAF:1;      //2
    unsigned short enMC:1;      //3
    unsigned short enDC:1;      //4
    unsigned short enVE:1;      //5
    unsigned short enCG:1;      //6
    unsigned short enICT:1;     //7
    unsigned short enLCfg:1;    //8
    unsigned short enRCfg:1;    //9
    unsigned short enFCfg:1;    //10
    unsigned short enCfg:1;     //11
    unsigned short ZERO1:2;     //12-13
    unsigned short enX:1;       //14
    unsigned short enOCV:1;     //15
}nNVCfg0_REGISTER_TYPE_STRU;


typedef struct 
{
    unsigned short ZERO1:1;		//0
    unsigned short enCTE:1;		//1
    unsigned short enCrv:1;		//2
    unsigned short enAT:1;		//3
    unsigned short enTTF:1;		//4
    unsigned short ZERO2:6;		//5-10
    unsigned short enODSC:1;	//11
    unsigned short enRFVSH:1;	//12
    unsigned short enFTH:1;		//13
    unsigned short enMTL:1;		//14
    unsigned short enTGO:1;		//15
}nNVCfg1_REGISTER_TYPE_STRU;

typedef struct 
{
    unsigned short CYCLESpSAVE:7;//0-6
    unsigned short enMet:1;             //7
    unsigned short enIAvg:1;            //8
    unsigned short enFC:1;              //9
    unsigned short enVT:1;              //10
    unsigned short enMMC:1;             //11
    unsigned short enMMV:1;             //12
    unsigned short enMMT:1;             //13
    unsigned short enSOC:1;             //14
    unsigned short enT:1;               //15
}nNVCfg2_REGISTER_TYPE_STRU;


typedef struct 
{
    unsigned short Status; //Contains alert status and chip status
    unsigned short VCell;  //Lowest cell voltage of a pack, or the cell voltage for a single cell
    unsigned short RepSOC; //Reported state of charge
    unsigned short RepCap; //Reported remaining capacity
    short Temp;    //Temperature
    short Current; //Battery current
    unsigned short TTE;     //Time to empty
    unsigned short TTF;     //Time to full    
    unsigned short FullCap;     //
    unsigned short FilterCfg;     // 
    unsigned short AvgVCell;
    unsigned short VEmpty;
    unsigned short nVEmpty; 
    unsigned short Status2;
    short IAvgEmpty;
    unsigned short MixCap;
    unsigned short FullCapRep; 
    unsigned short  VFSOC;
    unsigned short FullSOCThr;
    short IAlrtTh;
    short IChgTerm;
}MAX17201_REGISTER_VALUE_STRU;

typedef struct 
{
    unsigned short undefined1:1;     //0
    unsigned short POR:1;            //1
    unsigned short Imn:1;      //2
    unsigned short Bst:1;      //3
    unsigned short undefined2:1;      //4  
    unsigned short undefined3:1;      //5
    unsigned short Imx:1;      //6
    unsigned short dSOCi:1;     //7
    unsigned short Vmn:1;    //8  
    unsigned short Tmn:1;    //9
    unsigned short Smn:1;    //10 
    unsigned short Bi:1;     //11    
    unsigned short Vmx:1;     //12
    unsigned short Tmx:1;       //13  
    unsigned short Smx:1;     //14
    unsigned short Br:1;     //15
}STATUS_REGISTER_TYPE_STRU;


typedef struct 
{
    unsigned short DNR:1;			//0
    unsigned short undefined1:5;	//1-5
    unsigned short RelDt2:1;		//6
    unsigned short FQ:1;			//7
    unsigned short EDet:1;			//8
    unsigned short RelDt:1;			//9
    unsigned short undefined2:6;	//10-15
            
}rFSTAT_REGISTER_TYPE_STRU;

typedef union
{
    rFSTAT_REGISTER_TYPE_STRU    rFstat_register_bits;
    unsigned short               rFstat_register_short;
}rFSTAT_REGISTER_TYPE_UNION;




typedef union
{
    STATUS_REGISTER_TYPE_STRU    status_register_bits;
    unsigned short               status_register_short;
}STATUS_REGISTER_TYPE_UNION;


typedef struct 
{
    unsigned short undefined1:1;     //0
    unsigned short NVBusy:1;    //1
    unsigned short NVError:1;      //2
    unsigned short undefined2:13;      //3-15
            
}CommStat_REGISTER_TYPE_STRU;

typedef union
{
    
    CommStat_REGISTER_TYPE_STRU  CommStat_register_bits;
    unsigned short               CommStat_register_short;
}CommStat_REGISTER_TYPE_UNION;


/* VEmpty */
typedef struct 
{
    unsigned short VR:7;     //0-6
    unsigned short VE:9;    //7-15
            
}VEmpty_REGISTER_TYPE_STRU;

typedef union
{
    
    VEmpty_REGISTER_TYPE_STRU    VEmpty_register_bits;
    unsigned short               VEmpty_register_short;
}VEmpty_REGISTER_TYPE_UNION;


typedef struct 
{
    unsigned short VR:7;     //0-6
    unsigned short VE:9;    //7-15
            
}nVEmpty_REGISTER_TYPE_STRU;

typedef union
{
    
    nVEmpty_REGISTER_TYPE_STRU    nVEmpty_register_bits;
    unsigned short               nVEmpty_register_short;
}nVEmpty_REGISTER_TYPE_UNION;



typedef union
{
    
    NPACKCFG_REGISTER_TYPE_STRU  npackcfg_register_bits;
    unsigned short               npackcfg_register_short;
}NPACKCFG_REGISTER_TYPE_UNION;

typedef union
{
    
    nNVCfg0_REGISTER_TYPE_STRU   nNVCfg0_register_bits;
    unsigned short               nNVCfg0_register_short;
}nNVCfg0_REGISTER_TYPE_UNION;

typedef union
{
    
    nNVCfg1_REGISTER_TYPE_STRU   nNVCfg1_register_bits;
    unsigned short               nNVCfg1_register_short;
}nNVCfg1_REGISTER_TYPE_UNION;


typedef union
{
    
    nNVCfg2_REGISTER_TYPE_STRU   nNVCfg2_register_bits;
    unsigned short               nNVCfg2_register_short;
}nNVCfg2_REGISTER_TYPE_UNION;
extern MAX17201_REGISTER_VALUE_STRU max17201_register_value_stru;




///////////////////////////////////////////////////////////////////////////////
//模块型号： max17201
//硬件平台： HT225S V1.26
//当前版本： 2018.08.13 V1.0
//移植需要注意:	1.电池电压最小值和最大值
//				2.电池电量 
//外部调用函数：1.Max17201Init		= 模块初始化
//				2.Get_BatteryPerent	= 获取百分比 0-100(正常值) 
//				3.Get_BatteryState  = 电池充电状态
///////////////////////////////////////////////////////////////////////////////

/* ----------空电量点检测相关宏定义---------- */
#define nVEmpty_VE 3.30        //低于此点确定空电量点
//#define nVEmpty_VR 4.1       //Sets the voltage level for clearing empty detection
#define nVEmpty_VR 4.1       //Sets the voltage level for clearing empty detection

//电池个数
#define BatteryC 1

//电池电量 = (原始电量3400)*2
#define BatteryMah 6800      
#define AdvanceMah 2950      


#define MAX17201_Dat_RCC	RCC_APB2Periph_GPIOB
#define MAX17201_Dat_PORT	GPIOB
#define MAX17201_Dat_PIN	GPIO_Pin_6

#define MAX17201_SCL_RCC	RCC_APB2Periph_GPIOB
#define MAX17201_SCL_PORT	GPIOB
#define MAX17201_SCL_PIN	GPIO_Pin_7


extern u8  g_MaxState;
extern u16 g_MaxMaxDianLiang; 
extern u16 g_MaxDianLiang;
extern u16 g_MaxDianya;

extern u16 g_StartDianYa;
extern u16 g_MaxOffDianya;
extern u8 g_17201state;
extern s16 g_DianYaChar;
extern u16 g_StartDianLiang;

 
u8 Max17201Init(void);		//模块初始化,当前检测到电池更换,对芯片初始化700ms  	返回:0=芯片错误  1=成功
u8 Get_BatteryPerent(void);	//获取电量,执行一次时间3ms,20秒以上读取一次			返回:电池电量
u8 Get_BatteryState(void);	//电池充电状态 		返回:0=放电状态  1=充电状态  2=没有安装电池，连接USB
unsigned short Get_BatteryDianYa(void);

#endif

