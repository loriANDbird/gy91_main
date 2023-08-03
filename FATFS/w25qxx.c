/*******************************************************************************
创建者：康泉
创建日期：2018.03.01
功能描述：
    1. 串行FLASH驱动，用于文件系统，存取数据，程序不直接操作FLASH内容
    2. 
    
更新记录：
    1. 2018.03.01 创建
    
******************************************************************************/

#include "w25qxx.h" 
#include "Delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//////////////////////////////////////////////////////////////////////////////////

u16 W25QXX_TYPE = W25Q128;	//默认是W25Q128
u32 W25QXX_SIZE;

#define	W25QXX_CS 		PAout(4)  		//W25QXX的片选信号

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q128
//容量为16M字节,共有128个Block,4096个Sector 
													 
//初始化SPI FLASH的IO口
void W25QXX_Init(void)
{	
    SPI_InitTypeDef  SPI_InitStructure; 
    GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTB时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           // PA4 推挽 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_4); 
    
    W25QXX_CS=1;				//SPI FLASH不选中
            
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
    
	W25QXX_TYPE = W25QXX_ReadID();//读取FLASH ID.  
	if(W25QXX_TYPE == W25Q256){
		W25QXX_SIZE = 1024*1024*32;
	}else if(W25QXX_TYPE == W25Q128){
		W25QXX_SIZE = 1024*1024*16;
	}else if(W25QXX_TYPE == W25Q64){
		W25QXX_SIZE = 1024*1024*8;
	}else if(W25QXX_TYPE == W25Q32){
		W25QXX_SIZE = 1024*1024*4;
	}
	W25Qxx_Enable4ByteAddr();//设置4字节地址
//	Lcd_DispNum(ASC6x8,10,10,W25QXX_TYPE,4);
}

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节

u8 SPI_ReadWriteByte(u8 TxData) //SPI2 端口
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){ //检查指定的SPI标志位设置与否:发送缓存空标志位
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){//检查指定的SPI标志位设置与否:接受缓存非空标志位
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据		
}

///////////////////////////////////////////////////////////////////////////////////////////

//读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器
//状态寄存器1：
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
//状态寄存器2：
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//状态寄存器3：
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:状态寄存器号，范:1~3
//返回值:状态寄存器值
u8 W25QXX_ReadSR(u8 regno)   
{  
	u8 byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=W25X_ReadStatusReg1;    //读状态寄存器1指令
            break;
        case 2:
            command=W25X_ReadStatusReg2;    //读状态寄存器2指令
            break;
        case 3:
            command=W25X_ReadStatusReg3;    //读状态寄存器3指令
            break;
        default:
            command=W25X_ReadStatusReg1;    
            break;
    }    
	W25QXX_CS=0;                            //使能器件   
	SPI_ReadWriteByte(command);            //发送读取状态寄存器命令    
	byte=SPI_ReadWriteByte(0Xff);          //读取一个字节  
	W25QXX_CS=1;                            //取消片选     
	return byte;   
} 
//写W25QXX状态寄存器
void W25QXX_Write_SR(u8 regno,u8 sr)   
{   
    u8 command=0;
    switch(regno)
    {
        case 1:
            command=W25X_WriteStatusReg1;    //写状态寄存器1指令
            break;
        case 2:
            command=W25X_WriteStatusReg2;    //写状态寄存器2指令
            break;
        case 3:
            command=W25X_WriteStatusReg3;    //写状态寄存器3指令
            break;
        default:
            command=W25X_WriteStatusReg1;    
            break;
    }   
	W25QXX_CS=0;                            //使能器件   
	SPI_ReadWriteByte(command);            //发送写取状态寄存器命令    
	SPI_ReadWriteByte(sr);                 //写入一个字节  
	W25QXX_CS=1;                            //取消片选     	      
}

//使能4字节地址
void W25Qxx_Enable4ByteAddr(void)
{
	u8 temp;
	if(W25QXX_TYPE==W25Q256)        //SPI FLASH为W25Q256
    {
        temp=W25QXX_ReadSR(3);      //读取状态寄存器3，判断地址模式
        if((temp&0X01)==0)			//如果不是4字节地址模式,则进入4字节地址模式
		{
			W25QXX_CS=0; 			//选中
			SPI_ReadWriteByte(W25X_Enable4ByteAddr);//发送进入4字节地址模式指令   
			W25QXX_CS=1;       		//取消片选   
		}
    }
}
//W25QXX写使能	
//将WEL置位   
void W25QXX_Write_Enable(void)   
{
	W25QXX_CS=0;                          	//使能器件   
    SPI_ReadWriteByte(W25X_WriteEnable); 	//发送写使能  
	W25QXX_CS=1;                           	//取消片选     	      
} 
//W25QXX写禁止	
//将WEL清零  
void W25QXX_Write_Disable(void)   
{  
	W25QXX_CS=0;                            //使能器件   
    SPI_ReadWriteByte(W25X_WriteDisable);  //发送写禁止指令    
	W25QXX_CS=1;                            //取消片选     	      
} 		
//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128   16MByte
//0XEF18,表示芯片型号为W25Q256   32MByte
u16 W25QXX_ReadID(void)
{
	u16 Temp = 0;	  
	W25QXX_CS=0;				    
	SPI_ReadWriteByte(0x90);//发送读取ID命令	    
	SPI_ReadWriteByte(0x00); 	    
	SPI_ReadWriteByte(0x00); 	    
	SPI_ReadWriteByte(0x00); 	 			   
	Temp|=SPI_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI_ReadWriteByte(0xFF);	 
	W25QXX_CS=1;				    
	return Temp;
}

