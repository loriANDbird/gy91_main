#include "hcBT20.h"  
#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ff.h"
#include "OpPara.h"
#include "lcd.h"

//蓝牙设置指令
#define BT_AT			0
#define BT_RESET  		1
#define BT_DEFAULT  	2

#define BT_SNAME    	3
#define BT_SBAUD      	4

#define BT_ADVSA		5
#define BT_ADVSO		6
#define BT_AOADV		7
#define BT_SCANSA		8
#define BT_SCANSO		9
#define BT_CONN			10
#define BT_DISCON		11

#define BT_GNAME   		12
#define BT_GBAUD		13
#define BT_GLINKS     	14
#define BT_GMAC    		15


//蓝牙数据缓存
#define BT_MAXLEN	256
typedef struct {
	u8 RecvBuff[BT_MAXLEN];		//接收缓存
	u8 RecvBuffCopy[BT_MAXLEN];	//接收缓存复制
	u8 RecvCnt;					//接收长度
	u8 RecvState;				//接收状态 1=接收到数据 0=等待接收数据
	
	u8 SendBuff[BT_MAXLEN];		//发送数据缓存
	u8 SendCnt;					//发送长度
	u8 SendState;				//发送状态 1=发送数据 0=完成发送
	
	u8 RealSendEn;				//主动上传使能
	
	u8 RecvTXCmdSta;			//接收同显指令状态，0未接收，>0接收指令序号1-6
	u8 SendTXUIName[10];		//发送给同显的界面名称
}BT_DATASTRU;
BT_DATASTRU	BT_Data;

//主机指令与应答函数
typedef struct{
	char* cmdStr;//指令
	void (*cmdAnsFun)(void);//应答函数
}BT_CMD_FUN;

//指令与应答函数列表
#define BT_CMDCNT	14
BT_CMD_FUN BT_CmdList[BT_CMDCNT];

//接收指令和参数缓存
typedef struct{
	char cmdBuf[16];//指令字符串
	u8 cmdParaCnt;	//参数个数
	char cmdPara[2][64];//参数字符串，暂设置2个参数存储空间
}BT_CMD_PARA;
BT_CMD_PARA	BT_CmdPara_Buf;//主机指令和参数存储
BT_CMD_PARA	BT_TXCtrlCmdPara_Buf;//同显控制指令参数存储


//文件系统参数
FIL BT_fnew;
FRESULT BT_fres;
FILINFO BT_finfo;

//仪器编号和蓝牙编号指针
char *BT_DeviceNum = g_SysPara.EquipmentNum;
char *BT_ModuleNum = g_SysPara.BlueToothNum;

//内部调用函数声明
u8 BT_IsRecvATOK(void);
u8 BT_IsModelValid(void);
u32 BT_CheckBaud(void);
u8 BT_SetBaud(u32 newBaud,u32 oldBaud);
void BT_CmdList_Init(void);

