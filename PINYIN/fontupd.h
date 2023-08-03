#ifndef __FONTUPD_H__
#define __FONTUPD_H__
#include "mcu_io.h"


//������Ϣ�����ַ,ռ33���ֽ�,��1���ֽ����ڱ���ֿ��Ƿ����.����ÿ8���ֽ�һ��,�ֱ𱣴���ʼ��ַ���ļ���С
extern u32 FONTINFOADDR;
//�ֿ���Ϣ�ṹ�嶨��
//���������ֿ������Ϣ����ַ����С��
__packed typedef struct
{
	u8 fontok;				//�ֿ���ڱ�־��0XAA���ֿ��������������ֿⲻ����	 
	u8 blank[3];
	u32 f16addr;			//gbk16��ַ
	u32 gbk16size;			//gbk16�Ĵ�С
	u32 f20addr;			//gbk20��ַ
	u32 gbk20size;			//gbk20�Ĵ�С
	u32 f24addr;			//gbk24��ַ
	u32 gbk24size;			//gbk24�Ĵ�С
}_font_info;

//unicode2oem��洢
__packed typedef struct
{
	u8 tabok;				//�ֿ���ڱ�־��0XAA���ֿ��������������ֿⲻ����	 
	u8 blank[3];
	u32 uni2oemAddr;		//uni2oem��ַ
	u32 uni2oemSize;		//uni2oem�Ĵ�С
	u32 oem2uniAddr;		//oem2uni��ַ
	u32 oem2uniSize;		//oem2uni�Ĵ�С
	
}_uni_oem_info;

extern _font_info ftinfo;	//�ֿ���Ϣ�ṹ��
extern _uni_oem_info unioemInfo;	//uni��oem׼����ṹ��

u32 fupd_prog(u8 font,u16 x,u16 y,u32 fsize,u32 pos);	//��ʾ���½���
u8 updata_fontx(u8 font,u16 x,u16 y,u8 *fxpath,u8 fx);	//����ָ���ֿ�
u8 update_font(u16 x,u16 y,u8 font);					//����ȫ���ֿ�
u8 update_unioem(u16 x,u16 y,u8 font);
u8 font_init(void);										//��ʼ���ֿ�
u8 unioem_init(void);
#endif





















