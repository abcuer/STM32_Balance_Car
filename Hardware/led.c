#include "stm32f10x.h"                  // Device header


void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void LED_Red_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
}

void LED_Red_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
}
