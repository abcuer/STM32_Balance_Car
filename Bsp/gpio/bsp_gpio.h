#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "stm32f10x.h"

// 定义输出速率枚举，增加可读性
typedef enum {
    GPIO_SPEED_LOW  = GPIO_Speed_2MHz,
    GPIO_SPEED_MID  = GPIO_Speed_10MHz,
    GPIO_SPEED_HIGH = GPIO_Speed_50MHz
} GPIO_Speed_e;

void GPIO_Output(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_Speed_e speed);
void GPIO_Input(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef mode);
void GPIO_SetClock(GPIO_TypeDef* GPIOx, FunctionalState state);

#endif