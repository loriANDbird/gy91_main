/*******************************************************************************
创建者：康泉
创建日期：2018.03.01
功能描述：
    1. UART串行接口驱动
    2. UART2接收采用DMA方式
    3. UART2发送采用中断方式
    4. 数据包的头、尾、长度采用宏定义方式，方便移植
       接收到一包有效数据，g_Uart2RecValid=1， 接收数据缓冲区为g_Uart2RecBuf[]
    
更新记录：
    1. 2018.03.01 创建
    
******************************************************************************/
#include "uart.h"
#include "delay.h"
#include "string.h"
#include "ledBeep.h"

//UART1串口协议:  HEAD xx xx xx ... END
//UART1_DMA1_CH6_DAT_LEN: 数解包总长度   //当此处大于10时，需修改接收缓冲区数组的大小


#define  UART2_DAT_HEAD		0xA1
#define  UART2_DAT_END		0xA5
#define  UART2_DAT_LEN   	22

unsigned char g_ucUart2SendBuf[50];
unsigned char g_ucUart2RecBuf[50];

unsigned char g_Uart2DmaRecBuf[50];

unsigned char g_ucUart2RecCnt;
unsigned char g_ucUart2RecValid;

void USART2_RX_DMA_Init(int DataLen);

void USART2_Init(int iBaud)
{
    
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  //TXD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  //RXD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = iBaud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //接收中断
//	USART_ITConfig(USART1, USART_IT_TC, ENABLE);    //发送中断
//	USART_ClearITPendingBit(USART2, USART_IT_TC);

//	if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET){
//		USART_ClearFlag(USART2, USART_FLAG_ORE);
//	}
	
	USART_Cmd(USART2, ENABLE); //使能串口1
    USART2_RX_DMA_Init(UART2_DAT_LEN);
    
    g_ucUart2RecValid = 0;
    g_ucUart2RecCnt = 0;
	memset(g_ucUart2RecBuf,0x0,100);
}


//void USART2_IRQHandler(void)
//{
//	u8 temp;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){ 	//改用DMA 接收数据	
//		temp = USART_ReceiveData(USART2);
//		USART_SendData(USART2, temp);
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);	  	
//	}  
//}

//串口1接收采用 DMA1,数据流6=Stream6，channel6
void USART2_RX_DMA_Init(int DataLen)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
			
	DMA_Cmd(DMA1_Channel6, DISABLE); //关闭 DMA 传输 
	DMA_ClearFlag(DMA1_FLAG_TC6);
	
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE); 
	
	//DMA配置
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	DMA_DeInit(DMA1_Channel6);   //将DMA的通道5寄存器重设为缺省值
         
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)g_Uart2DmaRecBuf;  //DMA内存基地址        
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向
	DMA_InitStructure.DMA_BufferSize = DataLen;  //DMA通道的DMA缓存的大小        
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //循环模式，可以连续输出中断信号
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
  	//DMA输出配置
	DMA_DeInit(DMA1_Channel7);   //将DMA的通道x寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;	/*设置DMA源：串口数据寄存器地址*/	   
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)g_ucUart2SendBuf;	/*内存地址(要传输的变量的指针)*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;					/*方向：从内存到外设*/	
	DMA_InitStructure.DMA_BufferSize = DataLen;							/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 	/*外设地址不增*/	 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				/*内存地址自增*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	/*外设数据单位*/	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 	/*内存数据单位 8bit*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;						/*DMA模式：正常模式*/  
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  				/*优先级：中*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						/*禁止内存到内存的传输	*/
	DMA_Init(DMA1_Channel7, &DMA_InitStructure); 	/*配置DMA1的7通道*/		 
	
	//Usart2 NVIC 配置
	DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);	//使能DMA传输完成中断	DMA_IT_TC
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;//DMA2_Stream1_IRQn中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
    

    DMA_Cmd(DMA1_Channel6, DISABLE); //关闭 DMA 传输
    DMA_ClearFlag(DMA1_FLAG_TC6);
    DMA_SetCurrDataCounter(DMA1_Channel6,DataLen);  //UART1_DMA1_CH5_DAT_LEN    
	DMA_Cmd(DMA1_Channel6, ENABLE); //开启 DMA 传输
}

