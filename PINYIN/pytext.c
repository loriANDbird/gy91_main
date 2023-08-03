#include "mcu_io.h"
#include "fontupd.h"
#include "w25qxx.h"
#include "lcd.h"
#include "pytext.h"
#include "string.h"

//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С
//font:�����С
void Get_HzMat(u8 font,unsigned char *code,unsigned char *mat)
{
	unsigned char qh,ql;
	unsigned char i;
	unsigned long foffset;
	u8 csize=(font/8+((font%8)?1:0))*(font);//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	qh=*code;
	ql=*(++code);
	if(qh<0xB0||ql==0xff||qh==0xff){//�� ���ú���
	
	    for(i=0;i<csize;i++)*mat++=0x00;//�������
	    return; //��������
	}
//	if(ql<0x7f)ql-=0x40;//ע��!
//	else ql-=0x41;
	qh-=0xB0;
	ql-=0xA1;
	foffset=(qh*94+ql)*csize;	//�õ��ֿ��е��ֽ�ƫ����
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

//��ʾ�ַ���
//font:���壬mode:0������1���ӣ�str:�ַ���
void Show_Text(u8 font,u16 x,u16 y,u8 mode,u8 *str)
{
	u8 zimoBuf[100];
	u16 width,height;
	
	while(*str!=0){//����δ����
    
		if((*str&0x80) == 0){//Ӣ��
			width = font/2;
			height = font;
			if(mode == 0){
				LCD_ShowASC(font,x,y,*str);
			}else{
				LCD_ShowASC(font,x,y,*str);
			}
			x += width;
			str += 1;
		}else{//����
			
			width = font;
			height = font;
			Get_HzMat(font,str,zimoBuf);
			LCD_ShowZimo(x,y,width,height,mode,zimoBuf);
			x += width;
			str += 2;
		}
	}
}
























		  






