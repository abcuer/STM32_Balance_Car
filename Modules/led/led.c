#include "led.h"
#include "bsp_gpio.h" // 包含你的通用底层驱动
#include <string.h>

static LEDInstance led[LED_NUM];

/**
 * @brief 根据当前状态更新GPIO电平
 */
static void UpdatePinLevel(LED_Type_e LedType)
{
    LEDInstance *instance = &led[LedType];
    
    // 逻辑：如果当前模式(ON/OFF)等于激活电平(HIGH/LOW)，则输出高电平，否则输出低电平
    // 标准库中使用 Bit_SET 和 Bit_RESET
    BitAction bitState = (instance->RunningParam.CurrentMode == (LED_Mode_e)instance->StaticParam.ActiveLevel) ? 
                          Bit_SET : Bit_RESET;
    
    // 使用标准库底层函数
    GPIO_WriteBit(instance->StaticParam.GPIO_Port, instance->StaticParam.GPIO_Pin, bitState);
}

/**
 * @brief 设置LED状态
 */
void SetLedMode(LED_Type_e LedType, LED_Mode_e Mode)
{
    if (LedType >= LED_NUM) return;
    led[LedType].RunningParam.CurrentMode = Mode;
    UpdatePinLevel(LedType);
}

static void Led_Init(LedStaticParam_s *config, LED_Type_e LedType)
{
    if (LedType >= LED_NUM) return; 
    
    // 1. 复制配置
    led[LedType].StaticParam = *config;
    
    // 2. 调用 bsp_gpio 的通用初始化函数
    // 内部会自动处理 RCC 时钟开启
    GPIO_Output(config->GPIO_Port, config->GPIO_Pin, GPIO_SPEED_HIGH);
    
    // 3. 初始状态关闭
    SetLedMode(LedType, LED_OFF);
}

void LedDeviceInit(void)
{ 
    LedStaticParam_s config;

    // 注意：这里的 Balance_GPIO_Port 等宏应在 bsp_gpio.h 或专门的 pin_define.h 中定义
    config.GPIO_Port = Balance_GPIO_Port; // 示例
    config.GPIO_Pin =  Balance_Pin;
    config.ActiveLevel = LED_HIGH_LEVEL_ON;
    Led_Init(&config, LED_BALANCE);

    config.GPIO_Port = BlueTooth_GPIO_Port;
    config.GPIO_Pin = BlueTooth_Pin;
    config.ActiveLevel = LED_HIGH_LEVEL_ON;
    Led_Init(&config, LED_BLUETOOTH);

    config.GPIO_Port = Follow_GPIO_Port;
    config.GPIO_Pin = Follow_Pin;
    config.ActiveLevel = LED_HIGH_LEVEL_ON;
    Led_Init(&config, LED_FOLLOW);
}