/*
 *	Basic SPI flash controls source file
 *	@author Vino
 */

// Local header include
#include "UsrSrc/flash_spi.h"

// STD library include
#include <string.h>


// Consts
const uint8_t VNO_SPI_FLASH_OPC_READ 		= 0x03;
const uint8_t VNO_SPI_FLASH_OPC_ERASE		= 0x60;
const uint8_t VNO_SPI_FLASH_OPC_WRITE_BYTE	= 0x02;
const uint8_t VNO_SPI_FLASH_OPC_RDSR		= 0x05;
const uint8_t VNO_SPI_FLASH_OPC_EWSR		= 0x50;
const uint8_t VNO_SPI_FLASH_OPC_WRSR		= 0x01;
const uint8_t VNO_SPI_FLASH_OPC_WREN		= 0x06;
const uint8_t VNO_SPI_FLASH_OPC_WRDI		= 0x04;

static VNO_SPI_Flash_StateStruct g_spi_flash_struct;


/// PRIVATE HELPER FUNCTIONS

// Set the chip select to high
static void VNO_SPI_Flash_ChipSelect_High() {
	HAL_GPIO_WritePin(VNO_SPI_FLASH_CS_PORT, VNO_SPI_FLASH_CS_PIN, GPIO_PIN_SET);
}
// Set the chip select to high
static void VNO_SPI_Flash_ChipSelect_Low() {
	HAL_GPIO_WritePin(VNO_SPI_FLASH_CS_PORT, VNO_SPI_FLASH_CS_PIN, GPIO_PIN_RESET);
}
// SPI Transmit wrapper
static HAL_StatusTypeDef VNO_SPI_Flash_Transmit(uint8_t* data, const uint32_t size) {
	VNO_SPI_Flash_ChipSelect_Low();
	HAL_StatusTypeDef result = HAL_SPI_TransmitReceive(g_spi_flash_struct.hspi, data, data, size, HAL_MAX_DELAY);
	VNO_SPI_Flash_ChipSelect_High();

	return result;
}



/// MEMORY CHANGE FUNCTIONS

// Read flash memory into buffer
HAL_StatusTypeDef VNO_SPI_Flash_Read(
		uint8_t* const buffer,
		const uint32_t size,
		const uint32_t address,
		const uint32_t count) {

	// Invalid data
	if (buffer == NULL || size < count)
		return HAL_ERROR;

	// Make a bigger buffer
	uint8_t buffer_inner[count + 4];

	// Fill it with op code and memory address
	buffer_inner[0] = VNO_SPI_FLASH_OPC_READ;
	buffer_inner[1] = *((uint8_t*)(&address) + 2);
	buffer_inner[2] = *((uint8_t*)(&address) + 1);
	buffer_inner[3] = *((uint8_t*)(&address));

	// Transmit
	HAL_StatusTypeDef result = VNO_SPI_Flash_Transmit(buffer_inner, sizeof(buffer_inner));

	// Copy data to the original buffer
	memcpy(buffer, buffer_inner + 4, count);

	return result;
}
// Erase all flash memory
HAL_StatusTypeDef VNO_SPI_Flash_Erase() {
	// Transmit
	HAL_StatusTypeDef result = VNO_SPI_Flash_Transmit((uint8_t* const)&VNO_SPI_FLASH_OPC_ERASE, sizeof(1));

	// Hardware delay
	HAL_Delay(100);
	return result;
}
// Write one byte to a specific address in flash memory
HAL_StatusTypeDef VNO_SPI_Flash_WriteByte(
		const uint8_t data,
		const uint32_t address) {

	// Make a buffer for op code, address and a data byte
	uint8_t buffer_inner[5] = {
		VNO_SPI_FLASH_OPC_WRITE_BYTE,
		*((uint8_t*)(&address) + 2),
		*((uint8_t*)(&address) + 1),
		*((uint8_t*)(&address)),
		data
	};

	// Transmit
	return VNO_SPI_Flash_Transmit(buffer_inner, sizeof(buffer_inner));
}


/// SPECIAL FUNCTIONS

