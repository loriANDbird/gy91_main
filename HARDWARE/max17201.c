/******************************************************************************
创建者：代云飞
创建日期：2018.08.13
功能描述：
	1.百分比值，电池状态
更新记录：
    1. 2018.08.13  V1.0 初次创建
*******************************************************************************/
#include "stm32f10x.h"
//#include "delay.h"
#include "max17201.h"
//#include "File.h"


//IO操作函数
#define MAX17201_SCL_1()  GPIO_SetBits(MAX17201_SCL_PORT,MAX17201_SCL_PIN)
#define MAX17201_SCL_0()  GPIO_ResetBits(MAX17201_SCL_PORT,MAX17201_SCL_PIN)


//IO操作函数	 
#define MAX17201_SDA_1()    GPIO_SetBits(MAX17201_Dat_PORT,MAX17201_Dat_PIN)
#define MAX17201_SDA_0()    GPIO_ResetBits(MAX17201_Dat_PORT,MAX17201_Dat_PIN)
#define MAX17201_SDAIN()   GPIO_ReadInputDataBit(MAX17201_Dat_PORT,MAX17201_Dat_PIN)


NPACKCFG_REGISTER_TYPE_STRU  nPACKCFG_REGISTER_TYPE_STRU;
NPACKCFG_REGISTER_TYPE_UNION npackcfg_register_union;
nNVCfg0_REGISTER_TYPE_UNION  nnvcfg0_register_union;
nNVCfg1_REGISTER_TYPE_UNION  nnvcfg1_register_union;
nNVCfg2_REGISTER_TYPE_UNION  nnvcfg2_register_union;   

CommStat_REGISTER_TYPE_UNION commstat_register_union;
MAX17201_REGISTER_VALUE_STRU max17201_register_value_stru;
STATUS_REGISTER_TYPE_UNION   status_register_union;
rFSTAT_REGISTER_TYPE_UNION   rFstat_register_union;
nVEmpty_REGISTER_TYPE_UNION  nVEmpty_register_union;


u8  g_MaxState = 0;
u16 g_MaxMaxDianLiang = 0; 
u16 g_MaxDianLiang = 0;
u16 g_MaxDianya=0;

//48Mhz
static void delay_us(unsigned int timer)
{ 
	unsigned short int i; 
	while(timer--)
	{
		i=6;
		while(i--); 
	} 
}
static void delay_ms(unsigned int timer)
{ 
	while(timer--)
	{
		delay_us(1000);
	} 
}


//初始化IIC
void MAX17201_PORT_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(	MAX17201_Dat_RCC | MAX17201_SCL_RCC, ENABLE );	//使能时钟 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = MAX17201_Dat_PIN;
	GPIO_Init(MAX17201_Dat_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = MAX17201_SCL_PIN;
	GPIO_Init(MAX17201_SCL_PORT, &GPIO_InitStructure); 

 	GPIO_SetBits(MAX17201_Dat_PORT,MAX17201_Dat_PIN ); // 
 	GPIO_SetBits(MAX17201_SCL_PORT,MAX17201_SCL_PIN ); // 
}


void Max17201_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = MAX17201_Dat_PIN;
	GPIO_Init(MAX17201_Dat_PORT, &GPIO_InitStructure); 
}

void MAX17201_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = MAX17201_Dat_PIN;
	GPIO_Init(MAX17201_Dat_PORT, &GPIO_InitStructure); 
}


//产生IIC起始信号
void MAX17201_IIC_Start(void)
{
	MAX17201_SDA_OUT();     //sda线输出
	MAX17201_SDA_1();	  	  
	MAX17201_SCL_1();
	delay_us(4);
 	MAX17201_SDA_0();//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	MAX17201_SCL_0();//钳住I2C总线，准备发送或接收数据 
}

//产生IIC停止信号
void MAX17201_IIC_Stop(void)
{
	MAX17201_SDA_OUT();//sda线输出
	MAX17201_SCL_0();
	MAX17201_SDA_0();//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	MAX17201_SCL_1(); 
	MAX17201_SDA_1();//发送I2C总线结束信号
	delay_us(4);							   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 MAX17201_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	Max17201_SDA_IN();      //SDA设置为输入  
	MAX17201_SDA_1();delay_us(1);	   
	MAX17201_SCL_1();delay_us(1);	 
	while(MAX17201_SDAIN()) {
		ucErrTime++;
		if(ucErrTime>250) {
			MAX17201_IIC_Stop();
			return 1;
		}
	}
	MAX17201_SCL_0();//时钟输出0
	return 0;  
}

//产生ACK应答
void MAX17201_IIC_Ack(void)
{
	MAX17201_SCL_0();
	MAX17201_SDA_OUT();
	MAX17201_SDA_0();
	delay_us(2);
	MAX17201_SCL_1();
	delay_us(2);
	MAX17201_SCL_0();
}

//不产生ACK应答
void MAX17201_IIC_NAck(void)
{
	MAX17201_SCL_0();
	MAX17201_SDA_OUT();
	MAX17201_SDA_1();
	delay_us(2);
	MAX17201_SCL_1();
	delay_us(2);
	MAX17201_SCL_0();
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void MAX17201_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	MAX17201_SDA_OUT(); 	    
    MAX17201_SCL_0();//拉低时钟开始数据传输
    for(t=0;t<8;t++) {
		if((txd&0x80)>>7)
			MAX17201_SDA_1();
		else
			MAX17201_SDA_0();
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		MAX17201_SCL_1();
		delay_us(2); 
		MAX17201_SCL_0();	
		delay_us(2);
    }	 
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 MAX17201_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	Max17201_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ ) {
        MAX17201_SCL_0(); 
        delay_us(2);
		MAX17201_SCL_1();
        receive<<=1;
        if(MAX17201_SDAIN())receive++;   
		delay_us(2); 
    }					 
    if (!ack)
        MAX17201_IIC_NAck();//发送nACK
    else
        MAX17201_IIC_Ack(); //发送ACK   
    return receive;
}


