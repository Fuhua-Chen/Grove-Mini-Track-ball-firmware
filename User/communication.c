#include "main.h"

I2C_Slaver_Communicate_TypeDef I2C_Communication;
/*------------------------------------------------------------------------------------------------------------------------//
I2C init

*/
void I2CInit(I2C_TypeDef *I2C_Port, uint16_t I2C_OwnAddress, uint32_t ClockSpeed)
{
	I2C_InitTypeDef I2C_MyInit;
	enum IRQn I2C_IRQn;
	
	switch((uint32_t)I2C_Port)
	{
		case (uint32_t) I2C1:
	    GPIOInit(GPIOB,GPIO_Pin_6|GPIO_Pin_7,GPIO_Mode_AF_OD);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 ,ENABLE);
		  I2C_IRQn = I2C1_EV_IRQn;
			break;
		case (uint32_t) I2C2:
	    GPIOInit(GPIOB,GPIO_Pin_10|GPIO_Pin_11,GPIO_Mode_AF_OD);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2 ,ENABLE);
		  I2C_IRQn = I2C2_EV_IRQn;
			break;
	}
	I2C_DeInit(I2C_Port);
	I2C_MyInit.I2C_Ack = I2C_Ack_Enable ;
	I2C_MyInit.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_MyInit.I2C_ClockSpeed = ClockSpeed;
	I2C_MyInit.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_MyInit.I2C_Mode = I2C_Mode_I2C ;
	I2C_MyInit.I2C_OwnAddress1 = I2C_OwnAddress<<1;
	I2C_Init(I2C_Port , &I2C_MyInit);
	
	I2C_ITConfig(I2C_Port,I2C_IT_EVT|I2C_IT_BUF,ENABLE);
	NVIC_EnableIRQ(I2C_IRQn);
	
	I2C_Cmd(I2C_Port, ENABLE);
}

/*------------------------------------------------------------------------------------------------------------------------//
I2Cs interrupt


*/
void I2CPort_IRQHandler(void)
{
	static uint8_t Counter = 0; 
	uint32_t I2C_EVENT = I2C_GetLastEvent(I2CPort);
	
	switch(I2C_EVENT)  
	{     		
		 case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:  
		 {		 
       I2C_GenerateSTOP(I2CPort, DISABLE);
			 if(I2C_Communication.DataLength >= 3 && I2C_Communication.DataBuffer[0] == 0)
			 {
				 I2C_Communication.DataLength = I2C_Communication.DataBuffer[2];
				 I2C_Communication.ReadAddr = I2C_Communication.DataBuffer[1];
				 memset(I2C_Communication.DataBuffer,0,I2c_BufferSize);
		     memcpy(I2C_Communication.DataBuffer,PramReg+I2C_Communication.ReadAddr,I2C_Communication.DataLength);
			   Counter = 0;
			 }
			 break;  
		 }  
		 
		 case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:  
		 {
       I2C_GenerateSTOP(I2CPort, DISABLE); 	 
			 I2C_AcknowledgeConfig(I2CPort,ENABLE);
		   I2C_Communication.DataLength = 0;
			 break;  
		 }
		 
		 case I2C_EVENT_SLAVE_BYTE_RECEIVED:
		 {
			 I2C_Communication.DataBuffer[I2C_Communication.DataLength] = I2C_ReceiveData(I2CPort);
			 I2C_Communication.DataLength++;
			 break;
		 }
		 
		 case I2C_EVENT_SLAVE_BYTE_TRANSMITTED: 
		 {   
			 //with stm32's i2c ,you must send a byte at last,this byte will not send to the master because it has a error between DSR and DR
			 I2C_SendData(I2CPort,0);
			 break;   
		 }
		 
		 
		 case I2C_EVENT_SLAVE_STOP_DETECTED:   
		 {			 
			 
			 I2C_ClearFlag(I2CPort,I2C_FLAG_STOPF);  
       I2C_GenerateSTOP(I2C2, ENABLE); 
			 //PER FRAME Must more than 2 bytes data,and less than 3 + CONFIG_REG_NUM - MOTION_REG_NUM data, first bytes is mean read or write
			 if(I2C_Communication.DataLength >= 3 && I2C_Communication.DataBuffer[0] == 1 && I2C_Communication.DataLength < 3 + CONFIG_REG_NUM - MOTION_REG_NUM)
			 {
				 I2C_Communication.WriteAddr = I2C_Communication.DataBuffer[1];
				 if(I2C_Communication.WriteAddr >= MOTION_REG_NUM && I2C_Communication.WriteAddr < CONFIG_REG_NUM)
				 {
					 //copy to PramReg,it is use to read by user;
		       memcpy(PramReg+I2C_Communication.WriteAddr,&I2C_Communication.DataBuffer[2],I2C_Communication.DataLength-2);
					 IsDataNew = 1;
				 }
			 }
			 break;   
		 } 
		 
		 
		 case I2C_EVENT_SLAVE_BYTE_TRANSMITTING:   
		 { 	
			 if(I2C_Communication.DataLength > Counter)
			 {
				 I2C_SendData(I2CPort,I2C_Communication.DataBuffer[Counter]);
				 Counter++;
			 }
			 else
			 {
				 //memset(PramReg,0,MOTION_REG_NUM);
				 if((I2C_Communication.ReadAddr+I2C_Communication.DataLength) >= MOTION_REG_NUM) //cant set config reg;
				 {
					 I2C_Communication.DataLength = MOTION_REG_NUM - I2C_Communication.ReadAddr > 0 ? MOTION_REG_NUM - I2C_Communication.ReadAddr : 0;
				 }
				 memset(PramReg+I2C_Communication.ReadAddr,0,I2C_Communication.DataLength);
				 I2C_Communication.DataLength = 0;
				 Counter = 0;
			 }
			 break;   
		 } 
		 
		default :	 
	   I2C_ClearFlag(I2CPort,I2C_FLAG_AF); 
     break;	
		 
	 }
}