//***************************************************************************
//********************************蓝牙硬件配置*******************************
//***************************************************************************
//蓝牙延时ms
void BT_Sleep_ms(int nms)
{
	int idly,i;
	for(i=0;i<nms;i++){
		for(idly = 0;idly < 10000;idly++);
	}
}
//蓝牙管脚初始化
void BT_Port_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//电源
    RCC_APB2PeriphClockCmd(BT_PWR_RCC,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = BT_PWR_PIN;
    GPIO_Init(BT_PWR_PORT, &GPIO_InitStructure);
	BT_PowerOff();
	
	//片选
    RCC_APB2PeriphClockCmd(BT_CS_RCC,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = BT_CS_PIN;
    GPIO_Init(BT_CS_PORT, &GPIO_InitStructure);
	BT_Enable();
	
	//AT模式
    RCC_APB2PeriphClockCmd(BT_MODE_RCC,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = BT_MODE_PIN;
    GPIO_Init(BT_MODE_PORT, &GPIO_InitStructure);
	BT_ModeATOff();
	
	//连接状态
    RCC_APB2PeriphClockCmd(BT_LINK_RCC,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = BT_LINK_PIN;
    GPIO_Init(BT_LINK_PORT, &GPIO_InitStructure);
}

//串口初始化
void BT_UART_Init(u32 baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
	//时钟使能
	RCC_APB2PeriphClockCmd(BT_UART_PIN_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(BT_UART_RCC, ENABLE);
	USART_DeInit(BT_UART);
	
	//USART_TX
    GPIO_InitStructure.GPIO_Pin = BT_UART_PIN_TX;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(BT_UART_PORT, &GPIO_InitStructure);
	
	//USART_RX
    GPIO_InitStructure.GPIO_Pin = BT_UART_PIN_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(BT_UART_PORT, &GPIO_InitStructure);
	
	//USART
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(BT_UART, &USART_InitStructure);
	
	//Usart NVIC
    NVIC_InitStructure.NVIC_IRQChannel = BT_UART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(BT_UART, USART_IT_IDLE, ENABLE);
	
	
    USART_Cmd(BT_UART, ENABLE);
}

//DMA初始化
void BT_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(BT_DMA_RCC, ENABLE);
	
	USART_DMACmd(BT_UART, USART_DMAReq_Rx, ENABLE);
    
	//DMA输入配置
    DMA_DeInit(BT_DMA_CH_RX);   									//将DMA的通道1寄存器重设为缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&BT_UART->DR);	/*设置DMA源：串口数据寄存器地址*/ 
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)BT_Data.RecvBuff;	/*内存地址(要传输的变量的指针)*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					/*方向：从外设到内存*/
    DMA_InitStructure.DMA_BufferSize = BT_MAXLEN;							/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 	/*外设地址不增*/	    
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				/*内存地址自增*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	/*外设数据单位*/	
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		/*内存数据单位 8bit*/ 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;						/*DMA模式：正常模式*/  
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  				/*优先级：中*/	
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						/*禁止内存到内存的传输	*/
    DMA_Init(BT_DMA_CH_RX,&DMA_InitStructure); 	/*配置DMA1的通道x输入*/		
    DMA_Cmd (BT_DMA_CH_RX,ENABLE);					/*使能DMA pdata->InChannel*/ 
	
	//DMA输出配置
	DMA_DeInit(BT_DMA_CH_TX);   //将DMA的通道x寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&BT_UART->DR);	/*设置DMA源：串口数据寄存器地址*/	   
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)BT_Data.SendBuff;	/*内存地址(要传输的变量的指针)*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;					/*方向：从内存到外设*/	
	DMA_InitStructure.DMA_BufferSize = BT_MAXLEN;							/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 	/*外设地址不增*/	 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				/*内存地址自增*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	/*外设数据单位*/	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 	/*内存数据单位 8bit*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;						/*DMA模式：正常模式*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  				/*优先级：中*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						/*禁止内存到内存的传输	*/
	DMA_Init(BT_DMA_CH_TX,&DMA_InitStructure); 	/*配置DMA1的7通道*/
	
//	DMA_ITConfig(BT_DMA_CH_TX,DMA_IT_TC,ENABLE);  	//配置DMA发送完成后产生中断
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;//DMA2_Stream1_IRQn中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器	
//	DMA_Cmd(BT_DMA_CH_TX, DISABLE); //关闭 DMA 传输
}

//蓝牙初始化
void BT_Init(void)
{
	u32 checkBaud;
	BT_Port_Init();//控制端口
	BT_UART_Init(BT_UART_BAUD);//串口
	BT_DMA_Init();//DMA
	BT_CmdList_Init();//指令表
	//检测并设置蓝牙模块
	if(BT_IsModelValid() == 0){
		checkBaud = BT_CheckBaud();
		if((checkBaud != BT_UART_BAUD)&&(checkBaud != 0)){
			BT_SetBaud(BT_UART_BAUD,checkBaud);
		}
		BT_SetName(BT_ModuleNum);
	}
}

//串口接收中断
void USART1_IRQHandler(void)
{
    u16 cnt;
	char cmdBuf[128];
    if(USART_GetITStatus(BT_UART,USART_IT_IDLE) != RESET){
		USART_ReceiveData(BT_UART);
//		USART_SendData(BT_UART,temp);//测试
		cnt = DMA_GetCurrDataCounter(BT_DMA_CH_RX);
		BT_Data.RecvCnt = BT_MAXLEN-cnt; //算出接本帧数据长度
		BT_Data.RecvState = 1;
		
		memset(BT_Data.RecvBuffCopy,0,BT_MAXLEN);//接收缓存初始化
		memcpy(BT_Data.RecvBuffCopy,BT_Data.RecvBuff,BT_Data.RecvCnt);
		memset(BT_Data.RecvBuff,0,BT_MAXLEN);//接收缓存初始化
//		
		//测试
		LCD_SetColor(WHITE,RED);
		sprintf(cmdBuf,"%s    ",BT_Data.RecvBuffCopy);
		LCD_ShowString(Font12,0,0,0,cmdBuf);
//		
        USART_ClearITPendingBit(BT_UART, USART_IT_IDLE);//清除中断标志
//		
        //恢复DMA指针，等待下一次的接收
		DMA_Cmd(BT_DMA_CH_RX,DISABLE);//关闭 USART1 rx DMA1所指示的通道  
		DMA_SetCurrDataCounter(BT_DMA_CH_RX,BT_MAXLEN);//设置 DMA 缓存的大小  
		DMA_Cmd(BT_DMA_CH_RX, ENABLE); //使能 USART1 rx DMA1 所指示的通道  
	}
}
//DMA接收中断
void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC5)!=RESET){//判断通道5传输完成
		
		DMA_ClearFlag(DMA1_FLAG_TC5);//清除通道5传输完成标志
	}
}

//DMA自动发送数据
void BT_DMASend(u16 txLen)
{
//	u16 i;
//	for(i=0; i<txLen; i++){
//		USART_SendData(USART1, BT_Data.SendBuff[i]);
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//	}
//	memset(BT_Data.RecvBuff,0,BT_MAXLEN);//接收缓存初始化
	BT_Data.SendState = 1;
	USART_DMACmd(BT_UART,USART_DMAReq_Tx,ENABLE); //使能串口的DMA发送
	DMA_Cmd(BT_DMA_CH_TX, DISABLE);  //关闭USART1 TX DMA1 所指示的通道
 	DMA_SetCurrDataCounter(BT_DMA_CH_TX,txLen);//DMA通道的DMA缓存的大小
 	DMA_Cmd(BT_DMA_CH_TX, ENABLE);  //使能USART1 TX DMA1 所指示的通道
}

