/******************************************************************************
�����ߣ���Ȫ
�������ڣ�2018.03.16
����������
    1. LCD��ʾ�ؼ���غ���
    2. InitKJ() : ��ÿ��UI����ĳ�ʼ�����е���һ�Σ����ڳ�ʼ���ؼ�����
    3. AddKJ ():  ��UI�����ʼ�������У���ӿؼ�������ID��λ�ô�С����ɫ�Ȳ��� 
    4. DispKJ(short int  id,u8 bHighDisp, char * pText) :��ʾ�ؼ�������������ؼ�ID����ʾ���ͣ���ʾ�ı�
    
���¼�¼��
    1. 2018.03.16 ���δ���
    
*******************************************************************************/
#include "KongJian.h"
#include "Config_ui.h"
#include "touch.h"

//-------------------------------------------------------------------------------------------------------------------------
//�ؼ��İ���ɨ������
//����ֵ���壺 0=�޷���������
//				   >0 ��ת�µ�UI�����
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
		if(g_bBkLedOnOffSta == 1){//�����ѹر�
			g_bBkLedOnOffSta = 0;
			SetBkLedPercent(25+25*g_SysPara.iBKLedLight);
			g_u16KeyValBuf = 0;//��ť��Ч
		}
		if(g_u16KeyValBuf == LONG_KEY_POWER){//�ػ�
			PwrOffFunc();
		}
	}
	
	return g_u16KeyValBuf;
}

void ui_ButtonTask(u8 nums)
{
	g_u8TouchResult = TOUCH_NONE;
	if(g_bTouchScanFlag == 1){//ɨ������20ms
		g_bTouchScanFlag = 0;
		g_u8TouchResult = Get_Touch_Res(nums);
//		if(g_u8TouchResult != 0xff){
//			LCD_SetColor(WHITE,BLACK);
//			LCD_ShowNnNums(ASC24x12,246,168,0,3,g_u8TouchResult);
//		}
		if(g_u8TouchResult != TOUCH_NONE){//�д���
			
			PowerOffCnt_Reset();
			BKLedOffCnt_Reset();
			if(g_bBkLedOnOffSta == 1){//�����ѹر�
				g_bBkLedOnOffSta = 0;
				SetBkLedPercent(25+25*g_SysPara.iBKLedLight);
				g_u8TouchResult = BTN_INVALID_AREA;//���ڰ�ť����
			}
		}
	}
}

//��ť���º���ʾЧ����nums:��ǰҳ�水ť����
void ui_AllButtonReaction(u8 btNums,u16 oldColor,u16 newColor)
{
	u8 n=0;
	int i,j;
	u16 *readAddr;
	u16 width,height;
	int readLen;
//	for(n=0;n<btNums;n++){
//		if(g_ButtonBuf[n].btSta == BTN_STATUS_PRESS_UP){//��ţ̌��
//			g_ButtonBuf[n].btSta = BTN_STATUS_NONE;
//			for(i=g_ButtonBuf[n].sX;i<=g_ButtonBuf[n].eX;i++){
//				for(j=g_ButtonBuf[n].sY;j<=g_ButtonBuf[n].eY;j++){
//					if(LCD_ReadPoint(i,j) == newColor)LCD_DrawPoint(i,j,oldColor);
//				}
//			}
//		}
//	}
//	for(n=0;n<btNums;n++){
//		if(g_ButtonBuf[n].btSta == BTN_STATUS_PRESS_DOWN){//��ť����
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
		if(g_ButtonBuf[n].btSta == BTN_STATUS_PRESS_UP){//��ţ̌��
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
		if(g_ButtonBuf[n].btSta == BTN_STATUS_PRESS_DOWN){//��ť����
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

//��ť���º���ʾЧ����num:��ǰ��ť���
void ui_OneButtonReaction(u8 btNum,u16 oldColor,u16 newColor)
{
	int i,j;
	u16 *readAddr;
	u16 width,height;
	int readLen;
	
	if(g_ButtonBuf[btNum].btSta == BTN_STATUS_PRESS_UP){//��ţ̌��
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
	if(g_ButtonBuf[btNum].btSta == BTN_STATUS_PRESS_DOWN){//��ť����
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

//��ť���º���ʾЧ����ָ������num:��ǰ��ť���
void ui_OneButtonReactionXY(u8 btNum,u16 sX,u16 sY,u16 eX,u16 eY,u16 oldColor,u16 newColor)
{
	int i,j;
	u16 *readAddr;
	u16 width,height;
	int readLen;
	
	if(g_ButtonBuf[btNum].btSta == BTN_STATUS_PRESS_UP){//��ţ̌��
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
	if(g_ButtonBuf[btNum].btSta == BTN_STATUS_PRESS_DOWN){//��ť����
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

