/*
 *	Basic LED defines header file
 *	@author Vino
 */

#ifndef VNO_LED_DRIVER_
#define VNO_LED_DRIVER_

// Includes
#include "stm32f4xx_hal.h"

// LED layout
/*
 * 	^ BOARD TOP
 *
 *	  R
 *	O + B
 *	  G
 */

// LED driver defines
#define LD_LED_GREEN	GPIO_PIN_12
#define LD_LED_ORANGE	GPIO_PIN_13
#define LD_LED_RED		GPIO_PIN_14
#define LD_LED_BLUE		GPIO_PIN_15
#define LED_PORT_GROUP 	GPIOD

#endif