//在指定地址读出一个字节
//i2c_address:开始读数的地址  
//adc_command  :命令
//dat:  存储读取的数据
int32_t i2c_read_byte_data(uint8_t i2c_address, uint8_t adc_command, uint8_t *dat)
{	
	int32_t ack = 0;

	MAX17201_IIC_Start();  
	MAX17201_IIC_Send_Byte(i2c_address);   //发送器件地址写 	 
	ack	|= MAX17201_IIC_Wait_Ack(); 
	MAX17201_IIC_Send_Byte(adc_command);   //发送低地址
	ack	|= MAX17201_IIC_Wait_Ack();	    
	MAX17201_IIC_Start();  	 	   
	MAX17201_IIC_Send_Byte(i2c_address+1); //进入接收模式			   
	ack	|= MAX17201_IIC_Wait_Ack();	 
    *dat = MAX17201_IIC_Read_Byte(0);		   
    MAX17201_IIC_Stop();//产生一个停止条件	
    return ack;			
}

//在指定地址读出一个16位的数据
//i2c_address:开始读数的地址  
//adc_command  :命令
//dat:  存储读取的数据
//返回数据：ack         0 正常返回Ack      1 未返回ACK
int32_t i2c_read_word_data(uint8_t i2c_address, uint8_t adc_command, uint16_t *dat)
{ 
	int32_t ack = 0;
	
    uint16_t temp = 0;
    MAX17201_IIC_Start();  
	MAX17201_IIC_Send_Byte(i2c_address);   //发送器件地址写 	 
	ack	|= MAX17201_IIC_Wait_Ack(); 
    MAX17201_IIC_Send_Byte(adc_command);   //发送低地址
	ack	|= MAX17201_IIC_Wait_Ack();	    
	MAX17201_IIC_Start();  	 	   
	MAX17201_IIC_Send_Byte(i2c_address+1); //进入接收模式			   
	ack	|= MAX17201_IIC_Wait_Ack();	 
    temp  = MAX17201_IIC_Read_Byte(1);  //发送ACK     低位
	temp |= MAX17201_IIC_Read_Byte(0)<<8; //发送NACK  高位
    MAX17201_IIC_Stop();//产生一个停止条件	
	*dat = temp;
    return ack;
}


//指定地址写入一个字节
//i2c_address  :    
//adc_command:要写入的数据
//code:要写入的数据
// 时序：S ADDR ACK COMMAND ACK DATA ACK P
int32_t i2c_write_byte_data(uint8_t i2c_address, uint8_t adc_command, uint8_t code)
{
	int32_t ack = 0;

	MAX17201_IIC_Start();  
	MAX17201_IIC_Send_Byte(i2c_address);   //发送器件地址
	ack	|= MAX17201_IIC_Wait_Ack();	   
	MAX17201_IIC_Send_Byte(adc_command);   //命令
	ack	|= MAX17201_IIC_Wait_Ack(); 	 										  		   
	MAX17201_IIC_Send_Byte(code);     //发送数据						   
	ack	|= MAX17201_IIC_Wait_Ack();  		    	   
	MAX17201_IIC_Stop();//产生一个停止条件 
    return ack;	
}

//指定地址写入一个16位数
//i2c_address  :    
//adc_command:要写入的数据
//code:要写入的数据
int32_t i2c_write_word_data(uint8_t i2c_address, uint8_t adc_command, uint16_t code)
{
	int32_t ack = 0;

	MAX17201_IIC_Start();  
	MAX17201_IIC_Send_Byte(i2c_address);   //发送器件地址
	ack	|= MAX17201_IIC_Wait_Ack();	   
	MAX17201_IIC_Send_Byte(adc_command);   //命令
	ack	|= MAX17201_IIC_Wait_Ack(); 	 										  		   
	MAX17201_IIC_Send_Byte(code);     //发送低8位						   
	ack	|= MAX17201_IIC_Wait_Ack(); 	
	MAX17201_IIC_Send_Byte(code>>8);     //发送高8位						   
	ack	|= MAX17201_IIC_Wait_Ack();  		    	   
	MAX17201_IIC_Stop();//产生一个停止条件 
	return ack;	
}

// Write an 8-bit code to the MAX17201.
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
int8_t MAX17201_write(uint8_t i2c_address, uint8_t adc_command, uint8_t code)
{
  int32_t ack = 0;
 
  ack = i2c_write_byte_data(i2c_address, adc_command, code);
  return(ack);
}

// Write a 16-bit code to the MAX17201.
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
int8_t MAX17201_write_16_bits(uint8_t i2c_address, uint8_t adc_command, uint16_t code)
{
  int8_t ack;
 
  ack = i2c_write_word_data(i2c_address, adc_command, code);
  return(ack);
}
 
// Reads an 8-bit adc_code from MAX17201
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
int8_t MAX17201_read(uint8_t i2c_address, uint8_t adc_command, uint8_t *adc_code)
{
  int32_t ack;
 
  ack = i2c_read_byte_data(i2c_address, adc_command, adc_code);
  return(ack);
}

// Reads a 16-bit adc_code from MAX17201
// The function returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
int8_t MAX17201_read_16_bits(uint8_t i2c_address, uint8_t adc_command, uint16_t *adc_code)
{
  int32_t ack;
 
  ack = i2c_read_word_data(i2c_address, adc_command, adc_code);
  return(ack);
}

