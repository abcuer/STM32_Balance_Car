#ifndef _mode_h
#define _mode_h

void System_Init(void);
void Balance(void);
void ModeSelect(void);
void checkLiftState(void);
void detectPutDown(void);
void checkFallDown(void);
void Bluetooth(void);
void ObstacleAvoid(void);
void SoundLight(void);
void UpdateSoundLight(void);


typedef struct
{
	float yaw;
	float pitch;
	float roll;
}Euler_t;

typedef struct
{
	short x;
	short y;
	short z;
}Gyro_t;

typedef struct{
	uint8_t flag;
	uint16_t time;
} SoundLight_t;

typedef struct {
    uint8_t lifted_flag;       // 提起标志位：1 表示小车被提起，0 表示正常运行
    uint16_t putdown_counter;  // 放下计数器
    uint16_t lifted_counter;   // 提起计数器
    uint8_t balance_enable;    // 平衡控制使能：1 开启，0 暂停
} BalanceState_t;


#endif
