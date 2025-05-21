#include "headfile.h"


#define LIFT_ANGLE_THRESHOLD  40.0f    // Pitch 大于这个角度判断为拿起
#define LIFT_GYRO_THRESHOLD   100       // 陀螺仪 y 轴角速度
#define PUTDOWN_ANGLE_THRESHOLD 20.0f  // Pitch 小于这个角度认为可能已放下
#define PUTDOWN_WAIT_COUNT    20       // 放下后静止时间（例如 50 * 5ms = 250ms）

uint8_t lifted_flag = 0;
uint16_t putdown_counter = 0;
uint16_t lifted_counter = 0;
uint8_t balance_enable = 1;

uint8_t bluetooth_flag = 0;
uint16_t distance = 0;
uint8_t mode = 0;
uint8_t SoundLight_flag = 0;
uint8_t SoundLight_time = 0;


/**
 * 判断是否拿起
 */
void checkLiftState(void)
{
    // 拿起条件：角度大且角速度较大，持续一定时间
    if (fabs(Pitch) > LIFT_ANGLE_THRESHOLD && abs(gy) > LIFT_GYRO_THRESHOLD && Encoder_right > 50)
	{
		lifted_counter++;
		if (lifted_counter > 30)
		{
			lifted_flag = 1;
			balance_enable = 0;
			lifted_counter = 0;
			stop_flag = 1;
			stop();
		}
	}
}

/**
 * 判断是否放下
 */
void detectPutDown(void)
{
    if (lifted_flag || stop_flag)
    {
        if (fabs(Pitch) < 20 && abs(gy) < 150 && abs(Encoder_right) < 120)
        {
            if (putdown_counter++ > PUTDOWN_WAIT_COUNT)
            {
				lifted_flag = 0;
				putdown_counter = 0;
				stop_flag = 0;           // ? 清除紧急停止标志
				balance_enable = 1; 
            }
        }
        else
        {
            putdown_counter = 0;
        }
    }
}

void ModeSelect(void)
{
	if(Key_GetNum()) mode ++;
	mode %= 3;
	if(mode == 0) Balance_ON();
	else Balance_OFF();
	if(mode == 1) BlueTooth_ON();
	else BlueTooth_OFF();
	if(mode == 2) Follow_ON();
	else Follow_OFF();
	
	ObstacleAvoid();							// 蓝牙距离检测
	if(mode == 1) // 蓝牙
	{
		speed_kp = -0.42;
		speed_ki = -0.42/200;
	}
	else
	{
		speed_kp = -0.6;
		speed_ki = -0.6/200;
	}
	if(mode == 2)						// 超声波跟随
	{

		if(distance > 0 && distance <= 80)
		{
			dist_pid_control(); 
		}
		else
		{
			speed_tar = 0;  // 停止移动，避免无效距离导致继续前进
		}
	}
}	

void Bluetooth(void)
{
	if(straight || back || left || right)  bluetooth_flag = 1;
	else  bluetooth_flag = 0;
	/* 蓝牙控制 */
	if(straight == 1 && back == 0) 		speed_tar+=1;
	else if(back == 1 && straight == 0) speed_tar-=1;
	else if(straight == 0 && back == 0) speed_tar = 0;
	
	if(left == 1 && right == 0)			turn_speed-=1; 
	else if(right == 1 && left == 0) 	turn_speed+=1;
	else if(right == 0 && left == 0)	turn_speed = 0; 
	
	if(straight && left && back == 0 && right == 0)
	{
		speed_tar+=0.4;  turn_speed+=1.2; 
	}
	else if(straight && right && back == 0 && left == 0)
	{
		speed_tar+=0.4;  turn_speed-=1.2;
	}
	else if(back && left && straight == 0 && right == 0)
	{
		speed_tar-=0.4;	turn_speed+=1.2; 
	}
	else if(back && right && straight == 0 && left == 0)
	{
		speed_tar-=0.4;	turn_speed=1.2;
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
		speed_ki = -0.4 / 200; // 恢复自平衡回位
		turn_kd = 0.5;
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
	OLED_ShowNum(1, 7, distance, 3);
	if(mode == 1)
	{
		if (!obstacle_blocked) Bluetooth();  // 正常蓝牙控制
		if(distance > 0 && distance <= 80)
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
			
		}
	}
}
