#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "OpPara.h"
#include "gps.h"

#define GPS_UART			USART3
#define GPS_UART_RCC		RCC_APB1Periph_USART3
#define GPS_UART_IRQ		USART3_IRQn

#define GPS_UART_PIN_RCC	RCC_APB2Periph_GPIOB
#define GPS_UART_PIN_TX		GPIO_Pin_10
#define GPS_UART_PIN_RX		GPIO_Pin_11
#define GPS_UART_PORT		GPIOB
#define GPS_UART_BAUD		9600

#define GPS_DMA_CH			DMA1_Channel3
#define GPS_DMA_RCC			RCC_AHBPeriph_DMA1
#define GPS_DMA_IT_TC		DMA1_IT_TC3

#define GPS_PWR_RCC			RCC_APB2Periph_GPIOB
#define GPS_PWR_PIN			GPIO_Pin_8
#define GPS_PWR_PORT		GPIOB

#define GPS_RST_RCC			RCC_APB2Periph_GPIOB
#define GPS_RST_PIN			GPIO_Pin_9
#define GPS_RST_PORT		GPIOB

#define GPS_PWR_ON()		GPIO_ResetBits(GPS_PWR_PORT,GPS_PWR_PIN)
#define GPS_PWR_OFF()		GPIO_SetBits(GPS_PWR_PORT,GPS_PWR_PIN)

#define GPS_REC_BUF_LEN		500
#define GPS_GPRMC_PACK_LEN	100

//GPS
u8 GPS_Rec_Buf[GPS_REC_BUF_LEN];		//串口缓存
u8 GPS_Rec_Flag = 0;		//串口接收标志
u16 GPS_Rec_Cnt = 0;		//缓存长度
u8 GPS_GPRMC_Packet[GPS_GPRMC_PACK_LEN];	//GPS缓存

GPS_INFO_STR GPS_Info;		//PRS信息


//extern unsigned char g_CurMenuIndex;
unsigned char GPS_GPRMC_BUF[70]=//测试数据 40.05154 116.3225861
{"$GNRMC,084748.00,A,4003.09240,N,11619.35517,E,1.026,,080322,,,A,V*1E"};

int wgs2bd(double lat, double lon, double* pLat, double* pLon);

//电源和复位
void GPS_Port_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//电源引脚
    RCC_APB2PeriphClockCmd(GPS_PWR_RCC,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPS_PWR_PIN;
    GPIO_Init(GPS_PWR_PORT, &GPIO_InitStructure);
	GPS_PWR_OFF();
	
	//复位引脚
    RCC_APB2PeriphClockCmd(GPS_RST_RCC,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPS_RST_PIN;
    GPIO_Init(GPS_RST_PORT, &GPIO_InitStructure);
	GPIO_SetBits(GPS_RST_PORT, GPS_RST_PIN);
}

//GPS串口usart3
void GPS_UART_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
    //使能引脚时钟
	//使能串口4时钟
    //使能DMA2时钟
	RCC_APB2PeriphClockCmd(GPS_UART_PIN_RCC, ENABLE);	
    RCC_APB1PeriphClockCmd(GPS_UART_RCC, ENABLE);
	RCC_AHBPeriphClockCmd(GPS_DMA_RCC, ENABLE);	
	USART_DeInit(GPS_UART);  //复位串口
	
	//USART_TX
    GPIO_InitStructure.GPIO_Pin = GPS_UART_PIN_TX;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPS_UART_PORT, &GPIO_InitStructure);
	
	//USART_RX
    GPIO_InitStructure.GPIO_Pin = GPS_UART_PIN_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPS_UART_PORT, &GPIO_InitStructure);
	
	//Usart NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = GPS_UART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = GPS_UART_BAUD;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(GPS_UART, &USART_InitStructure); //初始化串口
    USART_ITConfig(GPS_UART, USART_IT_IDLE, ENABLE);//空闲中断
	USART_DMACmd(GPS_UART, USART_DMAReq_Rx, ENABLE);//接收DMA请求
    USART_Cmd(GPS_UART, ENABLE);                    //使能串口 
	
	DMA_DeInit(GPS_DMA_CH);   										//将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&GPS_UART->DR);//UART4_DR_Base;	  		/*设置DMA源：串口数据寄存器地址*/ 
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&GPS_Rec_Buf[0];	/*内存地址(要传输的变量的指针)*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					/*方向：从外设到内存*/
	DMA_InitStructure.DMA_BufferSize = GPS_REC_BUF_LEN;						/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 	/*外设地址不增*/	    
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				/*内存地址自增*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	/*外设数据单位*/	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		/*内存数据单位 8bit*/ 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;						/*DMA模式：正常模式*/  
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  				/*优先级：中*/	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						/*禁止内存到内存的传输	*/

	DMA_Init(GPS_DMA_CH, &DMA_InitStructure); 	/*配置DMA2的3通道*/		
	DMA_Cmd (GPS_DMA_CH,ENABLE);	/*使能DMA*/
}

