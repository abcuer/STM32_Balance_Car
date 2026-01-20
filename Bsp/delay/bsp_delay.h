#ifndef _BSP_DELAY_H
#define _BSP_DELAY_H

#include "stdint.h"

#define SYS_CLK 72

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
#endif

