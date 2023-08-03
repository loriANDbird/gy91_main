#ifndef UI_PUBLIC_H
#define UI_PUBLIC_H

#include "mcu_io.h"
#include "OpTable.h"

#define NORMALLIGHT	0
#define HIGHLIGHT	1
#define LOWLIGHT	2

void ui_DispTitleBar(u8 *name,u8 mode);
void ui_DispDate(void);
void ui_DispTime(u8 sta);
void ui_DispGPSIcon(u8 sta);
void ui_GetTempera(void);
void ui_DispTempera(void);
void ui_DispBluetooth(void);
void GetNameShort(char* nambuf,char* namSor);
void ui_DispGJName(u8 font,u16 x,u16 y,u16 nameCol,char* nameStr);
void ui_SetGJNameNext(char* src);
void ui_DispBattery(u8 sta);
void Get_BatPrcnt(void);

u8 Get_FatFreePrcnt(void);

void ui_DispMenuBtn(MENU_TABLE_STRUCT* menuBtn,u8 highID);
void ui_BrushMenuOneBtn(MENU_TABLE_STRUCT* menuBtn,u8 btnSta);

void ui_BatDataRecord(int pcnt);

#endif

