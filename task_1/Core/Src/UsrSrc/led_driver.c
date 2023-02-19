/*
 *	Basic LED driver header file
 *	@author Daniel Horhul
 */

// Includes
#include "UsrSrc/led_driver.h"


// Local variable to store the LED state
static LED_State g_led_state = { 
	BM_ROUND_CYCLE, 
	0, 
	100 
};


// Round cycle blink mode iteration
static void _round_cycle_blink() {

	if (g_led_state.base_led == 0) {
		HAL_GPIO_WritePin(LED_PORT_GROUP, LD_LED_GREEN, 	GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT_GROUP, LD_LED_ORANGE, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT_GROUP, LD_LED_RED, 	GPIO_PIN_SET);

		g_led_state.base_led = LD_LED_BLUE;
	}

	// Turn on the new LED
	HAL_GPIO_WritePin(LED_PORT_GROUP, g_led_state.base_led, GPIO_PIN_SET);

	// Go to the next LED
	if (g_led_state.base_led == LD_LED_BLUE)
		g_led_state.base_led = LD_LED_GREEN;
	else
		g_led_state.base_led *= 2;

	// Turn off the last LED
	HAL_GPIO_WritePin(LED_PORT_GROUP, g_led_state.base_led, GPIO_PIN_RESET);

	// Delay the operation
	HAL_Delay(g_led_state.blink_delay);
}



// Get the next iteration for the set blink mode
void blink_led() {

	switch (g_led_state.current_mode) {
	case BM_ROUND_CYCLE:
		_round_cycle_blink();
		break;

	case BM_NULL:
		break;

	default:
		// Unknown mode
		break;
	}
}
