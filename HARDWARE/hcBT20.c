#include "hcBT20.h"  
#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ff.h"
#include "OpPara.h"
#include "lcd.h"

//��������ָ��
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


//�������ݻ���
#define BT_MAXLEN	256
typedef struct {
	u8 RecvBuff[BT_MAXLEN];		//���ջ���
	u8 RecvBuffCopy[BT_MAXLEN];	//���ջ��渴��
	u8 RecvCnt;					//���ճ���
	u8 RecvState;				//����״̬ 1=���յ����� 0=�ȴ���������
	
	u8 SendBuff[BT_MAXLEN];		//�������ݻ���
	u8 SendCnt;					//���ͳ���
	u8 SendState;				//����״̬ 1=�������� 0=��ɷ���
	
	u8 RealSendEn;				//�����ϴ�ʹ��
	
	u8 RecvTXCmdSta;			//����ͬ��ָ��״̬��0δ���գ�>0����ָ�����1-6
	u8 SendTXUIName[10];		//���͸�ͬ�ԵĽ�������
}BT_DATASTRU;
BT_DATASTRU	BT_Data;

//����ָ����Ӧ����
typedef struct{
	char* cmdStr;//ָ��
	void (*cmdAnsFun)(void);//Ӧ����
}BT_CMD_FUN;

//ָ����Ӧ�����б�
#define BT_CMDCNT	14
BT_CMD_FUN BT_CmdList[BT_CMDCNT];

//����ָ��Ͳ�������
typedef struct{
	char cmdBuf[16];//ָ���ַ���
	u8 cmdParaCnt;	//��������
	char cmdPara[2][64];//�����ַ�����������2�������洢�ռ�
}BT_CMD_PARA;
BT_CMD_PARA	BT_CmdPara_Buf;//����ָ��Ͳ����洢
BT_CMD_PARA	BT_TXCtrlCmdPara_Buf;//ͬ�Կ���ָ������洢


//�ļ�ϵͳ����
FIL BT_fnew;
FRESULT BT_fres;
FILINFO BT_finfo;

//������ź��������ָ��
char *BT_DeviceNum = g_SysPara.EquipmentNum;
char *BT_ModuleNum = g_SysPara.BlueToothNum;

//�ڲ����ú�������
u8 BT_IsRecvATOK(void);
u8 BT_IsModelValid(void);
u32 BT_CheckBaud(void);
u8 BT_SetBaud(u32 newBaud,u32 oldBaud);
void BT_CmdList_Init(void);

