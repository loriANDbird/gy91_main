#include "ft6336u.h"
#include "string.h"
#include "mcu_io.h"
#include "delay.h"
#include "lcd.h"
#include "iic.h"

//IIC IO��������
#define IIC_SCL			PCout(4) //SCL
#define IIC_SDA			PCout(5) //SDA
#define READ_SDA		PCin(5)  //����SDA

#define IIC_RCC			RCC_APB2Periph_GPIOC
#define IIC_PORT		GPIOC
#define SCL_PIN			GPIO_Pin_4
#define SDA_PIN			GPIO_Pin_5

//GT911 IO��������
#define TP_RST_RCC		RCC_APB2Periph_GPIOB
#define TP_RST_PORT		GPIOB
#define TP_RST_PIN		GPIO_Pin_2

#define TP_INT_RCC		RCC_APB2Periph_GPIOB
#define TP_INT_PORT		GPIOB
#define TP_INT_PIN		GPIO_Pin_1
#define TP_EXTI_PORT	GPIO_PortSourceGPIOB
#define TP_EXTI_PIN		GPIO_PinSource1
#define TP_EXTI_LINE	EXTI_Line1


#define TP_RST_H		GPIO_SetBits(TP_RST_PORT, TP_RST_PIN)
#define TP_RST_L		GPIO_ResetBits(TP_RST_PORT, TP_RST_PIN)

#define TP_INT_H		GPIO_SetBits(TP_INT_PORT, TP_INT_PIN)
#define TP_INT_L		GPIO_ResetBits(TP_INT_PORT, TP_INT_PIN)
#define TP_INT_IN		GPIO_ReadInputDataBit(TP_INT_PORT, TP_INT_PIN)

static u8 TouchReady = 0;
static u8 TPN = 0;
static u16 TPX1 = 0;
static u16 TPY1 = 0;

const u8 GT911_Cfg[] = {
	0x5A,0x40,0x01,0xE0,0x01,0x01,0x05,0x00,0x01,0x08,0x28,0x05,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x86,0x26,0x08,0x19,0x16,0x12,0x0C,0x00,0x00,0x00,0x99,0x04,0x1D,
	0x00,0x00,0x00,0x00,0x00,0x03,0x64,0x32,0x00,0x00,0x00,0x0F,0xA1,0x94,0xC5,0x02,0x07,0x00,0x00,0x04,
	0x93,0x13,0x00,0x5E,0x1F,0x00,0x3D,0x32,0x00,0x28,0x51,0x00,0x1D,0x82,0x00,0x1D,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x10,0x0E,0x0C,0x0A,0x08,0x06,0x04,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x24,0x22,0x21,0x20,0x1F,0x1E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00
};//,0x5E,0x01

void TP_delay_ms(int nms)
{
	int idly,i;
	for(i=0;i<nms;i++){
		for(idly = 0;idly < 10000;idly++);
	}
}

void TP_delay_us(int nus)
{
	int idly,i;
	for(i=0;i<nus;i++){
		for(idly = 0;idly < 10;idly++);
	}
}

void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(IIC_RCC, ENABLE);
    
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStructure);       
}
void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(IIC_RCC, ENABLE);
    
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStructure);
}
//��ʼ��IIC
void IIC_Init(void)
{                        
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(IIC_RCC, ENABLE );   
	
	GPIO_InitStructure.GPIO_Pin = SCL_PIN|SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStructure);
	GPIO_SetBits(IIC_PORT,SCL_PIN|SDA_PIN);
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();   
	IIC_SDA=1;           
	IIC_SCL=1;
	TP_delay_us(4);
	IIC_SDA=0;//START:when CLK is high,DATA change form high to low
	TP_delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	TP_delay_us(4);
	IIC_SCL=1;
	IIC_SDA=1;//����I2C���߽����ź�
	TP_delay_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ���� 
	IIC_SDA=1;TP_delay_us(1);      
	IIC_SCL=1;TP_delay_us(1);    
	while(READ_SDA){
		ucErrTime++;
		if(ucErrTime>250){
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0       
	return 0; 
}
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	TP_delay_us(2);
	IIC_SCL=1;
	TP_delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��           
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	TP_delay_us(2);
	IIC_SCL=1;
	TP_delay_us(2);
	IIC_SCL=0;
}                                         
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��             
void IIC_Send_Byte(u8 txd)
{                       
	u8 t;  
	SDA_OUT();        
	IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++){
		//IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7){
			IIC_SDA=1;
		}else{
			IIC_SDA=0;
		}
		txd<<=1;
		TP_delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		TP_delay_us(2);
		IIC_SCL=0;
		TP_delay_us(2);
	}
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
	for(i=0;i<8;i++ ){
		IIC_SCL=0;
		TP_delay_us(2);
		IIC_SCL=1;
		receive<<=1;
		if(READ_SDA)receive++;
		TP_delay_us(1);
	}
	if(!ack){
		IIC_NAck();//����nACK
	}else{
		IIC_Ack(); //����ACK
	}
	return receive;
}

