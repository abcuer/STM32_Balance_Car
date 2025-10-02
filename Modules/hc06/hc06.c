#include "headfile.h"


uint8_t Serial_RxData;    // 接收数据
uint8_t Serial_RxFlag;    // 接收标志

BT_Command_t bt_cmd;      // 蓝牙命令结构体

void bt_init(void)
{
	HAL_UART_Receive_IT(&huart2, &Serial_RxData, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART2)   // 判断是USART2触发的回调
    {
        switch(Serial_RxData)
        {
            case 0x00: bt_cmd.forward=0; bt_cmd.backward=0; bt_cmd.left=0; bt_cmd.right=0; break;
            case 0x01: bt_cmd.forward=1; bt_cmd.backward=0; bt_cmd.left=0; bt_cmd.right=0; break; //前
            case 0x05: bt_cmd.forward=0; bt_cmd.backward=1; bt_cmd.left=0; bt_cmd.right=0; break; //后
            case 0x03: bt_cmd.forward=0; bt_cmd.backward=0; bt_cmd.left=1; bt_cmd.right=0; break; //左
            case 0x07: bt_cmd.forward=0; bt_cmd.backward=0; bt_cmd.left=0; bt_cmd.right=1; break; //右
            case 0x08: bt_cmd.forward=1; bt_cmd.backward=0; bt_cmd.left=1; bt_cmd.right=0; break; //左前
            case 0x02: bt_cmd.forward=1; bt_cmd.backward=0; bt_cmd.left=0; bt_cmd.right=1; break; //右前
            case 0x06: bt_cmd.forward=0; bt_cmd.backward=1; bt_cmd.left=1; bt_cmd.right=0; break; //左后
            case 0x04: bt_cmd.forward=0; bt_cmd.backward=1; bt_cmd.left=0; bt_cmd.right=1; break; //右后
            default:   bt_cmd.forward=0; bt_cmd.backward=0; bt_cmd.left=0; bt_cmd.right=0; break;
        }

        Serial_RxFlag = 1;  // 标记接收完成

        // 重新打开中断接收，否则只会触发一次
        HAL_UART_Receive_IT(&huart2, &Serial_RxData, 1);
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
	
	if(bt_cmd.left == 1 && bt_cmd.right == 0)				turn_pid.speed -= 1; 
	else if(bt_cmd.right == 1 && bt_cmd.left == 0) 			turn_pid.speed += 1;
	else if(bt_cmd.left == 0 && bt_cmd.right == 0)			turn_pid.speed = 0; 
	
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