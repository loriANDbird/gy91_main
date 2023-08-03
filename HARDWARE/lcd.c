#include "lcd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "malloc.h"
#include "exfuns.h"

#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR

u16 BkgColor, TxtColor;//背景颜色，前景颜色

u16 g_u16LcdReadBuf[LCDREADBUFLEN];

/******************DRIVE IC寄存器寻址 8080 SERIES*******************************/
void write_cmd(unsigned short int comh)
{
    *(__IO uint16_t *) (Bank1_LCD_C) = comh;
}

/*****************DRIVE IC寄存器写数据 8080 SERIES******************************/
void write_data(unsigned short int dh)
{
    *(__IO uint16_t *) (Bank1_LCD_D) = dh;
}
u16 read_data(void)
{
    return *(__IO uint16_t *) (Bank1_LCD_D);
}

void delayms(unsigned short int idly)
{
    u16 i, j;
    for(i = 0; i < idly; i++) {
        for(j = 0; j < idly; j++);
    }
}
void delayus(unsigned short int idly)
{
    u16 i;
    for(i = 0; i < idly; i++);
}

//lcd初始化
void Lcd_Config()
{
    //新液晶控制芯片ILI9341
    delayms(100);
	
	//新液晶控制芯片ILI9488
	GPIO_ResetBits(GPIOD, GPIO_Pin_13);
    delayms(100);       //1000  --120ms
    GPIO_SetBits(GPIOD, GPIO_Pin_13 );
    delayms(500);
	/* 此驱动有残影
	write_cmd(0xE0); //P-Gamma 
	write_data(0x00); 
	write_data(0x13); 
	write_data(0x18); 
	write_data(0x04); 
	write_data(0x0F); 
	write_data(0x06); 
	write_data(0x3A); 
	write_data(0x56); 
	write_data(0x4D); 
	write_data(0x03); 
	write_data(0x0A); 
	write_data(0x06); 
	write_data(0x30); 
	write_data(0x3E); 
	write_data(0x0F);
	delayus(10);
	
	write_cmd(0xE1); //N-Gamma 
	write_data(0x00); 
	write_data(0x13); 
	write_data(0x18); 
	write_data(0x01); 
	write_data(0x11); 
	write_data(0x06); 
	write_data(0x38); 
	write_data(0x34); 
	write_data(0x4D);
	write_data(0x06); 
	write_data(0x0D); 
	write_data(0x0B); 
	write_data(0x31); 
	write_data(0x37); 
	write_data(0x0F);
	delayus(10); 
	
	write_cmd(0XC0); //Power Control 1 
	write_data(0x18); //Vreg1out 
	write_data(0x17); //Verg2out
	delayus(10);
	
	write_cmd(0xC1); //Power Control 2 
	write_data(0x41); //VGH,VGL
	delayus(10);
	
	write_cmd(0xC5); //Power Control 3 
	write_data(0x00); 
	write_data(0x1A); //Vcom 
	write_data(0x80);
	delayus(10);

	write_cmd(0x36); //Memory Access 
	write_data(0xC8);//纵向递增
	delayus(10);

	write_cmd(0x3A); // Interface Pixel Format 
	write_data(0x55); //16bit
	delayus(10);

	write_cmd(0XB0); // Interface Mode Control 
	write_data(0x00);
	delayus(10);

	write_cmd(0xB1); //Frame rate 
	write_data(0xA0); //60Hz
	delayus(10);
	
	write_cmd(0xB4); //Display Inversion Control 
	write_data(0x02); //2-dot 
	delayus(10);
	
	write_cmd(0XB6); //RGB/MCU Interface Control 
	write_data(0x02); //MCU 
	write_data(0x02); //Source,Gate scan dieection
	delayus(10);
	
	write_cmd(0XE9); // Set Image Function 
	write_data(0x00); //disable 24 bit data input 
	delayus(10);
	
	write_cmd(0xF7);  // Adjust Control
	write_data(0xA9); 
	write_data(0x51); 
	write_data(0x2C); 
	write_data(0x82);  // D7 stream, loose
	delayus(10);
	
	write_cmd(0x21); //Normal Black 
	write_cmd(0x11); //Sleep out 
	delayms(120); 
	write_cmd(0x29);
	*/
//	/*
	write_cmd(0XF7);
	write_data(0xA9);
	write_data(0x51);
	write_data(0x2C);
	write_data(0x82);
	
	write_cmd(0xC0);  //Power Control 1   Vreg1out=4.56  Vreg2out=-4.56
	write_data(0x0F);
	write_data(0x0F);
	
	write_cmd(0xC1);  //Power Control 2    VGH=15.81 ,VGL=-10.41,DDVDH=5.35,DDVDL=-5.23  VCL=-2.7
	write_data(0x41);
	
	write_cmd(0xC2);  
	write_data(0x22);
	
	write_cmd(0XC5);  //DC VCOM
	write_data(0x00);
	write_data(0x53);//Vcom -0.70313
//	write_data(0x1A);//Vcom -1.59375
	write_data(0x80);
	
	write_cmd(0xB1);  //FRAME RATE
	write_data(0xA0);
	write_data(0x11);
	
	write_cmd(0xB4);  //Display Inversion Control
	write_data(0x02);
	
	write_cmd(0XB6); //RGB/MCU Interface Control 
	write_data(0x02); //MCU 
	write_data(0x02); //Source,Gate scan dieection
	
	//WriteComm(0xB6);//RGB 18BIT
	//WriteData(0x20);
	//WriteData(0x22);
	//WriteData(0x3B);
	
	write_cmd(0xB7);
	write_data(0xc6);
	
	write_cmd(0xBE);
	write_data(0x00);
	write_data(0x04);
	
	write_cmd(0xE9);
	write_data(0x00);
	
	write_cmd(0x36);
	write_data(0xC8);
	
	write_cmd(0x3A);
	write_data(0x55);
	
	write_cmd(0xE0);
	write_data(0x00);
	write_data(0x08);
	write_data(0x0C);
	write_data(0x02);
	write_data(0x0E);
	write_data(0x04);
	write_data(0x30);
	write_data(0x45);
	write_data(0x47);
	write_data(0x04);
	write_data(0x0C);
	write_data(0x0A);
	write_data(0x2E);
	write_data(0x34);
	write_data(0x0F);
	
	write_cmd(0XE1);
	write_data(0x00);
	write_data(0x11);
	write_data(0x0D);
	write_data(0x01);
	write_data(0x0F);
	write_data(0x05);
	write_data(0x39);
	write_data(0x36);
	write_data(0x51);
	write_data(0x06);
	write_data(0x0F);
	write_data(0x0D);
	write_data(0x33);
	write_data(0x37);
	write_data(0x0F);
	
	write_cmd(0x21);
	write_cmd(0x11);
	delayms(150);
	write_cmd(0x29);
	delayms(20);
//*/	
}

