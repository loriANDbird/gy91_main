#ifndef  ZIKU_H
#define ZIKU_H

#pragma  diag_suppress 870


#define FONT80		80
#define FONT56		56	//字符的宽度
#define FONT48		48
#define FONT40		40
#define FONT32		32
#define FONT24		24
#define FONT20		20
#define FONT16		16
#define FONT12		12
#define FONT6		 6


#define ASC6x8			 6  	//字符的宽度，放宽一个像素
#define ASC8x16		     8
#define ASC12x24		12
#define ASC20x32		20
#define ASC24x40		24
#define ASC28x48		28
#define ASC32x56		32
#define ASC48x80		48

#define ASC10x20		10

#define PICTYPE104  104


#define CH56x56_UNIT_LENTH	            392
#define CH48x48_UNIT_LENTH	            288
#define CH40x40_UNIT_LENTH	            200
#define CH32x32_UNIT_LENTH	            128
#define CH24x24_UNIT_LENTH	            72
#define CH20x20_UNIT_LENTH	            60
#define CH16x16_UNIT_LENTH	            32

#define PIC104x85_UNIT_LENTH	          1352

#define nAscii6x8Dot_UNIT_LENTH	          8
#define nAscii8x16Dot_UNIT_LENTH	     16
#define nAscii12x24Dot_UNIT_LENTH	     48 
#define nAscii20x32Dot_UNIT_LENTH	     96
#define nAscii24x40Dot_UNIT_LENTH	     120
#define nAscii28x48Dot_UNIT_LENTH	     192 
#define nAscii32x56Dot_UNIT_LENTH	     224
#define nAscii48x80Dot_UNIT_LENTH	     480


#define LANG_CHN        0
#define LANG_ENG        1
#define LANG_KOR        2
#define LANG_JPN        3

#define IS_VALID_LANG(LANG) (((LANG) == LANG_CHN) || \
                             ((LANG) == LANG_ENG) || \
                             ((LANG) == LANG_KOR) || \
                             ((LANG) == LANG_JPN))


//汉字数目
// ------------------  汉字字模的数据结构定义 ------------------------ //
typedef struct typFNT_GB56               // 汉字字模数据结构
{
       char Index[4];              // 汉字内码索引
       char Msk[392];              // 点阵码数据
       //char Msk[175];              // 点阵码数据
}typFNT_GB56;
typedef struct typFNT_GB48              // 汉字字模数据结构
{
       char Index[4];              // 汉字内码索引
       char Msk[288];              // 点阵码数据
       //char Msk[175];              // 点阵码数据
}typFNT_GB48;
typedef struct typFNT_GB40               // 汉字字模数据结构
{
       char Index[4];              // 汉字内码索引
       char Msk[200];              // 点阵码数据
       //char Msk[175];              // 点阵码数据
}typFNT_GB40;
typedef struct typFNT_GB32               // 汉字字模数据结构
{
       char Index[4];              // 汉字内码索引
       char Msk[128];              // 点阵码数据
       //char Msk[175];              // 点阵码数据
}typFNT_GB32;
// ------------------  汉字字模的数据结构定义 ------------------------ //
typedef struct typFNT_GB24         // 汉字字模数据结构
{
       char Index[4];              // 汉字内码索引
       char Msk[72];               // 点阵码数据
}typFNT_GB24;


// ------------------  汉字字模的数据结构定义 ------------------------ //
typedef struct typFNT_GB20                // 汉字字模数据结构
{
       char Index[4];              // 汉字内码索引
       char Msk[60];                      // 点阵码数据
}typFNT_GB20;
// ------------------  汉字字模的数据结构定义 ------------------------ //
typedef struct typFNT_GB16                 // 汉字字模数据结构
{
       char Index[4];               // 汉字内码索引
       char Msk[32];                       // 点阵码数据
}typFNT_GB16;
// ------------------  汉字字模的数据结构定义 ------------------------ //
typedef struct typFNT_GB12                // 汉字字模数据结构
{
       char Index[4];               // 汉字内码索引
       char Msk[24];                       // 点阵码数据
}typFNT_GB12;

typedef struct typPIC_W104_icon                 // 汉字字模数据结构
{
       char Index[4];               // 汉字内码索引
       char Msk[1352];                       // 点阵码数据
}typPIC_W104_icon;



typedef struct str_LANG_PACKAGE
{    
     char ui_Start_ID_Start[13];           //ui_Start界面  Start
     char ui_Start_ID_JianDing[10]; 
     char ui_Start_ID_Browse[10]; 
     char ui_Start_ID_Setting[10];
   
     char ui_Main_ID_Titile[10];          //ui_Start界面  Start  
     char ui_Main_ID_Info[10]; 
    
}str_LANG_PACKAGE;



extern const struct str_LANG_PACKAGE g_LangPack_CHN;
extern const struct str_LANG_PACKAGE g_LangPack_KOR;
extern const struct str_LANG_PACKAGE g_LangPack_JPN;
extern const struct str_LANG_PACKAGE g_LangPack_ENG; 


extern const struct str_LANG_PACKAGE *g_pLang;
extern unsigned char g_uclanguage;	//语言编号

extern const char *pFONT56;
extern const char *pFONT48;
extern const char *pFONT40;
extern const char *pFONT32;
extern const char *pFONT24;
extern const char *pFONT20;
extern const char *pFONT16;
extern const char *pFONT12;

extern const char *pPIC104;

extern const typPIC_W104_icon W_104[];
extern const unsigned char HC_HDxxx_48x256Dot[];

extern const typFNT_GB56 GB_56[];
extern const typFNT_GB48 GB_48[];
extern const typFNT_GB40 GB_40[];
extern const typFNT_GB32 GB_32[];
extern const typFNT_GB24 GB_24[];
extern const typFNT_GB20 GB_20[];
extern const typFNT_GB16 GB_16[];
extern const typFNT_GB12 GB_12[];

extern const typFNT_GB32 KOR_32[];
extern const typFNT_GB24 KOR_24[];
extern const typFNT_GB20 KOR_20[];
extern const typFNT_GB16 KOR_16[];

extern const typFNT_GB32 JPN_32[];
extern const typFNT_GB24 JPN_24[];
extern const typFNT_GB20 JPN_20[];
extern const typFNT_GB16 JPN_16[];

extern const unsigned char nAscii6x8Dot[]; 
extern const unsigned char nAscii8x16Dot[]; 
extern const unsigned char nAscii12x24Dot[];
extern const unsigned char nAscii20x32Dot[];
extern const unsigned char nAscii24x40Dot[]; 
extern const unsigned char nAscii28x48Dot[];
extern const unsigned char nAscii32x56Dot[];
extern const unsigned char nAscii48x80Dot[];
extern const unsigned char nAscii10x20Dot[];

extern void SetLanguage(unsigned char lang);


#endif
