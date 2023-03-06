/*
 *	Basic button defines header file
 *	@author Vino
 */

// Includes
#include "UsrSrc/adc_driver.h"

// Globals
volatile uint16_t	adcResultsDMA[CHANNEL_COUNT];
volatile int		adcConversionComplete = 0;

// Locals
static ADC_State g_adc_state = { 0b00000000, { 0, 0, 0 } };


/// General functions

//
void update_intensity() {

	float dc_val = 0.5 - (((float)adcResultsDMA[0] - g_adc_state.base_channels[0]) / ANALOG_STEP) * DUTY_CYCLE_STEP;

	if (dc_val < DUTY_CYCLE_MIN)
		dc_val = DUTY_CYCLE_MIN;
	if (dc_val > DUTY_CYCLE_MAX)
		dc_val = DUTY_CYCLE_MAX;

	TIMER->CCR2 = (uint32_t)(TIMER->ARR * dc_val);


	dc_val = 0.5 - (((float)adcResultsDMA[1] - g_adc_state.base_channels[1]) / ANALOG_STEP) * DUTY_CYCLE_STEP;

	if (dc_val < DUTY_CYCLE_MIN)
		dc_val = DUTY_CYCLE_MIN;
	if (dc_val > DUTY_CYCLE_MAX)
		dc_val = DUTY_CYCLE_MAX;

	TIMER->CCR1 = (uint32_t)(TIMER->ARR * dc_val);

	dc_val = 0.5 - (((float)adcResultsDMA[2] - g_adc_state.base_channels[2]) / ANALOG_STEP) * DUTY_CYCLE_STEP;

	if (dc_val < DUTY_CYCLE_MIN)
		dc_val = DUTY_CYCLE_MIN;
	if (dc_val > DUTY_CYCLE_MAX)
		dc_val = DUTY_CYCLE_MAX;

	TIMER->CCR4 = (uint32_t)(TIMER->ARR * dc_val);
}

// Emergency frequency change
void set_emergency_frequency() {

	// Check if the emergency frequency needs any updates

	// Get emergency count
	uint8_t em_count = 0;
	if ((g_adc_state.high_cases & 0b00000001) == 0b00000001)
		em_count++;
	if ((g_adc_state.high_cases & 0b00000010) == 0b00000010)
		em_count++;
	if ((g_adc_state.high_cases & 0b00000100) == 0b00000100)
		em_count++;

	// Set the new emergency frequency
	switch (em_count) {
	case 1:	// 1 Hz pulse
		HAL_Delay(500);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(500);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		break;

	case 2:	// 2.5 Hz pulse
		HAL_Delay(200);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(200);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		break;

	case 3:	// 5 Hz pulse
		HAL_Delay(100);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(100);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		break;
	}
}


/// OTHER

// Init the ADC
void init_adc(TIM_HandleTypeDef* timer_handle) {
	g_adc_state.base_channels[0] = adcResultsDMA[0];
	g_adc_state.base_channels[1] = adcResultsDMA[1];
	g_adc_state.base_channels[2] = adcResultsDMA[2];

	float dc_val = 0.5 - (((float)adcResultsDMA[0] - g_adc_state.base_channels[0]) / ANALOG_STEP) * DUTY_CYCLE_STEP;

	if (dc_val < DUTY_CYCLE_MIN)
		dc_val = DUTY_CYCLE_MIN;
	if (dc_val > DUTY_CYCLE_MAX)
		dc_val = DUTY_CYCLE_MAX;

	HAL_TIM_PWM_Start(timer_handle, TIM_CHANNEL_1);
	TIMER->CCR2 = (uint32_t)(TIMER->ARR * dc_val);


	dc_val = 0.5 - (((float)adcResultsDMA[1] - g_adc_state.base_channels[1]) / ANALOG_STEP) * DUTY_CYCLE_STEP;

	if (dc_val < DUTY_CYCLE_MIN)
		dc_val = DUTY_CYCLE_MIN;
	if (dc_val > DUTY_CYCLE_MAX)
		dc_val = DUTY_CYCLE_MAX;

	HAL_TIM_PWM_Start(timer_handle, TIM_CHANNEL_2);
	TIMER->CCR1 = (uint32_t)(TIMER->ARR * dc_val);


	dc_val = 0.5 - (((float)adcResultsDMA[2] - g_adc_state.base_channels[2]) / ANALOG_STEP) * DUTY_CYCLE_STEP;

	if (dc_val < DUTY_CYCLE_MIN)
		dc_val = DUTY_CYCLE_MIN;
	if (dc_val > DUTY_CYCLE_MAX)
		dc_val = DUTY_CYCLE_MAX;

	HAL_TIM_PWM_Start(timer_handle, TIM_CHANNEL_4);
	TIMER->CCR4 = (uint32_t)(TIMER->ARR * dc_val);
}

// Get the next iteration for ADC
void adc_iter() {

	// Internal temperature check
	if(adcResultsDMA[0] < ANALOG_TEMP_INT) {
		g_adc_state.high_cases |= 0b00000010;
	} else if (adcResultsDMA[0] > ANALOG_TEMP_INT + ANALOG_PADDING) {
		g_adc_state.high_cases &= 0b11111101;
	}

	// External temperature check
	if(adcResultsDMA[1] < ANALOG_TEMP_EXT) {
		g_adc_state.high_cases |= 0b00000100;
	} else if (adcResultsDMA[1] > ANALOG_TEMP_EXT + ANALOG_PADDING) {
		g_adc_state.high_cases &= 0b11111011;
	}

	// Potentiometer check
	if(adcResultsDMA[2] < ANALOG_POTENTIO) {
		g_adc_state.high_cases |= 0b00000001;
	} else if (adcResultsDMA[2] > ANALOG_POTENTIO + ANALOG_PADDING) {
		g_adc_state.high_cases &= 0b11111110;
	}

	// Update the emergency frequency
	set_emergency_frequency();
}