//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	W25QXX_CS=0;                            	//使能器件   
    SPI_ReadWriteByte(W25X_ReadData);         	//发送读取命令
	if(W25QXX_TYPE==W25Q256)                ////如果是W25Q256的话地址为4字节
    {
        SPI_ReadWriteByte((u8)((ReadAddr)>>24)); 
    }
    SPI_ReadWriteByte((u8)((ReadAddr)>>16));  	//发送24bit地址    
    SPI_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI_ReadWriteByte(0XFF);   	//循环读数  
    }
	W25QXX_CS=1;  				    	      
}
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
  W25QXX_Write_Enable();                  	//SET WEL 
	W25QXX_CS=0;                            	//使能器件   
    SPI_ReadWriteByte(W25X_PageProgram);      	//发送写页命令   
	if(W25QXX_TYPE==W25Q256)                ////如果是W25Q256的话地址为4字节
    {
        SPI_ReadWriteByte((u8)((WriteAddr)>>24)); 
    }
    SPI_ReadWriteByte((u8)((WriteAddr)>>16)); 	//发送24bit地址    
    SPI_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI_ReadWriteByte(pBuffer[i]);//循环写数  
	W25QXX_CS=1;                            	//取消片选 
	W25QXX_Wait_Busy();					   		//等待写入结束
} 

//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 
//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   

void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;
	int iStart = 0;
	u8 W25QXX_BUF[4096];
	
//	W25QXX_BUF = W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1)
	{	
		if(iStart == 0){
			//读出整个扇区的内容
			W25QXX_Read(W25QXX_BUF,secpos*4096,4096);
			
			//校验数据
			for(i=0;i<secremain;i++)
			{
				if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除
			}
			
			//需要擦除
			if(i<secremain){
				W25QXX_Erase_Sector(secpos);		//擦除这个扇区
				for(i=0;i<secremain;i++)	   		//复制
				{
					W25QXX_BUF[i+secoff]=pBuffer[i];	  
				}
				W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区

			}
			else{
				W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间.
			}
			iStart ++;
		}
	  else{
			W25QXX_Erase_Sector(secpos);//擦除这个扇区
			W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间.
		}
		
		if(NumByteToWrite==secremain){
			break;//写入结束了
		}
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

			pBuffer+=secremain;  				//指针偏移
			WriteAddr+=secremain;				//写地址偏移	   
			NumByteToWrite-=secremain;			//字节数递减
			if(NumByteToWrite>4096)secremain=4096;//下一个扇区还是写不完
			else secremain=NumByteToWrite;		//下一个扇区可以写完了
		}	 
	};
}
//擦除整个芯片		  
//等待时间超长...
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                 	 	//SET WEL 
    W25QXX_Wait_Busy();   
  	W25QXX_CS=0;                            	//使能器件   
    SPI_ReadWriteByte(W25X_ChipErase);        	//发送片擦除命令  
	W25QXX_CS=1;                            	//取消片选     	      
	W25QXX_Wait_Busy();   				   		//等待芯片擦除结束
}   
//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms
void W25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	//监视falsh擦除情况,测试用   
// 	printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
    W25QXX_Write_Enable();                  	//SET WEL 	 
    W25QXX_Wait_Busy();   
  	W25QXX_CS=0;                            	//使能器件   
    SPI_ReadWriteByte(W25X_SectorErase);      	//发送扇区擦除指令
	if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节
    {
        SPI_ReadWriteByte((u8)((Dst_Addr)>>24)); 
    }
    SPI_ReadWriteByte((u8)((Dst_Addr)>>16));  	//发送24bit地址    
    SPI_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI_ReadWriteByte((u8)Dst_Addr);  
	W25QXX_CS=1;                            	//取消片选     	      
    W25QXX_Wait_Busy();   				   		//等待擦除完成
}  
//等待空闲
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   // 等待BUSY位清空
}  
//进入掉电模式
void W25QXX_PowerDown(void)   
{ 
  	W25QXX_CS=0;                           	 	//使能器件   
    SPI_ReadWriteByte(W25X_PowerDown);        //发送掉电命令  
	W25QXX_CS=1;                            	//取消片选     	      
    Sleep_us(3);                               //等待TPD  
}   
//唤醒
void W25QXX_WAKEUP(void)   
{  
  	W25QXX_CS=0;                            	//使能器件   
    SPI_ReadWriteByte(W25X_ReleasePowerDown);	//  send W25X_PowerDown command 0xAB    
	W25QXX_CS=1;                            	//取消片选     	      
    Sleep_us(3);                            	//等待TRES1
}   

