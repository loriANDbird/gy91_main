/*******************************************************************************
创建者：
创建日期：2018.03.01
功能描述：
    1. ui_Main 界面初始化，
    2. ui_Main_Run 函数实现

    
更新记录：
    1. 2018.03.01 初次创建
    
*******************************************************************************/
#include "ui_SampleData.h"
#include "Config_ui.h"
#include "OpData.h"
#include "AimFunc.h"

#define BTN_BIAODING	0
#define BTN_MODIFY	    1
#define BTN_ADD_1  	    2
#define BTN_SUB_1	    3
#define BTN_ADD_10      4
#define BTN_SUB_10	    5

u8 m_bAutoJump = 1;
u8 m_bAutoClear = 1;//只开机时执行

int SampleDataCnt = 0;
int g_iSaveFileNum;

int g_iCh1AdcValZero;
int g_iCh2AdcValZero;
int g_iCh3AdcValZero;
int g_iCh4AdcValZero;
int g_iCh5AdcValZero;
int g_iCh6AdcValZero; 

unsigned int g_iCarCountZero = 0;

extern unsigned int g_iCarCount;

typedef struct {
	int Cnt;
    int Diam1;
    int Distance1;
    int Cover[10];
    int Diam2;
    int Distance2;
    
    int State;//0:显示  1:修改
    int HLNum;
}SourceDataParaType;

typedef struct {
    int Num;
	int X;
    int Y;
    int *ParaVal;
}ModifyParaTableType;

SourceDataParaType g_sourceDatapara;

ModifyParaTableType ParaTab[9]={
    {0,  10*1, 295, &g_sourceDatapara.Cnt},//钢筋根数
    {1,  10*5, 295, &g_sourceDatapara.Diam1},//主筋直径
    {2,  10*7, 295, &g_sourceDatapara.Distance1},//主筋间距
    {3, 10*11, 295, &g_sourceDatapara.Diam2},//箍筋直径
    {4, 10*13, 295, &g_sourceDatapara.Distance2},//箍筋间距
    {5, 10*17, 295, &g_sourceDatapara.Cover[0]},//厚度1
    {6, 10*20, 295, &g_sourceDatapara.Cover[1]},//厚度2
    {7, 10*23, 295, &g_sourceDatapara.Cover[2]},//厚度3
    {8, 10*26, 295, &g_sourceDatapara.Cover[3]},//厚度4
};

//原始数据
extern SOURCE_DATA SourceData;

extern AIMREBAR_BUF_STR AimRebarBuf_CH1;//钢筋定位缓存
//-------------------------------------------------------------------------------
//初始化原始数据存储参数
void InitSourceDataPara(void){
    g_sourceDatapara.Cnt = 1;
    g_sourceDatapara.Diam1 = 16;
    g_sourceDatapara.Distance1 = 0;
    g_sourceDatapara.Cover[0] = 30;
    g_sourceDatapara.Cover[1] = 0;
    g_sourceDatapara.Cover[2] = 0;
    g_sourceDatapara.Cover[3] = 0;
    g_sourceDatapara.Diam2 = 0;
    g_sourceDatapara.Distance2 = 0;
    
    g_sourceDatapara.HLNum = 0;
    g_sourceDatapara.State = 0;
}

//显示原始数据存储参数
void DispSourceDataPara(void)
{
    int X = 0, Y = 295;
    
    //显示原始数据存储参数
    if(g_sourceDatapara.Cnt == 1){
        sprintf(g_cStrBuf,"L%02d_D%02d%02d_D%02d%02d_C%02d.csv            ",g_sourceDatapara.Cnt, g_sourceDatapara.Diam1, g_sourceDatapara.Distance1,
            g_sourceDatapara.Diam2, g_sourceDatapara.Distance2, g_sourceDatapara.Cover[0]);
    }
    else if(g_sourceDatapara.Cnt == 2){
        sprintf(g_cStrBuf,"L%02d_D%02d%02d_D%02d%02d_C%02dC%02d.csv            ",g_sourceDatapara.Cnt, g_sourceDatapara.Diam1, g_sourceDatapara.Distance1,
            g_sourceDatapara.Diam2, g_sourceDatapara.Distance2, g_sourceDatapara.Cover[0], g_sourceDatapara.Cover[1]);
    }
    else if(g_sourceDatapara.Cnt == 3){
        sprintf(g_cStrBuf,"L%02d_D%02d%02d_D%02d%02d_C%02dC%02dC%02d.csv         ",g_sourceDatapara.Cnt, g_sourceDatapara.Diam1, g_sourceDatapara.Distance1,
            g_sourceDatapara.Diam2, g_sourceDatapara.Distance2, g_sourceDatapara.Cover[0], g_sourceDatapara.Cover[1], g_sourceDatapara.Cover[2]);
    }
    else if(g_sourceDatapara.Cnt == 4){
        sprintf(g_cStrBuf,"L%02d_D%02d%02d_D%02d%02d_C%02dC%02dC%02dC%02d.csv",g_sourceDatapara.Cnt, g_sourceDatapara.Diam1, g_sourceDatapara.Distance1,
            g_sourceDatapara.Diam2, g_sourceDatapara.Distance2, g_sourceDatapara.Cover[0], g_sourceDatapara.Cover[1],g_sourceDatapara.Cover[2], g_sourceDatapara.Cover[3]);
    }

    LCD_SetColor(WHITE,BLACK);
    LCD_ShowString(ASC20x10, X, Y, 0, g_cStrBuf);
    
    if(g_sourceDatapara.State == 1){
        LCD_SetColor(WHITE,RED);
        sprintf(g_cStrBuf,"%02d",*ParaTab[g_sourceDatapara.HLNum].ParaVal);
        LCD_ShowString(ASC20x10, ParaTab[g_sourceDatapara.HLNum].X, ParaTab[g_sourceDatapara.HLNum].Y, 0, g_cStrBuf);
        LCD_FillRect(ParaTab[g_sourceDatapara.HLNum].X, 317, 20, 2, RED);
    }
    else{
        LCD_FillRect(ParaTab[g_sourceDatapara.HLNum].X, 317, 20, 2, WHITE);
    }
}


