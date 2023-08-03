/******************************************************************************
创建者：康泉
创建日期：2018.03.01
功能描述：
    1. 参数读写相关函数
    2. 全局参数按照不同分类，建议采用结构体形式存储，同时预留20%--30%字节
    
更新记录：
    1. 2018.03.01 初次创建
*******************************************************************************/
#include "OpTable.h"
#include "OpFile.h"
#include "Config_ui.h"


const unsigned int Houdu_Table[200] = {//厚度表
	1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,
};
const unsigned int Adc_TableSmall_LWG[200] = {//信号量表，螺纹钢
	12345,
};
const unsigned int Adc_TableSmall_YG[200] = {//信号量表，圆钢
	12345,
};
const unsigned int Adc_TableBig_LWG[200] = {//信号量表，螺纹钢
	12345,
};
const unsigned int Adc_TableBig_YG[200] = {//信号量表，圆钢
	12345,
};
const unsigned int Rebar_Table_Diam[16] = {6,8,10,12,14,16,18,20,22,25,28,32,40,50,};
//*****************************参数设置列表*******************************
PARA_LIST_STRUCT RebarTypeList[2] = {
	{0,"螺纹"},//统一
	{1,"圆钢"} //统一
};

PARA_LIST_STRUCT GoujianTypeList[2] = {
	{0,"梁"},
	{1,"板"}
};

PARA_LIST_STRUCT SaveTypeList[2] = {
	{0,"自动"},
	{1,"手动"}
};

PARA_LIST_STRUCT ScreenWideList[2] = {
	{0,"800"},
	{1,"1200"}
};

PARA_LIST_STRUCT BKLedLightList[3] = {
	{0,"1"},
	{1,"2"},
	{2,"3"}
};

PARA_LIST_STRUCT LanguageList[2] = {
	{0,"中文"},
	{1,"英文"}
};

PARA_LIST_STRUCT SwitchList[2] = {
	{0,"关闭"},
	{1,"打开"}
};

PARA_LIST_STRUCT BlockAreaList[2] = {
	{0,"4x4"},
	{1,"8x8"}
};

PARA_LIST_STRUCT ThemeList[2] = {
	{0,"深色"},
	{1,"浅色"}
};

//构件名
PARA_LIST_STRUCT GoujianNameList[1];
//仪器编号
PARA_LIST_STRUCT YiqiNumList[1];


