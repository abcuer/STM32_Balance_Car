#include "headfile.h"

static BT_Command_t bt_cmd;
static RX_DATA_t rx_data;

/**
 * @brief 蓝牙遥控模式控制逻辑
 * @param 无
 * @retval 无
 * @note 根据前后左右命令调整目标速度与转向速度
 */
void BlueTooth(void)
{
    // 1. 优先判断组合指令
    if(bt_cmd.forward && bt_cmd.left)        { speed_pid.speed = MAX_Speed-10;  turn_pid.speed = MAX_Turn; }
    else if(bt_cmd.forward && bt_cmd.right)  { speed_pid.speed = MAX_Speed+10;  turn_pid.speed = -MAX_Turn;  }
    else if(bt_cmd.backward && bt_cmd.left)  { speed_pid.speed = -MAX_Speed+10; turn_pid.speed = -MAX_Turn; }
    else if(bt_cmd.backward && bt_cmd.right) { speed_pid.speed = -MAX_Speed-10; turn_pid.speed = MAX_Turn;  }
    // 2. 再判断单一方向指令
    else if(bt_cmd.forward)  { speed_pid.speed = MAX_Speed+3;  turn_pid.speed = 0; }
    else if(bt_cmd.backward) { speed_pid.speed = -MAX_Speed-3; turn_pid.speed = 0; }
    else if(bt_cmd.left)     { speed_pid.speed = 0;          turn_pid.speed = -25; }
    else if(bt_cmd.right)    { speed_pid.speed = 0;          turn_pid.speed = 25;  }
    else 
    {
        speed_pid.speed = 0;
        turn_pid.speed = 0;
    }
}

/**
 * @brief USART2 中断服务函数（用于蓝牙遥控接收）
 * @note 本函数用于处理蓝牙模块发送的串口指令，通过解析接收到的指令字节，
 *       控制小车的运动方向（前/后/左/右/斜方向）。
 */
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)		//判断是否是USART2的接收事件触发的中断
	{
		rx_data.data = USART_ReceiveData(USART2);				//读取数据寄存器，存放在接收的数据变量
		
		switch(rx_data.data)
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

        rx_data.flag = 1;  // 标记接收完成									//置接收标志位变量为1
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);			//清除USART2的RXNE标志位
	}
}
