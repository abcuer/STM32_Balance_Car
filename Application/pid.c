#include "headfile.h"

#define PitchOffset 9

pid_t angle;

void pid_Init(pid_t *pid, uint32_t mode, float p, float i, float d)
{
	pid->pid_mode = mode;
	pid->p = p;
	pid->i = i;
	pid->d = d;
}

void pidout_limit(pid_t *pid, int16_t limit)
{
	if(pid->out >= limit) pid->out = limit;
	if(pid->out <= -limit) pid->out = -limit;
}

void pid_cal(pid_t *pid)
{
	// ���㵱ǰƫ��
	pid->error[0] = pid->target - pid->now;

	// �������
	if(pid->pid_mode == DELTA_PID)  // ����ʽ
	{
		pid->pout = pid->p * (pid->error[0] - pid->error[1]);
		pid->iout = pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - 2 * pid->error[1] + pid->error[2]);
		pid->out += pid->pout + pid->iout + pid->dout;
	}
	else if(pid->pid_mode == POSITION_PID)  // λ��ʽ
	{
		pid->pout = pid->p * pid->error[0];
		pid->iout += pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - pid->error[1]);
		pid->out = pid->pout + pid->iout + pid->dout;
	}

	// ��¼ǰ����ƫ��
	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];
}

//void speed_pid_control(void)
//{
//	UpdateEncoderCounts();
//	if(motorA_dir) motorA.now = EncoderCnt_left; 	else  motorA.now = -EncoderCnt_left;
//	if(motorB_dir) motorB.now = EncoderCnt_right;	else  motorB.now = -EncoderCnt_right;
//	pid_cal(&motorA);
//	pid_cal(&motorB);
//	pidout_limit(&motorA, 800);
//	pidout_limit(&motorB, 800);
//	angle_target_set((motorA.out+ motorB.out) / 20);
//	speed_left_duty(motorA.out);
//	speed_right_duty(motorB.out);
//}

	float PWMA, PWMB;
    float turn_out;

void angle_pid_control(void)
{
	angle.now = PitchOffset + Pitch;
	angle.target = Speed_pid_control(filter, 0, speed_kp, speed_ki);
//	if (angle.target > 15.0f) angle.target = 15.0f;
//	if (angle.target < -15.0f) angle.target = -15.0f;
//	if (fabs(angle.target) < 0.1f) angle.target = 0;

	pid_cal(&angle);
	turn_out = Turn_pid_control(turn_kp);
    // Step 4: 合成PWM控制左右轮
    PWMA = angle.out + turn_out;
    PWMB = angle.out - turn_out;

    // Step 5: 限幅并输出
    limit(PWMA, PWMB);
    angle_left_duty(PWMA);
    angle_right_duty(PWMB);	
}

void angle_target_set(float tar)
{
	angle.target = tar;
}
