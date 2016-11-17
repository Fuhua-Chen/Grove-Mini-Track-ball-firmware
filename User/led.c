#include "main.h"


unsigned int Tim2_fre = 1000;

/*------------------------------------------------------------------------------------------------------------------------//

//TIM2 Set for Breathing LED

*/
void TIM2_SetDuty(unsigned char Channel, unsigned int Duty)
{
	uint16_t Period;
	Period =  (SystemCoreClock+Tim2_fre/2)/Tim2_fre-1; 
	if(Channel == 1)
	{
    TIM2->CCR3 = Period*Duty/100;
	}
	else if(Channel == 2)
	{
    TIM2->CCR4 = Period*Duty/100;
	}
}

void TIM2_ChannelCMD(unsigned char Channel, unsigned int State)
{	
	if(Channel == 1)
	{
    TIM2->CCER |=(uint16_t)(State << 8);
	}
	else if(Channel == 2)
	{
	  TIM2->CCER |= (uint16_t)(State << 12);
	}
}

void TIM2Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	uint16_t Period;
	
	Period =  (SystemCoreClock+Tim2_fre/2)/Tim2_fre-1; 
	TIM_DeInit(TIM2); 
	
	GPIOInit(GPIOA,GPIO_Pin_2,GPIO_Mode_AF_PP);
	GPIOInit(GPIOA,GPIO_Pin_3,GPIO_Mode_AF_PP);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); 
	
	TIM_TimeBaseStructInit(& TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_OCStructInit(& TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_Pulse = Period ; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	
  TIM_OC3Init(TIM2,&TIM_OCInitStructure);
  TIM_OC4Init(TIM2,&TIM_OCInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_Cmd(TIM2, ENABLE);
  TIM_CtrlPWMOutputs(TIM2, ENABLE);
}
/*------------------------------------------------------------------------------------------------------------------------//
LED INIT

*/
void LEDInit(void)
{
	GPIOInit(LED_PORT,LED2_PIN,GPIO_Mode_Out_PP);
	GPIOInit(LED_PORT,LED1_PIN,GPIO_Mode_Out_PP);
	GPIO_SetBits(LED_PORT,LED2_PIN);
	GPIO_SetBits(LED_PORT,LED1_PIN);
}


/*------------------------------------------------------------------------------------------------------------------------//
led's work mode
enum LED_MODE
{
	LED_FLASH_1 = 0X00,
	LED_FLASH_2,
	LED_FLASH_TOGGLE,
	LED_FLASH_ALL,
	LED_ALWAYS_ON_1,
	LED_ALWAYS_ON_2, 
	LED_ALWAYS_ON_ALL,
	LED_ALWAYS_OFF,
	LED_BREATHING_1,
	LED_BREATHING_2,
	LED_BREATHING_ALL,
	LED_MOVE_FLASH,
	LED_MODE_NUM
};
*/
void LED_Flash(void)
{
	static uint64_t  LED_Counter;
	
	#ifdef EXIT_MODE
	static uint8_t IsIncrease = 1;
  static uint32_t  Breathing_Counter;
	
	if(IsIncrease)
	{
		Breathing_Counter+=1;
	}
	else
	{
		Breathing_Counter-=1;
	}
	
	if(Breathing_Counter == 500000)
	{
		IsIncrease = 0;
	}
	else if(Breathing_Counter == 0)
	{
		IsIncrease = 1;
	}
	#endif	
			
	switch(Config.LED_Mode)
	{
		case LED_FLASH_1:
			if(localtime - LED_Counter >= Config.LED_FlashTime/2 && localtime - LED_Counter < Config.LED_FlashTime)
			{
				#ifdef EXIT_MODE
			  TIM2_SetDuty(1,100);
			  TIM2_SetDuty(2,0);
				#else
				LED1_TOGGLE();
				LED2_OFF();
				#endif
			}
			else if(localtime - LED_Counter < Config.LED_FlashTime/2)
			{
				#ifdef EXIT_MODE
			  TIM2_SetDuty(1,0);
			  TIM2_SetDuty(2,0);
				#else
				LED1_TOGGLE();
				LED2_OFF();
				#endif
			}
			else if(localtime - LED_Counter >= Config.LED_FlashTime)
			{
				LED_Counter = localtime;
			}
			break;
		case LED_FLASH_2:
			if(localtime - LED_Counter >= Config.LED_FlashTime/2 && localtime - LED_Counter < Config.LED_FlashTime)
			{
				#ifdef EXIT_MODE
			  TIM2_SetDuty(1,0);
			  TIM2_SetDuty(2,100);
				#else
				LED2_TOGGLE();
				LED1_OFF();
				#endif
			}
			else if(localtime - LED_Counter < Config.LED_FlashTime/2)
			{
				#ifdef EXIT_MODE
			  TIM2_SetDuty(1,0);
			  TIM2_SetDuty(2,0);
				#else
				LED2_TOGGLE();
				LED1_OFF();
				#endif
			}
			else if(localtime - LED_Counter >= Config.LED_FlashTime)
			{
				LED_Counter = localtime;
			}
			break;
		case LED_FLASH_TOGGLE:
			if(localtime - LED_Counter >= Config.LED_FlashTime/2 && localtime - LED_Counter < Config.LED_FlashTime)
			{
				#ifdef EXIT_MODE
			  TIM2_SetDuty(1,100);
			  TIM2_SetDuty(2,0);
				#else
				LED1_ON();
				LED2_OFF();
				#endif
			}
			else if(localtime - LED_Counter < Config.LED_FlashTime/2)
			{
				#ifdef EXIT_MODE
			  TIM2_SetDuty(1,0);
			  TIM2_SetDuty(2,100);
				#else
				LED2_ON();
				LED1_OFF();
				#endif
			}
			else if(localtime - LED_Counter >= Config.LED_FlashTime)
			{
				LED_Counter = localtime;
			}
			break;
		case LED_FLASH_ALL:	
			if(localtime - LED_Counter >= Config.LED_FlashTime/2 && localtime - LED_Counter < Config.LED_FlashTime)
			{
				#ifdef EXIT_MODE
			  TIM2_SetDuty(1,100);
			  TIM2_SetDuty(2,100);
				#else
				LED_TOGGLE();
				#endif
			}
			else if(localtime - LED_Counter < Config.LED_FlashTime/2)
			{
				#ifdef EXIT_MODE
			  TIM2_SetDuty(1,0);
			  TIM2_SetDuty(2,0);
				#else
				LED_TOGGLE();
				#endif
			}
			else if(localtime - LED_Counter >= Config.LED_FlashTime)
			{
				LED_Counter = localtime;
			}
			break;
		case LED_ALWAYS_ON_1:
			#ifdef EXIT_MODE
			TIM2_SetDuty(1,100);
			TIM2_SetDuty(2,0);
			#else
			LED1_ON();
			LED2_OFF();
		  #endif 
			break;
		case LED_ALWAYS_ON_2:
			#ifdef EXIT_MODE
			TIM2_SetDuty(1,0);
			TIM2_SetDuty(2,100);
			#else
			LED2_ON();
			LED1_OFF();
		  #endif 
			break;
		case LED_ALWAYS_ON_ALL:
			#ifdef EXIT_MODE
			TIM2_SetDuty(1,100);
			TIM2_SetDuty(2,100);
			#else
			LED1_ON();
			LED2_ON();
		  #endif 
			break;
		case LED_ALWAYS_OFF:
			#ifdef EXIT_MODE
			TIM2_SetDuty(1,0);
			TIM2_SetDuty(2,0);
			#else
			LED1_OFF();
			LED2_OFF();
		  #endif 
			break;
		case LED_BREATHING_1:
		  #ifdef EXIT_MODE
			TIM2_SetDuty(1,Breathing_Counter/5000);
			TIM2_SetDuty(2,0);
			#else
			LED1_ON();
			LED2_ON();
		  #endif
			break;
		case LED_BREATHING_2:
		  #ifdef EXIT_MODE
			TIM2_SetDuty(2,Breathing_Counter/5000);
			TIM2_SetDuty(1,0);
			#else
			LED1_ON();
			LED2_ON();
		  #endif
			break;
		case LED_BREATHING_ALL:
		  #ifdef EXIT_MODE
			TIM2_SetDuty(1,Breathing_Counter/5000);
			TIM2_SetDuty(2,Breathing_Counter/5000);
			#else
			LED1_ON();
			LED2_ON();
		  #endif
			break;
		case LED_MOVE_FLASH:
			if(localtime - LED_Counter >= 100/2 && localtime - LED_Counter < 100)
			{
				if((PramReg[MOTION_REG_UP] | PramReg[MOTION_REG_DOWN] | PramReg[MOTION_REG_LEFT] | PramReg[MOTION_REG_RIGHT] | PramReg[MOTION_REG_CONFIRM]) != 0 )
				{
					#ifdef EXIT_MODE
					TIM2_SetDuty(1,100);
					TIM2_SetDuty(2,100);
					#else
					LED_TOGGLE();
					#endif 
				}	
			}
			else if(localtime - LED_Counter < 100/2)
			{
				if((PramReg[MOTION_REG_UP] | PramReg[MOTION_REG_DOWN] | PramReg[MOTION_REG_LEFT] | PramReg[MOTION_REG_RIGHT] | PramReg[MOTION_REG_CONFIRM]) != 0 )
				{
					#ifdef EXIT_MODE
					TIM2_SetDuty(1,0);
					TIM2_SetDuty(2,0);
					#else
					LED_TOGGLE();
					#endif 
				}	
			}
			else if(localtime - LED_Counter >= 100)
			{
				TIM2_SetDuty(1,0);
				TIM2_SetDuty(2,0);
				LED_Counter = localtime;
			}
			break;
		default:
			break;
	}
		 
}

