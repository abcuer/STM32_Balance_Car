#include "headfile.h"
uint8_t Key_GetNum(void)
{
    static uint8_t key_pre = 1;  // 上一次按键状态（1=松开，0=按下）
    uint8_t key_curr = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin); // 当前状态

    uint8_t key_pressed = 0;

    if (key_pre == 1 && key_curr == 0)
    {
        // 检测到由松开变按下
        key_pressed = 1;
    }

    key_pre = key_curr; // 更新上一次状态
    return key_pressed;
}