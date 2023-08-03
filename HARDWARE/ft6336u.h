#ifndef _FT6336U_H
#define _FT6336U_H

#include "stm32f10x.h"

//I2C读写命令
//FT6336
#define TP_CMD_WR		((0x38<<1)|0x00)	//写命令
#define TP_CMD_RD		((0x38<<1)|0x01)	//读命令
#define TP_CMD_X		0X03		//读X
#define TP_CMD_Y		0X05		//读Y

//GT911
//#define GT_CMD_WR		0X28        //写命令
//#define GT_CMD_RD		0X29        //读命令

#define GT_CFG_VERSION	0x5A		//配置文件版本
  
//GT911 部分寄存器定义
#define GT_CTRL_REG		0X8040       //GT911控制寄存器
#define GT_CFGS_REG		0X8047       //GT911配置起始地址寄存器
#define GT_CHECK_REG	0X80FF       //GT911校验和寄存器
#define GT_PID_REG		0X8140       //GT911产品ID寄存器

#define GT_GSTID_REG	0X814E       //GT911当前检测到的触摸情况

//触摸数据
#define TP_NUM_MAX	5

struct TP_DATA{
	u8 TpNum;
	u16 TpX[TP_NUM_MAX];
	u16 TpY[TP_NUM_MAX];
};

void FT6336_Init(void);
void GT911_Init(void);
u8 TP_Scan(struct TP_DATA *gtData);

#endif
