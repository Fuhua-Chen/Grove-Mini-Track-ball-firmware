#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include "stm32f10x_conf.h"

#define TIM_UP     TIM3
#define TIM_UP_IRQHandler TIM3_CC_IRQHandler

#define TIM_DOWN   TIM1
#define TIM_DOWM_IRQHandler TIM1_IRQHandler

#define TIM_LEFT   TIM2
#define TIM_LEFT_IRQHandler TIM2_IRQHandler

#define TIM_RIGHT TIM4
#define TIM_RIGHT_IRQHandler TIM4_IRQHandler

void TIM_IC_Init(TIM_TypeDef* TIMx);
void GetTimerCounterData(void);
void ClearCounterData(void);

#endif


