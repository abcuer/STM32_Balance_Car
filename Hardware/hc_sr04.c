#include "headfile.h"

uint16_t Time; //记录时间
void HCSR04_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//开启GPIOA的时钟
	/*GPIO初始化*/ 
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//将PC15 TRIG
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//将PA1 ECHO

	GPIO_ResetBits(GPIOC, GPIO_Pin_15);
}

void HCSR04_Start(void)
{
	//给触发信号
	GPIO_SetBits(GPIOC, GPIO_Pin_15);
	Delay_us(10);
	GPIO_ResetBits(GPIOC, GPIO_Pin_15);
}

void HCSR04_GetValue(void)
{
	HCSR04_Start();
	Delay_us(50);
	distance = ((Time * 0.0001) * 34000) / 2;
	Time = 0;  //计时 返回时间计算距离
}
