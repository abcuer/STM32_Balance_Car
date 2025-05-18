#include "headfile.h"

uint8_t bluetooth_flag = 0;
uint16_t distance = 0;

void Bluetooth(void)
{
	if(straight || back || left || right)  bluetooth_flag = 1;
	else  bluetooth_flag = 0;
	/* 蓝牙控制 */
	if(straight == 1 && back == 0) 		speed_tar+=1.5;
	else if(back == 1 && straight == 0) speed_tar-=1.5;
	else if(straight == 0 && back == 0) speed_tar = 0;
	
	if(left == 1 && right == 0)			turn_speed-=1.5; 
	else if(right == 1 && left == 0) 	turn_speed+=1.5;
	else if(right == 0 && left == 0)	turn_speed = 0; 
	
	if(straight && left && back == 0 && right == 0)
	{
		speed_tar+=0.8;  turn_speed+=1.7; 
	}
	else if(straight && right && back == 0 && left == 0)
	{
		speed_tar+=0.8;  turn_speed-=1.7;
	}
	else if(back && left && straight == 0 && right == 0)
	{
		speed_tar-=0.8;	turn_speed+=1.7; 
	}
	else if(back && right && straight == 0 && left == 0)
	{
		speed_tar-=0.8;	turn_speed=1.7;
	}
	if(straight == 0 && back == 0 && left == 0 && right == 0)
	{
		speed_tar = 0; 
		turn_speed = 0;
	}
	
	if(speed_tar >= MAX_Speed) 			speed_tar = MAX_Speed;
	else if(speed_tar <= -MAX_Speed) 	speed_tar = -MAX_Speed;
	if(turn_speed >= MAX_Turn) 			turn_speed = MAX_Turn;
	else if(turn_speed <= -MAX_Turn) 	turn_speed = -MAX_Turn;
	/* 运动约束 */
	if (bluetooth_flag)
	{
		speed_ki = 0;       // 关闭回原位置积分
		turn_kd = 0;
	}
	else
	{
		speed_ki = -0.5 / 200; // 恢复自平衡回位
		turn_kd = 0.3;
	}
}

void ObstacleAvoid(void)
{
	HCSR04_GetValue();
	OLED_ShowSignedNum(2, 1, distance, 3);
//	if(distance <= 10)  // 遇到障碍停止
//	{
//		Buzzer_ON();
//	}
//	else
//	{
//		Buzzer_OFF();
//	}
	
}
