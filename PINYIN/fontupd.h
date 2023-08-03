#ifndef __FONTUPD_H__
#define __FONTUPD_H__
#include "mcu_io.h"


//字体信息保存地址,占33个字节,第1个字节用于标记字库是否存在.后续每8个字节一组,分别保存起始地址和文件大小
extern u32 FONTINFOADDR;
//字库信息结构体定义
//用来保存字库基本信息，地址，大小等
__packed typedef struct
{
	u8 fontok;				//字库存在标志，0XAA，字库正常；其他，字库不存在	 
	u8 blank[3];
	u32 f16addr;			//gbk16地址
	u32 gbk16size;			//gbk16的大小
	u32 f20addr;			//gbk20地址
	u32 gbk20size;			//gbk20的大小
	u32 f24addr;			//gbk24地址
	u32 gbk24size;			//gbk24的大小
}_font_info;

//unicode2oem表存储
__packed typedef struct
{
	u8 tabok;				//字库存在标志，0XAA，字库正常；其他，字库不存在	 
	u8 blank[3];
	u32 uni2oemAddr;		//uni2oem地址
	u32 uni2oemSize;		//uni2oem的大小
	u32 oem2uniAddr;		//oem2uni地址
	u32 oem2uniSize;		//oem2uni的大小
	
}_uni_oem_info;

extern _font_info ftinfo;	//字库信息结构体
extern _uni_oem_info unioemInfo;	//uni和oem准换表结构体

u32 fupd_prog(u8 font,u16 x,u16 y,u32 fsize,u32 pos);	//显示更新进度
u8 updata_fontx(u8 font,u16 x,u16 y,u8 *fxpath,u8 fx);	//更新指定字库
u8 update_font(u16 x,u16 y,u8 font);					//更新全部字库
u8 update_unioem(u16 x,u16 y,u8 font);
u8 font_init(void);										//初始化字库
u8 unioem_init(void);
#endif





















