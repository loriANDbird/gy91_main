/******************************************************************************
�����ߣ���Ȫ
�������ڣ�2018.03.01
����������
    1. ϵͳ����������ģ�鹦�ܳ�ʼ����
    2. UI��ʼ����UI����
    
���¼�¼��
    1. 2018.03.01 ����
*******************************************************************************/
#include "main.h"
#include "Config_ui.h"

FRESULT fRes;

u8 g_ucIWDogFeed_Enable = 1;//ι��ʹ��

char g_batFileBuf[20];

int main(void)
{
    BkLed_Init();
	__disable_irq(); //�ر����ж�
	
	Led_Beep_Init();
	Sleep_ms(100);
	
	
	Key_Init();
	Power_Init();
	Bat_Adc_Init();
	USART2_Init(115200);
//	UART4_Init(115200);
	
	RX8010_Init();//ʱ��
	CT1711_Init();//�¶�
	GPS_Init();//GPS
	
//	GT911_Init();
	FT6336_Init();//����
	
	InitSysPara();//ϵͳ����
	
	
	LCD_Init();
	BkLedPWM_Init();
	SetBkLedPercent(25+25*g_SysPara.iBKLedLight);
	
	//�ļ�ϵͳ
	W25QXX_Init();//��ʼ��W25Q128
 	my_mem_init(SRAMIN);//��ʼ���ڲ��ڴ��
	exfuns_init();//Ϊfatfs��ر��������ڴ�
	fRes = f_mount(fs[0],"0:",1);//����FLASH
	if(fRes != FR_OK){
		if(fRes == FR_NO_FILESYSTEM){//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
//			Lcd_DispAscStr(ASC6x8,40,4,"formatting");
			fRes = f_mkfs("0:",1,4096);//��ʽ��FLASH,0:�̷�;1,����Ҫ������,8������Ϊ1����
		}
	}
	
	fRes = f_mkdir("0:/GY91");
//	fRes = f_mkdir("0:/BMP");
//	fRes = f_mkdir("0:/FONT");
//	fRes = f_mkdir("0:/TEST");
    fRes = f_mkdir("0:/HD");
	
	//USB����
	USB_Memery_Init_AK();
	USB_Interrupts_Config();
 	Set_USBClock();
 	USB_Init();
	
	//��������
	#ifdef BATTEST
	sprintf(g_batFileBuf,"0:Bat%03d.txt",g_SysPara.iBatFileCnt++);
	fRes = f_open(file,g_batFileBuf,FA_CREATE_ALWAYS);
	f_close(file);
	SaveSysPara();
	#endif
	
	font_init();//��ȡ�ֿ���Ϣ
	unioem_init();//��ȡת������Ϣ
	UiInit();

	//ɨ���ļ�
	g_GlobalPara.iSaveFileCnt = Scan_FileIndex(FileDirectory);
	
	SysTick_Init();
	TIM2_Int_Init(600,4799);//30ms������������
	
	TIM4_Int_Init(200,4799);//ι����ʱ����λʱ��20ms
	IWDG_Init(IWDG_Prescaler_128,1000);//���ʱ��Ϊ1600ms
	g_ucIWDogFeed_Enable = 1;//==0ʱ��ι��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	__enable_irq(); //�ر����ж�
	
	//��������
//	if(IsBtModelValid() == 0){//��������Ӧ
//		CheckBtBaud();
//		SetBtBaud(BtBaud);
//		SetBtName(g_SysPara.BlueToothNum);
//	}
	//��ʼ������
	BT_Init();
	if(g_SysPara.iBlueToothSta == 0){//�����ڹر�״̬
		BT_PowerOff();//��������Դ
	}else{//�����ڿ���״̬
		BT_PowerOn();//��������Դ
	}
	
	while(1){
		(*g_pRunFunc)();//*ָ�롢()ָ��������
		UiSchedule();
	}
}
