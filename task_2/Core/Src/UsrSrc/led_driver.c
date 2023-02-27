/*
 *	Basic LED driver source file
 *	@author Vino
 */

// Includes
#include "UsrSrc/led_driver.h"


// Local variable to store the LED driver state
static LED_State g_led_state = {
		BM_ROUND_CYCLE,
		1,
		0,
		BLK_DELAY_START
};


static void _reset_leds() {
	// Reset LEDs
	HAL_GPIO_WritePin(LED_PORT_GROUP, LD_LED_GREEN,		GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT_GROUP, LD_LED_ORANGE, 	GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT_GROUP, LD_LED_RED, 		GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT_GROUP, LD_LED_BLUE, 		GPIO_PIN_RESET);

	// Reset init flag
	g_led_state.base_led = 0;
}


/// BLINKING SPEED CHANGE FUNCTIONS

// Blinking speed up function
void blink_speed_up() {
	if (g_led_state.blink_delay > BLK_DELAY_MIN)
		g_led_state.blink_delay -= BLK_DELAY_STEP;
}

// Blinking speed down function
void blink_speed_down() {
	if (g_led_state.blink_delay < BLK_DELAY_MAX)
		g_led_state.blink_delay += BLK_DELAY_STEP;
}


/// BLINKING MODE CHANGE FUNCTIONS

// Select the next blinking mode
void blink_mode_next() {

	// Reset LEDs
	_reset_leds();

	// Switch to the next blink mode
	if (g_led_state.current_mode + 1 == BM_NULL) {
		g_led_state.current_mode = 0;
		return;
	}

	g_led_state.current_mode += 1;
}

// Select the previous blinking mode
void blink_mode_prev() {

	// Reset LEDs
	_reset_leds();

	// Switch to the prev blink mode
	if (g_led_state.current_mode == 0 && BM_NULL > 0) {
		g_led_state.current_mode = BM_NULL - 1;
		return;
	}

	g_led_state.current_mode -= 1;
}


/// OTHER

// Turn the blinking on/off
void toggle_operation() {
	if (g_led_state.is_on == 0) {
		g_led_state.is_on = 1;
		return;
	}
	g_led_state.is_on = 0;
}



/// BLINKING MODE FUNCTIONS

// Round cycle blink mode iteration
static void _round_cycle_blink_iter() {

	// Init mode
	if (g_led_state.base_led == 0) {
		HAL_GPIO_WritePin(LED_PORT_GROUP, LD_LED_GREEN, 	GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT_GROUP, LD_LED_ORANGE, 	GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT_GROUP, LD_LED_RED, 		GPIO_PIN_SET);

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
}

// Leap mode iteration
static void _leap_blink_iter() {
	// Init mode
	if (g_led_state.base_led == 0) {
		HAL_GPIO_WritePin(LED_PORT_GROUP, LD_LED_GREEN, 	GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT_GROUP, LD_LED_RED, 		GPIO_PIN_SET);

		g_led_state.base_led = LD_LED_BLUE;
	}

	// Switch all LEDs
	HAL_GPIO_TogglePin(LED_PORT_GROUP, LD_LED_GREEN);
	HAL_GPIO_TogglePin(LED_PORT_GROUP, LD_LED_ORANGE);
	HAL_GPIO_TogglePin(LED_PORT_GROUP, LD_LED_RED);
	HAL_GPIO_TogglePin(LED_PORT_GROUP, LD_LED_BLUE);
}

// Wink mode iteration
static void _wink_blink_iter() {
	// Switch all LEDs
	HAL_GPIO_TogglePin(LED_PORT_GROUP, LD_LED_GREEN);
	HAL_GPIO_TogglePin(LED_PORT_GROUP, LD_LED_ORANGE);
	HAL_GPIO_TogglePin(LED_PORT_GROUP, LD_LED_RED);
	HAL_GPIO_TogglePin(LED_PORT_GROUP, LD_LED_BLUE);
}


// Get the next iteration for the set blink mode
void blink_led() {

	// If on state is false do nothing
	if (!g_led_state.is_on)
		return;

	// Find the current state and get the next iteration
	switch (g_led_state.current_mode) {
	case BM_ROUND_CYCLE:
		_round_cycle_blink_iter();
		break;

	case BM_LEAP:
		_leap_blink_iter();
		break;

	case BM_WINK:
		_wink_blink_iter();
		break;

	case BM_NULL:
		break;

	default:
		// Unknown mode
		break;
	}

	// Delay before the next iteration
	HAL_Delay(g_led_state.blink_delay);
}
