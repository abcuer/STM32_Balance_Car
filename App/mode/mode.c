#include "headfile.h"

BalanceState_t balance_state = {
    .lifted_flag = 0, 			// 提起标志位：1 表示小车被提起，0 表示正常运行
    .putdown_counter = 0,		// 放下计数器
    .lifted_counter = 0,		// 提起计数器
    .balance_enable = 1,		// 平衡控制使能：1 开启，0 暂停
	.mode = 0
};

float pwm_out, PWMA, PWMB = 0;
static uint8_t obstacle_blocked = 0;  // 是否被障碍物拦住


/**
 * @brief 平衡控制主函数，DMP读取姿态并计算三环PID，控制小车直立
 * @param 无
 * @retval 无
 */
void Balance(void)
{
	if (balance_state.balance_enable) 					// 默认平衡模式
	{
		if(balance_state.mode == 2)
		{
			if(distance > 5 && distance <= 80)	DistPidCtrl(); 
			else	speed_pid.speed = 0; 
		}
		upright_pid.out = AnglePidCtrl(upright_pid.med_angle, mpu.pitch, mpu.gyro[1]);
		speed_pid.out = SpeedPidCtrl(speed_pid.filter, speed_pid.speed);
//		turn_pid.out = TurnPidCtrl(mpu.gyro[2]);
		
		pwm_out = upright_pid.out - upright_pid.kp * speed_pid.out;
//		PWMA = pwm_out - turn_pid.out;
//		PWMB = pwm_out + turn_pid.out;
		
		PWMA = pwm_out;
		PWMB = pwm_out;
		PWMLimit(PWMA, PWMB);
		MotorSetDuty(PWMA, PWMB);
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
	static uint8_t last_mode = 0xFF;  
	if(Key_GetNum())
	{
		balance_state.mode++;
		balance_state.mode %= 3;
		Buzzer_ON();
	}
	else if(balance_state.mode == 1)
	{
		if(obstacle_blocked) Buzzer_ON();
		else Buzzer_OFF();
	} 
	else Buzzer_OFF();
	// 仅当模式发生变化时清除数据
    if (balance_state.mode != last_mode)
    {
        DataClear();
        last_mode = balance_state.mode;  
    }
	
	if(balance_state.mode == 0) 										// 平衡模式
	{
//		speed_pid.kp = -0.58;
//		speed_pid.ki = -0.58/200;
		Balance_ON(); 
	}
	else Balance_OFF();  
	if(balance_state.mode == 1) 										// 遥控模式
	{
		speed_pid.kp = -0.55;
		speed_pid.ki = 0;
		BlueTooth_ON();
		BlueTooth();  										
		ObstacleAvoid();												// 警报
	}
	else BlueTooth_OFF();
	if(balance_state.mode == 2)											// 跟随模式
	{
		speed_pid.kp = -0.5;
		speed_pid.ki = 0;
		Follow_ON(); 
		HCSR04_GetValue();
		//OLED_ShowNum(2, 7, distance, 3);
	}
	else Follow_OFF(); 
}	

/**
 * @brief 提起检测：检测是否被提起
 * @param 无
 * @retval 无
 * @note 根据Pitch角度和陀螺仪Y轴速度判断，触发停止控制
 */
void CheckLiftState(void)
{
    // 拿起条件：角度大且角速度较大，持续一定时间
    if (fabs(mpu.pitch) > LIFT_ANGLE_THRESHOLD && abs(mpu.gyro[1]) > LIFT_GYRO_THRESHOLD && motor_right.encoder > 50)
	{
		balance_state.lifted_counter++;
		if (balance_state.lifted_counter > 30)
		{
			balance_state.lifted_flag = 1;
			balance_state.balance_enable = 0;
			balance_state.lifted_counter = 0;
			MotorStop();
		}
	}
}

/**
 * @brief 着陆检测：检测是否已经放下
 * @param 无
 * @retval 无
 * @note 如果Pitch角度处于正常角度且静止一段时间，恢复平衡控制
 */
void DetectPutDown(void)
{
    if (balance_state.lifted_flag || stop_flag)
    {
        if (fabs(mpu.pitch) < 20 && abs(mpu.gyro[1]) < 150 && abs(motor_right.encoder) < 120)
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
void CheckFallDown(void)
{
	if (fabs(upright_pid.med_angle - mpu.pitch) > 70 && stop_flag == 0)			// 倒地检测
	{
		balance_state.balance_enable = 0;
		MotorStop();
	}   
}
 
/**
 * @brief 避障逻辑
 * @param 无
 * @retval 无
 * @note 距离过近则触发声光报警并禁止运动，距离恢复后解除限制
 */
void ObstacleAvoid(void)
{
	HCSR04_GetValue();
	if(distance > 0 && distance <= 80)
	{
		if(!obstacle_blocked && distance < 25.0f) 
		{
			Buzzer_ON();
			obstacle_blocked = 1;
		}
		else if (obstacle_blocked && distance > 60.0f)
		{
			Buzzer_OFF();
			obstacle_blocked = 0; 
		}
	}
}