//***************************************************************************
//********************************����Ӳ������*******************************
//***************************************************************************
//������ʱms
void BT_Sleep_ms(int nms)
{
	int idly,i;
	for(i=0;i<nms;i++){
		for(idly = 0;idly < 10000;idly++);
	}
}
//�����ܽų�ʼ��
void BT_Port_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//��Դ
    RCC_APB2PeriphClockCmd(BT_PWR_RCC,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = BT_PWR_PIN;
    GPIO_Init(BT_PWR_PORT, &GPIO_InitStructure);
	BT_PowerOff();
	
	//Ƭѡ
    RCC_APB2PeriphClockCmd(BT_CS_RCC,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = BT_CS_PIN;
    GPIO_Init(BT_CS_PORT, &GPIO_InitStructure);
	BT_Enable();
	
	//ATģʽ
    RCC_APB2PeriphClockCmd(BT_MODE_RCC,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = BT_MODE_PIN;
    GPIO_Init(BT_MODE_PORT, &GPIO_InitStructure);
	BT_ModeATOff();
	
	//����״̬
    RCC_APB2PeriphClockCmd(BT_LINK_RCC,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = BT_LINK_PIN;
    GPIO_Init(BT_LINK_PORT, &GPIO_InitStructure);
}

//���ڳ�ʼ��
void BT_UART_Init(u32 baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
	//ʱ��ʹ��
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

//DMA��ʼ��
void BT_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(BT_DMA_RCC, ENABLE);
	
	USART_DMACmd(BT_UART, USART_DMAReq_Rx, ENABLE);
    
	//DMA��������
    DMA_DeInit(BT_DMA_CH_RX);   									//��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&BT_UART->DR);	/*����DMAԴ���������ݼĴ�����ַ*/ 
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)BT_Data.RecvBuff;	/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					/*���򣺴����赽�ڴ�*/
    DMA_InitStructure.DMA_BufferSize = BT_MAXLEN;							/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 	/*�����ַ����*/	    
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				/*�ڴ��ַ����*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	/*�������ݵ�λ*/	
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		/*�ڴ����ݵ�λ 8bit*/ 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;						/*DMAģʽ������ģʽ*/  
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  				/*���ȼ�����*/	
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
    DMA_Init(BT_DMA_CH_RX,&DMA_InitStructure); 	/*����DMA1��ͨ��x����*/		
    DMA_Cmd (BT_DMA_CH_RX,ENABLE);					/*ʹ��DMA pdata->InChannel*/ 
	
	//DMA�������
	DMA_DeInit(BT_DMA_CH_TX);   //��DMA��ͨ��x�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&BT_UART->DR);	/*����DMAԴ���������ݼĴ�����ַ*/	   
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)BT_Data.SendBuff;	/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;					/*���򣺴��ڴ浽����*/	
	DMA_InitStructure.DMA_BufferSize = BT_MAXLEN;							/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 	/*�����ַ����*/	 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				/*�ڴ��ַ����*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	/*�������ݵ�λ*/	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 	/*�ڴ����ݵ�λ 8bit*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;						/*DMAģʽ������ģʽ*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  				/*���ȼ�����*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
	DMA_Init(BT_DMA_CH_TX,&DMA_InitStructure); 	/*����DMA1��7ͨ��*/
	
//	DMA_ITConfig(BT_DMA_CH_TX,DMA_IT_TC,ENABLE);  	//����DMA������ɺ�����ж�
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;//DMA2_Stream1_IRQn�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���	
//	DMA_Cmd(BT_DMA_CH_TX, DISABLE); //�ر� DMA ����
}

//������ʼ��
void BT_Init(void)
{
	u32 checkBaud;
	BT_Port_Init();//���ƶ˿�
	BT_UART_Init(BT_UART_BAUD);//����
	BT_DMA_Init();//DMA
	BT_CmdList_Init();//ָ���
	//��Ⲣ��������ģ��
	if(BT_IsModelValid() == 0){
		checkBaud = BT_CheckBaud();
		if((checkBaud != BT_UART_BAUD)&&(checkBaud != 0)){
			BT_SetBaud(BT_UART_BAUD,checkBaud);
		}
		BT_SetName(BT_ModuleNum);
	}
}

//���ڽ����ж�
void USART1_IRQHandler(void)
{
    u16 cnt;
	char cmdBuf[128];
    if(USART_GetITStatus(BT_UART,USART_IT_IDLE) != RESET){
		USART_ReceiveData(BT_UART);
//		USART_SendData(BT_UART,temp);//����
		cnt = DMA_GetCurrDataCounter(BT_DMA_CH_RX);
		BT_Data.RecvCnt = BT_MAXLEN-cnt; //����ӱ�֡���ݳ���
		BT_Data.RecvState = 1;
		
		memset(BT_Data.RecvBuffCopy,0,BT_MAXLEN);//���ջ����ʼ��
		memcpy(BT_Data.RecvBuffCopy,BT_Data.RecvBuff,BT_Data.RecvCnt);
		memset(BT_Data.RecvBuff,0,BT_MAXLEN);//���ջ����ʼ��
//		
		//����
		LCD_SetColor(WHITE,RED);
		sprintf(cmdBuf,"%s    ",BT_Data.RecvBuffCopy);
		LCD_ShowString(Font12,0,0,0,cmdBuf);
//		
        USART_ClearITPendingBit(BT_UART, USART_IT_IDLE);//����жϱ�־
//		
        //�ָ�DMAָ�룬�ȴ���һ�εĽ���
		DMA_Cmd(BT_DMA_CH_RX,DISABLE);//�ر� USART1 rx DMA1��ָʾ��ͨ��  
		DMA_SetCurrDataCounter(BT_DMA_CH_RX,BT_MAXLEN);//���� DMA ����Ĵ�С  
		DMA_Cmd(BT_DMA_CH_RX, ENABLE); //ʹ�� USART1 rx DMA1 ��ָʾ��ͨ��  
	}
}
//DMA�����ж�
void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC5)!=RESET){//�ж�ͨ��5�������
		
		DMA_ClearFlag(DMA1_FLAG_TC5);//���ͨ��5������ɱ�־
	}
}

