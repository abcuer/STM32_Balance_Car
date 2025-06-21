#ifndef _control_h
#define _control_h

float angle_pid_control(float tar, float angle, short gy);
float speed_pid_control(float x, float speed_tar);
float turn_pid_control(short gz);
void Limit(float PWMA, float PWMB);
void DataClear(void);

#endif
