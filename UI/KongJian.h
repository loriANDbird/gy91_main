#ifndef KONGJIAN_H
#define KONGJIAN_H

#include "mcu_io.h"

int ui_KeyTask(void);
void ui_ButtonTask(u8 nums);
void ui_AllButtonReaction(u8 btNums,u16 oldColor,u16 newColor);
void ui_OneButtonReaction(u8 btNum,u16 oldColor,u16 newColor);
void ui_OneButtonReactionXY(u8 btNum,u16 sX,u16 sY,u16 eX,u16 eY,u16 oldColor,u16 newColor);
void ui_OneButtonReaction_Pic(u8 btNum,u16* oldPic,u16* newPic);

#endif


