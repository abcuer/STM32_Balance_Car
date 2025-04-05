#include "headfile.h"

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


//void motor_target_set(int16_t tarA, int16_t tarB)
//{
//	if(tarA  >= 0)
//	{
//		motorA_dir = 0;
//		motorA.target = tarA;
//	}
//	else
//	{
//		motorA_dir = 1;
//		motorA.target = -tarA;
//	}
//	if(tarB  >= 0)
//	{
//		motorB_dir = 0;
//		motorB.target = tarB;
//	}
//	else
//	{
//		motorB_dir = 1;
//		motorB.target = -tarB;
//	}
//	
//}

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

void angle_pid_control(void)
{
	MPU6050_DMP_Get_Data(&Pitch, &Roll, &Yaw);
	angle.now = Pitch;
	pid_cal(&angle);
	pidout_limit(&angle, 800);
//	angle_left_duty(angle.out);
//	angle_right_duty(angle.out);	
}

void angle_target_set(float tar)
{
	angle.target = tar;
}
