#include "headfile.h"

#define LIFT_ANGLE_THRESHOLD  40.0f    // Pitch 大于这个角度判断为拿起
#define LIFT_GYRO_THRESHOLD   100       // 陀螺仪 y 轴角速度
#define PUTDOWN_ANGLE_THRESHOLD 20.0f  // Pitch 小于这个角度认为可能已放下
#define PUTDOWN_WAIT_COUNT    20       // 放下后静止时间（例如 50 * 5ms = 250ms）

uint8_t lifted_flag = 0;       
// 提起标志位：1 表示小车被提起，停止平衡控制；0 表示正常运行

uint16_t putdown_counter = 0;  
// 放下计数器：检测放下后是否静止一定时间，满足条件后重新启用平衡控制

uint16_t lifted_counter = 0;   
// 提起计数器：用于判断提起状态是否持续达到设定时间，以确认确实被提起

uint8_t balance_enable = 1;    
// 平衡控制使能标志：1 表示开启平衡控制（即允许运行 PID 控制）；0 表示暂停控制输出（如倒地/被提起）

uint8_t bluetooth_flag = 0;    
// 蓝牙控制激活标志：用于判断当前是否接收到蓝牙遥控指令（非零则表示正在遥控）

uint16_t distance = 0;         
// 当前测得的超声波距离值（单位：cm），用于跟随/避障控制逻辑

uint8_t mode = 0;              
// 当前工作模式编号：0=平衡模式，1=蓝牙遥控，2=超声波跟随

uint8_t SoundLight_flag = 0;   
// 声光提示激活标志：1 表示正在进行声光报警，如避障蜂鸣器提醒；0 表示无报警

uint8_t SoundLight_time = 0;   
// 声光提示计时器：记录声光报警的持续时间，到达设定值后自动关闭蜂鸣器等提示


float Pitch, Roll, Yaw;
short gx,gy,gz;
/* 直立环 */
float Med_angle = -5.3;  //机械中值
float angle_kp = 270*0.6;
float angle_kd = 1*0.6;

/* 速度环 */
float filter = 0.7;
float speed_kp = -0.58;
float speed_ki = -0.58/200;

/* 前进 后退 */
float speed_tar = 0;

/* 转向环 */
float turn_kd = 0.5;

/* 左右移动 */

float turn_kp = -35;
float turn_speed = 0;

float angle_out, speed_out, turn_out = 0;
float PWM_out, PWMA, PWMB = 0;

/**
 * @brief 系统初始化函数，初始化所有模块和外设
 * @param 无
 * @retval 无
 */
void System_Init(void)
{
	mpu6050_Init();	
	MPU6050_DMP_Init();
	MPU6050_EXTI_Init();
	Delay_ms(300);
	Key_Init();
	LED_Init();
	OLED_Init();		
	PWM_Init();
	Motor_Init();
	encoder_left_Init();
	encoder_right_Init();
	HCSR04_Init();
	UART2_Init(115200);
	Timer_Init();
	Buzzer_Init();	
	pid_init(&dist, POSITION_PID, -0.6, 0, 0.1); 
}

/**
 * @brief 平衡控制主函数，DMP读取姿态并计算三环PID，控制小车直立
 * @param 无
 * @retval 无
 */
void Balance(void)
{
	MPU6050_DMP_Get_Data(&Pitch, &Roll, &Yaw);
	MPU_Get_Gyroscope(&gx, &gy, &gz);
	
	if (balance_enable) 					// 默认平衡模式
	{
		ModeSelect();

		angle_out = angle_pid_control(Med_angle, Pitch, gy);
		speed_out = speed_pid_control(filter, speed_tar);
		turn_out = turn_pid_control(gz);
		
		PWM_out = angle_out - angle_kp * speed_out;
		PWMA = PWM_out - turn_out;
		PWMB = PWM_out + turn_out;

		Limit(PWMA, PWMB);
		motor_duty(PWMA, PWMB);
	} 
}

/**
 * @brief 模式选择与切换函数，根据按键切换运行模式（平衡、蓝牙、跟随）
 * @param 无
 * @retval 无
 * @note 会根据当前模式调整PID参数和控制逻辑
 */
void ModeSelect(void)
{
	static uint8_t last_mode = 0xFF;  // 初始化为一个不可能的模式值
	if(Key_GetNum())
	{
		mode++;
		mode %= 3;
	}
	// 仅当模式发生变化时清除数据
    if (mode != last_mode)
    {
        DataClear();
        last_mode = mode;  // 更新记录
    }
	
	if(mode == 0) 											//平衡模式
	{
		speed_kp = -0.58;
		speed_ki = -0.58/200;
		Balance_ON(); 
	}
	else Balance_OFF();  
	if(mode == 1)  //蓝牙遥控模式
	{
		speed_kp = -0.55;
		speed_ki = 0;
		BlueTooth_ON();
	}
	else 
	{
		BlueTooth_OFF();
	}
	if(mode == 2)											// 超声波跟随
	{
		speed_kp = -0.5;
		speed_ki = -0.5/200;

		Follow_ON(); 
		if(distance > 0 && distance <= 80)
		{
			dist_pid_control(); 
		}
		else
		{
			speed_tar = 0;  // 停止移动，避免无效距离导致继续前进
		}
	}
	else Follow_OFF(); 
	ObstacleAvoid();					// 蓝牙避障检测
}	

