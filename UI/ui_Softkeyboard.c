///*******************************************************************************
//创建者：赵基程
//创建日期：2018.05.07
//功能描述：
//    1. ui_Softkeyboard_Init 软键盘初始化，
//    2. ui_Sample10_Run 函数实现
//更新记录：
//    1. 2018.03.01 初次创建
//    
//*******************************************************************************/
//#include "Config_ui.h"
//#include "ui_Softkeyboard.h"
//#include "lcd.h"
//#include "KongJian.h"
//#include "ui_Sample.h"

//static int k_nHighID = 0;
//char g_KeyBoardBuffer[10] = {0};
//int m_iParaHighLightPos;//参数高亮位置
//int m_iParaValLen;//参数最大长度
//int m_iParaMaxLen;//最大长度

//STRUCT_SAPM_BOARD  *m_keyboard;

//void ui_Softkeyboard_Init(void)
//{
//	int i = 0;
//	k_nHighID = 0;
//	//InitKJ(SoftKeyboard_kj_Bottom);
//	m_keyboard = mymalloc(SRAMIN,sizeof(STRUCT_SAPM_BOARD));
//	memset(m_keyboard, 0, sizeof(STRUCT_SAPM_BOARD));
//	sprintf(m_keyboard->keyVal,"~0123456789$ABCDEFGHIJKLMNOPQRSTUVWXYZ#!_+-^()=;");
//	
////	SetColor(CL_BLACK,CL_WHITE);//test
////	DispText(FONT32,0,80,m_keyboard->keyVal);//test
//	
//	//	DispRect(0,184,853,224,CL_BLACK);//画背景
//	for(i=1;i<12;i++){
//		DispRect(i*71-1,230,i*71-1,480,CL_BLACK);
//	}
//	DispRect(0,185,854,236,CL_LIGHTGRAY);
//	
//	DispRect(0,237,854,237,CL_BLACK);
//	DispRect(0,238+61-1,854,238+61-1,CL_BLACK);
//	DispRect(0,238+61*2-1,854,238+61*2-1,CL_BLACK);
//	DispRect(0,238+61*3-1,854,238+61*3-1,CL_BLACK);
//	
//	DispOneKey(0,1);
//	for(i=1;i<48;i++){
//		DispOneKey(i,0);
//	}
//	
//	m_iParaMaxLen = 8;
//	sprintf(g_cBuf,"轴线");
//	if((m_keyboard->ParaName[0] == g_cBuf[0])&&(m_keyboard->ParaName[1] == g_cBuf[1])){
//		m_iParaMaxLen = 40;
//	}
//	
//	sprintf(m_keyboard->ParaName,"构件编号");//   --->构件编号  工程名称
//	sprintf(m_keyboard->chrDisParaVal,"%s",g_KeyBoardBuffer);
//	m_iParaHighLightPos = 0;
//	DispParaName2Val(m_iParaHighLightPos);
//#ifdef TESTMEM
//	sprintf(g_cBuf,"%d",my_mem_perused(SRAMIN));
//	SetColor(CL_BLACK,CL_SGRAY);//CL_BLUE
//	DispText(FONT24,650,20,g_cBuf);
//#endif
//}

//void DispParaName2Val(int iParaHighLihtPos)
//{
//	int x,y,ilen;

//	x = 10;
//	y = 199;
//	SetColor(CL_LIGHTGRAY,CL_BLACK);//CL_BLUE	
//	DispText(FONT24,x,y,m_keyboard->ParaName);
//	DispText(FONT24,120+x,y,m_keyboard->chrDisParaVal);
//	ilen = strlen(m_keyboard->chrDisParaVal);
//	if(ilen < 8){//清除尾巴
//		DispRect(120+ x + 12*ilen,y,120+x+96,y+23,CL_LIGHTGRAY);
//	}
//	if(ilen < 1){
//		ilen = 1;
//	}
//	m_iParaValLen = ilen;
//	
//	if(iParaHighLihtPos < 0){
//		iParaHighLihtPos = ilen-1;
//		m_iParaHighLightPos = iParaHighLihtPos;
//	}
//	//画黄色下划线
//	DispRect(120+x,y+25,120+x+100,y+26,CL_LIGHTGRAY);
//	DispRect(120+x+iParaHighLihtPos*12,y+25,120+x+iParaHighLihtPos*12 + 12,y+26,CL_SYELLOW);