void Measurements_VBATT_Pin_Config(void)
{
    uint16_t test = 0;
    npackcfg_register_union.npackcfg_register_bits.BtEn = 1;
    npackcfg_register_union.npackcfg_register_bits.CxEn = 0;
    npackcfg_register_union.npackcfg_register_bits.ChEn = 0;
    npackcfg_register_union.npackcfg_register_bits.TdEn = 1;
    npackcfg_register_union.npackcfg_register_bits.A1En = 0;
    npackcfg_register_union.npackcfg_register_bits.A2En = 0;
    npackcfg_register_union.npackcfg_register_bits.FGT  = 0;
    npackcfg_register_union.npackcfg_register_bits.NCELLS = BatteryC;  //3节电池

	
    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, PackCfg_register_addr,npackcfg_register_union.npackcfg_register_short);
    delay_ms(50);
    MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1,PackCfg_register_addr,&test);
}

/* 1. Write register 0x062 = 0x0059
   2. Write register 0x063 = 0x00C4
   3. Write register 0x080 = 0x0000
   4. Write register 0x060 = 0x000F
   5. Wait 150ms (NV memory recall and RAM initialization)
   6. Write register 0x0BA to clear bit 0x8000 (Disable hibernate mode.
   It should be re-enabled at
   the end of factory production.) */
void MAX17201_Reset(void)
{
    uint8_t  ack;
    
    ack =   MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, 0x62,0x0059);
    ack |=  MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, 0x63,0x00C4);
    ack |=  MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, 0x80,0x0000);
    ack |=  MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, 0x60,0x000F);
}

/* Fuel Gauge Reset [0001h to Address 0BBh]
       The fuel gauge reset command resets operation of the
       ICs without restoring nonvolatile memory values into
       shadow RAM. This command allows different configurations to be tested
       without using one of the limited number
       of nonvolatile memory writes. */
void Fuel_Gauge_Reset(void)
{
    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, 0xbb,0x0001);
}

/* Current Register = Current ADC Reading × (CGainRegister/0400h) + COffRegister */
void MAX17201_Current_Adjustment(uint16_t Cgain_dat)
{
	MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, CGain_register_addr,Cgain_dat);
}
 

void nNVCfg0_config(void)
{
    uint16_t test = 0;
	
    nnvcfg0_register_union.nNVCfg0_register_bits.enLCfg = 1;
    nnvcfg0_register_union.nNVCfg0_register_bits.enCG = 1;
    nnvcfg0_register_union.nNVCfg0_register_bits.enHCfg = 1;
    nnvcfg0_register_union.nNVCfg0_register_bits.enVE = 1;
    nnvcfg0_register_union.nNVCfg0_register_bits.enICT = 1;
	
    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, nNVCfg0_register_addr,nnvcfg0_register_union.nNVCfg0_register_short);
	delay_ms(50);
    MAX17201_read_16_bits(MAX17201_I2C_ADDRESS2,nNVCfg0_register_addr,&test); 
}

void nNVCfg1_config(void)
{
    uint16_t test = 0;
	
    nnvcfg1_register_union.nNVCfg1_register_bits.enFTH = 1;
    nnvcfg1_register_union.nNVCfg1_register_bits.enAT = 1;
    nnvcfg1_register_union.nNVCfg1_register_bits.enCTE = 1;

    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, nNVCfg1_register_addr,nnvcfg1_register_union.nNVCfg1_register_short);
    delay_ms(50);
    MAX17201_read_16_bits(MAX17201_I2C_ADDRESS2,nNVCfg1_register_addr,&test); 
}

void nNVCfg2_config(void)
{
    uint16_t test = 0;
	
    nnvcfg2_register_union.nNVCfg2_register_short = 0;
    nnvcfg2_register_union.nNVCfg2_register_bits.enT = 1;
    nnvcfg2_register_union.nNVCfg2_register_bits.enSOC = 1;
    nnvcfg2_register_union.nNVCfg2_register_bits.enMMT = 1;
    nnvcfg2_register_union.nNVCfg2_register_bits.enMMV = 1;
    nnvcfg2_register_union.nNVCfg2_register_bits.enMMC = 1;
    nnvcfg2_register_union.nNVCfg2_register_bits.enVT = 1;
    nnvcfg2_register_union.nNVCfg2_register_bits.enIAvg = 1;
    nnvcfg2_register_union.nNVCfg2_register_bits.CYCLESpSAVE = 0xa;

	MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, nNVCfg2_register_addr,nnvcfg2_register_union.nNVCfg2_register_short);
    delay_ms(50);
    MAX17201_read_16_bits(MAX17201_I2C_ADDRESS2,nNVCfg2_register_addr,&test); 
}

/* nVEmpty Register (19Eh)
   Register Type: Special
   Nonvolatile Restore: VEmpty (03Ah) if nNVCfg0.enVE is set.
   Alternate Initial Value: 0xA561 (VE=3.3V, VR=3.88V)
   The nVempty register sets thresholds related to empty
   detection during operation. Figure 23 shows the register
   format.
   VE: Empty Voltage. Sets the voltage level for detecting empty.
   A 10mV resolution gives a 0 to 5.11V range. This value is written to
   3.3V after reset if nonvolatile backup is disabled.
   VR: Recovery Voltage. Sets the voltage level for clearing
   empty detection. Once the cell voltage rises above this
   point, empty voltage detection is reenabled. A 40mV resolution gives
   a 0 to 5.08V range. This value is written to
   3.88V after reset if nonvolatile backup is disabled. */
void nVEmpty_config(void) //设计空电量检测电压
{
    nVEmpty_register_union.nVEmpty_register_bits.VE = nVEmpty_VE*(1000/10);
    nVEmpty_register_union.nVEmpty_register_bits.VR = nVEmpty_VR*(1000/40);
    
    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, nVEmpty_register_addr,nVEmpty_register_union.nVEmpty_register_short);
    delay_ms(5);  		//500ms	  - 100
}
 
 
 
