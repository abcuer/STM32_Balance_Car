#ifndef _control_h
#define _control_h

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

float angle_pid_control(float tar, float angle, short gy);
float speed_pid_control(float x, float speed_tar);
float turn_pid_control(short gz);
void Limit(float PWMA, float PWMB);
void dist_pid_control(void);
void DataClear(void);

#endif
