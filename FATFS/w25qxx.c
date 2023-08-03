/*******************************************************************************
�����ߣ���Ȫ
�������ڣ�2018.03.01
����������
    1. ����FLASH�����������ļ�ϵͳ����ȡ���ݣ�����ֱ�Ӳ���FLASH����
    2. 
    
���¼�¼��
    1. 2018.03.01 ����
    
******************************************************************************/

#include "w25qxx.h" 
#include "Delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//////////////////////////////////////////////////////////////////////////////////

u16 W25QXX_TYPE = W25Q128;	//Ĭ����W25Q128
u32 W25QXX_SIZE;

#define	W25QXX_CS 		PAout(4)  		//W25QXX��Ƭѡ�ź�

//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25Q128
//����Ϊ16M�ֽ�,����128��Block,4096��Sector 
													 
//��ʼ��SPI FLASH��IO��
void W25QXX_Init(void)
{	
    SPI_InitTypeDef  SPI_InitStructure; 
    GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTBʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           // PA4 ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_4); 
    
    W25QXX_CS=1;				//SPI FLASH��ѡ��
            
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
       
    /*!< Configure SPI_FLASH_SPI pins: SCK */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//SCK
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  //MOSI
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // SPI1 configuration
    // W25X16: data input on the DIO pin is sampled on the rising edge of the CLK. 
    // Data on the DO and DIO pins are clocked out on the falling edge of CLK.
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//SPI_BaudRatePrescaler_8
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);   
    
	W25QXX_TYPE = W25QXX_ReadID();//��ȡFLASH ID.  
	if(W25QXX_TYPE == W25Q256){
		W25QXX_SIZE = 1024*1024*32;
	}else if(W25QXX_TYPE == W25Q128){
		W25QXX_SIZE = 1024*1024*16;
	}else if(W25QXX_TYPE == W25Q64){
		W25QXX_SIZE = 1024*1024*8;
	}else if(W25QXX_TYPE == W25Q32){
		W25QXX_SIZE = 1024*1024*4;
	}
	W25Qxx_Enable4ByteAddr();//����4�ֽڵ�ַ
//	Lcd_DispNum(ASC6x8,10,10,W25QXX_TYPE,4);
}

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�

u8 SPI_ReadWriteByte(u8 TxData) //SPI2 �˿�
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){ //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����		
}

///////////////////////////////////////////////////////////////////////////////////////////

//��ȡW25QXX��״̬�Ĵ�����W25QXXһ����3��״̬�Ĵ���
//״̬�Ĵ���1��
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
//״̬�Ĵ���2��
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//״̬�Ĵ���3��
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:״̬�Ĵ����ţ���:1~3
//����ֵ:״̬�Ĵ���ֵ
u8 W25QXX_ReadSR(u8 regno)   
{  
	u8 byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=W25X_ReadStatusReg1;    //��״̬�Ĵ���1ָ��
            break;
        case 2:
            command=W25X_ReadStatusReg2;    //��״̬�Ĵ���2ָ��
            break;
        case 3:
            command=W25X_ReadStatusReg3;    //��״̬�Ĵ���3ָ��
            break;
        default:
            command=W25X_ReadStatusReg1;    
            break;
    }    
	W25QXX_CS=0;                            //ʹ������   
	SPI_ReadWriteByte(command);            //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI_ReadWriteByte(0Xff);          //��ȡһ���ֽ�  
	W25QXX_CS=1;                            //ȡ��Ƭѡ     
	return byte;   
} 
//дW25QXX״̬�Ĵ���
void W25QXX_Write_SR(u8 regno,u8 sr)   
{   
    u8 command=0;
    switch(regno)
    {
        case 1:
            command=W25X_WriteStatusReg1;    //д״̬�Ĵ���1ָ��
            break;
        case 2:
            command=W25X_WriteStatusReg2;    //д״̬�Ĵ���2ָ��
            break;
        case 3:
            command=W25X_WriteStatusReg3;    //д״̬�Ĵ���3ָ��
            break;
        default:
            command=W25X_WriteStatusReg1;    
            break;
    }   
	W25QXX_CS=0;                            //ʹ������   
	SPI_ReadWriteByte(command);            //����дȡ״̬�Ĵ�������    
	SPI_ReadWriteByte(sr);                 //д��һ���ֽ�  
	W25QXX_CS=1;                            //ȡ��Ƭѡ     	      
}

//ʹ��4�ֽڵ�ַ
void W25Qxx_Enable4ByteAddr(void)
{
	u8 temp;
	if(W25QXX_TYPE==W25Q256)        //SPI FLASHΪW25Q256
    {
        temp=W25QXX_ReadSR(3);      //��ȡ״̬�Ĵ���3���жϵ�ַģʽ
        if((temp&0X01)==0)			//�������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ
		{
			W25QXX_CS=0; 			//ѡ��
			SPI_ReadWriteByte(W25X_Enable4ByteAddr);//���ͽ���4�ֽڵ�ַģʽָ��   
			W25QXX_CS=1;       		//ȡ��Ƭѡ   
		}
    }
}
//W25QXXдʹ��	
//��WEL��λ   
void W25QXX_Write_Enable(void)   
{
	W25QXX_CS=0;                          	//ʹ������   
    SPI_ReadWriteByte(W25X_WriteEnable); 	//����дʹ��  
	W25QXX_CS=1;                           	//ȡ��Ƭѡ     	      
} 
//W25QXXд��ֹ	
//��WEL����  
void W25QXX_Write_Disable(void)   
{  
	W25QXX_CS=0;                            //ʹ������   
    SPI_ReadWriteByte(W25X_WriteDisable);  //����д��ָֹ��    
	W25QXX_CS=1;                            //ȡ��Ƭѡ     	      
} 		
//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128   16MByte
//0XEF18,��ʾоƬ�ͺ�ΪW25Q256   32MByte
u16 W25QXX_ReadID(void)
{
	u16 Temp = 0;	  
	W25QXX_CS=0;				    
	SPI_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
	SPI_ReadWriteByte(0x00); 	    
	SPI_ReadWriteByte(0x00); 	    
	SPI_ReadWriteByte(0x00); 	 			   
	Temp|=SPI_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI_ReadWriteByte(0xFF);	 
	W25QXX_CS=1;				    
	return Temp;
}

