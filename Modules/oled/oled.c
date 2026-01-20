#include "oled.h"
#include "oled_font.h"
#include "bsp_delay.h"

// 定义OLED的I2C总线结构体
static iic_bus_t oled_bus = {
    .IIC_SDA_PORT = GPIOB,
    .IIC_SDA_PIN = GPIO_Pin_13,
    .IIC_SCL_PORT = GPIOB,
    .IIC_SCL_PIN = GPIO_Pin_12
};

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
    IICSendByte(&oled_bus, Byte);
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
    IICStart(&oled_bus);
    OLED_I2C_SendByte(OLED_I2C_ADDR);     // 从机地址
    IICWaitAck(&oled_bus);
    OLED_I2C_SendByte(OLED_CMD_MODE);     // 写命令模式
    IICWaitAck(&oled_bus);
    OLED_I2C_SendByte(Command);           // 发送命令
    IICWaitAck(&oled_bus);
    IICStop(&oled_bus);
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
    IICStart(&oled_bus);
    OLED_I2C_SendByte(OLED_I2C_ADDR);     // 从机地址
    IICWaitAck(&oled_bus);
    OLED_I2C_SendByte(OLED_DATA_MODE);    // 写数据模式
    IICWaitAck(&oled_bus);
    OLED_I2C_SendByte(Data);              // 发送数据
    IICWaitAck(&oled_bus);
    IICStop(&oled_bus);
}

/**
  * @brief  快速写入多个数据（优化性能）
  * @param  Data 数据数组
  * @param  Count 数据数量
  * @retval 无
  */
void OLED_WriteMultiData(uint8_t *Data, uint16_t Count)
{
    if (Count == 0) return;
    
    IICStart(&oled_bus);
    OLED_I2C_SendByte(OLED_I2C_ADDR);     // 从机地址
    IICWaitAck(&oled_bus);
    OLED_I2C_SendByte(OLED_DATA_MODE);    // 写数据模式
    IICWaitAck(&oled_bus);
    
    for (uint16_t i = 0; i < Count; i++) {
        OLED_I2C_SendByte(Data[i]);
        IICWaitAck(&oled_bus);
    }
    
    IICStop(&oled_bus);
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    X += 2;
    Y += 1;
    OLED_WriteCommand(0xB0 | Y);                    // 设置Y位置
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));    // 设置X位置高4位
    OLED_WriteCommand(0x00 | (X & 0x0F));           // 设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
    uint8_t i, j;
    for (j = 0; j < 8; j++) {
        OLED_SetCursor(j, 0);
        for(i = 0; i < 128; i++) {
            OLED_WriteData(0x00);
        }
    }
}

/**
  * @brief  OLED快速清屏（使用批量写入）
  * @param  无
  * @retval 无
  */
void OLED_Clear_Fast(void)
{
    uint8_t i, j;
    uint8_t zero_data[128] = {0};  // 全零数组
    
    for (j = 0; j < 8; j++) {
        OLED_SetCursor(j, 0);
        OLED_WriteMultiData(zero_data, 128);
    }
}
/**
  * @brief  OLED指定行清屏（使用批量写入）
  * @param  Line 指定行，范围：1~4
  * @retval 无
  */
void OLED_ClearLine_Fast(uint8_t Line)
{
    uint8_t page;
    uint8_t zero_data[128] = {0};

    if (Line < 1 || Line > 4) return;

    for (page = (Line - 1) * 2; page < (Line - 1) * 2 + 2; page++)
    {
        OLED_SetCursor(page, 0);
        OLED_WriteMultiData(zero_data, 128);
    }
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      
    uint8_t i;
    // 显示上半部分
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);
    for (i = 0; i < 8; i++) {
        OLED_WriteData(OLED_F8x16[Char - ' '][i]);
    }
    
    // 显示下半部分
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);
    for (i = 0; i < 8; i++) {
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);
    }
}

/**
  * @brief  OLED快速显示一个字符（批量写入）
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符
  * @retval 无
  */
void OLED_ShowChar_Fast(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t char_data[16];
    uint8_t i;
    
    // 复制字模数据
    for (i = 0; i < 8; i++) {
        char_data[i] = OLED_F8x16[Char - ' '][i];
    }
    
    // 显示上半部分
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);
    OLED_WriteMultiData(char_data, 8);
    
    // 复制下半部分数据
    for (i = 0; i < 8; i++) {
        char_data[i] = OLED_F8x16[Char - ' '][i + 8];
    }
    
    // 显示下半部分
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);
    OLED_WriteMultiData(char_data, 8);
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++) {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}

/**
  * @brief  OLED快速显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串
  * @retval 无
  */
void OLED_ShowString_Fast(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++) {
        OLED_ShowChar_Fast(Line, Column + i, String[i]);
    }
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--) {
        Result *= X;
    }
    return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字
  * @param  Length 要显示数字的长度
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(Line, Column + i, 
                     Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字
  * @param  Length 要显示数字的长度
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0) {
        OLED_ShowChar(Line, Column, '+');
        Number1 = Number;
    } else {
        OLED_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(Line, Column + i + 1,
                     Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字
  * @param  Length 要显示数字的长度
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++) {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10) {
            OLED_ShowChar(Line, Column + i, SingleNumber + '0');
        } else {
            OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字
  * @param  Length 要显示数字的长度
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(Line, Column + i,
                     Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
    }
}

/**
  * @brief  OLED显示浮点数
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字
  * @param  IntegerLength 整数部分长度
  * @param  DecimalLength 小数部分长度
  * @retval 无
  */
void OLED_ShowFloat(uint8_t Line, uint8_t Column, float Number,
                    uint8_t IntegerLength, uint8_t DecimalLength)
{
    uint32_t integer_part = (uint32_t)Number;
    uint32_t decimal_part = (uint32_t)((Number - integer_part) * 
                                       OLED_Pow(10, DecimalLength));
    
    OLED_ShowNum(Line, Column, integer_part, IntegerLength);
    OLED_ShowChar(Line, Column + IntegerLength, '.');
    OLED_ShowNum(Line, Column + IntegerLength + 1, 
                 decimal_part, DecimalLength);
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
    // 初始化I2C总线
    IICInit(&oled_bus);
    delay_ms(5);  

    // SSD1306初始化序列
    OLED_WriteCommand(0xAE);    // 关闭显示

    OLED_WriteCommand(0xD5);    // 设置显示时钟分频比/振荡器频率
    OLED_WriteCommand(0x80);
    
    OLED_WriteCommand(0xA8);    // 设置多路复用率
    OLED_WriteCommand(0x3F);
    
    OLED_WriteCommand(0xD3);    // 设置显示偏移
    OLED_WriteCommand(0x00);
    
    OLED_WriteCommand(0x40);    // 设置显示开始行
    
    OLED_WriteCommand(0xA1);    // 设置左右方向，0xA1正常 0xA0左右反置
    
    OLED_WriteCommand(0xC8);    // 设置上下方向，0xC8正常 0xC0上下反置

    OLED_WriteCommand(0xDA);    // 设置COM引脚硬件配置
    OLED_WriteCommand(0x12);
    
    OLED_WriteCommand(0x81);    // 设置对比度控制
    OLED_WriteCommand(0xCF);

    OLED_WriteCommand(0xD9);    // 设置预充电周期
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB);    // 设置VCOMH取消选择级别
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4);    // 设置整个显示打开/关闭

    OLED_WriteCommand(0xA6);    // 设置正常/倒转显示

    OLED_WriteCommand(0x8D);    // 设置充电泵
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0xAF);    // 开启显示
    
    delay_ms(5);
    
    OLED_Clear();               // OLED清屏
}
