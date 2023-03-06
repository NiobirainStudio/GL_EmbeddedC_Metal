/*
 *	Basic controls source file
 *	@author Vino
 */

// Includes
#include "UsrSrc/controls.h"

// State struct
static LED_State g_led_state = {{ 0, 0, 0, 0}};

// Statics
static const char * msg_types[] 	= { "IN", "OUT" };
static const char * led_colors[] 	= { "GREEN", "ORANGE", "RED", "BLUE"};
static const char * led_state[]		= { "OFF", "ON" };

// Switch LED
void switch_led(UART_HandleTypeDef* huart, MSG_Type msg_type, uint16_t led_code) {
	char buff[] = "[   ]:           \r\n";

	// Message type
	for (uint8_t i = 0; i < 3; i++) {
		char tmp = msg_types[msg_type][i];
		if (tmp == '\0')
			break;

		buff[i + 1] = tmp;
	}

	uint8_t indx = 4;

	switch (led_code) {
	case LD_LED_GREEN:
		indx = 0;
		break;
	case LD_LED_ORANGE:
		indx = 1;
		break;
	case LD_LED_RED:
		indx = 2;
		break;
	case LD_LED_BLUE:
		indx = 3;
		break;
	default:
		// Unknown code
		break;
	}

	// LED colors
	for (uint8_t i = 0; i < 6; i++) {
		char tmp = led_colors[indx][i];
		if (tmp == '\0')
			break;

		buff[i + 7] = tmp;
	}

	// LED state
	if (g_led_state.leds[indx] == 0)
		g_led_state.leds[indx] = 1;
	else
		g_led_state.leds[indx] = 0;

	for (uint8_t i = 0; i < 3; i++) {
		char tmp = led_state[g_led_state.leds[indx]][i];
		if (tmp == '\0')
			break;

		buff[i + 14] = tmp;
	}

	// Toggle LED
	HAL_GPIO_TogglePin(LED_PORT_GROUP, led_code);

	// Transmit data
	HAL_UART_Transmit(huart, (uint8_t*)buff, 18+2, 10);
}

// !!!WIP!!!
#define V25 		.76
#define VSENSE 		3.3/4096
#define Avg_Slope 	.29

// Temperature value conversion
float get_temp_sensor_data (uint32_t variable) {
	return ((((variable*VSENSE) - V25) / Avg_Slope) + 25);
}
