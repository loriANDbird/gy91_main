/******************************************************************************
�����ߣ���Ȫ
�������ڣ�2018.03.01
����������
    1. ������д��غ���
    2. ȫ�ֲ������ղ�ͬ���࣬������ýṹ����ʽ�洢��ͬʱԤ��20%--30%�ֽ�
    
���¼�¼��
    1. 2018.03.01 ���δ���
*******************************************************************************/
#include "OpTable.h"
#include "OpFile.h"
#include "Config_ui.h"


const unsigned int Houdu_Table[200] = {//��ȱ�
	1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,
};
const unsigned int Adc_TableSmall_LWG[200] = {//�ź��������Ƹ�
	12345,
};
const unsigned int Adc_TableSmall_YG[200] = {//�ź�����Բ��
	12345,
};
const unsigned int Adc_TableBig_LWG[200] = {//�ź��������Ƹ�
	12345,
};
const unsigned int Adc_TableBig_YG[200] = {//�ź�����Բ��
	12345,
};
const unsigned int Rebar_Table_Diam[16] = {6,8,10,12,14,16,18,20,22,25,28,32,40,50,};
//*****************************���������б�*******************************
PARA_LIST_STRUCT RebarTypeList[2] = {
	{0,"����"},//ͳһ
	{1,"Բ��"} //ͳһ
};

PARA_LIST_STRUCT GoujianTypeList[2] = {
	{0,"��"},
	{1,"��"}
};

PARA_LIST_STRUCT SaveTypeList[2] = {
	{0,"�Զ�"},
	{1,"�ֶ�"}
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
	{0,"����"},
	{1,"Ӣ��"}
};

PARA_LIST_STRUCT SwitchList[2] = {
	{0,"�ر�"},
	{1,"��"}
};

PARA_LIST_STRUCT BlockAreaList[2] = {
	{0,"4x4"},
	{1,"8x8"}
};

PARA_LIST_STRUCT ThemeList[2] = {
	{0,"��ɫ"},
	{1,"ǳɫ"}
};

//������
PARA_LIST_STRUCT GoujianNameList[1];
//�������
PARA_LIST_STRUCT YiqiNumList[1];


