/*******************************************************************************
创建者：康泉
创建日期：2018.03.01
功能描述：
    1. 端口初始化
    2. 20mS按键扫描，返回结果：包括点动、长按2种
    3. 每次按键扫描有效，会更新全局变量  g_ucKeyVal， 大于零表示有键按下。
    4. 长按电源键，直接进行关机函数操作。
更新记录：
    1. 2018.03.01 新增
    
*******************************************************************************/
#include "mcu_io.h" 
#include "key.h"
#include "power.h"


u16 g_u16KeyVal;	
u16 g_u16KeyValBuf;	

u16 g_u16KeyValLast;
signed short int KeyDownDelayCnt;


//按键初始化函数
void Key_Init(void) //IO初始化
{     
	GPIO_InitTypeDef GPIO_InitStructure;    

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE); 	
   
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭 JTAG 功能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//Power
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//Key1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//Key2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
    

	g_u16KeyVal = 0;
	g_u16KeyValBuf = 0;
	g_u16KeyValLast = 0;
	KeyDownDelayCnt = 0;
}



u16 Key_Scan(void)
{	 
	u16 u16KeyFlag; 
    
	u16KeyFlag = 0x0000;
	
	if(IO_KEY_POWER == 0){//KEY1
//		u16KeyFlag |= KEY_POWER;
	}
	if(IO_KEY_BACK == 0){
		u16KeyFlag |= KEY_BACK;
	}
	if(IO_KEY_FN == 0){
		u16KeyFlag |= KEY_FN;
	}

	if((u16KeyFlag == g_u16KeyValLast)&&(g_u16KeyValLast != 0)) {
		KeyDownDelayCnt ++;	
		if(KeyDownDelayCnt >= 72){//80 1.6秒	 长按判断
//			if(u16KeyFlag == KEY_POWER){
				u16KeyFlag = u16KeyFlag | 0xFF00;  //设置长按标志位
//				KeyDownDelayCnt = 0;
				return u16KeyFlag;
//			}
		}
		if((KeyDownDelayCnt >= 80)&&(KeyDownDelayCnt % 10 == 0)){
			u16KeyFlag = u16KeyFlag | 0xEE00;  //设置长按标志位
			return u16KeyFlag;
		}
	}
	else{
		if((KeyDownDelayCnt > 3)&&(KeyDownDelayCnt < 80)){ //点按有效
			KeyDownDelayCnt = 0;
			if(g_u16KeyValLast > 0){
				u16KeyFlag = g_u16KeyValLast;
				return u16KeyFlag;
			}
		}
		if(u16KeyFlag != 0x00){
			g_u16KeyValLast = u16KeyFlag;	
		}
		else{
			g_u16KeyValLast = 0x00;
		}
		KeyDownDelayCnt = 0;
	}
	return 0;
}