//存储测试数据
int SaveSourceDataFile(void)
{
    int i;
    u8 result;
//////    u8 uPerCnt;
//////    char filename[50];
    char filesNum = 0;
    
//////    u8 uPer[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

//    for(i=1;i<100;i++){
//        sprintf(g_filname,"0:/HD/test%02d.txt",i);
//        result = f_open(file,g_filname, FA_READ); //判断文件是否存在
//        if (result == FR_OK){//写入数据信息
//            f_close(file);
//        }
//        else{
//            break;
//        }
//    }
//    sprintf(filename,"0:/HD/TestA%02d%02d%02d%02d-%02d.txt",Rx8010sjTime.year,Rx8010sjTime.month,
//        Rx8010sjTime.day,Rx8010sjTime.hour,g_iSaveFileNum);
//    sprintf(filename,"0:/HD/%d.txt",g_iSaveFileNum);
        
    //显示原始数据存储参数
    sprintf(g_cStrBuf,"0:/%02d%02d",g_SysTime.month, g_SysTime.day);
    f_mkdir(g_cStrBuf);
        
    if(g_sourceDatapara.Cnt == 1){
        sprintf(g_cStrBuf,"0:/%02d%02d/L%02d_D%02d%02d_D%02d%02d_C%02d.csv",g_SysTime.month, g_SysTime.day,g_sourceDatapara.Cnt, g_sourceDatapara.Diam1,
            g_sourceDatapara.Distance1,g_sourceDatapara.Diam2, g_sourceDatapara.Distance2, g_sourceDatapara.Cover[0]);
    }
    else if(g_sourceDatapara.Cnt == 2){
        sprintf(g_cStrBuf,"0:/%02d%02d/L%02d_D%02d%02d_D%02d%02d_C%02dC%02d.csv",g_SysTime.month, g_SysTime.day,g_sourceDatapara.Cnt,g_sourceDatapara.Diam1,
            g_sourceDatapara.Distance1,g_sourceDatapara.Diam2, g_sourceDatapara.Distance2, g_sourceDatapara.Cover[0], g_sourceDatapara.Cover[1]);
    }
    else if(g_sourceDatapara.Cnt == 3){
        sprintf(g_cStrBuf,"0:/%02d%02d/L%02d_D%02d%02d_D%02d%02d_C%02dC%02dC%02d.csv",g_SysTime.month, g_SysTime.day,g_sourceDatapara.Cnt, g_sourceDatapara.Diam1, 
            g_sourceDatapara.Distance1, g_sourceDatapara.Diam2, g_sourceDatapara.Distance2, g_sourceDatapara.Cover[0], g_sourceDatapara.Cover[1], g_sourceDatapara.Cover[2]);
    }
    else if(g_sourceDatapara.Cnt == 4){
        sprintf(g_cStrBuf,"0:/%02d%02d/L%02d_D%02d%02d_D%02d%02d_C%02dC%02dC%02dC%02d.csv",g_SysTime.month, g_SysTime.day,g_sourceDatapara.Cnt, g_sourceDatapara.Diam1, g_sourceDatapara.Distance1,
            g_sourceDatapara.Diam2, g_sourceDatapara.Distance2, g_sourceDatapara.Cover[0], g_sourceDatapara.Cover[1],g_sourceDatapara.Cover[2], g_sourceDatapara.Cover[3]);
    }
    
    result = f_open(file, g_cStrBuf, FA_OPEN_EXISTING);//文件存在，修改文件名
    if (result == FR_OK){//写入数据信息
        f_close(file);
        do{
            filesNum ++;
            if(g_sourceDatapara.Cnt == 1){
                sprintf(g_cStrBuf,"0:/%02d%02d/L%02d_D%02d%02d_D%02d%02d_C%02d_%02d.csv",g_SysTime.month, g_SysTime.day,g_sourceDatapara.Cnt, g_sourceDatapara.Diam1,
                    g_sourceDatapara.Distance1,g_sourceDatapara.Diam2, g_sourceDatapara.Distance2, g_sourceDatapara.Cover[0], filesNum);
            }
            else if(g_sourceDatapara.Cnt == 2){
                sprintf(g_cStrBuf,"0:/%02d%02d/L%02d_D%02d%02d_D%02d%02d_C%02dC%02d_%02d.csv",g_SysTime.month, g_SysTime.day,g_sourceDatapara.Cnt,g_sourceDatapara.Diam1,
                    g_sourceDatapara.Distance1,g_sourceDatapara.Diam2, g_sourceDatapara.Distance2, g_sourceDatapara.Cover[0], g_sourceDatapara.Cover[1], filesNum);
            }
            else if(g_sourceDatapara.Cnt == 3){
                sprintf(g_cStrBuf,"0:/%02d%02d/L%02d_D%02d%02d_D%02d%02d_C%02dC%02dC%02d_%02d.csv",g_SysTime.month, g_SysTime.day,g_sourceDatapara.Cnt, g_sourceDatapara.Diam1, 
                    g_sourceDatapara.Distance1, g_sourceDatapara.Diam2, g_sourceDatapara.Distance2, g_sourceDatapara.Cover[0], g_sourceDatapara.Cover[1], g_sourceDatapara.Cover[2], filesNum);
            }
            else if(g_sourceDatapara.Cnt == 4){
                sprintf(g_cStrBuf,"0:/%02d%02d/L%02d_D%02d%02d_D%02d%02d_C%02dC%02dC%02dC%02d_%02d.csv",g_SysTime.month, g_SysTime.day,g_sourceDatapara.Cnt, g_sourceDatapara.Diam1, g_sourceDatapara.Distance1,
                    g_sourceDatapara.Diam2, g_sourceDatapara.Distance2, g_sourceDatapara.Cover[0], g_sourceDatapara.Cover[1],g_sourceDatapara.Cover[2], g_sourceDatapara.Cover[3], filesNum);
            }
            result = f_open(file, g_cStrBuf, FA_OPEN_EXISTING);//打开已经存在的文档，判断文件是否存在
            if(result == FR_OK){
                f_close(file);
            }
            else{
                break;
            }
        }
        while(1);
    }
    
    result = f_open(file, g_cStrBuf, FA_WRITE | FA_CREATE_ALWAYS);//存在的覆盖之前文件
    if (result == FR_OK){//写入数据信息
//////		if(g_iCarDist>20){
//////			uPerCnt=g_iCarDist/20;
//////		}
//////		else{
//////			uPerCnt = 0;
//////		}
        
//////        sprintf(g_cStrBuf,"CH1,CH2,CH3,CH4,CH5,CH6,SUB1,SUB2,SUB3,SUBMARK1,SUBMARK2,SUBMARK3,DIF1,DIF2,DIF3\r\n");
//////        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf),&bw);
//////        
//////        for(i=1;i<g_iCarDist;i++){
//////            sprintf(g_cStrBuf,"%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d\r\n",
////////            SourceData.iCh1Sum[i],SourceData.iCh1Sub[i],SourceData.iCh2Sum[i],SourceData.iCh2Sub[i],SourceData.iCh3Sum[i],SourceData.iCh3Sub[i]);
//////                SourceData.iCH1[i],SourceData.iCH2[i],SourceData.iCH3[i],SourceData.iCH4[i],SourceData.iCH5[i],SourceData.iCH6[i]
//////                ,SourceData.iCh1Sub[i],SourceData.iCh2Sub[i],SourceData.iCh3Sub[i]
//////                ,SourceData.iCh1SubMark[i],SourceData.iCh2SubMark[i],SourceData.iCh3SubMark[i]
//////                ,SourceData.iCh1DifMark[i],SourceData.iCh2DifMark[i],SourceData.iCh3DifMark[i]);
//////            result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
//////            if(result != FR_OK){
//////                break;
//////            }
//////        }
        
//0311注释掉        
//////        sprintf(g_cStrBuf,"CH1,CH2,CH3,CH4,CH5,CH6,SUMMARK1,SUMMARK2,SUMMARK3,SUMMARK12,SUMMARK22,SUMMARK32,SUBMARK1,SUBMARK2,SUBMARK3,DIFMARK1,DIFMARK2,DIFMARK3\r\n");
//////        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf),&bw);
//////        
//////        for(i=1;i<g_iCarDist;i++){
//////            sprintf(g_cStrBuf,"%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d\r\n",
//////                SourceData.iCH1[i],SourceData.iCH2[i],SourceData.iCH3[i]
//////                ,SourceData.iCH4[i],SourceData.iCH5[i],SourceData.iCH6[i]
//////                ,SourceData.iCh1SumMark[i],SourceData.iCh2SumMark[i],SourceData.iCh3SumMark[i]
//////                ,SourceData.iCh1SumMark2[i],SourceData.iCh2SumMark2[i],SourceData.iCh3SumMark2[i]
//////                ,SourceData.iCh1SubMark[i],SourceData.iCh2SubMark[i],SourceData.iCh3SubMark[i]
//////                ,SourceData.iCh1DifMark[i],SourceData.iCh2DifMark[i],SourceData.iCh3DifMark[i]
//////            );
//////            result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
//////            if(result != FR_OK){
//////                break;
//////            }
//////        }
//END 

        sprintf(g_cStrBuf,"Sub1,Sub2,Sub3,Sum1,Sum2,Sum3,SUMMARK1,SUMMARK2,SUMMARK3,SUMMARK12,SUMMARK22,SUMMARK32,SUBMARK1,SUBMARK2,SUBMARK3,DIFMARK1,DIFMARK2,DIFMARK3,DIFMARKCNT1,DIFMARKCNT2,DIFMARKCNT3\r\n");
        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf),&bw);
        
        for(i=1;i<g_iCarDist;i++){
            sprintf(g_cStrBuf,"%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d\r\n",
                SourceData.iCh1Sub[i],SourceData.iCh2Sub[i],SourceData.iCh3Sub[i]
                ,SourceData.iCh1Sum[i],SourceData.iCh2Sum[i],SourceData.iCh3Sum[i]
                ,SourceData.iCh1SumMark[i],SourceData.iCh2SumMark[i],SourceData.iCh3SumMark[i]
                ,SourceData.iCh1SumMark2[i],SourceData.iCh2SumMark2[i],SourceData.iCh3SumMark2[i]
                ,SourceData.iCh1SubMark[i],SourceData.iCh2SubMark[i],SourceData.iCh3SubMark[i]
                ,SourceData.iCh1DifMark[i],SourceData.iCh2DifMark[i],SourceData.iCh3DifMark[i]
                ,SourceData.iCh1DifWaveCnt[i],SourceData.iCh2DifWaveCnt[i],SourceData.iCh3DifWaveCnt[i]
            );
            result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
            if(result != FR_OK){
                break;
            }
        }
        
        f_close(file);
    }
    return result;
}

