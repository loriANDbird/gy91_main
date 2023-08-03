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

//�ֿ�����ռ�õ�����������С(2���ֿ�:font16+font20+�ֿ���Ϣ<1024K�ֽ�,ȡ256��W25QXX����)
#define FONTSECSIZE	 	512//256//4k/����
//�ֿ�����ʼ��ַ 
#define FONTINFOADDR 	(1024*1024*2) 					//Explorer STM32F4�Ǵ�15M��ַ�Ժ�ʼ����ֿ�
														//ǰ��15M��fatfsռ����.
														//15M�Ժ����2���ֿ�,�ܴ�С1M,���ֿ�ռ����,���ܶ�!

#define UNIOEMSIZE	 	256//256//4k/����
//unicode2oem��ŵ�ַ
#define UNIOEMADDR		(1024*1024*3+256)

//���������ֿ������Ϣ����ַ����С��
_font_info ftinfo;	//�ֿ���Ϣ�ṹ��

_uni_oem_info unioemInfo;	//uni��oem׼����ṹ��

//�ֿ����ڴ����е�·�����ֿ�ʵ��ʹ��GB2312
char*const GBK24_PATH="0:/FONT/GBK24.DZK";	//GBK24�Ĵ��λ�ã����ɵ��ļ�ɾ��ǰ15*94���ַ�����
char*const GBK20_PATH="0:/FONT/GBK20.DZK";	//GBK20�Ĵ��λ��
char*const GBK16_PATH="0:/FONT/GBK16.DZK";	//GBK16�Ĵ��λ��

char*const UNI2OEM_PATH="0:/FONT/U2O.bin";	//uni2oem�Ĵ��λ��
char*const OEM2UNI_PATH="0:/FONT/O2U.bin";	//oem2uni�Ĵ��λ��

