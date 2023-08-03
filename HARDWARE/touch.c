#include "touch.h"
#include "ft6336u.h"
#include "main.h"
#include "KongJian.h"
#include "delay.h"
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "timer.h"
#include "lcd.h"
#include "font.h"

//ȫ�ֱ���
u8 g_u8TouchResult;//�����������ֵ ��ť��Ż��߻���������
u8 g_u8ButtonNums;//��������

struct TP_DATA TPData;
struct BUTTON g_ButtonBuf[BTN_NUM_MAX];

static u16 TouchCX;//������ǰλ��X
static u16 TouchCY;//������ǰλ��Y
static u16 TouchSX;//������ʼλ��X
static u16 TouchSY;//������ʼλ��Y
static u16 TouchEX;//������ֹλ��X
static u16 TouchEY;//������ֹλ��Y

static u16 SlideX;//��������λ��X
static u16 SlideY;//��������λ��Y
static u16 SlideX_L;//�ϴλ�������λ��X
static u16 SlideY_L;//�ϴλ�������λ��Y

static u16 MoveX;//������X
static u16 MoveY;//������Y
static u16 MoveStaX;//������X
static u16 MoveStaY;//������Y
static u16 MoveStaX_L;//�ϴλ�����X
static u16 MoveStaY_L;//�ϴλ�����Y

static u8 TouchStatus = 0;//0:�޴�����1�д���
static u8 SlideStatus = 0;//1:�л�����־
static u16 TouchCount;//��������
static u16 TouchNoneCount;//�޴�������


//ɨ�败���¼�,ɨ������20ms
//����ֵ
u8 Touch_Scan(void)
{
	if(TP_Scan(&TPData)){////�д���
		TouchCount += 1;
		if(TouchCount < 1)return TOUCH_ING;
		if(TouchCount == 1){//��¼������ʼ�����꣬��3����Ϊ��ʼ��
			TouchStatus = 1;//������Ч
			SlideStatus = 0;
			TouchNoneCount = 0;			
			TouchSX = TPData.TpX[0];
			TouchSY = TPData.TpY[0];
			MoveStaX = 0;
			MoveStaY = 0;
			MoveStaX_L = 0;
			MoveStaY_L = 0;
			SlideX_L = TouchSX;
			SlideY_L = TouchSY;
			return PRESS_DOWN;//������һ��
		}
		//����
		TouchCX = TPData.TpX[0];
		TouchCY = TPData.TpY[0];
		MoveX = abs(TouchCX-TouchSX);
		MoveY = abs(TouchCY-TouchSY);
		MoveStaX = MoveX/20;//�ƶ�20�����ش���һ��
		MoveStaY = MoveY/20;
		if(MoveStaX != MoveStaX_L){
			SlideStatus = 1;//�л������������ᴥ��
			SlideX = TouchCX;
			SlideY = TouchCY;
			MoveStaX_L = MoveStaX;
			MoveStaY_L = MoveStaY;
			if(MoveX > MoveY){//���򻬶�
				return SLIDE_GAP_HENG;//����
			}
		}
		if(MoveStaY != MoveStaY_L){
			SlideStatus = 1;//�л������������ᴥ��
			SlideX = TouchCX;
			SlideY = TouchCY;
			MoveStaX_L = MoveStaX;
			MoveStaY_L = MoveStaY;
			if(MoveX < MoveY){//���򻬶�
				return SLIDE_GAP_ZONG;//����
			}
		}
		MoveStaX_L = MoveStaX;
		MoveStaY_L = MoveStaY;
		
		//����
		if(TouchCount >= 100){
			if(SlideStatus == 0){//�޻���
				if(TouchCount == 100) return LONG_PRESS;
				if(TouchCount%10 == 0) return LONG_PRESS_HOLD;
			}
		}
		if(TouchStatus == 1)
			return TOUCH_ING;
	}else{//�޴���
		if(TouchStatus == 1){//���ڰ���״̬
			TouchNoneCount++;
			if(TouchNoneCount > 1){
				TouchStatus = 0;
				TouchEX = TouchCX;
				TouchEY = TouchCY;
				if(SlideStatus == 0){//�޻���
					if(TouchCount < 40){
						TouchCount = 0;
						return SINGLE_CLICK;//���̧��
					}else {
						TouchCount = 0;
//						return PRESS_UP;
						return SINGLE_CLICK;//���̧��
					}
				}else{
					TouchCount = 0;
					return SLIDE_END;//����̧��
				}
			}
			return TOUCH_ING;
		}
		return PRESS_NONE;
	}
	return TOUCH_ELSE;
}

