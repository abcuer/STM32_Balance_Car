#ifndef __HCSR04_H
#define __HCSR04_H

void DWT_Init(void);
// void delay_us(uint32_t us);
// float Filter_Distance(float new_value);
void HCSR04_Read(void); // 返回距离，单位：cm

#endif
