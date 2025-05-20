#include "headfile.h"

uint8_t bluetooth_flag = 0;
uint16_t distance = 0;
uint8_t mode = 0;
uint8_t SoundLight_flag = 0;
uint8_t SoundLight_time = 0;

void select(void)
{
	if(Key_GetNum()) mode ++;
	mode %= 3;
	if(mode == 0) Balance_ON();
	else Balance_OFF();
	if(mode == 1) Blue_ON();
	else Blue_OFF();
	if(mode == 2) Avoid_ON();
	else Avoid_OFF();
}	

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

void SoundLight(void)
{
	if(SoundLight_flag == 0)
	{
		Buzzer_ON();
		SoundLight_flag = 1;
	}
}

void UpdateSoundLight(void)
{
    if(SoundLight_flag)
    {
        SoundLight_time++;

		if(SoundLight_time >= 20) 
		{
			Buzzer_OFF();
			SoundLight_time = 0;
			SoundLight_flag = 0; 
		}
        
    }
}
uint8_t obstacle_blocked = 0;  // 是否被障碍物拦住

void ObstacleAvoid(void)
{
	HCSR04_GetValue();
	OLED_ShowSignedNum(1, 10, distance, 3);
	if(mode == 1)
	{
		if(!obstacle_blocked && distance < 20.0f) 
		{
			SoundLight();
			obstacle_blocked = 1;
		}
		else if (obstacle_blocked && distance > 50.0f)
        {
            obstacle_blocked = 0; // 恢复触发能力
        }
		if (!obstacle_blocked)
        {
            Bluetooth();  // 正常蓝牙控制
        }
	}
}