//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	W25QXX_CS=0;                            	//ʹ������   
    SPI_ReadWriteByte(W25X_ReadData);         	//���Ͷ�ȡ����
	if(W25QXX_TYPE==W25Q256)                ////�����W25Q256�Ļ���ַΪ4�ֽ�
    {
        SPI_ReadWriteByte((u8)((ReadAddr)>>24)); 
    }
    SPI_ReadWriteByte((u8)((ReadAddr)>>16));  	//����24bit��ַ    
    SPI_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI_ReadWriteByte(0XFF);   	//ѭ������  
    }
	W25QXX_CS=1;  				    	      
}
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
  W25QXX_Write_Enable();                  	//SET WEL 
	W25QXX_CS=0;                            	//ʹ������   
    SPI_ReadWriteByte(W25X_PageProgram);      	//����дҳ����   
	if(W25QXX_TYPE==W25Q256)                ////�����W25Q256�Ļ���ַΪ4�ֽ�
    {
        SPI_ReadWriteByte((u8)((WriteAddr)>>24)); 
    }
    SPI_ReadWriteByte((u8)((WriteAddr)>>16)); 	//����24bit��ַ    
    SPI_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	W25QXX_CS=1;                            	//ȡ��Ƭѡ 
	W25QXX_Wait_Busy();					   		//�ȴ�д�����
} 

//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   

void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;
	int iStart = 0;
	u8 W25QXX_BUF[4096];
	
//	W25QXX_BUF = W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1)
	{	
		if(iStart == 0){
			//������������������
			W25QXX_Read(W25QXX_BUF,secpos*4096,4096);
			
			//У������
			for(i=0;i<secremain;i++)
			{
				if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����
			}
			
			//��Ҫ����
			if(i<secremain){
				W25QXX_Erase_Sector(secpos);		//�����������
				for(i=0;i<secremain;i++)	   		//����
				{
					W25QXX_BUF[i+secoff]=pBuffer[i];	  
				}
				W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������

			}
			else{
				W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.
			}
			iStart ++;
		}
	  else{
			W25QXX_Erase_Sector(secpos);//�����������
			W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.
		}
		
		if(NumByteToWrite==secremain){
			break;//д�������
		}
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

			pBuffer+=secremain;  				//ָ��ƫ��
			WriteAddr+=secremain;				//д��ַƫ��	   
			NumByteToWrite-=secremain;			//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;//��һ����������д����
			else secremain=NumByteToWrite;		//��һ����������д����
		}	 
	};
}
//��������оƬ		  
//�ȴ�ʱ�䳬��...
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                 	 	//SET WEL 
    W25QXX_Wait_Busy();   
  	W25QXX_CS=0;                            	//ʹ������   
    SPI_ReadWriteByte(W25X_ChipErase);        	//����Ƭ��������  
	W25QXX_CS=1;                            	//ȡ��Ƭѡ     	      
	W25QXX_Wait_Busy();   				   		//�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
void W25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	//����falsh�������,������   
// 	printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
    W25QXX_Write_Enable();                  	//SET WEL 	 
    W25QXX_Wait_Busy();   
  	W25QXX_CS=0;                            	//ʹ������   
    SPI_ReadWriteByte(W25X_SectorErase);      	//������������ָ��
	if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽ�
    {
        SPI_ReadWriteByte((u8)((Dst_Addr)>>24)); 
    }
    SPI_ReadWriteByte((u8)((Dst_Addr)>>16));  	//����24bit��ַ    
    SPI_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI_ReadWriteByte((u8)Dst_Addr);  
	W25QXX_CS=1;                            	//ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				   		//�ȴ��������
}  
//�ȴ�����
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   // �ȴ�BUSYλ���
}  
//�������ģʽ
void W25QXX_PowerDown(void)   
{ 
  	W25QXX_CS=0;                           	 	//ʹ������   
    SPI_ReadWriteByte(W25X_PowerDown);        //���͵�������  
	W25QXX_CS=1;                            	//ȡ��Ƭѡ     	      
    Sleep_us(3);                               //�ȴ�TPD  
}   
//����
void W25QXX_WAKEUP(void)   
{  
  	W25QXX_CS=0;                            	//ʹ������   
    SPI_ReadWriteByte(W25X_ReleasePowerDown);	//  send W25X_PowerDown command 0xAB    
	W25QXX_CS=1;                            	//ȡ��Ƭѡ     	      
    Sleep_us(3);                            	//�ȴ�TRES1
}   