//DMA�Զ���������
void BT_DMASend(u16 txLen)
{
//	u16 i;
//	for(i=0; i<txLen; i++){
//		USART_SendData(USART1, BT_Data.SendBuff[i]);
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//	}
//	memset(BT_Data.RecvBuff,0,BT_MAXLEN);//���ջ����ʼ��
	BT_Data.SendState = 1;
	USART_DMACmd(BT_UART,USART_DMAReq_Tx,ENABLE); //ʹ�ܴ��ڵ�DMA����
	DMA_Cmd(BT_DMA_CH_TX, DISABLE);  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��
 	DMA_SetCurrDataCounter(BT_DMA_CH_TX,txLen);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(BT_DMA_CH_TX, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ��
}

//********************************������������*******************************
//===========================================================================//
//	����:		����ATָ��
//	��ڲ���:	Comָ��� Str�����ַ���  data������
//===========================================================================//
void BT_SendAT(u8 Com,char* str,u32 data)
{
	switch (Com)
	{
		case BT_AT:		//��������
			sprintf((char*)BT_Data.SendBuff,"AT\r");
			break;
		case BT_RESET:	//�����λ
			sprintf((char*)BT_Data.SendBuff,"AT+RESET\r");
			break;
		case BT_DEFAULT:	//�ָ���������
			sprintf((char*)BT_Data.SendBuff,"AT+DEF\r");
			break;
		
		//���ò���
		case BT_SNAME: 	//��������
			sprintf((char*)BT_Data.SendBuff,"AT+DEVNA=1,%s\r",str);
			break;
		case BT_SBAUD:	//���ò�����
			sprintf((char*)BT_Data.SendBuff,"AT+BAUD=1,%d\r",data);
			break;
		
		//��������
		case BT_ADVSA:	//�����㲥
			sprintf((char*)BT_Data.SendBuff,"AT+ADVSO\r");
			break;
		case BT_ADVSO:	//ֹͣ�㲥
			sprintf((char*)BT_Data.SendBuff,"AT+ADVSO\r");
			break;
		case BT_AOADV:	//�����Զ��㲥
			sprintf((char*)BT_Data.SendBuff,"AT+AOADV=1,1,%d\r",data);
			break;
		case BT_SCANSA:	//����ɨ��
			sprintf((char*)BT_Data.SendBuff,"AT+SCANSA\r");
			break;
		case BT_SCANSO:	//ֹͣɨ��
			sprintf((char*)BT_Data.SendBuff,"AT+SCANSO\r");
			break;
		case BT_CONN:	//���Ӵ��豸
			sprintf((char*)BT_Data.SendBuff,"AT+CONN=1,1,%s\r",str);//strΪMAC��ַ
			break;
		case BT_DISCON:	//�Ͽ�����
			sprintf((char*)BT_Data.SendBuff,"AT+DISCON=1\r");
			break;
		
		//��ȡ����
		case BT_GNAME:	//����������
			sprintf((char*)BT_Data.SendBuff,"AT+DEVNA?\r");
			break; 
		case BT_GBAUD:	//��������
			sprintf((char*)BT_Data.SendBuff,"AT+BAUD?\r");
			break;
		case BT_GLINKS:	//��ȡ����״̬
			sprintf((char*)BT_Data.SendBuff,"AT+LINKS?\r");
			break;
		case BT_GMAC:	//������MAC��ַ
			sprintf((char*)BT_Data.SendBuff,"AT+MAC?\r");
			break;
		
		default:
			break;
	}
	
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}
//��ʼ�����ղ���
void BT_RecvReset(void)
{
	BT_Data.RecvState = 0;
//	memset(BT_Data.RecvBuffCopy,0,sizeof(BT_Data.RecvBuffCopy));
}
//�ж��Ƿ���յ� AT+OK
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
//�ж��Ƿ���յ�����
u8 BT_IsRecvData(void)
{
	if(BT_Data.RecvState == 1){
		BT_Data.RecvState = 0;
		return 1;
	}
	return 0;
}
//�ж��Ƿ���յ�����
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
//�ж�����ģ���Ƿ����
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
//����ֵ��1���ɹ���0��ʧ��
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
	while(1){	//��������
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
	while(1){	//��������
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

//����ֵ��1���ɹ���0��ʧ��
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
	while(1){	//��������
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
//��Ⲩ����
u32 BT_CheckBaud(void)
{
	u8 i;
	u32 curBaud;
	//�����ʼ���
	u32 baudList[5] = {230400,115200,38400,19200,9600};
	
	for(i=0;i<5;i++){
		curBaud = baudList[i];
		BT_UART_Init(curBaud);	//�Զ���
		BT_Sleep_ms(10);
		if(BT_IsModelValid()){
			return baudList[i];
		}
	}
	return 0x0;
}
//���ò�����
u8 BT_SetBaud(u32 newBaud,u32 oldBaud)
{
	u16 timCnt = 0;
	BT_PowerOn();
    BT_Sleep_ms(200);
	BT_ModeATOn();
	BT_Sleep_ms(20);
	BT_RecvReset();
	BT_SendAT(BT_SBAUD,NULL,newBaud);		//���ò�����
	BT_UART_Init(newBaud);	//��ʼ������
//	BT_Sleep_ms(1);
	while(1) {	//��������
		if(BT_IsRecvATOK()){
			break;
		}
		timCnt += 1;
		if((timCnt%40) == 0){
			BT_UART_Init(oldBaud);	//��ʼ������
			BT_SendAT(BT_SBAUD,NULL,newBaud);	//���ò�����
			BT_UART_Init(newBaud);	//��ʼ������
		}
		if(timCnt>90){
			break;
		}
		BT_Sleep_ms(1);
	}
//	BT_Sleep_ms(20);
//	BT_SendAT(BT_RESET,NULL,0);
	BT_Sleep_ms(20);
//	BT_UART_Init(baud);	//��ʼ������
//	BT_Sleep_ms(10);
	if(BT_IsModelValid()){
		BT_ModeATOff();
		BT_Sleep_ms(20);
		return 1;
	}else {
		BT_CheckBaud();//����ʧ�����¼��
		BT_ModeATOff();
		BT_Sleep_ms(20);
		return 0;
	}
}

//����״̬
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

//�ָ���������
void BT_SetDefault(void)
{
	BT_PowerOn();
    BT_Sleep_ms(200);
	BT_ModeATOn();
	BT_Sleep_ms(20);
	BT_RecvReset();
	BT_SendAT(BT_DEFAULT,NULL,NULL);//�ָ���������
	BT_Sleep_ms(200);
	BT_ModeATOff();
	BT_Sleep_ms(20);
}
//***************************************************************************
//********************************����ͨ��Э��*******************************
//***************************************************************************
//����ָ��������
void BT_CmdUnpack(char* packStr)
{
	u8 i;
	u8 len;
	u8 paraFlag = 0;
	char* rxStr = NULL;
	
	//��ջ���
	memset(&BT_CmdPara_Buf,0x0,sizeof(BT_CmdPara_Buf));
	
	rxStr = strstr(packStr,"HC+");
	
	if(rxStr != NULL){
		len = strlen(rxStr);
		if(len > 200){//ָ�����
			sprintf(BT_CmdPara_Buf.cmdBuf,"%s","ERROR3");
			return;
		}
		for(i=0;i<len;i++){
			if(rxStr[i] == '?'){//��ѯָ���������
				memcpy(BT_CmdPara_Buf.cmdBuf,rxStr,i+1);
				BT_CmdPara_Buf.cmdBuf[i+1] = '\0';
				return;
			}
			if(rxStr[i] == '='){//������ָ��,�˳�ѭ��ɨ�����
				memcpy(BT_CmdPara_Buf.cmdBuf,rxStr,i+1);
				BT_CmdPara_Buf.cmdBuf[i+1] = '\0';
				
				rxStr = &rxStr[i+1];
				paraFlag = 1;
				break;
			}
			
			if((rxStr[i-1] == '\r')||(rxStr[i] == '\n')){//δ��⵽ָ������
				sprintf(BT_CmdPara_Buf.cmdBuf,"%s","ERROR5");
				return;
			}
			if(rxStr[i] == '\0'){
				sprintf(BT_CmdPara_Buf.cmdBuf,"%s","ERROR5");
				return;
			}
		}
		//ָ�������ȡ
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
		}else{//ָ�����
			sprintf(BT_CmdPara_Buf.cmdBuf,"%s","ERROR5");
		}
	}else{//δ��⵽ָ��
		sprintf(BT_CmdPara_Buf.cmdBuf,"%s","ERROR1");
	}
}

//ָ��ƥ�亯��
//����ֵָ�����
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
		(*BT_CmdList[funNum].cmdAnsFun)();//����Ӧ����
	}
}

//����Э���ѯ����
void BT_Lookup(void)
{
	if(BT_GetLinkSta()){//������
		if(BT_Data.RecvState == 1){
			BT_Data.RecvState = 0;
			BT_CmdUnpack((char*)BT_Data.RecvBuffCopy);
			BT_CmdMatch();
		}
	}
}

//********************************ָ��Ӧ����*******************************
//����ָ��
//�豸��Ϣ
void HC_INFO_Answer(void)
{
	sprintf((char *)BT_Data.SendBuff,"+INFO:%s,%s,%s,%s,%s\r\n",
		BT_DEVICE,BT_HARDVER,BT_SOFTVER,BT_MODULE,BT_DeviceNum); 
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}

//������ѯ
void HC_BAT_Answer(void)
{
	sprintf((char *)BT_Data.SendBuff,"+BAT:%d\r\n",g_GlobalPara.iBatPercent);
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}

//�ڴ��ѯ
void HC_MEM_Answer(void)
{
	sprintf((char *)BT_Data.SendBuff,"+MEM:%d,%d,%d\r\n",512,1024,512);//g_GlobalPara.iStorageFree,g_GlobalPara.iStorageAll,g_GlobalPara.iStorageUsedPcnt);
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}

//�¶Ȳ�ѯ
void HC_TEMP_Answer(void)
{
	sprintf((char *)BT_Data.SendBuff,"+TEMP:%4.1f\r\n",g_GlobalPara.fTemperature); 
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}

//��ȡ�ļ��б�
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
	u8 dateN,dateY,dateR,timeS,timeF,timeM; //����,ʱ��
	u32 txlen = 200;//170
	DIR dirs;
	
#if _USE_LFN
 	BT_finfo.lfsize = _MAX_LFN;
	BT_finfo.lfname = fNameBuf;
#endif
	
	memset(fIndexBuf,0,sizeof(fIndexBuf));
	
	sprintf(buff,"0:%s",BT_CmdPara_Buf.cmdPara[0]);
	BT_fres = f_opendir(&dirs,(const char *)buff);	//���´��ļ�
	if(BT_fres != FR_OK){//��Ŀ¼ʧ��
        return;
    }
    
	while(f_readdir(&dirs,&BT_finfo) == FR_OK){
		
		#if _USE_LFN
			fName = *BT_finfo.lfname ? BT_finfo.lfname : BT_finfo.fname;
		#else							   
			fName = BT_finfo.fname;
		#endif			
		
		if(!BT_finfo.fname[0]) { 		//û���ļ�BT_finfo.fname
			if(packFlag == 1){
                BT_Data.SendBuff[usCnt] = 0;
				sprintf((char *)&BT_Data.SendBuff[usCnt],"rows=%d,END\r\n",fNum); 
				
				BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);//usCnt;
                BT_DMASend(BT_Data.SendCnt);
                BT_Sleep_ms(BT_DELAY);
			}
			return;//1;
		}
		if(BT_finfo.fname[0] == '.'){	//�����ļ�
			continue;
		}
		if(strstr((const char*)fName,"SYSTEM")!=NULL){ 	//�ļ�
			continue;
		}
 		if(strstr((const char*)fName,"System")!=NULL){ 	//�ļ�
			continue;
		}
		if(dirs.index < (fIndexBuf[fIndex])){	//���ϴ�
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
		
		if(usCnt > txlen){				//����һ��
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

//��ȡ�ļ�����
void HC_RDFILE_Answer(void)
{
	u16 i;
	u32 fbw;
	char buff[40];
	u32 sendSize;	//�������ݴ�С
	u32 readAddr;	//��ȡ�ļ���ַ
	u8 endflag = 0;
	u32 dataSum = 0;
	u32 txlen = 200;
    
	readAddr = 0;
	
	sprintf(buff,"0:%s",BT_CmdPara_Buf.cmdPara[0]);
 	BT_fres = f_open(&BT_fnew,buff, FA_READ);
	
	if (BT_fres == FR_OK){
		if(BT_fnew.fsize > readAddr) { //��ȡ�б����
			for(;;){
				f_lseek(&BT_fnew,readAddr);
				sendSize = BT_fnew.fsize - readAddr;//���ݳ��ȼ���
				if(sendSize > txlen) {
					sendSize = txlen;
					
					endflag = 0;
				}else{
					endflag = 1;//���һ��
				}
				if(readAddr == 0){//��һ��
					sprintf((char *)BT_Data.SendBuff,"+FILE:\r\n");//8���ֽ�
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
				
				if(endflag == 1){//���һ��
					BT_Data.SendBuff[sendSize] = ((dataSum + BT_fnew.fsize)>>1)&0xff;//CRC
					sprintf((char*)&BT_Data.SendBuff[sendSize+1],"END\r\n");
					sendSize += 6;
				}
				
				BT_Data.SendCnt = sendSize;
				BT_DMASend(BT_Data.SendCnt);
				
				readAddr = readAddr + txlen;
				BT_Sleep_ms(BT_DELAY);
//				memset(g_UartStr.ucSendBuff,0,sizeof(g_UartStr.ucSendBuff));
				
				if(endflag == 1){//���һ��
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

//�Զ��ϴ�
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

//ͬ��ָ��
//����ָ��
void HC_CTRL_Answer(void)
{
	BT_Data.RecvTXCmdSta = 1;
	memcpy(&BT_TXCtrlCmdPara_Buf,&BT_CmdPara_Buf,sizeof(BT_CmdPara_Buf));
	
	sprintf((char *)BT_Data.SendBuff,"+CTRL:OK\r\n");
	
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}
//ϵͳ����
void HC_SYSPARA_Answer(void)
{
	char tempBuf[24];
	BT_Data.RecvTXCmdSta = 2;
	
	sprintf((char *)BT_Data.SendBuff,"+SYSPARA:");
	//������
	strcat((char *)BT_Data.SendBuff,(char*)BT_Data.SendTXUIName);
	//ʱ��
	sprintf(tempBuf,",%02d-%02d-%02d %02d:%02d:%02d,",
		g_SysTime.year,g_SysTime.month,g_SysTime.day,g_SysTime.hour,g_SysTime.min,g_SysTime.sec);	
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//ʵʱ�¶�
	sprintf(tempBuf,"%5.1f,",g_GlobalPara.fTemperature);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//�ϴα궨�¶�
	sprintf(tempBuf,"%5.1f,",g_SysPara.fTemperatureBDLast);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//ʵʱ����
	sprintf(tempBuf,"%d,",g_GlobalPara.iBatPercent);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//�ϴα궨����
	sprintf(tempBuf,"%d\r\n",g_SysPara.iBatPercentBDLast);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}
//���ټ��ʵʱ����
void HC_QUICKRT_Answer(void)
{
	char tempBuf[24];
	BT_Data.RecvTXCmdSta = 3;
	
	sprintf((char *)BT_Data.SendBuff,"+QUICKRT:");
	//������
	strcat((char *)BT_Data.SendBuff,(char*)BT_Data.SendTXUIName);
	strcat((char *)BT_Data.SendBuff,",");
	//��������
	strcat((char *)BT_Data.SendBuff,(char*)g_SysPara.GouJianName);
	strcat((char *)BT_Data.SendBuff,",");
	//��ʾģʽ
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
	//�ֽ�����
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iRebarType);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//�ֽ�ֱ��
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iZhujinDiam);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//��������
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iGoujianType);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//��ƺ��
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iDesignHoudu);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//�Ѵ���
	sprintf(tempBuf,"%d,%d,%d,",ResBuf_CH1.AllRebarCnt,ResBuf_CH2.AllRebarCnt,ResBuf_CH3.AllRebarCnt);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//ͨ��
	sprintf(tempBuf,"%d,",g_DetectPara.Channel+1);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//��׼������
	sprintf(tempBuf,"%d,",g_DetectPara.MZJSite);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//С��λ��
	sprintf(tempBuf,"%d,",g_DetectPara.CarSite);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//ʵʱ���
	sprintf(tempBuf,"%d\r\n",g_DetectPara.HouduRes);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}
//���ټ����ʷ����
void HC_QUICKHIST_Answer(void)
{
	u16 i;
	char tempBuf[24];
	u16 startNum,endNum;
	
	BT_Data.RecvTXCmdSta = 4;
	sprintf((char *)BT_Data.SendBuff,"+QUICKHIST:");
	//������
	strcat((char *)BT_Data.SendBuff,(char*)BT_Data.SendTXUIName);
	strcat((char *)BT_Data.SendBuff,",");
	//ͨ��
	sprintf(tempBuf,"%d,",g_DetectPara.Channel+1);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//��ʼ�ֽ�
	sprintf(tempBuf,"%s,",BT_CmdPara_Buf.cmdPara[0]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//�����ֽ�
	sprintf(tempBuf,"%s,",BT_CmdPara_Buf.cmdPara[1]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	
	//����
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
//JGJʵʱ����
void HC_JGJRT_Answer(void)
{
	u8 index;
	char tempBuf[24];
	BT_Data.RecvTXCmdSta = 5;
	
	sprintf((char *)BT_Data.SendBuff,"+JGJRT:");
	//������
	strcat((char *)BT_Data.SendBuff,(char*)BT_Data.SendTXUIName);
	strcat((char *)BT_Data.SendBuff,",");
	//��������
	strcat((char *)BT_Data.SendBuff,(char*)g_SysPara.GouJianName);
	strcat((char *)BT_Data.SendBuff,",");
	//��ʾģʽ
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
	//��ⷽ��
	sprintf(tempBuf,"%d,",g_SysPara.JGJScanDir);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//�ֽ�����
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iRebarType);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//�ֽ�ֱ��
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iZhujinDiam);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//��������
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iGoujianType);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//��ƺ��
	sprintf(tempBuf,"%d,",g_SysPara.g_SamplePara.iDesignHoudu);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//�Ѵ���
	sprintf(tempBuf,"%d,",g_GlobalPara.iSaveDataCnt);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//��׼������
	sprintf(tempBuf,"%d,",g_DetectPara.MZJSite);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//С��λ��
	sprintf(tempBuf,"%d,",g_DetectPara.CarSite);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//ʵʱ���
	sprintf(tempBuf,"%d,",g_DetectPara.HouduRes);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//��ǰ�ֽ����
	sprintf(tempBuf,"%d,",g_DetectPara.JGJNum);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//��ǰ������
	sprintf(tempBuf,"%d,",g_DetectPara.JGJDot);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	index = ((g_DetectPara.JGJDot-1)/2)*3;
	//��ǰ�����1
	sprintf(tempBuf,"%d,",JGJData[g_DetectPara.JGJNum][index]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//��ǰ�����2
	sprintf(tempBuf,"%d,",JGJData[g_DetectPara.JGJNum][index+1]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//ʵʱ���
	sprintf(tempBuf,"%d\r\n",JGJData[g_DetectPara.JGJNum][index+2]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	
	BT_Data.SendCnt = strlen((char*)BT_Data.SendBuff);
	BT_DMASend(BT_Data.SendCnt);
}
//JGJ��ʷ����
void HC_JGJHIST_Answer(void)
{
	u16 i;
	char tempBuf[24];
	u16 startNum,endNum;
	
	BT_Data.RecvTXCmdSta = 6;
	
	sprintf((char *)BT_Data.SendBuff,"+QUICKHIST:");
	//������
	strcat((char *)BT_Data.SendBuff,(char*)BT_Data.SendTXUIName);
	strcat((char *)BT_Data.SendBuff,",");
	//��ʼ�ֽ�
	sprintf(tempBuf,"%s,",BT_CmdPara_Buf.cmdPara[0]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	//�����ֽ�
	sprintf(tempBuf,"%s,",BT_CmdPara_Buf.cmdPara[1]);
	strcat((char *)BT_Data.SendBuff,tempBuf);
	
	//����
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

//������Ϣ�ظ�
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

//�����ϴ��ļ�·��
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


//����ָ���б��ʼ��
void BT_CmdList_Init(void)
{
	//����ָ��
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
	
	//ͬ��ָ��
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
	//ָ�����
	BT_CmdList[13].cmdStr = "ERROR";
	BT_CmdList[13].cmdAnsFun = (&ERROR_Answer);
}