//��ȡ��ť��Ż򻬶����
//(*touchEvent)(u8):��ť��������
//btnums:��ǰҳ�水������
u8 Get_Touch_Res(u8 btnums)
{
	u8 i,touchRes;
	u16 lenX,lenY;
	touchRes = Touch_Scan();
//	if(touchRes != 0){LCD_SetColor(WHITE,BLACK);LCD_ShowNnNums(ASC24x12,208,296,0,2,touchRes);}
//	LCD_ShowNnNums(ASC24x12,240,296,0,3,TouchCount);
	if(btnums > BTN_NUM_MAX)btnums = BTN_NUM_MAX;//�������
	//��������
	if(touchRes == PRESS_DOWN){
		for(i=0;i<btnums;i++){
			if(TouchSY>=g_ButtonBuf[i].sY){
				if(TouchSY<=g_ButtonBuf[i].eY){
					if(TouchSX>=g_ButtonBuf[i].sX){
						if(TouchSX<=g_ButtonBuf[i].eX){
							if(g_ButtonBuf[i].btAbility == BTN_ABILITY_DRAG){
								g_ButtonBuf[i].btSta = BTN_STATUS_DRAG;
							}else{
								g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_DOWN;
							}
							
							return i+BTN_NUM_PRESS_DOWN;
						}
					}
				}
			}
		}
	}
	//���̧��
//	if(touchRes == PRESS_UP){
//		for(i=0;i<btnums;i++){//���������
//			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_HOLD)
//				g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_UP;
//		}
//	}
	//���
	if(touchRes == SINGLE_CLICK){
		for(i=0;i<btnums;i++){
			if(TouchEY>=g_ButtonBuf[i].sY){
				if(TouchEY<=g_ButtonBuf[i].eY){
					if(TouchEX>=g_ButtonBuf[i].sX){
						if(TouchEX<=g_ButtonBuf[i].eX){
							if(TouchSY>=g_ButtonBuf[i].sY){
								if(TouchSY<=g_ButtonBuf[i].eY){
									if(TouchSX>=g_ButtonBuf[i].sX){
										if(TouchSX<=g_ButtonBuf[i].eX){
											g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_UP;//
											Touch_Beep_On();
											return i;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		for(i=0;i<btnums;i++){//���������
			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_HOLD){
				g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_UP;
			}
			if(g_ButtonBuf[i].btSta == BTN_STATUS_DRAG){
				g_ButtonBuf[i].btSta = BTN_STATUS_NONE;
			}
		}
		return BTN_INVALID_AREA;//�ǰ�ť������
	}
	//����
	if((touchRes == LONG_PRESS)||(touchRes == LONG_PRESS_HOLD)){
		for(i=0;i<btnums;i++){
			if(TouchSY>=g_ButtonBuf[i].sY){
				if(TouchSY<=g_ButtonBuf[i].eY){
					if(TouchSX>=g_ButtonBuf[i].sX){
						if(TouchSX<=g_ButtonBuf[i].eX){
							g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_HOLD;
							if(touchRes == LONG_PRESS) return i+BTN_NUM_LONGPRESS;
							else return i+BTN_NUM_LONGPRESS_HOLD;
						}
					}
				}
			}
		}
		return BTN_INVALID_AREA;//�ǰ�ť������
	}
	//���򻬶�
	if(touchRes == SLIDE_GAP_HENG){
		for(i=0;i<btnums;i++){//���������
			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_HOLD)
				g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_UP;
		}
		if(SlideX > SlideX_L){
			SlideX_L = SlideX;
			return SLIDE_GAP_RIGHT;//�һ�
		}
		if(SlideX < SlideX_L){
			SlideX_L = SlideX;
			return SLIDE_GAP_LEFT;//��
		}
	}
	//���򻬶�
	if(touchRes == SLIDE_GAP_ZONG){
		for(i=0;i<btnums;i++){//���������
			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_HOLD)
				g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_UP;
		}
		if(SlideY > SlideY_L){
			SlideY_L = SlideY;
			return SLIDE_GAP_DOWN;//�»�
		}
		if(SlideY < SlideY_L){
			SlideY_L = SlideY;
			return SLIDE_GAP_UP;//�ϻ�
		}
	}
	//����̧��
	if(touchRes == SLIDE_END){
		lenX = abs(TouchEX - TouchSX);
		lenY = abs(TouchEY - TouchSY);
		for(i=0;i<btnums;i++){//���������
			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_HOLD){
				g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_UP;
			}
			if(g_ButtonBuf[i].btSta == BTN_STATUS_DRAG){
				g_ButtonBuf[i].btSta = BTN_STATUS_NONE;
			}
		}
		//�����Ʋ���
		if(lenX > lenY){//���򻬶�
			if(lenX > 32){//�����������32��Ч
				if(TouchEX > TouchSX){
//					if(TouchSX >= 0)
						return SLIDE_END_RIGHT;//�һ�
				}
				if(TouchEX < TouchSX){
					if(TouchSX <= LCD_WIDTH)
						return SLIDE_END_LEFT;//��
				}
			}
		}
		if(lenX < lenY){//���򻬶�
			if(lenY > 32){//�����������32��Ч
				if(TouchEY > TouchSY){
//					if(TouchSY >= 0)
						return SLIDE_END_DOWN;//�»�
				}
				if(TouchEY < TouchSY){
					if(TouchSY <= LCD_HEIGHT)
						return SLIDE_END_UP;//�ϻ�	
				}
			}
		}
	}
	//��Ч����
	if(touchRes == TOUCH_ING){
		return TOUCH_INVALID;
	}
	//�޴���
	if(touchRes == PRESS_NONE){
		for(i=0;i<btnums;i++){//���������
			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_HOLD){
				g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_UP;
			}
			if(g_ButtonBuf[i].btSta == BTN_STATUS_DRAG){
				g_ButtonBuf[i].btSta = BTN_STATUS_NONE;
			}
		}
	}
	return TOUCH_NONE;//�޴���
}

//�϶�ѡ��	chBtn:�϶���ť����chQnt:�϶�������dir:�϶�����
u8 Get_DragChannel(struct BUTTON* chBtn,u8 chQnt,u8 dir)
{
	u8 i;
	u16 x,y;
	u8 chNum;
	
	x = TPData.TpX[0];
	y = TPData.TpY[0];
	
	if(dir == DRAG_HENG){
		for(i=0;i<chQnt;i++){
			if(x>=chBtn[i].sX){
				if(x<=chBtn[i].eX){
					chNum = i;
				}
			}
		}
		if(x < chBtn[0].sX){
			chNum = 0;
		}
		if(x > chBtn[chQnt-1].eX){
			chNum = chQnt-1;
		}
	}else{
		for(i=0;i<chQnt;i++){
			if(y>=chBtn[i].sY){
				if(y<=chBtn[i].eY){
					chNum = i;
				}
			}
		}
		if(y < chBtn[0].sY){
			chNum = 0;
		}
		if(y > chBtn[chQnt-1].eY){
			chNum = chQnt-1;
		}
	}
	if(chNum > chQnt-1){
		chNum = chQnt-1;
	}
	return chNum;
}

//�Ƿ���ָ����������1:�� 0:��
u8 Is_DesignArea_Touch(u16 sx,u16 sy,u16 ex,u16 ey)
{
	if((TouchCY>=sy)&&(TouchCY<=ey)){
		if((TouchCX>=sx)&&(TouchCX<=ex)){
			if((TouchSY>=sy)&&(TouchSY<=ey)){
				if((TouchSX>=sx)&&(TouchSX<=ex)){
					return 1;
				}
			}
		}
	}
	return 0;
}
//�Ƿ���ָ�����򻬶���1:�� 0:��
u8 Is_DesignArea_Slide(u16 sx,u16 sy,u16 ex,u16 ey)
{
	if((SlideY>=sy)&&(SlideY<=ey)){
		if((SlideX>=sx)&&(SlideX<=ex)){
			if((TouchSY>=sy)&&(TouchSY<=ey)){
				if((TouchSX>=sx)&&(TouchSX<=ex)){
					return 1;
				}
			}
		}
	}
	return 0;
}

//���û���������
//sens:1-8 ����
//static u16 SlideSensitive = 16;//����������
//void Set_Slide_Sensitive(u8 sens)
//{
//	if((sens>0)&&(sens<9)){
//		SlideSensitive = 16+8*sens;
//	}else {
//		SlideSensitive = 16;
//	}
//}

void ClearTouchSta(void)
{
	u8 i;
	for(i=0;i<5;i++){//�������
		ui_ButtonTask(0);
		Sleep_ms(10);
	}
}
