#ifndef __ENCODER_H
#define __ENCODER_H
#include "stm32f10x.h"                  // Device header


void encoder_left_Init(void);
void encoder_right_Init(void);
int16_t Encoder_left_Get(void);
int16_t Encoder_right_Get(void);
void UpdateEncoderCounts(void);
/*
	初始化编码器
	获取编码器的值
*/
#endif
