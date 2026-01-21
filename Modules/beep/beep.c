#include "beep.h"
#include "bsp_gpio.h" // ???????? GPIO ??

static BEEPInstance beep[BEEP_NUM];

static void UpdateBeepPinLevel(BEEP_Type_e BeepType)
{
    BEEPInstance *instance = &beep[BeepType];
    
    BitAction bitState = (instance->RunningParam.CurrentMode == (BEEP_Mode_e)instance->StaticParam.ActiveLevel) ? 
                          Bit_SET : Bit_RESET;
	
    GPIO_WriteBit(instance->StaticParam.GPIO_Port, instance->StaticParam.GPIO_Pin, bitState);
}

void SetBeepMode(BEEP_Type_e BeepType, BEEP_Mode_e Mode)
{
    if (BeepType >= BEEP_NUM) return;
    
    beep[BeepType].RunningParam.CurrentMode = Mode;
    UpdateBeepPinLevel(BeepType);
}

static void Beep_Init(BeepStaticParam_s *config, BEEP_Type_e BeepType)
{
    if (BeepType >= BEEP_NUM) return; 
    
    beep[BeepType].StaticParam = *config;
    
    GPIO_Output(config->GPIO_Port, config->GPIO_Pin, GPIO_SPEED_HIGH);
    
    SetBeepMode(BeepType, BEEP_OFF);
}

void BeepDeviceInit(void)
{ 
    BeepStaticParam_s config;
	
    config.GPIO_Port = BEEP_GPIO_Port; 
    config.GPIO_Pin = BEEP_Pin;
    config.ActiveLevel = BEEP_LOW_LEVEL_ON; 
    
    Beep_Init(&config, BEEP_SYSTEM);
}
