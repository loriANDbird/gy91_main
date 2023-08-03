#ifndef _LCD_H
#define _LCD_H

#include "stm32f10x.h"
#include "font.h"

#define LCDREADBUFLEN	38400
extern u16 g_u16LcdReadBuf[LCDREADBUFLEN];

#define LCD_HEIGHT	 319
#define LCD_WIDTH    479

//��ɫ���壺2�ֽڳ���16λ 5��6��5 =��������
#define BLACK	0x0000//��
#define RED		0xf800//��
#define GREEN	0x0640//0x07E0//��
#define DGREEN	0x0640//����
#define BLUE	0x0358//��
#define WHITE	0xFFFF//��
#define YELLOW	0xffe0//�� 
#define GOLD	0xfe40//��1111 1110 0100 0000
#define DGOLD	0xc500//��1100 0101 0000 0000
#define ENROD	0xe504//GOLDENROD������218 165 32
#define ORANGE	0xfce3//��
#define BROWN	0xa145//��1010 0001 0100 0101
#define DDGRAY	0x4208//�����	8 16 8
#define DGRAY	0x630c//���	12 24 12
#define GRAY	0x8410//��		16 32 16
#define LGRAY	0xa514//ǳ��	20 40 20
#define LLGRAY	0xc618//ǳǳ��	24 48 24
#define PURPLE	0x737b//��

#define BGCON	0	//д����ɫ
#define BGCOFF	1	//��д����ɫ


void LCD_Init(void);
void LCD_SetColor(uint16_t bkgclor, uint16_t txtclor);
void LCD_Clear(void);
void LCD_DrawPoint(u16 x,u16 y,u16 color);
u16 LCD_ReadPoint(u16 x,u16 y);
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
void LCD_DrawLineDotted(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
void LCD_DrawCircle(u16 x0,u16 y0,u8 r,u16 color);
void LCD_DrawRect(u16 x0,u16 y0,u16 xLen,u16 yLen,u16 color);
void LCD_FillRect(int16_t x, int16_t y, int16_t xlen, int16_t ylen, u16 color);
void LCD_FillRectMZJ(int16_t x, int16_t y, int16_t xlen, int16_t ylen, u16 color);//��׼��

void LCD_ShowASC(u8 ucFont,u16 x,u16 y,u8 num);
void LCD_ShowASC_NBK(u8 ucFont,u16 x,u16 y,u8 num);
void LCD_ShowString(u8 ucFont,u16 x,u16 y,u8 mode,char *iput);
void LCD_ShowChinese(u8 ucFont,u16 x,u16 y,u8 mode,u8 *iput);
void LCD_ShowText(u8 ucFont,u16 x,u16 y,u8 mode,u8 *iput);
void LCD_ShowText_Center(u8 ucFont,u16 x0,u16 y0,u16 width,u16 height,u8 mode,char *iput);
void LCD_ShowNum(u8 ucFont,u16 x,u16 y,u8 num);
void LCD_ShowNums(u8 ucFont,u16 x,u16 y,u8 mode,int num);
void LCD_ShowNums_Center(u8 ucFont,u16 x0,u16 y0,u16 width,u16 height,u8 mode,int num);
void LCD_ShowNnNums(u8 ucFont,u16 x,u16 y,u8 mode,u8 digit,int num);
void LCD_ShowFloat(u8 ucFont,u16 x,u16 y,u8 mode,u8 numLen,u8 decimal,float fnum);
void LCD_ShowZimo(u16 x,u16 y,u16 width,u16 height,u8 mode,u8* zimo);

u16* LCD_ReadRect(u16 x0,u16 y0,u16 width,u16 height);
void LCD_ChangeRectColor(u16 sx,u16 ex,u16 sy,u16 ey,u16 oldColor,u16 newColor);
//u16* LCD_ReadBMP(char* PicName,u32 PicSize);
u8 LCD_ShowBMP(u16 x0,u16 y0,u16 width,u16 height,char* PicName);
void LCD_ShowPicture(u16 x0,u16 y0,u16 width,u16 height,const u16* p);

#endif