//********************************蓝牙参数设置*******************************
//===========================================================================//
//	功能:		发送AT指令
//	入口参数:	Com指令号 Str名称字符串  data波特率
//===========================================================================//
void BT_SendAT(u8 Com,char* str,u32 data)
{
	switch (Com)
	{
		case BT_AT:		//蓝牙测试
			sprintf((char*)BT_Data.SendBuff,"AT\r");
			break;
		case BT_RESET:	//软件复位
			sprintf((char*)BT_Data.SendBuff,"AT+RESET\r");
			break;
		case BT_DEFAULT:	//恢复出厂设置
			sprintf((char*)BT_Data.SendBuff,"AT+DEF\r");
			break;
		
		//设置参数
		case BT_SNAME: 	//设置名称
			sprintf((char*)BT_Data.SendBuff,"AT+DEVNA=1,%s\r",str);
			break;
		case BT_SBAUD:	//设置波特率
			sprintf((char*)BT_Data.SendBuff,"AT+BAUD=1,%d\r",data);
			break;
		
		//控制命令
		case BT_ADVSA:	//启动广播
			sprintf((char*)BT_Data.SendBuff,"AT+ADVSO\r");
			break;
		case BT_ADVSO:	//停止广播
			sprintf((char*)BT_Data.SendBuff,"AT+ADVSO\r");
			break;
		case BT_AOADV:	//设置自动广播
			sprintf((char*)BT_Data.SendBuff,"AT+AOADV=1,1,%d\r",data);
			break;
		case BT_SCANSA:	//启动扫描
			sprintf((char*)BT_Data.SendBuff,"AT+SCANSA\r");
			break;
		case BT_SCANSO:	//停止扫描
			sprintf((char*)BT_Data.SendBuff,"AT+SCANSO\r");
			break;
		case BT_CONN:	//连接从设备
			sprintf((char*)BT_Data.SendBuff,"AT+CONN=1,1,%s\r",str);//str为MAC地址
			break;
		case BT_DISCON:	//断开连接
			sprintf((char*)BT_Data.SendBuff,"AT+DISCON=1\r");
			break;
		
		//读取参数
		case BT_GNAME:	//读蓝牙名称
			sprintf((char*)BT_Data.SendBuff,"AT+DEVNA?\r");
			break; 
		case BT_GBAUD:	//读波特率
			sprintf((char*)BT_Data.SendBuff,"AT+BAUD?\r");
			break;
		case BT_GLINKS:	//获取蓝牙状态
			sprintf((char*)BT_Data.SendBuff,"AT+LINKS?\r");
			break;
		case BT_GMAC:	//读蓝牙MAC地址
			sprintf((char*)BT_Data.SendBuff,"AT+MAC?\r");
			break;
		
		default:
			break;
	}
	
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}
//初始化接收参数
void BT_RecvReset(void)
{
	BT_Data.RecvState = 0;
//	memset(BT_Data.RecvBuffCopy,0,sizeof(BT_Data.RecvBuffCopy));
}
//判断是否接收到 AT+OK
u8 BT_IsRecvATOK(void)
{
	if(BT_Data.RecvState == 1){
		BT_Data.RecvState = 0;
		if((strstr((char*)BT_Data.RecvBuffCopy,"OK") != NULL)
			||(strstr((char*)BT_Data.RecvBuffCopy,"AT") != NULL)){
			return 1;
		}else {
			return 0;
		}
	}
	return 0;
}
//判断是否接收到数据
u8 BT_IsRecvData(void)
{
	if(BT_Data.RecvState == 1){
		BT_Data.RecvState = 0;
		return 1;
	}
	return 0;
}
//判断是否接收到数据
u8 BT_IsRecvTXCmd(void)
{
	u8 sta = 0;
	if(BT_Data.RecvTXCmdSta != 0){
		sta = BT_Data.RecvTXCmdSta;
		BT_Data.RecvTXCmdSta = 0;
		return sta;
	}
	return sta;
}
//判断蓝牙模块是否存在
u8 BT_IsModelValid(void)
{
    u16 usCount=0;
    BT_PowerOn();
    BT_Sleep_ms(200);
	BT_ModeATOn();
	BT_Sleep_ms(20);
	BT_RecvReset();
	BT_SendAT(BT_AT, NULL,0);
	BT_Sleep_ms(1);
    while(1){
		if(BT_IsRecvATOK()){
			BT_ModeATOff();
			BT_Sleep_ms(20);
			return 1;
		}
        usCount++;
        if(usCount%20 == 1){
            BT_SendAT(BT_AT,NULL,0);
        }
		BT_Sleep_ms(1);
        if(usCount>99){
			BT_ModeATOff();
			BT_Sleep_ms(20);
            return 0;
		}
    }
}
//返回值：1：成功，0：失败
u8 BT_SetName(char* BtName)
{
	u8 i;
	u16 timCnt = 0;
	char sName[12];
	char* rName = NULL;
	memcpy(sName,BtName,strlen(BtName));
	BT_PowerOn();
	BT_Sleep_ms(200);
	BT_ModeATOn();
	BT_Sleep_ms(20);
	BT_RecvReset();
	BT_SendAT(BT_SNAME,BtName,0);
	BT_Sleep_ms(1);
	while(1){	//蓝牙接收
		if(BT_IsRecvATOK()){
			break;
		}
		timCnt += 1;
		if((timCnt%10) == 0){
			BT_SendAT(BT_SNAME,BtName,0);
		}
		if(timCnt>90){
			BT_ModeATOff();
			BT_Sleep_ms(20);
			return 0;
		}
		BT_Sleep_ms(5);
	}
	timCnt = 0;
	BT_SendAT(BT_GNAME,BtName,0);
	BT_Sleep_ms(1);
	while(1){	//蓝牙接收
		if(BT_IsRecvData()){
			rName = strstr((char*)BT_Data.RecvBuffCopy,"+DEVNA:");
			if(rName != NULL){
				rName = &rName[7];
				break;
			}
		}
		timCnt += 1;
		if((timCnt%10) == 0){
			BT_SendAT(BT_GNAME, BtName,0);
		}
		if(timCnt>90){
			BT_ModeATOff();
			BT_Sleep_ms(20);
			return 0;
		}
		BT_Sleep_ms(5);
	}
	for(i=0;i<strlen(BtName);i++){
		if(rName[i] != sName[i]){
			BT_ModeATOff();
			BT_Sleep_ms(20);
			return 0;
		}
	}
	BT_ModeATOff();
	BT_Sleep_ms(20);
	return 1;
}

