#ifndef _hc_sr04_h
#define _hc_sr04_h
#include "stm32f10x.h"                  // Device header

#define FILTER_SIZE 5  // 滑动窗口大小

void HCSR04_Init(void);
void HCSR04_GetValue(void);

extern uint16_t distance;

#endif
