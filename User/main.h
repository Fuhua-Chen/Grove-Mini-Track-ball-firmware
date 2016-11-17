#ifndef _MAIN_H_
#define _MAIN_H_
/*------------------------------------------------------------------------------------------------------------------------//
include file

*/
#include "string.h"
#include "stm32f10x_conf.h"

/*------------------------------------------------------------------------------------------------------------------------//
define data

*/
#define ApplicationAddress    0x8005000

#define ConfigAddr  0X800FC00
#define ConfigValid 0x3a6fb67c

#define DefaultI2CAddr 0X4A
#define MaxLength 128

#define EXIT_MODE
/*------------------------------------------------------------------------------------------------------------------------//
type define

*/
//
//enum type
//
enum MOTION_REG_ADDR
{
  MOTION_REG_UP = 0X00,
  MOTION_REG_DOWN,
  MOTION_REG_LEFT,
  MOTION_REG_RIGHT,
	MOTION_REG_CONFIRM,
  MOTION_REG_NUM
};

enum CONFIG_REG_ADDR
{
	CONFIG_REG_VALID = MOTION_REG_NUM,
	CONFIG_REG_I2C_ADDR = CONFIG_REG_VALID + 4,
	CONFIG_REG_I2C_SPEED,
	CONFIG_REG_LED_MODE = CONFIG_REG_I2C_SPEED + 2,
	CONFIG_REG_LED_FLASH_TIME,
	CONFIG_REG_DATA_CLEAR_TIME = CONFIG_REG_LED_FLASH_TIME + 2,  //CONFIG_REG_LED_FLASH_TIME has 2bytes
	CONFIG_REG_DATA_READ_TIME = CONFIG_REG_DATA_CLEAR_TIME + 2,
	CONFIG_REG_NUM = CONFIG_REG_DATA_READ_TIME + 2
};

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

//
//Config Data
//
typedef struct
{
	uint32_t valid;
  //
	//I2C config
	//
	uint8_t  I2C_OwnAddress;
	uint16_t I2C_Speed;
	//
	//LED
	//
	enum LED_MODE LED_Mode;
	uint16_t LED_FlashTime;
	//
	//Data Clear Time
	//
	uint16_t DataClearTime;
	uint16_t DataReadTime;
	
}Config_TypeDef;

/*------------------------------------------------------------------------------------------------------------------------//
extern data

*/
extern Config_TypeDef Config;
extern uint8_t PramReg[CONFIG_REG_NUM];
extern volatile uint64_t localtime;

extern uint8_t IsDataNew;
/*------------------------------------------------------------------------------------------------------------------------//
function

*/
uint16_t CRC16( const void * const Block, uint32_t BlockSize );
	

#include "init.h"
#include "led.h"
#include "Communication.h"
#include "Movement.h"
#include "software_uart.h"

#endif