//停止定位
void GPS_Stop(void)
{
    u8 J;
    GPS_PWR_OFF();
    GPS_Info.RunState = 0;			//关闭
//    GPS_Info.usCountDown = 500; 	//测试关闭10秒
    GPS_Info.usCountDown = 15000; 	//倒计时5分钟
    if(GPS_Info.status != 'A'){		//自动定位未接收到定位
        J= 0;
        if((GPS_Info.latitude[0]>='0')
        && (GPS_Info.latitude[0]<='9')) {
            J= 1;
        }
        if((GPS_Info.longitude[0]>='0')
        && (GPS_Info.longitude[0]<='9')) {
            J= 1;
        }
        if(J){//上次定位有效
            GPS_Info.status = 'A';
        }
    }
}

//开始定位
void GPS_Start(void)
{
    GPS_PWR_ON();
    GPS_Info.RunState = 1;		//开启GPS
    GPS_Info.usCountDown = 3000;//倒计时1分钟
}

//手动定位开始
void GPS_ManualStart(void){
    GPS_Start();
    GPS_Info.RunState |=0x02;
}

//系统定时器20ms中断调用，倒计时
void GPS_CountDown(void){
    if(GPS_Info.usCountDown){
        if((GPS_Info.RunState&0x02)==0){//禁止倒计时
            GPS_Info.usCountDown--;
            if(GPS_Info.usCountDown == 0){
                if(GPS_Info.RunState == 1){
                    GPS_Stop();
                }else{
                    GPS_Start();
                }
            }
        }
    }
}

//GPS初始化
void GPS_Init(void)
{
    GPS_Port_Init();
	GPS_UART_Init();
	GPS_Start();
}

//串口中断接收处理
void USART3_IRQHandler(void)
{
	u16 cnt;
    
	if(USART_GetITStatus(GPS_UART, USART_IT_IDLE) != RESET) {
		USART_ReceiveData(GPS_UART);
		cnt = DMA_GetCurrDataCounter(GPS_DMA_CH);
		GPS_Rec_Cnt = GPS_REC_BUF_LEN-cnt; //算出接本帧数据长度
		GPS_Rec_Flag = 1;
        USART_ClearITPendingBit(GPS_UART,USART_IT_IDLE);	//清除中断标志
		
		
        DMA_Cmd(GPS_DMA_CH,DISABLE);//关闭 USART1 rx DMA1所指示的通道
        DMA_SetCurrDataCounter(GPS_DMA_CH,GPS_REC_BUF_LEN);//设置 DMA 缓存的大小
        DMA_Cmd(GPS_DMA_CH,ENABLE); //使能 USART1 rx DMA1 所指示的通道
	}
}

//DMA中断
void DMA1_Channel3_IRQHandler()
{
    if(DMA_GetITStatus(GPS_DMA_IT_TC)) {
        DMA_ClearITPendingBit(GPS_DMA_IT_TC);
    }    
} 

