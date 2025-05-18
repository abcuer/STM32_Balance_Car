#include "stm32f10x.h"                  // Device header
void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}


void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void Buzzer_OFF(void)  // PC13同时驱动LED和蜂鸣器
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}
