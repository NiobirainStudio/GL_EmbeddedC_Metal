/*
 *	Basic PWM controller (using i2c protocol) header file
 *	@author Vino
 */

#ifndef _VNO_PWM_I2C
#define _VNO_PWM_I2C

// Includes
#include <stdint.h>
#include <math.h>
#include "stm32f4xx_hal.h"


// Defines
#define VNO_ALL_LED_ON_L 	(uint8_t)0x3D

#define VNO_PWM_DUTY_CYCLE_MIN 	0.1
#define VNO_PWM_DUTY_CYCLE_MAX 	0.9
#define VNO_PWM_DUTY_CYCLE_STEP 0.3

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))



// Controller state structure
typedef struct PWM_I2C_State {
	uint8_t 			is_sleeping;
	uint8_t				is_on;
	uint8_t				devId;
	float				delay;
	uint8_t				TxBuffer[8];
	float				duty_cycles[16];
	I2C_HandleTypeDef* 	i2c;
} PWM_I2C_State;



/// SLEEP FUNCTIONS

// Go to sleep
void pwm_i2c_controller_sleep();

// Wake up
void pwm_i2c_controller_wake_up();

// Toggle sleep mode
void pwm_i2c_controller_toggle_sleep();


/// PWM FREQUENCY AND DUTY CYCLE SET FUNCTIONS

// Set PWM frequency
void pwm_i2c_controller_frequency_set(uint16_t hz);

// Set PWM duty cycle for led_num (0 <= led_num <= 15 || led_num == VNO_ALL_LED_ON_L)
uint8_t pwm_i2c_controller_duty_cycle_set(uint8_t led_num, float duty_cycle);

// Add duty_cycle for a specified LED
uint8_t pwm_i2c_controller_duty_cycle_add(uint8_t led_num);

// Subtract delay for a specified LED
uint8_t pwm_i2c_controller_duty_cycle_sub(uint8_t led_num);



/// OTHER

// Init function
uint8_t pwm_i2c_controller_init(I2C_HandleTypeDef* i2c, float delay);

// Toggle channels
void pwm_i2c_controller_toggle_channels();


#endif
