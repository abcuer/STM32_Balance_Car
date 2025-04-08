#include "headfile.h"

float Pitch, Roll, Yaw;
short gx,gy,gz;
/* 直立环 */
float angle_kp = -67*0.6;
float angle_kd = -270*0.6;
//float angle_kp = 0;
//float angle_kd = 0;
/* 速度环 */
float filter = 0.7;
float speed_kp = -0.25;
float speed_ki = -0.25 /200;
//float speed_kp = -0.12;
//float speed_ki = -0.12 /200;
/* 转向环 */
float turn_kp = -0.17;

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
	
	pid_Init(&angle, POSITION_PID, angle_kp,  0, angle_kd);
	
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
        // 读取最新姿态
        MPU6050_DMP_Get_Data(&Pitch, &Roll, &Yaw);
		MPU_Get_Gyroscope(&gx, &gy, &gz);
        // 调用你的控制函数
        angle_pid_control();

        // 清除中断标志
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

