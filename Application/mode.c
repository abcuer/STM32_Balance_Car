#include "headfile.h"

#define LIFT_ANGLE_THRESHOLD  40.0f    // Pitch 大于这个角度判断为拿起
#define LIFT_GYRO_THRESHOLD   100       // 陀螺仪 y 轴角速度
#define PUTDOWN_ANGLE_THRESHOLD 20.0f  // Pitch 小于这个角度认为可能已放下
#define PUTDOWN_WAIT_COUNT    20       // 放下后静止时间（例如 50 * 5ms = 250ms）

uint8_t bluetooth_flag = 0;    
// 蓝牙控制激活标志：用于判断当前是否接收到蓝牙遥控指令（非零则表示正在遥控）
uint16_t distance = 0;         
// 当前测得的超声波距离值（单位：cm），用于跟随/避障控制逻辑
uint8_t mode = 0;              
// 当前工作模式编号：0=平衡模式，1=蓝牙遥控，2=超声波跟随

BalanceState_t balance_state = {
    .lifted_flag = 0, 			// 提起标志位：1 表示小车被提起，0 表示正常运行
    .putdown_counter = 0,		// 放下计数器
    .lifted_counter = 0,		// 提起计数器
    .balance_enable = 1,		// 平衡控制使能：1 开启，0 暂停
};

SoundLight_t sound_light = {
	.flag = 0, // 声光提示激活标志：1 表示正在进行声光报警，如避障蜂鸣器提醒；0 表示无报警
	.time = 0  // 声光提示计时器：记录声光报警的持续时间，到达设定值后自动关闭蜂鸣器等提示
};
 
Euler_t euler;
Gyro_t gyro;

/* 直立环 */
UprightPID_t upright_pid = {
    .kp = 270*0.6,
    .kd = 1*0.6,
	.out = 0,
    .med_angle = -5.3
};

/* 速度环 */
SpeedPID_t speed_pid = {
	.kp = -0.58,
	.ki = -0.58/200,
	.out = 0,
	.filter = 0.7,
	.speed = 0  /* 前进 后退 */
};

/* 转向环 */
TurnPID_t turn_pid = {
	.kd = 0.5,
	/* 左右移动 */
	.kp = -35,
	.out = 0,
	.speed = 0 
};

