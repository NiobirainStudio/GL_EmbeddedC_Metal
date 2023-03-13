/*
 *	Basic PWM controller (using i2c protocol) source file
 *	@author Vino
 */

// Includes
#include "UsrSrc/pwm_i2c_controller.h"

static PWM_I2C_State g_pwm_i2c_state = {
		1,			// is_sleeping
		1,			// is_on
		0x40 << 1	// devId
};



/// SLEEP MODES

// Go to sleep
void pwm_i2c_controller_sleep() {
	g_pwm_i2c_state.is_sleeping = 1;
	g_pwm_i2c_state.TxBuffer[0] = 0x00; // MODE1
	g_pwm_i2c_state.TxBuffer[1] = 0x11;	// Go to sleep
	HAL_I2C_Master_Transmit(g_pwm_i2c_state.i2c, g_pwm_i2c_state.devId, (uint8_t*) &g_pwm_i2c_state.TxBuffer, 2, 1000);
}

// Wake up
void pwm_i2c_controller_wake_up() {
	g_pwm_i2c_state.is_sleeping = 0;
	g_pwm_i2c_state.TxBuffer[0] = 0x00; // MODE1
	g_pwm_i2c_state.TxBuffer[1] = 0x01;	// Wake up
	HAL_I2C_Master_Transmit(g_pwm_i2c_state.i2c, g_pwm_i2c_state.devId, (uint8_t*) &g_pwm_i2c_state.TxBuffer, 2, 1000);
}

// Toggle sleep mode
void pwm_i2c_controller_toggle_sleep() {

	if (g_pwm_i2c_state.is_sleeping == 1) {
		g_pwm_i2c_state.TxBuffer[0] = 0x00; // MODE1
		g_pwm_i2c_state.TxBuffer[1] = 0x01;	// Wake up
		g_pwm_i2c_state.is_sleeping = 0;
	} else {
		g_pwm_i2c_state.TxBuffer[0] = 0x00; // MODE1
		g_pwm_i2c_state.TxBuffer[1] = 0x11;	// Go to sleep
		g_pwm_i2c_state.is_sleeping = 1;
	}

	HAL_I2C_Master_Transmit(g_pwm_i2c_state.i2c, g_pwm_i2c_state.devId, (uint8_t*) &g_pwm_i2c_state.TxBuffer, 2, 1000);
}



/// PWM FREQUENCY AND DUTY CYCLE SET FUNCTIONS

// Set PWM frequency
void pwm_i2c_controller_frequency_set(uint16_t hz) {

	if (g_pwm_i2c_state.is_sleeping == 0)
		pwm_i2c_controller_sleep();

	// Calculate the prescale value
	uint16_t presc = (uint16_t)round(25000000 / (4096 * hz)) - 1;
	if (presc > 0xFF)
		presc = 0xFF;
	else if (presc < 0x03)
		presc = 0x03;

	g_pwm_i2c_state.TxBuffer[0] = 0xFE; // PRE_SCALE
	g_pwm_i2c_state.TxBuffer[1] = (uint8_t)presc;
	HAL_I2C_Master_Transmit(g_pwm_i2c_state.i2c, g_pwm_i2c_state.devId, (uint8_t*) &g_pwm_i2c_state.TxBuffer, 2, 1000); // Set new freq

	if (g_pwm_i2c_state.is_sleeping == 0)
		pwm_i2c_controller_wake_up();
}


// Private set PWM duty cycle function
static void _pwm_i2c_controller_duty_cycle_set(uint8_t led_num, float delay, float duty_cycle) {
	/* x = led_num
	 *							Offset
	 * LEDx_ON_L	6+x*4 	+ 	0
	 * LEDx_ON_H				1
	 * LEDx_OFF_L				2
	 * LEDx_OFF_H				3
	*/
	uint8_t LEDx_ON_L = 6 + led_num * 4;


	// Writing to PWM registers

	// 0001 1001 1001
	// H	L

	uint16_t delay_reg_val = 4096 * delay - 1;

	g_pwm_i2c_state.TxBuffer[0] = LEDx_ON_L; 							// LED_ON_L
	g_pwm_i2c_state.TxBuffer[1] = (uint8_t)delay_reg_val; 				// L
	HAL_I2C_Master_Transmit(g_pwm_i2c_state.i2c, g_pwm_i2c_state.devId, (uint8_t*) &g_pwm_i2c_state.TxBuffer, 2, 1000);

	g_pwm_i2c_state.TxBuffer[0] = LEDx_ON_L + 1; 						// LED_ON_H
	g_pwm_i2c_state.TxBuffer[1] = (uint8_t)(delay_reg_val >> 8); 		// H
	HAL_I2C_Master_Transmit(g_pwm_i2c_state.i2c, g_pwm_i2c_state.devId, (uint8_t*) &g_pwm_i2c_state.TxBuffer, 2, 1000);

	uint16_t duty_cycle_reg_val = 4096 * duty_cycle - 1;
	duty_cycle_reg_val += delay_reg_val;


	g_pwm_i2c_state.TxBuffer[0] = LEDx_ON_L + 2; 						// LED_OFF_L
	g_pwm_i2c_state.TxBuffer[1] = (uint8_t)duty_cycle_reg_val;			// L
	HAL_I2C_Master_Transmit(g_pwm_i2c_state.i2c, g_pwm_i2c_state.devId, (uint8_t*) &g_pwm_i2c_state.TxBuffer, 2, 1000);

	g_pwm_i2c_state.TxBuffer[0] = LEDx_ON_L + 3; 						// LED_OFF_H
	g_pwm_i2c_state.TxBuffer[1] = (uint8_t)(duty_cycle_reg_val >> 8);	// H
	HAL_I2C_Master_Transmit(g_pwm_i2c_state.i2c, g_pwm_i2c_state.devId, (uint8_t*) &g_pwm_i2c_state.TxBuffer, 2, 1000);
}

