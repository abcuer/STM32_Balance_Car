#include "stm32f10x.h"                  // Device header
#include "PWM.h"

#define offset 10

void Motor_Run_Init(void)
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
×óÂÖ: PA4¢PPA5   PWM1(PA6)
ÓÒÂÖ: PB0 PB1 PWM2(PA7)
*/
uint8_t motorA_dir = 1;
uint8_t motorB_dir = 1;

/*×óÂÖµ÷ËÙ*/
void Speed_L(uint16_t Speed_l)
{
	TIM_SetCompare1(TIM3, Speed_l);
}
/*ÓÒÂÖµ÷ËÙ*/
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
		motorA_dir = 0;
	}
	else
	{
		motorA_dir = 1;
		duty = -duty;
	}
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction) motorA_dir);
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction) !motorA_dir);
	Speed_L(duty + offset);
}

void angle_right_duty(int16_t duty) 
{
	if(duty >= 0)
	{
		motorB_dir = 0;
	}
	else
	{
		motorB_dir = 1;
		duty = -duty;
	}
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction) !motorB_dir);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction) motorB_dir);
	Speed_R(duty + offset);
}