//存储测试数据
int SaveTableDataFile(void)
{
//    char btDataBuf[16];
    char g_cStrBuf[64];
    char filename[50];
    char dataBuf[10];
    u8 result;
    
    //采集数据表
    sprintf(filename,"0:/HD/Sample1.txt");
    result = f_open(file, filename, FA_WRITE | FA_OPEN_ALWAYS);//测试采集数据文件
    if (result == FR_OK){//写入数据信息
        if(SampleDataCnt%10 == 9){
            sprintf(g_cStrBuf, "%d,//%d-%d\r\n", AimRebarPara_CH1.iSubMaxVal-AimRebarPara_CH1.iSubMinVal, SampleDataCnt/10*10, SampleDataCnt/10*10+9);
        }
        else{
            sprintf(g_cStrBuf,"%d,",AimRebarPara_CH1.iSubMaxVal-AimRebarPara_CH1.iSubMinVal);
        }
        f_lseek(file,file->fptr+file->fsize);
        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
        f_close(file);
    }
    Sleep_ms(200);
    
    sprintf(filename,"0:/HD/Sample2.txt");
    result = f_open(file, filename, FA_WRITE | FA_OPEN_ALWAYS);//测试采集数据文件
    if (result == FR_OK){//写入数据信息
        if(SampleDataCnt%10 == 9){
            sprintf(g_cStrBuf, "%d,//%d-%d\r\n", AimRebarPara_CH2.iSubMaxVal-AimRebarPara_CH2.iSubMinVal, SampleDataCnt/10*10, SampleDataCnt/10*10+9);
        }
        else{
            sprintf(g_cStrBuf,"%d,",AimRebarPara_CH2.iSubMaxVal-AimRebarPara_CH2.iSubMinVal);
        }
        
        f_lseek(file,file->fptr+file->fsize);
        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
        f_close(file);
    }
    Sleep_ms(200);
    
    sprintf(filename,"0:/HD/Sample3.txt");
    result = f_open(file, filename, FA_WRITE | FA_OPEN_ALWAYS);//测试采集数据文件
    if (result == FR_OK){//写入数据信息
        if(SampleDataCnt%10 == 9){
            sprintf(g_cStrBuf, "%d,//%d-%d\r\n", AimRebarPara_CH3.iSubMaxVal-AimRebarPara_CH3.iSubMinVal, SampleDataCnt/10*10, SampleDataCnt/10*10+9);
        }
        else{
            sprintf(g_cStrBuf, "%d,", AimRebarPara_CH3.iSubMaxVal-AimRebarPara_CH3.iSubMinVal);
        }
        f_lseek(file,file->fptr+file->fsize);
        result = f_write(file, g_cStrBuf,strlen(g_cStrBuf), &bw);
        f_close(file);
    }
    
    SampleDataCnt ++;
    LCD_SetColor(WHITE,BLACK);
    sprintf(dataBuf,"Cnt:%-5d",SampleDataCnt);
    LCD_ShowString(ASC16x8,400,300,0,dataBuf);
    
    return 0;
}


