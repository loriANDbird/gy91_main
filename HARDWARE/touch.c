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

//全局变量
u8 g_u8TouchResult;//触摸结果返回值 按钮序号或者滑动操作等
u8 g_u8ButtonNums;//按键个数

struct TP_DATA TPData;
struct BUTTON g_ButtonBuf[BTN_NUM_MAX];

static u16 TouchCX;//触摸当前位置X
static u16 TouchCY;//触摸当前位置Y
static u16 TouchSX;//触摸起始位置X
static u16 TouchSY;//触摸起始位置Y
static u16 TouchEX;//触摸终止位置X
static u16 TouchEY;//触摸终止位置Y

static u16 SlideX;//滑动触摸位置X
static u16 SlideY;//滑动触摸位置Y
static u16 SlideX_L;//上次滑动触摸位置X
static u16 SlideY_L;//上次滑动触摸位置Y

static u16 MoveX;//滑动量X
static u16 MoveY;//滑动量Y
static u16 MoveStaX;//滑动量X
static u16 MoveStaY;//滑动量Y
static u16 MoveStaX_L;//上次滑动量X
static u16 MoveStaY_L;//上次滑动量Y

static u8 TouchStatus = 0;//0:无触摸，1有触摸
static u8 SlideStatus = 0;//1:有滑动标志
static u16 TouchCount;//触摸计数
static u16 TouchNoneCount;//无触摸计数


//扫描触摸事件,扫描周期20ms
//返回值
u8 Touch_Scan(void)
{
	if(TP_Scan(&TPData)){////有触摸
		TouchCount += 1;
		if(TouchCount < 1)return TOUCH_ING;
		if(TouchCount == 1){//记录触摸起始点坐标，第3次作为起始点
			TouchStatus = 1;//触摸生效
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
			return PRESS_DOWN;//触摸第一点
		}
		//滑动
		TouchCX = TPData.TpX[0];
		TouchCY = TPData.TpY[0];
		MoveX = abs(TouchCX-TouchSX);
		MoveY = abs(TouchCY-TouchSY);
		MoveStaX = MoveX/20;//移动20个像素触发一次
		MoveStaY = MoveY/20;
		if(MoveStaX != MoveStaX_L){
			SlideStatus = 1;//有滑动，长按不会触发
			SlideX = TouchCX;
			SlideY = TouchCY;
			MoveStaX_L = MoveStaX;
			MoveStaY_L = MoveStaY;
			if(MoveX > MoveY){//横向滑动
				return SLIDE_GAP_HENG;//滑动
			}
		}
		if(MoveStaY != MoveStaY_L){
			SlideStatus = 1;//有滑动，长按不会触发
			SlideX = TouchCX;
			SlideY = TouchCY;
			MoveStaX_L = MoveStaX;
			MoveStaY_L = MoveStaY;
			if(MoveX < MoveY){//纵向滑动
				return SLIDE_GAP_ZONG;//滑动
			}
		}
		MoveStaX_L = MoveStaX;
		MoveStaY_L = MoveStaY;
		
		//长按
		if(TouchCount >= 100){
			if(SlideStatus == 0){//无滑动
				if(TouchCount == 100) return LONG_PRESS;
				if(TouchCount%10 == 0) return LONG_PRESS_HOLD;
			}
		}
		if(TouchStatus == 1)
			return TOUCH_ING;
	}else{//无触摸
		if(TouchStatus == 1){//处于按下状态
			TouchNoneCount++;
			if(TouchNoneCount > 1){
				TouchStatus = 0;
				TouchEX = TouchCX;
				TouchEY = TouchCY;
				if(SlideStatus == 0){//无滑动
					if(TouchCount < 40){
						TouchCount = 0;
						return SINGLE_CLICK;//点击抬起
					}else {
						TouchCount = 0;
//						return PRESS_UP;
						return SINGLE_CLICK;//点击抬起
					}
				}else{
					TouchCount = 0;
					return SLIDE_END;//滑动抬起
				}
			}
			return TOUCH_ING;
		}
		return PRESS_NONE;
	}
	return TOUCH_ELSE;
}

