//#include "PublicApi.h"
//#include "Config_ui.h"
//#include "HcBT10.h" 



//////================================================================//
//////功能：显示电量百分比
//////参数：无
//////返回：无
//////================================================================//
////void DispBatValPer(int iVal)
////{  
////  int x = 790;//电池图标X起始地址
////  int y = 5;//电池图标Y起始地址
////  float fLen = 0.0;
////  fLen = 100*34/100;//进度条长度本应该为33，但由于取整问题 这里取34
////    
////  if(iVal > 9){
////    //x、y 为电池图标的起始位置
////    DispRect(x +4, y + 16, x +4 + fLen*iVal/100, y + 31, CL_WHITE);
////    DispRect(x + 4 + fLen*iVal/100, y + 16, x + 37, y + 31, CL_BLACK);
////  }
////  else{
////    DispRect(x +4, y + 16, x +4 + fLen*iVal/100, y + 31, CL_RED);
////    DispRect(x + 4 + fLen*iVal/100, y + 16, x + 37, y + 31, CL_BLACK);
////  }
////}

////================================================================//
////功能：计算均值，四舍六入五成双
////参数：无
////返回：无
////================================================================//
//int CountAvg(int Sum, int Count)
//{
//	int Res;
//	int Shang = 0, Yushu = 0;
//	Shang = Sum/Count;
//	Yushu = Sum*10%(Count*10)/Count;
//	
//	if(Yushu < 5){
//		Res = Shang;
//	}
//	else if(Yushu == 5){
//		if(Shang%2 == 0){
//			Res = Shang;
//		}
//		else{
//			Res = Shang + 1;
//		}
//	}
//	else{
//		Res = Shang + 1;
//	}
//	return Res;
//}
////================================================================//
////功能：显示电量百分比
////参数：无
////返回：无
////================================================================//
//void DispBatValPer(int iRegVal)
//{  
//  int x = 790;//电池图标X起始地址
//  int y = 5;//电池图标Y起始地址
//  int iPicLen = iRegVal/3 + 1;//计算图标长度
//  if(iRegVal > 9){
//    //x、y 为电池图标的起始位置
//    DispRect(x + 4, y + 16, x + 4 + iPicLen, y + 31, CL_WHITE);
//    DispRect(x + 4 + iPicLen, y + 16, x + 38, y + 31, CL_BLACK);
//  }
//  else if(iRegVal > 2){
//    DispRect(x + 4, y + 16, x + 4 + iPicLen, y + 31, CL_RED);
//    DispRect(x + 4 + iPicLen, y + 16, x + 38, y + 31, CL_BLACK);
//  }
//  else{
//	DispRect(x + 4, y + 16, x + 4 + 2, y + 31, CL_RED);
//	DispRect(x + 4 + 2, y + 16, x + 38, y + 31, CL_BLACK);
//  }
//}

////================================================================//
////功能：显示电池电量
////参数：无
////返回：无
////================================================================//
//void DispBatPer(void)
//{
//	int iBatPer = 0;
//	int i;
//	SetColor(CL_BLACK,CL_WHITE);//CL_BLUE
//	AXP202_charging_monitor();
//	iBatPer = AXP202_GetPer();//读取电量
////	iBatPer = 5;//测试任意百分比
//	sprintf(g_cBuf,"%3d%%",iBatPer);
//	DispText(FONT24,735,17,g_cBuf);//显示百分比
//	DispBatValPer(iBatPer);//刷新电池图标
//	if(iBatPer <= 3 && USB_IN == 0){
//		g_SysPara.cOffTime++;
//		if(g_SysPara.cOffTime >= 4){			
//			//锁定键盘
//			g_iKeyLockFlag = 1;
//		
//			//关机提示框
//			DisplayDialogBox();
//			SetColor(CL_SGRAY,CL_WHITE);//设置字体颜色
//			DispText(FONT28,340,227,"关机中");
//			for(i = 0; i < 3; i++){
//				DispText(FONT28,345+28*3 + 28*i,227 ,"痘");
//				delay_s(3);
//			}
//			ReDispLcdBox();
//			g_iKeyLockFlag = 0;
//			PwrOffFunc();
//		}
//	}
//	else{
//		g_SysPara.cOffTime = 0;
//	}
//}

////================================================================//
////功能：显示日期和时间
////参数：无
////返回：无
////================================================================//
//void DispDateAndTime(void)
//{
//	SetColor(CL_BLACK,CL_WHITE);//CL_BLUE
//	RX8010_ReadTime(&g_Rx8010Time);
//	sprintf(g_cBuf,"%d-%02d-%02d",g_Rx8010Time.year,g_Rx8010Time.month,g_Rx8010Time.day);
//	DispText(FONT24,20,18,g_cBuf);
//	sprintf(g_cBuf,"%02d:%02d:%02d",g_Rx8010Time.hour,g_Rx8010Time.min,g_Rx8010Time.sec);
//	DispText(FONT24,380,18,g_cBuf);
//}

////================================================================//
////功能：显示时间
////参数：无
////返回：无
////================================================================//
//void OnlyDispTime(void)
//{
//	SetColor(CL_BLACK,CL_WHITE);//CL_BLUE
//	RX8010_ReadTime(&g_Rx8010Time);
//	sprintf(g_cBuf,"%02d:%02d:%02d",g_Rx8010Time.hour,g_Rx8010Time.min,g_Rx8010Time.sec);
//	DispText(FONT24,380,18,g_cBuf);
//}

////================================================================//
////功能：显示蓝牙状态
////参数：无
////返回：无
////================================================================//
//void DispBTPower(void)
//{
//	//显示蓝牙和电池电量
//	SetColor(CL_BLACK,CL_WHITE);//CL_BLUE
//	if(g_SysPara.cBlueToothOnOffValue == 1){
//		DispText(FONT32,700,13,"牙");
//	}
//	else{
//		DispText(FONT32,700,13,"空");
//	}
//}

////================================================================//
////功能：字符串，居中对齐，计算位置偏移函数
////参数：*cStr		      				字符串指针
////			iLenMax								字符串最大长度
////			iCharWidth						字符宽度
////返回：像素偏移个数
////================================================================//
//int StringOffset(const char *cStr, int iLenMax, int iCharWidth)
//{
//	int iLen = 0;
//	int iOffsetValue = 0;
//	iLen = strlen(cStr);
//	if(iLen >= iLenMax){
//		iLen = iLenMax;
//	}
//	iOffsetValue = (iLenMax*iCharWidth - iLen*iCharWidth)/2;
//	return iOffsetValue;
//}

////================================================================//
////功能：字符串，居中对齐，计算位置偏移函数
////参数：*cStr		      				字符串指针
////			iLenMax								字符串最大长度
////			iCharWidth						字符宽度
////返回：像素偏移个数
////================================================================//
//void Delay(int idly)
//{
//	int delay,i;
//	for(i=0;i<idly;i++){
//		for(delay = 0;delay < 1000;delay++);
//	}
//}

