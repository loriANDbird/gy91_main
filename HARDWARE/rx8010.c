/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：
    1. RX8010时钟芯片驱动
    2. 接口函数用时:  系统时钟48MHz
    RX8010_ReadTime():  用时约 xx uS
    RX8010_SetTime()    用时约 xx uS
    
更新记录：
    1. 2018.03.01 创建
    
******************************************************************************/
#include "rX8010.h" 
#include "Mcu_io.h"

#define RX8010_CLK_RCC       RCC_APB2Periph_GPIOC  
#define RX8010_CLK_PORT      GPIOC  
#define RX8010_CLK_PIN       GPIO_Pin_15  

#define RX8010_SDA_RCC       RCC_APB2Periph_GPIOC  
#define RX8010_SDA_PORT      GPIOC  
#define RX8010_SDA_PIN       GPIO_Pin_14

#define RX8010_IIC_CLK_1()   GPIO_SetBits(RX8010_CLK_PORT, RX8010_CLK_PIN)
#define RX8010_IIC_CLK_0()   GPIO_ResetBits(RX8010_CLK_PORT, RX8010_CLK_PIN)

#define RX8010_IIC_SDA_1()   GPIO_SetBits(RX8010_SDA_PORT, RX8010_SDA_PIN)
#define RX8010_IIC_SDA_0()   GPIO_ResetBits(RX8010_SDA_PORT, RX8010_SDA_PIN)

#define RX8010_Read_SDA_IN()  GPIO_ReadInputDataBit(RX8010_SDA_PORT,RX8010_SDA_PIN)

RX8010SJTIME g_SysTime;


void RX8010_SDA_OUT(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = RX8010_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RX8010_SDA_PORT, &GPIO_InitStructure);
}

void RX8010_SDA_IN(void){        
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = RX8010_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RX8010_SDA_PORT, &GPIO_InitStructure);
}



void RX8010_IIC_delay_us(int idly)
{
	int i,k;
	for(i=0;i<idly;i++){
        for(k=0;k<15;k++);  //168M 时钟   idly=1 约 1uS
	}
}

//控制I2C速度的延时
void RX8010_Delay(void)
{
	RX8010_IIC_delay_us(2);//akang
} 

