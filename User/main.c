#include "headfile.h"

volatile uint8_t mpu_data_flag = 0;

int main(void)
{
	System_Init();														// 模块初始化
	/*OLED显示*/
	OLED_ShowString(1, 4, "Standard");
	OLED_ShowString(2, 3, "Balance_Car");
	
	while (1)
	{	
		if(mpu_data_flag)
		{
			MPU_Get_Angle();
			Balance();													// 主控制逻辑
			CheckLiftState();  											// 提起检测
			CheckFallDown();											// 倒地检测
			DetectPutDown(); 											// 着陆检测
			ModeSelect();
			mpu_data_flag = 0;
		}
	}
}

/**
 * @brief 陀螺仪外部中断函数
 * @note 通过检测陀螺仪外部中断引脚的高低电平来判断陀螺仪数据输出
 */
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
