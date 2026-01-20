#include "bsp_iic.h"
#include "bsp_delay.h"

/**
  * @brief SDA线输入模式配置
  */
void SDA_Input_Mode(iic_bus_t *bus)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = bus->IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(bus->IIC_SDA_PORT, &GPIO_InitStructure);
}

/**
  * @brief SDA线输出模式配置
  */
void SDA_Output_Mode(iic_bus_t *bus)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = bus->IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // 开漏输出，IIC标准用法
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(bus->IIC_SDA_PORT, &GPIO_InitStructure);
}

/**
  * @brief SDA线输出一个位
  */
void SDA_Output(iic_bus_t *bus, uint16_t val)
{
    if (val)
        GPIO_SetBits(bus->IIC_SDA_PORT, bus->IIC_SDA_PIN);
    else
        GPIO_ResetBits(bus->IIC_SDA_PORT, bus->IIC_SDA_PIN);
}

/**
  * @brief SCL线输出一个位
  */
void SCL_Output(iic_bus_t *bus, uint16_t val)
{
    if (val)
        GPIO_SetBits(bus->IIC_SCL_PORT, bus->IIC_SCL_PIN);
    else
        GPIO_ResetBits(bus->IIC_SCL_PORT, bus->IIC_SCL_PIN);
}

/**
  * @brief SDA输入一位
  */
uint8_t SDA_Input(iic_bus_t *bus)
{
    return GPIO_ReadInputDataBit(bus->IIC_SDA_PORT, bus->IIC_SDA_PIN);
}

/**
  * @brief IIC起始信号
  */
void IICStart(iic_bus_t *bus)
{
    SDA_Output_Mode(bus);
    SDA_Output(bus, 1);
    SCL_Output(bus, 1);
    delay_us(4);
    SDA_Output(bus, 0);
    delay_us(4);
    SCL_Output(bus, 0);
}

/**
  * @brief IIC结束信号
  */
void IICStop(iic_bus_t *bus)
{
    SDA_Output_Mode(bus);
    SCL_Output(bus, 0);
    SDA_Output(bus, 0);
    delay_us(4);
    SCL_Output(bus, 1);
    delay_us(4);
    SDA_Output(bus, 1);
    delay_us(4);
}

/**
  * @brief IIC等待确认信号
  */
unsigned char IICWaitAck(iic_bus_t *bus)
{
    uint8_t t = 0;
    SDA_Input_Mode(bus);
    SDA_Output(bus, 1); delay_us(1);
    SCL_Output(bus, 1); delay_us(1);
    while (SDA_Input(bus))
    {
        t++;
        if (t > 250)
        {
            IICStop(bus);
            return ERROR;
        }
    }
    SCL_Output(bus, 0);
    return SUCCESS;
}

/**
  * @brief IIC发送确认信号
  */
void IICSendAck(iic_bus_t *bus)
{
    SCL_Output(bus, 0);
    SDA_Output_Mode(bus);
    SDA_Output(bus, 0);
    delay_us(2);
    SCL_Output(bus, 1);
    delay_us(2);
    SCL_Output(bus, 0);
}

/**
  * @brief IIC发送非确认信号
  */
void IICSendNotAck(iic_bus_t *bus)
{
    SCL_Output(bus, 0);
    SDA_Output_Mode(bus);
    SDA_Output(bus, 1);
    delay_us(2);
    SCL_Output(bus, 1);
    delay_us(2);
    SCL_Output(bus, 0);
}

/**
  * @brief IIC发送一个字节
  */
void IICSendByte(iic_bus_t *bus, unsigned char cSendByte)
{
    uint8_t t;
    SDA_Output_Mode(bus);
    SCL_Output(bus, 0);
    for (t = 0; t < 8; t++)
    {
        SDA_Output(bus, (cSendByte & 0x80) >> 7);
        cSendByte <<= 1;
        delay_us(2);
        SCL_Output(bus, 1);
        delay_us(2);
        SCL_Output(bus, 0);
        delay_us(2);
    }
}