//数据转换
void GPS_DataParsing(char * pdata,double* pbuf)
{
    u32 Buf;
    Buf = 0;
    Buf  = (pdata[0]-'0')*1000000;
    Buf += (pdata[1]-'0')*100000;
    Buf += (pdata[3]-'0')*10000;
    Buf += (pdata[4]-'0')*1000;
    Buf += (pdata[5]-'0')*100;
    Buf += (pdata[6]-'0')*10;
    Buf += (pdata[7]-'0');
    Buf  = Buf/60;//
    
    *pbuf = ((double)Buf)/100000;
}
/*
格式：$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh<CR><LF>
$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50
说明：
字段 0：$GPRMC，语句ID，表明该语句为Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐最小定位信息
字段 1：UTC时间，hhmmss.sss格式
字段 2：状态，A=定位，V=未定位 A
字段 3：纬度ddmm.mmmm，度分格式（前导位数不足则补0）
字段 4：纬度N（北纬）或S（南纬）
字段 5：经度dddmm.mmmm，度分格式（前导位数不足则补0）
字段 6：经度E（东经）或W（西经）
字段 7：速度，节，Knots（一节也是1.852千米／小时）
字段 8：方位角，度（二维方向指向，相当于二维罗盘）
字段 9：UTC日期，DDMMYY格式
*/
void GPS_Location_Data(void)
{
    u8 i,J;
    u8 cnt=0;
    u8 DataFlag=0;
    char * pData; 
//    char showbuff[8];
    double dBuf;
    
	static u32 rCnt = 0;
	/*if(g_debugMs>50){//测试运行状态
        g_debugMs = 0;
        SetColor(COLOR_White,COLOR_Black0);
        if(GPS_Info.RunState&0x01){
            LCD_ShowChar(220, 30, '1',FONT_ASC16);
        }else{
            LCD_ShowChar(220, 30, '0',FONT_ASC16);
        }
        
        sprintf(showbuff,"%04d",GPS_Info.usCountDown/50);  
        LCD_ShowEnCh(160,2,  (u8*)showbuff);
    }*/
    if(GPS_Rec_Flag == 1){
        GPS_Rec_Flag = 0;
        if(GPS_Info.RunState == 0){
            memset(GPS_Rec_Buf,0,GPS_REC_BUF_LEN);	
            return;
        }
        pData = strstr((const char*)GPS_Rec_Buf,"$GNRMC");
        if(pData != NULL) {//数据有效
            memcpy(GPS_GPRMC_Packet,pData,GPS_GPRMC_PACK_LEN);
			
			rCnt ++;
			LCD_SetColor(WHITE,BLACK);
			LCD_ShowString(Font12,0,308,0,(char*)GPS_GPRMC_Packet);
			LCD_ShowNnNums(Font12,0,296,0,4,rCnt);
//            if(debug){
//                memcpy(GPS_GPRMC_Packet,GPS_GPRMC_BUF,70);
//            }
            for(i=0;i<GPS_GPRMC_PACK_LEN;i++){//遍历字符
                if(GPS_GPRMC_Packet[i]==','){//间隔符,
                    DataFlag++;
                    if(GPS_GPRMC_Packet[i+1]!=','){//数据段
                        i = i+1;//跳过间隔符
                        J = i;//设置数据段长度
                        for(cnt=0;cnt<(J+16);cnt++,i++){
                            if(GPS_GPRMC_Packet[i]==','){i=i-1;break;}//数据段结束//间隔符
                            if(GPS_GPRMC_Packet[i]=='*'){
                                break;
                            }
                            switch (DataFlag) {
                                case 1:
                                    GPS_Info.UTC_Time[cnt] = GPS_GPRMC_Packet[i];
                                    break;
                                case 2:
                                    GPS_Info.status = GPS_GPRMC_Packet[i];
                                    break;
                                case 3:
                                    if(GPS_Info.status == 'A')//有效数据保存定位信息
                                        GPS_Info.latitude[cnt] = GPS_GPRMC_Packet[i];
                                break;
                                case 4:
                                    GPS_Info.lat_NS = GPS_GPRMC_Packet[i];
                                    break;
                                case 5:
                                    if(GPS_Info.status == 'A')
                                        GPS_Info.longitude[cnt] = GPS_GPRMC_Packet[i];
                                    break;
                                case 6:
                                    GPS_Info.lon_WE = GPS_GPRMC_Packet[i];
                                    break;
                                case 7:
                                    GPS_Info.UTC_Date[cnt] = GPS_GPRMC_Packet[i];
                                    break;
                                default:
                                    break;
                            }
                        }
                    }else{
                        if(DataFlag == 3){//
                            if(GPS_Info.status == 'V'){//有数据
                                J = 0;
                                if((GPS_Info.latitude[0]>='0')
                                && (GPS_Info.latitude[0]<='9')) {//有历史数据
                                    J = 1;
                                }
                                if((GPS_Info.longitude[0]>='0')
                                && (GPS_Info.longitude[0]<='9')) {//有历史数据
                                    J = 1;
                                }
                                if(J==0)
                                    GPS_Info.status = ' ';
                            }
                        }
                    }
                }
                if(GPS_GPRMC_Packet[i]=='*'){//结束符
                    if(GPS_Info.status == 'A'){//确认有效定位
                        J= 0;
                        if((GPS_Info.latitude[0]>='0')
                        && (GPS_Info.latitude[0]<='9')) {
                            GPS_Info.lat = ((GPS_Info.latitude[0]-'0')*10)
                                           +(GPS_Info.latitude[1]-'0');
                            GPS_DataParsing(&GPS_Info.latitude[2],&dBuf);
                            GPS_Info.lat = GPS_Info.lat+dBuf;
                            J= 1;
                        }
                        if((GPS_Info.longitude[0]>='0')
                        && (GPS_Info.longitude[0]<='9')) {
                            GPS_Info.lon = ((GPS_Info.longitude[0]-'0')*100)
                                          +((GPS_Info.longitude[1]-'0')*10)
                                          + (GPS_Info.longitude[2]-'0');
                            GPS_DataParsing(&GPS_Info.longitude[3],&dBuf);
                            GPS_Info.lon = GPS_Info.lon+dBuf;
                            J= 1;
                        }
                        if(J){
                            wgs2bd( GPS_Info.lat, GPS_Info.lon,
                                   &GPS_Info.lat,&GPS_Info.lon);
                            sprintf(GPS_Info.latitude, "%.5f",GPS_Info.lat);
                            sprintf(GPS_Info.longitude,"%.5f",GPS_Info.lon);      
                            GPS_Stop();//获取到有效数据，关闭GPS
                        }
                        /*if(g_CurMenuIndex == 1) {//主页面显示图标
                            SetColor(COLOR_Black0,COLOR_White);
                            LCD_ShowCh(175,1, "位", FONT_PIC16);
                        }*/
                    }
                    break;
                }
            }
        }
    }
} 



