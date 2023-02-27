/*
 *	Basic PWM driver header file
 *	@author Vino
 */

#ifndef VNO_PWM_DRIVER_
#define VNO_PWM_DRIVER_

// Includes
#include "stm32f407xx.h"
#include "leds.h"

// PWM driver defines
#define PWM_FREQ_START			1000 	// (1kHz)
#define PWM_FREQ_STEP			5000 	// (5kHz)
#define PWM_FREQ_LIMIT			30000	// (30kHz)

#define PWM_DUTY_CYCLE_START	0.5		// (%/100 of freq)
#define PWM_DUTY_CYCLE_STEP		0.05 	// (%/100 of freq)
#define PWM_DUTY_CYCLE_LIMIT	0.95	// (%/100 of freq)

#define PWM_ARR					(uint32_t)250		// (Blinking on 4Hz)

#define TIMER					TIM4


// Driver state structure
typedef struct PWM_State {
	// Disable/enable the output
	uint8_t		is_on;

	/*
	 * Where the signal is going
	 *
	 * Channel name		Defined code	Output LED	LED color
	 * TIM_CHANNEL_1	(0x00000000U)	PD12		Green
	 * TIM_CHANNEL_2	(0x00000004U)	PD13		Orange
	 * TIM_CHANNEL_3	(0x00000008U)	PD14		Red
	 * TIM_CHANNEL_4	(0x0000000CU)	PD15		Blue
	*/
	uint32_t	current_output;

	// Frequency change
	uint32_t	frequency;

	// Duty cycle percent
	double		duty_cycle_percent;
} PWM_State;



/// FREQUENCY CHANGE FUNCTIONS

// Increase the frequency by PWM_FREQ_STEP
void freq_up();

// Decrease the frequency by PWM_FREQ_STEP
void freq_down();


/// DUTY CYCLE CHANGE FUNCTIONS

// Increase the duty cycle by PWM_DUTY_CYCLE_STEP
void duty_cycle_up();

// Decrease the duty cycle by PWM_DUTY_CYCLE_STEP
void duty_cycle_down();


/// OTHER

// Init the PWM
void init_pwm(TIM_HandleTypeDef* timer_handle);

// Chose the next output
void next_output(TIM_HandleTypeDef* timer_handle);

#endif
