#ifndef __PYTEXT_H__
#define __PYTEXT_H__	 
#include "mcu_io.h"
#include "fontupd.h"
	 
 					     
void Get_HzMat(u8 font,unsigned char *code,unsigned char *mat);			//�õ����ֵĵ�����
void Show_Text(u8 font,u16 x,u16 y,u8 mode,u8 *str);
#endif
