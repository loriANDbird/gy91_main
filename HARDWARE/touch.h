#ifndef __TOUCH_H
#define	__TOUCH_H

#include "mcu_io.h"

#define BTN_NUM_MAX							50		//��ť����

//�����¼�����ֵ
#define BTN_NUM_PRESS_DOWN					0x40	//��ť�����¼�����ֵ����ť���+0x40
#define BTN_NUM_LONGPRESS					0x80	//��ť�����¼�����ֵ����ť���+0x80
#define BTN_NUM_LONGPRESS_HOLD				0xc0	//��ť���������¼�����ֵ����ť���+0xc0
#define IS_BTN_PRESS_DOWN(BTN_NUM)			(BTN_NUM+0x40)	//��ť�����¼�����ֵ����ť���+0x40
#define IS_BTN_LONGPRESS(BTN_NUM)			(BTN_NUM+0x80)	//��ť�����¼�����ֵ����ť���+0x80
#define IS_BTN_LONGPRESS_HOLD(BTN_NUM)		(BTN_NUM+0xc0)	//��ť���������¼�����ֵ����ť���+0xc0
#define BTN_INVALID_AREA					0xfc	//�ǰ�ť������
#define TOUCH_INVALID						0xfe	//��Ч��������
#define TOUCH_NONE							0xff	//�޴���

#define	SLIDE_GAP_UP		0xf2	//�ϻ�,�̶���ഥ��
#define	SLIDE_GAP_DOWN		0xf3	//�»�,
#define	SLIDE_GAP_LEFT		0xf4	//��,
#define	SLIDE_GAP_RIGHT		0xf5	//�һ�,

#define	SLIDE_END_UP		0xf6	//�ϻ�����
#define	SLIDE_END_DOWN		0xf7	//�»�����
#define	SLIDE_END_LEFT		0xf8	//�󻮽���
#define	SLIDE_END_RIGHT		0xf9	//�һ�����

#define SLIDE_BORDER_UP		0xfa	//�����ӱ߽翪ʼ�����Ʋ���
#define SLIDE_BORDER_DOWN	0xfb
#define SLIDE_BORDER_LEFT	0xfc
#define SLIDE_BORDER_RIGHT	0xfd

//��ť����
#define BTN_ABILITY_DEFAULT				0x0		//Ĭ��
#define BTN_ABILITY_SINGLE_CLICK		0x01	//����
#define BTN_ABILITY_DOUBLE_CLICK		0x02	//˫��
#define BTN_ABILITY_LONG_RRESS			0x04	//����
#define BTN_ABILITY_LONG_PRESS_HOLD		0x08	//��������
#define BTN_ABILITY_DRAG				0x10	//��ק

//����״̬
#define BTN_STATUS_NONE			0x0		//�޴���
#define BTN_STATUS_PRESS_DOWN	0x01	//����
#define BTN_STATUS_PRESS_HOLD	0x02	//������
#define BTN_STATUS_PRESS_UP		0x03	//̧��
#define BTN_STATUS_DRAG			0x04	//�϶�

//�϶�����
#define DRAG_HENG	0	//����
#define DRAG_ZONG	1	//����


//����״̬
typedef enum{
	PRESS_NONE = 0,		//0�޴���
	PRESS_DOWN,			//1��������
	PRESS_UP,			//2����̧��
	SINGLE_CLICK,		//3����
	DOUBLE_CLICK,		//4˫��
	LONG_PRESS,			//5����
	LONG_PRESS_HOLD,	//6��������
	SLIDE_GAP,			//7����,�̶���ഥ��
	SLIDE_GAP_HENG,		//8����
	SLIDE_GAP_ZONG,		//9����
	SLIDE_END,			//10��������
	TOUCH_ING,			//����������
	TOUCH_ELSE,			//�������
}TOUCH_EVENT;


struct BUTTON{	
	
	u16 sX;
	u16 sY;
	u16 eX;
	u16 eY;
	u8 btSta;			//����״̬
	u8 btAbility;		//��������
};

struct BUTTON1{	
	
	u16 sX;
	u16 sY;
	u16 eX;
	u16 eY;
	u8 btSta;			//����״̬
	u8 btAbility;		//��������
	u16 color;			//��ɫ
	u16 colorTouch;		//������ɫ
	u8* name;			//����
	u16 nameColor;		//
	u8 nameFont;		//
	
};

extern u8 g_u8ButtonNums;//��������
extern u8 g_u8TouchResult;
extern struct TP_DATA TPData;
extern struct BUTTON g_ButtonBuf[BTN_NUM_MAX];

u8 Get_Touch_Res(u8 btnums);
u8 Get_DragChannel(struct BUTTON* chBtn,u8 chQnt,u8 dir);
u8 Is_DesignArea_Touch(u16 sx,u16 sy,u16 ex,u16 ey);
u8 Is_DesignArea_Slide(u16 sx,u16 sy,u16 ex,u16 ey);

void ClearTouchSta(void);

#endif

