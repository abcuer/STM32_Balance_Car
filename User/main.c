#include "headfile.h"

volatile uint8_t mpu_data_flag = 0;

int main(void)
{
	mpu6050_Init();	
	MPU6050_DMP_Init();
	MPU6050_EXTI_Init();
	Delay_ms(300);
	Key_Init();
	LED_Init();
	OLED_Init();		
	PWM_Init();
	Motor_Init();
	encoder_left_Init();
	encoder_right_Init();
	HCSR04_Init();
	UART2_Init(115200);
	Timer_Init();
	Buzzer_Init();	
	pid_init(&dist, POSITION_PID, -1, 0, 1); 
	
	/*OLED显示*/
	OLED_ShowString(1, 1, "dis:");
	OLED_ShowString(1, 13, "cm");
	while (1)
	{	
		if(mpu_data_flag)
		{
			Balance();													// 主控制逻辑
			checkLiftState();  											// 拿起检测
			checkFallDown();											// 倒地检测
			detectPutDown(); 											// 放下检测
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
