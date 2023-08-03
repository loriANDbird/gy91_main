#include "m25p80.h"


#define SPI2_FLASH_PageSize      256
#define SPI2_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
#define M25X_WriteEnable		    0x06 
#define M25X_WriteDisable		    0x04 
#define M25X_ReadStatusReg		    0x05 
#define M25X_ReadData			    0x03 
#define M25X_PageProgram		    0x02 
#define M25X_SectorErase		    0x20 
#define M25X_ChipErase			    0xC7 
#define M25X_PowerDown			    0xB9 
#define M25X_ReleasePowerDown		0xAB 
#define M25X_DeviceID			    0xAB 
#define M25X_ManufactDeviceID   	0x90 
#define M25X_JedecDeviceID		    0x9F 

#define WIP_Flag					0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                	0xFF


void M25P80_SPI2_Config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/*!< Configure SPI_FLASH_SPI pins: SCK */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*!< Configure SPI_FLASH_SPI pins: MISO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*!< Configure SPI_FLASH_SPI pins: MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH CS pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

///* Deselect the FLASH: Chip Select high */
//  SPI_FLASH_CS_HIGH();
	
	/* SPI1 configuration */
	// W25X16: data input on the DIO pin is sampled on the rising edge of the CLK. 
	// Data on the DO and DIO pins are clocked out on the falling edge of CLK.
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//SPI_BaudRatePrescaler_4; SPI?????
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	/* Enable SPI2  */
	SPI_Cmd(SPI2, ENABLE);
//  SPI_ReadWriteByte(0xff); 
}


void SPI2_FLASH_WriteEnable(void)
{
	/* Select the FLASH: Chip Select low */
	SPI2_FLASH_CS_LOW();
	
	/* Send "Write Enable" instruction */
	SPI2_FLASH_SendByte(M25X_WriteEnable);
	
	/* Deselect the FLASH: Chip Select high */
	SPI2_FLASH_CS_HIGH();
}
void SPI2_FLASH_WaitForWriteEnd(void)
{
	u8 FLASH_Status = 0;
	
	/* Select the FLASH: Chip Select low */
	SPI2_FLASH_CS_LOW();
	
	/* Send "Read Status Register" instruction */
	SPI2_FLASH_SendByte(M25X_ReadStatusReg);
	
	/* Loop as long as the memory is busy with a write cycle */
	do{
	/* Send a dummy byte to generate the clock needed by the FLASH
	and put the value of the status register in FLASH_Status variable */
	FLASH_Status = SPI2_FLASH_SendByte(Dummy_Byte);	 
	}
	while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */
	
	/* Deselect the FLASH: Chip Select high */
	SPI2_FLASH_CS_HIGH();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_FLASH_SectorErase(u32	SectorAddr)
{
//	u32	SectorAddr=Sector*0x1000;
	/* Send write enable instruction */
	SPI2_FLASH_WriteEnable();
	SPI2_FLASH_WaitForWriteEnd();
	/* Sector Erase */
	/* Select the FLASH: Chip Select low */
	SPI2_FLASH_CS_LOW();
	/* Send Sector Erase instruction */
	SPI2_FLASH_SendByte(M25X_SectorErase);
	/* Send SectorAddr high nibble address byte */
	SPI2_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
	/* Send SectorAddr medium nibble address byte */
	SPI2_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
	/* Send SectorAddr low nibble address byte */
	SPI2_FLASH_SendByte(SectorAddr & 0xFF);
	/* Deselect the FLASH: Chip Select high */
	SPI2_FLASH_CS_HIGH();
	/* Wait the end of Flash writing */
	SPI2_FLASH_WaitForWriteEnd();
}

void SPI2_FLASH_BlockErase(u8 Sector)
{
	u32	SectorAddr=Sector*0x10000;
	/* Send write enable instruction */
	SPI2_FLASH_WriteEnable();
	SPI2_FLASH_WaitForWriteEnd();
	/* Sector Erase */
	/* Select the FLASH: Chip Select low */
	SPI2_FLASH_CS_LOW();
	/* Send Sector Erase instruction */
	SPI2_FLASH_SendByte(0xd8);
	/* Send SectorAddr high nibble address byte */
	SPI2_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
	/* Send SectorAddr medium nibble address byte */
	SPI2_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
	/* Send SectorAddr low nibble address byte */
	SPI2_FLASH_SendByte(SectorAddr & 0xFF);
	/* Deselect the FLASH: Chip Select high */
	SPI2_FLASH_CS_HIGH();
	/* Wait the end of Flash writing */
	SPI2_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI2_FLASH_SendByte(u8 byte)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI2, byte);
	
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI2);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	/* Select the FLASH: Chip Select low */
	SPI2_FLASH_CS_LOW();
	
	/* Send "Read from Memory " instruction */
	SPI2_FLASH_SendByte(M25X_ReadData);
	
	/* Send ReadAddr high nibble address byte to read from */
	SPI2_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte to read from */
	SPI2_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte to read from */
	SPI2_FLASH_SendByte(ReadAddr & 0xFF);
	
	while (NumByteToRead--) /* while there is data to be read */
	{
	/* Read a byte from the FLASH */
	*pBuffer = SPI2_FLASH_SendByte(Dummy_Byte);
	/* Point to the next location where the byte read will be saved */
	pBuffer++;
	}
	
	/* Deselect the FLASH: Chip Select high */
	SPI2_FLASH_CS_HIGH();
}

