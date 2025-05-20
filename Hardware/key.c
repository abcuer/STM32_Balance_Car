#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Key_Init(void) 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
    static uint8_t key_pre = 1;  // 记录上一次按键状态（初始为松开）
    uint8_t key_curr = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9); // 当前状态

    uint8_t key_pressed = 0;

    if (key_pre == 1 && key_curr == 0)
    {
        // 检测到由松变按 => 按键被按下
        key_pressed = 1;
    }

    key_pre = key_curr; // 更新上一次状态
    return key_pressed;
}
