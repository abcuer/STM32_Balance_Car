#include "motor.h"

void motor_init(void)
{
    // 启动PWM输出
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // 左电机 PWM
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); // 右电机 PWM

    // 默认停止
    HAL_GPIO_WritePin(GPIOA, Left_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, Left_IN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, Right_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, Right_IN2_Pin, GPIO_PIN_RESET);
}


// 设置左轮占空比 duty ∈ [-ARR, ARR]
void angle_left_duty(int16_t duty)
{
    if (duty >= 0)
    {
        HAL_GPIO_WritePin(GPIOA, Left_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, Left_IN2_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOA, Left_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, Left_IN2_Pin, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, -duty);
    }
}

// 设置右轮占空比 duty ∈ [-ARR, ARR]
void angle_right_duty(int16_t duty)
{
    if (duty >= 0)
    {
        HAL_GPIO_WritePin(GPIOB, Right_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, Right_IN2_Pin, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, duty);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, Right_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, Right_IN2_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, -duty);
    }
}

void motor_duty(int16_t dutyL, int16_t dutyR)
{
    angle_left_duty(dutyL);
    angle_right_duty(dutyR);
}