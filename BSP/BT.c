#include "headfile.h"

extern uint8_t Serial_RxData;		//定义串口接收的数据变量
extern uint8_t Serial_RxFlag;		//定义串口接收的标志位变量

BT_Command_t bt_cmd;
/**
 * @brief USART2 中断服务函数（用于蓝牙遥控接收）
 * @note 本函数用于处理蓝牙模块发送的串口指令，通过解析接收到的指令字节，
 *       控制小车的运动方向（前/后/左/右/斜方向）。
 *
 *       蓝牙端发送 1 个字节指令（0x00~0x08），对应不同的方向控制：
 *       - 0x00：停止
 *       - 0x01：前进
 *       - 0x05：后退
 *       - 0x03：左转
 *       - 0x07：右转
 *       - 0x08：左前
 *       - 0x02：右前
 *       - 0x06：左后
 *       - 0x04：右后
 *
 *       接收后设置方向标志变量（forward、backward、left、right），供主控逻辑使用。
 */
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)		//判断是否是USART2的接收事件触发的中断
	{
		Serial_RxData = USART_ReceiveData(USART2);				//读取数据寄存器，存放在接收的数据变量
		if(Serial_RxData == 0x00) bt_cmd.forward = 0, bt_cmd.backward = 0, bt_cmd.left = 0, bt_cmd.right = 0;
		else if(Serial_RxData == 0x01) bt_cmd.forward = 1, bt_cmd.backward = 0, bt_cmd.left = 0, bt_cmd.right = 0; //前
		else if(Serial_RxData == 0x05) bt_cmd.forward = 0, bt_cmd.backward = 1, bt_cmd.left = 0, bt_cmd.right = 0;	//后
		else if(Serial_RxData == 0x03) bt_cmd.forward = 0, bt_cmd.backward = 0, bt_cmd.left = 1, bt_cmd.right = 0;	//左
		else if(Serial_RxData == 0x07) bt_cmd.forward = 0, bt_cmd.backward = 0, bt_cmd.left = 0, bt_cmd.right = 1; //右
		else if(Serial_RxData == 0x08) bt_cmd.forward = 1, bt_cmd.backward = 0, bt_cmd.left = 1, bt_cmd.right = 0; //左前方
		else if(Serial_RxData == 0x02) bt_cmd.forward = 1, bt_cmd.backward = 0, bt_cmd.left = 0, bt_cmd.right = 1; //右前方
		else if(Serial_RxData == 0x06) bt_cmd.forward = 0, bt_cmd.backward = 1, bt_cmd.left = 1, bt_cmd.right = 0; //左后方
		else if(Serial_RxData == 0x04) bt_cmd.forward = 0, bt_cmd.backward = 1, bt_cmd.left = 0, bt_cmd.right = 1; //右后方
		else  						   bt_cmd.forward = 0, bt_cmd.backward = 0, bt_cmd.left = 0, bt_cmd.right = 0;
		
		Serial_RxFlag = 1;										//置接收标志位变量为1
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);			//清除USART2的RXNE标志位
	}
}
