#include "headfile.h"
void Follow_OFF(void)
{
    HAL_GPIO_WritePin(GPIOB, Follow_Pin, GPIO_PIN_RESET);
}

void Follow_ON(void)
{
	HAL_GPIO_WritePin(GPIOB, Follow_Pin, GPIO_PIN_SET);
}

void BlueTooth_OFF(void)
{
	HAL_GPIO_WritePin(GPIOB, BlueTooth_Pin, GPIO_PIN_RESET);
}

void BlueTooth_ON(void)
{
	HAL_GPIO_WritePin(GPIOB, BlueTooth_Pin, GPIO_PIN_SET);
}

void Balance_OFF(void)
{
	HAL_GPIO_WritePin(GPIOB, Balance_Pin, GPIO_PIN_RESET);
}

void Balance_ON(void)
{
	HAL_GPIO_WritePin(GPIOB, Balance_Pin, GPIO_PIN_SET);
}