//{参数分组，参数ID，中文参数名称，英文参数名称，参数显示类型，参数单位，列表参数指针，列表长度，参数最小值，参数最大值，参数默认值，参数指针}
SYS_PARA_TABLE_STRUCT ParaTable[PARATABLELEN] = {
	//快速检测
    {PARA_GROUP_SAMPLE,  0,	"构件编号", "ProName",  5, TYP_VALUE_ASIC,    NULL,  GoujianNameList, 1,    0,  20,  0,   NULL },  //&g_SysPara.iSavedFileNumMax      
    {PARA_GROUP_SAMPLE,  1, "主筋直径", "M.Diam.",  5, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iZhujinDiam  },
    {PARA_GROUP_SAMPLE,  2,	"主筋间距", "M.Dist.",  5, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 100,  0,  &g_SysPara.g_SamplePara.iZhujinJianju},
    {PARA_GROUP_SAMPLE,  3,	"设计厚度", "DesVal.",  5, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 200,  0,  &g_SysPara.g_SamplePara.iDesignHoudu },   
    {PARA_GROUP_SAMPLE,  4,	"钢筋类型", "R.Type ",  5, TYP_LIST_HANZI,    NULL,  RebarTypeList,   2,    0,   2,  0,  &g_SysPara.g_SamplePara.iRebarType   },
	//高级参数
	{PARA_GROUP_SAMPLE,  5,	"箍筋直径", "S.Diam.",  8, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iGujinDiam   },
    {PARA_GROUP_SAMPLE,  6,	"箍筋间距", "S.Dist.",  8, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 100,  0,  &g_SysPara.g_SamplePara.iGujinJianju },
	{PARA_GROUP_SAMPLE,  7,	"存储方式", "ProName",  8, TYP_LIST_HANZI,    NULL,  SaveTypeList,    2,    0,   1,  0,  &g_SysPara.g_SamplePara.iSaveType    },
    {PARA_GROUP_SAMPLE,  8,	"构件类型", "H.Diam.",  8, TYP_LIST_HANZI,    NULL,  GoujianTypeList, 2,    0,   2,  0,  &g_SysPara.g_SamplePara.iGoujianType },
    {PARA_GROUP_SAMPLE,  9, "覆盖厚度", "H.Dist.",  8, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 200,  0,  &g_SysPara.g_SamplePara.iFugaiHoudu  },
    {PARA_GROUP_SAMPLE, 10,	"凹面直径", "V.Diam.",  8, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iAomianDiam  },
    {PARA_GROUP_SAMPLE, 11,	"凸面直径", "V.Dist.",  8, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iTumianDiam  },
    {PARA_GROUP_SAMPLE, 12,	"单屏宽度", "DesVal.",  8, TYP_LIST_ASIC,     NULL,  ScreenWideList,  2,    0,   1,  0,  &g_SysPara.g_SamplePara.iScreenWide  },
    //JGJ                                                                        
	{PARA_GROUP_SAMPLE, 13,	"构件编号", "ProName",  9, TYP_VALUE_ASIC,    NULL,  GoujianNameList, 1,    0,  20,  0,  NULL },        
    {PARA_GROUP_SAMPLE, 14, "主筋直径", "M.Diam.",  9, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iZhujinDiam  },
    {PARA_GROUP_SAMPLE, 15,	"主筋间距", "M.Dist.",  9, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 100,  0,  &g_SysPara.g_SamplePara.iZhujinJianju},
	{PARA_GROUP_SAMPLE, 16,	"箍筋直径", "S.Diam.",  9, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iGujinDiam   },
    {PARA_GROUP_SAMPLE, 17,	"箍筋间距", "S.Dist.",  9, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 100,  0,  &g_SysPara.g_SamplePara.iGujinJianju },
    {PARA_GROUP_SAMPLE, 18,	"设计厚度", "DesVal.",  9, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 200,  0,  &g_SysPara.g_SamplePara.iDesignHoudu },
	{PARA_GROUP_SAMPLE, 19,	"构件类型", "R.Type ",  9, TYP_LIST_HANZI,    NULL,  GoujianTypeList, 2,    0,   2,  0,  &g_SysPara.g_SamplePara.iGoujianType },	
    {PARA_GROUP_SAMPLE, 20,	"钢筋类型", "R.Type ",  9, TYP_LIST_HANZI,    NULL,  RebarTypeList,   2,    0,   2,  0,  &g_SysPara.g_SamplePara.iRebarType   },    
	{PARA_GROUP_SAMPLE, 21,	"钢筋根数", "R.Type ",  9, TYP_VALUE_DATA,    NULL,  NULL,            0,    1,  12,  6,  &g_SysPara.g_SamplePara.iRebarNumJGJ },	
	//区块参数
	{PARA_GROUP_SAMPLE, 22,	"构件编号", "ProName",  8, TYP_VALUE_ASIC,    NULL,  GoujianNameList, 1,    0,  20,  0,  NULL },        
    {PARA_GROUP_SAMPLE, 23,	"X方向直径", "V.Dist.", 8, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iBlockDiamX  },
    {PARA_GROUP_SAMPLE, 24,	"X方向间距", "DesVal.", 8, TYP_VALUE_DATA,    "mm",  NULL,            2,    0, 100,  0,  &g_SysPara.g_SamplePara.iBlockJianjuX  },
	{PARA_GROUP_SAMPLE, 25,	"Y方向直径", "V.Diam.", 8, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iBlockDiamY  },
    {PARA_GROUP_SAMPLE, 26,	"Y方向间距", "V.Dist.", 8, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 100,  0,  &g_SysPara.g_SamplePara.iBlockJianjuY  },
    {PARA_GROUP_SAMPLE, 27,	"设计厚度", "V.Diam.",  8, TYP_VALUE_DATA,    "mm",  NULL,            0,    6, 100, 16,  &g_SysPara.g_SamplePara.iDesignHoudu  },
	{PARA_GROUP_SAMPLE, 28,	"钢筋类型", "DesVal.",  8, TYP_LIST_HANZI,    NULL,  RebarTypeList,   2,    0,   1,  0,  &g_SysPara.g_SamplePara.iBlockRebarType  },
	{PARA_GROUP_SAMPLE, 29,	"扫描面积", "V.Diam.",  8, TYP_LIST_ASIC,     NULL,  BlockAreaList,   2,    0,   1,  0,  &g_SysPara.g_SamplePara.iBlockAreaNum  },
	//仪器检定
    {PARA_GROUP_SAMPLE, 30,	"构件编号", "ProName",  3, TYP_VALUE_ASIC,    NULL,  GoujianNameList, 1,    0,  20,  0,  NULL },        
    {PARA_GROUP_SAMPLE, 31, "主筋直径", "M.Diam.",  3, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iZhujinDiam  },
    {PARA_GROUP_SAMPLE, 32,	"钢筋类型", "R.Type ",  3, TYP_LIST_HANZI,    NULL,  RebarTypeList,   2,    0,   2,  0,  &g_SysPara.g_SamplePara.iRebarType   },
	//系统参数                                                                   
	{PARA_GROUP_SAMPLE, 33,	"蓝牙上传", "ProName", 11, TYP_LIST_HANZI,    NULL,  SwitchList,      2,    0,  20,  0,  &g_SysPara.iBlueToothSta   },        
    {PARA_GROUP_SAMPLE, 34, "触摸音效", "M.Diam.", 11, TYP_LIST_HANZI,    NULL,  SwitchList,      2,    6,  50,  1,  &g_SysPara.iTouchSoundSta  },
    {PARA_GROUP_SAMPLE, 35,	"水平激光", "S.Dist.", 11, TYP_LIST_HANZI,    NULL,  SwitchList,      2,    0,   1,  0,  &g_SysPara.iLaserRowSta   },
	{PARA_GROUP_SAMPLE, 36,	"垂直激光", "S.Diam.", 11, TYP_LIST_HANZI,    NULL,  SwitchList,      2,    0,   1,  0,  &g_SysPara.iLaserColSta   },
    {PARA_GROUP_SAMPLE, 37,	"背光亮度", "M.Dist.", 11, TYP_LIST_ASIC,     NULL,  BKLedLightList,  3,    0, 100,  2,  &g_SysPara.iBKLedLight     },
    {PARA_GROUP_SAMPLE, 38,	"语言选择", "R.Type ", 11, TYP_LIST_HANZI,    NULL,  LanguageList,    2,    0,   2,  0,  &g_SysPara.iLanguage       },
	{PARA_GROUP_SAMPLE, 39,	"系统日期", "ProName", 11, TYP_VALUE_DATE,    NULL,  NULL,            0,    0,   1,  0,  NULL },
    {PARA_GROUP_SAMPLE, 40,	"系统时间", "H.Diam.", 11, TYP_VALUE_TIME,    NULL,  NULL,            0,    0,   2,  0,  NULL },
    {PARA_GROUP_SAMPLE, 41, "操作密码", "H.Dist.", 11, TYP_VALUE_PASSWORD,NULL,  NULL,            0,    0,9999,  0,  NULL },
    {PARA_GROUP_SAMPLE, 42,	"关机时间", "DesVal.", 11, TYP_VALUE_DATA,    "min", NULL,            0,   20, 200, 30,  &g_SysPara.iPowerOffMinCnt },
    {PARA_GROUP_SAMPLE, 43,	"界面外观", "S.Dist.", 11, TYP_LIST_HANZI,     NULL,  ThemeList,       2,    0,   1,  0,  &g_SysPara.iTheme   },
	
	{PARA_GROUP_SAMPLE, 44,	"仪器编号", "ProName",  1, TYP_VALUE_ASIC,    NULL,  YiqiNumList,     1,    0,  11,  0,  NULL },
};


//************************************************************************
//参数默认值
void ParaTableDefault(SYS_PARA_TABLE_STRUCT* tableAddr)
{
	u8 i;
	for(i=0;i<tableAddr[0].ucTableLen;i++){
		if(ParaTable[i].ParaAddr != NULL)
			*ParaTable[i].ParaAddr = ParaTable[i].iParaDftVal;
	}
	SaveSysPara();
}
//参数默认值
void ParaTableDefaultAll(void)
{
	u8 i;
	for(i=0;i<PARATABLELEN;i++){
		if(ParaTable[i].ParaAddr != NULL)
			*ParaTable[i].ParaAddr = ParaTable[i].iParaDftVal;
	}
	SaveSysPara();
}

