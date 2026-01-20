#include "mpu6050.h"
#include "bsp_iic.h"
#include "bsp_exti.h"
#include "inv_mpu.h"
#include "sys.h"

MPU_t mpu;

// 定义MPU6050的总线实例
static iic_bus_t mpu6050_bus = {
    .IIC_SDA_PORT = GPIOB,
    .IIC_SDA_PIN = GPIO_Pin_11,
    .IIC_SCL_PORT = GPIOB,
    .IIC_SCL_PIN = GPIO_Pin_10
};

// 写多个字节
uint8_t mpu6050_write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t* buf)
{
    IICStart(&mpu6050_bus);
    
    // 发送器件地址（写模式）
    IICSendByte(&mpu6050_bus, addr << 1);
    if (IICWaitAck(&mpu6050_bus) != SUCCESS) {
        IICStop(&mpu6050_bus);
        return 1;  // 失败
    }
    
    // 发送寄存器地址
    IICSendByte(&mpu6050_bus, reg);
    if (IICWaitAck(&mpu6050_bus) != SUCCESS) {
        IICStop(&mpu6050_bus);
        return 1;
    }
    
    // 发送数据
    for (uint8_t i = 0; i < len; i++) {
        IICSendByte(&mpu6050_bus, buf[i]);
        if (IICWaitAck(&mpu6050_bus) != SUCCESS) {
            IICStop(&mpu6050_bus);
            return 1;
        }
    }
    
    IICStop(&mpu6050_bus);
    return 0;  // 成功
}

// 读多个字节
uint8_t mpu6050_read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t* buf)
{
    IICStart(&mpu6050_bus);
    
    // 发送器件地址（写模式）- 写寄存器地址
    IICSendByte(&mpu6050_bus, addr << 1);
    if (IICWaitAck(&mpu6050_bus) != SUCCESS) {
        IICStop(&mpu6050_bus);
        return 1;
    }
    
    // 发送寄存器地址
    IICSendByte(&mpu6050_bus, reg);
    if (IICWaitAck(&mpu6050_bus) != SUCCESS) {
        IICStop(&mpu6050_bus);
        return 1;
    }
    
    // 重复开始信号，切换到读模式
    IICStart(&mpu6050_bus);
    
    // 发送器件地址（读模式）
    IICSendByte(&mpu6050_bus, (addr << 1) | 0x01);
    if (IICWaitAck(&mpu6050_bus) != SUCCESS) {
        IICStop(&mpu6050_bus);
        return 1;
    }
    
    // 读取数据
    for (uint8_t i = 0; i < len; i++) {
        if (i < len - 1) {
            buf[i] = IICReceiveByte(&mpu6050_bus);
            IICSendAck(&mpu6050_bus);  // 发送ACK
        } else {
            buf[i] = IICReceiveByte(&mpu6050_bus);
            IICSendNotAck(&mpu6050_bus);  // 最后一个字节发送NACK
        }
    }
    
    IICStop(&mpu6050_bus);
    return 0;  // 成功
}

// 写单个寄存器
static void mpu6050_write_reg(uint8_t reg, uint8_t dat)
{
    mpu6050_write(MPU_ADDR, reg, 1, &dat);
}

// 读单个寄存器
static uint8_t mpu6050_read_reg(uint8_t reg)
{
    uint8_t dat;
    mpu6050_read(MPU_ADDR, reg, 1, &dat);
    return dat;
}

// 设置陀螺仪满量程范围
static uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    mpu6050_write_reg(GYRO_CONFIG, fsr << 3);
    return 0;
}

// 设置加速度计满量程范围
static uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
    mpu6050_write_reg(ACCEL_CONFIG, fsr << 3);
    return 0;
}

// 设置数字低通滤波器
static uint8_t MPU_Set_LPF(uint16_t lpf)
{
    uint8_t data = 0;
    if (lpf >= 188) data = 1;
    else if (lpf >= 98) data = 2;
    else if (lpf >= 42) data = 3;
    else if (lpf >= 20) data = 4;
    else if (lpf >= 10) data = 5;
    else data = 6;
    
    mpu6050_write_reg(MPU_CFG_REG, data);
    return 0;
}