/***************************************GT911**************************************/
//�ж��������
void TP_INT_Out_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     RCC_APB2PeriphClockCmd(TP_INT_RCC, ENABLE);
     GPIO_InitStructure.GPIO_Pin = TP_INT_PIN;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(TP_INT_PORT, &GPIO_InitStructure);
     GPIO_ResetBits(TP_INT_PORT, TP_INT_PIN);
}
//�ж���������
void TP_INT_In_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     RCC_APB2PeriphClockCmd(TP_INT_RCC, ENABLE);
     GPIO_InitStructure.GPIO_Pin = TP_INT_PIN;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(TP_INT_PORT, &GPIO_InitStructure);
     GPIO_ResetBits(TP_INT_PORT, TP_INT_PIN);   
}
//�ж������ⲿ�ж�
void TP_INT_EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
   
    RCC_APB2PeriphClockCmd(TP_INT_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin =  TP_INT_PIN;
    GPIO_Init(TP_INT_PORT, &GPIO_InitStructure);
		
	//�ⲿ�ж�
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

    //GPIOB.5 �ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(TP_EXTI_PORT,TP_EXTI_PIN);

  	EXTI_InitStructure.EXTI_Line=TP_EXTI_LINE;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  
}
//��λ����
void TP_RST_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     RCC_APB2PeriphClockCmd(TP_RST_RCC, ENABLE);
     GPIO_InitStructure.GPIO_Pin = TP_RST_PIN;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(TP_RST_PORT, &GPIO_InitStructure);
     GPIO_ResetBits(TP_RST_PORT, TP_RST_PIN);
}


//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 TP_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	IIC_Start();   
	IIC_Send_Byte(TP_CMD_WR);       //����д����     
	IIC_Wait_Ack();
//	IIC_Send_Byte(reg>>8);       //���͸�8λ��ַ
//	IIC_Wait_Ack();
	IIC_Send_Byte(reg&0XFF);       //���͵�8λ��ַ
	IIC_Wait_Ack();
	for(i=0;i<len;i++){
		IIC_Send_Byte(buf[i]);      //������
		ret=IIC_Wait_Ack();
		if(ret)break;
	}
	IIC_Stop();                    //����һ��ֹͣ����       
	return ret;
}

//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���
void TP_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	IIC_Start();
	IIC_Send_Byte(TP_CMD_WR);   //����д����
	IIC_Wait_Ack();
//	IIC_Send_Byte(reg>>8);       //���͸�8λ��ַ
//	IIC_Wait_Ack();
	IIC_Send_Byte(reg&0XFF);       //���͵�8λ��ַ
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(TP_CMD_RD);   //���Ͷ�����
	IIC_Wait_Ack();
	for(i=0;i<len;i++){
		buf[i]=IIC_Read_Byte(i==(len-1)?0:1); //������     
	}
	IIC_Stop();//����һ��ֹͣ����
}

//************************GT911*************************//
//�������ò���
//mode:0,���������浽flash
//     1,�������浽flash
u8 GT911_Send_Cfg(u8 mode)
{
	u8 buf[2];
	u8 i=0;
	buf[0]=0;
	buf[1]=mode;    //�Ƿ�д�뵽GT911 FLASH?  ���Ƿ���籣��
	for(i=0;i<sizeof(GT911_Cfg);i++){
		buf[0]+=GT911_Cfg[i];//����У���
	}
	buf[0]=(~buf[0])+1;
	TP_WR_Reg(GT_CFGS_REG,(u8*)GT911_Cfg,sizeof(GT911_Cfg));//���ͼĴ�������
	TP_WR_Reg(GT_CHECK_REG,buf,2);//д��У���,�����ø��±��
	return 0;
}

//��ȡ���ò���
//mode:0,���������浽flash
//     1,�������浽flash
u8 GT911_Read_Cfg(void)
{
	u16 i;
	u8 buf[200];
	for(i=0;i<186;i++){
		TP_RD_Reg(GT_CFGS_REG+i,&buf[i],1);
	}
	return 0;
}

