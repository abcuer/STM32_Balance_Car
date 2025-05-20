#ifndef _hc_sr04_h
#define _hc_sr04_h
#include "stm32f10x.h"                  // Device header

void HCSR04_Init(void);
void HCSR04_Start(void);
void HCSR04_GetValue(void);
float Filter_Distance(float new_value);
#endif
