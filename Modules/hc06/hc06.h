#ifndef _BT_h
#define _BT_h

#include "stdint.h"

#define MAX_Speed 24
#define MAX_Turn 20


typedef struct
{
	uint8_t forward;
    uint8_t backward;
    uint8_t left;
    uint8_t right;
} BT_Command_t;

typedef struct
{
    uint8_t data;
    uint8_t flag;
} RX_DATA_t;

void BlueTooth(void);

#endif
