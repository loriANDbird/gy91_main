#include "fontupd.h"
#include "ff.h"	  
#include "w25qxx.h"   
#include "lcd.h"  
#include "string.h"
#include "malloc.h"
#include "delay.h"
#include "OpPara.h"

#define GBK16	0
#define GBK20	1
#define GBK24	2

#define UNI2OEM	3
#define OEM2UNI	4

//字库区域占用的总扇区数大小(2个字库:font16+font20+字库信息<1024K字节,取256个W25QXX扇区)
#define FONTSECSIZE	 	512//256//4k/扇区
//字库存放起始地址 
#define FONTINFOADDR 	(1024*1024*2) 					//Explorer STM32F4是从15M地址以后开始存放字库
														//前面15M被fatfs占用了.
														//15M以后紧跟2个字库,总大小1M,被字库占用了,不能动!

#define UNIOEMSIZE	 	256//256//4k/扇区
//unicode2oem存放地址
#define UNIOEMADDR		(1024*1024*3+256)

//用来保存字库基本信息，地址，大小等
_font_info ftinfo;	//字库信息结构体

_uni_oem_info unioemInfo;	//uni和oem准换表结构体

//字库存放在磁盘中的路径，字库实际使用GB2312
char*const GBK24_PATH="0:/FONT/GBK24.DZK";	//GBK24的存放位置，生成的文件删除前15*94个字符编码
char*const GBK20_PATH="0:/FONT/GBK20.DZK";	//GBK20的存放位置
char*const GBK16_PATH="0:/FONT/GBK16.DZK";	//GBK16的存放位置

char*const UNI2OEM_PATH="0:/FONT/U2O.bin";	//uni2oem的存放位置
char*const OEM2UNI_PATH="0:/FONT/O2U.bin";	//oem2uni的存放位置

