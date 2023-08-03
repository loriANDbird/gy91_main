#ifndef __PYTEXT_H__
#define __PYTEXT_H__	 
#include "mcu_io.h"
#include "fontupd.h"
	 
 					     
void Get_HzMat(u8 font,unsigned char *code,unsigned char *mat);			//得到汉字的点阵码
void Show_Text(u8 font,u16 x,u16 y,u8 mode,u8 *str);
#endif