void DMA1_Channel6_IRQHandler(void) //串口2 RX DMA 接收完成中断 服务函数
{
    int ilenth;

	if(DMA_GetFlagStatus(DMA1_FLAG_TC6) == SET){		  
        DMA_ClearFlag(DMA1_FLAG_TC6);
        
        ilenth = DMA_GetCurrDataCounter(DMA1_Channel6);         
        memcpy(&g_ucUart2RecBuf[g_ucUart2RecCnt],g_Uart2DmaRecBuf,ilenth);
        
        //用于数据对齐操作
        if(ilenth == 1){  //查找数据包头部，
            //RedLedOnOff(); //串口接收错误指示灯
            if(g_ucUart2RecBuf[0] == UART2_DAT_HEAD){
                g_ucUart2RecCnt += 1;
                if(g_ucUart2RecCnt >= UART2_DAT_LEN){                    
                    g_ucUart2RecCnt = 0;
                    if(g_ucUart2RecBuf[UART2_DAT_LEN-1] == UART2_DAT_END){
                        g_ucUart2RecValid = 1; //接收到一包正确的数据
//						memcpy(g_ucUart2SendBuf,g_ucUart2RecBuf,UART2_DAT_LEN);
//						USART2_DMASendData(UART2_DAT_LEN);
                        USART2_RX_DMA_Init(UART2_DAT_LEN);                        
                    }
                }
            }
            return;
        }       
        if(ilenth == UART2_DAT_LEN){ //正常的DMA批量数据读取
            g_ucUart2RecCnt = 0;
            //开始\结束  符判断
            if((g_ucUart2RecBuf[0] == UART2_DAT_HEAD)&&(g_ucUart2RecBuf[UART2_DAT_LEN-1] == UART2_DAT_END)){ 
                g_ucUart2RecValid = 1;
//				memcpy(g_ucUart2SendBuf,g_ucUart2RecBuf,UART2_DAT_LEN);
//				USART2_DMASendData(UART2_DAT_LEN);
            }else{
                USART2_RX_DMA_Init(1); //接收单字节数据，用于查找数据同步
            }
        }
    }
}

//DMA自动发送数据
void USART2_DMASendData(u16 SendLen)
{
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE); //使能串口的DMA发送
	DMA_Cmd(DMA1_Channel7, DISABLE );  //关闭USART1 TX DMA1 所指示的通道      
 	DMA_SetCurrDataCounter(DMA1_Channel7,SendLen);//DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA1_Channel7, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
}

/************************
* 初始化uart4
************************/
void UART4_Init(u32 baud)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_Initstructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	
    //USART1引脚初始化 TX  ***********************************************
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOA.9
    //USART1引脚初始化 RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //usart1初始化
    USART_Initstructure.USART_BaudRate = baud;//串口波特率
    USART_Initstructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_Initstructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_Initstructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_Initstructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_Initstructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(UART4, &USART_Initstructure); //初始化串口1
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	    
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启串口1空闲中断USART_IT_RXNE
	USART_Cmd(UART4, ENABLE); //使能串口1
}

void UART4_IRQHandler(void)
{
	u8 temp;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);	
		temp = USART_ReceiveData(UART4);	//读取接收到的数据

		g_ucUart2RecValid = 1;
		g_ucUart2RecBuf[0] = 0xA0;
		g_ucUart2RecBuf[1] = 0x02;
		g_ucUart2RecBuf[2] = temp;
		g_ucUart2RecBuf[3] = (temp>>8);
		g_ucUart2RecBuf[4] = g_ucUart2RecBuf[2]^g_ucUart2RecBuf[3];
		g_ucUart2RecBuf[5] = 0xA5;
		USART_SendData(UART4, temp);
	}
}

/**************************************
* 函数名  	: fputc
* 函数描述  : 将printf函数重定位到USATRx
***************************************/
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (u8) ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    return ch;
}

#endif


