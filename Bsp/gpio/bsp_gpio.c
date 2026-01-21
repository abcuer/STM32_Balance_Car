#include "bsp_gpio.h"

/**
 * @brief  内部私有函数：根据 GPIOx 自动开启或关闭对应的 RCC 时钟
 */
void GPIO_SetClock(GPIO_TypeDef* GPIOx, FunctionalState state)
{
    uint32_t RCC_Periph;

    if      (GPIOx == GPIOA) RCC_Periph = RCC_APB2Periph_GPIOA;
    else if (GPIOx == GPIOB) RCC_Periph = RCC_APB2Periph_GPIOB;
    else if (GPIOx == GPIOC) RCC_Periph = RCC_APB2Periph_GPIOC;
    else if (GPIOx == GPIOD) RCC_Periph = RCC_APB2Periph_GPIOD;
    else if (GPIOx == GPIOE) RCC_Periph = RCC_APB2Periph_GPIOE;
    else return;

    RCC_APB2PeriphClockCmd(RCC_Periph, state);
}

/**
 * @brief  通用的推挽输出初始化
 * @param  GPIOx: 端口 (GPIOA~G)
 * @param  GPIO_Pin: 引脚 (GPIO_Pin_0~15)
 * @param  speed: 速率
 */
void GPIO_Output(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_Speed_e speed)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. 自动开启时钟
    GPIO_SetClock(GPIOx, ENABLE);

    // 2. 配置参数
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; // 标准推挽输出
    GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)speed;
    
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

/**
 * @brief  通用的输入初始化
 * @param  mode: GPIO_Mode_IPU (上拉), GPIO_Mode_IPD (下拉), GPIO_Mode_IN_FLOATING (浮空)
 */
void GPIO_Input(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_SetClock(GPIOx, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = mode;
    // 输入模式下无需配置 GPIO_Speed
    
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}