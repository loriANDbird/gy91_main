#ifndef __M25P80_H_
#define __M25P80_H_

#include "stm32f10x.h"


#define SPI2_FLASH_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SPI2_FLASH_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)

//extern u8 guFontAddr[128]; 
void M25P80_SPI2_Config(void);
//void Font_Info(SHOW_INFO * Data,u8 Font);


void SPI2_FLASH_Init(void);
//void SPI2_FLASH_SectorErase(u8 Sector);
void SPI2_FLASH_SectorErase(u32	SectorAddr);
void SPI2_FLASH_BlockErase(u8 Sector);
void SPI2_FLASH_BulkErase(void);
void SPI2_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI2_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI2_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI2_FLASH_ReadID(void);
u32 SPI2_FLASH_ReadDeviceID(void);
void SPI2_FLASH_StartReadSequence(u32 ReadAddr);
void SPI2_Flash_PowerDown(void);
void SPI2_Flash_WAKEUP(void);


//u8 SPI2_FLASH_ReadByte(void);
u8 SPI2_FLASH_SendByte(u8 byte);
u16 SPI2_FLASH_SendHalfWord(u16 HalfWord);
void SPI2_FLASH_WriteEnable(void);
void SPI2_FLASH_WriteDisable(void);
void SPI2_FLASH_WaitForWriteEnd(void);

void SPI2_ReadArray(u16 x,u16 y,u8* pdata);
void SPI2_ReadAddr(u16 x,u16 y,u32 addr);
#endif /* __SPI2_FLASH_H */
 



