#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "stm32f10x_conf.h"


#define Communication_PORT     GPIOB
#define Communication_SCL_TX   GPIO_Pin_10
#define Communication_SDA_RX   GPIO_Pin_11

#define I2CPort                I2C2
#define I2CPort_IRQn           I2C2_EV_IRQn
#define I2CPort_IRQHandler     I2C2_EV_IRQHandler

#define I2c_BufferSize 32
typedef struct 
{
	uint8_t DataLength;
	uint8_t DataBuffer[I2c_BufferSize];
	uint8_t ReadAddr;
	uint8_t WriteAddr;
}I2C_Slaver_Communicate_TypeDef;


void I2CInit(I2C_TypeDef *I2C_Port, uint16_t I2C_OwnAddress, uint32_t ClockSpeed);

#endif






