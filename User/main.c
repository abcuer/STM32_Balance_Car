#include "headfile.h"

float Pitch, Roll, Yaw;
short gx,gy,gz;
/* 直立环 */
float angle_kp = -65*0.6;
float angle_kd = -3600*0.6;
/* 速度环 */
float filter = 0.7;
float speed_kp = 0.22;
float speed_ki = 0.22/ 200;
/* 转向环 */
float turn_kp = -0.7;

int main(void)
{
	mpu6050_Init();	
	MPU6050_DMP_Init();
	Delay_ms(100);
	LED_Init();
	OLED_Init();		
	Timer2_Init();
	PWM_Init();
	Motor_Run_Init();
	encoder_left_Init();
	encoder_right_Init();
	UART2_Init(115200);
//	pid_Init(&motorA, DELTA_PID, -60, 0, -60/200);
//	pid_Init(&motorB, DELTA_PID, -60, 0, -60/200);
	
	pid_Init(&angle, POSITION_PID, angle_kp,  0, angle_kd);
	
	/*OLED显示*/
	OLED_ShowString(1, 2, "Pitch: ");
	OLED_ShowString(2, 2, "Left: ");
	OLED_ShowString(3, 2, "Right: ");
	
	  // 50
	angle_target_set(2);
	
	while (1)
	{	
		//MPU_Get_Gyroscope(&gx,&gy,&gz);
		OLED_ShowSignedNum(1, 9, Pitch, 3);
		OLED_ShowSignedNum(2, 9, Encoder_left, 3);
		OLED_ShowSignedNum(3, 9, Encoder_right, 3);
	}
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		angle_pid_control();
		SpeedBalanceControl();
		
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位
    }
}

