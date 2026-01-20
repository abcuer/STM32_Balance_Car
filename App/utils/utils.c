#include "headfile.h"

static SoundLight_t sound_light = {
	.flag = 0, // 声光提示激活标志：1 表示正在进行声光报警，如避障蜂鸣器提醒；0 表示无报警
	.time = 0  // 声光提示计时器：记录声光报警的持续时间，到达设定值后自动关闭蜂鸣器等提示
};

/**
 * @brief 系统初始化函数，初始化所有模块和外设
 * @param 无
 * @retval 无
 */
void System_Init(void)
{
	IMU_Init();
	/* 先初始化 IMU，否则无论如何都无法平衡，嘻嘻 */
	Usart_Init(115200);
	Timer_Init();
	Key_Init();
	LED_Init();
	Buzzer_Init();	
	OLED_Init();	
	Motor_Init();
	Encoder_Init();
	HCSR04_Init();
	PID_Init(&dist, POSITION_PID, -0.6, 0, 0.1); 
	delay_ms(10);
}

/**
 * @brief 声光提示启动函数
 * @param 无
 * @retval 无
 */
static void SoundLight(void)
{
	if(sound_light.flag == 0)
	{
		Buzzer_ON();
		Follow_ON();
		sound_light.flag = 1;
	}
}

/**
 * @brief 声光提示状态更新函数，计时并关闭报警
 * @param 无
 * @retval 无
 */
static void UpdateSoundLight(void)
{
    if(sound_light.flag)
    {
        sound_light.time++;

		if(sound_light.time >= 20) 
		{
			Buzzer_OFF();
			Follow_OFF();
			sound_light.time = 0;
			sound_light.flag = 0; 
		}
        
    }
}