//}

//void DispOneKey(int kNum,u8 kHighLight)
//{
//	int x,y;
//	x = (kNum%12)*71;
//	y = 238+(kNum/12)*61;
//	
//	if(kHighLight == 1){
//		DispRect(x,y,x+69,y+59,CL_YELLOW);
//		SetColor(CL_YELLOW,CL_BLACK);//CL_BLUE
//		DispASC(ASC20x32,x+(69-20)/2 + 3,y+(59-32)/2 - 3,m_keyboard->keyVal[kNum]);
//	}
//	else{
//		DispRect(x,y,x+69,y+59,CL_LIGHTGRAY);
//		SetColor(CL_LIGHTGRAY,CL_BLACK);//CL_BLUE
//		DispASC(ASC20x32,x+(69-20)/2 + 3,y+(59-32)/2 - 3,m_keyboard->keyVal[kNum]);
//	}
//}

////---------------------------------------------------------------------------------------------------------------
////UI运行函数
//void ui_Softkeyboard_Run(void)
//{
//	while(1){
//		if(g_bSysTick20mS == 1){
//			g_bSysTick20mS = 0;
//			g_iTime20mSCnt += 1;
//			if(g_iTime20mSCnt % 250 == 0){ //500   10S
//				//BatTest();
//			}
//			if(g_iTime20mSCnt % 50 == 0){  //1S
////				RX8010_ReadTime(&g_Rx8010Time);
////				
////				SetColor(CL_BLACKA,CL_WHITE);//CL_BLUE
////				sprintf(g_cBuf,"%02d:%02d:%02d",g_Rx8010Time.hour,g_Rx8010Time.min,g_Rx8010Time.sec);
////				DispText(FONT24,370,16,g_cBuf);
//			}
//		}

//		if(g_u16KeyVal > 0){
//			g_u16KeyValBuf = g_u16KeyVal;
//			g_u16KeyVal = 0;
//			if(SoftKeyboard_kj_Bottom() > 0){
//				return;
//			}
//		}
//	}
//}

//int SoftKeyboard_kj_Bottom(void)
//{
//	int ret = -1;	
//	int i;
//	
//	if(g_u16KeyValBuf == SHIFT_KEY){ 	//按切换键，
//		ret = 0;
//	}
//	
//	if(g_u16KeyValBuf == LEFT_KEY){     //按左键
//		DispOneKey(k_nHighID,0);
//		k_nHighID --;
//		if(k_nHighID%12 == 11){
//			k_nHighID += 12;
//		}
//		if(k_nHighID < 0){
//			k_nHighID = 11;
//		}

