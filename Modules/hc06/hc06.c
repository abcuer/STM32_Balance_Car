#include "headfile.h"


uint8_t Serial_RxData;    // 接收数据
uint8_t Serial_RxFlag;    // 接收标志

BT_Command_t bt_cmd;      // 蓝牙命令结构体

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

void Bluetooth(void)
{
if(bt_cmd.forward == 1 && bt_cmd.backward == 0) 		Follow_ON();
	else if(bt_cmd.backward == 1 && bt_cmd.forward == 0)    BlueTooth_ON();
	// else if(bt_cmd.forward == 0 && bt_cmd.backward == 0) 	
	if(bt_cmd.left == 1 && bt_cmd.right == 0)				Balance_ON();
	else if(bt_cmd.right == 1 && bt_cmd.left == 0) 			BlueTooth_OFF();
	// else if(bt_cmd.left == 0 && bt_cmd.right == 0)			
	
	if(bt_cmd.forward && bt_cmd.left && bt_cmd.backward == 0 && bt_cmd.right == 0)
	{
		Follow_OFF(); 
	}
	// else if(bt_cmd.forward && bt_cmd.right && bt_cmd.backward == 0 && bt_cmd.left == 0)
	// {
	// 	speed_pid.speed += 0.4;  turn_pid.speed -= 1.2;
	// }
	// else if(bt_cmd.backward && bt_cmd.left && bt_cmd.forward == 0 && bt_cmd.right == 0)
	// {
	// 	speed_pid.speed -= 0.4;	 turn_pid.speed += 1.2; 
	// }
	// else if(bt_cmd.backward && bt_cmd.right && bt_cmd.forward == 0 && bt_cmd.left == 0)
	// {
	// 	speed_pid.speed -= 0.2;	 turn_pid.speed = 1.4;
	// }
	// if(bt_cmd.forward == 0 && bt_cmd.backward == 0 && bt_cmd.left == 0 && bt_cmd.right == 0)
	// {
	// 	speed_pid.speed = 0; 
	// 	turn_pid.speed = 0;
	// }
}