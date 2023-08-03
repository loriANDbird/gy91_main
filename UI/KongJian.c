/******************************************************************************
创建者：康泉
创建日期：2018.03.16
功能描述：
    1. LCD显示控件相关函数
    2. InitKJ() : 在每个UI界面的初始函数中调用一次，用于初始化控件变量
    3. AddKJ ():  在UI界面初始化函数中，添加控件，包括ID、位置大小、颜色等参数 
    4. DispKJ(short int  id,u8 bHighDisp, char * pText) :显示控件，输入参数：控件ID，显示类型，显示文本
    
更新记录：
    1. 2018.03.16 初次创建
    
*******************************************************************************/
#include "KongJian.h"
#include "Config_ui.h"
#include "touch.h"

//-------------------------------------------------------------------------------------------------------------------------
//控件的按键扫描任务
//返回值定义： 0=无反馈动作。
//				   >0 跳转新的UI界面号
int ui_KeyTask(void)
{
	g_u16KeyValBuf = 0;
	if(g_u16KeyVal == 0){ 
		return 0;
	}else{
		g_u16KeyValBuf = g_u16KeyVal;
		g_u16KeyVal = 0;
		PowerOffCnt_Reset();
		BKLedOffCnt_Reset();
		if(g_bBkLedOnOffSta == 1){//背光已关闭
			g_bBkLedOnOffSta = 0;
			SetBkLedPercent(25+25*g_SysPara.iBKLedLight);
			g_u16KeyValBuf = 0;//按钮无效
		}
		if(g_u16KeyValBuf == LONG_KEY_POWER){//关机
			PwrOffFunc();
		}
	}
	
	return g_u16KeyValBuf;
}

void ui_ButtonTask(u8 nums)
{
	g_u8TouchResult = TOUCH_NONE;
	if(g_bTouchScanFlag == 1){//扫描周期20ms
		g_bTouchScanFlag = 0;
		g_u8TouchResult = Get_Touch_Res(nums);
//		if(g_u8TouchResult != 0xff){
//			LCD_SetColor(WHITE,BLACK);
//			LCD_ShowNnNums(ASC24x12,246,168,0,3,g_u8TouchResult);
//		}
		if(g_u8TouchResult != TOUCH_NONE){//有触摸
			
			PowerOffCnt_Reset();
			BKLedOffCnt_Reset();
			if(g_bBkLedOnOffSta == 1){//背光已关闭
				g_bBkLedOnOffSta = 0;
				SetBkLedPercent(25+25*g_SysPara.iBKLedLight);
				g_u8TouchResult = BTN_INVALID_AREA;//不在按钮区域
			}
		}
	}
}

