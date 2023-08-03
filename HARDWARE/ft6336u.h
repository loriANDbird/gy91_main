#ifndef _FT6336U_H
#define _FT6336U_H

#include "stm32f10x.h"

//I2C��д����
//FT6336
#define TP_CMD_WR		((0x38<<1)|0x00)	//д����
#define TP_CMD_RD		((0x38<<1)|0x01)	//������
#define TP_CMD_X		0X03		//��X
#define TP_CMD_Y		0X05		//��Y

//GT911
//#define GT_CMD_WR		0X28        //д����
//#define GT_CMD_RD		0X29        //������

#define GT_CFG_VERSION	0x5A		//�����ļ��汾
  
//GT911 ���ּĴ�������
#define GT_CTRL_REG		0X8040       //GT911���ƼĴ���
#define GT_CFGS_REG		0X8047       //GT911������ʼ��ַ�Ĵ���
#define GT_CHECK_REG	0X80FF       //GT911У��ͼĴ���
#define GT_PID_REG		0X8140       //GT911��ƷID�Ĵ���

#define GT_GSTID_REG	0X814E       //GT911��ǰ��⵽�Ĵ������

//��������
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
