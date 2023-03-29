/*
 *	Basic SPI flash controls header file
 *	@author Vino
 */

// Include guard
#ifndef VNO_FLASH_SPI_H_
#define VNO_FLASH_SPI_H_

// CS defines
#define VNO_SPI_FLASH_CS_PORT	GPIOD
#define VNO_SPI_FLASH_CS_PIN	GPIO_PIN_7

// STD (STM32) library include
#include "stm32f4xx_hal.h"



// SSR Struct
typedef struct {
	/*
	 * Check if any writes are in progress
	 */
	uint8_t BUSY	: 1;
	uint8_t WEL 	: 1;

	/*
	 * Sector writing bits
	 */
	uint8_t BP0 	: 1;
	uint8_t BP1 	: 1;
	uint8_t BP2 	: 1;
	uint8_t BP3 	: 1;

	/*
	 * Auto increment address
	 */
	uint8_t AAI 	: 1;

	/*
	 * Must be set to 0 to change BP bits
	 */
	uint8_t BPL 	: 1;
} VNO_SPI_Flash_SoftwareStatus_Struct;


// SPI Handle
typedef struct {
	SPI_HandleTypeDef* hspi;
} VNO_SPI_Flash_StateStruct;





/// MEMORY CHANGE FUNCTIONS

// Read flash memory into buffer
HAL_StatusTypeDef VNO_SPI_Flash_Read(
		uint8_t* const buffer,
		const uint32_t size,
		const uint32_t address,
		const uint32_t count);

// Erase all flash memory
HAL_StatusTypeDef VNO_SPI_Flash_Erase();

// Write one byte to a specific address in flash memory
HAL_StatusTypeDef VNO_SPI_Flash_WriteByte(
		const uint8_t data,
		const uint32_t address);


/// SPECIAL FUNCTIONS

// Read the state register
HAL_StatusTypeDef VNO_SPI_Flash_SR_Read(uint8_t* const ret_val);
// Enable writing to the state register
HAL_StatusTypeDef VNO_SPI_Flash_SR_WriteEnable();
// Write to the state register
HAL_StatusTypeDef VNO_SPI_Flash_SR_Write(const uint8_t reg_val);

// Enable writing to memory
HAL_StatusTypeDef VNO_SPI_Flash_WriteEnable();
// Disable writing to memory
HAL_StatusTypeDef VNO_SPI_Flash_WriteDisable();

// Lock memory
HAL_StatusTypeDef VNO_SPI_Flash_MemLock();
// Unlock memory
HAL_StatusTypeDef VNO_SPI_Flash_MemUnlock();


/// OTHER

// Init the state struct
void VNO_SPI_Flash_Init(SPI_HandleTypeDef* hspi);

#endif /* INC_SST25VF016B_H_ */
