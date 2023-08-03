#ifndef __KEY_H
#define __KEY_H	 
#include "mcu_io.h"

#define KEY_NONE			0X0
#define KEY_POWER			0X0001
#define KEY_OK				0X0001
#define KEY_BACK			0X0002
#define KEY_FN				0X0004

#define KEY_UP				0X0002
#define KEY_DOWN			0X0004


#define LONG_KEY_POWER		(KEY_POWER | 0xFF00)
#define LONG_KEY_OK			(KEY_OK | 0xFF00)
#define LONG_KEY_BACK		(KEY_BACK  | 0xFF00)
#define LONG_KEY_FN			(KEY_FN | 0xFF00)


#define INTE_KEY_POWER		(KEY_POWER | 0xEE00)
#define INTE_KEY_OK			(KEY_OK | 0xEE00)
#define INTE_KEY_UP			(KEY_UP  | 0xEE00)
#define INTE_KEY_DOWN		(KEY_DOWN | 0xEE00)


#define IO_KEY_POWER 		PDin(6)		//µÁ‘¥
#define IO_KEY_BACK 		PDin(3)		//∑µªÿ
#define IO_KEY_FN			PBin(5)		//Fn


extern u16 g_u16KeyVal;
extern u16 g_u16KeyValBuf;

void Key_Init(void);
u16 Key_Scan(void);

#endif
