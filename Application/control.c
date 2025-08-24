#include "headfile.h"

#define limit 19000

static float Encoder_Err, filtered_Err, last_filtered_Err, Encoder_S;

/* 机械中值，当前角度， gy*/
float angle_pid_control(float tar, float angle, short gy)
{
	float pwm_out;
	pwm_out = upright_pid.kp*(angle - tar) + upright_pid.kd*(gy-0);
	return pwm_out;
}

float speed_pid_control(float x, float speed_tar)
{
	float pwm_out;
	UpdateEncoderCounts();
	Encoder_Err = (motor_left.encoder + motor_right.encoder) - speed_tar;
	filtered_Err = (1-x)*Encoder_Err + x*last_filtered_Err;
	last_filtered_Err = filtered_Err;
	Encoder_S += filtered_Err;
    if (Encoder_S > 10000) Encoder_S = 10000;
    if (Encoder_S < -10000) Encoder_S = -10000;
	if(stop_flag) 
	{
		Encoder_S = 0; // 小车偏转角度过大时清零积分量，防止小车重启时乱跑
		stop_flag = 0;
	}
	pwm_out = speed_pid.kp*filtered_Err + speed_pid.ki*Encoder_S;
	return pwm_out;
}

float turn_pid_control(short gz)
{
	float pwm_out = turn_pid.kd*gz + turn_pid.kp*turn_pid.speed;
	return pwm_out;
}

void Limit(float PWMA, float PWMB)
{
	if(PWMA > limit) PWMA = limit;
	if(PWMA < -limit) PWMA = -limit;
	if(PWMB > limit) PWMB = limit;
	if(PWMB < -limit) PWMB = -limit;
}

void dist_pid_control(void)
{
	dist.target = 30;
	dist.now = distance;
	pid_cal(&dist);
	pidout_limit(&dist);
	speed_pid.speed = dist.out;
}

void DataClear(void)
{
	Encoder_Err = 0, filtered_Err = 0, last_filtered_Err = 0, Encoder_S = 0;
}
