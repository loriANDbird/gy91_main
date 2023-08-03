/******************************************************************************
创建者：
创建日期：2018.06.01
功能描述：
    1. 数据文件读写相关函数
    2. 
    
更新记录：
    1. 2018.06.01 初次创建
*******************************************************************************/
#include "OpFile.h"
#include "Config_ui.h"
#include "exfuns.h"

static unsigned short sFileIndex[MaxFileCount]={0};

FileListStruct FileList[6];//文件名列表
FileDataHeadStruct FlieDataHead;//文件头

//扫描文件个数
int Scan_FileCount(char* path)
{
	int fileCount = 0;
	FRESULT fRes;
	DIR fDir;
	static FILINFO fInfo;
	fRes = f_opendir(&fDir, path);
	
	if(fRes == FR_OK){
		while(1){
			fRes = f_readdir(&fDir, &fInfo);
			if(fRes != FR_OK || fInfo.fname[0] == 0) break;
			if(fInfo.fattrib & AM_DIR) continue;//是目录
			fileCount++;
		}
	}
	f_closedir(&fDir);
	return fileCount;
}

//获得每页第一个文件索引
int Scan_FileIndex(char* path)
{
	int fileCount = 0;
//	short pageNum = 0;
//	unsigned char fileNum = 0;
	FRESULT fRes;
	DIR fDir;
	static FILINFO fInfo;
	fRes = f_opendir(&fDir, path);
	
	if(fRes == FR_OK){
		while(1){
			fRes = f_readdir(&fDir, &fInfo);
			if(fRes != FR_OK || fInfo.fname[0] == 0) break;
			if(fInfo.fattrib & AM_DIR) continue;//是目录
			if(fileCount >= MaxFileCount) break;//最多2000个文件
//			pageNum = fileCount/5;
//			fileNum = fileCount%5;
//			if(pageNum >= (MaxFileCount/5)) break;
//			
//			if(fileCount == 0){
//				sFileIndex[0] = fDir.index;
//			}else{
//				if(fileNum == 0){
//					sFileIndex[pageNum] = fDir.index;
//				}
//			}
			sFileIndex[fileCount] = fDir.index;
			fileCount++;
		}
	}
	f_closedir(&fDir);
	return fileCount;
}
//扫描指定页文件名列表,page:1,2,3...
void Scan_OnePageFiles(char* path,unsigned short page,unsigned short fileQnt)
{
	char i = 0,j;
	unsigned char recordFlag = 0;
	unsigned char pageLen = 6;
	unsigned char scanLen = 6;
	unsigned short startIndex;
	unsigned short pageQnt;
	
	FRESULT fRes;
	DIR fDir;
	FILINFO fInfo;
	
	unsigned char lfnBuf[_MAX_LFN];
	
	#if _USE_LFN
		fInfo.lfname = (TCHAR*)lfnBuf;
		fInfo.lfsize = _MAX_LFN;
	#endif
	
	if(fileQnt%pageLen == 0){
		pageQnt = fileQnt/pageLen;
	}else{
		pageQnt = fileQnt/pageLen+1;
	}
	
	for(j=0;j<pageLen;j++){
		FileList[j].fileEn = 0;
	}
	
	if(page == pageQnt){
		startIndex = 0;
		scanLen = fileQnt%pageLen;
		if(scanLen != 0){
			i = pageLen - scanLen;
		}
	}else if(page < pageQnt){
		startIndex = fileQnt-page*pageLen;
		scanLen = pageLen;
	}else{
		return;
	}
	
	fRes = f_opendir(&fDir, path);
	
	if(fRes == FR_OK){
		while(1){
			fRes = f_readdir(&fDir, &fInfo);
			if(fRes != FR_OK || fInfo.fname[0] == 0) break;
			if(fInfo.fattrib & AM_DIR) continue;//是目录
			
			if(recordFlag == 0){
				if(sFileIndex[startIndex] == fDir.index){
					recordFlag = 1;
					#if _USE_LFN
					if(fInfo.lfname[0] != 0){
						sprintf((char*)FileList[pageLen-1-i].NameStr,"%s",fInfo.lfname);
					}else{
						sprintf((char*)FileList[pageLen-1-i].NameStr,"%s",fInfo.fname);
					}
					#else
						sprintf((char*)FileList[pageLen-1-i].NameStr,"%s",fInfo.fname);
					#endif
					FileList[pageLen-1-i].FileDate = fInfo.fdate;
					FileList[pageLen-1-i].FileTime = fInfo.ftime;
					FileList[pageLen-1-i].fileEn = 1;
					i++;
					if(i >= pageLen)break;
				}
			}else{
				#if _USE_LFN
					if(fInfo.lfname[0] != 0){
						sprintf((char*)FileList[pageLen-1-i].NameStr,"%s",fInfo.lfname);
					}else{
						sprintf((char*)FileList[pageLen-1-i].NameStr,"%s",fInfo.fname);
					}
				#else
					sprintf((char*)FileList[pageLen-1-i].NameStr,"%s",fInfo.fname);
				#endif
				FileList[pageLen-1-i].FileDate = fInfo.fdate;
				FileList[pageLen-1-i].FileTime = fInfo.ftime;
				FileList[pageLen-1-i].fileEn = 1;
				i++;
				if(i >= pageLen)break;
			}
		}
	}
	f_closedir(&fDir);
}


//逐个删除文件
int DeleteFile(char* path)
{
	char filename[32];
	int fileCount = 0;
	FRESULT fRst;
	DIR fDir;
	static FILINFO fInfo;
	fRst = f_opendir(&fDir, path);//打开目录
	
	if(fRst == FR_OK){
		while(1){
			fRst = f_readdir(&fDir, &fInfo);
			if(fRst != FR_OK || fInfo.fname[0] == 0) break;
			if(fRst == FR_OK){

				sprintf(filename,"0:/GY91/%s",fInfo.fname);

				fRst = f_unlink(filename);

				fileCount++;
			}		
		}
	}
	f_closedir(&fDir);
	return fileCount;
}


//====================================================================================================
//函 数 名 : f_deldir
//====================================================================================================
FRESULT f_deldir(char *path)
{
    FRESULT res;
//	DeleteFile(path);
	res = f_unlink(path);
	return res;
}

//设置文件时间
FRESULT set_timestamp(char *obj, RX8010SJTIME RX8010Ssys)
{
    FILINFO fno;

    fno.fdate = (WORD)(((RX8010Ssys.year - 1980) * 512U) | RX8010Ssys.month * 32U | RX8010Ssys.day);
    fno.ftime = (WORD)(RX8010Ssys.hour * 2048U | RX8010Ssys.min * 32U | RX8010Ssys.sec / 2U);

    return f_utime(obj, &fno);
	
//			fno.fdate = (WORD)((g_SysTime.year-1980)<<9)|((g_SysTime.month&0xf)<<5)|(g_SysTime.day&0x1f);
//			fno.ftime = (WORD)(g_SysTime.hour<<11)|((g_SysTime.min&0x3f)<<6)|(g_SysTime.sec&0x1f);
//			f_utime(FileOprtPara.fileName,&fno);
}
