#include "hcsr04.h"

#define FILTER_SIZE 5
float distance_buffer[FILTER_SIZE] = {0};
uint8_t filter_index = 0;

void DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // 允许 DWT
    DWT->CYCCNT = 0;                               // 清零计数器
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;           // 使能计数
}

static void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000);
    while ((DWT->CYCCNT - start) < ticks);
}

// 滑动平均滤波
static float Filter_Distance(float new_value)
{
    distance_buffer[filter_index] = new_value;
    filter_index = (filter_index + 1) % FILTER_SIZE;

    float sum = 0;
    for (int i = 0; i < FILTER_SIZE; i++)
        sum += distance_buffer[i];

    return sum / FILTER_SIZE;
}

/**
 * @brief 测量距离（阻塞方式）
 * @retval 平均滤波后的距离值（cm），异常返回 0
 */
float HCSR04_Read(void)
{
    uint32_t timeout = 0;

    // 触发
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);

    // 等待 ECHO 上升沿
    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == GPIO_PIN_RESET) {
        if (timeout++ > 30000) return 0;
    }

    uint32_t start = DWT->CYCCNT;  // 记录上升沿时间

    // 等待 ECHO 下降沿
    timeout = 0;
    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == GPIO_PIN_SET) {
        if (timeout++ > 60000) return 0;
    }

    uint32_t end = DWT->CYCCNT;    // 记录下降沿时间

    // 计算脉宽时间（us）
    uint32_t cycles = end - start;
    float time_us = (float)cycles / (HAL_RCC_GetHCLKFreq() / 1000000.0f);

    // 换算距离（cm），声速 340m/s≈0.034 cm/us
    float distance = (time_us * 0.034f) / 2.0f;

    return Filter_Distance(distance);
}
