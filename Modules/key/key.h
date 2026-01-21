#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

#define KEY_NUM 1 

#define KEY_Pin GPIO_Pin_9
#define KEY_GPIO_Port GPIOB

typedef enum {
    KEY_USER = 0,
} KEY_Type_e;

typedef enum {
    KEY_RELEASED = 0,
    KEY_PRESSED
} KEY_State_e;

typedef enum {
    KEY_LOW_LEVEL_PRESS = 0,
    KEY_HIGH_LEVEL_PRESS = 1,
} KEY_PressLevel_e;

typedef enum {
    KEY_MODE_NORMAL = 0,     
    KEY_MODE_CONTINUOUS      
} KEY_Mode_e;

typedef struct {
    GPIO_TypeDef* GPIO_Port;
    uint16_t GPIO_Pin;
    KEY_PressLevel_e PressLevel; 
    KEY_Mode_e Mode;             
} KeyStaticParam_s;

typedef struct {
    KEY_State_e LastState;       
    uint32_t LastTick;           
} KeyRunningParam_s;

typedef struct {
    KeyStaticParam_s StaticParam;
    KeyRunningParam_s RunningParam;
} KEYInstance;

void KeyDeviceInit(void);
uint8_t Key_GetNum(KEY_Type_e KeyType);

#endif
