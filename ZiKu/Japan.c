/******************************************************************************
创建者：康泉
创建日期：2018.03.01
功能描述：
    1. 字库
    2. 
    
更新记录：
    1. 2018.03.01 初次创建
    
*******************************************************************************/

#include "ZiKu.h"


const struct str_LANG_PACKAGE g_LangPack_JPN = 
{
    "する検査",   //char ui_Start_ID_Start[10];          //ui_Start界面
    "とう",   //char ui_Start_ID_JianDing[10]; 
    "とう",   //char ui_Start_ID_Browse[10]; 
    "とう",   //char ui_Start_ID_Setting[10];
   
    "とう",   //char ui_Main_ID_Titile[10];          //ui_Main界面  
    "とう",   //char ui_Main_ID_Info[10]; 
};

/////////////////////////////////////////////////////////////////////////
// 汉字库: 黑体35.dot 纵向取模上高位,数据排列:从上到下从左到右  图标       //
const typFNT_GB32 JPN_32[] =        // 数据表   // 16
{
"と", 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x07,
      0x02,0x00,0x00,0x00,0x07,0x04,0x00,0x00,
      0x00,0x07,0x08,0x00,0x00,0x00,0x07,0x10,
      0x00,0x00,0x00,0x07,0x20,0x00,0x00,0x00,
      0x07,0x40,0x00,0x00,0x00,0x07,0x00,0x00,
      0x00,0x00,0x07,0x02,0x00,0x00,0x00,0x07,
      0x04,0x00,0x00,0x00,0x07,0x08,0x00,0x00,
      0x00,0x07,0x10,0x00,0x00,0x00,0x07,0x20,
      0x03,0xE0,0x00,0x07,0x40,0x07,0xF0,0x00,
      0x07,0x00,0x07,0xF0,0x00,0x07,0x00,0x07,
      0xF0,0x00,0x07,0x02,0x07,0xF0,0x00,0x07,
      0x04,0x07,0xF0,0x00,0x07,0x08,0x03,0xE0,
      0x00,0x07,0x10,0x00,0x00,0x00,0x07,0x20,
      0x00,0x00,0x00,0x07,0x40,0x00,0x00,0x00,
      0x07,0x00,0x00,0x00,0x00,0x07,0x02,0x00,

"う", 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x07,
      0x02,0x00,0x00,0x00,0x07,0x04,0x00,0x00,
      0x00,0x07,0x08,0x00,0x00,0x00,0x07,0x10,
      0x00,0x00,0x00,0x07,0x20,0x00,0x00,0x00,
      0x07,0x40,0x00,0x00,0x00,0x07,0x00,0x00,
      0x00,0x00,0x07,0x02,0x00,0x00,0x00,0x07,
      0x04,0x00,0x00,0x00,0x07,0x08,0x00,0x00,
      0x00,0x07,0x10,0x00,0x00,0x00,0x07,0x20,
      0x03,0xE0,0x00,0x07,0x40,0x07,0xF0,0x00,
      0x07,0x00,0x07,0xF0,0x00,0x07,0x00,0x07,
      0xF0,0x00,0x07,0x02,0x07,0xF0,0x00,0x07,
      0x04,0x07,0xF0,0x00,0x07,0x08,0x03,0xE0,
      0x00,0x07,0x10,0x00,0x00,0x00,0x07,0x20,
      0x00,0x00,0x00,0x07,0x40,0x00,0x00,0x00,
      0x07,0x00,0x00,0x00,0x00,0x07,0x02,0x00,


"呕", 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	  //结束字符
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

/////////////////////////////////////////////////////////////////////////
// 汉字字模表    
// 汉字库: 黑体24.dot 纵向取模上高位,数据排列:从上到下从左到右  空       //
/////////////////////////////////////////////////////////////////////////
const struct typFNT_GB24 JPN_24[] =        // 数据表   // 66
{
"す", 	
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x00,0x38,0x00,0x00,0x38,0x00,0x00,
0x3F,0xF0,0x7F,0xFF,0xF0,0x7F,0xFF,0xF0,0x7F,0xF8,0x00,0x01,0xF8,0x00,0x03,0xF8,
0x00,0x07,0xF8,0x00,0x07,0x38,0x00,0x07,0x38,0x00,0x07,0xF8,0x00,0x03,0xF8,0x00,
0x01,0xF8,0x00,0x00,0x70,0x00,0x00,0xF0,0x00,0x01,0xE0,0x00,0x03,0xC0,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
     
 "る",
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x07,0xFE,0x00,0x07,0xFE,0x00,0x03,
0xFC,0x00,0x00,0x38,0x00,0x00,0x70,0x00,0x00,0xE0,0x00,0x01,0xFE,0x00,0x03,0xFF,
0x80,0x07,0xFF,0x80,0x0F,0x83,0xC0,0x1E,0xE1,0xC0,0x1D,0xF0,0xC0,0x0B,0xF8,0xC0,
0x03,0x9D,0xC0,0x03,0xDF,0xC0,0x01,0xFF,0x80,0x01,0xFF,0x00,0x00,0x7C,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      
 "検",
0x00,0x00,0x00,0x18,0x0E,0x00,0x18,0x1F,0x00,0x18,0x1F,0x80,0x18,0x3B,0x80,0x7E,
0x71,0xC0,0x7E,0xFF,0xF0,0x7F,0xFF,0xB0,0x39,0xCE,0x10,0x3C,0x8E,0x00,0x3F,0xFF,
0xF0,0x7F,0xFF,0xF0,0x79,0xCE,0x30,0xF9,0xCE,0x30,0xD9,0xFF,0xF0,0x59,0xFF,0xF0,
0x18,0x1F,0x00,0x18,0x3B,0x80,0x18,0xF9,0xC0,0x19,0xF0,0xE0,0x19,0xE0,0x70,0x18,
0x80,0x30,0x00,0x00,0x00,0x00,0x00,0x00,

 "査",
0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x70,0x00,0x7F,0xFF,0xF0,0x7F,0xFF,0xF0,0x03,
0xFC,0x00,0x0F,0x77,0x00,0x3E,0x73,0xE0,0xFC,0x71,0xF0,0x7F,0xFF,0xF0,0x5F,0xFF,
0xF0,0x1C,0x01,0xC0,0x1F,0xFF,0xC0,0x1F,0xFF,0xC0,0x1C,0x01,0xC0,0x1C,0x01,0xC0,
0x1F,0xFF,0xC0,0x1F,0xFF,0xC0,0x1C,0x01,0xC0,0x7F,0xFF,0xF0,0x7F,0xFF,0xF0,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

"呕", 0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,  //结束字符
      0xFE,0x7F,0xFF,0xFE,0x7F,0xFF,0xFE,0x70,
      0x00,0x1C,0x70,0x00,0x1C,0x71,0xFF,0x1C,
      0x71,0xFF,0x1C,0x71,0xFF,0x1C,0x71,0x83,
      0x1C,0x71,0x83,0x1C,0x71,0x83,0x1C,0x71,
      0x83,0x1C,0x71,0xFF,0x1C,0x71,0xFF,0x1C,
      0x71,0xFF,0x1C,0x70,0x00,0x1C,0x70,0x00,
      0x1C,0x7F,0xFF,0xFE,0x7F,0xFF,0xFE,0x7F,
      0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,

};


// 汉字表：Font:黑体20,多余字体
const typFNT_GB20 JPN_20[] =        // 数据表183+4  
{
"と", 0x00,0x00,0x00,0x0C,0x18,0x00,0x0C,0x78,
      0x00,0x0D,0xE0,0x00,0xFF,0xFF,0xC0,0xFF,
      0xFF,0xC0,0x0C,0x60,0x00,0x0D,0x31,0x80,
      0x03,0x61,0x80,0x0E,0x7D,0x80,0x1F,0x1D,
      0x80,0x3B,0x61,0x80,0xF3,0x79,0x80,0xF3,
      0x19,0x80,0x3B,0x07,0x80,0x1F,0x3F,0x80,
      0x0E,0x79,0x80,0x07,0x61,0x80,0x03,0x01,
      0x80,0x00,0x00,0x00,

"う", 0x00,0x00,0x00,0x02,0x00,0x80,0x63,0x03,
      0xC0,0x71,0x8F,0x00,0x30,0x0C,0x00,0x7F,
      0xF0,0xC0,0x7F,0xF1,0x80,0x60,0x07,0x00,
      0x6F,0xFE,0x00,0x6F,0xFC,0x00,0x60,0x02,
      0x00,0x7F,0xF3,0x00,0x00,0x01,0x80,0x1F,
      0xF8,0x00,0x1F,0xF8,0x00,0x00,0x00,0xC0,
      0x7F,0xFF,0xC0,0x7F,0xFF,0x80,0x00,0x00,
      0x00,0x00,0x00,0x00,


"呕", 0x00,0x00,0x00,0x3F,0xFF,0x80,0x3F,0xFF, //结束字符
      0x80,0x30,0x0C,0x00,0x30,0x0C,0x00,0x3F,
      0xFE,0x00,0x3F,0xFE,0x00,0x00,0x00,0x00,
      0x7F,0xFF,0xC0,0x7F,0xFF,0xC0,0x60,0x00,
      0xC0,0x66,0x0C,0xC0,0x67,0x9C,0xC0,0x61,
      0xB8,0xC0,0x60,0xF0,0xC0,0x6F,0xF8,0xC0,
      0x6F,0x1C,0xC0,0x60,0x0C,0xC0,0x60,0x00,
      0xC0,0x00,0x00,0x00,


};

// 汉字字模表      
const typFNT_GB16 JPN_16[] =          // 数据表60
{
"と", 0x0C,0x00,0x3C,0xC0,0xF7,0xFC,0xF7,0xFC,
      0x36,0xCC,0x36,0xD8,0x00,0x00,0x7F,0xFE,
      0x7F,0xFE,0x6C,0x60,0x63,0xC0,0x67,0xC0,
      0x6C,0x66,0x7F,0xFE,0x7F,0xFC,0x00,0x00,

"う", 0x10,0x04,0x37,0xFE,0x77,0xF8,0xE6,0xA0,
      0xF6,0xA6,0x77,0xFE,0x67,0xFC,0x70,0x06,
      0x3B,0x0E,0xF7,0xFC,0xE7,0xF0,0x73,0x06,
      0x7B,0xFE,0x6B,0xFC,0x60,0x00,0x00,0x00,

	  
"呕", 0x00,0x06,0x00,0x06,0x00,0x06,0x0F,0xFE,	//结束字符
      0x0F,0xFE,0x00,0x06,0x00,0x06,0xFF,0xFE,
      0xFF,0xFE,0x06,0x06,0x06,0x06,0x06,0x06,
      0x06,0x06,0x06,0x06,0x00,0x06,0x00,0x00,
};

