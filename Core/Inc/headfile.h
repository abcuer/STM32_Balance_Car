#ifndef _headfile_h
#define _headfile_h

#include "stm32f1xx_hal.h"
#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include <stdlib.h>
/***************** *** ****************/
#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"

/***************** Bsp ****************/
#include "timer.h"
/***************** Modules ****************/
#include "led.h"
#include "key.h"
#include "buzzer.h"
#include "oled.h"
#include "mpu6050.h"
#include "motor.h"
#include "encoder.h"
#include "hcsr04.h"
#include "hc06.h"
/***************** App ****************/
#include "mode.h"
#include "pid.h"
#include "control.h"

#define MAX_Speed 24
#define MAX_Turn 20

extern Motor_t motor_left;
extern Motor_t motor_right;
extern UprightPID_t upright_pid;
extern SpeedPID_t speed_pid;
extern TurnPID_t turn_pid;
extern BT_Command_t bt_cmd;
extern pid_t dist;
extern BalanceState_t balance_state;

extern Euler_t euler;
extern Gyro_t gyro;
/************************ 标志位 *********************/
extern uint8_t stop_flag;
extern uint8_t bluetooth_flag;
/************************ 全局变量 *********************/
//extern uint16_t Time;
extern uint16_t distance;
extern uint8_t mode;
#endif