//{�������飬����ID�����Ĳ������ƣ�Ӣ�Ĳ������ƣ�������ʾ���ͣ�������λ���б����ָ�룬�б��ȣ�������Сֵ���������ֵ������Ĭ��ֵ������ָ��}
SYS_PARA_TABLE_STRUCT ParaTable[PARATABLELEN] = {
	//���ټ��
    {PARA_GROUP_SAMPLE,  0,	"�������", "ProName",  5, TYP_VALUE_ASIC,    NULL,  GoujianNameList, 1,    0,  20,  0,   NULL },  //&g_SysPara.iSavedFileNumMax      
    {PARA_GROUP_SAMPLE,  1, "����ֱ��", "M.Diam.",  5, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iZhujinDiam  },
    {PARA_GROUP_SAMPLE,  2,	"������", "M.Dist.",  5, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 100,  0,  &g_SysPara.g_SamplePara.iZhujinJianju},
    {PARA_GROUP_SAMPLE,  3,	"��ƺ��", "DesVal.",  5, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 200,  0,  &g_SysPara.g_SamplePara.iDesignHoudu },   
    {PARA_GROUP_SAMPLE,  4,	"�ֽ�����", "R.Type ",  5, TYP_LIST_HANZI,    NULL,  RebarTypeList,   2,    0,   2,  0,  &g_SysPara.g_SamplePara.iRebarType   },
	//�߼�����
	{PARA_GROUP_SAMPLE,  5,	"����ֱ��", "S.Diam.",  8, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iGujinDiam   },
    {PARA_GROUP_SAMPLE,  6,	"������", "S.Dist.",  8, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 100,  0,  &g_SysPara.g_SamplePara.iGujinJianju },
	{PARA_GROUP_SAMPLE,  7,	"�洢��ʽ", "ProName",  8, TYP_LIST_HANZI,    NULL,  SaveTypeList,    2,    0,   1,  0,  &g_SysPara.g_SamplePara.iSaveType    },
    {PARA_GROUP_SAMPLE,  8,	"��������", "H.Diam.",  8, TYP_LIST_HANZI,    NULL,  GoujianTypeList, 2,    0,   2,  0,  &g_SysPara.g_SamplePara.iGoujianType },
    {PARA_GROUP_SAMPLE,  9, "���Ǻ��", "H.Dist.",  8, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 200,  0,  &g_SysPara.g_SamplePara.iFugaiHoudu  },
    {PARA_GROUP_SAMPLE, 10,	"����ֱ��", "V.Diam.",  8, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iAomianDiam  },
    {PARA_GROUP_SAMPLE, 11,	"͹��ֱ��", "V.Dist.",  8, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iTumianDiam  },
    {PARA_GROUP_SAMPLE, 12,	"�������", "DesVal.",  8, TYP_LIST_ASIC,     NULL,  ScreenWideList,  2,    0,   1,  0,  &g_SysPara.g_SamplePara.iScreenWide  },
    //JGJ                                                                        
	{PARA_GROUP_SAMPLE, 13,	"�������", "ProName",  9, TYP_VALUE_ASIC,    NULL,  GoujianNameList, 1,    0,  20,  0,  NULL },        
    {PARA_GROUP_SAMPLE, 14, "����ֱ��", "M.Diam.",  9, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iZhujinDiam  },
    {PARA_GROUP_SAMPLE, 15,	"������", "M.Dist.",  9, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 100,  0,  &g_SysPara.g_SamplePara.iZhujinJianju},
	{PARA_GROUP_SAMPLE, 16,	"����ֱ��", "S.Diam.",  9, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iGujinDiam   },
    {PARA_GROUP_SAMPLE, 17,	"������", "S.Dist.",  9, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 100,  0,  &g_SysPara.g_SamplePara.iGujinJianju },
    {PARA_GROUP_SAMPLE, 18,	"��ƺ��", "DesVal.",  9, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 200,  0,  &g_SysPara.g_SamplePara.iDesignHoudu },
	{PARA_GROUP_SAMPLE, 19,	"��������", "R.Type ",  9, TYP_LIST_HANZI,    NULL,  GoujianTypeList, 2,    0,   2,  0,  &g_SysPara.g_SamplePara.iGoujianType },	
    {PARA_GROUP_SAMPLE, 20,	"�ֽ�����", "R.Type ",  9, TYP_LIST_HANZI,    NULL,  RebarTypeList,   2,    0,   2,  0,  &g_SysPara.g_SamplePara.iRebarType   },    
	{PARA_GROUP_SAMPLE, 21,	"�ֽ����", "R.Type ",  9, TYP_VALUE_DATA,    NULL,  NULL,            0,    1,  12,  6,  &g_SysPara.g_SamplePara.iRebarNumJGJ },	
	//�������
	{PARA_GROUP_SAMPLE, 22,	"�������", "ProName",  8, TYP_VALUE_ASIC,    NULL,  GoujianNameList, 1,    0,  20,  0,  NULL },        
    {PARA_GROUP_SAMPLE, 23,	"X����ֱ��", "V.Dist.", 8, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iBlockDiamX  },
    {PARA_GROUP_SAMPLE, 24,	"X������", "DesVal.", 8, TYP_VALUE_DATA,    "mm",  NULL,            2,    0, 100,  0,  &g_SysPara.g_SamplePara.iBlockJianjuX  },
	{PARA_GROUP_SAMPLE, 25,	"Y����ֱ��", "V.Diam.", 8, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iBlockDiamY  },
    {PARA_GROUP_SAMPLE, 26,	"Y������", "V.Dist.", 8, TYP_VALUE_DATA,    "mm",  NULL,            0,    0, 100,  0,  &g_SysPara.g_SamplePara.iBlockJianjuY  },
    {PARA_GROUP_SAMPLE, 27,	"��ƺ��", "V.Diam.",  8, TYP_VALUE_DATA,    "mm",  NULL,            0,    6, 100, 16,  &g_SysPara.g_SamplePara.iDesignHoudu  },
	{PARA_GROUP_SAMPLE, 28,	"�ֽ�����", "DesVal.",  8, TYP_LIST_HANZI,    NULL,  RebarTypeList,   2,    0,   1,  0,  &g_SysPara.g_SamplePara.iBlockRebarType  },
	{PARA_GROUP_SAMPLE, 29,	"ɨ�����", "V.Diam.",  8, TYP_LIST_ASIC,     NULL,  BlockAreaList,   2,    0,   1,  0,  &g_SysPara.g_SamplePara.iBlockAreaNum  },
	//�����춨
    {PARA_GROUP_SAMPLE, 30,	"�������", "ProName",  3, TYP_VALUE_ASIC,    NULL,  GoujianNameList, 1,    0,  20,  0,  NULL },        
    {PARA_GROUP_SAMPLE, 31, "����ֱ��", "M.Diam.",  3, TYP_VALUE_DATA,    "mm",  NULL,            0,    6,  50, 16,  &g_SysPara.g_SamplePara.iZhujinDiam  },
    {PARA_GROUP_SAMPLE, 32,	"�ֽ�����", "R.Type ",  3, TYP_LIST_HANZI,    NULL,  RebarTypeList,   2,    0,   2,  0,  &g_SysPara.g_SamplePara.iRebarType   },
	//ϵͳ����                                                                   
	{PARA_GROUP_SAMPLE, 33,	"�����ϴ�", "ProName", 11, TYP_LIST_HANZI,    NULL,  SwitchList,      2,    0,  20,  0,  &g_SysPara.iBlueToothSta   },        
    {PARA_GROUP_SAMPLE, 34, "������Ч", "M.Diam.", 11, TYP_LIST_HANZI,    NULL,  SwitchList,      2,    6,  50,  1,  &g_SysPara.iTouchSoundSta  },
    {PARA_GROUP_SAMPLE, 35,	"ˮƽ����", "S.Dist.", 11, TYP_LIST_HANZI,    NULL,  SwitchList,      2,    0,   1,  0,  &g_SysPara.iLaserRowSta   },
	{PARA_GROUP_SAMPLE, 36,	"��ֱ����", "S.Diam.", 11, TYP_LIST_HANZI,    NULL,  SwitchList,      2,    0,   1,  0,  &g_SysPara.iLaserColSta   },
    {PARA_GROUP_SAMPLE, 37,	"��������", "M.Dist.", 11, TYP_LIST_ASIC,     NULL,  BKLedLightList,  3,    0, 100,  2,  &g_SysPara.iBKLedLight     },
    {PARA_GROUP_SAMPLE, 38,	"����ѡ��", "R.Type ", 11, TYP_LIST_HANZI,    NULL,  LanguageList,    2,    0,   2,  0,  &g_SysPara.iLanguage       },
	{PARA_GROUP_SAMPLE, 39,	"ϵͳ����", "ProName", 11, TYP_VALUE_DATE,    NULL,  NULL,            0,    0,   1,  0,  NULL },
    {PARA_GROUP_SAMPLE, 40,	"ϵͳʱ��", "H.Diam.", 11, TYP_VALUE_TIME,    NULL,  NULL,            0,    0,   2,  0,  NULL },
    {PARA_GROUP_SAMPLE, 41, "��������", "H.Dist.", 11, TYP_VALUE_PASSWORD,NULL,  NULL,            0,    0,9999,  0,  NULL },
    {PARA_GROUP_SAMPLE, 42,	"�ػ�ʱ��", "DesVal.", 11, TYP_VALUE_DATA,    "min", NULL,            0,   20, 200, 30,  &g_SysPara.iPowerOffMinCnt },
    {PARA_GROUP_SAMPLE, 43,	"�������", "S.Dist.", 11, TYP_LIST_HANZI,     NULL,  ThemeList,       2,    0,   1,  0,  &g_SysPara.iTheme   },
	
	{PARA_GROUP_SAMPLE, 44,	"�������", "ProName",  1, TYP_VALUE_ASIC,    NULL,  YiqiNumList,     1,    0,  11,  0,  NULL },
};


//************************************************************************
//����Ĭ��ֵ
void ParaTableDefault(SYS_PARA_TABLE_STRUCT* tableAddr)
{
	u8 i;
	for(i=0;i<tableAddr[0].ucTableLen;i++){
		if(ParaTable[i].ParaAddr != NULL)
			*ParaTable[i].ParaAddr = ParaTable[i].iParaDftVal;
	}
	SaveSysPara();
}
//����Ĭ��ֵ
void ParaTableDefaultAll(void)
{
	u8 i;
	for(i=0;i<PARATABLELEN;i++){
		if(ParaTable[i].ParaAddr != NULL)
			*ParaTable[i].ParaAddr = ParaTable[i].iParaDftVal;
	}
	SaveSysPara();
}