/* Determining the Number of Remaining Updates
   The configuration memory can only be updated 7 times
   by the user (First update occurs during manufacturing
   test). The number of remaining updates can be calculated
   using the following procedure:
   1. Write 0xE2FA to the Command register (060h).
   2. Wait tRECALL.
   3. Read memory address 1EDh.
   4. Decode address 1EDh data as shown in Table 21.
   
   Each block write has redundant indicator flags for reliability. Logically
   OR the upper and lower bytes together then
   count the number of 1s determine how many updates
   have already been used. The first update occurs in manufacturing test
   prior to shipping to the user. */
unsigned char NV_remain_num(void)
{
	unsigned char ack = 0,i = 0;
	uint16_t dat_temp;
	unsigned char dat_H,dat_L;
	unsigned char remain_cnt;

//    1. Write 0xE2FA to the Command register (060h).
	ack = MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, Command_register_addr,0xE2FA);

//    2. Wait tRECALL.
	delay_ms(20);
	
//    3. Read memory address 1EDh.
	ack |= MAX17201_read_16_bits(MAX17201_I2C_ADDRESS2, 0xED,&dat_temp); 

//    4. Decode address 1EDh data as shown in Table 21.
	if(ack == 0) {
		dat_H = dat_temp>>8;
		dat_L = dat_temp;
		dat_H  = dat_H|dat_L;
		for(i=0;i<8;i++) {
			if(((dat_H<<i)&0x80) == 0x80) {
				remain_cnt = i;
				return remain_cnt;
			} 
		}
	 }
	return 0;
}

/* 0x180 = 0x0000	//nXTable0 Register
   0x180 = 0x0000	//nXTable0 Register
   0x181 = 0x0000	//nXTable1 Register
   0x182 = 0x0000	//nXTable2 Register
   0x183 = 0x0000	//nXTable3 Register
   0x184 = 0x0000	//nXTable4 Register
   0x185 = 0x0000	//nXTable5 Register
   0x186 = 0x0000	//nXTable6 Register
   0x187 = 0x0000	//nXTable7 Register
   0x188 = 0x0000	//nXTable8 Register
   0x189 = 0x0000	//nXTable9 Register
   0x18A = 0x0000	//nXTable10 Register
   0x18B = 0x0000	//nXTable11 Register
   0x18C = 0x0000	//nUser18C Register
   0x18D = 0x0000	//nUser18D Register
   0x18E = 0x0000	//nODSCTh Register
   0x18F = 0x0000	//nODSCCfg Register
   0x190 = 0x0000	//nOCVTable0 Register
   0x191 = 0x0000	//nOCVTable1 Register
   0x192 = 0x0000	//nOCVTable2 Register
   0x193 = 0x0000	//nOCVTable3 Register
   0x194 = 0x0000	//nOCVTable4 Register
   0x195 = 0x0000	//nOCVTable5 Register
   0x196 = 0x0000	//nOCVTable6 Register
   0x197 = 0x0000	//nOCVTable7 Register
   0x198 = 0x0000	//nOCVTable8 Register
   0x199 = 0x0000	//nOCVTable9 Register
   0x19A = 0x0000	//nOCVTable10 Register
   0x19B = 0x0000	//nOCVTable11 Register
   0x19C = 0x0000	//nIChgTerm Register
   0x19D = 0x0000	//nFilterCfg Register
   0x19E = 0x0000	//nVEmpty Register
   0x19F = 0x2602	//nLearnCfg Register
   0x1A0 = 0x3C00	//nQRTable00 Register
   0x1A1 = 0x1B80	//nQRTable10 Register
   0x1A2 = 0x0B04	//nQRTable20 Register
   0x1A3 = 0x0885	//nQRTable30 Register
   0x1A4 = 0x0000	//nCycles Register
   0x1A5 = 0x0910	//nFullCapNom Register
   0x1A6 = 0x1070	//nRComp0 Register
   0x1A7 = 0x263D	//nTempCo Register
   0x1A8 = 0xF830	//nIAvgEmpty Register
   0x1A9 = 0x07D0	//nFullCapRep Register
   0x1AA = 0x0000	//nVoltTemp Register
   0x1AB = 0x807F	//nMaxMinCurr Register
   0x1AC = 0x00FF	//nMaxMinVolt Register
   0x1AD = 0x807F	//nMaxMinTemp Register
   0x1AE = 0x0000	//nSOC Register
   0x1AF = 0x0000	//nTimerH Register
   0x1B0 = 0x0000	//nConfig Register
   0x1B1 = 0x0204	//nRippleCfg Register
   0x1B2 = 0x0000	//nMiscCfg Register
   0x1B3 = 0x07D0	//nDesignCap Register
   0x1B4 = 0x801C	//nHibCfg Register
   0x1B5 = 0x0C01	//nPackCfg Register
   0x1B6 = 0x0000	//nRelaxCfg Register
   0x1B7 = 0x2241	//nConvgCfg Register
   0x1B8 = 0x0142	//nNVCfg0 Register
   0x1B9 = 0x0006	//nNVCfg1 Register
   0x1BA = 0xFC0A	//nNVCfg2 Register
   0x1BB = 0x0002	//nSBSCfg Register
   0x1BC = 0x0000	//nROMID0 Register
   0x1BD = 0x0000	//nROMID1 Register
   0x1BE = 0x0000	//nROMID2 Register
   0x1BF = 0x0000	//nROMID3 Register
   0x1C0 = 0x0000	//nVAlrtTh Register
   0x1C1 = 0x0000	//nTAlrtTh Register
   0x1C2 = 0x0000	//nSAlrtTh Register
   0x1C3 = 0x0000	//nIAlrtTh Register
   0x1C4 = 0x0000	//nUser1C4 Register
   0x1C5 = 0x0000	//nUser1C5 Register
   0x1C6 = 0x0000	//nFullSOCThr Register
   0x1C7 = 0x0000	//nTTFCfg Register
   0x1C8 = 0xC000	//nCGain Register
   0x1C9 = 0x0025	//nTCurve Register
   0x1CA = 0x0000	//nTGain Register
   0x1CB = 0x0000	//nTOff Register
   0x1CC = 0x0000	//nManfctrName0 Register
   0x1CD = 0x0000	//nManfctrName1 Register
   0x1CE = 0x0000	//nManfctrName2 Register
   0x1CF = 0x03E8	//nRSense Register
   0x1D0 = 0x0000	//nUser1D0 Register
   0x1D1 = 0x0000	//nUser1D1 Register
   0x1D2 = 0xD5E3	//nAgeFcCfg Register
   0x1D3 = 0x0000	//nDesignVoltage Register
   0x1D4 = 0x0000	//nUser1D4 Register
   0x1D5 = 0x0000	//nRFastVShdn Register
   0x1D6 = 0x0000	//nManfctrDate Register
   0x1D7 = 0x0000	//nFirstUsed Register
   0x1D8 = 0x0000	//nSerialNumber0 Register
   0x1D9 = 0x0000	//nSerialNumber1 Register
   0x1DA = 0x0000	//nSerialNumber2 Register
   0x1DB = 0x0000	//nDeviceName0 Register
   0x1DC = 0x0000	//nDeviceName1 Register
   0x1DD = 0x0000	//nDeviceName2 Register
   0x1DE = 0x0000	//nDeviceName3 Register
   0x1DF = 0x0000	//nDeviceName4 Register */
