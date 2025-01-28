/**
 * @file Main.cpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Example file for ILPS22QS driver.
 * 
 * @copyright Copyright (c) 2025, silvio3105
 * 
 */

// ----- INCLUDE FILES
#include "main.h"
#include "ILPS22QS.hpp"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "SEGGER_RTT.h"


// ----- DEFINES
#define SS_Pin						GPIO_PIN_5
#define SS_GPIO_Port 				GPIOB
//#define ONESHOT // Comment this define to use continuous mode


// ----- FUNCTION DECLARATIONS
void SystemClock_Config(void);
void Error_Handler(void);
static void gpioInit(void);
static void i2cInit(void);


// ----- SENSOR STUFF
I2C_HandleTypeDef hi2c1;
ILPS22QS::Return_t I2Cread(const uint8_t address, void* data, const uint8_t len, const uint8_t timeout)
{
	if (HAL_I2C_Master_Receive(&hi2c1, address << 1, (uint8_t*)data, len, timeout) != HAL_OK)
	{
		return ILPS22QS::Return_t::NOK;
	}

	return ILPS22QS::Return_t::OK;
}

ILPS22QS::Return_t I2Cwrite(const uint8_t address, void* data, const uint8_t len, const uint8_t timeout)
{
	if (HAL_I2C_Master_Transmit(&hi2c1, address << 1, (uint8_t*)data, len, timeout) != HAL_OK)
	{
		return ILPS22QS::Return_t::NOK;
	}

	return ILPS22QS::Return_t::OK;	
}

ILPS22QS::Return_t I2CMSPInit(void)
{
	// Sensor MSP init will only init slave select control. I2C is a bus and makes no sense to init (shared) bus per sensor.

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = SS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SS_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_SET); // Set SS high to use I2C
	return ILPS22QS::Return_t::OK;
}

ILPS22QS::Return_t I2CMSPDeinit(void)
{
	HAL_GPIO_DeInit(SS_GPIO_Port, SS_Pin);
	return ILPS22QS::Return_t::OK;
}

ILPS22QS::I2C Sensor = ILPS22QS::I2C(I2Cread, I2Cwrite, I2CMSPInit, I2CMSPDeinit);


// ----- FUNCTION DEFINITIONS
int main(void)
{
	#ifdef ONESHOT
	uint8_t skipDelay = 1;
	#endif // ONESHOT

	uint8_t strLen = 0;
	char str[32];

	int16_t temperature;
	uint16_t pressure;
	ILPS22QS::data_status_t status;

	// Init STM32 stuff
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_Init();
	SystemClock_Config();
	gpioInit();
	i2cInit();

	// Write something with RTT
	strLen = snprintf(str, sizeof(str), "\n\nILPS22QS %s example\n", ILPS22QS::version);
	SEGGER_RTT_Write(0, str, strLen);

	// Try to init the sensor
	if (Sensor.init() != ILPS22QS::Return_t::OK)
	{
		SEGGER_RTT_Write(0, "Sensor fail\n", 12);
	}
	else
	{
		SEGGER_RTT_Write(0, "Sensor OK\n", 10);
	}

	// Disable analog hub/qvar to reduce power consumption
	if (Sensor.disableAnalogHub() != ILPS22QS::Return_t::OK)
	{
		SEGGER_RTT_Write(0, "Dis AH fail\n", 12);		
	}

	// Enable full pressure scale
	if (Sensor.setPressureScale(ILPS22QS::PressureScale_t::Scale4060hPa) != ILPS22QS::Return_t::OK)
	{
		SEGGER_RTT_Write(0, "Scale fail\n", 11);	
	}

	// Config output data rate
	ILPS22QS::data_output_cfg_t odrCfg = 
	{
		#ifdef ONESHOT
		.dataRate = ILPS22QS::OutputDataRate_t::OneShot,
		#else 
		.dataRate = ILPS22QS::OutputDataRate_t::ODR1Hz,
		#endif // ONESHOT
		.average = ILPS22QS::Average_t::Average64
	};
	if (Sensor.setDataOutputConfig(odrCfg) != ILPS22QS::Return_t::OK)
	{
		SEGGER_RTT_Write(0, "ODR fail\n", 9);
	}

	// Config filter
	ILPS22QS::filter_config_t filterCfg = 
	{
		.discard = ILPS22QS::FilterDiscard_t::Discard6Samples,
		.filter = ILPS22QS::State_t::Enable
	};
	if (Sensor.setFilterConfig(filterCfg) != ILPS22QS::Return_t::OK)
	{	
		SEGGER_RTT_Write(0, "Filter fail\n", 12);
	}

	// Roll the example
	while (1)
	{
		#ifdef ONESHOT
		// Stop example for 5 seconds
		if (!skipDelay)
		{
			HAL_Delay(5000);
		}

		// Start measure process
		if (Sensor.measure() != ILPS22QS::Return_t::OK)
		{
			SEGGER_RTT_Write(0, "Measure fail\n", 13);
			continue;
		}
		else
		{
			skipDelay = 1;
		}
		#endif // ONESHOT

		// Get data status
		if (Sensor.getDataStatus(status) == ILPS22QS::Return_t::OK)
		{
			// Pressure data is available
			if (status.pressureAvailable &&
				status.temperatureAvailable)
			{
				// Read pressure and output it to RTT
				if (Sensor.getPressure(pressure) == ILPS22QS::Return_t::OK)
				{
					strLen = snprintf(str, sizeof(str), "- Pressure: %u.%03ubar\n", pressure / 1000, (uint8_t)(pressure % 1000));
					SEGGER_RTT_Write(0, str, strLen);

					#ifdef ONESHOT
					skipDelay = 0;
					#endif // ONESHOT
				}

				// Read temperature data and output it to RTT
				if (Sensor.getTemperature(temperature) == ILPS22QS::Return_t::OK)
				{
					strLen = snprintf(str, sizeof(str), "- Temperature: %d.%02uC\n", temperature / 100, temperature % 100);
					SEGGER_RTT_Write(0, str, strLen);
				}				
			}
		}
	}
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

static void i2cInit(void)
{
	__HAL_RCC_I2C1_CLK_ENABLE();

	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}
}

static void gpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(void)
{
	__disable_irq();
	while (1)
	{

	}
}