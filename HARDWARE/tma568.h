#ifndef __TMA568_H
#define __TMA568_H

#include "iic.h"

//IO��������	 
#define FT_RESET_H() 			(GPIOE->BSRR =  GPIO_Pin_3)		//FT5206��λ����
#define FT_RESET_L() 			(GPIOE->BRR =  GPIO_Pin_3)			//FT5206��λ����
#define FT_INT    				//((GPIOC->IDR & GPIO_Pin_7)!=0)		//FT5206�ж�����

#define TP_INT_IS_H	            ((GPIOE->IDR & GPIO_Pin_4)!=0)
#define TP_INT_IS_L	            ((GPIOE->IDR & GPIO_Pin_4)==0)

//I2C��д����	
#define FT_CMD_WR 				0X48    	//д����
#define FT_CMD_RD 				0X49		//������
  
//CYTMA568 ���ּĴ������� 
#define FT_DEVIDE_MODE 			0x00   		//FT5206ģʽ���ƼĴ���

#define  INT_PULSE_DATA         0x1e        //�ж�����ʱ�䣨ms��
#define  INTERRUPT_PIN_OVERRIDE 0x2e        //�ж�����

#define FT_REG_NUM_FINGER       0x03		//����״̬�Ĵ���

#define FT_TP1_REG 				0X03	  	//��һ�����������ݵ�ַ
#define FT_TP2_REG 				0X09		//�ڶ������������ݵ�ַ
#define FT_TP3_REG 				0X0F		//���������������ݵ�ַ
#define FT_TP4_REG 				0X15		//���ĸ����������ݵ�ַ
#define FT_TP5_REG 				0X1B		//��������������ݵ�ַ  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//�汾
#define FT_ID_G_MODE 			0xA4   		//FT5206�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP			0x80   		//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE	0x88   		//����״̬�������üĴ���

#define TP_NUM_MAX				(5)			// ���5�㴥��

// �������꼰�������
extern u16 TpX[TP_NUM_MAX];
extern u16 TpY[TP_NUM_MAX];
extern s16 TpNum;

u8 CYTMA568_WR_Reg(u16 reg,u8 *buf,u8 len);
void CYTMA568_RD_Reg(u16 reg,u8 *buf,u8 len);
u8 CYTMA568_Init(void);
u8 CYTMA568_Scan(void);

#endif

