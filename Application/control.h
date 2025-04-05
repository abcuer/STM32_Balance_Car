#ifndef _control_h
#define _control_h

float Speed_pid_control(float filter, float Speed_Target, float Speed_Kp, float Speed_Ki);
float Turn_pid_control(float turn_kp);
void limit(float PWMA, float PWMB);
void SpeedBalanceControl(void);

#endif
