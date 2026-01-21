#ifndef _BEEP_H
#define _BEEP_H

#include "stm32f10x.h" // ?? stm32f1xx_hal.h

#define BEEP_GPIO_Port GPIOC
#define BEEP_Pin GPIO_Pin_13


#define BEEP_NUM 1

typedef enum
{
    BEEP_SYSTEM = 0,
} BEEP_Type_e;

typedef enum
{
    BEEP_OFF = 0,
    BEEP_ON
} BEEP_Mode_e;

typedef enum
{
    BEEP_LOW_LEVEL_ON = 0,
    BEEP_HIGH_LEVEL_ON = 1, 
} BEEP_ActiveLevel_e; 

typedef struct
{
    GPIO_TypeDef* GPIO_Port;
    uint16_t GPIO_Pin;
    BEEP_ActiveLevel_e ActiveLevel; 
} BeepStaticParam_s;

typedef struct
{
    BEEP_Mode_e CurrentMode;
} BeepRunningParam_s;

typedef struct
{
    BeepStaticParam_s StaticParam;
    BeepRunningParam_s RunningParam;
} BEEPInstance;

void BeepDeviceInit(void);
void SetBeepMode(BEEP_Type_e BeepType, BEEP_Mode_e Mode);

#endif