/**
 * @brief 提起检测：检测是否被提起
 * @param 无
 * @retval 无
 * @note 根据Pitch角度和陀螺仪Y轴速度判断，触发停止控制
 */
void checkLiftState(void)
{
    // 拿起条件：角度大且角速度较大，持续一定时间
    if (fabs(Pitch) > LIFT_ANGLE_THRESHOLD && abs(gy) > LIFT_GYRO_THRESHOLD && Encoder_right > 50)
	{
		lifted_counter++;
		if (lifted_counter > 30)
		{
			lifted_flag = 1;
			balance_enable = 0;
			lifted_counter = 0;
			stop();
		}
	}
}

/**
 * @brief 着陆检测：检测是否已经放下
 * @param 无
 * @retval 无
 * @note 如果Pitch角度小于阈值且静止一段时间，恢复平衡控制
 */
void detectPutDown(void)
{
    if (lifted_flag || stop_flag)
    {
        if (fabs(Pitch) < 20 && abs(gy) < 150 && abs(Encoder_right) < 120)
        {
            if (putdown_counter++ > PUTDOWN_WAIT_COUNT)
            {
				lifted_flag = 0;
				putdown_counter = 0;
				stop_flag = 0;           // 清除紧急停止标志
				balance_enable = 1; 
            }
        }
        else
        {
            putdown_counter = 0;
        }
    }
}

/**
 * @brief 倒地检测函数
 * @param 无
 * @retval 无
 * @note 若倾斜角度超过设定阈值，则关闭平衡控制并停止电机
 */
void checkFallDown(void)
{
	if (fabs(Med_angle - Pitch) > 70 && stop_flag == 0)			// 倒地检测
	{
		balance_enable = 0;
		stop();
	}   
}
 
/**
 * @brief 蓝牙遥控模式控制逻辑
 * @param 无
 * @retval 无
 * @note 根据前后左右命令调整目标速度与转向速度
 */
void Bluetooth(void)
{
	if(straight || back || left || right)  bluetooth_flag = 1;
	else  bluetooth_flag = 0;
	/* 蓝牙控制 */
	if(straight == 1 && back == 0) 		speed_tar+=1;
	else if(back == 1 && straight == 0) speed_tar-=1;
	else if(straight == 0 && back == 0) speed_tar = 0;
	
	if(left == 1 && right == 0)			turn_speed-=1; 
	else if(right == 1 && left == 0) 	turn_speed+=1;
	else if(right == 0 && left == 0)	turn_speed = 0; 
	
	if(straight && left && back == 0 && right == 0)
	{
		speed_tar+=0.3;  turn_speed+=1.3; 
	}
	else if(straight && right && back == 0 && left == 0)
	{
		speed_tar+=0.4;  turn_speed-=1.2;
	}
	else if(back && left && straight == 0 && right == 0)
	{
		speed_tar-=0.4;	turn_speed+=1.2; 
	}
	else if(back && right && straight == 0 && left == 0)
	{
		speed_tar-=0.2;	turn_speed=1.4;
	}
	if(straight == 0 && back == 0 && left == 0 && right == 0)
	{
		speed_tar = 0; 
		turn_speed = 0;
	}
	
	if(speed_tar >= MAX_Speed) 			speed_tar = MAX_Speed;
	else if(speed_tar <= -MAX_Speed) 	speed_tar = -MAX_Speed;
	if(turn_speed >= MAX_Turn) 			turn_speed = MAX_Turn;
	else if(turn_speed <= -MAX_Turn) 	turn_speed = -MAX_Turn;
	/* 运动约束 */
	if (bluetooth_flag)
	{
		speed_ki = 0;       // 关闭回原位置积分
		turn_kd = 0;
	}
	else
	{
		turn_kd = 0.5;
	}
}

/**
 * @brief 声光提示启动函数（例如避障触发后报警）
 * @param 无
 * @retval 无
 */
void SoundLight(void)
{
	if(SoundLight_flag == 0)
	{
		Buzzer_ON();
		Follow_ON();
		SoundLight_flag = 1;
	}
}

/**
 * @brief 声光提示状态更新函数，计时并关闭报警
 * @param 无
 * @retval 无
 */
void UpdateSoundLight(void)
{
    if(SoundLight_flag)
    {
        SoundLight_time++;

		if(SoundLight_time >= 20) 
		{
			Buzzer_OFF();
			Follow_OFF();
			SoundLight_time = 0;
			SoundLight_flag = 0; 
		}
        
    }
}

/**
 * @brief 蓝牙避障逻辑
 * @param 无
 * @retval 无
 * @note 距离过近则触发声光报警并禁止运动，距离恢复后解除限制
 */
uint8_t obstacle_blocked = 0;  // 是否被障碍物拦住

void ObstacleAvoid(void)
{
	HCSR04_GetValue();
	OLED_ShowNum(1, 7, distance, 3);
	if(mode == 1)
	{
		if (!obstacle_blocked) Bluetooth();  // 正常蓝牙控制
		if(distance > 0 && distance <= 80)
		{
			if(!obstacle_blocked && distance < 25.0f) 
			{
				SoundLight();
				obstacle_blocked = 1;
			}
			else if (obstacle_blocked && distance > 60.0f)
			{
				obstacle_blocked = 0; // 恢复触发能力
			}
			
		}
	}
}
