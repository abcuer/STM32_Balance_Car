#ifndef _headfile_h
#define _headfile_h
#include "stm32f10x.h"                  // Device header

#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"

#include "delay.h"
#include "timer.h"
#include "sys.h"
#include "uart.h"
#include "pwm.h"
#include "exti.h"

#include "led.h"
#include "key.h"
#include "buzzer.h"
#include "OLED.h"
#include "motor.h"
#include "encoder.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "hc_sr04.h"

#include "control.h"
#include "mode.h"
#include "mode_show.h"

extern uint8_t motorA_dir;
extern uint8_t motorB_dir;

extern int16_t Encoder_left;
extern int16_t Encoder_right;

#define MAX_Speed 24
#define MAX_Turn 20

extern float Med_angle;
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
extern float speed_tar;

extern float turn_kd;
extern float turn_kp;
extern float turn_speed;

extern uint8_t straight;
extern uint8_t back;
extern uint8_t left;
extern uint8_t right;

extern uint8_t stop_flag;
extern uint8_t bluetooth_flag;

extern uint16_t Time;
extern uint16_t distance;
extern uint8_t mode;

extern uint8_t balance_enable;
#endif