//按钮按下后显示效果，nums:当前页面按钮个数
void ui_AllButtonReaction(u8 btNums,u16 oldColor,u16 newColor)
{
	u8 n=0;
	int i,j;
	u16 *readAddr;
	u16 width,height;
	int readLen;
//	for(n=0;n<btNums;n++){
//		if(g_ButtonBuf[n].btSta == BTN_STATUS_PRESS_UP){//按钮抬起
//			g_ButtonBuf[n].btSta = BTN_STATUS_NONE;
//			for(i=g_ButtonBuf[n].sX;i<=g_ButtonBuf[n].eX;i++){
//				for(j=g_ButtonBuf[n].sY;j<=g_ButtonBuf[n].eY;j++){
//					if(LCD_ReadPoint(i,j) == newColor)LCD_DrawPoint(i,j,oldColor);
//				}
//			}
//		}
//	}
//	for(n=0;n<btNums;n++){
//		if(g_ButtonBuf[n].btSta == BTN_STATUS_PRESS_DOWN){//按钮按下
//			g_ButtonBuf[n].btSta = BTN_STATUS_PRESS_HOLD;
//			for(i=g_ButtonBuf[n].sX;i<=g_ButtonBuf[n].eX;i++){
//				for(j=g_ButtonBuf[n].sY;j<=g_ButtonBuf[n].eY;j++){
//					if(LCD_ReadPoint(i,j) == oldColor)LCD_DrawPoint(i,j,newColor);
////					if(LCD_ReadPoint(i,j) == WHITE)LCD_DrawPoint(i,j,LLGRAY);
//				}
//			}
//		}
//	}
	for(n=0;n<btNums;n++){
		if(g_ButtonBuf[n].btSta == BTN_STATUS_PRESS_UP){//按钮抬起
			g_ButtonBuf[n].btSta = BTN_STATUS_NONE;
			width = g_ButtonBuf[n].eX-g_ButtonBuf[n].sX+1;
			height = g_ButtonBuf[n].eY-g_ButtonBuf[n].sY+1;
			readAddr = LCD_ReadRect(g_ButtonBuf[n].sX,g_ButtonBuf[n].sY,width,height);
			readLen = width * height;
			if(readAddr != NULL){
				for(i=0;i<readLen;i++){
					if(readAddr[i] == newColor)readAddr[i] = oldColor;
				}
				LCD_ShowPicture(g_ButtonBuf[n].sX,g_ButtonBuf[n].sY,width,height,readAddr);
				myfree(SRAMIN,readAddr);
			}else{
				for(i=g_ButtonBuf[n].sX;i<=g_ButtonBuf[n].eX;i++){
					for(j=g_ButtonBuf[n].sY;j<=g_ButtonBuf[n].eY;j++){
						if(LCD_ReadPoint(i,j) == newColor)LCD_DrawPoint(i,j,oldColor);
					}
				}
			}
		}
	}
	for(n=0;n<btNums;n++){
		if(g_ButtonBuf[n].btSta == BTN_STATUS_PRESS_DOWN){//按钮按下
			g_ButtonBuf[n].btSta = BTN_STATUS_PRESS_HOLD;
			width = g_ButtonBuf[n].eX-g_ButtonBuf[n].sX+1;
			height = g_ButtonBuf[n].eY-g_ButtonBuf[n].sY+1;
			readAddr = LCD_ReadRect(g_ButtonBuf[n].sX,g_ButtonBuf[n].sY,width,height);
			readLen = width * height;
			if(readAddr != NULL){
				for(i=0;i<readLen;i++){
					if(readAddr[i] == oldColor)readAddr[i] = newColor;
				}
				LCD_ShowPicture(g_ButtonBuf[n].sX,g_ButtonBuf[n].sY,width,height,readAddr);
				myfree(SRAMIN,readAddr);
			}else{
				for(i=g_ButtonBuf[n].sX;i<=g_ButtonBuf[n].eX;i++){
					for(j=g_ButtonBuf[n].sY;j<=g_ButtonBuf[n].eY;j++){
						if(LCD_ReadPoint(i,j) == oldColor)LCD_DrawPoint(i,j,newColor);
					}
				}
			}
		}
	}
}

