#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include "mcu_io.h"
#include "OpTable.h"
#include "rX8010.h" 

//提示弹窗
#define DIALOGSAVE		0
#define DIALOGDELEALL	1
#define DIALOGDELESEL	2
#define DIALOGBATLOW	3
#define DIALOGBATNONE	4
#define DIALOGSTORLOW	5
#define DIALOGSTORNONE	6
#define DIALOGUSBLINK	7
#define DIALOGBTLINK	8

#define DIALOGGOUJIANEXIST		9
#define DIALOGBIAODINGBATLOW	10
#define DIALOGBIAODINGBATJUMP	11
#define DIALOGBIAODINGTEMPER	12

#define CONFIRM		1
#define CANCEL		0
//提示弹窗表
struct PROMPTStr{
	u8 id;
	u8* name;
	u8* strWords1;
	u8* strWords2;
	u8* btnWords1;
	u8* btnWords2;
};

void TextSetDialog(SYS_PARA_TABLE_STRUCT_P paraTable_p);
int NumSetDialog(SYS_PARA_TABLE_STRUCT_P paraTable_p);
int iNumInputDialog(int* iNumAddr);
float fNumInputDialog(float fNumAddr,u8 dicimal);
void SelectDialog(SYS_PARA_TABLE_STRUCT_P paraTable_p,u8 optNums);
void TimeSetDialog(u8 mode);
u8 PromptDialog(u8 id);
#endif