//获取按钮序号或滑动结果
//(*touchEvent)(u8):按钮操作函数
//btnums:当前页面按键个数
u8 Get_Touch_Res(u8 btnums)
{
	u8 i,touchRes;
	u16 lenX,lenY;
	touchRes = Touch_Scan();
//	if(touchRes != 0){LCD_SetColor(WHITE,BLACK);LCD_ShowNnNums(ASC24x12,208,296,0,2,touchRes);}
//	LCD_ShowNnNums(ASC24x12,240,296,0,3,TouchCount);
	if(btnums > BTN_NUM_MAX)btnums = BTN_NUM_MAX;//溢出保护
	//触摸按下
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
	//点击抬起
//	if(touchRes == PRESS_UP){
//		for(i=0;i<btnums;i++){//防点击误判
//			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_HOLD)
//				g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_UP;
//		}
//	}
	//点击
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
		for(i=0;i<btnums;i++){//防点击误判
			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_HOLD){
				g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_UP;
			}
			if(g_ButtonBuf[i].btSta == BTN_STATUS_DRAG){
				g_ButtonBuf[i].btSta = BTN_STATUS_NONE;
			}
		}
		return BTN_INVALID_AREA;//非按钮区域触摸
	}
	//长按
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
		return BTN_INVALID_AREA;//非按钮区域触摸
	}
	//横向滑动
	if(touchRes == SLIDE_GAP_HENG){
		for(i=0;i<btnums;i++){//防点击误判
			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_HOLD)
				g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_UP;
		}
		if(SlideX > SlideX_L){
			SlideX_L = SlideX;
			return SLIDE_GAP_RIGHT;//右滑
		}
		if(SlideX < SlideX_L){
			SlideX_L = SlideX;
			return SLIDE_GAP_LEFT;//左滑
		}
	}
	//纵向滑动
	if(touchRes == SLIDE_GAP_ZONG){
		for(i=0;i<btnums;i++){//防点击误判
			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_HOLD)
				g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_UP;
		}
		if(SlideY > SlideY_L){
			SlideY_L = SlideY;
			return SLIDE_GAP_DOWN;//下滑
		}
		if(SlideY < SlideY_L){
			SlideY_L = SlideY;
			return SLIDE_GAP_UP;//上滑
		}
	}
	//滑动抬起
	if(touchRes == SLIDE_END){
		lenX = abs(TouchEX - TouchSX);
		lenY = abs(TouchEY - TouchSY);
		for(i=0;i<btnums;i++){//防点击误判
			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_HOLD){
				g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_UP;
			}
			if(g_ButtonBuf[i].btSta == BTN_STATUS_DRAG){
				g_ButtonBuf[i].btSta = BTN_STATUS_NONE;
			}
		}
		//非手势操作
		if(lenX > lenY){//横向滑动
			if(lenX > 32){//滑动距离大于32有效
				if(TouchEX > TouchSX){
//					if(TouchSX >= 0)
						return SLIDE_END_RIGHT;//右滑
				}
				if(TouchEX < TouchSX){
					if(TouchSX <= LCD_WIDTH)
						return SLIDE_END_LEFT;//左滑
				}
			}
		}
		if(lenX < lenY){//纵向滑动
			if(lenY > 32){//滑动距离大于32有效
				if(TouchEY > TouchSY){
//					if(TouchSY >= 0)
						return SLIDE_END_DOWN;//下滑
				}
				if(TouchEY < TouchSY){
					if(TouchSY <= LCD_HEIGHT)
						return SLIDE_END_UP;//上滑	
				}
			}
		}
	}
	//无效触摸
	if(touchRes == TOUCH_ING){
		return TOUCH_INVALID;
	}
	//无触摸
	if(touchRes == PRESS_NONE){
		for(i=0;i<btnums;i++){//防点击误判
			if(g_ButtonBuf[i].btSta == BTN_STATUS_PRESS_HOLD){
				g_ButtonBuf[i].btSta = BTN_STATUS_PRESS_UP;
			}
			if(g_ButtonBuf[i].btSta == BTN_STATUS_DRAG){
				g_ButtonBuf[i].btSta = BTN_STATUS_NONE;
			}
		}
	}
	return TOUCH_NONE;//无触摸
}

//拖动选择	chBtn:拖动按钮区域，chQnt:拖动个数，dir:拖动方向
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

//是否是指定区域触摸，1:是 0:否
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
//是否是指定区域滑动，1:是 0:否
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

//设置滑动灵敏度
//sens:1-8 降低
//static u16 SlideSensitive = 16;//滑动灵敏度
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
	for(i=0;i<5;i++){//清除误触摸
		ui_ButtonTask(0);
		Sleep_ms(10);
	}
}