// Read the state register
HAL_StatusTypeDef VNO_SPI_Flash_SR_Read(uint8_t* const ret_val) {

	// Invalid data
	if (ret_val == NULL)
		return HAL_ERROR;

	// Make transmit/receive buffer
	uint8_t buffer_inner[2] = {
		VNO_SPI_FLASH_OPC_RDSR,
		0
	};

	// Execute t/r
	VNO_SPI_Flash_ChipSelect_Low();
	HAL_StatusTypeDef result = HAL_SPI_TransmitReceive(
		g_spi_flash_struct.hspi,
		buffer_inner,
		buffer_inner,
		sizeof(buffer_inner),
		HAL_MAX_DELAY);
	VNO_SPI_Flash_ChipSelect_High();

	// Put the received value into the original buffer
	*ret_val = buffer_inner[1];

	return result;
}
// Enable writing to the state register
HAL_StatusTypeDef VNO_SPI_Flash_SR_WriteEnable() {
	// Transmit
	return VNO_SPI_Flash_Transmit((uint8_t* const)&VNO_SPI_FLASH_OPC_EWSR, sizeof(1));
}
// Write to the state register
HAL_StatusTypeDef VNO_SPI_Flash_SR_Write(const uint8_t reg_val) {

	// Put the op code and the data bit into a new buffer
	uint8_t buffer_inner[2] = {
		VNO_SPI_FLASH_OPC_WRSR,
		reg_val
	};

	// Transmit
	return VNO_SPI_Flash_Transmit(buffer_inner, 2);
}

// Enable writing to memory
HAL_StatusTypeDef VNO_SPI_Flash_WriteEnable() {
	// Transmit
	return VNO_SPI_Flash_Transmit((uint8_t* const)&VNO_SPI_FLASH_OPC_WREN, sizeof(1));
}
// Disable writing to memory
HAL_StatusTypeDef VNO_SPI_Flash_WriteDisable() {
	// Transmit
	return VNO_SPI_Flash_Transmit((uint8_t* const)&VNO_SPI_FLASH_OPC_WRDI, sizeof(1));
}

// Lock memory
HAL_StatusTypeDef VNO_SPI_Flash_MemLock() {
	// Read the state register
	uint8_t reg_val;
	HAL_StatusTypeDef result = VNO_SPI_Flash_SR_Read(&reg_val);
	if (result != HAL_OK)
		return result;

	// Unlock the memory space
	VNO_SPI_Flash_SoftwareStatus_Struct sss = { 0 };
	sss.BP0 = 1;
	sss.BP1 = 1;
	sss.BP2 = 1;

	reg_val |= *((uint8_t*)(&sss));

	// Update the status register
	result = VNO_SPI_Flash_SR_WriteEnable();
	if (result != HAL_OK)
		return result;
	result = VNO_SPI_Flash_SR_Write(reg_val);
	if (result != HAL_OK)
		return result;
	result = VNO_SPI_Flash_WriteDisable();
	if (result != HAL_OK)
		return result;

	return result;
}
// Unlock memory
HAL_StatusTypeDef VNO_SPI_Flash_MemUnlock() {
	// Read the state register
	uint8_t reg_val;
	HAL_StatusTypeDef result = VNO_SPI_Flash_SR_Read(&reg_val);
	if (result != HAL_OK)
		return result;

	// Lock the memory space
	VNO_SPI_Flash_SoftwareStatus_Struct sss;
	sss.BP0 = 1;
	sss.BP1 = 1;
	sss.BP2 = 1;

	reg_val &= (*((uint8_t*)(&sss)) ^ 0xFF);

	// Update the status register
	result = VNO_SPI_Flash_SR_WriteEnable();
	if (result != HAL_OK)
		return result;
	result = VNO_SPI_Flash_SR_Write(reg_val);
	if (result != HAL_OK)
		return result;
	result = VNO_SPI_Flash_WriteDisable();
	if (result != HAL_OK)
		return result;

	return result;
}


/// OTHER

// Init the state struct
void VNO_SPI_Flash_Init(SPI_HandleTypeDef* hspi) {
	VNO_SPI_Flash_ChipSelect_High();
	HAL_Delay(8000);
	g_spi_flash_struct.hspi = hspi;
}
