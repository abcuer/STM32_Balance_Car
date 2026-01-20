#ifndef __MOTOR_RUN_H
#define __MOTOR_RUN_H
#include "stm32f10x.h"                  // Device header

/*
×óÂÖ: Pin_14¢PPin_15  PWM2(PIn_2)
ÓÒÂÖ: Pin_3   Pin_5 	PWM1(Pin_1)
*/

typedef struct{
	uint8_t dir;
	int32_t encoder;
} Motor_t;

void Motor_Init(void);
void MotorSetDuty(float PWMA, float PWMB);
void MotorStop(void);

extern Motor_t motor_left;
extern Motor_t motor_right;
extern uint8_t stop_flag;

#endif
