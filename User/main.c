#include "headfile.h"

/**
 * @brief 注意事项
 * @note 更改代码再烧录后，重新上电，代码才生效
 *		 一定先初始化 IMU，否则无论如何都无法平衡？！ 嘻嘻
		 最好不要更改模块的初始化顺序
 */

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