void SPI2_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	/* Enable the write access to the FLASH */
	SPI2_FLASH_WriteEnable();
	
	/* Select the FLASH: Chip Select low */
	SPI2_FLASH_CS_LOW();
	/* Send "Write to Memory " instruction */
	SPI2_FLASH_SendByte(M25X_PageProgram);
	/* Send WriteAddr high nibble address byte to write to */
	SPI2_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
	/* Send WriteAddr medium nibble address byte to write to */
	SPI2_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
	/* Send WriteAddr low nibble address byte to write to */
	SPI2_FLASH_SendByte(WriteAddr & 0xFF);
	
	if(NumByteToWrite > SPI2_FLASH_PerWritePageSize)
	{
	NumByteToWrite = SPI2_FLASH_PerWritePageSize;
	//printf("\n\r Err: SPI_FLASH_PageWrite too large!");
	}
	
	/* while there is data to be written on the FLASH */
	while (NumByteToWrite--)
	{
	/* Send the current byte */
	SPI2_FLASH_SendByte(*pBuffer);
	/* Point on the next byte to be written */
	pBuffer++;
	}
	
	/* Deselect the FLASH: Chip Select high */
	SPI2_FLASH_CS_HIGH();
	
	/* Wait the end of Flash writing */
	SPI2_FLASH_WaitForWriteEnd();
}


