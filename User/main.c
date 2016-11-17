#include "main.h"
/*------------------------------------------------------------------------------------------------------------------------//
define data

*/

//
//Config data
//
Config_TypeDef Config;
Config_TypeDef DeConfigValue = {ConfigValid,DefaultI2CAddr,100,LED_BREATHING_ALL,1000,2000,20};
//
//movement data
//
uint8_t PramReg[CONFIG_REG_NUM] = {0};

uint8_t IsDataNew = 0;
//
//time 
//
volatile uint64_t localtime = 0;	

/*------------------------------------------------------------------------------------------------------------------------//
flash operation

*/
#define PageSize 1024

uint8_t flash_write(uint32_t StartAddr,uint32_t *p_data,uint32_t size)
{	
	volatile FLASH_Status FLASHStatus;	
	uint32_t EndAddr=StartAddr+size*4;	
	vu32 NbrOfPage = 0;	
	uint32_t EraseCounter = 0x0, Address = 0x0;
	int i;
	int MemoryProgramStatus=1;
	
	FLASH_Unlock();     
	NbrOfPage=((EndAddr-StartAddr)>>10)+1;
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASHStatus=FLASH_COMPLETE;
	
	for(EraseCounter=0;(EraseCounter<NbrOfPage)&&(FLASHStatus==FLASH_COMPLETE);EraseCounter++)
	{		
		FLASHStatus=FLASH_ErasePage(StartAddr+(PageSize*EraseCounter));	
	}
	
	Address = StartAddr;
	i=0;
	
	while((Address<EndAddr)&&(FLASHStatus==FLASH_COMPLETE))	
	{	FLASHStatus=FLASH_ProgramWord(Address,p_data[i++]);
		Address=Address+4;
	}
	
	Address = StartAddr;
	i=0;
	
	while((Address < EndAddr) && (MemoryProgramStatus != 0))
	{	
		if((*(vu32*) Address) != p_data[i++])
		{		MemoryProgramStatus = 0;
				return 1;
		}
		Address += 4;
	}
	
	return 0;
}



int flash_read(uint32_t StartAddr,uint32_t *p_data,uint32_t size)
{
	uint32_t EndAddr=StartAddr+size*4;
	int MemoryProgramStatus=1;
	uint32_t Address = 0x0;
	int i=0;
	Address = StartAddr;
	while((Address < EndAddr) && (MemoryProgramStatus != 0))
	{
		p_data[i++]=(*(vu32*) Address);
		Address += 4;	
	}
	return 0;	
}


/*------------------------------------------------------------------------------------------------------------------------//
Save config into flash

*/
void SaveData(void)
{
	unsigned char IsNeedSave = 0;
	Config_TypeDef _Config;
	if(IsDataNew == 1)
	{
//		I2C_Cmd(I2CPort,DISABLE);
		//--------------------------------------------------------------------//
		_Config.valid = PramReg[CONFIG_REG_VALID] + (PramReg[CONFIG_REG_VALID+1]<<8) + (PramReg[CONFIG_REG_VALID+2]<<16) + (PramReg[CONFIG_REG_VALID+3]<<24);
		_Config.I2C_OwnAddress = PramReg[CONFIG_REG_I2C_ADDR];
		_Config.I2C_Speed = PramReg[CONFIG_REG_I2C_SPEED] + (PramReg[CONFIG_REG_I2C_SPEED+1]<<8 );
		_Config.LED_Mode = (enum LED_MODE)PramReg[CONFIG_REG_LED_MODE];
		_Config.LED_FlashTime = PramReg[CONFIG_REG_LED_FLASH_TIME] + (PramReg[CONFIG_REG_LED_FLASH_TIME+1]<<8 );
		_Config.DataClearTime = PramReg[CONFIG_REG_DATA_CLEAR_TIME] + (PramReg[CONFIG_REG_DATA_CLEAR_TIME+1]<<8 );
		_Config.DataReadTime = PramReg[CONFIG_REG_DATA_READ_TIME] + (PramReg[CONFIG_REG_DATA_READ_TIME+1]<<8 );
		
		Config.valid = _Config.valid ;//== ConfigValid ? _Config.valid : Config.valid;
		if(_Config.I2C_OwnAddress > 0 && _Config.I2C_OwnAddress <= 127)
		{
			if(Config.I2C_OwnAddress != _Config.I2C_OwnAddress)
			{
				IsNeedSave = 1;
			  Config.I2C_OwnAddress = _Config.I2C_OwnAddress;
			}
		}
		if(_Config.I2C_Speed >= 1 && _Config.I2C_Speed <= 400)
		{
			
			if(Config.I2C_Speed != _Config.I2C_Speed)
			{
				IsNeedSave = 1;
			  Config.I2C_Speed = _Config.I2C_Speed;
			}
		}
		if(_Config.LED_Mode <= LED_MODE_NUM)
		{
			Config.LED_Mode = _Config.LED_Mode;
		}
		if(_Config.LED_FlashTime > 0 && _Config.LED_FlashTime <= 65535)
		{
			Config.LED_FlashTime = _Config.LED_FlashTime;
		}
		if(_Config.DataClearTime > 0 && _Config.DataClearTime <= 65535)
		{
			Config.DataClearTime = _Config.DataClearTime;
		}
		if(_Config.DataReadTime > 0 && _Config.DataReadTime <= 65535)
		{
			Config.DataReadTime = _Config.DataReadTime;
		}
		//--------------------------------------------------------------------//
		if(IsNeedSave || Config.valid != ConfigValid)
		{
      flash_write(ConfigAddr,(uint32_t*)&Config,(sizeof(Config_TypeDef)+2)/4);
			//system reset to load new data
			__set_FAULTMASK(1);
			NVIC_SystemReset();
		}
		IsDataNew = 0;
	}
}

