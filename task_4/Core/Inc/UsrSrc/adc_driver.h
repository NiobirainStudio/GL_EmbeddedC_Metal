/*
 *	Basic button defines header file
 *	@author Vino
 */

#ifndef VNO_ADC_DRIVER_
#define VNO_ADC_DRIVER_

// Includes
#include "stm32f4xx_hal.h"
#include "stm32f407xx.h"


// ADC driver defines
#define DUTY_CYCLE_MIN 0.1
#define DUTY_CYCLE_MAX 0.9
#define DUTY_CYCLE_STEP 0.05

// Analog toggle borders
#define ANALOG_PADDING	8
#define ANALOG_STEP		50
#define ANALOG_TEMP_INT 1030
#define ANALOG_TEMP_EXT 2070
#define ANALOG_POTENTIO	1000

// Other defines
#define TIMER	TIM4
#define CHANNEL_COUNT 3


// Driver state structure
typedef struct ADC_State {
	uint8_t	high_cases;
/*	^
 *  |
 *	0x00000111
 *	       ^^^
 *	       |||
 *	       +++-- Emergency bits (One for each emergency)
*/
	// Starting values that are considered as duty cycle 50%
	uint16_t base_channels[CHANNEL_COUNT];
} ADC_State;



/// General functions

// Update led intensity to emulate analog signal
void update_intensity();


// Emergency frequency change
void set_emergency_frequency();


/// OTHER

// Init the ADC
void init_adc();

// Get the next iteration for ADC
void adc_iter();

#endif // VNO_ADC_DRIVER_
