#ifndef _LED_H_
#define _LED_H_

#define LED_PORT  GPIOA
#define LED1_PIN  GPIO_Pin_2
#define LED2_PIN  GPIO_Pin_3

#define LED1_OFF() GPIO_ResetBits(LED_PORT,LED1_PIN)
#define LED1_ON()  GPIO_SetBits(LED_PORT,LED1_PIN)
#define LED1_TOGGLE() GPIO_WriteBit(LED_PORT,LED1_PIN,!(GPIO_ReadOutputDataBit(LED_PORT,LED1_PIN)))


#define LED2_OFF() GPIO_ResetBits(LED_PORT,LED2_PIN)
#define LED2_ON()  GPIO_SetBits(LED_PORT,LED2_PIN)
#define LED2_TOGGLE() GPIO_WriteBit(LED_PORT,LED2_PIN,!(GPIO_ReadOutputDataBit(LED_PORT,LED2_PIN)))

#define LED_ON()   GPIO_SetBits(LED_PORT,LED1_PIN);GPIO_SetBits(LED_PORT,LED2_PIN)
#define LED_OFF()  GPIO_ResetBits(LED_PORT,LED1_PIN);GPIO_ResetBits(LED_PORT,LED2_PIN)
#define LED_TOGGLE() GPIO_WriteBit(LED_PORT,LED1_PIN,(BitAction)!(GPIO_ReadOutputDataBit(LED_PORT,LED1_PIN)));\
                     GPIO_WriteBit(LED_PORT,LED2_PIN,(BitAction)!(GPIO_ReadOutputDataBit(LED_PORT,LED2_PIN)))



void LEDInit(void);
void LED_Flash(void);
void TIM2_SetDuty(unsigned char Channel, unsigned int Duty);
void TIM2Init(void);
void TIM2_ChannelCMD(unsigned char Channel, unsigned int State);
#endif