/*------------------------------------------------------------------------------------------------------------------------//
system of sensor init

*/
void SysInit(void)
{
  SystemClockInit();	
	//
	//get config data from flash and  is it valid?if not , init "Config" to default value and flash the light
	//
	flash_read(ConfigAddr,(uint32_t*)&Config,(sizeof(Config_TypeDef)+2)/4);
	if(Config.valid != ConfigValid)
	{
	  memcpy(&Config,&DeConfigValue,sizeof(Config_TypeDef));
	}
	PramReg[CONFIG_REG_VALID] = Config.valid & 0x0FF;
	PramReg[CONFIG_REG_VALID+1] = (Config.valid>>8) & 0x0FF;
	PramReg[CONFIG_REG_VALID+2] = (Config.valid>>16) & 0x0FF;
	PramReg[CONFIG_REG_VALID+3] = (Config.valid>>24) & 0x0FF;
	PramReg[CONFIG_REG_I2C_ADDR] = Config.I2C_OwnAddress;
	PramReg[CONFIG_REG_I2C_SPEED] = Config.I2C_Speed & 0XFF;
	PramReg[CONFIG_REG_I2C_SPEED+1] = (Config.I2C_Speed>>8) & 0XFF;
	PramReg[CONFIG_REG_LED_MODE] = Config.LED_Mode;
	PramReg[CONFIG_REG_LED_FLASH_TIME] = Config.LED_FlashTime & 0XFF;
	PramReg[CONFIG_REG_LED_FLASH_TIME+1] = (Config.LED_FlashTime>>8) & 0XFF;
	PramReg[CONFIG_REG_DATA_CLEAR_TIME] = Config.DataClearTime& 0XFF;
	PramReg[CONFIG_REG_DATA_CLEAR_TIME+1] = (Config.DataClearTime>>8) & 0XFF;
	PramReg[CONFIG_REG_DATA_READ_TIME] = Config.DataReadTime & 0XFF;
	PramReg[CONFIG_REG_DATA_READ_TIME+1] = (Config.DataReadTime>>8) & 0XFF;
  //
  I2CInit(I2CPort,Config.I2C_OwnAddress,Config.I2C_Speed*1000);
	//
	//LED INIT
	//
	LEDInit();
	//
	//extern interrupt init
	//
	ExtiInit();
	//
	//INIT timer to CAPTURE INPUT
	//
	#if !defined EXIT_MODE
	TIM_IC_Init(TIM_UP);
	TIM_IC_Init(TIM_DOWN);
	TIM_IC_Init(TIM_LEFT);
	TIM_IC_Init(TIM_RIGHT);
	#else
	TIM2Init();
	TIM2_ChannelCMD(1,1);
	TIM2_ChannelCMD(2,1);
	TIM2_SetDuty(1,50);
	TIM2_SetDuty(2,50);
	#endif	
	
}



/*------------------------------------------------------------------------------------------------------------------------//
Main function

*/
int main(void)
{
	//重定向中断向量
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, ApplicationAddress-NVIC_VectTab_FLASH);	
	SysInit();
	
	while(1)
	{
		//
		//LED
		//
		LED_Flash();
		//
		//Get data
		//
		GetTimerCounterData();
		//
		//CLEAR DATA
		//
		ClearCounterData();
		//
		//it will save config data and reboot
		//
		SaveData();
	}
	
}


/*------------------------------------------------------------------------------------------------------------------------//
clock
minmun value is 1ms
	LED_ALWAYS_ON = 0X00,
	LED_ALWAYS_OFF,
	LED_FLASH,
	LED_MOVE_FLASH,
	LED_MODE_NUM
*/
void SysTick_Handler(void)
{
	localtime++;
}

