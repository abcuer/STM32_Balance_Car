#include "headfile.h"

#define limit 19000

/* 机械中值，当前角度， gy*/
float angle_pid_control(float tar, float angle, short gy)
{
	float PWM_out;
	PWM_out = angle_kp*(angle - tar) + angle_kd*(gy-0);
	return PWM_out;
}

float speed_pid_control(float x, float speed_tar)
{
	float PWM_out;
	static float Encoder_Err, filtered_Err, last_filtered_Err, Encoder_S;
	UpdateEncoderCounts();
	Encoder_Err = (Encoder_left + Encoder_right) - speed_tar;
	filtered_Err = (1-x)*Encoder_Err + x*last_filtered_Err;
	last_filtered_Err = filtered_Err;
	Encoder_S += filtered_Err;
    if (Encoder_S > 10000) Encoder_S = 10000;
    if (Encoder_S < -10000) Encoder_S = -10000;
	PWM_out = speed_kp*filtered_Err + speed_ki*Encoder_S;
	return PWM_out;
}

float turn_pid_control(short gz)
{
	float PWM_out = turn_kd*gz;
	return PWM_out;
}

void Limit(float PWMA, float PWMB)
{
	if(PWMA > limit) PWMA = limit;
	if(PWMA < -limit) PWMA = -limit;
	if(PWMB > limit) PWMB = limit;
	if(PWMB < -limit) PWMB = -limit;
	
}
