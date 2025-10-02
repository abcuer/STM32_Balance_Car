#include "headfile.h"
uint8_t mode = 0;
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
        last_mode = mode;  // 更新记录
    }
	
	if(mode == 0) 											//平衡模式
	{
		Balance_ON(); 
	}
	else Balance_OFF();  
	if(mode == 1)  //蓝牙遥控模式
	{
		BlueTooth_ON();
	}
	else BlueTooth_OFF();
	if(mode == 2)											// 超声波跟随
	{
		Follow_ON(); 
	}
	else Follow_OFF(); 
}	