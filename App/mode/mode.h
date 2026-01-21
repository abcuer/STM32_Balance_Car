#ifndef _mode_h
#define _mode_h
#include "stm32f10x.h"                  // Device header

typedef struct {
    uint8_t lifted_flag;       // 提起标志位：1 表示小车被提起，0 表示正常运行
    uint16_t putdown_counter;  // 放下计数器
    uint16_t lifted_counter;   // 提起计数器
    uint8_t balance_enable;    // 平衡控制使能：1 开启，0 暂停
	uint8_t mode;
} BalanceState_t;

void ModeSelect(void);
void Balance(void);
void CheckLiftState(void);
void DetectPutDown(void);
void CheckFallDown(void);
void ObstacleAvoid(void);

#endif
