#include "ct1711.h"

#define STA_IDLE		0x0
#define STA_START		0x01
#define STA_CONVERT		0x02
#define STA_DEGREE		0x03

static u8 CT1711ReadSta;//空闲->开始->转换->解码->空闲
static u16 CT1711ReadCnt;//转换时间计数，20ms自加
static u8 CT1711CvrDoneFlag;//转换完成标志，1：转换完成

//48M主频
void SWDelay_ms(int nms)
{
	int idly,i;
	for(i=0;i<nms;i++){
		for(idly = 0;idly < 10000;idly++);
	}
}
void SWDelay_us(int nus)
{
	int idly,i;
	for(i=0;i<nus;i++){
		for(idly = 0;idly < 8;idly++);
	}
}

//复位SWire
void SWire_Rst(void)	   
{                 
	SWire_IO_OUT(); 	//SET IO OUTPUT
    SWire_DQ_OUT=0; 	//拉低DQ
    SWDelay_us(600);    //拉低750us
    SWire_DQ_OUT=1; 	//DQ=1
//	SWDelay_us(15);     	//15US
}
//等待SWire的回应
//返回1:未检测到SWire的存在
//返回0:存在
u8 SWire_Check(void) 	   
{   
	u8 retry=0;
	SWire_IO_IN();	//SET IO INPUT	 
    while(SWire_DQ_IN&&retry<200)
	{
		retry++;
		SWDelay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while(!SWire_DQ_IN&&retry<240)
	{
		retry++;
		SWDelay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
//从SWire读取一个位
//返回值：1/0
u8 SWire_Read_Bit(void) 	 
{
    u8 data;
	SWire_IO_OUT();	//SET IO OUTPUT
    SWire_DQ_OUT=0; 
	SWDelay_us(2);
    SWire_DQ_OUT=1;
	SWire_IO_IN();	//SET IO INPUT
	SWDelay_us(20);
	if(SWire_DQ_IN)data=1;
    else data=0;
    SWDelay_us(30);
    return data;
}
//从SWire读取一个字节
//返回值：读到的数据
u8 SWire_Read_Byte(void)
{
    u8 i,byte=0;
	
	for(i=8;i>0;i--){
		byte <<= 1;
        byte |= SWire_Read_Bit();
    }
    return byte;
}
//写一个字节到SWire
//dat：要写入的字节
void SWire_Write_Byte(u8 data)     
{
    u8 i;
    u8 byte;
	SWire_IO_OUT();	//SET IO OUTPUT;
    for(i=0;i<8;i++){
		byte=data&0x01;
        data=data>>1;
        if(byte){
            SWire_DQ_OUT=0;	//Write 1
            SWDelay_us(2);
            SWire_DQ_OUT=1;
            SWDelay_us(60);
        }else{
            SWire_DQ_OUT=0;	//Write 0
            SWDelay_us(60);
            SWire_DQ_OUT=1;
            SWDelay_us(2);
        }
    }
}

//***********************DS18B20**********************//
//开始温度转换
u8 DS18B20_Start(void) 
{
    SWire_Rst();
	if(SWire_Check())
		return 1;
    SWire_Write_Byte(0xcc);	// skip rom
    SWire_Write_Byte(0x44);	// convert
	return 0;
} 
//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
float DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
	short temOrig;
	float temRes;
    DS18B20_Start();//ds1820 start convert
    SWire_Rst();
    SWire_Check();	 
    SWire_Write_Byte(0xcc);	// skip rom
    SWire_Write_Byte(0xbe);	// convert	    
    TL=SWire_Read_Byte(); 	// LSB   
    TH=SWire_Read_Byte(); 	// MSB  
	    	  
    if(TH>7){
        TH=~TH;
        TL=~TL; 
        temp=0;			//温度为负  
    }else temp=1;		//温度为正	  	  
    temOrig=TH; 		//获得高八位
    temOrig<<=8;    
    temOrig+=TL;		//获得底八位
    temRes=(float)temOrig*0.625f/10.0f;	//转换     
	if(temp)return temRes; 		//返回温度值
	else return -temRes;    
}
//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
u8 DS18B20_Init(void)
{
	u8 val;
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(SWire_RCC, ENABLE);	//使能PORTG口时钟 
	
 	GPIO_InitStructure.GPIO_Pin = SWire_PIN;	//PORTG.11 推挽输出
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SWire_PORT, &GPIO_InitStructure);

 	GPIO_SetBits(SWire_PORT,SWire_PIN);    //输出1
	
	val = DS18B20_Start();
	DS18B20_Get_Temp();
	
	return val;
}

//***********************CT1711**********************//
//开始温度转换
void CT1711_Start(void) 
{
	if(CT1711ReadSta == STA_IDLE){
		CT1711ReadSta = STA_START;
		SWire_Rst();
		CT1711ReadSta = STA_CONVERT;
		CT1711ReadCnt = 0;
		CT1711CvrDoneFlag = 0;
	}
}

//转换等待,CT1711开始后需要150ms温度转换时间，转换完成才能读取数据
//本函数在systic中断调用
void CT1711_CvrWait(void)
{
	//转换时间
	u8 cTim = 30;//20*20ms后读取数据
	if(CT1711ReadSta != STA_IDLE){
		CT1711ReadCnt += 1;
		
		if(CT1711ReadCnt > 50){//超时重置
			CT1711ReadSta = STA_IDLE;
			CT1711ReadCnt = 0;
			CT1711CvrDoneFlag = 0;
		}else if(CT1711ReadCnt > cTim){//转换完成
			CT1711ReadSta = STA_DEGREE;
		}else{//转换时间
			CT1711ReadSta = STA_CONVERT;
		}
		
		if(CT1711ReadCnt == cTim){//转换完成标志
			CT1711CvrDoneFlag = 1;
		}
	}
}
//从CT1711得到温度值
//精度：0.1C
//返回值：温度值
float CT1711_Temp_Degree(void)
{
	u8 bit_cc0,bit_cc1,bit_sign;
    u8 byte_L,byte_H;
	u16 temp_val;
	int temp_int;
	float temp_res;
	static float temp_last = 0.0f;;
	
	if(CT1711CvrDoneFlag == 1){
		CT1711CvrDoneFlag = 0;
		
		bit_cc0 = SWire_Read_Bit();
		bit_cc1 = SWire_Read_Bit();
		bit_cc0 = bit_cc0 & 0x01; 
		bit_cc1 = bit_cc1 & 0x01;
		
		if((bit_cc0 == 0x00) && (bit_cc1 == 0x00)){
			bit_sign = SWire_Read_Bit();
			
			byte_H = SWire_Read_Byte();
			byte_L = SWire_Read_Byte();
			temp_val = (byte_H<<8) + byte_L;
			
			if(bit_sign == 0x01){
				temp_val = ~temp_val;
				temp_val &= 0xffff;
				temp_val += 1;
				temp_res = -3.90625f * temp_val / 1000.00f;
			}else{
				temp_res = 3.90625f * temp_val / 1000.00f;
			}
			temp_int = temp_res*10.0f + 0.5f;//保留1位小数取整
			temp_res = (float)temp_int/10.0f;
			if(temp_res < -50.0f)temp_res = -50.0f;
			if(temp_res > 150.0f)temp_res = 150.0f;
			temp_last = temp_res;
			return temp_res;
		}
	}else{
		return temp_last;
	}
	return temp_last;
}
//从CT1711得到温度值 
float CT1711_Get_Temp(void)
{
	float tempRes;
	CT1711_Start();
	tempRes = CT1711_Temp_Degree();
	return tempRes;
}

//初始化CT1711IO
void CT1711_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(SWire_RCC, ENABLE);	//使能口时钟 
	
 	GPIO_InitStructure.GPIO_Pin = SWire_PIN;	//推挽输出
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SWire_PORT, &GPIO_InitStructure);
	
 	GPIO_SetBits(SWire_PORT,SWire_PIN);   //输出1
}