//按钮按下后显示效果，num:当前按钮编号
void ui_OneButtonReaction(u8 btNum,u16 oldColor,u16 newColor)
{
	int i,j;
	u16 *readAddr;
	u16 width,height;
	int readLen;
	
	if(g_ButtonBuf[btNum].btSta == BTN_STATUS_PRESS_UP){//按钮抬起
		g_ButtonBuf[btNum].btSta = BTN_STATUS_NONE;
		
		width = g_ButtonBuf[btNum].eX-g_ButtonBuf[btNum].sX+1;
		height = g_ButtonBuf[btNum].eY-g_ButtonBuf[btNum].sY+1;
		readAddr = LCD_ReadRect(g_ButtonBuf[btNum].sX,g_ButtonBuf[btNum].sY,width,height);
		readLen = width * height;
		if(readAddr != NULL){
			for(i=0;i<readLen;i++){
				if(readAddr[i] == newColor)readAddr[i] = oldColor;
			}
			LCD_ShowPicture(g_ButtonBuf[btNum].sX,g_ButtonBuf[btNum].sY,width,height,readAddr);
			myfree(SRAMIN,readAddr);
		}else{
			for(i=g_ButtonBuf[btNum].sX;i<=g_ButtonBuf[btNum].eX;i++){
				for(j=g_ButtonBuf[btNum].sY;j<=g_ButtonBuf[btNum].eY;j++){
					if(LCD_ReadPoint(i,j) == newColor)LCD_DrawPoint(i,j,oldColor);
				}
			}
		}
	}
	if(g_ButtonBuf[btNum].btSta == BTN_STATUS_PRESS_DOWN){//按钮按下
		g_ButtonBuf[btNum].btSta = BTN_STATUS_PRESS_HOLD;
		
		width = g_ButtonBuf[btNum].eX-g_ButtonBuf[btNum].sX+1;
		height = g_ButtonBuf[btNum].eY-g_ButtonBuf[btNum].sY+1;
		readAddr = LCD_ReadRect(g_ButtonBuf[btNum].sX,g_ButtonBuf[btNum].sY,width,height);
		readLen = width * height;
		if(readAddr != NULL){
			for(i=0;i<readLen;i++){
				if(readAddr[i] == oldColor)readAddr[i] = newColor;
			}
			LCD_ShowPicture(g_ButtonBuf[btNum].sX,g_ButtonBuf[btNum].sY,width,height,readAddr);
			myfree(SRAMIN,readAddr);
		}else{
			for(i=g_ButtonBuf[btNum].sX;i<=g_ButtonBuf[btNum].eX;i++){
				for(j=g_ButtonBuf[btNum].sY;j<=g_ButtonBuf[btNum].eY;j++){
					if(LCD_ReadPoint(i,j) == oldColor)LCD_DrawPoint(i,j,newColor);
				}
			}
		}
	}
}

//按钮按下后显示效果，指定区域，num:当前按钮编号
void ui_OneButtonReactionXY(u8 btNum,u16 sX,u16 sY,u16 eX,u16 eY,u16 oldColor,u16 newColor)
{
	int i,j;
	u16 *readAddr;
	u16 width,height;
	int readLen;
	
	if(g_ButtonBuf[btNum].btSta == BTN_STATUS_PRESS_UP){//按钮抬起
		g_ButtonBuf[btNum].btSta = BTN_STATUS_NONE;
		
		width = eX-sX+1;
		height = eY-sY+1;
		readAddr = LCD_ReadRect(sX,sY,width,height);
		readLen = width * height;
		if(readAddr != NULL){
			for(i=0;i<readLen;i++){
				if(readAddr[i] == newColor)readAddr[i] = oldColor;
			}
			LCD_ShowPicture(sX,sY,width,height,readAddr);
			myfree(SRAMIN,readAddr);
		}else{
			for(i=sX;i<=eX;i++){
				for(j=sY;j<=eY;j++){
					if(LCD_ReadPoint(i,j) == newColor)LCD_DrawPoint(i,j,oldColor);
				}
			}
		}
	}
	if(g_ButtonBuf[btNum].btSta == BTN_STATUS_PRESS_DOWN){//按钮按下
		g_ButtonBuf[btNum].btSta = BTN_STATUS_PRESS_HOLD;
		
		width = eX-sX+1;
		height = eY-sY+1;
		readAddr = LCD_ReadRect(sX,sY,width,height);
		readLen = width * height;
		if(readAddr != NULL){
			for(i=0;i<readLen;i++){
				if(readAddr[i] == oldColor)readAddr[i] = newColor;
			}
			LCD_ShowPicture(sX,sY,width,height,readAddr);
			myfree(SRAMIN,readAddr);
		}else{
			for(i=sX;i<=eX;i++){
				for(j=sY;j<=eY;j++){
					if(LCD_ReadPoint(i,j) == oldColor)LCD_DrawPoint(i,j,newColor);
				}
			}
		}
	}
}

