#ifndef _hc06_h
#define _hc06_h
#include "stdint.h"
typedef struct
{
	uint8_t forward;
    uint8_t backward;
    uint8_t left;
    uint8_t right;
} BT_Command_t;

extern uint8_t Serial_RxData;    // 接收数据
extern uint8_t Serial_RxFlag;    // 接收标志

void Bluetooth(void);

#endif