//返回值：1：成功，0：失败
u8 Bt_GetName(char* BtName)
{
	u16 timCnt = 0;
	char* rName = NULL;
	BT_PowerOn();
	BT_Sleep_ms(200);
	BT_ModeATOn();
	BT_Sleep_ms(20);
	BT_RecvReset();
	BT_SendAT(BT_GNAME,BtName,0);
	BT_Sleep_ms(1);
	while(1){	//蓝牙接收
		if(BT_IsRecvData()){
			rName = strstr((char*)BT_Data.RecvBuffCopy,"+DEVNA:");
			if(rName != NULL){
				rName = &rName[7];
				break;
			}
		}
		timCnt += 1;
		if((timCnt%10) == 0){
			BT_SendAT(BT_GNAME,BtName,0);
		}
		if(timCnt>90){
			BT_ModeATOff();
			BT_Sleep_ms(20);
			return 0;
		}
		BT_Sleep_ms(5);
	}
	sprintf(BtName,"%s",rName);
	BT_ModeATOff();
	BT_Sleep_ms(20);
	return 1;
}
//检测波特率
u32 BT_CheckBaud(void)
{
	u8 i;
	u32 curBaud;
	//波特率检测表
	u32 baudList[5] = {230400,115200,38400,19200,9600};
	
	for(i=0;i<5;i++){
		curBaud = baudList[i];
		BT_UART_Init(curBaud);	//自定义
		BT_Sleep_ms(10);
		if(BT_IsModelValid()){
			return baudList[i];
		}
	}
	return 0x0;
}
//设置波特率
u8 BT_SetBaud(u32 newBaud,u32 oldBaud)
{
	u16 timCnt = 0;
	BT_PowerOn();
    BT_Sleep_ms(200);
	BT_ModeATOn();
	BT_Sleep_ms(20);
	BT_RecvReset();
	BT_SendAT(BT_SBAUD,NULL,newBaud);		//设置波特率
	BT_UART_Init(newBaud);	//初始化串口
//	BT_Sleep_ms(1);
	while(1) {	//蓝牙接收
		if(BT_IsRecvATOK()){
			break;
		}
		timCnt += 1;
		if((timCnt%40) == 0){
			BT_UART_Init(oldBaud);	//初始化串口
			BT_SendAT(BT_SBAUD,NULL,newBaud);	//设置波特率
			BT_UART_Init(newBaud);	//初始化串口
		}
		if(timCnt>90){
			break;
		}
		BT_Sleep_ms(1);
	}
//	BT_Sleep_ms(20);
//	BT_SendAT(BT_RESET,NULL,0);
	BT_Sleep_ms(20);
//	BT_UART_Init(baud);	//初始化串口
//	BT_Sleep_ms(10);
	if(BT_IsModelValid()){
		BT_ModeATOff();
		BT_Sleep_ms(20);
		return 1;
	}else {
		BT_CheckBaud();//设置失败重新检测
		BT_ModeATOff();
		BT_Sleep_ms(20);
		return 0;
	}
}

//连接状态
u8 BT_GetLinkSta(void)
{
	u8 sta = 0;
	
	if(BT_LinkStaRead() == 1){
		sta = 1;
	}else{
		sta = 0;
	}
	return sta;
}

//恢复出厂设置
void BT_SetDefault(void)
{
	BT_PowerOn();
    BT_Sleep_ms(200);
	BT_ModeATOn();
	BT_Sleep_ms(20);
	BT_RecvReset();
	BT_SendAT(BT_DEFAULT,NULL,NULL);//恢复出厂设置
	BT_Sleep_ms(200);
	BT_ModeATOff();
	BT_Sleep_ms(20);
}
//***************************************************************************
//********************************蓝牙通信协议*******************************
//***************************************************************************
//接收指令解包函数
void BT_CmdUnpack(char* packStr)
{
	u8 i;
	u8 len;
	u8 paraFlag = 0;
	char* rxStr = NULL;
	
	//清空缓存
	memset(&BT_CmdPara_Buf,0x0,sizeof(BT_CmdPara_Buf));
	
	rxStr = strstr(packStr,"HC+");
	
	if(rxStr != NULL){
		len = strlen(rxStr);
		if(len > 200){//指令过长
			sprintf(BT_CmdPara_Buf.cmdBuf,"%s","ERROR3");
			return;
		}
		for(i=0;i<len;i++){
			if(rxStr[i] == '?'){//查询指令，不带参数
				memcpy(BT_CmdPara_Buf.cmdBuf,rxStr,i+1);
				BT_CmdPara_Buf.cmdBuf[i+1] = '\0';
				return;
			}
			if(rxStr[i] == '='){//带参数指令,退出循环扫描参数
				memcpy(BT_CmdPara_Buf.cmdBuf,rxStr,i+1);
				BT_CmdPara_Buf.cmdBuf[i+1] = '\0';
				
				rxStr = &rxStr[i+1];
				paraFlag = 1;
				break;
			}
			
			if((rxStr[i-1] == '\r')||(rxStr[i] == '\n')){//未检测到指令内容
				sprintf(BT_CmdPara_Buf.cmdBuf,"%s","ERROR5");
				return;
			}
			if(rxStr[i] == '\0'){
				sprintf(BT_CmdPara_Buf.cmdBuf,"%s","ERROR5");
				return;
			}
		}
		//指令参数截取
		if(paraFlag == 1){
			len = strlen(rxStr);
			i = 0;
			while(i<len){
				if(rxStr[i] == ','){
					memcpy(BT_CmdPara_Buf.cmdPara[BT_CmdPara_Buf.cmdParaCnt],rxStr,i);
					BT_CmdPara_Buf.cmdParaCnt += 1;
					rxStr = &rxStr[i+1];
					len = strlen(rxStr);
					i = 0;
					continue;
				}
				if((rxStr[i-1] == '\r')&&(rxStr[i] == '\n')){
					memcpy(BT_CmdPara_Buf.cmdPara[BT_CmdPara_Buf.cmdParaCnt],rxStr,i-1);
					BT_CmdPara_Buf.cmdParaCnt += 1;
					break;
				}
				if(rxStr[i] == '\0'){
					sprintf(BT_CmdPara_Buf.cmdBuf,"%s","ERROR4");
					return;
				}
				
				i += 1;
			}
		}else{//指令不完整
			sprintf(BT_CmdPara_Buf.cmdBuf,"%s","ERROR5");
		}
	}else{//未检测到指令
		sprintf(BT_CmdPara_Buf.cmdBuf,"%s","ERROR1");
	}
}

