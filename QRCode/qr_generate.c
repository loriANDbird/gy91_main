#include <string.h>
#include <stdlib.h>
#include "lcd.h"
#include "qr_encode.h"
#include "qr_generate.h"


void QRDisplay(int side,u8 *bitdata)
{
	int i,j,k;
	int bitwide = 8;
	for(i=0;i<side;i++){
		for(j=0;j<side;j++){
			k = j*side + i;
			if(bitdata[k/8]&(1<<(7-k%8))){
				LCD_FillRect(20+i*bitwide,20+j*bitwide,bitwide,bitwide,BLACK);
			}
		}
	}
}

void QRGenerator(char *input)
{
	int side;
	u8 bitdata[QR_MAX_BITDATA];
	
	//remove newline
	if(input[strlen(input)-1]=='\n'){
		input[strlen(input)-1] = 0;
	}
	
	side = qr_encode(QR_LEVEL_L,0,input,0,bitdata);
	QRDisplay(side,bitdata);
}
