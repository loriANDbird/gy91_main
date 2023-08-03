/**
  ******************************************************************************
  * @file    mass_mal.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Medium Access Layer interface
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/


#include "mass_mal.h"
//#include "bsp_spi_flash.h"
#include "w25qxx.h" 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Mass_Memory_Size[2];
uint32_t Mass_Block_Size[2];
uint32_t Mass_Block_Count[2];
__IO uint32_t Status = 0;

//SD_CardInfo mSDCardInfo;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Init(uint8_t lun)
{
  u16 status = MAL_OK;
  switch (lun)
  {
    case 0:		
		Mass_Memory_Size[0]= 1024*1024*12;//4M×Ö½Ú
		Mass_Block_Size[0] = 4096;
		Mass_Block_Count[0]= Mass_Memory_Size[0]/Mass_Block_Size[0];
		W25QXX_Init();//³õÊ¼»¯W25Q32
		  return status;
//		if(SPI_FLASH_TYPE != FLASH_ADDRESS)
//			status = RES_NOTRDY;
//		else
//			status = RES_OK;
//		break;
		default:
      return MAL_FAIL;
  }
}
/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Write(uint8_t lun, u32 Memory_Offset, u8 *Writebuff, u16 Transfer_Length)
{
	u8 STA;
	switch (lun)
	{
		case 0:
			STA=0;
#ifdef BSP_SPI_EN	
			SPI_FLASH_SectorErase(Memory_Offset);
      SPI_FLASH_BufferWrite( Writebuff,Memory_Offset, Transfer_Length);
#endif	

#ifdef W25X_SPI_EN
			W25QXX_Write( Writebuff,Memory_Offset, Transfer_Length);
#endif
			break; 
		default:
			return MAL_FAIL;
	}
	if(STA!=0)return MAL_FAIL;
	return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/
uint16_t MAL_Read(uint8_t lun, u32 Memory_Offset, u8 *Readbuff, u16 Transfer_Length)
{

	u8 STA;
	switch (lun)
	{
		case 0:
			STA=0;
		W25QXX_Read((u8*)Readbuff, Memory_Offset, Transfer_Length);
			//SPI_FLASH_BufferRead((u8*)Readbuff, Memory_Offset, Transfer_Length);   		  
			break;	  
		default:
			return MAL_FAIL;
	}
	if(STA!=0)return MAL_FAIL;
	return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_GetStatus (uint8_t lun)
{
    switch(lun)
    {
    case 0:
        return MAL_OK;
    case 1:
        return MAL_FAIL;
    case 2:
        return MAL_FAIL;
    default:
        return MAL_FAIL;
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

