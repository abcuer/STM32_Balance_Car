#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h" // 替换 stm32f1xx_hal.h

#define LED_NUM 3

#define Balance_GPIO_Port 				GPIOB
#define Balance_Pin 							GPIO_Pin_8
#define BlueTooth_GPIO_Port 			GPIOB
#define BlueTooth_Pin 						GPIO_Pin_15
#define Follow_GPIO_Port 				GPIOB
#define Follow_Pin 							GPIO_Pin_14

typedef enum
{
    LED_FOLLOW = 0,
    LED_BLUETOOTH,
    LED_BALANCE
} LED_Type_e;

typedef enum
{
    LED_OFF = 0,
    LED_ON
} LED_Mode_e;

typedef enum
{
    // 修改为与模式对应的数值，方便逻辑判断
    LED_LOW_LEVEL_ON = 0,
    LED_HIGH_LEVEL_ON = 1,
} LED_ActiveLevel_e;

typedef struct
{
    GPIO_TypeDef* GPIO_Port;
    uint16_t GPIO_Pin;
    LED_ActiveLevel_e ActiveLevel; 
} LedStaticParam_s;

typedef struct
{
    LED_Mode_e CurrentMode;
} LedRunningParam_s;

typedef struct
{
    LedStaticParam_s StaticParam;
    LedRunningParam_s RunningParam;
} LEDInstance;

void LedDeviceInit(void);
void SetLedMode(LED_Type_e LEDType, LED_Mode_e Mode);

#endif