//GT911��ʼ��
void GT911_Init(void)
{   
	u8 tmp[4]={0};
	TP_RST_Init();
	TP_INT_Out_Init();
	IIC_Init();
	
	TP_RST_L;
	TP_INT_H;
	TP_delay_us(200);
	TP_RST_H;
	TP_delay_ms(6);
	TP_INT_L;
	TP_delay_ms(55);
//	TP_INT_In_Init();
	TP_INT_EXTI_Init();
	TP_delay_ms(50);
	
	//��ȡPID
	TP_RD_Reg(GT_PID_REG, tmp, 4);
	
	//�޸�����
	tmp[0] = 0x02;
	TP_WR_Reg(GT_CTRL_REG, tmp, 1);
	TP_RD_Reg(GT_CFGS_REG, tmp, 1);
	if(tmp[0] < GT_CFG_VERSION){
		GT911_Send_Cfg(1);
	}
//	GT911_Read_Cfg();
	TP_delay_ms(10);
	tmp[0] = 0x00;
	TP_WR_Reg(GT_CTRL_REG, tmp, 1);
}
//GT911�ⲿ�жϷ������ 
//void EXTI1_IRQHandler(void)
//{
//	u8 tmp;
//	u8 data[0x40];
//	if(EXTI_GetITStatus(GT911_EXTI_LINE) != RESET){
//		GT911_RD_Reg(GT_GSTID_REG,data,0x10);//���㴥��ģʽֻ��ȡ��һ������
//		tmp = data[0];
//		if((tmp&0x80)&&((tmp&0x0f)>0)){			
//			TPN = tmp&0x0f;
//			TPX1 = 479 - ((data[5]<<8) + data[4]);
//			TPY1 = (data[3]<<8) + data[2];
//			TouchReady = 1;
//		}
//		tmp = 0;
//		GT911_WR_Reg(GT_GSTID_REG,&tmp,1);
//		EXTI_ClearITPendingBit(GT911_EXTI_LINE); //���LINE0�ϵ��жϱ�־λ
//	}
//}

//************************FT6336U*************************//
//FT6336��ʼ��
void FT6336_Init(void)
{   
//	u8 tmp[0x42]={0};
	TP_RST_Init();
//	TP_INT_Out_Init();
	IIC_Init();
	
	TP_RST_L;
	TP_delay_ms(20);
	TP_RST_H;
	TP_delay_ms(20);
//	TP_INT_In_Init();
	TP_INT_EXTI_Init();
	TP_delay_ms(100);
	
	//��ȡPID��
	
//	TP_RD_Reg(GT_PID_REG, tmp, 4);
	
	//�޸�����
//	tmp[0] = 0x02;
//	TP_WR_Reg(GT_CTRL_REG, tmp, 1);
//	TP_RD_Reg(GT_CFGS_REG, tmp, 1);
//	if(tmp[0] < GT_CFG_VERSION){
//		TP_Send_Cfg(1);
//	}
//	TP_Read_Cfg();
//	TP_delay_ms(10);
//	tmp[0] = 0x00;
//	TP_WR_Reg(GT_CTRL_REG, tmp, 1);
}

//FT6336�ⲿ�жϷ������
void EXTI1_IRQHandler(void)
{
//	u8 tmp;
	u8 data[8];
	if(EXTI_GetITStatus(TP_EXTI_LINE) != RESET){
		TP_RD_Reg(TP_CMD_X,data,2);//���㴥��ģʽֻ��ȡ��һ������
		TP_RD_Reg(TP_CMD_Y,&data[2],2);//���㴥��ģʽֻ��ȡ��һ������
//		tmp = data[0]&0x0f;
//		if(tmp == 0x80){
			TPN = 1;
			TPY1 = (((data[0]&0x0f)<<8) + data[1]);//*4/3
			TPX1 = 479-(((data[2]&0x0f)<<8) + data[3]);
			TouchReady = 1;
//		}
//		tmp = 0;
		EXTI_ClearITPendingBit(TP_EXTI_LINE); //���LINE0�ϵ��жϱ�־λ
	}
}

//*************************************************************
//������ɨ��,Ӧ�ó�����û�ȡ��������
u8 TP_Scan(struct TP_DATA *tData)
{
	u8 valid = 0;
	if(TouchReady == 1){
		TouchReady = 0;
		valid = 1;
		tData->TpNum = TPN;
		tData->TpX[0] = TPX1;
		tData->TpY[0] = TPY1;
		LCD_DrawPoint(TPX1,TPY1,RED);
		LCD_SetColor(WHITE,BLACK);
		LCD_ShowNnNums(ASC24x12,200,8,0,3,TPX1);
		LCD_ShowNnNums(ASC24x12,246,8,0,3,TPY1);
		return valid;
	}
	return valid;
}
