/*
 *	Basic controls header file
 *	@author Vino
 */

#ifndef VNO_CONTROLS_
#define VNO_CONTROLS_

// Includes
#include "UsrSrc/buttons.h"
#include "UsrSrc/leds.h"

// LED state structure
typedef struct LED_State {
	uint8_t leds[4];
} LED_State;


// Message type enum
typedef enum MSG_Type {
	MSG_IN,
	MSG_OUT
} MSG_Type;


// Switch LED
void switch_led(UART_HandleTypeDef* huart, MSG_Type msg_type, uint16_t led_code);

// Temperature value conversion
float get_temp_sensor_data(uint32_t variable);

#endif // VNO_CONTROLS_
