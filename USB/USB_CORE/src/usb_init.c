/**
  ******************************************************************************
  * @file    usb_init.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Initialization routines & global variables
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
#include "usb_lib.h"

#include "w25qxx.h" 
#include "malloc.h"	 
#include "mass_mal.h"
#include "memory.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*  The number of current endpoint, it will be used to specify an endpoint */
 uint8_t	EPindex;
/*  The number of current device, it is an index to the Device_Table */
/* uint8_t	Device_no; */
/*  Points to the DEVICE_INFO structure of current device */
/*  The purpose of this register is to speed up the execution */
DEVICE_INFO *pInformation;
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */
DEVICE_PROP *pProperty;
/*  Temporary save the state of Rx & Tx status. */
/*  Whenever the Rx or Tx state is changed, its value is saved */
/*  in this variable first and will be set to the EPRB or EPRA */
/*  at the end of interrupt process */
uint16_t	SaveState ;
uint16_t  wInterrupt_Mask;
DEVICE_INFO	Device_Info;
USER_STANDARD_REQUESTS  *pUser_Standard_Requests;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void USB_Memery_Init_AK(void)
{
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128   16MByte
//0XEF18,表示芯片型号为W25Q256   32MByte
   
	 Mass_Memory_Size[0] = 1024*1024*8;	//前12M字节
	if(W25QXX_TYPE == 0XEF15){//w25q32
		Mass_Memory_Size[0] = 1024*1024*4;	//前4M字节  4M用于文件系统
	}
	if(W25QXX_TYPE == 0XEF16){//w25q64
		Mass_Memory_Size[0] = 1024*1024*8;	//前8M字节  4M用于文件系统
	}
	if(W25QXX_TYPE == 0XEF17){//w25q128
		Mass_Memory_Size[0] = 1024*1024*12;	//前12M字节  4M用于文件系统
	}
	if(W25QXX_TYPE == 0XEF18){//w25q256
		Mass_Memory_Size[0] = 1024*1024*28;	//前12M字节  4M用于文件系统
	}

	Mass_Block_Size[0] = 4096;			//设置SPI FLASH的操作扇区大小为512
	Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
	 
//   Data_Buffer = mymalloc(SRAMIN,BULK_MAX_PACKET_SIZE*2*4);  //为USB数据缓存区申请内存
//	 Bulk_Data_Buff = mymalloc(SRAMIN,BULK_MAX_PACKET_SIZE);	  //申请内存
}

/*******************************************************************************
* Function Name  : USB_Init
* Description    : USB system initialization
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Init(void)
{
  pInformation = &Device_Info;
  pInformation->ControlState = 2;
  pProperty = &Device_Property;
  pUser_Standard_Requests = &User_Standard_Requests;
  /* Initialize devices one by one */
  pProperty->Init();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
