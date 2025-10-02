#include "headfile.h"

volatile uint8_t mpu_data_flag = 0;

int main(void)
{
	System_Init();														// 模块初始化
	/*OLED显示*/
	OLED_ShowString(1, 1, "dis:");
	OLED_ShowString(1, 13, "cm");
	OLED_ShowString(3, 3, "Balance_Car");
	
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

/**
 * @brief 陀螺仪外部中断函数（平衡关键）
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

/**
 * @brief TIM2 定时器中断服务函数（用于超声波测距计时）
 * @note 本函数与 HC-SR04 超声波模块配合使用，实现测距时间的精确计数。
 *       工作原理如下：
 *       1. 触发测距后，Echo 引脚为高电平时开始计时；
 *       2. TIM2 定时器以固定周期中断（例如 0.1us / 次），每次中断判断 Echo 是否仍为高电平；
 *       3. 若 Echo 高电平持续，则递增计数变量 Time；
 *       4. 当 Echo 变为低电平（在其他地方检测），停止计时，通过 Time 计算出声波往返时间并换算为距离；
 *       5. 同时执行声光提示计时逻辑（如蜂鸣器自动关闭等）。
 */
void TIM2_IRQHandler(void)		
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 1) Time ++;
		UpdateSoundLight();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		
	}
}
