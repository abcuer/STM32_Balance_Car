#include "stm32f10x.h"                  // Device header
#include "headfile.h"

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
uint8_t motorA_dir = 1;
uint8_t motorB_dir = 1;

/*左轮调速*/
void Speed_L(uint16_t Speed_l)
{
	TIM_SetCompare1(TIM3, Speed_l);
}
/*右轮调速*/
void Speed_R(uint16_t Speed_r)
{
	TIM_SetCompare2(TIM3, Speed_r);
}

//void speed_left_duty(int16_t duty)
//{
//	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction) motorA_dir);
//	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction) !motorA_dir);
//	Speed_L(duty);
//}
//void speed_right_duty(int16_t duty)
//{
//	GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction) !motorB_dir);
//	GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction) motorB_dir);
//	Speed_R(duty);
//}

void angle_left_duty(int16_t duty) 
{
	if(duty >= 0)
	{
		motorA_dir = 1;
	}
	else
	{
		motorA_dir = 0;
		duty = -duty;
	}
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction) motorA_dir);
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction) !motorA_dir);
	Speed_L(duty);
}

void angle_right_duty(int16_t duty) 
{
	if(duty >= 0)
	{
		motorB_dir = 1;
	}
	else
	{
		motorB_dir = 0;
		duty = -duty;
	}
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction) !motorB_dir);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction) motorB_dir);
	Speed_R(duty);
}
void motor_duty(float PWMA, float PWMB)
{
	angle_left_duty(PWMA);
	angle_right_duty(PWMB);
}


void stop(void)
{
	stop_flag = 1;  // 立即设置停止标志
	DataClear();
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction) 1);
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction) 1);
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction) 1);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction) 1);
}