//��ʾ��ǰ������½���
//x,y:����
//font:�����С
//fsize:�����ļ���С
//pos:��ǰ�ļ�ָ��λ��
u32 fupd_prog(u8 font,u16 x,u16 y,u32 fsize,u32 pos)
{
	float prog;
	u8 t=0XFF;
	
	u16 upDioCol;
	u16 upDioTxCol;
	//��ɫ����
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
		LCD_ShowNnNums(font,x,y,0,3,t);//��ʾ��ֵ
	}
	return 0;
}
//����ĳһ��
//x,y:����
//font:�����С
//fxpath:·��
//fx:���µ����� 0,ungbk;1,gbk12;2,gbk16;3,gbk24;
//����ֵ:0,�ɹ�;����,ʧ��.
u8 updata_fontx(u8 font,u16 x,u16 y,u8 *fxpath,u8 fx)
{
	u32 flashaddr=0;
	FIL *fftemp;
	u8 *tempbuf;
 	u8 res;
	u16 bread;
	u32 offx=0;
	u8 rval=0;
	fftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));	//�����ڴ�
	if(fftemp==NULL)rval=1;
	tempbuf=mymalloc(SRAMIN,4096);				//����4096���ֽڿռ�
	if(tempbuf==NULL)rval=1;
 	res=f_open(fftemp,(const TCHAR*)fxpath,FA_READ);
 	if(res)rval=2;//���ļ�ʧ��
 	if(rval==0)
	{
		switch(fx)
		{
			case GBK16:
				ftinfo.f16addr=FONTINFOADDR+sizeof(ftinfo);	//GBK12֮�󣬽���GBK16�ֿ�
				ftinfo.gbk16size=fftemp->fsize;					//GBK16�ֿ��С
				flashaddr=ftinfo.f16addr;						//GBK16����ʼ��ַ
				break;
			case GBK20:
				ftinfo.f20addr=ftinfo.f16addr+ftinfo.gbk16size;	//GBK16֮�󣬽���GBK24�ֿ�
				ftinfo.gbk20size=fftemp->fsize;					//GBK24�ֿ��С
				flashaddr=ftinfo.f20addr;						//GBK24����ʼ��ַ
				break;
			case GBK24:
				ftinfo.f24addr=ftinfo.f20addr+ftinfo.gbk20size;	//GBK16֮�󣬽���GBK24�ֿ�
				ftinfo.gbk24size=fftemp->fsize;					//GBK24�ֿ��С
				flashaddr=ftinfo.f24addr;						//GBK24����ʼ��ַ
				break;
			case UNI2OEM:
				unioemInfo.uni2oemAddr=UNIOEMADDR+sizeof(unioemInfo);	//GBK16֮�󣬽���GBK24�ֿ�
				unioemInfo.uni2oemSize=fftemp->fsize;					//GBK24�ֿ��С
				flashaddr=unioemInfo.uni2oemAddr;						//GBK24����ʼ��ַ
				break;
			case OEM2UNI:
				unioemInfo.oem2uniAddr=unioemInfo.uni2oemAddr+unioemInfo.uni2oemSize;	//GBK16֮�󣬽���GBK24�ֿ�
				unioemInfo.oem2uniSize=fftemp->fsize;					//GBK24�ֿ��С
				flashaddr=unioemInfo.oem2uniAddr;						//GBK24����ʼ��ַ
				break;
			default:
				break;
		}
		
		while(res==FR_OK)//��ѭ��ִ��
		{
	 		res=f_read(fftemp,tempbuf,4096,(UINT *)&bread);		//��ȡ����
			if(res!=FR_OK)break;								//ִ�д���
			W25QXX_Write(tempbuf,offx+flashaddr,4096);		//��0��ʼд��4096������
	  		offx+=bread;
			fupd_prog(font,x,y,fftemp->fsize,offx);	 			//������ʾ
			if(bread!=4096)break;								//������.
	 	}
		f_close(fftemp);
	}
	myfree(SRAMIN,fftemp);	//�ͷ��ڴ�
	myfree(SRAMIN,tempbuf);	//�ͷ��ڴ�
	return res;
}
//���������ļ�,UNIGBK,GBK12,GBK16,GBK24һ�����
//x,y:��ʾ��Ϣ����ʾ��ַ
//font:�����С
//��ʾ��Ϣ�����С
//����ֵ:0,���³ɹ�;
//		 ����,�������.
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
	//��ɫ����
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
	pname=mymalloc(SRAMIN,100);	//����100�ֽ��ڴ�
	buf=mymalloc(SRAMIN,4096);	//����4K�ֽ��ڴ�
	fftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));	//�����ڴ�
	if(buf==NULL||pname==NULL||fftemp==NULL)
	{
		myfree(SRAMIN,fftemp);
		myfree(SRAMIN,pname);
		myfree(SRAMIN,buf);
		return 5;	//�ڴ�����ʧ��
	}
	//�Ȳ����ļ��Ƿ�����
	strcpy((char*)pname,(char*)GBK16_PATH);
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ);
 	if(res)rval|=1<<4;//���ļ�ʧ��
	strcpy((char*)pname,(char*)GBK20_PATH);
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ);
 	if(res)rval|=1<<5;//���ļ�ʧ��
	strcpy((char*)pname,(char*)GBK24_PATH);
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ);
 	if(res)rval|=1<<6;//���ļ�ʧ��
	myfree(SRAMIN,fftemp);//�ͷ��ڴ�
	if(rval==0)//�ֿ��ļ�������.
	{
		LCD_FillRect(130,100,280,120,ufDioCol);//����
		LCD_DrawRect(130,100,280,120,GRAY);//�߿�
		xd = 155;
		yd = 150;
		LCD_SetColor(ufDioCol,ufDioTxCol);
		LCD_ShowString(Font20,xd,yd,0,"Erasing sectors... ");//��ʾ���ڲ�������
		for(i=0;i<FONTSECSIZE;i++)	//�Ȳ����ֿ�����,���д���ٶ�
		{
			fupd_prog(Font20,xd+19*Font20/2,yd,FONTSECSIZE,i);//������ʾ
			W25QXX_Read((u8*)buf,((FONTINFOADDR/4096)+i)*4096,4096);//������������������
			for(j=0;j<1024;j++)//У������
			{
				if(buf[j]!=0XFFFFFFFF)break;//��Ҫ����
			}
			if(j!=1024)W25QXX_Erase_Sector((FONTINFOADDR/4096)+i);	//��Ҫ����������
		}
		myfree(SRAMIN,buf);
		LCD_ShowString(Font20,xd,yd,0,"Updating GBK16.DZK ");
		strcpy((char*)pname,(char*)GBK16_PATH);
		res=updata_fontx(Font20,xd+19*Font20/2,yd,pname,GBK16);	//����GBK16.FON
		if(res){myfree(SRAMIN,pname);return 3;}
		LCD_ShowString(Font20,xd,yd,0,"Updating GBK20.DZK ");
		strcpy((char*)pname,(char*)GBK20_PATH);
		res=updata_fontx(Font20,xd+19*Font20/2,yd,pname,GBK20);	//����GBK24.FON
		if(res){myfree(SRAMIN,pname);return 4;}
		LCD_ShowString(Font20,xd,yd,0,"Updating GBK24.DZK ");
		strcpy((char*)pname,(char*)GBK24_PATH);
		res=updata_fontx(Font20,xd+19*Font20/2,yd,pname,GBK24);	//����GBK24.FON
		if(res){myfree(SRAMIN,pname);return 5;}
		//ȫ�����º���
		ftinfo.fontok=0XAA;
		W25QXX_Write((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//�����ֿ���Ϣ
		
		Sleep_ms(100);
		LCD_FillRect(130,100,280,120,ufBkCol);//����
		
		//ɾ���ֿ��ļ�
		f_unlink(GBK16_PATH);
		f_unlink(GBK20_PATH);
		f_unlink(GBK24_PATH);
	}else{
		LCD_FillRect(130,100,280,120,ufDioCol);//����
		LCD_DrawRect(130,100,280,120,GRAY);//�߿�
		xd = 155;
		yd = 150;
		LCD_SetColor(ufDioCol,ufDioTxCol);
		LCD_ShowString(Font20,xd,yd,0,"Some files are missing!");//��ʾ���ڲ�������
		Sleep_ms(2000);
		LCD_FillRect(130,100,280,120,ufBkCol);//����
		return 5;
	}

	myfree(SRAMIN,pname);//�ͷ��ڴ�
	myfree(SRAMIN,buf);
	return rval;//�޴���.
}
//��ʼ������
//����ֵ:0,�ֿ����.
//		 ����,�ֿⶪʧ
u8 font_init(void)
{
	u8 t=0;
	W25QXX_Init();
	while(t<10)//������ȡ10��,���Ǵ���,˵��ȷʵ��������,�ø����ֿ���
	{
		t++;
		W25QXX_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//����ftinfo�ṹ������
		if(ftinfo.fontok==0XAA)break;
		Sleep_ms(20);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;
}
//��ʼ������
//����ֵ:0,�ֿ����.
//		 ����,�ֿⶪʧ
u8 unioem_init(void)
{
	u8 t=0;
	W25QXX_Init();
	while(t<10)//������ȡ10��,���Ǵ���,˵��ȷʵ��������,�ø����ֿ���
	{
		t++;
		W25QXX_Read((u8*)&unioemInfo,UNIOEMADDR,sizeof(unioemInfo));//����ftinfo�ṹ������
		if(unioemInfo.tabok==0XAA)break;
		Sleep_ms(20);
	}
	if(unioemInfo.tabok!=0XAA)return 1;
	return 0;
}
//����unicoid��gbkת����
//x,y:��ʾ��Ϣ����ʾ��ַ
//font:�����С
//��ʾ��Ϣ�����С
//����ֵ:0,���³ɹ�;
//		 ����,�������.
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
	//��ɫ����
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
	pname=mymalloc(SRAMIN,100);	//����100�ֽ��ڴ�
	buf=mymalloc(SRAMIN,4096);	//����4K�ֽ��ڴ�
	fftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));	//�����ڴ�
	if(buf==NULL||pname==NULL||fftemp==NULL)
	{
		myfree(SRAMIN,fftemp);
		myfree(SRAMIN,pname);
		myfree(SRAMIN,buf);
		return 5;	//�ڴ�����ʧ��
	}
	//�Ȳ����ļ��Ƿ�����
	strcpy((char*)pname,(char*)UNI2OEM_PATH);
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ);
 	if(res)rval|=1<<4;//���ļ�ʧ��
	strcpy((char*)pname,(char*)OEM2UNI_PATH);
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ);
 	if(res)rval|=1<<5;//���ļ�ʧ��
	myfree(SRAMIN,fftemp);//�ͷ��ڴ�
	if(rval==0)//�ֿ��ļ�������.
	{
		LCD_FillRect(130,100,280,120,unoDioCol);//����
		LCD_DrawRect(130,100,280,120,GRAY);//�߿�
		xd = 155;
		yd = 150;
		LCD_SetColor(LLGRAY,unoDioTxCol);
		LCD_ShowString(Font20,xd,yd,0,"Erasing sectors... ");//��ʾ���ڲ�������
		for(i=0;i<UNIOEMSIZE;i++)	//�Ȳ����ֿ�����,���д���ٶ�
		{
			fupd_prog(Font20,xd+19*Font20/2,yd,UNIOEMSIZE,i);//������ʾ
			W25QXX_Read((u8*)buf,((UNIOEMADDR/4096)+i)*4096,4096);//������������������
			for(j=0;j<1024;j++)//У������
			{
				if(buf[j]!=0XFFFFFFFF)break;//��Ҫ����
			}
			if(j!=1024)W25QXX_Erase_Sector((UNIOEMADDR/4096)+i);	//��Ҫ����������
		}
		myfree(SRAMIN,buf);
		LCD_ShowString(Font20,xd,yd,0,"Updating UNI2OEM.bin");
		strcpy((char*)pname,(char*)UNI2OEM_PATH);
		res=updata_fontx(Font20,xd+19*Font20/2,yd,pname,UNI2OEM);	//����GBK16.FON
		if(res){myfree(SRAMIN,pname);return 3;}
		LCD_ShowString(Font20,xd,yd,0,"Updating OEM2UNI.bin");
		strcpy((char*)pname,(char*)OEM2UNI_PATH);
		res=updata_fontx(Font20,xd+19*Font20/2,yd,pname,OEM2UNI);	//����GBK24.FON
		if(res){myfree(SRAMIN,pname);return 4;}
		//ȫ�����º���
		unioemInfo.tabok=0XAA;
		W25QXX_Write((u8*)&unioemInfo,UNIOEMADDR,sizeof(unioemInfo));	//�����ֿ���Ϣ
		
		Sleep_ms(100);
		LCD_FillRect(130,100,280,120,unoBkCol);//����
		
		//ɾ���ֿ��ļ�
		f_unlink(UNI2OEM_PATH);
		f_unlink(OEM2UNI_PATH);
	}else{
		LCD_FillRect(130,100,280,120,unoDioCol);//����
		LCD_DrawRect(130,100,280,120,GRAY);//�߿�
		xd = 155;
		yd = 150;
		LCD_SetColor(unoDioCol,unoDioTxCol);
		LCD_ShowString(Font20,xd,yd,0,"Some files are missing!");//��ʾ���ڲ�������
		Sleep_ms(2000);
		LCD_FillRect(130,100,280,120,unoBkCol);//����
		return 5;
	}

	myfree(SRAMIN,pname);//�ͷ��ڴ�
	myfree(SRAMIN,buf);
	return rval;//�޴���.			 
}
