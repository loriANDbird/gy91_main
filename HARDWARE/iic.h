#ifndef __IIC_H
#define __IIC_H

#include "stm32f10x.h"

#define I2C_SCL_H()		GPIO_SetBits(GPIOC, GPIO_Pin_4)
#define I2C_SCL_L()		GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define I2C_SDA_H()		GPIO_SetBits(GPIOC, GPIO_Pin_5)
#define I2C_SDA_L()		GPIO_ResetBits(GPIOC, GPIO_Pin_5)
#define I2C_SDA_IS_H	((GPIOC->IDR & GPIO_Pin_5)!=0)
#define I2C_SDA_IS_L	((GPIOC->IDR & GPIO_Pin_5)==0)


void I2C_Config(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendAck(unsigned char ack);
unsigned char I2C_SendByte(unsigned char bytedata);
unsigned char I2C_ReceiveByte(void);
signed char  I2C_ByteWrite(unsigned char device,unsigned char address,unsigned char bytedata);
#endif


