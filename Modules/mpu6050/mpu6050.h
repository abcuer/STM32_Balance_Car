#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "bsp_iic.h"
#include "bsp_delay.h"
#include <stdint.h>
#include "math.h"

#define	MPU_ADDR		0x68	// MPU6050 I2C地址
// MPU6050寄存器地址
#define MPU_SAMPLE_RATE_REG		0X19	// 采样频率分频器
#define MPU_CFG_REG				0X1A	// 配置寄存器
#define	GYRO_CONFIG		0x1B	// 陀螺仪自检及测量范围
#define	ACCEL_CONFIG	0x1C	// 加速计自检、测量范围及高通滤波频率
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B 
#define	PWR_MGMT_2		0x6C
#define	MPU_DEVICE_ID_REG 0x75	

#define MPU_FIFO_EN_REG			0X23	//FIFO使能寄存器
#define MPU_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define MPU_INTBP_CFG_REG		0X37	//中断/旁路设置寄存器
#define MPU_INT_EN_REG			0X38	//中断使能寄存器
#define MPU_INT_STA_REG			0X3A	//中断状态寄存器
#define MPU_USER_CTRL_REG		0X6A	//用户控制寄存器

#define GYRO_250DPS            0
#define GYRO_500DPS            1
#define GYRO_1000DPS           2
#define GYRO_2000DPS           3

#define ACC_2G                 0
#define ACC_4G                 1 
#define ACC_8G                 2
#define ACC_16G                3    

#define MPU_IIC_GPIO                   GPIOB
#define MPU_IIC_SCL_Pin                GPIO_Pin_10	         //PB10
#define MPU_IIC_SDA_Pin                GPIO_Pin_11	         //PB11

#define	MPU_IIC_SCL                    PBout(10)
#define	MPU_IIC_SDA                    PBout(11)  
#define	MPU_IIC_SDA_IN                 PBin(11)              //读数据端口
#define MPU_IIC_delay_4us()            delay_us(4)

typedef struct
{
	float pitch, roll, yaw;
	short gyro[3], acc[3];
}MPU_t;

//MPU6050_IIC所有操作函数    
uint8_t mpu6050_write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t* buf);
uint8_t mpu6050_read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t* buf);

void IMU_Init(void);
void MPU_IIC_Init(void);        
void MPU_Get_Angle(void);

extern MPU_t mpu;

#endif

