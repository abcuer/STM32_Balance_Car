#include "headfile.h"

uint8_t mode = 0;
volatile uint8_t mpu_data_flag = 0;

float Pitch, Roll, Yaw;
short gx,gy,gz;
/* 直立环 */
float Med_angle = -7.8;  //机械中值
float angle_kp = 260*0.6;
float angle_kd = 0.9*0.6;

/* 速度环 */
float filter = 0.7;
float speed_kp = -0.25;
float speed_ki = -0.25/200;

/* 前进 后退 */
float speed_tar = 0;

/* 转向环 */
float turn_kd = 0;

/* 左右移动 */
float turn_kp = -35;
float turn_speed = 0;

float angle_out, speed_out, turn_out = 0;
float PWM_out, PWMA, PWMB = 0;

int main(void)
{
	mpu6050_Init();	
	MPU6050_DMP_Init();
	MPU6050_EXTI_Init();
	Delay_ms(100);
	Key_Init();
	OLED_Init();		
	PWM_Init();
	Motor_Run_Init();
	encoder_left_Init();
	encoder_right_Init();
	HCSR04_Init();
	UART2_Init(115200);
	Timer_Init();
	Buzzer_Init();
	/*OLED显示*/
//	OLED_ShowString(1, 1, "distance:");
//	OLED_ShowString(1, 13, "cm");
	
	while (1)
	{	
		if(mpu_data_flag)
		{
			MPU6050_DMP_Get_Data(&Pitch, &Roll, &Yaw);
			MPU_Get_Gyroscope(&gx, &gy, &gz);
			
			Bluetooth();
			
			angle_out = angle_pid_control(Med_angle, Pitch, gy);
			speed_out = speed_pid_control(filter, speed_tar);
			turn_out = turn_pid_control(gz);
			
			PWM_out = angle_out - angle_kp * speed_out;
			PWMA = PWM_out - turn_out;
			PWMB = PWM_out + turn_out;
			
			Limit(PWMA, PWMB);
			motor_duty(PWMA, PWMB);
			
			stop();
			mpu_data_flag = 0;
		}
		
		ObstacleAvoid();	
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

void TIM2_IRQHandler()		//定时器2的中断函数，不懂直接套用
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 1)
		{
			Time ++;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//清空标志位
	}
}