//指令匹配函数
//返回值指令序号
void BT_CmdMatch(void)
{
	u8 i;
	u8 funNum = 0xff;
	char* strp = NULL;
	for(i=0;i<BT_CMDCNT;i++){
		strp = strstr(BT_CmdList[i].cmdStr,BT_CmdPara_Buf.cmdBuf);
		if(strp != NULL){
			funNum = i;
			break;
		}
	}
	if(funNum != 0xff){
		(*BT_CmdList[funNum].cmdAnsFun)();//调用应答函数
	}
}

//蓝牙协议查询函数
void BT_Lookup(void)
{
	if(BT_GetLinkSta()){//已连接
		if(BT_Data.RecvState == 1){
			BT_Data.RecvState = 0;
			BT_CmdUnpack((char*)BT_Data.RecvBuffCopy);
			BT_CmdMatch();
		}
	}
}

//********************************指令应答函数*******************************
//主机指令
//设备信息
void HC_INFO_Answer(void)
{
	sprintf((char *)BT_Data.SendBuff,"+INFO:%s,%s,%s,%s,%s\r\n",
		BT_DEVICE,BT_HARDVER,BT_SOFTVER,BT_MODULE,BT_DeviceNum); 
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}

//电量查询
void HC_BAT_Answer(void)
{
	sprintf((char *)BT_Data.SendBuff,"+BAT:%d\r\n",g_GlobalPara.iBatPercent);
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}

//内存查询
void HC_MEM_Answer(void)
{
	sprintf((char *)BT_Data.SendBuff,"+MEM:%d,%d,%d\r\n",512,1024,512);//g_GlobalPara.iStorageFree,g_GlobalPara.iStorageAll,g_GlobalPara.iStorageUsedPcnt);
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}

//温度查询
void HC_TEMP_Answer(void)
{
	sprintf((char *)BT_Data.SendBuff,"+TEMP:%4.1f\r\n",g_GlobalPara.fTemperature); 
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}

//读取文件列表
void HC_DIR_Answer(void)
{
	char buff[32];
	char *fName;
	
	u8 packFlag = 0;
	u16 fNum = 0;
	u16 usCnt = 0;
	u16 fIndex = 0;
	u16 fIndexBuf[50];
	char fNameBuf[_MAX_LFN];
	u8 dateN,dateY,dateR,timeS,timeF,timeM; //日期,时间
	u32 txlen = 200;//170
	DIR dirs;
	
#if _USE_LFN
 	BT_finfo.lfsize = _MAX_LFN;
	BT_finfo.lfname = fNameBuf;
#endif
	
	memset(fIndexBuf,0,sizeof(fIndexBuf));
	
	sprintf(buff,"0:%s",BT_CmdPara_Buf.cmdPara[0]);
	BT_fres = f_opendir(&dirs,(const char *)buff);	//重新打开文件
	if(BT_fres != FR_OK){//打开目录失败
        return;
    }
    
	while(f_readdir(&dirs,&BT_finfo) == FR_OK){
		
		#if _USE_LFN
			fName = *BT_finfo.lfname ? BT_finfo.lfname : BT_finfo.fname;
		#else							   
			fName = BT_finfo.fname;
		#endif			
		
		if(!BT_finfo.fname[0]) { 		//没有文件BT_finfo.fname
			if(packFlag == 1){
                BT_Data.SendBuff[usCnt] = 0;
				sprintf((char *)&BT_Data.SendBuff[usCnt],"rows=%d,END\r\n",fNum); 
				
				BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);//usCnt;
                BT_DMASend(BT_Data.SendCnt);
                BT_Sleep_ms(BT_DELAY);
			}
			return;//1;
		}
		if(BT_finfo.fname[0] == '.'){	//错误文件
			continue;
		}
		if(strstr((const char*)fName,"SYSTEM")!=NULL){ 	//文件
			continue;
		}
 		if(strstr((const char*)fName,"System")!=NULL){ 	//文件
			continue;
		}
		if(dirs.index < (fIndexBuf[fIndex])){	//已上传
			continue;
		}
		if(packFlag == 0) {
			sprintf((char *)&BT_Data.SendBuff[0],"+DIR:\r\n");
			usCnt=7;
			packFlag = 1;
			fIndexBuf[fIndex] = dirs.index;
		}
		dateN = (BT_finfo.fdate >>9)-20;
		dateY = (BT_finfo.fdate >>5)&0xf;
		dateR = BT_finfo.fdate&0x1f;
        
		timeS = BT_finfo.ftime >>11;
		timeF = (BT_finfo.ftime >>5)&0x3f;
		timeM = BT_finfo.ftime&0x1f;
		
		if(0 == BT_finfo.fsize){
			sprintf((char *)&BT_Data.SendBuff[usCnt],
			"%s,%02d-%02d-%02d %02d:%02d:%02d,%1ld\r\n",fName,dateN,dateY,dateR,timeS,timeF,timeM,BT_finfo.fsize);	
		}else{
			sprintf((char *)&BT_Data.SendBuff[usCnt],
			"%s,%02d-%02d-%02d %02d:%02d:%02d,DX=%1ld\r\n",fName,dateN,dateY,dateR,timeS,timeF,timeM,BT_finfo.fsize);			
		}
		usCnt = usCnt+strlen((char*)&BT_Data.SendBuff[usCnt]);
		fNum++;
		
		if(usCnt > txlen){				//发送一包
            BT_Data.SendBuff[usCnt] = 0;
			BT_Data.SendCnt = usCnt;
			BT_DMASend(BT_Data.SendCnt);
            BT_Sleep_ms(BT_DELAY);
			fIndex += 1;
//			packFlag = 0;
			usCnt=0;
		}
	}
	return;
}

