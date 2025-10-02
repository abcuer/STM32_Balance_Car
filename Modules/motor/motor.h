#ifndef _motor_h
#define _motor_h

#include "stdint.h"

typedef struct{
	uint8_t dir;
	int16_t encoder;
} Motor_t;

void motor_init(void);
void angle_left_duty(int16_t duty);
void angle_right_duty(int16_t duty);
void motor_duty(int16_t dutyL, int16_t dutyR);
void stop(void);

#endif