//UI初始化函数
void InitTouchButton(void)
{
//    LCD_SetColor(WHITE,BLACK);
//	LCD_ShowChinese(Font32,8,1,0,"迴");//返回图标
    
    LCD_SetColor(LGRAY,BLACK);
    LCD_ShowChinese(Font20,0,5,0,"标定");//返回图标
    
	g_ButtonBuf[BTN_BIAODING].sX = 0;
	g_ButtonBuf[BTN_BIAODING].sY = 0;
	g_ButtonBuf[BTN_BIAODING].eX = 60;
	g_ButtonBuf[BTN_BIAODING].eY = 40;
	g_ButtonBuf[BTN_BIAODING].btSta = 0;
    
    //修改参数图标
    g_ButtonBuf[BTN_MODIFY].sX = 430;
	g_ButtonBuf[BTN_MODIFY].sY = 290;
	g_ButtonBuf[BTN_MODIFY].eX = 479;
	g_ButtonBuf[BTN_MODIFY].eY = 319;
	g_ButtonBuf[BTN_MODIFY].btSta = 0;
    
    if(g_sourceDatapara.State == 1){
        
        LCD_SetColor(LLGRAY,RED);
        LCD_ShowString(ASC20x10,410,295,0,"Modify");//修改参数图标

        LCD_SetColor(WHITE,BLACK);
        LCD_ShowString(ASC20x10,430,110,0,"+1");//加1
        g_ButtonBuf[BTN_ADD_1].sX = 400;
        g_ButtonBuf[BTN_ADD_1].sY = 90;
        g_ButtonBuf[BTN_ADD_1].eX = 479;
        g_ButtonBuf[BTN_ADD_1].eY = 130;
        g_ButtonBuf[BTN_ADD_1].btSta = 0;
        
        LCD_SetColor(WHITE,BLACK);
        LCD_ShowString(ASC20x10,430,170,0,"-1");//减1
        g_ButtonBuf[BTN_SUB_1].sX = 400;
        g_ButtonBuf[BTN_SUB_1].sY = 150;
        g_ButtonBuf[BTN_SUB_1].eX = 479;
        g_ButtonBuf[BTN_SUB_1].eY = 190;
        g_ButtonBuf[BTN_SUB_1].btSta = 0;
        
        LCD_SetColor(WHITE,BLACK);
        LCD_ShowString(ASC20x10,430,50,0,"+10");//加10
        g_ButtonBuf[BTN_ADD_10].sX = 400;
        g_ButtonBuf[BTN_ADD_10].sY = 30;
        g_ButtonBuf[BTN_ADD_10].eX = 479;
        g_ButtonBuf[BTN_ADD_10].eY = 70;
        g_ButtonBuf[BTN_ADD_10].btSta = 0;
        
        LCD_SetColor(WHITE,BLACK);
        LCD_ShowString(ASC20x10,430,230,0,"-10");//减10
        g_ButtonBuf[BTN_SUB_10].sX = 400;
        g_ButtonBuf[BTN_SUB_10].sY = 210;
        g_ButtonBuf[BTN_SUB_10].eX = 479;
        g_ButtonBuf[BTN_SUB_10].eY = 250;
        g_ButtonBuf[BTN_SUB_10].btSta = 0;
    }
    else{
        LCD_SetColor(LLGRAY,BLACK);
        LCD_ShowString(ASC20x10,410,295,0,"Modify");//修改参数图标
        
        LCD_SetColor(WHITE,WHITE);
        LCD_ShowString(ASC20x10,430,110,0,"+1");//加1
        LCD_ShowString(ASC20x10,430,170,0,"-1");//减1
        LCD_ShowString(ASC20x10,430,50,0,"+10");//加10
        LCD_ShowString(ASC20x10,430,230,0,"-10");//减10
    }
}