float pwm_out, PWMA, PWMB = 0;

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
	MPU6050_DMP_Get_Data(&euler.pitch, &euler.roll, &euler.yaw);
	MPU_Get_Gyroscope(&gyro.x, &gyro.y, &gyro.z);
	
	if (balance_state.balance_enable) 					// 默认平衡模式
	{
		ModeSelect();

		upright_pid.out = angle_pid_control(upright_pid.med_angle, euler.pitch, gyro.y);
		speed_pid.out = speed_pid_control(speed_pid.filter, speed_pid.speed);
		turn_pid.out = turn_pid_control(gyro.z);
		
		pwm_out = upright_pid.out - upright_pid.kp * speed_pid.out;
		PWMA = pwm_out - turn_pid.out;
		PWMB = pwm_out + turn_pid.out;

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
		speed_pid.kp = -0.58;
		speed_pid.ki = -0.58/200;
		Balance_ON(); 
	}
	else Balance_OFF();  
	if(mode == 1)  //蓝牙遥控模式
	{
		speed_pid.kp = -0.55;
		speed_pid.ki = 0;
		BlueTooth_ON();
	}
	else BlueTooth_OFF();
	if(mode == 2)											// 超声波跟随
	{
		speed_pid.kp = -0.5;
		speed_pid.ki = -0.5/200;

		Follow_ON(); 
		if(distance > 0 && distance <= 80)
		{
			dist_pid_control(); 
		}
		else
		{
			speed_pid.speed = 0;  // 停止移动，避免无效距离导致继续前进
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
    if (fabs(euler.pitch) > LIFT_ANGLE_THRESHOLD && abs(gyro.y) > LIFT_GYRO_THRESHOLD && motor_right.encoder > 50)
	{
		balance_state.lifted_counter++;
		if (balance_state.lifted_counter > 30)
		{
			balance_state.lifted_flag = 1;
			balance_state.balance_enable = 0;
			balance_state.lifted_counter = 0;
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
    if (balance_state.lifted_flag || stop_flag)
    {
        if (fabs(euler.pitch) < 20 && abs(gyro.y) < 150 && abs(motor_right.encoder) < 120)
        {
            if (balance_state.putdown_counter++ > PUTDOWN_WAIT_COUNT)
            {
				balance_state.lifted_flag = 0;
				balance_state.putdown_counter = 0;
				balance_state.balance_enable = 1; 
				stop_flag = 0;           // 清除紧急停止标志
            }
        }
        else
        {
            balance_state.putdown_counter = 0;
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
	if (fabs(upright_pid.med_angle - euler.pitch) > 70 && stop_flag == 0)			// 倒地检测
	{
		balance_state.balance_enable = 0;
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
	if(bt_cmd.forward || bt_cmd.backward || bt_cmd.left || bt_cmd.right)  bluetooth_flag = 1;
	else  bluetooth_flag = 0;
	/* 蓝牙控制 */
	if(bt_cmd.forward == 1 && bt_cmd.backward == 0) 		speed_pid.speed += 1;
	else if(bt_cmd.backward == 1 && bt_cmd.forward == 0)    speed_pid.speed -= 1;
	else if(bt_cmd.forward == 0 && bt_cmd.backward == 0) 	speed_pid.speed = 0;
	
	if(bt_cmd.left == 1 && bt_cmd.right == 0)				speed_pid.speed -= 1; 
	else if(bt_cmd.right == 1 && bt_cmd.left == 0) 			speed_pid.speed += 1;
	else if(bt_cmd.left == 0 && bt_cmd.right == 0)			speed_pid.speed = 0; 
	
	if(bt_cmd.forward && bt_cmd.left && bt_cmd.backward == 0 && bt_cmd.right == 0)
	{
		speed_pid.speed += 0.3;  turn_pid.speed += 1.3; 
	}
	else if(bt_cmd.forward && bt_cmd.right && bt_cmd.backward == 0 && bt_cmd.left == 0)
	{
		speed_pid.speed += 0.4;  turn_pid.speed -= 1.2;
	}
	else if(bt_cmd.backward && bt_cmd.left && bt_cmd.forward == 0 && bt_cmd.right == 0)
	{
		speed_pid.speed -= 0.4;	 turn_pid.speed += 1.2; 
	}
	else if(bt_cmd.backward && bt_cmd.right && bt_cmd.forward == 0 && bt_cmd.left == 0)
	{
		speed_pid.speed -= 0.2;	 turn_pid.speed = 1.4;
	}
	if(bt_cmd.forward == 0 && bt_cmd.backward == 0 && bt_cmd.left == 0 && bt_cmd.right == 0)
	{
		speed_pid.speed = 0; 
		turn_pid.speed = 0;
	}
	
	if(speed_pid.speed >= MAX_Speed) 			speed_pid.speed = MAX_Speed;
	else if(speed_pid.speed <= -MAX_Speed) 	    speed_pid.speed = -MAX_Speed;
	if(turn_pid.speed >= MAX_Turn) 				turn_pid.speed = MAX_Turn;
	else if(turn_pid.speed <= -MAX_Turn) 		turn_pid.speed = -MAX_Turn;
	/* 运动约束 */
	if (bluetooth_flag)
	{
		speed_pid.ki = 0;       // 关闭回原位置积分
		turn_pid.kd = 0;
	}
	else
	{
		turn_pid.kd = 0.5;
	}
}

/**
 * @brief 声光提示启动函数（例如避障触发后报警）
 * @param 无
 * @retval 无
 */
void SoundLight(void)
{
	if(sound_light.flag == 0)
	{
		Buzzer_ON();
		Follow_ON();
		sound_light.flag = 1;
	}
}

/**
 * @brief 声光提示状态更新函数，计时并关闭报警
 * @param 无
 * @retval 无
 */
void UpdateSoundLight(void)
{
    if(sound_light.flag)
    {
        sound_light.time++;

		if(sound_light.time >= 20) 
		{
			Buzzer_OFF();
			Follow_OFF();
			sound_light.time = 0;
			sound_light.flag = 0; 
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
