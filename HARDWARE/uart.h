#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include "main.h"
#include <stdio.h>

extern unsigned char g_ucUart2SendBuf[50];
extern unsigned char g_ucUart2RecBuf[50];
extern unsigned char g_ucUart2RecValid;

void USART2_Init(int iBaud);
void UART4_Init(u32 baud);
void USART2_DMASendData(u16 SendLen);

#endif /* __USART1_H */
