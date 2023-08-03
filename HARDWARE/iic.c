#include "iic.h"
#include "delay.h"

/*************************************************
Function: I2C_Wait
Description: delay
Input: no
Output: no
More:no
*************************************************/
void I2C_Wait(void)//需要填入适当的延时函数
{
//    DelayUs(2);
    __nop();__nop();__nop();__nop();__nop();
    __nop();__nop();__nop();__nop();__nop();
    __nop();__nop();__nop();__nop();__nop();
    __nop();__nop();__nop();__nop();__nop();
    __nop();__nop();__nop();__nop();__nop();
    __nop();__nop();__nop();__nop();__nop();
    __nop();__nop();__nop();
}

/*****************************************************************

	函数功能：	初始化I2C
	输入：		无
	返回：		无
	更新日期:	2015-7-18
	备注：		无

******************************************************************/
void I2C_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	// 默认为高
	I2C_SCL_H();
	I2C_SDA_H();
	// PB10 -> SCL PB11 -> SDA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*************************************************
Function: I2C_Start
Description: signal of start
Input: no
Output: no
More:no
*************************************************/
void I2C_Start(void)   
{
	I2C_SDA_H();
	I2C_SCL_H();
	I2C_Wait();
	I2C_SDA_L();
	I2C_Wait();
	I2C_SCL_L();
}

/*************************************************
Function: I2C_Stop
Description: signal of stop
Input: no
Output: no
More:no
*************************************************/
void I2C_Stop(void)    
{
	I2C_SDA_L();
	I2C_Wait();
	I2C_SCL_H();
	I2C_Wait();
	I2C_SDA_H();
	I2C_Wait();
}

/*************************************************
Function: I2C_SendAck
Description: send ack to slave
Input: signal of ack
Output: no
More:no
*************************************************/
void I2C_SendAck(unsigned char ack) 
{	
	if(ack){
		I2C_SDA_H();
	}
	else{
		I2C_SDA_L();
	}
	I2C_Wait();
	I2C_SCL_H();
	I2C_Wait();
	I2C_SCL_L();
	I2C_Wait();
	I2C_SDA_H();
	I2C_Wait();
}

/*************************************************
Function: I2C_SendByte
Description: send data  to I2C register
Input: unsigned char of data
Output: return ack  0:receive ack  1:no ack
More:no
*************************************************/
unsigned char I2C_SendByte(unsigned char bytedata) 
{
	unsigned char i;
	unsigned char ack;
	I2C_Wait();
	for(i=0;i<8;i++){
		if(bytedata & 0x80)
		  	I2C_SDA_H();
		else
		  	I2C_SDA_L();
		bytedata <<= 1;
		I2C_Wait();
		I2C_SCL_H();
		I2C_Wait();
		I2C_SCL_L();
		I2C_Wait();
	}
	I2C_SDA_H();
	I2C_Wait();
	I2C_SCL_H();
	I2C_Wait();
	ack = I2C_SDA_IS_H;    
	I2C_SCL_L();
	I2C_Wait();

	return ack;
}

/******************************************************
Function: I2C_ReceiveByte
Description: receive data  from I2C register
Input: no
Output: data 
More:no
********************************************************/
unsigned char I2C_ReceiveByte(void)  
{
	unsigned char i;
	unsigned char bytedata = 0;
	I2C_Wait();
	for(i=0;i<8;i++){
		I2C_SCL_H();
		I2C_Wait();
		bytedata <<= 1;
		if(I2C_SDA_IS_H){
			bytedata |= 0x01;
		}
		I2C_SCL_L();
		I2C_Wait();
	}
	return bytedata;
}

/******************************************************
Function: I2C_ByteWrite
Description: write the data to the address 
Input: device  0xC0  write to  OV6620
               0XC1  read from OV6620
               0x42  write to  OV7620
               0x43  read from OV7620
Output: no
More:no
********************************************************/
signed char I2C_ByteWrite(unsigned char device,unsigned char address,unsigned char bytedata)
{     
	unsigned char ack;
	if(!I2C_SDA_IS_H) return -1;
	I2C_Start();
	ack = I2C_SendByte(device);
	if(ack){
		I2C_Stop();
		return -2;
	}	 
	ack = I2C_SendByte(address);
	if(ack){
		I2C_Stop();
		return -3;
	}	 
	ack = I2C_SendByte(bytedata);
	I2C_Stop();
	if(ack){
		return -4;
	}
	return 0;	 
}

