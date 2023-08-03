#ifndef __TMA568_H
#define __TMA568_H

#include "iic.h"

//IO操作函数	 
#define FT_RESET_H() 			(GPIOE->BSRR =  GPIO_Pin_3)		//FT5206复位引脚
#define FT_RESET_L() 			(GPIOE->BRR =  GPIO_Pin_3)			//FT5206复位引脚
#define FT_INT    				//((GPIOC->IDR & GPIO_Pin_7)!=0)		//FT5206中断引脚

#define TP_INT_IS_H	            ((GPIOE->IDR & GPIO_Pin_4)!=0)
#define TP_INT_IS_L	            ((GPIOE->IDR & GPIO_Pin_4)==0)

//I2C读写命令	
#define FT_CMD_WR 				0X48    	//写命令
#define FT_CMD_RD 				0X49		//读命令
  
//CYTMA568 部分寄存器定义 
#define FT_DEVIDE_MODE 			0x00   		//FT5206模式控制寄存器

#define  INT_PULSE_DATA         0x1e        //中断脉冲时间（ms）
#define  INTERRUPT_PIN_OVERRIDE 0x2e        //中断类型

#define FT_REG_NUM_FINGER       0x03		//触摸状态寄存器

#define FT_TP1_REG 				0X03	  	//第一个触摸点数据地址
#define FT_TP2_REG 				0X09		//第二个触摸点数据地址
#define FT_TP3_REG 				0X0F		//第三个触摸点数据地址
#define FT_TP4_REG 				0X15		//第四个触摸点数据地址
#define FT_TP5_REG 				0X1B		//第五个触摸点数据地址  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//版本
#define FT_ID_G_MODE 			0xA4   		//FT5206中断模式控制寄存器
#define FT_ID_G_THGROUP			0x80   		//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE	0x88   		//激活状态周期设置寄存器

#define TP_NUM_MAX				(5)			// 最多5点触控

// 触点坐标及触点个数
extern u16 TpX[TP_NUM_MAX];
extern u16 TpY[TP_NUM_MAX];
extern s16 TpNum;

u8 CYTMA568_WR_Reg(u16 reg,u8 *buf,u8 len);
void CYTMA568_RD_Reg(u16 reg,u8 *buf,u8 len);
u8 CYTMA568_Init(void);
u8 CYTMA568_Scan(void);

#endif