//读取文件数据
void HC_RDFILE_Answer(void)
{
	u16 i;
	u32 fbw;
	char buff[40];
	u32 sendSize;	//发送数据大小
	u32 readAddr;	//读取文件地址
	u8 endflag = 0;
	u32 dataSum = 0;
	u32 txlen = 200;
    
	readAddr = 0;
	
	sprintf(buff,"0:%s",BT_CmdPara_Buf.cmdPara[0]);
 	BT_fres = f_open(&BT_fnew,buff, FA_READ);
	
	if (BT_fres == FR_OK){
		if(BT_fnew.fsize > readAddr) { //读取列表溢出
			for(;;){
				f_lseek(&BT_fnew,readAddr);
				sendSize = BT_fnew.fsize - readAddr;//数据长度计算
				if(sendSize > txlen) {
					sendSize = txlen;
					
					endflag = 0;
				}else{
					endflag = 1;//最后一包
				}
				if(readAddr == 0){//第一包
					sprintf((char *)BT_Data.SendBuff,"+FILE:\r\n");//8个字节
					f_read(&BT_fnew,(u8*)(&BT_Data.SendBuff[8]),sendSize,&fbw);
					for(i=0;i<sendSize;i++){
						dataSum += BT_Data.SendBuff[8+i];
					}
					sendSize += 8;
				}else{
					f_read(&BT_fnew,(u8*)(&BT_Data.SendBuff[0]),sendSize,&fbw);
					for(i=0;i<sendSize;i++){
						dataSum += BT_Data.SendBuff[i];
					}
				}
				
				if(endflag == 1){//最后一包
					BT_Data.SendBuff[sendSize] = ((dataSum + BT_fnew.fsize)>>1)&0xff;//CRC
					sprintf((char*)&BT_Data.SendBuff[sendSize+1],"END\r\n");
					sendSize += 6;
				}
				
				BT_Data.SendCnt = sendSize;
				BT_DMASend(BT_Data.SendCnt);
				
				readAddr = readAddr + txlen;
				BT_Sleep_ms(BT_DELAY);
//				memset(g_UartStr.ucSendBuff,0,sizeof(g_UartStr.ucSendBuff));
				
				if(endflag == 1){//最后一包
					f_close(&BT_fnew);
					return;
				}
			}
		}else{
			f_close(&BT_fnew);
			return;
		}
	}
	f_close(&BT_fnew);
	return;
}

//自动上传
void HC_AUTOSEND_Answer(void)
{
	sprintf((char *)BT_Data.SendBuff,"+AUTOSEND:OK\r\n");
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
	
	if(strstr(BT_CmdPara_Buf.cmdPara[0],"ON") != NULL){
		BT_Data.RealSendEn = 1;
	}else{
		BT_Data.RealSendEn = 0;
	}
}

//同显指令
//控制指令
void HC_CTRL_Answer(void)
{
	BT_Data.RecvTXCmdSta = 1;
	memcpy(&BT_TXCtrlCmdPara_Buf,&BT_CmdPara_Buf,sizeof(BT_CmdPara_Buf));
	
	sprintf((char *)BT_Data.SendBuff,"+CTRL:OK\r\n");
	
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}
//系统参数
void HC_SYSPARA_Answer(void)
{
	char tempBuf[24];
	BT_Data.RecvTXCmdSta = 2;
	
	sprintf((char *)BT_Data.SendBuff,"+SYSPARA:");
	//界面编号
	strcat((char *)BT_Data.SendBuff,(char*)BT_Data.SendTXUIName);
	//时间
	sprintf(tempBuf,",%02d-%02d-%02d %02d:%02d:%02d,",
		g_SysTime.year,g_SysTime.month,g_SysTime.day,g_SysTime.hour,g_SysTime.min,g_SysTime.sec);	
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//实时温度
	sprintf(tempBuf,"%5.1f,",g_GlobalPara.fTemperature);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//上次标定温度
	sprintf(tempBuf,"%5.1f,",g_SysPara.fTemperatureBDLast);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//实时电量
	sprintf(tempBuf,"%d,",g_GlobalPara.iBatPercent);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//上次标定电量
	sprintf(tempBuf,"%d\r\n",g_SysPara.iBatPercentBDLast);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}