// 设置采样率
static uint8_t MPU_Set_Rate(uint16_t rate)
{
    uint8_t data;
    if (rate > 1000) rate = 1000;
    if (rate < 4) rate = 4;
    data = 1000 / rate - 1;
    mpu6050_write_reg(MPU_SAMPLE_RATE_REG, data);
    return MPU_Set_LPF(rate / 2);  // 自动设置LPF为采样率的一半
}

// 获取温度值（摄氏度）
static float MPU_Get_Temperature(void)
{
    uint8_t buf[2];
    int16_t raw;
    float temp;
    
    mpu6050_read(MPU_ADDR, TEMP_OUT_H, 2, buf);
    raw = ((uint16_t)buf[0] << 8) | buf[1];
    temp = 36.53f + ((float)raw) / 340.0f;
    
    return temp;
}

// 获取陀螺仪原始数据
static uint8_t MPU_Get_Gyroscope(int16_t *gx, int16_t *gy, int16_t *gz)
{
    uint8_t buf[6];
    uint8_t res;
    
    res = mpu6050_read(MPU_ADDR, GYRO_XOUT_H, 6, buf);
    if (res == 0)
    {
        *gx = ((uint16_t)buf[0] << 8) | buf[1];
        *gy = ((uint16_t)buf[2] << 8) | buf[3];
        *gz = ((uint16_t)buf[4] << 8) | buf[5];
    }
    
    return res;
}

// 获取加速度计原始数据
static uint8_t MPU_Get_Accelerometer(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t buf[6];
    uint8_t res;
    
    res = mpu6050_read(MPU_ADDR, ACCEL_XOUT_H, 6, buf);
    if (res == 0)
    {
        *ax = ((uint16_t)buf[0] << 8) | buf[1];
        *ay = ((uint16_t)buf[2] << 8) | buf[3];
        *az = ((uint16_t)buf[4] << 8) | buf[5];
    }
    
    return res;
}

static void MPU_Init(void)
{
	uint8_t res = 0; 
    IICInit(&mpu6050_bus);
    // 1. 复位
    mpu6050_write_reg(PWR_MGMT_1, 0x80);
    delay_ms(100);
    // 2. 唤醒 + 选 PLL
    mpu6050_write_reg(PWR_MGMT_1, 0x00);
    delay_ms(10);
    // 3. 量程
    MPU_Set_Gyro_Fsr(GYRO_2000DPS);    // ±2000dps
    MPU_Set_Accel_Fsr(ACC_2G);   // ±2g
    // 4. 采样率
    MPU_Set_Rate(200);      // 100Hz
    // 5. 中断配置
    mpu6050_write_reg(MPU_INT_EN_REG, 0x00);   
    mpu6050_write_reg(MPU_INTBP_CFG_REG, 0x80); 
    // 6. 其他功能全部关掉
    mpu6050_write_reg(MPU_FIFO_EN_REG, 0x00);
    mpu6050_write_reg(MPU_USER_CTRL_REG, 0x00);
	if(res==MPU_ADDR)//器件ID正确
	{
		mpu6050_write_reg(PWR_MGMT_1,0X01);	//设置CLKSEL,PLL X轴为参考
		mpu6050_write_reg(PWR_MGMT_2,0X00);	//加速度与陀螺仪都工作
		MPU_Set_Rate(100);					//设置采样率为50Hz
 	}
}

void MPU_IIC_Init(void)
{
	My_GPIO_Init(MPU_IIC_GPIO, MPU_IIC_SCL_Pin|MPU_IIC_SDA_Pin, GPIO_TW_OUT, GPIO_P_NO, GPIO_50MHz);//推挽输出 不拉 50m
	MPU_IIC_SCL = 1;
	MPU_IIC_SDA = 1;
}

void IMU_Init(void)
{
	MPU_Init();
	MPU_DMP_Init();
	MPU_INT_Init();
	delay_ms(300);
}

void MPU_Get_Angle(void)
{
	MPU_DMP_GetData(&mpu.pitch, &mpu.roll, &mpu.yaw);
	MPU_Get_Gyroscope(&mpu.gyro[0], &mpu.gyro[1], &mpu.gyro[2]);
}