//显示当前字体更新进度
//x,y:坐标
//font:字体大小
//fsize:整个文件大小
//pos:当前文件指针位置
u32 fupd_prog(u8 font,u16 x,u16 y,u32 fsize,u32 pos)
{
	float prog;
	u8 t=0XFF;
	
	u16 upDioCol;
	u16 upDioTxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		upDioCol = DGRAY;
		upDioTxCol = WHITE;
	}else{
		upDioCol = LGRAY;
		upDioTxCol = BLACK;
	}
	
	prog=(float)pos/fsize;
	prog*=100;
	if(t!=prog)
	{
		LCD_SetColor(upDioCol,upDioTxCol);
		LCD_ShowString(font,x+3*font/2+1,y,0,"%");
		t=prog;
		if(t>100)t=100;
		LCD_SetColor(upDioCol,upDioTxCol);
		LCD_ShowNnNums(font,x,y,0,3,t);//显示数值
	}
	return 0;
}
//更新某一个
//x,y:坐标
//font:字体大小
//fxpath:路径
//fx:更新的内容 0,ungbk;1,gbk12;2,gbk16;3,gbk24;
//返回值:0,成功;其他,失败.
u8 updata_fontx(u8 font,u16 x,u16 y,u8 *fxpath,u8 fx)
{
	u32 flashaddr=0;
	FIL *fftemp;
	u8 *tempbuf;
 	u8 res;
	u16 bread;
	u32 offx=0;
	u8 rval=0;
	fftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));	//分配内存
	if(fftemp==NULL)rval=1;
	tempbuf=mymalloc(SRAMIN,4096);				//分配4096个字节空间
	if(tempbuf==NULL)rval=1;
 	res=f_open(fftemp,(const TCHAR*)fxpath,FA_READ);
 	if(res)rval=2;//打开文件失败
 	if(rval==0)
	{
		switch(fx)
		{
			case GBK16:
				ftinfo.f16addr=FONTINFOADDR+sizeof(ftinfo);	//GBK12之后，紧跟GBK16字库
				ftinfo.gbk16size=fftemp->fsize;					//GBK16字库大小
				flashaddr=ftinfo.f16addr;						//GBK16的起始地址
				break;
			case GBK20:
				ftinfo.f20addr=ftinfo.f16addr+ftinfo.gbk16size;	//GBK16之后，紧跟GBK24字库
				ftinfo.gbk20size=fftemp->fsize;					//GBK24字库大小
				flashaddr=ftinfo.f20addr;						//GBK24的起始地址
				break;
			case GBK24:
				ftinfo.f24addr=ftinfo.f20addr+ftinfo.gbk20size;	//GBK16之后，紧跟GBK24字库
				ftinfo.gbk24size=fftemp->fsize;					//GBK24字库大小
				flashaddr=ftinfo.f24addr;						//GBK24的起始地址
				break;
			case UNI2OEM:
				unioemInfo.uni2oemAddr=UNIOEMADDR+sizeof(unioemInfo);	//GBK16之后，紧跟GBK24字库
				unioemInfo.uni2oemSize=fftemp->fsize;					//GBK24字库大小
				flashaddr=unioemInfo.uni2oemAddr;						//GBK24的起始地址
				break;
			case OEM2UNI:
				unioemInfo.oem2uniAddr=unioemInfo.uni2oemAddr+unioemInfo.uni2oemSize;	//GBK16之后，紧跟GBK24字库
				unioemInfo.oem2uniSize=fftemp->fsize;					//GBK24字库大小
				flashaddr=unioemInfo.oem2uniAddr;						//GBK24的起始地址
				break;
			default:
				break;
		}
		
		while(res==FR_OK)//死循环执行
		{
	 		res=f_read(fftemp,tempbuf,4096,(UINT *)&bread);		//读取数据
			if(res!=FR_OK)break;								//执行错误
			W25QXX_Write(tempbuf,offx+flashaddr,4096);		//从0开始写入4096个数据
	  		offx+=bread;
			fupd_prog(font,x,y,fftemp->fsize,offx);	 			//进度显示
			if(bread!=4096)break;								//读完了.
	 	}
		f_close(fftemp);
	}
	myfree(SRAMIN,fftemp);	//释放内存
	myfree(SRAMIN,tempbuf);	//释放内存
	return res;
}
//更新字体文件,UNIGBK,GBK12,GBK16,GBK24一起更新
//x,y:提示信息的显示地址
//font:字体大小
//提示信息字体大小
//返回值:0,更新成功;
//		 其他,错误代码.
u8 update_font(u16 x,u16 y,u8 font)
{
	u8 *pname;
	u32 *buf;
	u8 res=0;
 	u16 i,j;
	u16 xd,yd;
	FIL *fftemp;
	u8 rval=0;
	
	u16 ufBkCol;
	u16 ufDioCol;
	u16 ufDioTxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		ufBkCol = BLACK;
		ufDioCol = DGRAY;
		ufDioTxCol = WHITE;
	}else{
		ufBkCol = WHITE;
		ufDioCol = LGRAY;
		ufDioTxCol = BLACK;
	}
	
	if(FONTINFOADDR > W25QXX_SIZE){
		return 1;
	}
	res=0XFF;
	ftinfo.fontok=0XFF;
	pname=mymalloc(SRAMIN,100);	//申请100字节内存
	buf=mymalloc(SRAMIN,4096);	//申请4K字节内存
	fftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));	//分配内存
	if(buf==NULL||pname==NULL||fftemp==NULL)
	{
		myfree(SRAMIN,fftemp);
		myfree(SRAMIN,pname);
		myfree(SRAMIN,buf);
		return 5;	//内存申请失败
	}
	//先查找文件是否正常
	strcpy((char*)pname,(char*)GBK16_PATH);
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ);
 	if(res)rval|=1<<4;//打开文件失败
	strcpy((char*)pname,(char*)GBK20_PATH);
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ);
 	if(res)rval|=1<<5;//打开文件失败
	strcpy((char*)pname,(char*)GBK24_PATH);
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ);
 	if(res)rval|=1<<6;//打开文件失败
	myfree(SRAMIN,fftemp);//释放内存
	if(rval==0)//字库文件都存在.
	{
		LCD_FillRect(130,100,280,120,ufDioCol);//弹窗
		LCD_DrawRect(130,100,280,120,GRAY);//边框
		xd = 155;
		yd = 150;
		LCD_SetColor(ufDioCol,ufDioTxCol);
		LCD_ShowString(Font20,xd,yd,0,"Erasing sectors... ");//提示正在擦除扇区
		for(i=0;i<FONTSECSIZE;i++)	//先擦除字库区域,提高写入速度
		{
			fupd_prog(Font20,xd+19*Font20/2,yd,FONTSECSIZE,i);//进度显示
			W25QXX_Read((u8*)buf,((FONTINFOADDR/4096)+i)*4096,4096);//读出整个扇区的内容
			for(j=0;j<1024;j++)//校验数据
			{
				if(buf[j]!=0XFFFFFFFF)break;//需要擦除
			}
			if(j!=1024)W25QXX_Erase_Sector((FONTINFOADDR/4096)+i);	//需要擦除的扇区
		}
		myfree(SRAMIN,buf);
		LCD_ShowString(Font20,xd,yd,0,"Updating GBK16.DZK ");
		strcpy((char*)pname,(char*)GBK16_PATH);
		res=updata_fontx(Font20,xd+19*Font20/2,yd,pname,GBK16);	//更新GBK16.FON
		if(res){myfree(SRAMIN,pname);return 3;}
		LCD_ShowString(Font20,xd,yd,0,"Updating GBK20.DZK ");
		strcpy((char*)pname,(char*)GBK20_PATH);
		res=updata_fontx(Font20,xd+19*Font20/2,yd,pname,GBK20);	//更新GBK24.FON
		if(res){myfree(SRAMIN,pname);return 4;}
		LCD_ShowString(Font20,xd,yd,0,"Updating GBK24.DZK ");
		strcpy((char*)pname,(char*)GBK24_PATH);
		res=updata_fontx(Font20,xd+19*Font20/2,yd,pname,GBK24);	//更新GBK24.FON
		if(res){myfree(SRAMIN,pname);return 5;}
		//全部更新好了
		ftinfo.fontok=0XAA;
		W25QXX_Write((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//保存字库信息
		
		Sleep_ms(100);
		LCD_FillRect(130,100,280,120,ufBkCol);//弹窗
		
		//删除字库文件
		f_unlink(GBK16_PATH);
		f_unlink(GBK20_PATH);
		f_unlink(GBK24_PATH);
	}else{
		LCD_FillRect(130,100,280,120,ufDioCol);//弹窗
		LCD_DrawRect(130,100,280,120,GRAY);//边框
		xd = 155;
		yd = 150;
		LCD_SetColor(ufDioCol,ufDioTxCol);
		LCD_ShowString(Font20,xd,yd,0,"Some files are missing!");//提示正在擦除扇区
		Sleep_ms(2000);
		LCD_FillRect(130,100,280,120,ufBkCol);//弹窗
		return 5;
	}

	myfree(SRAMIN,pname);//释放内存
	myfree(SRAMIN,buf);
	return rval;//无错误.
}
//初始化字体
//返回值:0,字库完好.
//		 其他,字库丢失
u8 font_init(void)
{
	u8 t=0;
	W25QXX_Init();
	while(t<10)//连续读取10次,都是错误,说明确实是有问题,得更新字库了
	{
		t++;
		W25QXX_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//读出ftinfo结构体数据
		if(ftinfo.fontok==0XAA)break;
		Sleep_ms(20);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;
}
//初始化字体
//返回值:0,字库完好.
//		 其他,字库丢失
u8 unioem_init(void)
{
	u8 t=0;
	W25QXX_Init();
	while(t<10)//连续读取10次,都是错误,说明确实是有问题,得更新字库了
	{
		t++;
		W25QXX_Read((u8*)&unioemInfo,UNIOEMADDR,sizeof(unioemInfo));//读出ftinfo结构体数据
		if(unioemInfo.tabok==0XAA)break;
		Sleep_ms(20);
	}
	if(unioemInfo.tabok!=0XAA)return 1;
	return 0;
}
//更新unicoid和gbk转换表
//x,y:提示信息的显示地址
//font:字体大小
//提示信息字体大小
//返回值:0,更新成功;
//		 其他,错误代码.
u8 update_unioem(u16 x,u16 y,u8 font)
{
	u8 *pname;
	u32 *buf;
	u8 res=0;
 	u16 i,j;
	u16 xd,yd;
	FIL *fftemp;
	u8 rval=0;
	
	u16 unoBkCol;
	u16 unoDioCol;
	u16 unoDioTxCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		unoBkCol = BLACK;
		unoDioCol = DGRAY;
		unoDioTxCol = WHITE;
	}else{
		unoBkCol = WHITE;
		unoDioCol = LGRAY;
		unoDioTxCol = BLACK;
	}
	
	if(UNIOEMADDR > W25QXX_SIZE){
		return 1;
	}
	res=0XFF;
	unioemInfo.tabok=0XFF;
	pname=mymalloc(SRAMIN,100);	//申请100字节内存
	buf=mymalloc(SRAMIN,4096);	//申请4K字节内存
	fftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));	//分配内存
	if(buf==NULL||pname==NULL||fftemp==NULL)
	{
		myfree(SRAMIN,fftemp);
		myfree(SRAMIN,pname);
		myfree(SRAMIN,buf);
		return 5;	//内存申请失败
	}
	//先查找文件是否正常
	strcpy((char*)pname,(char*)UNI2OEM_PATH);
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ);
 	if(res)rval|=1<<4;//打开文件失败
	strcpy((char*)pname,(char*)OEM2UNI_PATH);
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ);
 	if(res)rval|=1<<5;//打开文件失败
	myfree(SRAMIN,fftemp);//释放内存
	if(rval==0)//字库文件都存在.
	{
		LCD_FillRect(130,100,280,120,unoDioCol);//弹窗
		LCD_DrawRect(130,100,280,120,GRAY);//边框
		xd = 155;
		yd = 150;
		LCD_SetColor(LLGRAY,unoDioTxCol);
		LCD_ShowString(Font20,xd,yd,0,"Erasing sectors... ");//提示正在擦除扇区
		for(i=0;i<UNIOEMSIZE;i++)	//先擦除字库区域,提高写入速度
		{
			fupd_prog(Font20,xd+19*Font20/2,yd,UNIOEMSIZE,i);//进度显示
			W25QXX_Read((u8*)buf,((UNIOEMADDR/4096)+i)*4096,4096);//读出整个扇区的内容
			for(j=0;j<1024;j++)//校验数据
			{
				if(buf[j]!=0XFFFFFFFF)break;//需要擦除
			}
			if(j!=1024)W25QXX_Erase_Sector((UNIOEMADDR/4096)+i);	//需要擦除的扇区
		}
		myfree(SRAMIN,buf);
		LCD_ShowString(Font20,xd,yd,0,"Updating UNI2OEM.bin");
		strcpy((char*)pname,(char*)UNI2OEM_PATH);
		res=updata_fontx(Font20,xd+19*Font20/2,yd,pname,UNI2OEM);	//更新GBK16.FON
		if(res){myfree(SRAMIN,pname);return 3;}
		LCD_ShowString(Font20,xd,yd,0,"Updating OEM2UNI.bin");
		strcpy((char*)pname,(char*)OEM2UNI_PATH);
		res=updata_fontx(Font20,xd+19*Font20/2,yd,pname,OEM2UNI);	//更新GBK24.FON
		if(res){myfree(SRAMIN,pname);return 4;}
		//全部更新好了
		unioemInfo.tabok=0XAA;
		W25QXX_Write((u8*)&unioemInfo,UNIOEMADDR,sizeof(unioemInfo));	//保存字库信息
		
		Sleep_ms(100);
		LCD_FillRect(130,100,280,120,unoBkCol);//弹窗
		
		//删除字库文件
		f_unlink(UNI2OEM_PATH);
		f_unlink(OEM2UNI_PATH);
	}else{
		LCD_FillRect(130,100,280,120,unoDioCol);//弹窗
		LCD_DrawRect(130,100,280,120,GRAY);//边框
		xd = 155;
		yd = 150;
		LCD_SetColor(unoDioCol,unoDioTxCol);
		LCD_ShowString(Font20,xd,yd,0,"Some files are missing!");//提示正在擦除扇区
		Sleep_ms(2000);
		LCD_FillRect(130,100,280,120,unoBkCol);//弹窗
		return 5;
	}

	myfree(SRAMIN,pname);//释放内存
	myfree(SRAMIN,buf);
	return rval;//无错误.			 
}
