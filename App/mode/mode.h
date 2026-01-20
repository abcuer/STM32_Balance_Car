#ifndef _mode_h
#define _mode_h
#include "stm32f10x.h"                  // Device header

#define LIFT_ANGLE_THRESHOLD  40.0f    // Pitch 大于这个角度判断为拿起
#define LIFT_GYRO_THRESHOLD   100       // 陀螺仪 y 轴角速度
#define PUTDOWN_ANGLE_THRESHOLD 20.0f  // Pitch 小于这个角度认为可能已放下
#define PUTDOWN_WAIT_COUNT    20       // 放下后静止时间（例如 50 * 5ms = 250ms）

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
