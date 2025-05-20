#include "stm32f10x.h"                  // Device header
#include "headfile.h"

#define offset 0
uint8_t stop_flag = 0;

void Motor_Run_Init(void)
{
	PWM_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/*
左轮: PA4PA5   PWM1(PA6)
右轮: PB0 PB1 PWM2(PA7)
*/
uint8_t motorA_dir = 1;
uint8_t motorB_dir = 1;

/*左轮调速*/
void Speed_L(uint16_t Speed_l)
{
	TIM_SetCompare1(TIM3, Speed_l);
}
/*右轮调速*/
void Speed_R(uint16_t Speed_r)
{
	TIM_SetCompare2(TIM3, Speed_r);
}

//void speed_left_duty(int16_t duty)
//{
//	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction) motorA_dir);
//	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction) !motorA_dir);
//	Speed_L(duty);
//}
//void speed_right_duty(int16_t duty)
//{
//	GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction) !motorB_dir);
//	GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction) motorB_dir);
//	Speed_R(duty);
//}

void angle_left_duty(int16_t duty) 
{
	if(duty >= 0)
	{
		motorA_dir = 1;
	}
	else
	{
		motorA_dir = 0;
		duty = -duty;
	}
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction) motorA_dir);
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction) !motorA_dir);
	Speed_L(duty + offset);
}

void angle_right_duty(int16_t duty) 
{
	if(duty >= 0)
	{
		motorB_dir = 1;
	}
	else
	{
		motorB_dir = 0;
		duty = -duty;
	}
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction) !motorB_dir);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction) motorB_dir);
	Speed_R(duty + offset);
}
void motor_duty(float PWMA, float PWMB)
{
	angle_left_duty(PWMA);
	angle_right_duty(PWMB);
}
#define LIFT_ANGLE_THRESHOLD  20.0f    // Pitch 大于这个角度判断为拿起
#define LIFT_GYRO_THRESHOLD   100       // 陀螺仪 y 轴角速度
#define PUTDOWN_ANGLE_THRESHOLD 20.0f  // Pitch 小于这个角度认为可能已放下
#define PUTDOWN_WAIT_COUNT    20       // 放下后静止时间（例如 50 * 5ms = 250ms）

uint8_t lifted_flag = 0;
uint16_t putdown_counter = 0;
uint16_t lifted_counter = 0;
uint8_t balance_enable = 1;
/**
 * 判断是否拿起
 */
void checkLiftState(void)
{
    // 拿起条件：角度大且角速度较大，持续一定时间
    if (fabs(Pitch) > LIFT_ANGLE_THRESHOLD && abs(gy) > LIFT_GYRO_THRESHOLD && Encoder_right > 50)
	{
		lifted_counter++;
		if (lifted_counter > 30)
		{
			lifted_flag = 1;
			balance_enable = 0;
			lifted_counter = 0;
			stop_flag = 1;
			stop();
		}
	}
}

/**
 * 判断是否放下
 */
void detectPutDown(void)
{
    if (lifted_flag || stop_flag)
    {
        if (fabs(Pitch) < 20 && abs(gy) < 150 && abs(Encoder_right) < 120)
        {
            if (putdown_counter++ > PUTDOWN_WAIT_COUNT)
            {
				lifted_flag = 0;
				putdown_counter = 0;
				stop_flag = 0;           // ? 清除紧急停止标志
				balance_enable = 1; 
            }
        }
        else
        {
            putdown_counter = 0;
        }
    }
}

void stop(void)
{
	stop_flag = 1;  // 立即设置停止标志
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction) 1);
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction) 1);
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction) 1);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction) 1);
}

