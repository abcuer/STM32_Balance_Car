#include "headfile.h"

#define FILTER_SIZE 5  // 滑动窗口大小

float distance_buffer[FILTER_SIZE] = {0};  // 存储历史测距值
uint8_t filter_index = 0;
uint16_t Time; //记录时间


void HCSR04_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//开启GPIOA的时钟
	/*GPIO初始化*/ 
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//将PC15 TRIG
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//将PC14 ECHO

	GPIO_ResetBits(GPIOC, GPIO_Pin_15);
}

/**
 * @brief 启动 HC-SR04 超声波测距模块
 * @note 通过给 Trig 引脚发送一个 10us 的高电平脉冲，触发一次测距
 */
void HCSR04_Start(void)
{
	//给触发信号
	GPIO_SetBits(GPIOC, GPIO_Pin_15);
	Delay_us(10);
	GPIO_ResetBits(GPIOC, GPIO_Pin_15);
}

/**
 * @brief 对测距结果进行滑动平均滤波
 * @param new_value 当前测得的原始距离值
 * @retval float 平均滤波后的距离值
 * @note 使用长度为 FILTER_SIZE 的循环缓冲区进行均值滤波，降低抖动
 */
float Filter_Distance(float new_value)
{
    distance_buffer[filter_index] = new_value;  // 存入当前值
    filter_index = (filter_index + 1) % FILTER_SIZE;  // 更新索引（循环）

    float sum = 0;
    for (int i = 0; i < FILTER_SIZE; i++)
    {
        sum += distance_buffer[i];
    }

    return sum / FILTER_SIZE;  // 返回平均值
}

/**
 * @brief 获取一次距离测量值并滤波
 * @note 流程：
 *       1. 触发一次超声波测距
 *       2. 根据回响时间 Time 计算距离（单位：cm）
 *       3. 使用滤波函数平滑输出
 */
void HCSR04_GetValue(void)
{
	HCSR04_Start();
	Delay_us(50);
	float raw_distance = ((Time * 0.0001) * 34000) / 2;
	Time = 0;  //计时 返回时间计算距离
	distance = Filter_Distance(raw_distance);
}
