#include "headfile.h"
Motor_t motor_left = {
	.dir = 1,
	.encoder = 0
};
Motor_t motor_right = {
	.dir = 1,
	.encoder = 0
};
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


uint8_t stop_flag = 0;
void stop(void)
{
	stop_flag = 1;  // 立即设置停止标志
	DataClear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

}