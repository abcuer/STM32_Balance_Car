#ifndef _encoder_h
#define _encoder_h
#include "headfile.h"

void encoder_init(void);
int16_t Encoder_left_Get(void);
int16_t Encoder_right_Get(void);
void UpdateEncoderCounts(void);
#endif