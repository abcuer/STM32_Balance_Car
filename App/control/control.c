#include "control.h"
#include "encoder.h"
#include "hc_sr04.h"

/* 直立环 */
PIDParam_t upright_pid = {
    .kp = 400*0.6,		
    .kd = 1.55*0.6,		
	.out = 0,
    .tar = 0.8
};

/* 速度环 */
PIDParam_t speed_pid = {
	.kp = -0.62,   		 
	.ki = -0.62/200,	
	.out = 0,
	.filter = 0.7,
	.tar = 0  /* 前进 后退 */
};

/* 转向环 */
PIDParam_t turn_pid = {
	.kd = 0.35,			
	/* 左右移动 */
	.kp = -30,			// 遥控模式下的转向速度
	.out = 0,
	.tar = 0 
};

PIDParam_t dist_pid = {
	.kp = -0.35,				
	.ki = -0.35/200,			
	.out = 0,
	.tar = 40
};


PID_t dist;

static float Encoder_Err, filtered_Err, last_filtered_Err, Encoder_S;

/* 机械中值，当前角度， gy*/
float AnglePidCtrl(float tar, float current, short gy)
{
	return upright_pid.kp*(current - tar) + upright_pid.kd*gy;
}

float SpeedPidCtrl(float filter, float tar)
{
	UpdateEncoder();
	Encoder_Err = (motor_left.encoder + motor_right.encoder) - tar;
	filtered_Err = (1-filter)*Encoder_Err + filter*last_filtered_Err;
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
	return turn_pid.kd*gz + turn_pid.kp*turn_pid.tar;
}

void DistPidCtrl(void)
{
	dist.target = dist_pid.tar;
	dist.now = distance;
	PidCalucate(&dist);
	speed_pid.tar = dist.out;
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
