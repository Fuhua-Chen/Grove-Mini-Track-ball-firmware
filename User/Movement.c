#include "main.h"


/*------------------------------------------------------------------------------------------------------------------------//
timer input capture mode init

*/
void TIM_IC_Init(TIM_TypeDef* TIMx)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	uint16_t TIM_IT;
	//enum IRQn TIM_IRQn;
	
	TIM_DeInit(TIMx);
	TIM_ICStructInit(&TIM_ICInitStructure);
	
	switch((uint32_t)TIMx)
	{
		case (uint32_t)TIM1:
	    GPIOInit(GPIOA,GPIO_Pin_8,GPIO_Mode_IN_FLOATING);
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
		  TIM_TIxExternalClockConfig(TIMx,TIM_TIxExternalCLK1Source_TI1ED,TIM_ICPolarity_BothEdge,0x02);
		  //TIM_IRQn = TIM1_CC_IRQn;
		  TIM_IT = TIM_IT_CC1;
			break;
		case (uint32_t)TIM2:
	    GPIOInit(GPIOA,GPIO_Pin_1,GPIO_Mode_IN_FLOATING);
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;TIM_TIxExternalClockConfig(TIMx,TIM_TIxExternalCLK1Source_TI2,TIM_ICPolarity_BothEdge,0x02);
		  //TIM_IRQn = TIM2_IRQn;
		  TIM_IT = TIM_IT_CC2;
			break;
		case (uint32_t)TIM3:
	    GPIOInit(GPIOA,GPIO_Pin_6,GPIO_Mode_IN_FLOATING);
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;TIM_TIxExternalClockConfig(TIMx,TIM_TIxExternalCLK1Source_TI1ED,TIM_ICPolarity_BothEdge,0x02);
		  //TIM_IRQn = TIM3_IRQn;
		  TIM_IT = TIM_IT_CC1;
			break;
		case (uint32_t)TIM4:
	    GPIOInit(GPIOB,GPIO_Pin_9,GPIO_Mode_IN_FLOATING);
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);TIM_TIxExternalClockConfig(TIMx,TIM_TIxExternalCLK1Source_TI2,TIM_ICPolarity_BothEdge,0x02);
	    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
		  //TIM_IRQn = TIM4_IRQn;
		  TIM_IT = TIM_IT_CC2;
			break;
	}
	
	TIM_TimeBaseStructure.TIM_Period = 0XFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);
	 
	TIM_ICInitStructure.TIM_ICFilter = 0x08;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	
  TIM_ICInit(TIMx,&TIM_ICInitStructure);

	TIM_SetCounter(TIMx,0);
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);
  TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
  TIM_ClearITPendingBit(TIMx, TIM_IT);
	
  TIM_Cmd(TIMx, ENABLE);
	
}


/*------------------------------------------------------------------------------------------------------------------------//
extern interrupt init


*/
void ExtiInit(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	EXTI_DeInit();
	GPIOInit(GPIOB,GPIO_Pin_0,GPIO_Mode_IPU);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	EXTI_ClearITPendingBit(EXTI_Line0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_EnableIRQ(EXTI0_IRQn);
	
#ifdef EXIT_MODE
	GPIOInit(GPIOA,GPIO_Pin_1,GPIO_Mode_IN_FLOATING);
	EXTI_ClearITPendingBit(EXTI_Line1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_EnableIRQ(EXTI1_IRQn);	
	
	GPIOInit(GPIOA,GPIO_Pin_6,GPIO_Mode_IN_FLOATING);
	EXTI_ClearITPendingBit(EXTI_Line6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
	
	GPIOInit(GPIOA,GPIO_Pin_8,GPIO_Mode_IN_FLOATING);
	EXTI_ClearITPendingBit(EXTI_Line1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
	
	
	GPIOInit(GPIOB,GPIO_Pin_7,GPIO_Mode_IN_FLOATING);
	EXTI_ClearITPendingBit(EXTI_Line7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
#endif
	
}


/*------------------------------------------------------------------------------------------------------------------------//
exit0 interrupt for key "confirm"

*/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)      
   {      
	   EXTI_ClearITPendingBit(EXTI_Line0); 
		 EXTI->IMR &= ~EXTI_Line0;EXTI->EMR &= ~EXTI_Line0;
     PramReg[MOTION_REG_CONFIRM] ++;	
		 EXTI->IMR |= EXTI_Line0;EXTI->EMR |= EXTI_Line0;
	 }
}

#ifdef EXIT_MODE

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)!= RESET)      
   {            
	   EXTI_ClearITPendingBit(EXTI_Line1);
		 EXTI->IMR &= ~EXTI_Line1;EXTI->EMR &= ~EXTI_Line1;
     PramReg[MOTION_REG_LEFT] ++;
		 EXTI->IMR |= EXTI_Line1;EXTI->EMR |= EXTI_Line1;
	 }
}


void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line6)!= RESET)      
   {            
	   EXTI_ClearITPendingBit(EXTI_Line6);
		 EXTI->IMR &= ~EXTI_Line6;EXTI->EMR &= ~EXTI_Line6;
     PramReg[MOTION_REG_UP] ++;
		 EXTI->IMR |= EXTI_Line6;EXTI->EMR |= EXTI_Line6;
	 }
	if(EXTI_GetITStatus(EXTI_Line7)!= RESET)      
   {            
	   EXTI_ClearITPendingBit(EXTI_Line7);
		 EXTI->IMR &= ~EXTI_Line7;EXTI->EMR &= ~EXTI_Line7;
     PramReg[MOTION_REG_RIGHT] ++;
		 EXTI->IMR |= EXTI_Line7;EXTI->EMR |= EXTI_Line7;
	 }
	if(EXTI_GetITStatus(EXTI_Line8)!= RESET)      
   {            
	   EXTI_ClearITPendingBit(EXTI_Line8);
		 EXTI->IMR &= ~EXTI_Line8;EXTI->EMR &= ~EXTI_Line8;
     PramReg[MOTION_REG_DOWN] ++;
		 EXTI->IMR |= EXTI_Line8;EXTI->EMR |= EXTI_Line8;
	 }
}

#endif

/*------------------------------------------------------------------------------------------------------------------------//
Get Counter

*/
void GetTimerCounterData(void)
{
	#if !defined EXIT_MODE
	static uint64_t Counter;	
	if(localtime - Counter >= Config.DataReadTime)
	{
		PramReg[MOTION_REG_UP] = TIM_GetCounter(TIM_UP);
		PramReg[MOTION_REG_DOWN] = TIM_GetCounter(TIM_DOWN);
		PramReg[MOTION_REG_LEFT] = TIM_GetCounter(TIM_LEFT);
		PramReg[MOTION_REG_RIGHT] = TIM_GetCounter(TIM_RIGHT);	
		TIM_SetCounter(TIM_UP,0);
		TIM_SetCounter(TIM_DOWN,0);
		TIM_SetCounter(TIM_LEFT,0);
		TIM_SetCounter(TIM_RIGHT,0);
		Counter = localtime;
	}
	#endif
}
/*------------------------------------------------------------------------------------------------------------------------//
Clear Data

*/
void ClearCounterData(void)
{
	static uint64_t Counter;

	if(localtime - Counter >= Config.DataClearTime)
	{
		memset(PramReg,0,MOTION_REG_NUM);	
		
		#if !defined EXIT_MODE
		TIM_SetCounter(TIM_UP,0);
		TIM_SetCounter(TIM_DOWN,0);
		TIM_SetCounter(TIM_LEFT,0);
		TIM_SetCounter(TIM_RIGHT,0);
		#endif
		
		Counter = localtime;
	}
}