//快速检测实时数据
void HC_QUICKRT_Answer(void)
{
	char tempBuf[24];
	BT_Data.RecvTXCmdSta = 3;
	
	sprintf((char *)BT_Data.SendBuff,"+QUICKRT:");
	//界面编号
	strcat((char *)BT_Data.SendBuff,(char*)BT_Data.SendTXUIName);
	strcat((char *)BT_Data.SendBuff,",");
	//构件名称
	strcat((char *)BT_Data.SendBuff,(char*)g_SysPara.GouJianName);
	strcat((char *)BT_Data.SendBuff,",");
	//显示模式
	if(g_SysPara.SampleDispMode == MIAO_POU){
		sprintf(tempBuf,"MP,");
	}else if(g_SysPara.SampleDispMode == MIAO_BO){
		sprintf(tempBuf,"MB,");
	}else if(g_SysPara.SampleDispMode == TIAO_POU){
		sprintf(tempBuf,"TP,");
	}else if(g_SysPara.SampleDispMode == TIAO_BO){
		sprintf(tempBuf,"TB,");
	}
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//钢筋类型
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iRebarType);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//钢筋直径
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iZhujinDiam);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//构件类型
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iGoujianType);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//设计厚度
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iDesignHoudu);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//已存测点
	sprintf(tempBuf,"%d,%d,%d,",ResBuf_CH1.AllRebarCnt,ResBuf_CH2.AllRebarCnt,ResBuf_CH3.AllRebarCnt);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//通道
	sprintf(tempBuf,"%d,",g_DetectPara.Channel+1);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//瞄准镜坐标
	sprintf(tempBuf,"%d,",g_DetectPara.MZJSite);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//小车位移
	sprintf(tempBuf,"%d,",g_DetectPara.CarSite);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//实时厚度
	sprintf(tempBuf,"%d\r\n",g_DetectPara.HouduRes);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}