// Set PWM duty cycle for led_num (0 <= led_num <= 15 || led_num == VNO_ALL_LED_ON_L)
uint8_t pwm_i2c_controller_duty_cycle_set(uint8_t led_num, float duty_cycle) {

	if (led_num < 0 || (led_num >= 16 && led_num != VNO_ALL_LED_ON_L))
		return 1; // NO SUCH LED

	if (duty_cycle < 0 || g_pwm_i2c_state.delay + duty_cycle > 100.0)
		return 2; // INCORRECT DELAY OR DUTY_CYCLE OR BOTH

	if (led_num == VNO_ALL_LED_ON_L) {
		for (uint8_t i = 0; i < 16; i++) {
			g_pwm_i2c_state.duty_cycles[i] = duty_cycle;
		}
	} else {
		g_pwm_i2c_state.duty_cycles[led_num] = duty_cycle;
	}
	_pwm_i2c_controller_duty_cycle_set(led_num, g_pwm_i2c_state.delay, duty_cycle);

	return 0; // SUCCESS
}

// Add duty_cycle for a specified LED
uint8_t pwm_i2c_controller_duty_cycle_add(uint8_t led_num) {

	if (led_num < 0 || (led_num >= 16 && led_num != VNO_ALL_LED_ON_L))
		return 1; // NO SUCH LED

	// Modify all LEDs duty cycles
	if (led_num == VNO_ALL_LED_ON_L) {
		for (uint8_t i = 0; i < 16; i++) {
			g_pwm_i2c_state.duty_cycles[i] = MIN(
				g_pwm_i2c_state.duty_cycles[i] + VNO_PWM_DUTY_CYCLE_STEP,
				VNO_PWM_DUTY_CYCLE_MAX
			);
			_pwm_i2c_controller_duty_cycle_set(i, g_pwm_i2c_state.delay, g_pwm_i2c_state.duty_cycles[i]);
		}
	}
	// Modify the specific LED duty cycle
	else {
		g_pwm_i2c_state.duty_cycles[led_num] = MIN(
			g_pwm_i2c_state.duty_cycles[led_num] + VNO_PWM_DUTY_CYCLE_STEP,
			VNO_PWM_DUTY_CYCLE_MAX
		);
		_pwm_i2c_controller_duty_cycle_set(led_num, g_pwm_i2c_state.delay, g_pwm_i2c_state.duty_cycles[led_num]);
	}

	return 0; // SUCCESS
}

// Subtract delay for a specified LED
uint8_t pwm_i2c_controller_duty_cycle_sub(uint8_t led_num) {

	if (led_num < 0 || (led_num >= 16 && led_num != VNO_ALL_LED_ON_L))
		return 1; // NO SUCH LED

	// Modify all LEDs duty cycles
	if (led_num == VNO_ALL_LED_ON_L) {
		for (uint8_t i = 0; i < 16; i++) {
			g_pwm_i2c_state.duty_cycles[i] = MAX(
				g_pwm_i2c_state.duty_cycles[i] + VNO_PWM_DUTY_CYCLE_STEP,
				VNO_PWM_DUTY_CYCLE_MIN
			);
			_pwm_i2c_controller_duty_cycle_set(i, g_pwm_i2c_state.delay, g_pwm_i2c_state.duty_cycles[i]);
		}
	}
	// Modify the specific LED duty cycle
	else {
		g_pwm_i2c_state.duty_cycles[led_num] = MAX(
			g_pwm_i2c_state.duty_cycles[led_num] + VNO_PWM_DUTY_CYCLE_STEP,
			VNO_PWM_DUTY_CYCLE_MIN
		);
		_pwm_i2c_controller_duty_cycle_set(led_num, g_pwm_i2c_state.delay, g_pwm_i2c_state.duty_cycles[led_num]);
	}

	return 0; // SUCCESS
}



/// OTHER

// Init function
uint8_t pwm_i2c_controller_init(I2C_HandleTypeDef* i2c, float delay) {

	// Base duty cylce value fill
	for (uint8_t i = 0; i < 16; i++)
		g_pwm_i2c_state.duty_cycles[i] 	= 0.5;

	// Periphery init delay
	HAL_Delay(10);

	// PWM ENABLE
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

	g_pwm_i2c_state.i2c = i2c;

	if (delay > 100.0)
		return 1;
	g_pwm_i2c_state.delay = delay;
	return 0;
}

// Toggle channels
void pwm_i2c_controller_toggle_channels() {
	if (g_pwm_i2c_state.is_on == 1) {
		g_pwm_i2c_state.is_on = 0;
	} else {
		g_pwm_i2c_state.is_on = 1;
	}
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
}
