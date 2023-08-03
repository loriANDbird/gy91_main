
#ifndef UI_SOFTKEYBOARD_H
#define UI_SOFTKEYBOARD_H

#include "main.h"

typedef struct STRUCT_SAPM_BOARD{
	char ParaName[15];
	char *pParaVal;
	char chrDisParaVal[10];	
		
	char keyVal[50];	
}STRUCT_SAPM_BOARD;

extern char g_KeyBoardBuffer[10];

void ui_Softkeyboard_Init(void);
void ui_Softkeyboard_Run(void);

void DispParaName2Val(int iParaHighLihtPos);

void DispOneKey(int kNum,u8 kHighLight);
int SoftKeyboard_kj_Bottom(void);


#endif