//快速检测历史数据
void HC_QUICKHIST_Answer(void)
{
	u16 i;
	char tempBuf[24];
	u16 startNum,endNum;
	
	BT_Data.RecvTXCmdSta = 4;
	sprintf((char *)BT_Data.SendBuff,"+QUICKHIST:");
	//界面编号
	strcat((char *)BT_Data.SendBuff,(char*)BT_Data.SendTXUIName);
	strcat((char *)BT_Data.SendBuff,",");
	//通道
	sprintf(tempBuf,"%d,",g_DetectPara.Channel+1);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//起始钢筋
	sprintf(tempBuf,"%s,",BT_CmdPara_Buf.cmdPara[0]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//结束钢筋
	sprintf(tempBuf,"%s,",BT_CmdPara_Buf.cmdPara[1]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	
	//数据
	startNum = atoi(BT_CmdPara_Buf.cmdPara[0]);
	endNum = atoi(BT_CmdPara_Buf.cmdPara[1]);
	for(i=startNum;i<=endNum;i++){
		if(i<endNum){
			sprintf(tempBuf,"%d,%d,",ResBuf_CH1.RebarSite[i],ResBuf_CH1.RebarCover[i]);
			strcat((char *)BT_Data.SendBuff,tempBuf);
		}else{
			sprintf(tempBuf,"%d,%d\r\n",ResBuf_CH1.RebarSite[i],ResBuf_CH1.RebarCover[i]);
			strcat((char *)BT_Data.SendBuff,tempBuf);
		}
	}
	
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}
//JGJ实时数据
void HC_JGJRT_Answer(void)
{
	u8 index;
	char tempBuf[24];
	BT_Data.RecvTXCmdSta = 5;
	
	sprintf((char *)BT_Data.SendBuff,"+JGJRT:");
	//界面编号
	strcat((char *)BT_Data.SendBuff,(char*)BT_Data.SendTXUIName);
	strcat((char *)BT_Data.SendBuff,",");
	//构件名称
	strcat((char *)BT_Data.SendBuff,(char*)g_SysPara.GouJianName);
	strcat((char *)BT_Data.SendBuff,",");
	//显示模式
	if(g_SysPara.SampleDispMode == MIAO_POU){
		sprintf(tempBuf,"MP,");
	}else if(g_SysPara.SampleDispMode == MIAO_BO){
		sprintf(tempBuf,"MB,");
	}else if(g_SysPara.SampleDispMode == TIAO_POU){
		sprintf(tempBuf,"TP,");
	}else if(g_SysPara.SampleDispMode == TIAO_BO){
		sprintf(tempBuf,"TB,");
	}
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//检测方向
	sprintf(tempBuf,"%d,",g_SysPara.JGJScanDir);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//钢筋类型
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iRebarType);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//钢筋直径
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iZhujinDiam);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//构件类型
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iGoujianType);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//设计厚度
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iDesignHoudu);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//已存测点
	sprintf(tempBuf,"%d,",g_GlobalPara.iSaveDataCnt);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//瞄准镜坐标
	sprintf(tempBuf,"%d,",g_DetectPara.MZJSite);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//小车位移
	sprintf(tempBuf,"%d,",g_DetectPara.CarSite);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//实时厚度
	sprintf(tempBuf,"%d,",g_DetectPara.HouduRes);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//当前钢筋序号
	sprintf(tempBuf,"%d,",g_DetectPara.JGJNum);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//当前测点序号
	sprintf(tempBuf,"%d,",g_DetectPara.JGJDot);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	index = ((g_DetectPara.JGJDot-1)/2)*3;
	//当前测点厚度1
	sprintf(tempBuf,"%d,",JGJData[g_DetectPara.JGJNum][index]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//当前测点厚度2
	sprintf(tempBuf,"%d,",JGJData[g_DetectPara.JGJNum][index+1]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//实时厚度
	sprintf(tempBuf,"%d\r\n",JGJData[g_DetectPara.JGJNum][index+2]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}
//JGJ历史数据
void HC_JGJHIST_Answer(void)
{
	u16 i;
	char tempBuf[24];
	u16 startNum,endNum;
	
	BT_Data.RecvTXCmdSta = 6;
	
	sprintf((char *)BT_Data.SendBuff,"+QUICKHIST:");
	//界面编号
	strcat((char *)BT_Data.SendBuff,(char*)BT_Data.SendTXUIName);
	strcat((char *)BT_Data.SendBuff,",");
	//起始钢筋
	sprintf(tempBuf,"%s,",BT_CmdPara_Buf.cmdPara[0]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//结束钢筋
	sprintf(tempBuf,"%s,",BT_CmdPara_Buf.cmdPara[1]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	
	//数据
	startNum = atoi(BT_CmdPara_Buf.cmdPara[0]);
	endNum = atoi(BT_CmdPara_Buf.cmdPara[1]);
	for(i=startNum;i<=endNum;i++){
		if(i<endNum){
			sprintf(tempBuf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,",
				JGJData[i][0],JGJData[i][1],JGJData[i][2],JGJData[i][3],JGJData[i][4],JGJData[i][5],JGJData[i][6],JGJData[i][7],JGJData[i][8]);
			strcat((char *)BT_Data.SendBuff,tempBuf);
		}else{
			sprintf(tempBuf,"%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
				JGJData[i][0],JGJData[i][1],JGJData[i][2],JGJData[i][3],JGJData[i][4],JGJData[i][5],JGJData[i][6],JGJData[i][7],JGJData[i][8]);
			strcat((char *)BT_Data.SendBuff,tempBuf);
		}
	}
	
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}

//错误信息回复
void ERROR_Answer(void)
{
	switch(BT_CmdPara_Buf.cmdBuf[5]){
		case 1:
			sprintf((char *)BT_Data.SendBuff,"+ERR:1 no HC+\r\n");
			break;
		case 2:
			sprintf((char *)BT_Data.SendBuff,"+ERR:2 Err Val\r\n");
			break;
		case 3:
			sprintf((char *)BT_Data.SendBuff,"+ERR:3 Too Long\r\n");
			break;
		case 4:
			sprintf((char *)BT_Data.SendBuff,"+ERR:4 no \\r\\n\r\n");
			break;
		case 5:
			sprintf((char *)BT_Data.SendBuff,"+ERR:5 unknow\r\n");
			break;
		default:
			sprintf((char *)BT_Data.SendBuff,"+ERR:5 unknow\r\n");
			break;
	}
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}

//主动上传文件路径
void BT_RealSendFile(const char *pFileName)
{
   if(BT_Data.RealSendEn == 1){
//    sprintf((char *)BT_Data.SendBuff,"+UPFILE:%s\r\n",pFileName); 
//        BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff); 
//        BT_DMASend(BT_Data.SendCnt);
        sprintf(BT_CmdPara_Buf.cmdPara[0],"%s",pFileName);
        HC_RDFILE_Answer();
    }
}


//蓝牙指令列表初始化
void BT_CmdList_Init(void)
{
	//主机指令
	BT_CmdList[0].cmdStr = "HC+INFO?";
	BT_CmdList[0].cmdAnsFun = (&HC_INFO_Answer);
	
	BT_CmdList[1].cmdStr = "HC+BAT?";
	BT_CmdList[1].cmdAnsFun = (&HC_BAT_Answer);
	
	BT_CmdList[2].cmdStr = "HC+MEM?";
	BT_CmdList[2].cmdAnsFun = (&HC_MEM_Answer);
	
	BT_CmdList[3].cmdStr = "HC+TEMP?";
	BT_CmdList[3].cmdAnsFun = (&HC_TEMP_Answer);
	
	BT_CmdList[4].cmdStr = "HC+DIR=";
	BT_CmdList[4].cmdAnsFun = (&HC_DIR_Answer);
	
	BT_CmdList[5].cmdStr = "HC+RDFILE=";
	BT_CmdList[5].cmdAnsFun = (&HC_RDFILE_Answer);
	
	BT_CmdList[6].cmdStr = "HC+AUTOSEND=";
	BT_CmdList[6].cmdAnsFun = (&HC_AUTOSEND_Answer);
	
	//同显指令
	BT_CmdList[7].cmdStr = "HC+CTRL=";
	BT_CmdList[7].cmdAnsFun = (&HC_CTRL_Answer);
	
	BT_CmdList[8].cmdStr = "HC+SYSPARA?";
	BT_CmdList[8].cmdAnsFun = (&HC_SYSPARA_Answer);
	
	BT_CmdList[9].cmdStr = "HC+QUICKRT?";
	BT_CmdList[9].cmdAnsFun = (&HC_QUICKRT_Answer);
	
	BT_CmdList[10].cmdStr = "HC+QUICKHIST=";
	BT_CmdList[10].cmdAnsFun = (&HC_QUICKHIST_Answer);
	
	BT_CmdList[11].cmdStr = "HC+JGJRT?";
	BT_CmdList[11].cmdAnsFun = (&HC_JGJRT_Answer);
	
	BT_CmdList[12].cmdStr = "HC+JGJHIST=";
	BT_CmdList[12].cmdAnsFun = (&HC_JGJHIST_Answer);
	//指令错误
	BT_CmdList[13].cmdStr = "ERROR";
	BT_CmdList[13].cmdAnsFun = (&ERROR_Answer);
}