//		DispOneKey(k_nHighID,1);
//		ret = 0;
//	}
//	else if(g_u16KeyValBuf == RIGHT_KEY){ //按右键
//		DispOneKey(k_nHighID,0);
//		k_nHighID ++;
////			if(k_nHighID >47){
////				k_nHighID = 47;
////			}
//		if(k_nHighID%12 == 0){
//			k_nHighID -= 12;
//		}
//		DispOneKey(k_nHighID,1);
//		ret = 0;
//	}
//	else if(g_u16KeyValBuf == UP_KEY){ //按上键
//		DispOneKey(k_nHighID,0);
//		k_nHighID-=12;
//		if(k_nHighID <0){
//			k_nHighID+=48;
//		}
//		DispOneKey(k_nHighID,1);
//		ret = 0;
//	}
//	else if(g_u16KeyValBuf == DOWN_KEY){ //按下键
//		DispOneKey(k_nHighID,0);
//		k_nHighID+=12;
//		if(k_nHighID > 47){
//			k_nHighID-=48;
//		}
//		DispOneKey(k_nHighID,1);
//		ret = 0;
//	}
//	else if(g_u16KeyValBuf == OK_KEY){ //按OK键
//		if(k_nHighID == 11){ //删除按钮
//			//光标在最后面
//			if((m_iParaValLen > 0)&(m_iParaHighLightPos >= (m_iParaValLen-1))){ 
//				m_keyboard->chrDisParaVal[m_iParaHighLightPos] = 0;
//				m_iParaHighLightPos = -1;
//				DispParaName2Val(m_iParaHighLightPos);
//			}
//			//光标在中间
//			else if(m_iParaHighLightPos < (m_iParaValLen-1)){
//				for(i=m_iParaHighLightPos;i<(m_iParaValLen-1);i++){
//					m_keyboard->chrDisParaVal[i] = m_keyboard->chrDisParaVal[i+1];
//				}
//				m_keyboard->chrDisParaVal[i] = 0; //清除尾巴
//				
//				DispParaName2Val(m_iParaHighLightPos);
//			}
//		}
//		else if(k_nHighID == 46){ //左移光标
//			m_iParaHighLightPos -= 1;
//			if(m_iParaHighLightPos < 0){
//				m_iParaHighLightPos = m_iParaValLen - 1;
//			}
//			DispParaName2Val(m_iParaHighLightPos);
//		}
//		else if(k_nHighID == 47){ //右移光标
//			m_iParaHighLightPos += 1;
//			if(m_iParaHighLightPos >= m_iParaValLen){
//				m_iParaHighLightPos = 0;
//			}
//			if(m_iParaHighLightPos > (m_iParaMaxLen-1)){
//				m_iParaHighLightPos = 0;
//			}
//			DispParaName2Val(m_iParaHighLightPos);
//		}
//		else{
//			if(m_iParaMaxLen == 8){ //工程名称、构件名称
//				if((k_nHighID >= 0)&(k_nHighID < 46)){
//					if((m_iParaHighLightPos>0)&&(m_keyboard->chrDisParaVal[m_iParaHighLightPos - 1] == 0)){
//						m_keyboard->chrDisParaVal[m_iParaHighLightPos - 1] = m_keyboard->keyVal[k_nHighID];
//					}
//					else{
//						m_keyboard->chrDisParaVal[m_iParaHighLightPos] = m_keyboard->keyVal[k_nHighID];
//					}
//					
//					m_iParaValLen = strlen(m_keyboard->chrDisParaVal);
//					
//					m_iParaHighLightPos += 1;
//					if(m_iParaHighLightPos >= m_iParaValLen){
//						m_iParaHighLightPos = m_iParaValLen;
//					}
//					if(m_iParaHighLightPos > (m_iParaMaxLen-1)){
//						m_iParaHighLightPos = (m_iParaMaxLen-1);
//					}
//					DispParaName2Val(m_iParaHighLightPos);
//				}				
//			}
//			else{
//			
//			}
//		}
//		//m_keyboard->chrDisParaVal
//		ret = 0;
//	}
//	
//	if(g_u16KeyValBuf == CANCEL_KEY){//按返回键
//		memset(g_KeyBoardBuffer,0,10);
//		sprintf(g_KeyBoardBuffer,"%s",m_keyboard->chrDisParaVal);
//		myfree(SRAMIN,m_keyboard);
//		
//		if(g_KeyBoardBuffer[0] == 0){
//			sprintf(g_KeyBoardBuffer,"GYHD0001");
//		}
//		
//		
//		//获取高亮图标号 g_cLastUiNo
//		switch(g_cLastUiNo)
//		{
//			case UI_SAMPLE10-1:
//				SendUiNo(UI_SAMPLE10);
//				break;
//			case UI_SAMPLE20-1:
//				SendUiNo(UI_SAMPLE20);
//				break;
//			case UI_SAMPLE30-1:
//				SendUiNo(UI_SAMPLE30);
//				break;
//		}
//		return 1;
//	}
//	return ret;
//}

