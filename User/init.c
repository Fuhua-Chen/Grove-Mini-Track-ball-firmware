#include "init.h"

/*------------------------------------------------------------------------------------------------------------------------//
systick init for timing to Clear

*/
void SystickInit(void)
{
	SysTick_Config(SystemCoreClock/1000);
}


/*------------------------------------------------------------------------------------------------------------------------//
clock init


*/
void SystemClockInit(void)
{
  RCC_DeInit();     
  RCC_HSEConfig(RCC_HSE_ON);
	while(!RCC_WaitForHSEStartUp());
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_2); 
	RCC_HCLKConfig(RCC_SYSCLK_Div1); 
	RCC_PCLK2Config(RCC_HCLK_Div1); 
	RCC_PCLK1Config(RCC_HCLK_Div2);              
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_4);   //HSE VALUE IS 16M
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
	SystemCoreClock = 64000000;	
	SystickInit();
}



/*------------------------------------------------------------------------------------------------------------------------//
GPIO init


*/
void GPIOInit(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef Mod)
{
  
	GPIO_InitTypeDef GPIO_MyInit;
	switch((uint32_t)GPIOx)
	{
	  case (uint32_t)GPIOA:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);break;
	  case (uint32_t)GPIOB:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);break;
		case (uint32_t)GPIOC:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);break;
		case (uint32_t)GPIOD:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);break;
		case (uint32_t)GPIOE:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);break;
	}
	GPIO_MyInit.GPIO_Pin   =  GPIO_Pin;
	GPIO_MyInit.GPIO_Mode  =  Mod;
	GPIO_MyInit.GPIO_Speed =  GPIO_Speed_2MHz;
	GPIO_Init(GPIOx,&GPIO_MyInit);

}

