#include "headfile.h"

pid_t dist;

void pid_init(pid_t *pid, uint32_t mode, float p, float i, float d)
{
	pid->pid_mode = mode;
	pid->p = p;
	pid->i = i;
	pid->d = d;
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
		
		if(fabs(pid->error[0]) < 3.0f) {
			pid->out = 0;
			pid->iout = 0; // 可选：同时清除积分项，防止小误差导致积分累积
		}
	}

	// ��¼ǰ����ƫ��
	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];

	// ����޷�
}

void pidout_limit(pid_t *pid)
{
	if(pid->out>=19000)	
		pid->out=19000;
	if(pid->out<=-19000)	
		pid->out=-19000;
}

void dist_pid_control(void)
{
	dist.target = 30;
	dist.now = distance;
	pid_cal(&dist);
	pidout_limit(&dist);
	speed_tar = dist.out;
}
