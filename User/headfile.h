#ifndef _headfile_h
#define _headfile_h


#include "stm32f10x.h"                  // Device header

#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"

#include "Delay.h"
#include "Timer.h"
#include "sys.h"

#include "led.h"
#include "key.h"
#include "OLED.h"
#include "pwm.h"
#include "motor_run.h"
#include "encoder.h"
#include "uart.h"
#include "mpu6050.h"
#include "inv_mpu.h"

#include "pid.h"
#include "control.h"

extern uint8_t motorA_dir;
extern uint8_t motorB_dir;

extern int16_t Encoder_left;
extern int16_t Encoder_right;
//extern int16_t EncoderCnt_left;
//extern int16_t EncoderCnt_right;

//extern pid_t motorA;
//extern pid_t motorB;

extern pid_t angle;

extern float Pitch;
extern float Roll;
extern float Yaw;
extern short gx;
extern short gy;
extern short gz;

extern float angle_kp;
extern float angle_kd;
extern float filter;
extern float speed_kp;
extern float speed_ki;
extern float turn_kp;
#endif