double pi = 3.14159265358979324;
double ee = 0.00669342162296594323;
double a = 6378245.0;
double x_pi = 3.14159265358979324 * 3000.0 / 180.0;
/**
 * Description: 中国境外返回true,境内返回false
 * @param lon 	经度
 * @param lat	纬度
 * @return
 */
int outOfChina(double lat, double lon) {
    if (lon < 72.004 || lon > 137.8347)
        return true;
    if (lat < 0.8293 || lat > 55.8271)
        return true;
    return false;
}

// 纬度偏移量
double transformLat(double x, double y) {
       double ret = 0.0;
       ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(fabs(x));
       ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
       ret += (20.0 * sin(y * pi) + 40.0 * sin(y / 3.0 * pi)) * 2.0 / 3.0;
       ret += (160.0 * sin(y / 12.0 * pi) + 320 * sin(y * pi  / 30.0)) * 2.0 / 3.0;
       return ret;
}

// 经度偏移量
double transformLon(double x, double y) {
       double ret = 0.0;
       ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(fabs(x));
       ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
       ret += (20.0 * sin(x * pi) + 40.0 * sin(x / 3.0 * pi)) * 2.0 / 3.0;
       ret += (150.0 * sin(x / 12.0 * pi) + 300.0 * sin(x / 30.0 * pi)) * 2.0 / 3.0;
       return ret;
}
// WGS84=>GCJ02 地球坐标系=>火星坐标系
int wgs2gcj(double lat, double lon, double* pLat, double* pLon) {
    double dLat;
    double dLon;
    double radLat;
    double magic;
    double sqrtMagic;
    
    if (outOfChina(lat,lon)) {
        *pLat = lat;
        *pLon = lon;
        return 0;
    }
    dLat = transformLat(lon - 105.0, lat - 35.0);
    dLon = transformLon(lon - 105.0, lat - 35.0);
    radLat = lat / 180.0 * pi;
    magic = sin(radLat);
    magic = 1 - ee * magic * magic;
    sqrtMagic = sqrt(magic);
    dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
    dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
    *pLat = lat + dLat;
    *pLon = lon + dLon;
   return 0;
}
// GCJ02=>BD09 火星坐标系=>百度坐标系  
int gcj2bd(double lat, double lon, double* pLat, double* pLon) {
   double x = lon, y = lat;
   double z = sqrt(x * x + y * y) + 0.00002 * sin(y * x_pi);
   double theta = atan2(y, x) + 0.000003 * cos(x * x_pi);
   *pLon = z * cos(theta) + 0.0065;
   *pLat = z * sin(theta) + 0.006;
   return 0;
}

//WGS84纬度 经度=>BD09纬度 经度
int wgs2bd(double lat, double lon, double* pLat, double* pLon) {
   double lat_ = 0.0, lon_ = 0.0;
   wgs2gcj(lat, lon, &lat_, &lon_);
   gcj2bd(lat_, lon_,  pLat, pLon);
   return 0;
}



