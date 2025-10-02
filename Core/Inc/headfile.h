#ifndef _headfile_h
#define _headfile_h

#include "stm32f1xx_hal.h"
#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
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

extern int16_t left_encoder;
extern int16_t right_encoder;

#endif