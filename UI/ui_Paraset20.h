
#ifndef UI_PARASET20_H
#define UI_PARASET20_H

#include "main.h"

//#define TESTJIAOZHUN

extern  u16  g_u16BkRGB;
extern u16  g_u16TextRGB;


void ui_Paraset20_InitDisp(void);
void ui_Paraset20_Run(void);


int Paraset20_kj_Bottom(void);

int BkColorR_Kj01(void);
int BkColorG_Kj02(void);
int BkColorB_Kj03(void);
int TxtColorR_Kj04(void);
int TxtColorG_Kj05(void);
int TxtColorB_Kj06(void);


#endif
