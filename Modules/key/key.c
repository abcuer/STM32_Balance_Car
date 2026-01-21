#include "key.h"
#include "bsp_gpio.h" // ????? GPIO ??
#include <string.h>

static KEYInstance key[KEY_NUM];

static KEY_State_e GetKeyState(KEY_Type_e KeyType)
{
    KEYInstance *ins = &key[KeyType];
    
    // ?????????????
    uint8_t pinState = GPIO_ReadInputDataBit(ins->StaticParam.GPIO_Port, ins->StaticParam.GPIO_Pin);
    
    // ????????????,??????
    return (pinState == (uint8_t)ins->StaticParam.PressLevel) ? KEY_PRESSED : KEY_RELEASED;
}

static void Key_Init(KeyStaticParam_s *config, KEY_Type_e KeyType)
{
    if (KeyType >= KEY_NUM) return;
    
    key[KeyType].StaticParam = *config;
    
    GPIOMode_TypeDef inputMode = (config->PressLevel == KEY_LOW_LEVEL_PRESS) ? GPIO_Mode_IPU : GPIO_Mode_IPD;
    GPIO_Input(config->GPIO_Port, config->GPIO_Pin, inputMode);
    
    key[KeyType].RunningParam.LastState = KEY_RELEASED;
    key[KeyType].RunningParam.LastTick = 0;
}

void KeyDeviceInit(void)
{
    KeyStaticParam_s config;

    config.GPIO_Port = KEY_GPIO_Port; 
    config.GPIO_Pin = KEY_Pin; 
    config.PressLevel = KEY_LOW_LEVEL_PRESS; 
    config.Mode = KEY_MODE_NORMAL;
    Key_Init(&config, KEY_USER);
}

uint8_t Key_GetNum(KEY_Type_e KeyType)
{
    if (KeyType >= KEY_NUM) return 0;
    
    KEYInstance *instance = &key[KeyType];
    KEY_State_e currentState = GetKeyState(KeyType);
    uint8_t key_event = 0;
    
    if (currentState == KEY_PRESSED && instance->RunningParam.LastState == KEY_RELEASED)
    {
        if (GetKeyState(KeyType) == KEY_PRESSED)
        {
            key_event = 1;
        }
    }
    
    instance->RunningParam.LastState = currentState;
    return key_event;
}