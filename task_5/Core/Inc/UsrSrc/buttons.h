/*
 *	Basic button defines header file
 *	@author Vino
 */

#ifndef VNO_BUTTONS_
#define VNO_BUTTONS_

// Includes
#include "stm32f4xx_hal.h"

// Button layout
/*
 * 	^ BOARD TOP
 *
 *	SWTx
 *	  4
 *	3 2 1
 *	  5
 */

// Button defines
#define SWT1    GPIO_PIN_11
#define SWT3	GPIO_PIN_9
#define SWT4	GPIO_PIN_6
#define SWT5	GPIO_PIN_8

#endif
