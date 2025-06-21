#include "headfile.h"

volatile uint8_t mpu_data_flag = 0;

int main(void)
{
	System_Init();														// 模块初始化
	/*OLED显示*/
	OLED_ShowString(1, 1, "dis:");
	OLED_ShowString(1, 13, "cm");
	while (1)
	{	
		if(mpu_data_flag)
		{
			Balance();													// 主控制逻辑
			checkLiftState();  											// 提起检测
			checkFallDown();											// 倒地检测
			detectPutDown(); 											// 着陆检测
			mpu_data_flag = 0;
		}
	}
}

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
		{
			mpu_data_flag = 1;
			
			EXTI_ClearITPendingBit(EXTI_Line0);
		}
    }
}

void TIM2_IRQHandler(void)		
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 1) Time ++;
		UpdateSoundLight();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		
	}
}