void max17201_register_cfg(void)
{
    unsigned char ack = 0;
	//0x180 = 0x0000	//nXTable0 Register
	//    ack  = MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x80,0x0000);

	//0x180 = 0x0000	//nXTable0 Register
		ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x80,0x0000);

	//0x181 = 0x0000	//nXTable1 Register
		ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x81,0xff00);

	//0x182 = 0x0000	//nXTable2 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x82,0x7f80);

	//0x183 = 0x0000	//nXTable3 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x83,0xff00);

	//0x184 = 0x0000	//nXTable4 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x84,0x0000);

	//0x185 = 0x0000	//nXTable5 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x85,0x060F);

	//0x186 = 0x0000	//nXTable6 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x86,0x0000);

	//0x187 = 0x0000	//nXTable7 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x87,0x0000);

	//0x188 = 0x0000	//nXTable8 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x88,0x0000);

	//0x189 = 0x0000	//nXTable9 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x89,0x0000);

	//0x18A = 0x0000	//nXTable10 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x8A,0x0000);

	//0x18B = 0x0000	//nXTable11 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x8B,0x0000);

	//0x18C = 0x0000	//nUser18C Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x8C,0x0000);

	//0x18D = 0x0000	//nUser18D Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x8D,0x0000);

	//0x18E = 0x0000	//nODSCTh Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x8E,0x0000);

	//0x18F = 0x0000	//nODSCCfg Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x8F,0x0000);

	//0x190 = 0x0000	//nOCVTable0 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x90,0x0000);

	//0x191 = 0x0000	//nOCVTable1 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x91,0x0000);

	//0x192 = 0x0000	//nOCVTable2 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x92,0x0000);

	//0x193 = 0x0000	//nOCVTable3 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x93,0x0000);

	//0x194 = 0x0000	//nOCVTable4 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x94,0x0000);

	//0x195 = 0x0000	//nOCVTable5 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x95,0x0000);

	//0x196 = 0x0000	//nOCVTable6 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x96,0x0000);

	//0x197 = 0x0000	//nOCVTable7 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x97,0x0000);

	//0x198 = 0x0000	//nOCVTable8 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x98,0x0000);

	//0x199 = 0x0000	//nOCVTable9 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x99,0x0000);

	//0x19A = 0x0000	//nOCVTable10 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x9A,0x0000);

	//0x19B = 0x0000	//nOCVTable11 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x9B,0x0000);

	//0x19C = 0X280	//nIChgTerm Register  100MA/0.15625 = 0X280
	//ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x9C,0X280);   //2018.10.30  100mA
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x9C,0X400);     //2018.10.30  160mA
	
	//0x19D =  0x0EA4	//nFilterCfg Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x9D, 0x0EA4);

//        //0x19E = 0x0000	//nVEmpty Register
//        ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x9E,0x0000);

	//0x19F = 0x2602	//nLearnCfg Register  原始值
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0x9F,0x2602);

	//0x1A0 = 0x3C00	//nQRTable00 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xA0,0x3C00);

	//0x1A1 = 0x1B80	//nQRTable10 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xA1,0x1B80);

	//0x1A2 = 0x0B04	//nQRTable20 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xA2,0x0B04);

	//0x1A3 = 0x0885	//nQRTable30 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xA3,0x0885);

	//0x1A4 = 0x0000	//nCycles Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xA4,0x0000);

	//0x1A5 = 0x0910	//nFullCapNom Register 0.5ma*2320 = 1160mah  13600/0.5 = 27200 = 0x6a40
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xA5,BatteryMah);

	//0x1A6 = 0x1070	//nRComp0 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xA6,0x1070);

	//0x1A7 = 0x263D	//nTempCo Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xA7,0x263D);

	//0x1A8 = 0xF830	//nIAvgEmpty Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xA8,0x0200);

	//0x1A9 = 0x07D0	//nFullCapRep Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xA9,BatteryMah);

	//0x1AA = 0x0000	//nVoltTemp Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xAA,0x0000);

//   //0x1AB = 0x807F	//nMaxMinCurr Register
//   ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xAB,0x807F);

//   //0x1AC = 0x00FF	//nMaxMinVolt Register
//   ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xAC,0x00FF);

