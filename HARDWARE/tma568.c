#include "tma568.h"
#include "delay.h"

const u16 CYTMA568_TPX_TBL[5]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};
// �������꼰�������
u16 TpX[TP_NUM_MAX];
u16 TpY[TP_NUM_MAX];
s16 TpNum=0;
/*****************************************************************

	�������ܣ�	��CYTMA568��ĳ����ʼ��ַд��len���ֽ�
	���룺		-
	���أ�		
				0: �ɹ�
				��0: ʧ��
	��������:	2015-7-18
	��ע��		��

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

	�������ܣ�	��CYTMA568��ĳ����ʼ��ַ��ȡlen���ֽ�
	���룺		-
	���أ�		��
	��������:	2015-7-18
	��ע��		��

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

	�������ܣ�	CYTMA568��ʼ��
	���룺		��
	���أ�
				0: �ɹ�
				��0: ʧ��
	��������:	2015-7-18
	��ע��		��

******************************************************************/ 
u8 CYTMA568_Init(void)
{
	u8 temp[2]; 

	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	//��λ�ܽ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    //�ж�����
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
	CYTMA568_WR_Reg(FT_DEVIDE_MODE,temp,1);//������������ģʽ
	CYTMA568_WR_Reg(FT_ID_G_MODE,temp,1);//��ѯģʽ
	temp[0]=23;	//������Чֵ��22��ԽСԽ����
	CYTMA568_WR_Reg(FT_ID_G_THGROUP,temp,1);//���ô�����Чֵ

	temp[0]=12;	//�������ڣ�����С��12�����14
	CYTMA568_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1);
	//��ȡ�汾�ţ��ο�ֵ��0x3003
	CYTMA568_RD_Reg(FT_ID_G_LIB_VERSION,temp,2);
	//��ȡ�汾�ţ��ο�ֵ��0x3003
	if(temp[0]==0X30&&temp[1]==0X03)//�汾:0X3003
	{
		return 0;
	} 
	return 1;
}

/*****************************************************************

	�������ܣ�	ɨ�败����(���ò�ѯ��ʽ)
	���룺		��
	���أ�		������ĸ���
	��������:	2015-7-18
	��ע��		��

******************************************************************/ 

u8 CYTMA568_Scan(void)
{
    static unsigned char ucTpState = 0;

	u8 buf[50];
	u8 i=0;
	u8 mode;
	u8 num=0;
	CYTMA568_RD_Reg(FT_REG_NUM_FINGER,buf,0x11);//��ȡ�������״̬
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
			CYTMA568_RD_Reg(CYTMA568_TPX_TBL[i],buf,4);//��ȡXY����ֵ
			TpX[i]=((u16)(buf[10])<<8)+buf[9];
			TpY[i]=319-(((u16)(buf[12])<<8)+buf[11]);
//  	    TpX[i] = 800 - TpX[i];
//  	    if((buf[0]&0XF0)!=0X80)//������contact�¼�������Ϊ��Ч
//  	    {
//  	      TpX[i]=TpY[i]=0;
//  	    }
		}       
		if((TpX[0]==0) && (TpY[0]==0)){//���������ݶ���0,����Դ˴�����
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
//	CYTMA568_RD_Reg(FT_REG_NUM_FINGER,buf,0x11);//��ȡ�������״̬ 
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
//			CYTMA568_RD_Reg(CYTMA568_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
//			TpY[i]=((u16)(buf[10])<<8)+buf[9];
//			TpX[i]=((u16)(buf[12])<<8)+buf[11];
////			TpX[i] = 800 - TpX[i];
////			if((buf[0]&0XF0)!=0X80)//������contact�¼�������Ϊ��Ч
////			{
////				TpX[i]=TpY[i]=0;
////			}
//		}
//		if((TpX[0]==0) && (TpY[0]==0)){ 	//���������ݶ���0,����Դ˴�����	
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