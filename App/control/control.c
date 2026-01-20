#include "control.h"
#include "encoder.h"
#include "hc_sr04.h"

/* 直立环 */
UprightPID_t upright_pid = {
    .kp = 300*0.6,		// 280*0.6
    .kd = 2.2*0.6,		// 0.9*0.6
	.out = 0,
    .med_angle = 0.5
};

/* 速度环 */
SpeedPID_t speed_pid = {
	.kp = -1,   		 //-0.58
	.ki = -1/200,	//-0.58/200
	.out = 0,
	.filter = 0.7,
	.speed = 0  /* 前进 后退 */
};

/* 转向环 */
TurnPID_t turn_pid = {
	.kd = 0,			// 0.5
	/* 左右移动 */
	.kp = 0,			// -20
	.out = 0,
	.speed = 0 
};

PID_t dist;

static float Encoder_Err, filtered_Err, last_filtered_Err, Encoder_S;

/* 机械中值，当前角度， gy*/
float AnglePidCtrl(float tar, float angle, short gy)
{
	return upright_pid.kp*(angle - tar) + upright_pid.kd*gy;
}

float SpeedPidCtrl(float x, float speed_tar)
{
	UpdateEncoder();
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
	return speed_pid.kp*filtered_Err + speed_pid.ki*Encoder_S;
}

float TurnPidCtrl(short gz)
{
	return turn_pid.kd*gz + turn_pid.kp*turn_pid.speed;
}

void DistPidCtrl(void)
{
	dist.target = 30;
	dist.now = distance;
	PidCalucate(&dist);
	speed_pid.speed = dist.out;
}

void PWMLimit(float PWMA, float PWMB)
{
	if(PWMA > MAXPWM) PWMA = MAXPWM;
	if(PWMA < -MAXPWM) PWMA = -MAXPWM;
	if(PWMB > MAXPWM) PWMB = MAXPWM;
	if(PWMB < -MAXPWM) PWMB = -MAXPWM;
}

void DataClear(void)
{
	Encoder_Err = 0, filtered_Err = 0, last_filtered_Err = 0, Encoder_S = 0;
}