//   //0x1AD = 0x807F	//nMaxMinTemp Register
//   ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xAD,0x807F);

	//0x1AE = 0x0000	//nSOC Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xAE,0x0000);

	//0x1AF = 0x0000	//nTimerH Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xAF,0x0000);

	//0x1B0 = 0x0000	//nConfig Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xB0,0x0000);

	//0x1B1 = 0x0204	//nRippleCfg Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xB1,0x0204);

	//0x1B2 = 0x3870	//nMiscCfg Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xB2,0x3870);

	//0x1B3 = 0x07D0	//nDesignCap Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xB3,BatteryMah); //0x7d0

	//0x1B4 = 0x801C	//nHibCfg Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xB4,0x001C);

	//0x1B5 = 0x0a03	//nPackCfg Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xB5,0x0a03);

	//0x1B6 = 0x0000	//nRelaxCfg Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xB6,0x0000);

	//0x1B7 = 0x2241	//nConvgCfg Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xB7,0x2241);

//        //0x1B8 = 0x0142	//nNVCfg0 Register ENcg = 1;
//        ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xB8,0x0140);

//        //0x1B9 = 0x0006	//nNVCfg1 Register
//        ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xB9,0x0006);

//        //0x1BA = 0xFD0A	//nNVCfg2 Register
//        ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xBA,0xFD0A);

	//0x1BB = 0x0002	//nSBSCfg Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xBB,0x0002);

	//0x1BC = 0x0000	//nROMID0 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xBC,0x0000);

	//0x1BD = 0x0000	//nROMID1 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xBD,0x0000);

	//0x1BE = 0x0000	//nROMID2 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xBE,0x0000);

	//0x1BF = 0x0000	//nROMID3 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xBF,0x0000);

	//0x1C0 =  0xFF00	//nVAlrtTh Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xC0, 0xFF00);

	//0x1C1 = 0x7F80	//nTAlrtTh Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xC1,0x7F80);

	//0x1C2 = 0xFF00	//nSAlrtTh Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xC2,0xFF00);

	//0x1C3 =  0x7F80	//nIAlrtTh Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xC3, 0x7F80);

	//0x1C4 = 0x0000	//nUser1C4 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xC4,0x0000);

	//0x1C5 = 0x0000	//nUser1C5 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xC5,0x0000);

	//0x1C6 = 0x5005	//nFullSOCThr Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xC6,0x5005);

	//0x1C7 = 0x0000	//nTTFCfg Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xC7,0x0000);

//        //0x1C8 = 0xC000	//nCGain Register
//        ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xC8,0xC000);

	//0x1C9 = 0x0025	//nTCurve Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xC9,0x0025);

	//0x1CA = 0x0000	//nTGain Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xCA,0x0000);

	//0x1CB = 0x0000	//nTOff Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xCB,0x0000);

	//0x1CC = 0x0000	//nManfctrName0 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xCC,0x0000);

	//0x1CD = 0x0000	//nManfctrName1 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xCD,0x0000);

	//0x1CE = 0x0000	//nManfctrName2 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xCE,0x0000);

	//0x1CF = 0x03E8	//nRSense Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xCF,0x03E8);  

	//0x1D0 = 0x0000	//nUser1D0 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xD0,0x0000);

	//0x1D1 = 0x0000	//nUser1D1 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xD1,0x0000);

	//0x1D2 = 0xD5E3	//nAgeFcCfg Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xD2,0xD5E3);

	//0x1D3 = 0x0000	//nDesignVoltage Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xD3,0x0000);

	//0x1D4 = 0x0000	//nUser1D4 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xD4,0x0000);

	//0x1D5 = 0x0000	//nRFastVShdn Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xD5,0x0000);

	//0x1D6 = 0x0000	//nManfctrDate Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xD6,0x0000);

	//0x1D7 = 0x0000	//nFirstUsed Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xD7,0x0000);

	//0x1D8 = 0x0000	//nSerialNumber0 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xD8,0x0000);

	//0x1D9 = 0x0000	//nSerialNumber1 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xD9,0x0000);

	//0x1DA = 0x0000	//nSerialNumber2 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xDA,0x0000);

	//0x1DB = 0x0000	//nDeviceName0 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xDB,0x0000);

	//0x1DC = 0x0000	//nDeviceName1 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xDC,0x0000);

	//0x1DD = 0x0000	//nDeviceName2 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xDD,0x0000);

	//0x1DE = 0x0000	//nDeviceName3 Register
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xDE,0x0000);

	//0x1DF = 0x0000	//nDeviceName4 Register    
	ack |= MAX17201_write_16_bits(MAX17201_I2C_ADDRESS2, 0xDF,0x0000); 
}
//===========================================================================//
//	功能:		模块初始化
//	主要思路:
//	备注:		内部调用
//===========================================================================//
void MAX17201_init(void) //注意，在串口初始化后面调用
{ 
    MAX17201_Reset();
//    delay_ms(30); 
    Fuel_Gauge_Reset();
    max17201_register_cfg();
    delay_ms(1); 
    
    nNVCfg0_config(); 
    nNVCfg1_config(); 
    nNVCfg2_config();  

    NV_remain_num();
    delay_ms(200); 
    Measurements_VBATT_Pin_Config(); //测电压管脚设置
	nVEmpty_config();//空电压检测电压点设置
	
	
    MAX17201_Current_Adjustment(0x391); //(100ma/111ma)*1024 电流校准
}
//===========================================================================//
//	功能:		获取电量百分比
//	返回:		0-101=正常范围    0xff=电压过低
//	主要思路:	1.读取芯片型号-> 2.计算总电量-> 3.电池电压-> 4.剩余电量
//				当电压>3.31V-> 当前电量/最大电量-30(在电量到100后断掉充电不会立即变成99)
//				3.32V以下-> 关闭设备电源
//	备注:		内部调用
//===========================================================================//
u16 g_FullRepCap;
//u16 g_RepCap;
s16 g_Current;

u16 g_StartDianLiang =0;


u8  g_17201state =0;
u16 g_StartDianYa =0; 
s16 g_DianYaChar;

u16 g_MaxOffDianya =0;
u16 BatteryRepCap = 0;
u8  BatterF=0;
    
