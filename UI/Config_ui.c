/******************************************************************************
创建者：康泉
创建日期：2018.03.01
功能描述：
    1. UI 配置 、初始化
    2. SendUiNo()发送UI编号函数，为切换UI做准备
    3. UI调度，先执行一次 InitDisp 函数，然后切换函数指针，等待主程序 Main 调用
    
更新记录：
    1. 2018.03.01 初次创建
    
*******************************************************************************/
#include "Config_ui.h"

#define  MAX_UI_CNT   17   //最大ui界面数量

signed char g_cLastUiNo;  	//上一任务号
signed char g_cUiNo;      	//有负数的情况
void (*g_pRunFunc)();       //运行任务  指针

typedef struct  {
	signed char ui_Index;     //ui索引号
	void	(*InitDisp)();      	//初始化和背景显示函数
	void	(*Run)();       	//运行函数/任务
}STRUCT_UI_FUNC_TABLE;


const STRUCT_UI_FUNC_TABLE  tab_ui_Func[MAX_UI_CNT] = {
	
	{1,   (&ui_Start_InitDisp),  		(&ui_Start_Run)},	//预留
	{2,   (&ui_Main_InitDisp),        	(&ui_Main_Run)},		//1页 UI 界面
	{3,   (&ui_Sample10_InitDisp),    	(&ui_Sample10_Run)},	//2页 UI 界面
	{4,   (&ui_SampJGJ_InitDisp),    	(&ui_SampJGJ_Run)},		//3页 UI 界面
	{5,   (&ui_SampBlock_InitDisp),    	(&ui_SampBlock_Run)},	//4页 UI 界面
	{6,   (&ui_Browse10_InitDisp),    	(&ui_Browse10_Run)},	//5页 UI 界面
	{7,   (&ui_Verify_InitDisp),   		(&ui_Verify_Run)},		//6页 UI 界面
//////    {7,  (&ui_SampleData_InitDisp),  	(&ui_SampleData_Run)},	//预留
	{8,   (&ui_Paraset10_InitDisp),     (&ui_Paraset10_Run)},	//7页 UI 界面
	{9,   (&ui_Sampset_InitDisp),      	(&ui_Sampset_Run)},		//8页 UI 界面
	{10,  (&ui_Analyze_InitDisp),       (&ui_Analyze_Run)},		//9页 UI 界面
	{11,  (&ui_Sampdisp_InitDisp),    	(&ui_Sampdisp_Run)},	//10页 UI 界面
  	{12,  (&ui_Condition_InitDisp),   	(&ui_Condition_Run)},	//11页 UI 界面
	{13,  (&ui_Biaoding_InitDisp),   	(&ui_Biaoding_Run)},	//12页 UI 界面
    {14,  (&ui_Gain_InitDisp),   		(&ui_Gain_Run)},	//预留
	{15,  (&ui_Heat_InitDisp),  		(&ui_Heat_Run)},	//预留
	{16,  (&ui_View_InitDisp),  		(&ui_View_Run)},
//	{16,  (&ui_Start_InitDisp),  		(&ui_Start_Run)},	//预留
};

//------------------------------------------------------------------------
void UiInit(void)  //任务初始化，并执行第一个UI界面
{	
	g_cUiNo = 0; 
	g_cLastUiNo  = 0;   

	g_pRunFunc = tab_ui_Func[g_cUiNo].InitDisp;
	(*g_pRunFunc)(); 	
	g_pRunFunc = tab_ui_Func[g_cUiNo].Run;	
}

//输入参数为 小于等于0 时 返回上一任务序号
void SendUiNo(signed char UiNo)  
{    
	if(UiNo >= MAX_UI_CNT){  //任务号 范围保护，防止溢出
		return;
	}
    
	if(UiNo <= 0){   //小于等于0，
		g_cUiNo = g_cLastUiNo; 
	}
	else{				
//		if(g_cUiNo != (UiNo-1)){
//			g_cLastUiNo = g_cUiNo; //记录上一任务序号
//			g_cUiNo = (UiNo-1);  
//		}
		g_cLastUiNo = g_cUiNo; //记录上一任务序号
		g_cUiNo = (UiNo-1);
	}
    
	if(g_cUiNo < 0){  	//任务号 范围保护，防止溢出
		g_cUiNo = 0;
		g_cLastUiNo = 0;
	}
}

//任务调度， 先执行一次 ui_XXX 界面显示函数，然后切换任务指针，等待主循环执行任务
void UiSchedule(void) 
{	    
	g_pRunFunc = tab_ui_Func[g_cUiNo].InitDisp;
	(*g_pRunFunc)(); 	
	g_pRunFunc = tab_ui_Func[g_cUiNo].Run;	
}