void Biaoding_Adc_Data(void)
{
	u8 i = 0;
	u16 j = 0;
	uint32_t Ch1Zero1 = 0;
	uint32_t Ch2Zero2 = 0;
	uint32_t Ch3Zero3 = 0;
	uint32_t Ch4Zero4 = 0;
	uint32_t Ch5Zero5 = 0;
	uint32_t Ch6Zero6 = 0;
	
	int carCnt = 0;
	
	LCD_SetColor(WHITE,BLACK);
	LCD_ShowString(ASC24x12,216,0,0,"Wait");
	while(1){
		if(g_ucUart2RecValid == 1){
			g_ucUart2RecValid = 0;
	
			g_iCh1AdcValOrig = g_ucUart2RecBuf[4];
			g_iCh1AdcValOrig = (g_iCh1AdcValOrig << 8);
			g_iCh1AdcValOrig += g_ucUart2RecBuf[5];
			g_iCh1AdcValOrig = (g_iCh1AdcValOrig << 8);
			g_iCh1AdcValOrig += g_ucUart2RecBuf[6];
	
			g_iCh2AdcValOrig = g_ucUart2RecBuf[1];
			g_iCh2AdcValOrig = (g_iCh2AdcValOrig << 8);
			g_iCh2AdcValOrig += g_ucUart2RecBuf[2];
			g_iCh2AdcValOrig = (g_iCh2AdcValOrig << 8);
			g_iCh2AdcValOrig += g_ucUart2RecBuf[3];
			
			g_iCh3AdcValOrig = g_ucUart2RecBuf[10];
			g_iCh3AdcValOrig = (g_iCh3AdcValOrig << 8);
			g_iCh3AdcValOrig += g_ucUart2RecBuf[11];
			g_iCh3AdcValOrig = (g_iCh3AdcValOrig << 8);
			g_iCh3AdcValOrig += g_ucUart2RecBuf[12];
			
			g_iCh4AdcValOrig = g_ucUart2RecBuf[7];
			g_iCh4AdcValOrig = (g_iCh4AdcValOrig << 8);
			g_iCh4AdcValOrig += g_ucUart2RecBuf[8];
			g_iCh4AdcValOrig = (g_iCh4AdcValOrig << 8);
			g_iCh4AdcValOrig += g_ucUart2RecBuf[9];
			
			g_iCh5AdcValOrig = g_ucUart2RecBuf[16];
			g_iCh5AdcValOrig = (g_iCh5AdcValOrig << 8);
			g_iCh5AdcValOrig += g_ucUart2RecBuf[17];
			g_iCh5AdcValOrig = (g_iCh5AdcValOrig << 8);
			g_iCh5AdcValOrig += g_ucUart2RecBuf[18];
			
			g_iCh6AdcValOrig = g_ucUart2RecBuf[13];
			g_iCh6AdcValOrig = (g_iCh6AdcValOrig << 8);
			g_iCh6AdcValOrig += g_ucUart2RecBuf[14];
			g_iCh6AdcValOrig = (g_iCh6AdcValOrig << 8);
			g_iCh6AdcValOrig += g_ucUart2RecBuf[15];
			
			carCnt = g_ucUart2RecBuf[19];
			carCnt = (carCnt << 8);
			carCnt += g_ucUart2RecBuf[20];
		
			Ch1Zero1 += g_iCh1AdcValOrig;
			Ch2Zero2 += g_iCh2AdcValOrig;
			Ch3Zero3 += g_iCh3AdcValOrig;
			Ch4Zero4 += g_iCh4AdcValOrig;
			Ch5Zero5 += g_iCh5AdcValOrig;
			Ch6Zero6 += g_iCh6AdcValOrig;
			i++;
		}
		j++;
		if(j>500)break;
		if(i >= 100)break;
		Sleep_ms(10);
	}
	g_iCh1AdcValZero = Ch1Zero1/100;
	g_iCh2AdcValZero = Ch2Zero2/100;
	g_iCh3AdcValZero = Ch3Zero3/100;
	g_iCh4AdcValZero = Ch4Zero4/100;
	g_iCh5AdcValZero = Ch5Zero5/100;
	g_iCh6AdcValZero = Ch6Zero6/100;
    
    g_SysPara.uiCh1Zero = g_iCh1AdcValZero;
    g_SysPara.uiCh2Zero = g_iCh2AdcValZero;
    g_SysPara.uiCh3Zero = g_iCh3AdcValZero;
    g_SysPara.uiCh4Zero = g_iCh4AdcValZero;
    g_SysPara.uiCh5Zero = g_iCh5AdcValZero;
    g_SysPara.uiCh6Zero = g_iCh6AdcValZero;
    
    SaveSysPara();
	
	g_iCarCountZero = carCnt;
	
	g_iCh1AdcVal = g_iCh1AdcValOrig - g_iCh1AdcValZero;
	g_iCh2AdcVal = g_iCh2AdcValOrig - g_iCh2AdcValZero;
	g_iCh3AdcVal = g_iCh3AdcValOrig - g_iCh3AdcValZero;
	g_iCh4AdcVal = g_iCh4AdcValOrig - g_iCh4AdcValZero;
	g_iCh5AdcVal = g_iCh5AdcValOrig - g_iCh5AdcValZero;
	g_iCh6AdcVal = g_iCh6AdcValOrig - g_iCh6AdcValZero;
	
	g_iCh1AdcValMax = g_iCh1AdcVal;
	g_iCh2AdcValMax = g_iCh2AdcVal;
	g_iCh3AdcValMax = g_iCh3AdcVal;
	g_iCh4AdcValMax = g_iCh4AdcVal;
	g_iCh5AdcValMax = g_iCh5AdcVal;
	g_iCh6AdcValMax = g_iCh6AdcVal;
	
	g_iCh1AdcValMin = g_iCh1AdcVal;
	g_iCh2AdcValMin = g_iCh2AdcVal;
    g_iCh3AdcValMin = g_iCh3AdcVal;
    g_iCh4AdcValMin = g_iCh4AdcVal;
    g_iCh5AdcValMin = g_iCh5AdcVal;
    g_iCh6AdcValMin = g_iCh6AdcVal;
    
//    g_iCh1SumBuf = 0;
//    g_iCh2SumBuf = 0;
//    g_iCh3SumBuf = 0;
//	
	LCD_SetColor(WHITE,BLACK);
	LCD_ShowString(ASC24x12,216,0,0,"    ");
}


