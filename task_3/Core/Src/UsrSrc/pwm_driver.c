/*
 *	Basic PWM driver source file
 *	@author Vino
 */

// Includes
#include "UsrSrc/pwm_driver.h"

static PWM_State g_pwm_state = {
		1,
		TIM_CHANNEL_4,
		PWM_FREQ_START,
		PWM_DUTY_CYCLE_START
};



/// FREQUENCY CHANGE FUNCTIONS

static void _vno_recalc_pwm() {
	// Get new register values
	uint32_t ARRn = (PWM_ARR * PWM_FREQ_START) / g_pwm_state.frequency;
	uint32_t CCRn = (uint32_t)(ARRn * g_pwm_state.duty_cycle_percent);

	// Update register values
	TIMER->ARR = ARRn;

	switch (g_pwm_state.current_output) {
	case TIM_CHANNEL_1:
		TIMER->CCR1 = CCRn;
		break;
	case TIM_CHANNEL_2:
		TIMER->CCR2 = CCRn;
		break;
	case TIM_CHANNEL_3:
		TIMER->CCR3 = CCRn;
		break;
	case TIM_CHANNEL_4:
		TIMER->CCR4 = CCRn;
		break;
	}
}



// Increase the frequency by PWM_FREQ_STEP
void freq_up() {
	// Limit reached
	if (g_pwm_state.frequency + PWM_FREQ_STEP >= PWM_FREQ_LIMIT)
		return;

	// Increase the frequency value
	g_pwm_state.frequency += PWM_FREQ_STEP;


	// Recalculate the PWM value and set the registers
	_vno_recalc_pwm();
}

// Decrease the frequency by PWM_FREQ_STEP
void freq_down() {
	// Limit reached
	if (g_pwm_state.frequency <= PWM_FREQ_START)
		return;

	// Decrease the frequency value
	g_pwm_state.frequency -= PWM_FREQ_STEP;


	// Recalculate the PWM value and set the registers
	_vno_recalc_pwm();
}


/// DUTY CYCLE CHANGE FUNCTIONS

// Increase the duty cycle by PWM_DUTY_CYCLE_STEP
void duty_cycle_up() {
	// Limit reached
	if (g_pwm_state.duty_cycle_percent + PWM_DUTY_CYCLE_STEP >= PWM_DUTY_CYCLE_LIMIT)
		return;

	// Increase the duty cycle value
	g_pwm_state.duty_cycle_percent += PWM_DUTY_CYCLE_STEP;

	// Recalculate the PWM value and set the registers
	_vno_recalc_pwm();
}

// Decrease the duty cycle by PWM_DUTY_CYCLE_STEP
void duty_cycle_down() {
	// Limit reached
	if (g_pwm_state.duty_cycle_percent <= PWM_DUTY_CYCLE_START)
		return;

	// Decrease the duty cycle value
	g_pwm_state.duty_cycle_percent -= PWM_DUTY_CYCLE_STEP;

	// Recalculate the PWM value and set the registers
	_vno_recalc_pwm();
}


/// OTHER

// Init the PWM
void init_pwm(TIM_HandleTypeDef* timer_handle) {
	// Start the timer
	HAL_TIM_PWM_Start(timer_handle, g_pwm_state.current_output);

	// Set the frequency
	TIMER->ARR = PWM_ARR;

	// Set the duty cycle
	uint32_t CCRn = (uint32_t)(PWM_ARR * PWM_DUTY_CYCLE_START);

	switch (g_pwm_state.current_output) {
	case TIM_CHANNEL_1:
		TIMER->CCR1 = CCRn;
		break;
	case TIM_CHANNEL_2:
		TIMER->CCR2 = CCRn;
		break;
	case TIM_CHANNEL_3:
		TIMER->CCR3 = CCRn;
		break;
	case TIM_CHANNEL_4:
		TIMER->CCR4 = CCRn;
		break;
	}
}

// Chose the next output
void next_output(TIM_HandleTypeDef* timer_handle) {
	// If on the last mode, switch to the first mode and disable the PWM
	if (g_pwm_state.current_output == TIM_CHANNEL_1) {

		// Halt the timer
		HAL_TIM_PWM_Stop(timer_handle, g_pwm_state.current_output);

		// Switch to the first mode
		g_pwm_state.current_output = TIM_CHANNEL_4;
		// Set the is_on flag
		g_pwm_state.is_on = 0;

		return;
	}

	// If the PWM is disabled, enable it
	if (g_pwm_state.is_on == 0) {

		// Start the timer
		HAL_TIM_PWM_Start(timer_handle, g_pwm_state.current_output);

		// Set the is_on flag
		g_pwm_state.is_on = 1;

		return;
	}

	/// If normal
	// Halt the timer
	HAL_TIM_PWM_Stop(timer_handle, g_pwm_state.current_output);

	// Switch to the next mode
	g_pwm_state.current_output -= 0x00000004U;

	// Recalculate the PWM value and set the registers
	_vno_recalc_pwm();

	// Start the timer
	HAL_TIM_PWM_Start(timer_handle, g_pwm_state.current_output);
}
