#include "wtc040.h" 
#include "math.h"  


unsigned char g_ucSpeakerIndex;
unsigned char g_ucSpeakerbuf[5];
unsigned char g_ucSpeakerDlyCnt;


#define Speaker_H() 		GPIO_SetBits(GPIOE, GPIO_Pin_0)
#define Speaker_L() 		GPIO_ResetBits(GPIOE, GPIO_Pin_0)

#define Speaker_ResetH() 	GPIO_SetBits(GPIOE, GPIO_Pin_1)
#define Speaker_ResetL() 	GPIO_ResetBits(GPIOE, GPIO_Pin_1)


/*
	����оƬ��������
*/
void Speaker_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1;		
    GPIO_Init(GPIOE, &GPIO_InitStructure);
		Speaker_ResetL();
		Speaker_L();
	g_ucSpeakerDlyCnt=0;
}



static void sDelay(unsigned short int idly)
{
	u16 i;
    for(; idly != 0; idly--)
	for(i=0;i<720;i++);
}
static void lDelay(unsigned short int idly)
{
	u16 i;
    for(; idly != 0; idly--)
	for(i=0;i<500;i++);
}

//������������n�������򷢵�n������//�ڵ�
void SpeakerPulses(unsigned char number)
{
	unsigned char i;
	//���ȵ�������
//	if(g_SysPara.ucBeepEn == 1)
//	{
		Speaker_ResetH();
		sDelay(31);	 
		Speaker_ResetL();
		sDelay(31); //44
		for(i = 0;i<number;i++){
			Speaker_L();
			lDelay(3);//18 = 120uS  55
			Speaker_H();
			lDelay(3);//18 = 120uS  55
		}
		Speaker_L() ;		
		Speaker_ResetL();
//	}	
}
 
//�������
void SpeakerNum(unsigned char result)
{
	unsigned char i;

	if(result > 99){ 		//��������     118�� �εΣ���ɲ�������  130�����������ܼ�¼		
		if(g_ucSpeakerIndex > 0){
			if(g_ucSpeakerIndex > 3){
				g_ucSpeakerIndex = 3;
			}
			for(i=g_ucSpeakerIndex;i>0;i--){
				g_ucSpeakerbuf[i] = g_ucSpeakerbuf[i-1];
			}
			g_ucSpeakerbuf[0] = result - 100;
			g_ucSpeakerIndex += 1;
		}	
		else{			
			g_ucSpeakerbuf[0] = result - 100;
			SpeakerPulses(g_ucSpeakerbuf[0]);
			g_ucSpeakerIndex = 0;
			g_ucSpeakerDlyCnt = 15; //300mS	
		}
	}
	else{
		g_ucSpeakerbuf[1] = result/10;  	//ʮλ��
		g_ucSpeakerbuf[0] = result % 10;  	//��λ��
		if(g_ucSpeakerbuf[0] == 0){
			g_ucSpeakerbuf[0] = 10;
		}
		if(g_ucSpeakerbuf[1] == 0){
			g_ucSpeakerbuf[1] = 10;
		}
		SpeakerPulses(g_ucSpeakerbuf[1]);	//Puls_number(SpeakerDat[SpeakerFlag-1]);
	      		
        g_ucSpeakerIndex = 1;
		g_ucSpeakerDlyCnt = 15; //300mS	
	}
}


unsigned char SpeakerScan(void)
{
	if(g_ucSpeakerDlyCnt > 0){
		g_ucSpeakerDlyCnt -= 1;
		if(g_ucSpeakerDlyCnt == 0){
			if(g_ucSpeakerIndex > 0){
				g_ucSpeakerDlyCnt = 15;
				g_ucSpeakerIndex -= 1;
				SpeakerPulses(g_ucSpeakerbuf[g_ucSpeakerIndex]);
			}
			else{
				return 1;
			}
		}
	}
	return 0;
}




