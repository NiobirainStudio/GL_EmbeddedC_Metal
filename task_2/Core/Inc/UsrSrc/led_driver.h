/*
 *	Basic LED driver header file
 *	@author Daniel Horhul
 */

#ifndef DREADVER_LED_DRIVER_
#define DREADVER_LED_DRIVER_

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

// Delays (in milliseconds)
#define BLK_DELAY_STEP	50
#define BLK_DELAY_START	100
#define BLK_DELAY_MIN	BLK_DELAY_STEP
#define BLK_DELAY_MAX	BLK_DELAY_STEP * 5



// Possible blink modes
typedef enum {
	BM_ROUND_CYCLE = 0,
	BM_LEAP,
	BM_WINK,

	//! DO NOT USE AS AN ACTUAL MODE!
	BM_NULL
} BlinkMode;


// Driver state structure
typedef struct LED_State {
	BlinkMode 	current_mode;
	uint8_t 	is_on;
	uint16_t 	base_led;
	uint32_t 	blink_delay;
} LED_State;


/// BLINKING SPEED CHANGE FUNCTIONS

// Blinking speed up function
void blink_speed_up();

// Blinking speed down function
void blink_speed_down();


/// BLINKING MODE CHANGE FUNCTIONS

// Select the next blinking mode
void blink_mode_next();

// Select the previous blinking mode
void blink_mode_prev();


/// OTHER

// Turn the blinking on/off
void toggle_operation();

// Get the next iteration for the set blink mode
void blink_led();

#endif
