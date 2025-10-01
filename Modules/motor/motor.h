#ifndef _motor_h
#define _motor_h

#include "headfile.h"
void motor_init(void);
void angle_left_duty(int16_t duty);
void angle_right_duty(int16_t duty);
void motor_duty(int16_t dutyL, int16_t dutyR);

#endif