/**
  * @brief IIC接收一个字节
  */
unsigned char IICReceiveByte(iic_bus_t *bus)
{
    uint8_t i, receive = 0;
    SDA_Input_Mode(bus);
    for (i = 0; i < 8; i++)
    {
        SCL_Output(bus, 0);
        delay_us(2);
        SCL_Output(bus, 1);
        receive <<= 1;
        if (SDA_Input(bus)) receive++;
        delay_us(1);
    }
    return receive;
}

uint8_t IIC_Write_One_Byte(iic_bus_t *bus, uint8_t daddr,uint8_t reg,uint8_t data)
{				   	  	    																 
  IICStart(bus);  
	
	IICSendByte(bus,daddr<<1);	    
	if(IICWaitAck(bus))	//�ȴ�Ӧ��
	{
		IICStop(bus);		 
		return 1;		
	}
	IICSendByte(bus,reg);
	IICWaitAck(bus);	   	 										  		   
	IICSendByte(bus,data);     						   
	IICWaitAck(bus);  		    	   
  IICStop(bus);
	delay_us(1);
	return 0;
}

uint8_t IIC_Write_Multi_Byte(iic_bus_t *bus, uint8_t daddr,uint8_t reg,uint8_t length,uint8_t buff[])
{			
	unsigned char i;	
  IICStart(bus);  
	
	IICSendByte(bus,daddr<<1);	    
	if(IICWaitAck(bus))
	{
		IICStop(bus);
		return 1;
	}
	IICSendByte(bus,reg);
	IICWaitAck(bus);	
	for(i=0;i<length;i++)
	{
		IICSendByte(bus,buff[i]);     						   
		IICWaitAck(bus); 
	}		    	   
  IICStop(bus);
	delay_us(1);
	return 0;
} 

unsigned char IIC_Read_One_Byte(iic_bus_t *bus, uint8_t daddr,uint8_t reg)
{
	unsigned char dat;
	IICStart(bus);
	IICSendByte(bus,daddr<<1);
	IICWaitAck(bus);
	IICSendByte(bus,reg);
	IICWaitAck(bus);
	
	IICStart(bus);
	IICSendByte(bus,(daddr<<1)+1);
	IICWaitAck(bus);
	dat = IICReceiveByte(bus);
	IICSendNotAck(bus);
	IICStop(bus);
	return dat;
}


uint8_t IIC_Read_Multi_Byte(iic_bus_t *bus, uint8_t daddr, uint8_t reg, uint8_t length, uint8_t buff[])
{
	unsigned char i;
	IICStart(bus);
	IICSendByte(bus,daddr<<1);
	if(IICWaitAck(bus))
	{
		IICStop(bus);		 
		return 1;		
	}
	IICSendByte(bus,reg);
	IICWaitAck(bus);
	
	IICStart(bus);
	IICSendByte(bus,(daddr<<1)+1);
	IICWaitAck(bus);
	for(i=0;i<length;i++)
	{
		buff[i] = IICReceiveByte(bus);
		if(i<length-1)
		{IICSendAck(bus);}
	}
	IICSendNotAck(bus);
	IICStop(bus);
	return 0;
}


void IICInit(iic_bus_t *bus)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 注意：外部需要先开启对应 GPIO 端口的时钟 (RCC_APB2PeriphClockCmd)
    
    GPIO_InitStructure.GPIO_Pin = bus->IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
	//    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(bus->IIC_SCL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = bus->IIC_SDA_PIN;
    GPIO_Init(bus->IIC_SDA_PORT, &GPIO_InitStructure);

    // 默认高电平（空闲状态）
    GPIO_SetBits(bus->IIC_SCL_PORT, bus->IIC_SCL_PIN);
    GPIO_SetBits(bus->IIC_SDA_PORT, bus->IIC_SDA_PIN);
}