//产生IIC起始信号
void RX8010_IIC_Start(void)
{
	RX8010_SDA_OUT();     //sda线输出
	RX8010_IIC_SDA_1();	  	  
	RX8010_IIC_CLK_1();

    RX8010_IIC_delay_us(2);  //akang
    
 	RX8010_IIC_SDA_0();//START:when CLK is high,DATA change form high to low
    
	RX8010_IIC_delay_us(2);  //akang
	RX8010_IIC_CLK_0();//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void RX8010_IIC_Stop(void)
{
	RX8010_SDA_OUT();//sda线输出
	RX8010_IIC_CLK_1();
    RX8010_IIC_delay_us(2);  //akang
	RX8010_IIC_SDA_0();//STOP:when CLK is high DATA change form low to high
	RX8010_IIC_delay_us(2);  //akang
	RX8010_IIC_SDA_1();//发送I2C总线结束信号  
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 RX8010_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	RX8010_SDA_IN();      //SDA设置为输入  
	RX8010_IIC_SDA_1();	   
	RX8010_IIC_CLK_1();
	RX8010_Delay();
	while(RX8010_Read_SDA_IN()){
		ucErrTime++;
		if(ucErrTime > 250)	{
			RX8010_IIC_Stop();			
			RX8010_IIC_delay_us(2);
			return 1;
		} 
		RX8010_IIC_delay_us(2);
	}
	RX8010_IIC_CLK_0();//时钟输出0 	   
	return 0;  
} 
////产生ACK应答
//void RX8010_IIC_Ack(void)
//{
//	RX8010_IIC_CLK_0();
//	RX8010_SDA_OUT();
//	RX8010_Delay();
//	RX8010_IIC_SDA_0();
//	RX8010_Delay();
//	RX8010_IIC_CLK_1();
//	RX8010_Delay();
//	RX8010_IIC_CLK_0();
//}

//单片机发出一个 ACK应答，又名 NACK
void RX8010_IIC_NAck(void)
{
	RX8010_IIC_CLK_0();
	RX8010_SDA_OUT();
	RX8010_Delay();
	RX8010_IIC_SDA_1();
	RX8010_Delay();
	RX8010_IIC_CLK_1();
	RX8010_Delay();
	RX8010_IIC_CLK_0();
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void RX8010_IIC_Send_Byte(unsigned char txd)
{                        
    u8 t;   
	RX8010_SDA_OUT(); 	    
    RX8010_IIC_CLK_0();//拉低时钟开始数据传输
	RX8010_Delay();
	for(t=0;t<8;t++){     
		if((txd&0x80) > 0){
			RX8010_IIC_SDA_1();
		}
		else{
			RX8010_IIC_SDA_0();
		}		
        txd<<=1;
		RX8010_IIC_CLK_1(); 
		RX8010_Delay();
		RX8010_IIC_CLK_0();	
		RX8010_Delay();
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
unsigned char RX8010_IIC_Read_Byte()
{
	u8 i,receive=0;
 	RX8010_SDA_IN();//SDA设置为输入	
    RX8010_IIC_delay_us(2);  //akang    
	for(i=0;i<8;i++ ){
		RX8010_IIC_CLK_0();	    	   
		RX8010_Delay();
		RX8010_IIC_CLK_1();
		receive<<=1;
        RX8010_IIC_delay_us(2);
		if(RX8010_Read_SDA_IN())receive++;
	}
    
    RX8010_IIC_CLK_0();
    
	//if (!ack)RX8010_IIC_NAck();//发送nACK
	//else RX8010_IIC_Ack(); //发送ACK
    
 	return receive;
}

//================================================================
unsigned char RX8010_WR_Reg(unsigned char regAddr ,unsigned char regDat)
{
	unsigned char ret;
	
	RX8010_IIC_Start();	
 	RX8010_IIC_Send_Byte(0x64);   	//发送写命令 0x64	 
	RX8010_IIC_Wait_Ack();
	RX8010_IIC_Send_Byte(regAddr);   	//发送高8位地址
	RX8010_IIC_Wait_Ack(); 	 										  		   

	RX8010_IIC_Send_Byte(regDat);  	//发数据
	ret = RX8010_IIC_Wait_Ack();    
	
    RX8010_IIC_Stop();					//产生一个停止条件
	
	return ret;
}

unsigned char RX8010_RD_Reg(unsigned char regAddr)
{
	unsigned char ret;
 
 	RX8010_IIC_Start();	
 	RX8010_IIC_Send_Byte(0x64);         //发送写命令 	 
	RX8010_IIC_Wait_Ack();
	
 	RX8010_IIC_Send_Byte(regAddr);   	//发送高8位地址
	RX8010_IIC_Wait_Ack(); 		
	
 	RX8010_IIC_Start();  	 	   
	RX8010_IIC_Send_Byte(0x65);   //发送读命令	0x65	   
	RX8010_IIC_Wait_Ack();	  
	
	ret = RX8010_IIC_Read_Byte(); //只读取一个字节
    
    RX8010_IIC_NAck();    //读取完成后，单片机需要发 一个NACK
    
    RX8010_IIC_Stop();//产生一个停止条件   
    
	return ret; 
}


//IIC接口初始化
void RX8010_Init(void)
{			       
    GPIO_InitTypeDef GPIO_InitStructure;     
	
	PWR_BackupAccessCmd(ENABLE);/* 允许修改RTC和后备寄存器*/
    RCC_LSEConfig(RCC_LSE_OFF); /* 关闭外部低速时钟,PC14+PC15可以用作普通IO*/
    BKP_TamperPinCmd(DISABLE);  /* 关闭入侵检测功能,PC13可以用作普通IO*/
	PWR_BackupAccessCmd(DISABLE);/* 禁止修改RTC和后备寄存器*/
	//BKP_ITConfig(DISABLE);       /* 禁止TAMPER 中断*/
	
    RCC_APB2PeriphClockCmd(RX8010_CLK_RCC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = RX8010_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RX8010_CLK_PORT, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RX8010_SDA_RCC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = RX8010_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RX8010_SDA_PORT, &GPIO_InitStructure);
	              
	RX8010_SDA_IN();        
//    RX8010_WR_Reg(0x17,0xD8);
//    RX8010_WR_Reg(0x30,0x00);
//    RX8010_WR_Reg(0x31,0x08);
//    RX8010_WR_Reg(0x32,0x00);
//    RX8010_WR_Reg(0x1d,0x04);   
    
    RX8010_WR_Reg(0x1E,0x00);    //清除标志位
//    RX8010_WR_Reg(0x1F,0x00);      
}

void RX8010_TimeRst(void)
{
    RX8010_WR_Reg(0x1E,0x00);    //清除标志位
    RX8010_IIC_delay_us(200);
    
    g_SysTime.sec = 1;
    g_SysTime.min = 0;
    g_SysTime.hour = 0;
    g_SysTime.week = 1;
    g_SysTime.day = 1;
    g_SysTime.month = 1;
    g_SysTime.year = 2020;        
    RX8010_SetTime(&g_SysTime);
}


//时间格式：秒，分，时，日，月，年  BCD码
void RX8010_SetTime(RX8010SJTIME* Time)
{
	unsigned char tmp[7];
    tmp[0] = Time->sec/10;
    tmp[0]<<=4;
    tmp[0]+=(Time->sec%10);
    
    tmp[1] = Time->min/10;
    tmp[1]<<=4;
    tmp[1]+=(Time->min%10);
    
    tmp[2] = Time->hour/10;
    tmp[2]<<=4;
    tmp[2]+=(Time->hour%10);
    
    tmp[3] = 1;//默认为周日
    
    tmp[4] = Time->day/10;
    tmp[4]<<=4;
    tmp[4]+=(Time->day%10);    
    
    tmp[5] = Time->month/10;
    tmp[5]<<=4;
    tmp[5]+=(Time->month%10);
    
    tmp[6] = (Time->year%100)/10;
    tmp[6]<<=4;
    tmp[6]+=((Time->year%100)%10);
    
    RX8010_WR_Reg(0x10,tmp[0]); //设置为00S
    RX8010_WR_Reg(0x11,tmp[1]); //设置为38分
    RX8010_WR_Reg(0x12,tmp[2]); //设置为14时
    RX8010_WR_Reg(0x13,tmp[3]); //设置为周四
    RX8010_WR_Reg(0x14,tmp[4]); //设置为日
    RX8010_WR_Reg(0x15,tmp[5]); //设置为月
    RX8010_WR_Reg(0x16,tmp[6]); //设置为年
    
}

//时间格式：秒，分，时，日，月，年  BCD码
void RX8010_ReadTime(RX8010SJTIME* Time)
{
    unsigned char time[7];
		char test;
             
    time[0] = RX8010_RD_Reg(0x10);
    time[1] = RX8010_RD_Reg(0x11);
    time[2] = RX8010_RD_Reg(0x12);
//    time[3] = RX8010_RD_Reg(0x13);  //星期
    time[4] = RX8010_RD_Reg(0x14);
    time[5] = RX8010_RD_Reg(0x15);
    time[6] = RX8010_RD_Reg(0x16);
		test = RX8010_RD_Reg(0x1E); 

    if(time[4] == 0){       //时间初始化
        RX8010_TimeRst();
        return;
    }
	if(time[5] == 0){
        RX8010_TimeRst();
        return;
    }
    if(time[6] == 0xFF){
        RX8010_TimeRst();
        return;
    }
    if(time[0] == 0xFF){
        RX8010_TimeRst();   
        return;
    }
		
	if((test&0x02) == 0x02){
		RX8010_TimeRst();   
		return;
	}
    
    Time->sec  = (time[0]>>4)*10+(time[0]&0xff0f);     
    Time->min  = (time[1]>>4)*10+(time[1]&0xff0f);     
    Time->hour = (time[2]>>4)*10+(time[2]&0xff0f);     
    
    Time->day  = (time[4]>>4)*10+(time[4]&0xff0f);     
    Time->month = (time[5]>>4)*10+(time[5]&0xff0f);      
    Time->year  = 2000+(time[6]>>4)*10+(time[6]&0xff0f);
	
	if((Time->sec >= 60)||(Time->min >= 60)||(Time->hour >= 24)||(Time->day > 31)||(Time->month > 12)){
		RX8010_TimeRst();   
		return;
	}
}

u8 MaxDay(u8 year,u8 month)
{
	if((month ==4)||(month ==6)||(month ==9)||(month ==11)){
		return 30;
	}
	else if(month == 2){
		if(year%4 == 0){
			return 29;
		}
		else{
			return 28;
		}
	}
	else{
		return 31;
	}
}

