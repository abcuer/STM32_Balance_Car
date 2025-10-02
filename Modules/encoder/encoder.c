#include "encoder.h"

void encoder_init(void)
{
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1); // 开启编码器A
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_2); 	
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_1); // 开启编码器B	
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_2); 
}


// 左轮编码器（TIM4）
 int16_t Encoder_left_Get(void)
{
    int16_t temp;
    temp = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);   // 读取计数
    __HAL_TIM_SET_COUNTER(&htim4, 0);                // 清零
    return temp;
}

// 右轮编码器（TIM1）
int16_t Encoder_right_Get(void)
{
    int16_t temp;
    temp = (int16_t)__HAL_TIM_GET_COUNTER(&htim1);
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    return -temp;
}
void UpdateEncoderCounts(void)
{
    motor_left.encoder  = Encoder_left_Get();
    motor_right.encoder = Encoder_right_Get();
}