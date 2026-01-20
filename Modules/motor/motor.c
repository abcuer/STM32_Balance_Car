#include "motor.h"
#include "bsp_pwm.h"
#include "control.h"

Motor_t motor_left = {
	.dir = 1,
	.encoder = 0
};
Motor_t motor_right = {
	.dir = 1,
	.encoder = 0
};
uint8_t stop_flag = 0;

void Motor_Init(void)
{
	PWM_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/*
左轮: PA4PA5   PWM1(PA6)
右轮: PB0 PB1 PWM2(PA7)
*/

/*左轮调速*/
static void MotorLeftDuty(int16_t duty) 
{
	if(duty >= 0)	motor_left.dir = 1;
	else
	{
		motor_left.dir = 0;
		duty = -duty;
	}
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction) motor_left.dir);
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction) !motor_left.dir);
	TIM_SetCompare1(TIM3, duty);
}
/*右轮调速*/
static void MotorRightDuty(int16_t duty) 
{
	if(duty >= 0)
	{
		motor_right.dir = 1;
	}
	else
	{
		motor_right.dir = 0;
		duty = -duty;
	}
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction) !motor_right.dir);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction) motor_right.dir);
	TIM_SetCompare2(TIM3, duty);
}

void MotorSetDuty(float PWMA, float PWMB)
{
	MotorLeftDuty(PWMA);
	MotorRightDuty(PWMB);
}

void MotorStop(void)
{
	stop_flag = 1;  // 立即设置停止标志
	DataClear();
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction) 1);
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction) 1);
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction) 1);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction) 1);
}

