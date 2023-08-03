/******************************************************************************
创建者：康泉
创建日期：2018.03.01
功能描述：
    1. 系统主函数，各模块功能初始化。
    2. UI初始化，UI调度
    
更新记录：
    1. 2018.03.01 创建
*******************************************************************************/
#include "main.h"
#include "Config_ui.h"

FRESULT fRes;

u8 g_ucIWDogFeed_Enable = 1;//喂狗使能

char g_batFileBuf[20];

int main(void)
{
    BkLed_Init();
	__disable_irq(); //关闭总中断
	
	Led_Beep_Init();
	Sleep_ms(100);
	
	
	Key_Init();
	Power_Init();
	Bat_Adc_Init();
	USART2_Init(115200);
//	UART4_Init(115200);
	
	RX8010_Init();//时间
	CT1711_Init();//温度
	GPS_Init();//GPS
	
//	GT911_Init();
	FT6336_Init();//触摸
	
	InitSysPara();//系统参数
	
	
	LCD_Init();
	BkLedPWM_Init();
	SetBkLedPercent(25+25*g_SysPara.iBKLedLight);
	
	//文件系统
	W25QXX_Init();//初始化W25Q128
 	my_mem_init(SRAMIN);//初始化内部内存池
	exfuns_init();//为fatfs相关变量申请内存
	fRes = f_mount(fs[0],"0:",1);//挂载FLASH
	if(fRes != FR_OK){
		if(fRes == FR_NO_FILESYSTEM){//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
//			Lcd_DispAscStr(ASC6x8,40,4,"formatting");
			fRes = f_mkfs("0:",1,4096);//格式化FLASH,0:盘符;1,不需要引导区,8个扇区为1个簇
		}
	}
	
	fRes = f_mkdir("0:/GY91");
//	fRes = f_mkdir("0:/BMP");
//	fRes = f_mkdir("0:/FONT");
//	fRes = f_mkdir("0:/TEST");
    fRes = f_mkdir("0:/HD");
	
	//USB配置
	USB_Memery_Init_AK();
	USB_Interrupts_Config();
 	Set_USBClock();
 	USB_Init();
	
	//电量测试
	#ifdef BATTEST
	sprintf(g_batFileBuf,"0:Bat%03d.txt",g_SysPara.iBatFileCnt++);
	fRes = f_open(file,g_batFileBuf,FA_CREATE_ALWAYS);
	f_close(file);
	SaveSysPara();
	#endif
	
	font_init();//读取字库信息
	unioem_init();//读取转换表信息
	UiInit();

	//扫描文件
	g_GlobalPara.iSaveFileCnt = Scan_FileIndex(FileDirectory);
	
	SysTick_Init();
	TIM2_Int_Init(600,4799);//30ms，按键蜂鸣器
	
	TIM4_Int_Init(200,4799);//喂狗定时，单位时间20ms
	IWDG_Init(IWDG_Prescaler_128,1000);//溢出时间为1600ms
	g_ucIWDogFeed_Enable = 1;//==0时不喂狗
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	__enable_irq(); //关闭总中断
	
	//设置蓝牙
//	if(IsBtModelValid() == 0){//蓝牙无响应
//		CheckBtBaud();
//		SetBtBaud(BtBaud);
//		SetBtName(g_SysPara.BlueToothNum);
//	}
	//初始化蓝牙
	BT_Init();
	if(g_SysPara.iBlueToothSta == 0){//蓝牙在关闭状态
		BT_PowerOff();//关蓝牙电源
	}else{//蓝牙在开启状态
		BT_PowerOn();//开蓝牙电源
	}
	
	while(1){
		(*g_pRunFunc)();//*指针、()指参数类型
		UiSchedule();
	}
}
