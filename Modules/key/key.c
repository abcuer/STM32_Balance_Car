#include "stm32f10x.h"                  // Device header
#include "key.h"                  

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
    static uint8_t key_pre = 1;  
    uint8_t key_curr = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9); // µ±Ç°×´Ì¬
    uint8_t key_pressed = 0;
    if (key_pre == 1 && key_curr == 0)
    {
        key_pressed = 1;
    }
    key_pre = key_curr;
    return key_pressed;
}
