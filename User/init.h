#ifndef _INIT_H_
#define _INIT_H_

#include "main.h"

void SystemClockInit(void);

void GPIOInit(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef Mod);

void ExtiInit(void);

void TimerInit(TIM_TypeDef* Timer , uint16_t us);


#endif