//UI初始化函数
void ui_SampleData_InitDisp(void)
{
    char dateBuf[12];
//	LCD_FillRect(0,0,240,26,GOLD);
//	LCD_SetColor(WHITE,BLACK);
//	LCD_ShowString(ASC24x12,90,296,0,"Page1");
	
	LCD_FillRect(0,0,480,320,WHITE);
	
//	ui_DispTime(1);
	ui_DispBattery(1);//显示电量
	
    //初始化虚拟按键
    InitTouchButton();
	
//	LCD_FillRect(100,100,100,100,0x7bff);
//	LCD_FillRect(200,100,100,100,0x41ff);
//	LCD_FillRect(300,100,100,100,0x211f);

    //检测参数赋值
    g_tWorkPara.cZhujinDiam = 16;		//主筋直径
    g_tWorkPara.cZhujinDistance = 0;	//主筋间距
    g_tWorkPara.cGujinDiam = 0;		    //箍筋直径
    g_tWorkPara.cGujinDistance = 0;	    //箍筋间距

    g_tWorkPara.cGangJinType = 0;		//钢筋类型
    g_tWorkPara.cLingChengType = 1;	    //大小量程

    g_tWorkPara.cJiLiang = 0;			//计量模式
    g_tWorkPara.cShejiHD = 30;			//设计厚度
    
    //差值定位钢筋并计算结果
    LCD_SetColor(WHITE,BLUE);//CL_BLUE
    LCD_ShowString(ASC16x8,50, 40, 0,"CH1");
    LCD_ShowString(ASC16x8,200, 40, 0,"CH2");
    LCD_ShowString(ASC16x8,350, 40, 0,"CH3");
    
    //初始化原始数据存储参数
    InitSourceDataPara();
    
    //显示原始数据存储参数
    DispSourceDataPara();
    
	sprintf(&dateBuf[0],"%4d",g_SysTime.year);
	sprintf(&dateBuf[5],"%02d",g_SysTime.month);
	sprintf(&dateBuf[8],"%02d",g_SysTime.day);
//	if(dateBuf[5] == ' ') dateBuf[5] = '0';
//	if(dateBuf[8] == ' ') dateBuf[8] = '0';
	dateBuf[4] = '-';
	dateBuf[7] = '-';
	LCD_SetColor(WHITE,BLACK);
	LCD_ShowString(ASC16x8,100,8,0,dateBuf);
}

