#include "power.h"
#include "ledBeep.h"
#include "bkLed.h" 
#include "delay.h"
#include "Config_ui.h"

void Power_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;    
	
	RCC_APB2PeriphClockCmd(POWER_RCC|USBEN_RCC|CarPOWER_RCC|CHARGE_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = POWER_PIN;//�����Դ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(POWER_PORT, &GPIO_InitStructure);
	MainPowerOff();//��Դ��
	
	GPIO_InitStructure.GPIO_Pin = CarPOWER_PIN;//�ɼ����Դ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CarPOWER_PORT, &GPIO_InitStructure);
	CarPowerOff();//��Դ��
	
	GPIO_InitStructure.GPIO_Pin = USBEN_PIN;//USBʹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USBEN_PORT, &GPIO_InitStructure);
	USB_Enable();//ʹ��USB
	
	GPIO_InitStructure.GPIO_Pin = CHARGE_PIN|CHARGEDONE_PIN;//���״̬
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CHARGE_PORT, &GPIO_InitStructure);
	
	PwrOnFunc();//����
	//while(ReadPwrKeySta() == 0){};  //�ȴ���Դ����̧��
}


//���ܵ�Դ
void PwrOnFunc(void)
{
	MainPowerOn();
	CarPowerOn();
	PromptTone_Once(200);//��ʾ��
}
//���ܵ�Դ
void PwrOffFunc(void)
{
// 	__disable_irq(); //�ر����ж�     
//	PromptTone_Twice(135);//��ʾ��
//	
//	SetBkLedPercent(0);
//	CarPowerOff();
//	MainPowerOff();   //PowerCtrl   Off
//    while(1){
//		Sleep_ms(500);
//    }
}

//��⿪����ʽ
void CheckOpenType(void)
{
	if((ReadPwrKeySta() == 0)&&(ReadChargeSta() == 0)&&(ReadChargeDoneSta() == 0)){//��������
		g_GlobalPara.iOpenType = OPEN_KEY;
		g_GlobalPara.iChargeSta = CHARGE_NO;
		while(ReadPwrKeySta() == 0){};  //�ȴ���Դ����̧��
	}
	if((ReadPwrKeySta() == 1)&&((ReadChargeSta() == 1)||(ReadChargeDoneSta() == 1))){//��翪��
		g_GlobalPara.iOpenType = OPEN_CHARGE;
		g_GlobalPara.iChargeSta = CHARGE_ING;
	}
}

//�����״̬
void CheckChargeStep(void)
{
	if((ReadChargeSta() == 0)&&(ReadChargeDoneSta() == 0)){//δ���
		if((g_GlobalPara.iChargeSta != CHARGE_ING)&&(g_GlobalPara.iChargeSta != CHARGE_DONE)){
			g_GlobalPara.iChargeSta = CHARGE_NO;
		}
	}
	if((ReadChargeSta() == 1)&&(ReadChargeDoneSta() == 0)){//�����
		g_GlobalPara.iChargeSta = CHARGE_ING;

	}
	if((ReadChargeSta() == 0)&&(ReadChargeDoneSta() == 1)){//������
		g_GlobalPara.iChargeSta = CHARGE_DONE;
	}
	
	Sleep_ms(1);
}
//�����״̬
//����ֵ��0:��״̬�仯��1:��ʼ��磬2:�����ɣ�3:�ε������
u8 CheckChargeSta(void)
{
	static int chargeSta_L = CHARGE_NO; 
//	if((ReadChargeSta() == 0)&&(ReadChargeDoneSta() == 0)){//δ���
//		g_GlobalPara.iChargeSta = CHARGE_NO;
//		LCD_SetColor(WHITE,BLACK);
//		LCD_ShowChinese(Font12,450,40,0,"��");
//	}
//	if((ReadChargeSta() == 1)&&(ReadChargeDoneSta() == 0)){//�����
//		g_GlobalPara.iChargeSta = CHARGE_ING;
//		LCD_SetColor(WHITE,BLACK);
//		LCD_ShowChinese(Font12,450,40,0,"��");
//	}
//	if((ReadChargeSta() == 0)&&(ReadChargeDoneSta() == 1)){//������
//		g_GlobalPara.iChargeSta = CHARGE_DONE;
//	}
	
	if((g_GlobalPara.iChargeStaRes == CHARGE_ING)&&(chargeSta_L == CHARGE_NO)){//��ʼ���
		chargeSta_L = g_GlobalPara.iChargeStaRes;
		return 1;
	}
	if((g_GlobalPara.iChargeStaRes == CHARGE_DONE)&&(chargeSta_L == CHARGE_ING)){//������
		chargeSta_L = g_GlobalPara.iChargeStaRes;
		return 2;
	}
	if((g_GlobalPara.iChargeStaRes == CHARGE_NO)&&(chargeSta_L != CHARGE_NO)){//�ε������
//		if(g_SamplePara.iOpenType == 1){//��翪��
//			PwrOffFunc();
//		}
		chargeSta_L = g_GlobalPara.iChargeStaRes;
		return 3;
	}
	chargeSta_L = g_GlobalPara.iChargeStaRes;
	return 0;
}
