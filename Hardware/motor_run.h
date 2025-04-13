#ifndef __MOTOR_RUN_H
#define __MOTOR_RUN_H
#include "stm32f10x.h"                  // Device header

/*
左轮: Pin_14Pin_15  PWM2(PIn_2)
右轮: Pin_3   Pin_5 	PWM1(Pin_1)
*/

void Motor_Run_Init(void);
void Speed_L(uint16_t Speed_l); /*左轮调速*/
void Speed_R(uint16_t Speed_r); /*右轮调速*/
void angle_left_duty(int16_t duty); 
void angle_right_duty(int16_t duty); 
void motor_duty(float PWMA, float PWMB);

#endif
