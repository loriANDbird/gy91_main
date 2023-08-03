/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "diskio.h"			/* FatFs lower layer API */
//#include "sdio_sdcard.h"
#include "w25qxx.h"
#include "malloc.h"	
#include "rX8010.h" 

//#include "bsp_spi_flash.h"


#define EX_FLASH 0	//外部flash,卷标为1
#define SD_CARD	 1  //SD卡,卷标为0

#define FLASH_SECTOR_SIZE 	4096
//前12M字节给fatfs用,12M字节后,用于存放字库,字库占用3.09M.	剩余部分,给客户自己用	 			    
u16	    FLASH_SECTOR_COUNT = 1024*2;	//1024*3;
#define FLASH_BLOCK_SIZE   	 16     	//每个BLOCK有16个扇区

//获得磁盘状态
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{ 
	return RES_OK;
}
//初始化磁盘
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	u8 res=0;	    
	switch(pdrv)
	{
		case SD_CARD://SD卡
//			res=SD_Init();//SD卡初始化 
//  			break;
		case EX_FLASH://外部flash
			W25QXX_Init();
			if(W25QXX_TYPE == 0XEF15){//w25q32
				FLASH_SECTOR_COUNT=1024*1;//W25Q1218,前4M字节给FATFS占用 1024*1
			}
			if(W25QXX_TYPE == 0XEF16){//w25q64
				FLASH_SECTOR_COUNT=1024*2;//W25Q1218,前8M字节给FATFS占用 1024*2
			}
			if(W25QXX_TYPE == 0XEF17){//w25q128
				FLASH_SECTOR_COUNT=1024*3;//W25Q1218,前12M字节给FATFS占用 1024*3
			}
			if(W25QXX_TYPE == 0XEF18){//w25q128
				FLASH_SECTOR_COUNT=1024*7;//W25Q1218,前12M字节给FATFS占用 1024*3
			}
			
 			break;
		default:
			res=1; 
	}		 
	if(res)return  STA_NOINIT;
	else return 0; //初始化成功 
}
//读扇区
//pdrv:磁盘编号0~9
//*buff:数据接收缓冲首地址
//sector:扇区地址
//count:需要读取的扇区数
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	u8 res=0; 
    if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误		 	 
	switch(pdrv)
	{
		case SD_CARD://SD卡
//			res=SD_ReadDisk(buff,sector,count);	 
//			while(res)//读出错
//			{
//				SD_Init();	//重新初始化SD卡
//				res=SD_ReadDisk(buff,sector,count);	
//				//printf("sd rd error:%d\r\n",res);
//			}
			break;
		case EX_FLASH://外部flash
			for(;count>0;count--)
			{
				W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
   //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res==0x00)return RES_OK;	 
    else return RES_ERROR;
}
//写扇区
//pdrv:磁盘编号0~9
//*buff:发送数据首地址
//sector:扇区地址
//count:需要写入的扇区数
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,	   		/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,	  	/* Sector address in LBA */
	UINT count		  	/* Number of sectors to write */
)
{
	u8 res=0;  
    if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误		 	 
	switch(pdrv)
	{
		case SD_CARD://SD卡
//			res=SD_WriteDisk((u8*)buff,sector,count);
//			while(res)//写出错
//			{
//				SD_Init();	//重新初始化SD卡
//				res=SD_WriteDisk((u8*)buff,sector,count);	
//				//printf("sd wr error:%d\r\n",res);
//			}
			break;
		case EX_FLASH://外部flash
			for(;count>0;count--)
			{
#ifdef BSP_SPI_EN	
				SPI_FLASH_SectorErase(sector*4096);
				SPI_FLASH_BufferWrite((u8*)buff,sector*4096,4096);
#endif	
				
#ifdef W25X_SPI_EN	
				W25QXX_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
#endif
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
    //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res == 0x00)return RES_OK;	 
    else return RES_ERROR;	
}
#endif
//其他表参数的获得
//pdrv:磁盘编号0~9
//ctrl:控制代码
//*buff:发送/接收缓冲区指针
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
DRESULT res;						  			     
if(pdrv==EX_FLASH)	//外部FLASH  
	{
	    switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = FLASH_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = FLASH_BLOCK_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = FLASH_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}else res=RES_ERROR;//其他的不支持
    return res;
}
#endif
//获得时间
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{
	unsigned long int t=0,tmp=0;
	
	RX8010_ReadTime(&g_SysTime);
	tmp = (unsigned long)g_SysTime.year - 1980;
	t = tmp << 25;

	tmp = (unsigned long)g_SysTime.month;
	t |= tmp << 21;

	tmp = (unsigned long)g_SysTime.day;
	t |= tmp << 16;

	tmp = (unsigned long)g_SysTime.hour;
	t |= tmp << 11;

	tmp = (unsigned long)g_SysTime.min;
	t |= tmp << 5;

	tmp = (unsigned long)g_SysTime.sec;
	t |= tmp>>1;	
	return t;
}
//动态分配内存
void *ff_memalloc (UINT size)			
{
	return (void*)mymalloc(SRAMIN,size);
}
//释放内存
void ff_memfree (void* mf)		 
{
	myfree(SRAMIN,mf);
}
