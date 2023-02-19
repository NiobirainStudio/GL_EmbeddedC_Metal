/*
 *	Basic LED driver header file
 *	@author Daniel Horhul
 */

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


// Possible blink modes
typedef enum {
	BM_ROUND_CYCLE = 0,

	BM_NO_MODE
} BlinkMode;


// Driver state structure
typedef struct LED_State {
	BlinkMode current_mode;
	uint16_t base_led;
	uint32_t blink_delay;
} LED_State;

// Set the current blink mode to a new one
void set_blink_mode(BlinkMode mode);

// Get the next iteration for the set blink mode
void blink_led();
