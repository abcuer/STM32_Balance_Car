#ifndef _uart_h
#define _uart_h
#include "stm32f10x.h"                  // Device header
#include <stdio.h>

void UART2_Init(uint32_t bound);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);



#endif