//---------------------------------------------------------------------------------------------------------------
//UI运行函数
void ui_SampleData_Run(void)
{
//	char btDataBuf[16];
//    char g_cStrBuf[64];
//    char filename[50];
//    char dataBuf[10];
//////    u8 result;
//////    unsigned int iTest = 0;
//////    int i=0,j=0;
    
	while(1){
		ui_KeyTask();
		ui_ButtonTask(6);
		
//		ui_DispTime(0);//时间
		ui_DispBattery(0);//电量

        //ADC采集
		Get_Adc_Data();
        
        //有位移变化再进行运算
        if(g_iCarDist != g_iCarLastDist){
            if(g_iCarDist > g_iCarLastDist){
                //滤波函数,计算差值及微分算法缓存数据
                SaveAdcDataBuf();
                
                //对数据进行降频滤波
                AdcFilter();

                //均值定位钢筋
                AimRebarSum();
                
                //差值定位钢筋
                AimRebarSub();
                
                //微分定位钢筋
                AimRebarDif();
                
                //综合分析处理结果
                AimRebarAnanlze_Sub();
            }
            
            g_iCarLastDist = g_iCarDist;
        }
        
		//标定
		if(g_u8TouchResult == BTN_BIAODING){
            Biaoding_Adc_Data();
		}
        
        //修改
        if(g_u8TouchResult == BTN_MODIFY){
            if(g_sourceDatapara.State == 1){
                g_sourceDatapara.State = 0;
            }
            else if(g_sourceDatapara.State == 0){
                g_sourceDatapara.State = 1;
            }
            DispSourceDataPara();
            InitTouchButton();
		}
        
        //加1
        if(g_u8TouchResult == BTN_ADD_1){
            if(g_sourceDatapara.State == 1){
                *ParaTab[g_sourceDatapara.HLNum].ParaVal += 1;
                if(*ParaTab[g_sourceDatapara.HLNum].ParaVal > 99){
                    *ParaTab[g_sourceDatapara.HLNum].ParaVal = 99;
                }
                if(g_sourceDatapara.HLNum == 0){
                    if(*ParaTab[g_sourceDatapara.HLNum].ParaVal > 4){
                        *ParaTab[g_sourceDatapara.HLNum].ParaVal = 1;
                    }
                }
                DispSourceDataPara();
            }
		}
            
        //减1
        if(g_u8TouchResult == BTN_SUB_1){
            *ParaTab[g_sourceDatapara.HLNum].ParaVal -= 1;
            if(*ParaTab[g_sourceDatapara.HLNum].ParaVal < 0){
                *ParaTab[g_sourceDatapara.HLNum].ParaVal = 0;
            }
            if(g_sourceDatapara.HLNum == 0){
                if(*ParaTab[g_sourceDatapara.HLNum].ParaVal == 0){
                    *ParaTab[g_sourceDatapara.HLNum].ParaVal = 4;
                }
            }
            DispSourceDataPara();
		}
        
        //加10
        if(g_u8TouchResult == BTN_ADD_10){
            if(g_sourceDatapara.State == 1){
                if(g_sourceDatapara.HLNum != 0){
                    *ParaTab[g_sourceDatapara.HLNum].ParaVal += 10;
                    if(*ParaTab[g_sourceDatapara.HLNum].ParaVal > 99){
                        *ParaTab[g_sourceDatapara.HLNum].ParaVal = 99;
                    }
                }
                DispSourceDataPara();
            }
		}
            
        //减10
        if(g_u8TouchResult == BTN_SUB_10){
            if(g_sourceDatapara.State == 1){
                if(g_sourceDatapara.HLNum != 0){
                    *ParaTab[g_sourceDatapara.HLNum].ParaVal -= 10;
                    if(*ParaTab[g_sourceDatapara.HLNum].ParaVal < 0){
                        *ParaTab[g_sourceDatapara.HLNum].ParaVal = 1;
                    }
                }
                DispSourceDataPara();
            }
		}
        
        //左滑
		if(g_u8TouchResult == SLIDE_END_LEFT){
            if(g_sourceDatapara.State == 1){
                LCD_FillRect(ParaTab[g_sourceDatapara.HLNum].X, 317, 20, 2, WHITE);
                g_sourceDatapara.HLNum --;
                if(g_sourceDatapara.HLNum < 0){
                    g_sourceDatapara.HLNum = g_sourceDatapara.Cnt + 4;
                }
                DispSourceDataPara();
            }
        }
        //右滑
		if(g_u8TouchResult == SLIDE_END_RIGHT){
            if(g_sourceDatapara.State == 1){
                LCD_FillRect(ParaTab[g_sourceDatapara.HLNum].X, 317, 20, 2, WHITE);
                g_sourceDatapara.HLNum ++;
                if(g_sourceDatapara.HLNum > g_sourceDatapara.Cnt + 4){
                    g_sourceDatapara.HLNum = 0;
                }
                DispSourceDataPara();
            }
        }

//////		//切换界面
//////		if(g_u16KeyValBuf == KEY_DOWN){
//////			Touch_Beep_On();
//////			SendUiNo(UI_SAMPLE10);
//////			LCD_FillRect(0,0,480,320,WHITE);
//////			return;
//////		}
		//清除界面
		if(g_u16KeyValBuf == KEY_FN){
			Touch_Beep_On();
//			SendUiNo(UI_BROWSE10);
//			LCD_FillRect(0,0,240,30,GOLD);
			LCD_FillRect(0,40,480,250,WHITE);
			LCD_SetColor(WHITE,BLACK);
            
            LCD_SetColor(LGRAY,BLACK);
            LCD_ShowChinese(Font20,0,5,0,"标定");//返回图标
            
//			LCD_ShowChinese(Font32,8,1,0,"迴");//返回图标
			ui_DispBattery(1);//显示电量
            
            g_iCarDist = 0;
            g_iCarLastDist = 0;
            g_iCarCountZero = g_iCarCount;

//            g_iCh1SumBuf = 0;
//            g_iCh2SumBuf = 0;
//            g_iCh3SumBuf = 0;
            
            //初始化定位钢筋参数
            InitAimRebarParaCH1();
            InitAimRebarParaCH2();
            InitAimRebarParaCH3();
            
            //初始化定位钢筋缓存
            InitAimRebarBuf();
            
            //初始化定位钢筋检测结果
            InitAimRebarResult();
		}
        
        //数据存储
		if(g_u16KeyValBuf == KEY_POWER){
//            BeepOn();
            
//            SaveTableDataFile();
            SaveSourceDataFile();
                
            //初始化检测参数
            Touch_Beep_On();
//			SendUiNo(UI_BROWSE10);
//			LCD_FillRect(0,0,240,30,GOLD);
			LCD_FillRect(0,40,480,250,WHITE);
            
            LCD_SetColor(LGRAY,BLACK);
            LCD_ShowChinese(Font20,0,5,0,"标定");//返回图标
            
//			LCD_SetColor(WHITE,BLACK);
//			LCD_ShowChinese(Font32,8,1,0,"迴");//返回图标
//			ui_DispBattery(1);//显示电量
            
            g_iCarDist = 0;
            g_iCarLastDist = 0;
            g_iCarCountZero = g_iCarCount;

//            g_iCh1SumBuf = 0;
//            g_iCh2SumBuf = 0;
//            g_iCh3SumBuf = 0;
            
            BeepOff();
            
            //初始化定位钢筋参数
            InitAimRebarParaCH1();
            InitAimRebarParaCH2();
            InitAimRebarParaCH3();
            
            //初始化定位钢筋缓存
            InitAimRebarBuf();
            
            //初始化定位钢筋检测结果
            InitAimRebarResult();
            
            //初始化虚拟按键
            InitTouchButton();

//            //采集原始数据
//            if(g_icarDist > 20){
//                LCD_SetColor(BLUE,LGRAY);//CL_BLUE
//                LCD_FillRect(55,85,265,155,LGRAY);
//                LCD_SetColor(LGRAY,LGRAY);//CL_BLUE
//                LCD_FillRect(60,90,260,150,LGRAY);
////		            iTest =  ReadLcdBoxBuf(0,27,120,180);//获取LCD 缓冲数据
//                sprintf(g_cStrBuf,"Data Saving...");
//                LCD_SetColor(LGRAY,WHITE);//CL_BLUE
//                LCD_ShowString(ASC16x8,80,92,0,g_cStrBuf);
////                  sprintf(g_cStrBuf,"TestA%02d%02d%02d%02d-%02d.txt",Rx8010sjTime.year,Rx8010sjTime.month,
////                  Rx8010sjTime.day,Rx8010sjTime.hour,g_iSaveFileNum);
////                  sprintf(g_cStrBuf,"0:/HD/%d.txt",g_iSaveFileNum);
//                sprintf(g_cStrBuf,"0:/HD/R%02d.csv",g_iSaveFileNum);
//                LCD_ShowString(ASC16x8,80,132,0,g_cStrBuf);
//                SaveSourceDataFile();
//                sprintf(g_cStrBuf,"Data Save OK!");
//                LCD_SetColor(LGRAY,WHITE);//CL_BLUE
//                LCD_ShowString(ASC16x8,80,92,0,g_cStrBuf);
//                iTest=10000000;
//                while(iTest--);
//                LCD_SetColor(WHITE,WHITE);//CL_BLUE
//                LCD_FillRect(55,75,265,165,WHITE);
//                LCD_SetColor(WHITE,BLUE);
//                sprintf(g_cStrBuf,"Cnt: 0     ");
//                LCD_ShowString(ASC16x8,230,5,0,g_cStrBuf);
//                sprintf(g_cStrBuf,"fNo: %d",g_iSaveFileNum);
//                LCD_ShowString(ASC16x8,230,25,0,g_cStrBuf);
//                
//                LCD_SetColor(WHITE,BLUE);
//                sprintf(g_cStrBuf,"0:/HD/R%02d.csv",g_iSaveFileNum-1);
//                LCD_ShowString(ASC16x8,10,30,0,g_cStrBuf);
//                
//                //初始化定位钢筋参数
//                InitAimRebarPara();
//                TIM4->CNT = 0;
//            }
        }
        
        if(g_u16KeyValBuf == KEY_BACK){
            SendUiNo(UI_MAIN);
			LCD_FillRect(0,0,480,320,WHITE);
			return;
        }

//////		//跳转界面
//////		if((g_u16KeyValBuf == KEY_UP)||(g_u8TouchResult == SLIDE_END_LEFT)){
//////			Touch_Beep_On();
//////			SendUiNo(UI_SAMPLE10);
//////			LCD_FillRect(0,0,480,320,WHITE);
//////			return;
//////		}

        
		//显示温度
//		if(g_bTemperaDispFlag == 1){
//			g_bTemperaDispFlag = 0;
//			LCD_SetColor(WHITE,BLACK);
//			LCD_ShowFloat(ASC24x12,420,296,0,5,1,g_GlobalPara.fTemperature);
//		}
		Sleep_ms(1);
	}
}





