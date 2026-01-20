#include "bsp_delay.h"
#include "stm32f10x.h"

void delay_us(uint32_t us)
{
	SysTick->LOAD = SYS_CLK * us;			//设置定时器重装值
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_ENABLE_Msk;
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
    SysTick->CTRL = 0;    // 彻底关闭
}

void delay_ms(uint32_t ms)
{
    for(uint32_t i=0;i<ms;i++) delay_us(1000);
}