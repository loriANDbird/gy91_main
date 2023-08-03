#ifndef __TOUCH_H
#define	__TOUCH_H

#include "mcu_io.h"

#define BTN_NUM_MAX							50		//按钮数量

//触摸事件返回值
#define BTN_NUM_PRESS_DOWN					0x40	//按钮按下事件返回值：按钮序号+0x40
#define BTN_NUM_LONGPRESS					0x80	//按钮长按事件返回值：按钮序号+0x80
#define BTN_NUM_LONGPRESS_HOLD				0xc0	//按钮持续长按事件返回值：按钮序号+0xc0
#define IS_BTN_PRESS_DOWN(BTN_NUM)			(BTN_NUM+0x40)	//按钮按下事件返回值：按钮序号+0x40
#define IS_BTN_LONGPRESS(BTN_NUM)			(BTN_NUM+0x80)	//按钮长按事件返回值：按钮序号+0x80
#define IS_BTN_LONGPRESS_HOLD(BTN_NUM)		(BTN_NUM+0xc0)	//按钮持续长按事件返回值：按钮序号+0xc0
#define BTN_INVALID_AREA					0xfc	//非按钮区域触摸
#define TOUCH_INVALID						0xfe	//无效触摸操作
#define TOUCH_NONE							0xff	//无触摸

#define	SLIDE_GAP_UP		0xf2	//上划,固定间距触发
#define	SLIDE_GAP_DOWN		0xf3	//下划,
#define	SLIDE_GAP_LEFT		0xf4	//左划,
#define	SLIDE_GAP_RIGHT		0xf5	//右划,

#define	SLIDE_END_UP		0xf6	//上划结束
#define	SLIDE_END_DOWN		0xf7	//下划结束
#define	SLIDE_END_LEFT		0xf8	//左划结束
#define	SLIDE_END_RIGHT		0xf9	//右划结束

#define SLIDE_BORDER_UP		0xfa	//滑动从边界开始，手势操作
#define SLIDE_BORDER_DOWN	0xfb
#define SLIDE_BORDER_LEFT	0xfc
#define SLIDE_BORDER_RIGHT	0xfd

//按钮属性
#define BTN_ABILITY_DEFAULT				0x0		//默认
#define BTN_ABILITY_SINGLE_CLICK		0x01	//单击
#define BTN_ABILITY_DOUBLE_CLICK		0x02	//双击
#define BTN_ABILITY_LONG_RRESS			0x04	//长按
#define BTN_ABILITY_LONG_PRESS_HOLD		0x08	//持续长按
#define BTN_ABILITY_DRAG				0x10	//拖拽

//按键状态
#define BTN_STATUS_NONE			0x0		//无触摸
#define BTN_STATUS_PRESS_DOWN	0x01	//按下
#define BTN_STATUS_PRESS_HOLD	0x02	//按下中
#define BTN_STATUS_PRESS_UP		0x03	//抬起
#define BTN_STATUS_DRAG			0x04	//拖动

//拖动方向
#define DRAG_HENG	0	//横向
#define DRAG_ZONG	1	//纵向


//触摸状态
typedef enum{
	PRESS_NONE = 0,		//0无触摸
	PRESS_DOWN,			//1触摸按下
	PRESS_UP,			//2触摸抬起
	SINGLE_CLICK,		//3单击
	DOUBLE_CLICK,		//4双击
	LONG_PRESS,			//5长按
	LONG_PRESS_HOLD,	//6持续长按
	SLIDE_GAP,			//7划动,固定间距触发
	SLIDE_GAP_HENG,		//8横向
	SLIDE_GAP_ZONG,		//9纵向
	SLIDE_END,			//10划动结束
	TOUCH_ING,			//触摸操作中
	TOUCH_ELSE,			//其他情况
}TOUCH_EVENT;


struct BUTTON{	
	
	u16 sX;
	u16 sY;
	u16 eX;
	u16 eY;
	u8 btSta;			//按键状态
	u8 btAbility;		//按键属性
};

struct BUTTON1{	
	
	u16 sX;
	u16 sY;
	u16 eX;
	u16 eY;
	u8 btSta;			//按键状态
	u8 btAbility;		//按键属性
	u16 color;			//颜色
	u16 colorTouch;		//按下颜色
	u8* name;			//名称
	u16 nameColor;		//
	u8 nameFont;		//
	
};

extern u8 g_u8ButtonNums;//按键个数
extern u8 g_u8TouchResult;
extern struct TP_DATA TPData;
extern struct BUTTON g_ButtonBuf[BTN_NUM_MAX];

u8 Get_Touch_Res(u8 btnums);
u8 Get_DragChannel(struct BUTTON* chBtn,u8 chQnt,u8 dir);
u8 Is_DesignArea_Touch(u16 sx,u16 sy,u16 ex,u16 ey);
u8 Is_DesignArea_Slide(u16 sx,u16 sy,u16 ex,u16 ey);

void ClearTouchSta(void);

#endif