void SPI2_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

	Addr = WriteAddr % SPI2_FLASH_PageSize;
	count = SPI2_FLASH_PageSize - Addr;
	NumOfPage =  NumByteToWrite / SPI2_FLASH_PageSize;
	NumOfSingle = NumByteToWrite % SPI2_FLASH_PageSize;

	if (Addr == 0){ /* WriteAddr is SPI_FLASH_PageSize aligned  */
	
		if (NumOfPage == 0){ /* NumByteToWrite < SPI_FLASH_PageSize */
		
			SPI2_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
		}
		else{ /* NumByteToWrite > SPI_FLASH_PageSize */
    
			while (NumOfPage--) {
				SPI2_FLASH_PageWrite(pBuffer, WriteAddr, SPI2_FLASH_PageSize);
				WriteAddr +=  SPI2_FLASH_PageSize;
				pBuffer += SPI2_FLASH_PageSize;
			}

			SPI2_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	else{ /* WriteAddr is not SPI_FLASH_PageSize aligned  */
	
		if (NumOfPage == 0){ /* NumByteToWrite < SPI_FLASH_PageSize */
		
			if (NumOfSingle > count){ /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
			
				temp = NumOfSingle - count;

				SPI2_FLASH_PageWrite(pBuffer, WriteAddr, count);
				WriteAddr +=  count;
				pBuffer += count;

				SPI2_FLASH_PageWrite(pBuffer, WriteAddr, temp);
			}
			else{
				SPI2_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else {/* NumByteToWrite > SPI_FLASH_PageSize */
		
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / SPI2_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI2_FLASH_PageSize;

			SPI2_FLASH_PageWrite(pBuffer, WriteAddr, count);
			WriteAddr +=  count;
			pBuffer += count;

			while (NumOfPage--){
				SPI2_FLASH_PageWrite(pBuffer, WriteAddr, SPI2_FLASH_PageSize);
				WriteAddr +=  SPI2_FLASH_PageSize;
				pBuffer += SPI2_FLASH_PageSize;
			}

			if (NumOfSingle != 0){
				SPI2_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
}


//void Font_Info(SHOW_INFO * Data,u8 Font)
//{
//	switch (Font)
//	{
//		case FONT_ASC16:	//Ä¬ÈÏ×Ö·û´®
//			Data->Wide = STR_WIDTH;
//			Data->High = STR_HEIGHT;
//			Data->Max  = STR_COUNT; 
//			break;
//			
//		case FONT_ASC12: 	//
//			Data->Wide = 6;
//			Data->High = 12;
//			Data->Max  = 12;  
//			break;
//	
//		case FONT_ASC24:  	//
//			Data->Wide = 14;
//			Data->High = 24;
//			Data->Max  = 48;   
//			break; 
//			
//		case FONT_ASC40:
//			Data->Wide = 24;
//			Data->High = 40;
//			Data->Max  = 120;  
//			break; 


//		case FONT_GB12:		//°æ±¾ºÅ Ð¡ºº×Ö
//			Data->Wide = 12;
//			Data->High = 12;
//			Data->Max  = 24; 
//			break;

//		case EXT_GB16: 		//Ä¬ÈÏºº×Ö
//			Data->Wide = 16;
//			Data->High = 16;
//			Data->Max  = 32;  
//			break;		
//		 
//		case FONT_GB18: 	// ºº×Ö
//			Data->Wide = 19;
//			Data->High = 18;
//			Data->Max  = 54;  
//			break;

//		case FONT_GB20:		//Logoºº×Ö
//			Data->Wide = 20;
//			Data->High = 20;
//			Data->Max  = 60;   
//			break;
//            
//		case FONT_GB24:		//Logoºº×Ö
//			Data->Wide = 24;
//			Data->High = 24;
//			Data->Max  = 72;   
//			break;
//            

//		case FONT_PIC16: 	//Ð¡Í¼±ê
//			Data->Wide = 16;
//			Data->High = 16;
//			Data->Max  = 32;   
//			break;	

//		case FONT_GB26:   	//
//			Data->Wide = 26;
//			Data->High = 26;
//			Data->Max  = 104;  
//			break;
//		
//		case FONT_ASC32:  	//
//			Data->Wide = ASC32_WIDTH;
//			Data->High = ASC32_HEIGHT;
//			Data->Max  = ASC32_COUNT;   
//			break;	 


////¿í32 * ¸ß42 
////FNT_ASC32x42 ASC32x42

////FNT_ASC32x42 const ASC32x42[MAX_ASC32x42] =    

//		case FONT_ASC32x42:  	//
//			Data->Wide = 32;
//			Data->High = 42;
//			Data->Max  = 168;   
//			break;	 
//            
////    else  if(Font == FONT_ASC32x42){
////		Data.Buff = (u8*)ASC32x42[ascii-'0'].Msk;
//		case PIC: 
//			Data->Wide = 24;
//			Data->High = 16;
//			Data->Max  = 48;
//			break;

//		default:
//			break;
//	}     
//}

///*
//	¶ÁÈ¡×Ö¿âÐ¾Æ¬Êý¾Ý²¢ÏÔÊ¾
//*/
//void SPI2_ReadArray(u16 x,u16 y,u8* pdata)
//{ 
//	unsigned char l_ucbuff[32];   
//	u32 uiAddr;
//	SHOW_INFO Data; 

//	Font_Info(&Data,EXT_GB16);  
//	Data.X = x;  
//	Data.Y = y;  
//	if((u8)pdata[0]>=0xB0 && (u8)pdata[0]<=0xF7 && (u8)pdata[1]>0xa0)
//	{
//		uiAddr = (((u8)pdata[0]-0xB0)*94 + ((u8)pdata[1]-0xa1))*Data.Max; 
//		SPI2_FLASH_BufferRead(l_ucbuff,uiAddr,32);
//		Data.Buff = &l_ucbuff[0];
//		LCD_ShowDrive(&Data);
//	}
//}
 






