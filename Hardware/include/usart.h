#ifndef _USART_H_
#define _USART_H_

#include <stdio.h>
#include "stm32f10x.h"

#define MAX_RX_LEN 512

// uint16_t Usart1ReciveLen = 0;
// uint16_t Usart2ReciveLen = 0;
// uint16_t Usart3ReciveLen = 0;

// uint8_t Usart1RXFinishFlag = 0;
// uint8_t Usart2RXFinishFlag = 0;
// uint8_t Usart3RXFinishFlag = 0;

// char Usart1RXBUF[MAX_RECIVE_LEN];
// char Usart2RXBUF[MAX_RECIVE_LEN];
// char Usart3RXBUF[MAX_RECIVE_LEN];

extern struct RX_Data {
	uint16_t UsartRxLen;
	uint8_t UsartRxFinishFlag;
	char UsartRXBUF[MAX_RX_LEN];
} RX1_Data, RX2_Data, RX3_Data;

void initUsart1(int baudrate); // 是否接收数据
void initUsart2(int baudrate); // 是否接收数据
void initUsart3(int baudrate); // 是否接收数据

void USART_printf(USART_TypeDef * USARTx, char * Data, ...);

#endif /* _USART_H_ */
