#ifndef UI_SAMPJGJ_H
#define UI_SAMPJGJ_H

typedef struct{
	unsigned char jgj1_1;
	unsigned char jgj1_2;
	unsigned char jgjAvg1;
	unsigned char jgj2_1;
	unsigned char jgj2_2;
	unsigned char jgjAvg2;
	unsigned char jgj3_1;
	unsigned char jgj3_2;
	unsigned char jgjAvg3;
}JGJDataStruct;


void ui_SampJGJ_InitDisp(void);
void ui_SampJGJ_Run(void);

#endif

