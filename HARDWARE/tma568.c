#include "tma568.h"
#include "delay.h"

const u16 CYTMA568_TPX_TBL[5]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};
// 触点坐标及触点个数
u16 TpX[TP_NUM_MAX];
u16 TpY[TP_NUM_MAX];
s16 TpNum=0;
/*****************************************************************

	函数功能：	向CYTMA568的某个起始地址写入len个字节
	输入：		-
	返回：		
				0: 成功
				非0: 失败
	更新日期:	2015-7-18
	备注：		无

******************************************************************/
u8 CYTMA568_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 return_val=0;
	I2C_Start();
	I2C_SendByte(FT_CMD_WR); 	 										  		   
	I2C_SendByte(reg&0XFF); 
	I2C_SendByte(reg>>8);
	for(i=0;i<len;i++)
	{
    	return_val=I2C_SendByte(buf[i]);
		if(return_val) break;
	}
    I2C_Stop();
	return return_val;
}

/*****************************************************************

	函数功能：	从CYTMA568的某个起始地址读取len个字节
	输入：		-
	返回：		无
	更新日期:	2015-7-18
	备注：		无

******************************************************************/
void CYTMA568_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	s8 i; 
 	I2C_Start();	
 	I2C_SendByte(FT_CMD_WR);
 	I2C_SendByte(reg&0XFF);
	I2C_SendByte(reg>>8); 
 	I2C_Start();  	 	   
	I2C_SendByte(FT_CMD_RD);
	for(i=0;i<len-1;i++)
	{
    	buf[i]=I2C_ReceiveByte();
		I2C_SendAck(0);
	}
	buf[i]=I2C_ReceiveByte();
	I2C_SendAck(1);
    I2C_Stop();
}

/*****************************************************************

	函数功能：	CYTMA568初始化
	输入：		无
	返回：
				0: 成功
				非0: 失败
	更新日期:	2015-7-18
	备注：		无

******************************************************************/ 
u8 CYTMA568_Init(void)
{
	u8 temp[2]; 

	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	//复位管脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    //中断引脚
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_SetBits(GPIOE, GPIO_Pin_4);

	I2C_Config();
	FT_RESET_L();
	Sleep_ms(20);
 	FT_RESET_H();			    
	Sleep_ms(50);
	
	temp[0]=0x30;
	temp[1]=0x03;
	
	temp[0]=0;
	CYTMA568_WR_Reg(FT_DEVIDE_MODE,temp,1);//进入正常操作模式
	CYTMA568_WR_Reg(FT_ID_G_MODE,temp,1);//查询模式
	temp[0]=23;	//触摸有效值，22，越小越灵敏
	CYTMA568_WR_Reg(FT_ID_G_THGROUP,temp,1);//设置触摸有效值

	temp[0]=12;	//激活周期，不能小于12，最大14
	CYTMA568_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1);
	//读取版本号，参考值：0x3003
	CYTMA568_RD_Reg(FT_ID_G_LIB_VERSION,temp,2);
	//读取版本号，参考值：0x3003
	if(temp[0]==0X30&&temp[1]==0X03)//版本:0X3003
	{
		return 0;
	} 
	return 1;
}

/*****************************************************************

	函数功能：	扫描触摸屏(采用查询方式)
	输入：		无
	返回：		触摸点的个数
	更新日期:	2015-7-18
	备注：		无

******************************************************************/ 

u8 CYTMA568_Scan(void)
{
    static unsigned char ucTpState = 0;

	u8 buf[50];
	u8 i=0;
	u8 mode;
	u8 num=0;
	CYTMA568_RD_Reg(FT_REG_NUM_FINGER,buf,0x11);//读取触摸点的状态
	mode = buf[0];
	if(mode >= 0x11){
		mode = (mode-7)/10;
        ucTpState = 1;
	}
    else if(((mode == 0x07)||(mode == 0x00))&&(ucTpState == 1)){
        ucTpState = 0;
		TpNum=0;
		return 0x55;
	}else{
		TpNum=0;
		return 0;
	}
	num=mode&0XF;
	if((num)&&(num<=TP_NUM_MAX)){
		for(i=0;i<num;i++){
			CYTMA568_RD_Reg(CYTMA568_TPX_TBL[i],buf,4);//读取XY坐标值
			TpX[i]=((u16)(buf[10])<<8)+buf[9];
			TpY[i]=319-(((u16)(buf[12])<<8)+buf[11]);
//  	    TpX[i] = 800 - TpX[i];
//  	    if((buf[0]&0XF0)!=0X80)//必须是contact事件，才认为有效
//  	    {
//  	      TpX[i]=TpY[i]=0;
//  	    }
		}       
		if((TpX[0]==0) && (TpY[0]==0)){//读到的数据都是0,则忽略此次数据
			num=0;
		}
	}
	TpNum=num;
	if(TpNum > 1){
		FT_RESET_L();
		Sleep_ms(5);
		FT_RESET_H();
		Sleep_ms(5);
	}
    
    if((num)&&(num<=TP_NUM_MAX)){
        return num;
    }
    else{
        return 0;
    }
}

//u8 CYTMA568_Scan(void)
//{
//	u8 buf[50];
//	u8 i=0;
//	u8 mode;
//	u8 num=0;
//	CYTMA568_RD_Reg(FT_REG_NUM_FINGER,buf,0x11);//读取触摸点的状态 
//	mode = buf[0];
//	if(mode>=0x11){
//		mode=(mode-7)/10;
//	}else{
//		TpNum=0;
//		return 0;
//	}
//	num=mode&0xF;
//	if((num)&&(num<=TP_NUM_MAX)){
//		for(i=0;i<num;i++){
//			CYTMA568_RD_Reg(CYTMA568_TPX_TBL[i],buf,4);	//读取XY坐标值
//			TpY[i]=((u16)(buf[10])<<8)+buf[9];
//			TpX[i]=((u16)(buf[12])<<8)+buf[11];
////			TpX[i] = 800 - TpX[i];
////			if((buf[0]&0XF0)!=0X80)//必须是contact事件，才认为有效
////			{
////				TpX[i]=TpY[i]=0;
////			}
//		}
//		if((TpX[0]==0) && (TpY[0]==0)){ 	//读到的数据都是0,则忽略此次数据	
//			num=0;
//		}
//	}
//	TpNum=num;
//	if(TpNum > 1){
//		FT_RESET_L();
//		Sleep_ms(5);
//		FT_RESET_H();
//		Sleep_ms(5);
//	}
//    
//    if((num)&&(num<=TP_NUM_MAX)){
//        return num;
//    }else{
//        return 0;
//    }
//}