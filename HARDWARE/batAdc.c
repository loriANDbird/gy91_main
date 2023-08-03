/*******************************************************************************
创 建 者：张喜双
创建日期：2018.08.16
功能描述：
    1. ADC 采集电池电压.
        
更新记录：
    1. 2018.08.16 创建   
******************************************************************************/

#include "batAdc.h"
#include <stdio.h>
#include "lcd.h"
#include "font.h"
#include "OpPara.h"

//电压滤波
int Filter[FILTERLEN+1];
//电量折线表
const int BatPercentTable[13] =    {0,  5,	  10,  20,	30,	 40,  50,  60,	70,	 80,  90, 100,};
const float BatVoltageTable[13] =  {3.2,3.31,3.39,3.48,3.53,3.57,3.62,3.71,3.79,3.88,4.0,4.20,};

const float HardVersionTable[12] = {0,0.25,0.5,0.75,1.0,1.25,1.5,1.75,2.0,2.25,2.5,3.3};

//--------------------------------------------------------------------
// 函数名称: Bat_Adc_Init
// 说    明: 电量采集ADC初始化
// 输入参数: 无
//--------------------------------------------------------------------
void Bat_Adc_Init(void)
{
	int i;
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);  //使能ADC1通道时钟

	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 

	//PA0                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	 //模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);   //复位ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);	 //初始化外设ADC1的寄存器   

	ADC_Cmd(ADC1, ENABLE);	 //使能指定的ADC1	
	ADC_ResetCalibration(ADC1);	  //使能复位校准  
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		

	for(i=0;i<FILTERLEN;i++) Get_Bat_Quantity();
}				  

//--------------------------------------------------------------------
// 函数名称: Get_ADC_Value
// 说    明: 获取ADC采样值
// 输入参数: ch(ADC通道号)
//--------------------------------------------------------------------
int Get_ADC_Value(unsigned char ch)   
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	 			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));

	return ADC_GetConversionValue(ADC1);
}

//--------------------------------------------------------------------
// 函数名称: ADC_Filter
// 说    明: AD值滤波处理
// 输入参数: 无
//--------------------------------------------------------------------
int ADC_Filter(void)
{
	int i,j;
	int adcSum = 0;
	int temp = 0;
	int sortBuf[FILTERLEN];
	Filter[FILTERLEN] = Get_ADC_Value(ADC_Channel_0);
	for(i=0; i<FILTERLEN; i++){
		Filter[i] = Filter[i+1];
		sortBuf[i] = Filter[i];		
	}
	for(i=0; i<FILTERLEN-1; i++){
		for(j=i+1; j<FILTERLEN; j++){
			if(sortBuf[j] < sortBuf[i]){
				temp = sortBuf[i];
				sortBuf[i] = sortBuf[j];
				sortBuf[j] = temp;
			}
		}
	}
	for(i=10; i<FILTERLEN-10; i++){
		adcSum += sortBuf[i];
	}
	return (adcSum/(FILTERLEN-20));
}

//ADC电压准换
float Get_Bat_Voltage(void)
{
	u16 adcVal;
	int ibatVoltage;
	float adcVol, fbatVoltage;
	adcVal = ADC_Filter();///4096 * 3.3; //ADC采集电压值
//	printf("ADCAVG = %f \n", adcVal);
	adcVol = (adcVal>>2) / 1024.0f * 2.5f;
	ibatVoltage = adcVol * 3.0f * 100; //计算电池电压
	fbatVoltage = (float)ibatVoltage/100.0f;	//保留两位小数
	
	return fbatVoltage;
}
//--------------------------------------------------------------------
// 函数名称: Get_Bat_Percent
// 说    明: 折线计算电量
// 输入参数: 无
//--------------------------------------------------------------------
int Get_Bat_Percent(float voltage)
{
	int i;
	u8 tabLen = 12;
	int outValue = 0;
	float votgDif,pcntDif;
	float ratio;
	
	if(voltage < BatVoltageTable[0]){
		outValue = 0;
	}else if(voltage >= BatVoltageTable[tabLen-1]){
		outValue = 100;
	}else{
		for(i=0; i<tabLen-1; i++) {
			if((voltage >= BatVoltageTable[i]) && (voltage < BatVoltageTable[i+1])) {
				pcntDif = (float)(BatPercentTable[i+1] - BatPercentTable[i]);
				votgDif = (BatVoltageTable[i+1] - BatVoltageTable[i]);
				ratio = pcntDif / votgDif;
				outValue = BatPercentTable[i] + ratio * (voltage - BatVoltageTable[i]);
				break;
			}
		}
	}
	return outValue;
}
//--------------------------------------------------------------------
// 函数名称: Get_Bat_Quantity
// 说    明: 计算电池电压值
// 输入参数: 无
//--------------------------------------------------------------------
int Get_Bat_Quantity(void)
{
	int percent;
	float fVoltage;
	
	fVoltage = Get_Bat_Voltage();
	percent = Get_Bat_Percent(fVoltage);
	g_GlobalPara.fBatVoltage = fVoltage;
	
	return percent;
}

//--------------------------------------------------------------------
//读取硬件版本号
void Get_Hard_Version(void)
{
	u8 i;
	int mainAdc = 0,subAdc = 0;
	float mainVtg = 0.0f,subVtg = 0.0f;
	u8 mianNum = 0,subNum = 0;
	for(i=0;i<10;i++){
		mainAdc += Get_ADC_Value(ADC_Channel_2);
		subAdc += Get_ADC_Value(ADC_Channel_3);
	}
	mainAdc /= 10;
	subAdc /= 10;
	
	mainVtg = (float)mainAdc/4096*2.5;
	subVtg = (float)subAdc/4096*2.5;
	
	for(i=0;i<11;i++){
		if((mainVtg > HardVersionTable[i])&&(mainVtg < HardVersionTable[i+1])){
			mianNum = i;
		}
		if((subVtg > HardVersionTable[i])&&(subVtg < HardVersionTable[i+1])){
			subNum = i;
		}
	}
	sprintf(g_SysPara.HardVersion,"V%d.%d",mianNum,subNum);
}
