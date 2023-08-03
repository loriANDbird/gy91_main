#ifndef __GPS_H
#define __GPS_H	 

#define true	1
#define false	0

typedef struct{
	char UTC_Time[11];		//UTC时间(时分秒.毫秒)格式	  时分秒
	char status;	        //定位状态 A:有效 V:无效   ' ':临时有效和
	char latitude[13];		//纬度ddmm.mmmm (度分)格式
	char lat_NS;	        //北南
	char longitude[13];		//经度dddmm.mmmm (度分)格式
	char lon_WE;	        //西东
	char UTC_Date[8];		//UTC日期，ddmmyy(日月年)格式  年月日
    char RunState;          //运行状态 bit1锁定状态 bit0  1开启 0关闭状态
    char cSaveFlag;         //定位保存
    unsigned short int usCountDown;       //定时开启和关闭 3K=1分钟
    
	char latitudeBuf[10];	//显示纬度,x.xxxxx度
	char longitudeBuf[10];	//显示经度
    
    double lat;             //纬度,x.xxxxx度
    double lon;             //经度
    
}GPS_INFO_STR;

extern GPS_INFO_STR GPS_Info; // GPS_Info.cSaveFlag  longitudeBuf


void GPS_Init(void);	//默认9600
void GPS_Stop(void);	//
void GPS_Start(void);	//
void GPS_ManualStart(void);	//手动开始
void GPS_CountDown(void);	//系统20ms定时器调用，自动定位倒计时函数

void GPS_Location_Data(void);//主程序查询，定位数据

#endif

