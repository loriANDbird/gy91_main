#ifndef _LCD_H
#define _LCD_H

#include "stm32f10x.h"
#include "font.h"

#define LCDREADBUFLEN	38400
extern u16 g_u16LcdReadBuf[LCDREADBUFLEN];

#define LCD_HEIGHT	 319
#define LCD_WIDTH    479

//ÑÕÉ«¶¨Òå£º2×Ö½Ú³¤¶È16Î» 5£¬6£¬5 =¡·ºìÂÌÀ¶
#define BLACK	0x0000//ºÚ
#define RED		0xf800//ºì
#define GREEN	0x0640//0x07E0//ÂÌ
#define DGREEN	0x0640//ÉîÂÌ
#define BLUE	0x0358//À¶
#define WHITE	0xFFFF//°×
#define YELLOW	0xffe0//»Æ 
#define GOLD	0xfe40//½ð1111 1110 0100 0000
#define DGOLD	0xc500//½ð1100 0101 0000 0000
#define ENROD	0xe504//GOLDENRODÇï÷è÷ë218 165 32
#define ORANGE	0xfce3//³È
#define BROWN	0xa145//×Ø1010 0001 0100 0101
#define DDGRAY	0x4208//ÉîÉî»Ò	8 16 8
#define DGRAY	0x630c//Éî»Ò	12 24 12
#define GRAY	0x8410//»Ò		16 32 16
#define LGRAY	0xa514//Ç³»Ò	20 40 20
#define LLGRAY	0xc618//Ç³Ç³»Ò	24 48 24
#define PURPLE	0x737b//×Ï

#define BGCON	0	//Ð´±³¾°É«
#define BGCOFF	1	//²»Ð´±³¾°É«


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
void LCD_FillRectMZJ(int16_t x, int16_t y, int16_t xlen, int16_t ylen, u16 color);//Ãé×¼¾µ

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
