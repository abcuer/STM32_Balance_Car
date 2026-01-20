#ifndef _control_h
#define _control_h
#include "pid.h"

#define MAXPWM 25000

typedef struct {
    float kp;
    float kd;
	float out;
    float med_angle;
} UprightPID_t;  

typedef struct {
    float kp;
    float ki;
	float out;
    float filter;
	float speed;
} SpeedPID_t; 

typedef struct {
    float kp;
    float kd;
	float out;
	float speed;
} TurnPID_t; 

float AnglePidCtrl(float tar, float angle, short gy);
float SpeedPidCtrl(float x, float speed_tar);
float TurnPidCtrl(short gz);
void DistPidCtrl(void);

void PWMLimit(float PWMA, float PWMB);
void DataClear(void);

extern UprightPID_t upright_pid;
extern SpeedPID_t speed_pid;
extern TurnPID_t turn_pid;
extern PID_t dist;

#endif
