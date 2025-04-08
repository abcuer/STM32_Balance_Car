#include "headfile.h"

float Speed_target = 0;
int g_iAbnormalSpinFlag = 0;//异常转动标志
unsigned char g_cMotorDisable = 0;//值等于0时电机正常转动，否则停止转动

/************
速度环PI：Kp*Ek+Ki*Ek_S
************/
float Speed_pid_control(float filter, float Speed_Target, float Speed_Kp, float Speed_Ki)
{
	static float PWM_out, Encoder_Err, EnC_Err_Lowout, EnC_Err_Lowout_last, Encoder_S = 0;

    UpdateEncoderCounts();
    Encoder_Err = (Encoder_left + Encoder_right) - Speed_Target;
    EnC_Err_Lowout = (1 - filter) * Encoder_Err + filter * EnC_Err_Lowout_last;
    EnC_Err_Lowout_last = EnC_Err_Lowout;

    Encoder_S += EnC_Err_Lowout;
    if (Encoder_S > 10000) Encoder_S = 10000;
    else if (Encoder_S < -10000) Encoder_S = -10000;

    PWM_out = Speed_Kp * EnC_Err_Lowout + Speed_Ki * Encoder_S;

    return PWM_out;
}

/**********************
转向环：系数*Z轴角速度
***********************/
float Turn_pid_control(float turn_kp)
{
	float PWM_out = 0;
	PWM_out = turn_kp*gz;
	return PWM_out;
}

void limit(float PWMA, float PWMB)
{
	if(PWMA >= 950) PWMA = 950;
	if(PWMA <= -950) PWMA = -950;
	if(PWMB >= 950) PWMB = 950;
	if(PWMB <= -950) PWMB = -950;
}

/***********************
串联控制
***********************/
//void SpeedBalanceControl(void)
//{
//	float speed_out, turn_out, PWM_out, PWMA, PWMB;
//	speed_out = Speed_pid_control(filter, Speed_target, speed_kp, speed_ki);
//	turn_out = Turn_pid_control(turn_kp);
//	PWM_out = angle.out - angle_kp*speed_out;
//	PWMA = PWM_out + turn_out;
//	PWMB = PWM_out - turn_out;
//	limit(PWMA, PWMB);
//	angle_left_duty(PWMA);
//	angle_right_duty(PWMB);
//}
void SpeedBalanceControl(void)
{
    
    // Step 1: 速度环（外环） → 输出目标角度
    

    // Step 2: 平衡环（内环） → 控制角度靠近目标角度
    //PWM_out = Angle_pid_control(angle.angle, angle_target, angle_kp, angle_kd);

    // Step 3: 转向控制（可选单独处理）
    
}

