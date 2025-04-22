#include "headfile.h"

float Pitch, Roll, Yaw;
short gx,gy,gz;
/* 直立环 */
float Med_angle = -7.8;  //机械中值
float angle_kp = 250*0.6;
float angle_kd = 0.8*0.6;

/* 速度环 */
float filter = 0.7;
float speed_kp = -0.5;
float speed_ki = -0.5/200;

/* 转向环 */
float turn_kd = 0.3;

float angle_out, speed_out, turn_out = 0;
float PWM_out, PWMA, PWMB = 0;

int main(void)
{
	mpu6050_Init();	
	MPU6050_DMP_Init();
	MPU6050_EXTI_Init();
	LED_Init();
	OLED_Init();		
	PWM_Init();
	Motor_Run_Init();
	encoder_left_Init();
	encoder_right_Init();
	UART2_Init(115200);
	
	/*OLED显示*/
	OLED_ShowString(1, 2, "Pitch: ");
	OLED_ShowString(2, 2, "Left: ");
	OLED_ShowString(3, 2, "Right: ");
	
	while (1)
	{	
		OLED_ShowSignedNum(1, 9, Pitch, 3);
		OLED_ShowSignedNum(2, 9, Encoder_left, 3);
		OLED_ShowSignedNum(3, 9, Encoder_right, 3);
	}
}

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
		{
			MPU6050_DMP_Get_Data(&Pitch, &Roll, &Yaw);
			MPU_Get_Gyroscope(&gx, &gy, &gz);
			
			angle_out = angle_pid_control(Med_angle, Pitch, gy);
			speed_out = speed_pid_control(filter, 0);
			turn_out = turn_pid_control(gz);
			
			PWM_out = angle_out - angle_kp * speed_out;
			PWMA = PWM_out - turn_out;
			PWMB = PWM_out + turn_out;
			
			Limit(PWMA, PWMB);
			motor_duty(PWMA, PWMB);
			
			EXTI_ClearITPendingBit(EXTI_Line0);
		}
    }
}

