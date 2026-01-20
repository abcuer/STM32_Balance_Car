#include "hc_sr04.h"
#include "bsp_delay.h"

static float distance_buffer[FILTER_SIZE] = {0};  // 存储历史测距值
static uint8_t filter_index = 0;

static uint16_t time; 
uint16_t distance = 0;      	// 超声波距离值

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
static void HCSR04_Start(void)
{
	//给触发信号
	GPIO_SetBits(GPIOC, GPIO_Pin_15);
	delay_us(10);
	GPIO_ResetBits(GPIOC, GPIO_Pin_15);
}

/**
 * @brief 对测距结果进行滑动平均滤波
 * @param new_value 当前测得的原始距离值
 * @retval float 平均滤波后的距离值
 * @note 使用长度为 FILTER_SIZE 的循环缓冲区进行均值滤波，降低抖动
 */
static float Filter_Distance(float new_value)
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
	delay_us(50);
	float raw_distance = ((time * 0.0001) * 34000) / 2;
	time = 0;  //计时 返回时间计算距离
	distance = Filter_Distance(raw_distance);
}

/**
 * @brief TIM2 定时器中断服务函数（用于超声波测距计时）
 * @note 本函数与 HC-SR04 超声波模块配合使用，实现测距时间的精确计数。
 *       工作原理如下：
 *       1. 触发测距后，Echo 引脚为高电平时开始计时；
 *       2. TIM2 定时器以固定周期中断（例如 0.1us / 次），每次中断判断 Echo 是否仍为高电平；
 *       3. 若 Echo 高电平持续，则递增计数变量 Time；
 *       4. 当 Echo 变为低电平（在其他地方检测），停止计时，通过 Time 计算出声波往返时间并换算为距离；
 *       5. 同时执行声光提示计时逻辑（如蜂鸣器自动关闭等）。
 */
void TIM2_IRQHandler(void)		
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 1) time ++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		
	}
}
