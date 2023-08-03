#include "mcu_io.h"
#include "fontupd.h"
#include "w25qxx.h"
#include "lcd.h"
#include "pytext.h"
#include "string.h"

//code 字符指针开始
//从字库中查找出字模
//code 字符串的开始地址,GBK码
//mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小
//font:字体大小
void Get_HzMat(u8 font,unsigned char *code,unsigned char *mat)
{
	unsigned char qh,ql;
	unsigned char i;
	unsigned long foffset;
	u8 csize=(font/8+((font%8)?1:0))*(font);//得到字体一个字符对应点阵集所占的字节数
	qh=*code;
	ql=*(++code);
	if(qh<0xB0||ql==0xff||qh==0xff){//非 常用汉字
	
	    for(i=0;i<csize;i++)*mat++=0x00;//填充满格
	    return; //结束访问
	}
//	if(ql<0x7f)ql-=0x40;//注意!
//	else ql-=0x41;
	qh-=0xB0;
	ql-=0xA1;
	foffset=(qh*94+ql)*csize;	//得到字库中的字节偏移量
	switch(font){
	
		case 16:
			W25QXX_Read(mat,foffset+ftinfo.f16addr,csize);
			break;
		case 20:
			W25QXX_Read(mat,foffset+ftinfo.f20addr,csize);
			break;
		case 24:
			W25QXX_Read(mat,foffset+ftinfo.f24addr,csize);
			break;
	}
}

//显示字符串
//font:字体，mode:0不叠加1叠加，str:字符串
void Show_Text(u8 font,u16 x,u16 y,u8 mode,u8 *str)
{
	u8 zimoBuf[100];
	u16 width,height;
	
	while(*str!=0){//数据未结束
    
		if((*str&0x80) == 0){//英文
			width = font/2;
			height = font;
			if(mode == 0){
				LCD_ShowASC(font,x,y,*str);
			}else{
				LCD_ShowASC(font,x,y,*str);
			}
			x += width;
			str += 1;
		}else{//中文
			
			width = font;
			height = font;
			Get_HzMat(font,str,zimoBuf);
			LCD_ShowZimo(x,y,width,height,mode,zimoBuf);
			x += width;
			str += 2;
		}
	}
}
























		  






