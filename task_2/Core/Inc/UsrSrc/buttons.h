/*
 *	Basic button defines header file
 *	@author Daniel Horhul
 */

#ifndef DREADVER_BUTTONS_
#define DREADVER_BUTTONS_

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
#define SWT2	GPIO_PIN_15
#define SWT3	GPIO_PIN_9
#define SWT4	GPIO_PIN_6
#define SWT5	GPIO_PIN_8

#endif
