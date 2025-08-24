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
#include "pwm.h"
#include "exti.h"
#include "uart.h"
#include "BT.h"

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
#include "pid.h"

#define MAX_Speed 24
#define MAX_Turn 20
/************************ 结构体 *********************/
extern Motor_t motor_left;
extern Motor_t motor_right;
extern UprightPID_t upright_pid;
extern SpeedPID_t speed_pid;
extern TurnPID_t turn_pid;
extern BT_Command_t bt_cmd;
extern pid_t dist;
extern BalanceState_t balance_state;
/************************ 标志位 *********************/
extern uint8_t stop_flag;
extern uint8_t bluetooth_flag;
/************************ 全局变量 *********************/
extern uint16_t Time;
extern uint16_t distance;
extern uint8_t mode;


#endif