/****************************************************************************
* 名    称：FSMC_LCD_Init(void)
* 功    能：FSMC接口配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void LCD_Init(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p;
    GPIO_InitTypeDef GPIO_InitStructure;
    //使能FSMC外设时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);

    // 复用端口为FSMC接口 FSMC-D0--D15
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 |
                                  GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
	
    //FSMC NE1  LCD片选
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //FSMC RS---LCD指令 指令/数据	切换
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_SetBits(GPIOD, GPIO_Pin_13);			           //LCD_RST

    memset((unsigned char *)&p, 0, sizeof(FSMC_NORSRAMTimingInitTypeDef));
    memset((unsigned char *)&FSMC_NORSRAMInitStructure, 0, sizeof(FSMC_NORSRAMInitTypeDef));

    //FSMC接口特性配置
    p.FSMC_AddressSetupTime = 0x02;
    p.FSMC_AddressHoldTime = 0x00;
    p.FSMC_DataSetupTime = 0x05;
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision = 0x00;
    p.FSMC_DataLatency = 0x00;
    p.FSMC_AccessMode = FSMC_AccessMode_B;

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
    /* Enable FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);

    Lcd_Config();
	delayms(100);
	LCD_Clear();
}

//显示范围
void SetDispArea(u16 x_start, u16 x_end, u16 y_start, u16 y_end)
{
    if(x_start > LCD_WIDTH) x_start = LCD_WIDTH;
	if(x_end > LCD_WIDTH) x_end = LCD_WIDTH;
	if(y_start > LCD_HEIGHT) y_start = LCD_HEIGHT;
	if(y_end > LCD_HEIGHT) y_end = LCD_HEIGHT;
	
    write_cmd(0x2B);	//X Address Setup
    write_data((x_start & 0xff00) >> 8);
    write_data(x_start & 0x00ff);
    write_data((x_end & 0xff00) >> 8);
    write_data(x_end & 0x00ff);
	
    write_cmd(0x2A);	//Y Address Setup
    write_data((y_start & 0xff00) >> 8);
    write_data((y_start & 0x00ff));
    write_data((y_end & 0xff00) >> 8);
    write_data(y_end & 0x00ff);
	
    write_cmd(0x2C);	//Write Display Data
}
//显示范围
void SetReadArea(u16 x_start, u16 x_end, u16 y_start, u16 y_end)
{
    if(x_start > LCD_WIDTH) x_start = LCD_WIDTH;
	if(x_end > LCD_WIDTH) x_end = LCD_WIDTH;
	if(y_start > LCD_HEIGHT) y_start = LCD_HEIGHT;
	if(y_end > LCD_HEIGHT) y_end = LCD_HEIGHT;
	
    write_cmd(0x2B);	//X Address Setup
    write_data((x_start & 0xff00) >> 8);
    write_data(x_start & 0x00ff);
    write_data((x_end & 0xff00) >> 8);
    write_data(x_end & 0x00ff);

    write_cmd(0x2A);	//Y Address Setup
    write_data((y_start & 0xff00) >> 8);
    write_data((y_start & 0x00ff));
    write_data((y_end & 0xff00) >> 8);
    write_data(y_end & 0x00ff);

    write_cmd(0x2E);	//Write Display Data
}

void SetCursor(u16 x,u16 y)
{
	if(x > LCD_WIDTH) x = LCD_WIDTH;
	if(y > LCD_HEIGHT) y = LCD_HEIGHT;
	write_cmd(0x2B);	//X Address Setup
    write_data((x & 0xff00) >> 8);
    write_data(x & 0x00ff);

    write_cmd(0x2A);	//Y Address Setup
    write_data((y & 0xff00) >> 8);
    write_data((y & 0x00ff));
	
	write_cmd(0x3C);	//Write Display Data
}

//设置颜色
void LCD_SetColor(uint16_t bkgclor, uint16_t txtclor)
{
    BkgColor = bkgclor;
    TxtColor = txtclor;
}
//清屏
void LCD_Clear(void)
{
	LCD_FillRect(0,0,LCD_WIDTH+1,LCD_HEIGHT+1,BLACK);
}
//画点
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	if(x > LCD_WIDTH)x = LCD_WIDTH;
	if(y > LCD_HEIGHT)y = LCD_HEIGHT;
	write_cmd(0x2B);	//X Address Setup
    write_data((x&0xff00) >> 8);
    write_data(x&0x00ff);
	write_data((x&0xff00) >> 8);
    write_data(x&0x00ff);

    write_cmd(0x2A);	//Y Address Setup
    write_data((y&0xff00) >> 8);
    write_data((y&0x00ff));
	write_data((y&0xff00) >> 8);
    write_data((y&0x00ff));

    write_cmd(0x2C);	//Write Display Data
	write_data(color);
}
//读点
u16 LCD_ReadPoint(u16 x,u16 y)
{
	u16 dR,dG,dB,dRGB;
	u16 rd1=0,rd2=0;
	if(x > LCD_WIDTH)x = LCD_WIDTH;
	if(y > LCD_HEIGHT)y = LCD_HEIGHT;
	SetReadArea(x, x, y, y);
	read_data();
	rd1 = read_data();
	rd2 = read_data();
	dR = (rd1&0xF800);
	dG = (rd1&0xFC)<<3;
	dB = (rd2&0xF800)>>11;
	dRGB = dR+dG+dB;
	return dRGB;
}
//画线
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{	
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol; 
	if(x1>LCD_WIDTH)x1=LCD_WIDTH;
	if(x2>LCD_WIDTH)x2=LCD_WIDTH;
	if(y1 > LCD_HEIGHT)y1 = LCD_HEIGHT;
	if(y2 > LCD_HEIGHT)y2 = LCD_HEIGHT;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向
	else if(delta_x==0)incx=0;//垂直线
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ ){//画线输出	  
		LCD_DrawPoint(uRow,uCol,color);//画点 			
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance){ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance){ 
			yerr-=distance;
			uCol+=incy; 
		}
	}
}
//画虚线
void LCD_DrawLineDotted(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 i;
	u16 xt1,yt1,xt2,yt2;
	float xf1,yf1,xf2,yf2;
	float xStepLen;
	float yStepLen;
	float stepCntf;
	u16 stepCnt;
	xt1 = x1;
	yt1 = y1;
	xt2 = x2;
	yt2 = y2;
	if(x1 > x2){
		xt1 = x2;
		xt2 = x1;
	}
	if(y1 > y2){
		yt1 = y2;
		yt2 = y1;
	}
	if((xt2-xt1) >= (yt2-yt1)){
		xStepLen = 6.0f;
		stepCntf = (float)(xt2-xt1)/xStepLen;
		yStepLen = (float)(yt2-yt1)/stepCntf;
		stepCnt = (u16)(stepCntf + 0.5);
	}else{
		yStepLen = 6.0f;
		stepCntf = (float)(yt2-yt1)/yStepLen;
		xStepLen = (float)(xt2-xt1)/stepCntf;
		stepCnt = (u16)(stepCntf + 0.5);
	}
	
	xf1 = xt1;
	yf1 = yt1;
	for(i=0;i<stepCnt;i++){
		xf2 = xf1 + xStepLen/2.0f;
		yf2 = yf1 + yStepLen/2.0f;
		if(xf1 > xt2)break;
		if(yf1 > yt2)break;
		if(xf2 > xt2)xf2 = xt2;
		if(yf2 > yt2)yf2 = yt2;
		LCD_DrawLine(xf1,yf1,xf2,yf2,color);
		
		xf1 += xStepLen;
		yf1 += yStepLen;
	}
}
//画圆
void LCD_DrawCircle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	int di;

	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	//地址保护
	if(x0<r || y0<r || x0+r>LCD_WIDTH || y0+r>LCD_HEIGHT){
		return;
	}
	while(a<=b)
	{
//		Sleep_ms(5);
		LCD_DrawPoint(x0+a,y0-b,color);             //5
 		LCD_DrawPoint(x0+b,y0-a,color);             //0
		LCD_DrawPoint(x0+b,y0+a,color);             //4
		LCD_DrawPoint(x0+a,y0+b,color);             //6
		LCD_DrawPoint(x0-a,y0+b,color);             //1
 		LCD_DrawPoint(x0-b,y0+a,color);             //3
		LCD_DrawPoint(x0-a,y0-b,color);             //2
  		LCD_DrawPoint(x0-b,y0-a,color);             //7
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		}
	}
}
//画矩形
void LCD_DrawRect(u16 x0,u16 y0,u16 xLen,u16 yLen,u16 color)
{
	u16 x1 = x0 + xLen - 1;
	u16 y1 = y0 + yLen - 1;
	if(x1 > LCD_WIDTH)x1 = LCD_WIDTH;
	if(y1 > LCD_HEIGHT)y1 = LCD_HEIGHT;
	LCD_DrawLine(x0,y0,x1,y0,color);
	LCD_DrawLine(x0,y0,x0,y1,color);
	LCD_DrawLine(x0,y1,x1,y1,color);
	LCD_DrawLine(x1,y0,x1,y1,color);
}

//显示指定颜色，指定宽度和高度的方块
//void LCD_FillRect(int16_t x, int16_t y, int16_t xlen, int16_t ylen, u16 color)
//{
//	u16 i,j;
//	u16 x1,y1;
//	x1 = x+xlen-1;
//	y1 = y+ylen-1;
//	if(y > LCD_HEIGHT) return;
//    if(y1 > LCD_HEIGHT) y1 = LCD_HEIGHT;
//    if(x > LCD_WIDTH) return;
//    if(x1 > LCD_WIDTH) x1 = LCD_WIDTH;
//	for(i=x;i<=x1;i++){
//		for(j=y;j<=y1;j++){
//			LCD_DrawPoint(i,j,color);
//		}
//	}
//}
void LCD_FillRect(int16_t x, int16_t y, int16_t xlen, int16_t ylen, u16 color)
{
	short x1,y1;
    int i,alllenth;
	
	x1 = x+xlen-1;
	y1 = y+ylen-1;
	if(x < 0) x = 0;
	if(x1 < 0) return;
	if(x1 < x) return;
	if(y < 0) y = 0;
	if(y1 < 0) return;
	if(y1 < y) return;
    if(y > LCD_HEIGHT) return;
    if(y1 > LCD_HEIGHT) y1 = LCD_HEIGHT;
    if(x > LCD_WIDTH) return;
    if(x1 > LCD_WIDTH) x1 = LCD_WIDTH;

	SetDispArea(x, x1, y, y1);//显示范围

	alllenth = xlen*ylen;
	
	while(alllenth > 7){
		*(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
		alllenth -= 8;
	}
	for(i=0;i<alllenth;i++){
		*(__IO uint16_t *) (Bank1_LCD_D) = color; 
	}
}

//读矩形数据
u16* LCD_ReadRect(u16 x0,u16 y0,u16 width,u16 height)
{
	u32 i=0;
	u32 readLen;
	u16 x1,y1;
	u16 rd1=0,rd2=0,rd3=0;
	u16 dR,dG,dB,dRGB;
	u16 *pReadBuf = NULL;
	x1 = x0+width-1;
	y1 = y0+height-1;
	if(x0 > LCD_WIDTH) return NULL;
	if(x1 > LCD_WIDTH) x1 = LCD_WIDTH;
	if(y0 > LCD_HEIGHT) return NULL;
	if(y1 > LCD_HEIGHT) y1 = LCD_HEIGHT;
	readLen = (x1-x0+1)*(y1-y0+1);
	pReadBuf = (u16*)mymalloc(SRAMIN,readLen*2);
//	pReadBuf = (u16*)malloc(readLen*2);
	SetReadArea(x0, x1, y0, y1);
	read_data();
	while(i<readLen){
		rd1 = read_data();
		rd2 = read_data();
		rd3 = read_data();
		dR = (rd1&0xF800);
		dG = (rd1&0xFC)<<3;
		dB = (rd2&0xF800)>>11;
		dRGB = dR+dG+dB;
//		g_u16LcdReadBuf[i] = dRGB;
		pReadBuf[i] = dRGB;
		dR = (rd2&0xF8)<<8;
		dG = (rd3&0xFC00)>>5;
		dB = (rd3&0xF8)>>3;
		dRGB = dR+dG+dB;
//		g_u16LcdReadBuf[i+1] = dRGB;
		pReadBuf[i+1] = dRGB;
		i+=2;
	}
	return pReadBuf;
}

void LCD_ChangeRectColor(u16 sx,u16 ex,u16 sy,u16 ey,u16 oldColor,u16 newColor)
{
	u16 i,j;
	for(i=sx;i<=ex;i++){
		for(j=sy;j<=ey;j++){
			if(LCD_ReadPoint(i,j) == oldColor)LCD_DrawPoint(i,j,newColor);
		}
	}
}
////从内存读取图片
//u16* LCD_ReadBMP(char* PicName,u32 PicSize)
//{	
//	FRESULT result;
//	u16 *bmp_addr = NULL;
//	u32 size = PicSize;
//	char g_FileName[32];
//	char bmp[3];
//	//BMP_PICTURE BMP;	//bmp文件头 兼容解析图片，为了效率不做此兼容

//	sprintf(g_FileName,"0:/BMP/%s.bmp",PicName); 
//	result = f_open(file,g_FileName, FA_READ); //判断文件是否存在
//	if(result != FR_OK){//写入数据信息
//		sprintf(g_FileName,"0:/Backup/%s.bmp",PicName); 
//		result = f_open(file,g_FileName, FA_READ); //判断文件是否存在
//		if(result != FR_OK){
//			return NULL;  //未找到文件
//		}
//	}
//	result = f_read(file,bmp, 3, &br);

//	if((bmp[0] != 0x42)||(bmp[1] != 0x4D)){  //BMP 文件格式
//		f_close(file);
//		return NULL;
//	}
//	//为位图信息申请空间
//	bmp_addr = mymalloc(SRAMIN,size);
//	memset(bmp_addr,0,size);

//	f_lseek(file,70);//指向位图数据开始位置
//	f_read(file, bmp_addr ,size, &br);//读取全部像素信息

//	f_close(file);
//	return bmp_addr;
//}


//从内存读取图片并显示
//#define PIXBUFLEN	2048
u8 LCD_ShowBMP(u16 x0,u16 y0,u16 width,u16 height,char* PicName)
{	
	u16 x1,y1;
	u16 xLen,yLen;
	u32 picLen,i,j,k;
	
	char bmp[3];
//	u8 bmpPixBuf[PIXBUFLEN*2];
	u16 pixData;
	char g_FileName[32];	
	FRESULT result;
	
	u32 readSize;
	u8* bmpBufAddr = NULL;
	
	x1 = x0+width-1;
	y1 = y0+height-1;
	if(x0 > LCD_WIDTH) return 0;
	if(x1 > LCD_WIDTH) x1 = LCD_WIDTH;
	if(y0 > LCD_HEIGHT) return 0;
	if(y1 > LCD_HEIGHT) y1 = LCD_HEIGHT;
	xLen = x1-x0+1;
	yLen = y1-y0+1;
	picLen = xLen*yLen;

	sprintf(g_FileName,"0:/BMP/%s.bmp",PicName); 
	result = f_open(file,g_FileName, FA_READ); //判断文件是否存在
	if(result != FR_OK){//写入数据信息
		sprintf(g_FileName,"0:/Backup/%s.bmp",PicName); 
		result = f_open(file,g_FileName, FA_READ); //判断文件是否存在
		if(result != FR_OK){
			return 0;  //未找到文件
		}
	}
	result = f_read(file,bmp, 3, &br);

	if((bmp[0] != 0x42)||(bmp[1] != 0x4D)){  //BMP 文件格式
		f_close(file);
		return 0;
	}
	
	readSize = 5*1024;
	bmpBufAddr = mymalloc(SRAMIN,readSize*2);
	memset(bmpBufAddr,0,readSize*2);
	k = picLen/readSize;
	if(picLen%readSize != 0) k += 1;
	SetDispArea(x0, x1, y0, y1);
	for(i=0;i<k;i++){
		f_lseek(file,70+i*readSize*2);//指向位图数据开始位置
		f_read(file,bmpBufAddr,readSize*2,&br);//读取要显示的像素信息
		for(j=0;j<readSize;j++){
			pixData = bmpBufAddr[j*2]+(bmpBufAddr[j*2+1]<<8);
			write_data(pixData);
			if(j*2 >= br)break;
		}
	}
	myfree(SRAMIN,bmpBufAddr);
//	k = picLen/PIXBUFLEN;
//	SetDispArea(x0, x1, y0, y1);
//	for(i=0;i<k;i++){
//		f_lseek(file,70+i*PIXBUFLEN*2);//指向位图数据开始位置
//		f_read(file,bmpPixBuf,PIXBUFLEN*2,&br);//读取要显示的像素信息
//		for(j=0;j<PIXBUFLEN;j++){
//			pixData = bmpPixBuf[j*2]+(bmpPixBuf[j*2+1]<<8);
//			write_data(pixData);
//			if(j*2 >= br)break;
//		}
//	}
//	for(i=0;i<picLen;i++){
//		f_lseek(file,70+i*2);//指向位图数据开始位置
//		f_read(file,bmpPix,2,&br);//读取要显示的像素信息
//		pixData = bmpPix[0]+(bmpPix[1]<<8);
//		write_data(pixData);
//	}
	
	f_close(file);
	return 1;
}
//显示图片
void LCD_ShowPicture(u16 x0,u16 y0,u16 width,u16 height,const u16* p)
{
	u16 x1,y1;
	u16 xLen,yLen;
	u32 picLen,i;
	x1 = x0+width-1;
	y1 = y0+height-1;
	if(x0 > LCD_WIDTH) return;
	if(x1 > LCD_WIDTH) x1 = LCD_WIDTH;
	if(y0 > LCD_HEIGHT) return;
	if(y1 > LCD_HEIGHT) y1 = LCD_HEIGHT;
	xLen = x1-x0+1;
	yLen = y1-y0+1;
	picLen = xLen*yLen;
	SetDispArea(x0, x1, y0, y1);
	for(i=0;i<picLen;i++){
		write_data(p[i]);
	}
}
//显示字模,显示背景
void LCD_ShowZimo_BK(u16 x,u16 y,u16 width,u16 height,u8* zimo)
{
	u16 i,j;
	u16 y0=y;
	u8 temp;
	
	u8 spareBit;											//字符取模多出的点数，对于字符大小不是8的倍数的字模
	u16 udrawBit;											//未显示的点数，用于超出屏显区域处理
	
	u16 coluSize = (height/8+((height%8)?1:0));				//字符一列对应字节数
	u16 fontSize = coluSize*width;						//字符对应点阵集所占的字节数
	
	u16 x1 = x+width-1;
	u16 y1 = y+height-1;
	
	SetDispArea(x, x1, y, y1);//设置显示区域
	
	for(i=0;i<fontSize;i++) {
		
		temp = zimo[i];
		
		for(j=0;j<8;j++) {			    
			if(temp&0x80)write_data(TxtColor);//LCD_DrawPoint(x,y,TxtColor);
			else write_data(BkgColor);//LCD_DrawPoint(x,y,BkgColor);//显示背景
			temp<<=1;
			y++;
			if(y>LCD_HEIGHT){		//超区域了
				if((y-y0)==height){
					y=y0;
					x++; if(x>LCD_WIDTH)return;	//超区域了
					break;
				}
				if((y-y0)<height){	//字符未写完
					spareBit = coluSize*8-height;
					udrawBit = height+spareBit-(y-y0);
					if(udrawBit<8){
						y=y0; x++; if(x>LCD_WIDTH)return;	//超区域了
						break;
					}else{
						i+=udrawBit/8;
						y=y0; x++; if(x>LCD_WIDTH)return;	//超区域了
						break;
					}
				}
				break;
			}
			if((y-y0)==height){
				y=y0;
				x++; if(x>LCD_WIDTH)return;	//超区域了
				break;
			}
		}
	}
}
//显示字模,不显示背景
void LCD_ShowZimo_NBK(u16 x,u16 y,u16 width,u16 height,u8* zimo)
{
	u16 i,j;
	u16 y0=y;
	u8 temp;
	
	u8 spareBit;											//字符取模多出的点数，对于字符大小不是8的倍数的字模
	u16 udrawBit;											//未显示的点数，用于超出屏显区域处理
	
	u16 coluSize = (height/8+((height%8)?1:0));				//字符一列对应字节数
	u16 fontSize = coluSize*width;						//字符对应点阵集所占的字节数
	
	for(i=0;i<fontSize;i++) {
		
		temp = zimo[i];
		
		for(j=0;j<8;j++) {			    
			if(temp&0x80)LCD_DrawPoint(x,y,TxtColor);//LCD_DrawPoint(x,y,TxtColor);//
			temp<<=1;
			y++;
			if(y>LCD_HEIGHT){		//超区域了
				if((y-y0)==height){
					y=y0;
					x++; if(x>LCD_WIDTH)return;	//超区域了
					break;
				}
				if((y-y0)<height){	//字符未写完
					spareBit = coluSize*8-height;
					udrawBit = height+spareBit-(y-y0);
					if(udrawBit<8){
						y=y0; x++; if(x>LCD_WIDTH)return;	//超区域了
						break;
					}else{
						i+=udrawBit/8;
						y=y0; x++; if(x>LCD_WIDTH)return;	//超区域了
						break;
					}
				}
				break;
			}
			if((y-y0)==height){
				y=y0;
				x++; if(x>LCD_WIDTH)return;	//超区域了
				break;
			}
		}
	}
}
void LCD_ShowZimo(u16 x,u16 y,u16 width,u16 height,u8 mode,u8* zimo)
{
	if(mode == 0){
		LCD_ShowZimo_BK(x,y,width,height,zimo);
	}else{
		LCD_ShowZimo_NBK(x,y,width,height,zimo);
	}
}
//显示字符,显示背景
void LCD_ShowASC(u8 ucFont,u16 x,u16 y,u8 num)
{
	u16 i,j;
	u16 y0=y;
	u8 temp;
	
	u8 spareBit;											//字符取模多出的点数，对于字符大小不是8的倍数的字模
	u16 udrawBit;											//未显示的点数，用于超出屏显区域处理
	
	u16 coluSize = (ucFont/8+((ucFont%8)?1:0));				//字符一列对应字节数
	u16 fontSize = coluSize*(ucFont/2);						//字符对应点阵集所占的字节数
	
	u16 x1 = x+ucFont/2-1;
	u16 y1 = y+ucFont-1;
	
	SetDispArea(x, x1, y, y1);//设置显示区域
	
	num=num-' ';											//得到偏移后的值即为字库偏移量
	if(num > '~')num = '~';
	for(i=0;i<fontSize;i++) {
		if(ucFont==ASC12x6)temp = ASC2_1206[num][i]; 	 	//调用1206字体
		else if(ucFont==ASC16x8)temp = ASC2_1608[num][i];	//调用1608字体
		else if(ucFont==ASC20x10)temp = ASC2_2010[num][i];	//调用2010字体
		else if(ucFont==ASC24x12)temp = ASC2_2412[num][i];	//调用2412字体
		else if(ucFont==ASC32x16)temp = ASC2_3216[num][i];	//调用2814字体
		else if(ucFont==ASC40x20)temp = ASC2_4020[num][i];	//调用3216字体
		else return;								//没有的字库		
		for(j=0;j<8;j++) {			    
			if(temp&0x80)write_data(TxtColor);//LCD_DrawPoint(x,y,TxtColor);
			else write_data(BkgColor);//LCD_DrawPoint(x,y,BkgColor);//显示背景
			temp<<=1;
			y++;
			if(y>LCD_HEIGHT){		//超区域了
				if((y-y0)==ucFont){
					y=y0;
					x++; if(x>LCD_WIDTH)return;	//超区域了
					break;
				}
				if((y-y0)<ucFont){	//字符未写完
					spareBit = coluSize*8-ucFont;
					udrawBit = ucFont+spareBit-(y-y0);
					if(udrawBit<8){
						y=y0; x++; if(x>LCD_WIDTH)return;	//超区域了
						break;
					}else{
						i+=udrawBit/8;
						y=y0; x++; if(x>LCD_WIDTH)return;	//超区域了
						break;
					}
				}
				break;
			}
			if((y-y0)==ucFont){
				y=y0;
				x++; if(x>LCD_WIDTH)return;	//超区域了
				break;
			}
		}
	}
}
//显示字符,不显示背景
void LCD_ShowASC_NBK(u8 ucFont,u16 x,u16 y,u8 num)
{
	u16 i,j;
	u16 y0=y;
	u8 temp;
	
	u8 spareBit;											//字符取模多出的点数，对于字符大小不是8的倍数的字模
	u16 udrawBit;											//未显示的点数，用于超出屏显区域处理
	
	u16 coluSize = (ucFont/8+((ucFont%8)?1:0));				//字符一列对应字节数
	u16 fontSize = coluSize*(ucFont/2);						//字符对应点阵集所占的字节数

	num=num-' ';											//得到偏移后的值即为字库偏移量
	if(num > '~')num = '~';
	for(i=0;i<fontSize;i++) {
		if(ucFont==ASC12x6)temp = ASC2_1206[num][i]; 	 	//调用1206字体
		else if(ucFont==ASC16x8)temp = ASC2_1608[num][i];	//调用1608字体
		else if(ucFont==ASC20x10)temp = ASC2_2010[num][i];	//调用2010字体
		else if(ucFont==ASC24x12)temp = ASC2_2412[num][i];	//调用2412字体
		else if(ucFont==ASC32x16)temp = ASC2_3216[num][i];	//调用2814字体
		else if(ucFont==ASC40x20)temp = ASC2_4020[num][i];	//调用3216字体
		else return;								//没有的字库		
		for(j=0;j<8;j++) {			    
			if(temp&0x80)LCD_DrawPoint(x,y,TxtColor);//LCD_DrawPoint(x,y,TxtColor);//
			temp<<=1;
			y++;
			if(y>LCD_HEIGHT){		//超区域了
				if((y-y0)==ucFont){
					y=y0;
					x++; if(x>LCD_WIDTH)return;	//超区域了
					break;
				}
				if((y-y0)<ucFont){	//字符未写完
					spareBit = coluSize*8-ucFont;
					udrawBit = ucFont+spareBit-(y-y0);
					if(udrawBit<8){
						y=y0; x++; if(x>LCD_WIDTH)return;	//超区域了
						break;
					}else{
						i+=udrawBit/8;
						y=y0; x++; if(x>LCD_WIDTH)return;	//超区域了
						break;
					}
				}
				break;
			}
			if((y-y0)==ucFont){
				y=y0;
				x++; if(x>LCD_WIDTH)return;	//超区域了
				break;
			}
		}
	}
}

//显示字符串
void LCD_ShowString(u8 ucFont,u16 x,u16 y,u8 mode,char *iput)
{
    while((*iput<='~')&&(*iput>=' '))//判断是不是非法字符!
    {
		if(mode == 0)LCD_ShowASC(ucFont,x,y,*iput);
		else LCD_ShowASC_NBK(ucFont,x,y,*iput);
//		if(*iput == '.')x+=(ucFont+1)/4;
//        else x+=(ucFont+1)/2;
		x+=(ucFont+1)/2;
        iput++;
    }
}

//显示一位数字
void LCD_ShowNum(u8 ucFont,u16 x,u16 y,u8 num)
{    
	u16 i,j;
	u16 y0=y;
	u8 temp;
	
	u8 spareBit;											//字符取模多出的点数，对于字符大小不是8的倍数的字模
	u16 udrawBit;											//未显示的点数，用于超出屏显区域处理
	
	u16 coluSize = (ucFont/8+((ucFont%8)?1:0));				//字符一列对应字节数
	u16 fontSize = coluSize*(ucFont/2);						//字符对应点阵集所占的字节数
		
	u16 x1 = x+ucFont/2-1;
	u16 y1 = y+ucFont-1;
	
	SetDispArea(x, x1, y, y1);//设置显示区域
	
	if(num == '.'){
		num = 10;//浮点数特殊情况
		fontSize /= 2;
	}else if(num == '-'){
		num = 11;//空格特殊情况
	}else if(num == ' '){
		num = 12;//空格特殊情况
	}else if(num == 'o'){
		if(ucFont==NUM80x40){
			num = 13;//o
		}
	}else if(num == 'L'){
		if(ucFont==NUM80x40){
			num = 14;//L
		}
	}else num=num-'0';	
	if(num > 12){
		if(ucFont==NUM80x40){
			if(num > 14){
				num = 12;
			}
		}else {
			num = 12;
		}
	}
	if(ucFont==NUM96x48){
		if(num > 6)return;
	}
//	if(num < 0)num = 0;	//得到偏移后的值即为字库偏移量
	
	for(i=0;i<fontSize;i++) {
		if(ucFont==NUM12x6)temp = NUM_1206[num][i];	//调用1608字体
		else if(ucFont==NUM16x8)temp = NUM_1608[num][i];	//调用1608字体
		else if(ucFont==NUM20x10)temp = NUM_2010[num][i];	//调用2010字体
		else if(ucFont==NUM24x12)temp = NUM_2412[num][i];	//调用2412字体
		else if(ucFont==NUM32x16)temp = NUM_3216[num][i];	//调用2412字体
		else if(ucFont==NUM40x20)temp = NUM_4020[num][i];	//调用3216字体
		else if(ucFont==NUM96x48)temp = NUM_9648[num][i];	//调用3216字体
		else return;								//没有的字库		
		for(j=0;j<8;j++) {			    
			if(temp&0x80)write_data(TxtColor);//LCD_DrawPoint(x,y,TxtColor);
			else write_data(BkgColor);//LCD_DrawPoint(x,y,BkgColor);//显示背景
			temp<<=1;
			y++;
			if(y>LCD_HEIGHT){		//超区域了
				if((y-y0)==ucFont){
					y=y0;
					x++; if(x>LCD_WIDTH)return;	//超区域了
					break;
				}
				if((y-y0)<ucFont){	//字符未写完
					spareBit = coluSize*8-ucFont;
					udrawBit = ucFont+spareBit-(y-y0);
					if(udrawBit<8){
						y=y0; x++; if(x>LCD_WIDTH)return;	//超区域了
						break;
					}else{						
						i+=udrawBit/8;
						y=y0; x++; if(x>LCD_WIDTH)return;	//超区域了
						break;
					}
				}
				break;
			}
			if((y-y0)==ucFont){
				y=y0;
				x++; if(x>LCD_WIDTH)return;	//超区域了
				break;
			}
		}
	}
}

//显示一位数字,不显示背景
void LCD_ShowNum_NBK(u8 ucFont,u16 x,u16 y,u8 num)
{    
	u16 i,j;   
	u16 y0=y;
	u8 temp;
	
	u8 spareBit;											//字符取模多出的点数，对于字符大小不是8的倍数的字模
	u16 udrawBit;											//未显示的点数，用于超出屏显区域处理
	
	u16 coluSize = (ucFont/8+((ucFont%8)?1:0));				//字符一列对应字节数
	u16 fontSize = coluSize*(ucFont/2);						//字符对应点阵集所占的字节数

	if(num == '.'){
		num = 10;//浮点数特殊情况
		fontSize /= 2;
	}else if(num == '-'){
		num = 11;//空格特殊情况
	}else if(num == ' '){
		num = 12;//空格特殊情况
	}else if(num == 'o'){
		if(ucFont==NUM80x40){
			num = 13;//o
		}
	}else if(num == 'L'){
		if(ucFont==NUM80x40){
			num = 14;//L
		}
	}else num=num-'0';	
	if(num > 12){
		if(ucFont==NUM80x40){
			if(num > 14){
				num = 12;
			}
		}else {
			num = 12;
		}
	}
	if(ucFont==NUM96x48){
		if(num > 6)return;
	}
//	if(num < 0)num = 0;	//得到偏移后的值即为字库偏移量
	
	for(i=0;i<fontSize;i++) {
		if(ucFont==NUM12x6)temp = NUM_1206[num][i];	//调用1608字体
		else if(ucFont==NUM16x8)temp = NUM_1608[num][i];	//调用1608字体
		else if(ucFont==NUM20x10)temp = NUM_2010[num][i];	//调用2010字体
		else if(ucFont==NUM24x12)temp = NUM_2412[num][i];	//调用2412字体
		else if(ucFont==NUM32x16)temp = NUM_3216[num][i];	//调用2412字体
		else if(ucFont==NUM40x20)temp = NUM_4020[num][i];	//调用3216字体
		else if(ucFont==NUM96x48)temp = NUM_9648[num][i];	//调用3216字体
		else return;								//没有的字库		
		for(j=0;j<8;j++) {			    
			if(temp&0x80)LCD_DrawPoint(x,y,TxtColor);//write_data(TxtColor);
			temp<<=1;
			y++;
			if(y>LCD_HEIGHT){		//超区域了
				if((y-y0)==ucFont){
					y=y0;
					x++; if(x>LCD_WIDTH)return;	//超区域了
					break;
				}
				if((y-y0)<ucFont){	//字符未写完
					spareBit = coluSize*8-ucFont;
					udrawBit = ucFont+spareBit-(y-y0);
					if(udrawBit<8){
						y=y0; x++; if(x>LCD_WIDTH)return;	//超区域了
						break;
					}else{						
						i+=udrawBit/8;
						y=y0; x++; if(x>LCD_WIDTH)return;	//超区域了
						break;
					}
				}
				break;
			}
			if((y-y0)==ucFont){
				y=y0;
				x++; if(x>LCD_WIDTH)return;	//超区域了
				break;
			}
		}
	}
}

//显示多位数字
void LCD_ShowNums(u8 ucFont,u16 x,u16 y,u8 mode,int num)
{    
	int n = 0;
	char numStr[10] = {0};

	sprintf(numStr,"%d",num);
	while(numStr[n] != '\0'){
		if(mode == 0)LCD_ShowNum(ucFont,x,y,numStr[n]);
		else LCD_ShowNum_NBK(ucFont,x,y,numStr[n]);
		//字间隔
		x += ucFont/2;
		n += 1;
	}
}

//居中显示数字
void LCD_ShowNums_Center(u8 ucFont,u16 x0,u16 y0,u16 width,u16 height,u8 mode,int num)
{
	u16 x,y,temp;
	u8 numLen,numWid;
	u16 fillWidX1,fillWidY1,fillWidX2,fillWidY2;
	
	char numStr[10] = {0};

	sprintf(numStr,"%d",num);
	
	numLen = strlen(numStr);
	numWid = numLen*ucFont/2;
	
	if(width < numWid){
		x = x0;
		fillWidX1 = 0;
		fillWidX2 = 0;
	}else{
		temp = width - numWid;
		x = x0 + temp/2;
		fillWidX1 = temp/2;
		if(temp%2 == 0){
			fillWidX2 = fillWidX1;
		}else{
			fillWidX2 = fillWidX1 + 1;
		}
	}
	if(height < ucFont){
		y = y0;
		fillWidY1 = 0;
		fillWidY2 = 0;
	}else{
		temp = height - ucFont;
		y = y0 + temp/2;
		fillWidY1 = temp/2;
		if(temp%2 == 0){
			fillWidY2 = fillWidY1;
		}else{
			fillWidY2 = fillWidY1+1;
		}
	}
	LCD_ShowNums(ucFont,x,y,mode,num);
	if(mode == 0){
		LCD_FillRect(x0,y0,width,fillWidY1,BkgColor);
		LCD_FillRect(x0,y0+fillWidY1+ucFont,width,fillWidY2,BkgColor);
		LCD_FillRect(x0,y0+fillWidY1,fillWidX1,ucFont,BkgColor);
		LCD_FillRect(x0+fillWidX1+numWid,y0+fillWidY1,fillWidX2,ucFont,BkgColor);
	}
}

//显示指定位数字
void LCD_ShowNnNums(u8 ucFont,u16 x,u16 y,u8 mode,u8 digit,int num)
{
	int n = 0;
	char numStr[10] = {0};
	if(digit != 0){
		sprintf(numStr,"%*d",digit,num);
		numStr[digit] = '\0';
	}
	else sprintf(numStr,"%d",num);
	while(numStr[n] != '\0'){
		if(numStr[n] == ' ')numStr[n] = ' ';
		if(mode == 0)LCD_ShowNum(ucFont,x,y,numStr[n]);
		else LCD_ShowNum_NBK(ucFont,x,y,numStr[n]);
		//字间隔
		x += ucFont/2;
		n += 1;
	}
}
//显示浮点数
void LCD_ShowFloat(u8 ucFont,u16 x,u16 y,u8 mode,u8 numLen,u8 decimal,float fnum)
{    
	int n = 0;
	char numStr[10] = {0};
	sprintf(numStr,"%*.*f",numLen,decimal,fnum);//
	numStr[6] = '\0';//只保留前6位
	while(numStr[n] != '\0'){
		if(mode == 0)LCD_ShowNum(ucFont,x,y,numStr[n]);
		else LCD_ShowNum_NBK(ucFont,x,y,numStr[n]);
		//字间隔
		if(numStr[n] == '.') x += ucFont/4;
		else x += ucFont/2;
		n += 1;
	}
}
//显示汉字,显示背景
static void LCD_ShowCh(u8 ucFont,u16 chx,u16 chy,uc8* iput)
{  	
	u8 temp;
	u16 i,j;
	u16 y0=chy;
	
	u8 spareBit;											//字符取模多出的点数，对于字符大小不是8的倍数的字模
	u8 udrawbit;											//未显示的点数，用于超出屏显区域处理

	u16 coluSize = (ucFont/8+((ucFont%8)?1:0));				//字符一列对应字节数
	u16 fontSize = coluSize*ucFont;							//字符对应点阵集所占的字节数
	
	u16 x1 = chx+ucFont-1;
	u16 y1 = chy+ucFont-1;
	
	SetDispArea(chx, x1, chy, y1);

	for(i=0;i<fontSize;i++) {
		temp = *(iput+i);
		for(j=0;j<8;j++) {
			if(temp&0x80)write_data(TxtColor);//LCD_DrawPoint(chx,chy,TxtColor);//
			else write_data(BkgColor);//LCD_DrawPoint(x,y,BkgColor);//显示背景
			temp<<=1;
			chy++;
			if(chy>LCD_HEIGHT){		//超区域了
				if((chy-y0)==ucFont){	//字符第chx列写完
					chy=y0;
					chx++; if(chx>LCD_WIDTH)return;	//超区域了
					break;
				}
				if((chy-y0)<ucFont){	//字符第chx列未写完
					spareBit = coluSize*8-ucFont;
					udrawbit = ucFont+spareBit-(chy-y0);
					if(udrawbit<8){
						chy=y0; chx++; if(chx>LCD_WIDTH)return;	//超区域了
						break;
					}else{						
						i+=udrawbit/8;
						chy=y0; chx++; if(chx>LCD_WIDTH)return;	//超区域了
						break;
					}
				}
				break;
			}
			if((chy-y0)==ucFont){
				chy=y0;
				chx++; if(chx>LCD_WIDTH)return;	//超区域了
				break;
			}
		}
	}	
}
//显示汉字,不显示背景
static void LCD_ShowCh_NBK(u8 ucFont,u16 chx,u16 chy,uc8* iput)
{  	
	u8 temp;
	u16 i,j;
	u16 y0=chy;
	
	u8 spareBit;											//字符取模多出的点数，对于字符大小不是8的倍数的字模
	u8 udrawbit;											//未显示的点数，用于超出屏显区域处理

	u16 coluSize = (ucFont/8+((ucFont%8)?1:0));				//字符一列对应字节数
	u16 fontSize = coluSize*ucFont;							//字符对应点阵集所占的字节数

	for(i=0;i<fontSize;i++) {
		temp = *(iput+i);
		for(j=0;j<8;j++) {
			if(temp&0x80)LCD_DrawPoint(chx,chy,TxtColor);//write_data(TxtColor);//
			temp<<=1;
			chy++;
			if(chy>LCD_HEIGHT){		//超区域了
				if((chy-y0)==ucFont){	//字符第chx列写完
					chy=y0;
					chx++; if(chx>LCD_WIDTH)return;	//超区域了
					break;
				}
				if((chy-y0)<ucFont){	//字符第chx列未写完
					spareBit = coluSize*8-ucFont;
					udrawbit = ucFont+spareBit-(chy-y0);
					if(udrawbit<8){
						chy=y0; chx++; if(chx>LCD_WIDTH)return;	//超区域了
						break;
					}else{						
						i+=udrawbit/8;
						chy=y0; chx++; if(chx>LCD_WIDTH)return;	//超区域了
						break;
					}
				}
				break;
			}
			if((chy-y0)==ucFont){
				chy=y0;
				chx++; if(chx>LCD_WIDTH)return;	//超区域了
				break;
			}
		}
	}	
}
//显示汉字串
void LCD_ShowChinese(u8 ucFont,u16 x,u16 y,u8 mode,u8 *iput)
{   
	u8 i=0;
	uc8 *buf;
	char endChar[3];
	sprintf(endChar,"呕"); 	//汉字库结束字符
	
    while(*iput!='\0')		//判断字符结束符
    {       
		while(1){
			if(ucFont == Font8){
				buf = &GB_8[i].Index[0];
			}
			if(ucFont == Font12){
				buf = &GB_12[i].Index[0];
			}
			if(ucFont == Font14){
				buf = &GB_14[i].Index[0];
			}
			if(ucFont == Font16){
				buf = &GB_16[i].Index[0];
			}
			if(ucFont == Font20){
				buf = &GB_20[i].Index[0];
			}
			if(ucFont == Font24){
				buf = &GB_24[i].Index[0];
			}
			if(ucFont == Font32){
				buf = &GB_32[i].Index[0];
			}
//			if(ucFont == Font36){
//				buf = &GB_36[i].Index[0];
//			}
//			if(ucFont == Font40){
//				buf = &GB_40[i].Index[0];
//			}
//			if(ucFont == Font48){
//				buf = &GB_48[i].Index[0];
//			}
//			if(ucFont == Font64){
//				buf = &GB_64[i].Index[0];
//			}
			
			//增加不同大小汉字字模
				
			if((*buf == endChar[0]) && (*(buf+1) == endChar[1])){ //判断是否为结束字符 “呕”
				i=0;
				break;
            }
            if((*buf == 0xFF) || (*(buf+1) == 0xFF)){ //判断是否为结束字符  Flash 存储器有异常，退出。
                i=0;
				break;
            }
			if((*iput == *buf) && (*(iput+1) == *(buf+1))){
				if(mode == 0)LCD_ShowCh(ucFont,x,y,buf+3);
				else LCD_ShowCh_NBK(ucFont,x,y,buf+3);
				i=0;
				break;
			}
			i++;
		}
		x+=ucFont;
        iput+=2;
    }
}

//显示汉字串
void LCD_ShowText(u8 ucFont,u16 x,u16 y,u8 mode,u8 *iput)
{
	u8 i=0;
	uc8 *buf;
	char endChar[3];
	sprintf(endChar,"呕"); 	//汉字库结束字符
	
    while(*iput!='\0')		//判断字符结束符
    {
		if((*iput&0x80) == 0){//英文
			if((*iput<='~')&&(*iput>=' ')){//判断是不是非法字符!			
				if(mode == 0)LCD_ShowASC(ucFont,x,y,*iput);
				else LCD_ShowASC_NBK(ucFont,x,y,*iput);
				x+=(ucFont+1)/2;
				iput++;
			}
		}else{//中文
			while(1){
				if(ucFont == Font8){
					buf = &GB_8[i].Index[0];
				}
				if(ucFont == Font12){
					buf = &GB_12[i].Index[0];
				}
				if(ucFont == Font14){
					buf = &GB_14[i].Index[0];
				}
				if(ucFont == Font16){
					buf = &GB_16[i].Index[0];
				}
				if(ucFont == Font20){
					buf = &GB_20[i].Index[0];
				}
				if(ucFont == Font24){
					buf = &GB_24[i].Index[0];
				}
				if(ucFont == Font32){
					buf = &GB_32[i].Index[0];
				}
//				if(ucFont == Font36){
//					buf = &GB_36[i].Index[0];
//				}
				if(ucFont == Font40){
					buf = &GB_40[i].Index[0];
				}
//				if(ucFont == Font48){
//					buf = &GB_48[i].Index[0];
//				}
//				if(ucFont == Font64){
//					buf = &GB_64[i].Index[0];
//				}
				if((*buf == endChar[0]) && (*(buf+1) == endChar[1])){ //判断是否为结束字符 “呕”
					i=0;
					break;
				}
				if((*buf == 0xFF) || (*(buf+1) == 0xFF)){ //判断是否为结束字符  Flash 存储器有异常，退出。
					i=0;
					break;
				}
				if((*iput == *buf) && (*(iput+1) == *(buf+1))){
					if(mode == 0)LCD_ShowCh(ucFont,x,y,buf+3);
					else LCD_ShowCh_NBK(ucFont,x,y,buf+3);
					i=0;
					break;
				}
				i++;
			}
			x+=ucFont;
			iput+=2;
		}
	}
}
//居中显示字符串
void LCD_ShowText_Center(u8 ucFont,u16 x0,u16 y0,u16 width,u16 height,u8 mode,char *iput)
{
	u16 x,y,temp;
	u8 strLen,strWid;
	u16 fillWidX1,fillWidY1,fillWidX2,fillWidY2;
	
	strLen = strlen(iput);
	strWid = strLen*ucFont/2;
	
	if(width < strWid){
		x = x0;
		fillWidX1 = 0;
		fillWidX2 = 0;
	}else{
		temp = width - strWid;
		x = x0 + temp/2;
		fillWidX1 = temp/2;
		if(temp%2 == 0){
			fillWidX2 = fillWidX1;
		}else{
			fillWidX2 = fillWidX1 + 1;
		}
	}
	if(height < ucFont){
		y = y0;
		fillWidY1 = 0;
		fillWidY2 = 0;
	}else{
		temp = height - ucFont;
		y = y0 + temp/2;
		fillWidY1 = temp/2;
		if(temp%2 == 0){
			fillWidY2 = fillWidY1;
		}else{
			fillWidY2 = fillWidY1+1;
		}
	}
	LCD_ShowText(ucFont,x,y,mode,(u8*)iput);
	if(mode == 0){
		LCD_FillRect(x0,y0,width,fillWidY1,BkgColor);
		LCD_FillRect(x0,y0+fillWidY1+ucFont,width,fillWidY2,BkgColor);
		LCD_FillRect(x0,y0+fillWidY1,fillWidX1,ucFont,BkgColor);
		LCD_FillRect(x0+fillWidX1+strWid,y0+fillWidY1,fillWidX2,ucFont,BkgColor);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//瞄准镜专用
void LCD_FillRectMZJ(int16_t x, int16_t y, int16_t xlen, int16_t ylen, u16 color)
{
	short x1,y1;
    int i,alllenth;
	
	u16 left = 60;
	u16 right = 448;
	x1 = x+xlen-1;
	y1 = y+ylen-1;
	if(x < 0) x = 0;
	if(x1 < 0) return;
	if(y < 0) y = 0;
	if(y1 < 0) return;
    if(y > LCD_HEIGHT) return;
    if(y1 > LCD_HEIGHT) y1 = LCD_HEIGHT;
    if(x > LCD_WIDTH) return;
    if(x1 > LCD_WIDTH) x1 = LCD_WIDTH;
	if(x < left)x = left;
	if(x1 <= left) return;
	if(x > right)return;
	if(x1 > right) x1 = right;
	
	SetDispArea(x, x1, y, y1);
	
    alllenth = xlen*ylen;
	
	while(alllenth > 7){
		*(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
        *(__IO uint16_t *) (Bank1_LCD_D) = color;
		alllenth -= 8;
	}
	for(i=0;i<alllenth;i++){
		*(__IO uint16_t *) (Bank1_LCD_D) = color; 
	}
}

