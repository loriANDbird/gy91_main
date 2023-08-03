#ifndef FONT_H
#define FONT_H


#define Font8	    8
#define Font12		12
#define Font14		14
#define Font16		16
#define Font20		20
#define Font24		24
#define Font32		32
#define Font36		36
#define Font40		40
#define Font48		48
#define Font64		64

#define ASC12x6			12
#define ASC16x8			16
#define ASC20x10		20
#define ASC24x12		24
#define ASC32x16		32
#define ASC40x20		40
#define ASC64x32		64
#define ASC80x40		80

#define NUM12x6			12
#define NUM16x8			16
#define NUM20x10		20
#define NUM24x12		24
#define NUM32x16		32
#define NUM40x20		40
#define NUM64x32		64
#define NUM80x40		80
#define NUM96x48		96


// ------------------  汉字字模的数据结构定义 ------------------------ //
typedef struct typFNT_GB8               // 汉字字模数据结构
{
       unsigned char Index[3];              // 汉字内码索引
       unsigned char Msk[8];              // 点阵码数据
}typFNT_GB8;

typedef struct typFNT_GB12               // 汉字字模数据结构
{
       unsigned char Index[3];              // 汉字内码索引
       unsigned char Msk[24];              // 点阵码数据
}typFNT_GB12;

typedef struct typFNT_GB14               // 汉字字模数据结构
{
       unsigned char Index[3];              // 汉字内码索引
       unsigned char Msk[28];              // 点阵码数据
}typFNT_GB14;

typedef struct typFNT_GB16               // 汉字字模数据结构
{
       unsigned char Index[3];              // 汉字内码索引
       unsigned char Msk[32];              // 点阵码数据
}typFNT_GB16;

typedef struct typFNT_GB20               // 汉字字模数据结构
{
       unsigned char Index[3];              // 汉字内码索引
       unsigned char Msk[60];              // 点阵码数据
}typFNT_GB20;

typedef struct typFNT_GB24               // 汉字字模数据结构
{
       unsigned char Index[3];              // 汉字内码索引
       unsigned char Msk[72];              // 点阵码数据
}typFNT_GB24;

typedef struct typFNT_GB28               // 汉字字模数据结构
{
       unsigned char Index[3];              // 汉字内码索引
       unsigned char Msk[112];              // 点阵码数据
}typFNT_GB28;

typedef struct typFNT_GB32               // 汉字字模数据结构
{
       unsigned char Index[3];              // 汉字内码索引
       unsigned char Msk[128];              // 点阵码数据
}typFNT_GB32;

typedef struct typFNT_GB36               // 汉字字模数据结构
{
       unsigned char Index[3];              // 汉字内码索引
       unsigned char Msk[180];              // 点阵码数据
}typFNT_GB36;

typedef struct typFNT_GB40               // 汉字字模数据结构
{
       unsigned char Index[3];              // 汉字内码索引
       unsigned char Msk[200];              // 点阵码数据
}typFNT_GB40;

typedef struct typFNT_GB48               // 汉字字模数据结构
{
       unsigned char Index[3];              // 汉字内码索引
       unsigned char Msk[288];              // 点阵码数据
}typFNT_GB48;

//typedef struct typFNT_GB64               // 汉字字模数据结构
//{
//       unsigned char Index[3];              // 汉字内码索引
//       unsigned char Msk[512];              // 点阵码数据
//}typFNT_GB64;

extern const unsigned char ASC2_1206[95][12];
extern const unsigned char ASC2_1608[95][16];
extern const unsigned char ASC2_2010[95][30];
extern const unsigned char ASC2_2412[95][36];
extern const unsigned char ASC2_3216[95][64];
extern const unsigned char ASC2_4020[95][100];


extern const unsigned char NUM_1206[13][12];
extern const unsigned char NUM_1608[13][16];
extern const unsigned char NUM_2010[13][30];
extern const unsigned char NUM_2412[13][36];
extern const unsigned char NUM_3216[13][64];
extern const unsigned char NUM_4020[13][100];

extern const unsigned char NUM_9648[7][576];

extern const typFNT_GB8 GB_8[];
extern const typFNT_GB12 GB_12[];
extern const typFNT_GB14 GB_14[];
extern const typFNT_GB16 GB_16[];
extern const typFNT_GB20 GB_20[];
extern const typFNT_GB24 GB_24[];
//extern const typFNT_GB28 GB_28[];
extern const typFNT_GB32 GB_32[];
//extern const typFNT_GB36 GB_36[];
extern const typFNT_GB40 GB_40[];
//extern const typFNT_GB48 GB_48[];
//extern const typFNT_GB64 GB_64[];

//图片
extern const unsigned char gImage_detect[7200];
extern const unsigned char gImage_jgj[7200];  
extern const unsigned char gImage_block[7200];
extern const unsigned char gImage_browse[7200];
extern const unsigned char gImage_verify[7200];
extern const unsigned char gImage_sysset[7200];

extern const unsigned char	gImage_detect_light[7200];
extern const unsigned char gImage_jgj_light[7200];  
extern const unsigned char gImage_block_light[7200];
extern const unsigned char gImage_browse_light[7200];
extern const unsigned char gImage_verify_light[7200];
extern const unsigned char gImage_sysset_light[7200];

extern const unsigned char gImage_detect_hl[7200];
extern const unsigned char gImage_jgj_hl[7200];  
extern const unsigned char gImage_block_hl[7200];
extern const unsigned char gImage_browse_hl[7200];
extern const unsigned char gImage_verify_hl[7200];
extern const unsigned char gImage_sysset_hl[7200];


extern const unsigned char gImage_hcgk_dark[11520];
extern const unsigned char gImage_hcgk_light[11520];
#endif
