#include "stm32f10x.h"                  // Device header


void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Follow_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
}

void Follow_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
}

void BlueTooth_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}

void BlueTooth_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
}

void Balance_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
}

void Balance_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
}