u8 Get_BatteryPerent(void)
{
	unsigned char ack;
	u8 data =0;
    uint16_t  DevName,test;
    uint16_t Vbatt_datA;
    
	
	ack = MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1,DevName_register_addr,&DevName); 									//1.检查DEVName
    ack |= MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1, FullCapRep_register_addr,&(max17201_register_value_stru.FullCapRep));//2.计算总电量
	ack |= MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1,Batt_register_addr,&Vbatt_datA); 								//3.电池电压
    ack |= MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1, RepCap_register_addr,&(max17201_register_value_stru.RepCap)); 	//4.剩余电量  
    ack |= MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1, RepSOC_register_addr,&(max17201_register_value_stru.RepSOC));   //5.计算百分比

    ack |= MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1, Current_register_addr,(unsigned short *)&(max17201_register_value_stru.Current)); 

    if(ack == 0) {
        max17201_register_value_stru.FullCapRep = max17201_register_value_stru.FullCapRep * 0.5;
        max17201_register_value_stru.RepCap = max17201_register_value_stru.RepCap*0.5;  
        max17201_register_value_stru.Current = max17201_register_value_stru.Current*0.15625; 
		Vbatt_datA = (Vbatt_datA * 0.125);
		max17201_register_value_stru.RepSOC = max17201_register_value_stru.RepSOC/256;

        g_DianYaChar = Vbatt_datA;
        MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, Status_register_addr,status_register_union.status_register_short);    
        MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1, Status_register_addr,&(max17201_register_value_stru.Status));

        if(g_MaxMaxDianLiang<max17201_register_value_stru.RepCap){//最大电量保存
            g_MaxMaxDianLiang=max17201_register_value_stru.RepCap;
        }
        if(g_MaxMaxDianLiang>max17201_register_value_stru.FullCapRep){
            g_MaxMaxDianLiang=max17201_register_value_stru.FullCapRep;
        }
        if(max17201_register_value_stru.RepSOC == 100){
            g_MaxMaxDianLiang=max17201_register_value_stru.FullCapRep;
        } 

        if(max17201_register_value_stru.Current>100){
            g_MaxState |= 0x08;
        }
        else if(max17201_register_value_stru.Current<-10){
            g_MaxDianya = Vbatt_datA;
            g_MaxState &= 0xf7;
        }
        
        if(BatteryRepCap == 0) {//电量跳变下降
           BatteryRepCap = max17201_register_value_stru.RepCap;
        }
        if(max17201_register_value_stru.RepCap < (BatteryRepCap-3)){
                BatterF++;
                if(BatterF&0x01){
                    BatteryRepCap = BatteryRepCap-1;
                }
                else{
                    BatteryRepCap = BatteryRepCap;
                }
                max17201_register_value_stru.RepCap = BatteryRepCap;
                MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, MixCap_register_addr,(max17201_register_value_stru.RepCap+100)*2); 
                MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, RepCap_register_addr,max17201_register_value_stru.RepCap*2); 
        }
        
        g_MaxDianLiang = max17201_register_value_stru.RepCap;//电量百分比计算
        data = (max17201_register_value_stru.RepCap*100) /( g_MaxMaxDianLiang-10);
		if(Vbatt_datA<335)
			data = 0;      
	}
	test = 0;
	ack = MAX17201_read_16_bits(MAX17201_I2C_ADDRESS2,nPackCfg_register_addr,&test);
	if(data == 0){
		data = 1;
	}
	return data;
}

u8 Get_BatteryState(void)
{
	int16_t AvCurrent_dat=0; //电流缓存
	MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1, AvgCurrent_register_addr,(u16 *)&AvCurrent_dat);  //读电流
	if(AvCurrent_dat>50)
		return 1;
	else if(AvCurrent_dat < -50)
		return 0;
	return 2;	//没有安装电池，连接USB
}

u16 Quxianouhe(u16 dainya ,u16 Dianliang){

    if((dainya <= 407)&&(dainya>=400)){
        Dianliang = Dianliang-60;
    }
    if((dainya<375)&&(dainya>=372)){
        Dianliang = Dianliang-100;
    }
    if((dainya<372)&&(dainya>=368)){
        Dianliang = Dianliang-180;
    }
    else if((dainya<368)&&(dainya>=348)){
        Dianliang = Dianliang-270;
    }
    else  if((dainya<348)&&(dainya>=337)){
        Dianliang = Dianliang*0.6;
    }
    else Dianliang=0xffff;
    return Dianliang;
}
  
//===========================================================================//
//	功能:		Max芯片初始化函数
//	主要思路:	1.开机后读取芯片，电压电流，总电量-> 2.不在充电状态 && 最大电量<1700(更换电池后电量芯片的最大电量=1500)
//	返回:		0=模块错误  1=初始化	2=运行正常
//===========================================================================//
u8 Max17201Init(void)
{
	u8 ack;
	u8 Fileack;             //文件
	uint16_t Vbatt_data;    //电压缓存
    s16 sDianyaCharZhi;     //电压差值
    
	uint16_t VbattCapRep;   //电压库伦值
    s16 sCapRepCharZhi;     //电量差值
    u16 buff;
    
    MAX17201_PORT_Init();
    
    ack  = MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1, Batt_register_addr,&Vbatt_data);	//读电压  		
    ack |= MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1, Current_register_addr,(unsigned short *)&(max17201_register_value_stru.Current)); 	 
	ack |= MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1, FullCapRep_register_addr,&(max17201_register_value_stru.FullCapRep));//读最大容量    
    ack |= MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1, RepCap_register_addr,&(max17201_register_value_stru.RepCap)); 	//4.剩余电量 
    
