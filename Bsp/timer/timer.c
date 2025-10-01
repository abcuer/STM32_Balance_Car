#include "timer.h"

void tim2_init(void)
{
      HAL_TIM_Base_Start_IT(&htim2);                // 使能定时器2中断
}