//    Fileack = ReadDianLiangFile();
    
	if(ack == 0) {
        Vbatt_data = (Vbatt_data * 0.125);  //开机电压
        max17201_register_value_stru.Current = max17201_register_value_stru.Current*0.15625;//电流
        max17201_register_value_stru.RepCap = max17201_register_value_stru.RepCap*0.5;//芯片库伦
        sDianyaCharZhi = Vbatt_data - g_MaxDianya;  //电压差 
        if(Vbatt_data>334){                         //电压库伦
            VbattCapRep = (Vbatt_data-334)*37;
        }
        else{
            if(Vbatt_data<330){
//                FuncPowerOff();
            }
            VbattCapRep = 1;
        }
        if(g_MaxMaxDianLiang <1700){    //文件未存储           
            g_MaxMaxDianLiang=AdvanceMah;
        }
        g_StartDianYa = Vbatt_data;       //调试信息
        g_StartDianLiang = g_MaxDianLiang;
        g_MaxOffDianya = g_MaxDianya;
        
        if((max17201_register_value_stru.Current > 0)){//1.充电状态 
            g_17201state=1;
            return 1;
        }
        
		if(max17201_register_value_stru.FullCapRep <= 3600) {//关机延时较长
            MAX17201_init();
            g_17201state=2;
            
            if(Fileack == 1){//文件存在
                if((sDianyaCharZhi >6)||(sDianyaCharZhi<-6)){//关机更换电池
                    delay_ms(200);
                    g_17201state=21;
                    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, MixCap_register_addr,(g_MaxDianLiang+200)*2); 
                    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, RepCap_register_addr,g_MaxDianLiang*2); 
                    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, FullCapRep_register_addr,(g_MaxMaxDianLiang+20)*2);  
                    return 1;
                }
            }
            g_17201state=22;
            buff = Quxianouhe(Vbatt_data,VbattCapRep);
            if(buff != 0xffff){
                delay_ms(200);
                VbattCapRep = buff;
                MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, MixCap_register_addr,(VbattCapRep+200)*2); 
                MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, RepCap_register_addr,VbattCapRep*2); 
                MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, FullCapRep_register_addr,(AdvanceMah+20)*2);
                g_17201state=23;
            }
            
            g_MaxMaxDianLiang=AdvanceMah;
            return 1;
		}
        
        
        if((Fileack == 0)||(g_MaxDianya == 0)){//强制关机查看电量是否更换电池
            sCapRepCharZhi = max17201_register_value_stru.RepCap-VbattCapRep;
            g_17201state=3;
            if(sCapRepCharZhi >= 250){//满电池更换低电池
                g_17201state=31;
                MAX17201_init();
                delay_ms(200);   
                buff = Quxianouhe(Vbatt_data,VbattCapRep);
                if(buff != 0xffff){
                g_17201state=32;
                    VbattCapRep = buff;
                    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, MixCap_register_addr,(VbattCapRep+200)*2); 
                    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, RepCap_register_addr,VbattCapRep*2); 
                    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, FullCapRep_register_addr,(AdvanceMah+20)*2);            
                }
                g_MaxMaxDianLiang=AdvanceMah;
            }
            else if(sCapRepCharZhi <= -250){//低电池更换满电池 或 弥补
                g_17201state=33;
                MAX17201_init();
                delay_ms(200);
                    
                buff = Quxianouhe(Vbatt_data,VbattCapRep);
                if(buff != 0xffff){
                    g_17201state=34;
                    VbattCapRep = buff;
                    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, MixCap_register_addr,(VbattCapRep+200)*2); 
                    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, RepCap_register_addr,VbattCapRep*2); 
                    MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, FullCapRep_register_addr,(AdvanceMah+20)*2);             
                
                }
                g_MaxMaxDianLiang=AdvanceMah;
            }
            else {
                max17201_register_value_stru.FullCapRep = max17201_register_value_stru.FullCapRep*0.5;
                
                if(max17201_register_value_stru.FullCapRep<AdvanceMah){
                    g_MaxMaxDianLiang=max17201_register_value_stru.FullCapRep;
                    g_17201state=35;
                }
                else{
                    g_MaxMaxDianLiang=AdvanceMah;
                    g_17201state=36;
                }
            }
            return 1;
        }
        
        if(g_MaxState&0x08){    //关机前充电
            g_17201state=4;
            return 1;
        }
        
        
        if((sDianyaCharZhi >7)||(sDianyaCharZhi<-7)){//关机更换电池
            g_17201state=5;
            sCapRepCharZhi = max17201_register_value_stru.RepCap-g_MaxDianLiang;
            if(sCapRepCharZhi<100){
                g_MaxMaxDianLiang=AdvanceMah;
                g_17201state=51;
                
                buff = Quxianouhe(Vbatt_data,VbattCapRep);
                if(buff != 0xffff){
                    VbattCapRep = buff;
                    g_17201state=52;
                }
                MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, MixCap_register_addr,(VbattCapRep+200)*2); 
                MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, RepCap_register_addr,VbattCapRep*2); 
                MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, FullCapRep_register_addr,(AdvanceMah+20)*2);  
            }
        }
        if((sDianyaCharZhi<6)&&(sDianyaCharZhi>-6)){
            g_17201state=6;
            sCapRepCharZhi = max17201_register_value_stru.RepCap-g_MaxDianLiang;
            if(sCapRepCharZhi< -20){
                g_17201state=61;
                MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, MixCap_register_addr,(g_MaxDianLiang+100)*2); 
                MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, RepCap_register_addr,(g_MaxDianLiang-10)*2); 
                MAX17201_write_16_bits(MAX17201_I2C_ADDRESS1, FullCapRep_register_addr,(g_MaxMaxDianLiang)*2);  
            }
        }
        return 1;
	}
    return 0;
}

unsigned short Get_BatteryDianYa(void)  //danwei  10mV
{
    uint16_t Vbatt_datA = 0;
	u8 ack;
    
	ack = MAX17201_read_16_bits(MAX17201_I2C_ADDRESS1,Batt_register_addr,&Vbatt_datA); 								//3.电池电压
	if(ack == 0){
		Vbatt_datA = (Vbatt_datA * 0.125);
		return Vbatt_datA;
	}
	return Vbatt_datA;
}
