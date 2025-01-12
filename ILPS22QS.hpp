/**
 * @file ILPS22QS.hpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief ILPS22QS I2C/3-wire SPI driver header file.
 * 
 * @copyright Copyright (c) 2025, silvio3105
 * 
 */

/*
	Copyright (c) 2025, silvio3105 (www.github.com/silvio3105)

	Access and use of this Project and its contents are granted free of charge to any Person.
	The Person is allowed to copy, modify and use The Project and its contents only for non-commercial use.
	Commercial use of this Project and its contents is prohibited.
	Modifying this License and/or sublicensing is prohibited.

	THE PROJECT AND ITS CONTENT ARE PROVIDED "AS IS" WITH ALL FAULTS AND WITHOUT EXPRESSED OR IMPLIED WARRANTY.
	THE AUTHOR KEEPS ALL RIGHTS TO CHANGE OR REMOVE THE CONTENTS OF THIS PROJECT WITHOUT PREVIOUS NOTICE.
	THE AUTHOR IS NOT RESPONSIBLE FOR DAMAGE OF ANY KIND OR LIABILITY CAUSED BY USING THE CONTENTS OF THIS PROJECT.

	This License shall be included in all functional textual files.
*/

#ifndef _ILPS22QS_HPP_
#define _ILPS22QS_HPP_

// ----- INCLUDE FILES
#include			<stdint.h>
#include			<string.h>


// ----- DEFINES
#ifndef ILPS22QS_I2C_READ_TIMEOUT
#define ILPS22QS_I2C_READ_TIMEOUT				10 /**< @brief Timeout in ms for I2C read operations. Can be changed during compile. */
#endif // ILPS22QS_I2C_READ_TIMEOUT

#ifndef ILPS22QS_I2C_WRITE_TIMEOUT
#define ILPS22QS_I2C_WRITE_TIMEOUT				10 /**< @brief Timeout in ms for I2C write operations. Can be changed during compile. */
#endif // ILPS22QS_I2C_WRITE_TIMEOUT

#ifndef ILPS22QS_SPI_READ_TIMEOUT
#define ILPS22QS_SPI_READ_TIMEOUT				10 /**< @brief Timeout in ms for SPI read operations. Can be changed during compile. */
#endif // ILPS22QS_SPI_READ_TIMEOUT

#ifndef ILPS22QS_SPI_WRITE_TIMEOUT
#define ILPS22QS_SPI_WRITE_TIMEOUT				10 /**< @brief Timeout in ms for SPI write operations. Can be changed during compile. */
#endif // ILPS22QS_SPI_WRITE_TIMEOUT


// ----- NAMESPACES
/**
 * @brief Namespace for ILPS22QS driver.
 * 
 */
namespace ILPS22QS
{

// ----- ENUMS
/**
 * @brief Return values for ILPS22QS driver.
 * 
 */
enum class Return_t : uint8_t
{
	OK = 0, /**< @brief OK return value. */
	NOK, /**< @brief Generic not-OK return value. */
	Timeout, /**< @brief Return value for timeouted operation. */
};

/**
 * @brief Driver process semaphore values.
 * 
 */
enum class Semaphore_t : uint8_t 
{
	Free = 0, /**< @brief Semaphore value when process is free. */
	Taken = 1 /**< @brief Semaphore value when process is taken. */
};

/**
 * @brief Enable/Disable state enum class.
 * 
 */
enum class State_t : uint8_t
{
	Disable = 0,
	Enable = 1
};

/**
 * @brief Interrupt latch values.
 * 
 */
enum class InterruptLatch_t : uint8_t
{
	NotLatch = 0, /**< @brief Interrupt is not latched. */
	Latch = 1 /**< @brief Interrupt is latched. */
};

/**
 * @brief Temperature unit values.
 * 
 */
enum class TemperatureScale_t : uint8_t
{
	Celsius = 0, /**< @brief Value for temperature in centideegres celsius. */
	Fahrenheit = 1, /**< @brief Value for temperature in centideegres fahrenheit. */
};

/**
 * @brief Pressure scale values.
 * 
 */
enum class PressureScale_t : uint8_t
{
	Scale1260hPa = 0, /**< @brief Value for pressure scale up to 1260hPa. */
	Scale4060hPa = 1 /**< @brief Value for pressure scale up to 4060hPa. */
};


// ----- STRUCTS
/**
 * @brief Interrupt config struct.
 * 
 */
struct interrupt_cfg_t
{
	State_t autoREFP; /**< @brief Enable or disable AutoREFP feature. */
	State_t resetARP; /**< @brief Reset autoREFP feature. */
	State_t autoZero; /**< @brief Enable or disable auto zero feature. */
	State_t resetAZ; /**< @brief Reset auto zero feature. */
	InterruptLatch_t interruptLatch; /**< @brief Configure  */
	State_t pressureLowInterrupt; /**< @brief Generate interrupt on low pressure. */
	State_t pressureHighInterrupt; /**< @brief Generate interrupt on high pressure. */
};

/**
 * @brief Interface config struct.
 * 
 */
struct interface_cfg_t
{
	State_t i2ci3cOff; /**< @brief Turn off I2C/I3C interface. */
	State_t SPIRead; /**< @brief Turn on 3-wire SPI read. */
	State_t sdaPullUp; /**< @brief Enable or disable pull-up on SDA line. */
	State_t ssPullUpOff; /**< @brief Turn off pull-up on SS line. */
};


// ----- TYPEDEFS
/**
 * @brief Typedef for external delay/wait handler.
 * 
 * @param period Delay/wait period in ms.
 * 
 * @return No return value.
 */
typedef void (*Delay_f)(const uint32_t period);

/**
 * @brief Typedef for external handler for getting tick.
 * 
 * @return Tick count.
 */
typedef uint32_t (*Tick_f)(void);

/**
 * @brief Typedef for external handler for I2C read/write operations.
 * 
 * @param address Slave 7-bit I2C address.
 * @param data Pointer to IO data.
 * @param len Length of \c data in bytes.
 * @param tiemout Read/write timeout in ms.
 * 
 * @return \c Return_t::NOK on fail.
 * @return \c Return_t::OK on success.
 */
typedef Return_t (*I2CRW_f)(const uint8_t address, void* data, const uint8_t len, const uint8_t timeout);

/**
 * @brief Typedef for external handler for SPI read/write operations.
 * 
 * @param txData Pointer to output data.
 * @param txLen Length of \c txData in bytes.
 * @param rxData Pointer to input buffer for incoming data.
 * @param rxLen Length of \c rxData buffer(number of bytes to receive).
 * @param timeout Operation timeout in ms.
 * 
 * @return \c Return_t::NOK on fail.
 * @return \c Return_t::OK on success.
 */
typedef Return_t (*SPIRXTX_f)(const void* txData, const uint8_t txLen, void* rxData, const uint8_t rxLen, const uint8_t timeout);

/**
 * @brief Typedef for external handler for SPI slave select operations.
 * 
 * @return \c Return_t::NOK on fail.
 * @return \c Return_t::OK on success.
 */
typedef Return_t (*Select_f)(void);

/**
 * @brief Typedef for external handler for MSP init and deinit.
 * 
 * @return \c Return_t::NOK on fail.
 * @return \c Return_t::OK on success.
 */
typedef Return_t (*MSP_f)(void);


// ----- CLASSES
/**
 * @brief Class for ILPS22QS driver.
 * 
 * @tparam E Object for external operations.
 */
template<class E>
class Driver
{
	public:
	// ----- METHOD DEFINITIONS
	/**
	 * @brief Object deconstructor
	 * 
	 * @return No return value.
	 */
	~Driver(void)
	{
		memset(this, 0, sizeof(E));
	}	

	/**
	 * @brief Take semaphore for read and write operations.
	 * 
	 * This is required if async read and write is used(eg., interrupt driven or DMA).
	 * 
	 * @return No return value.
	 */
	inline void takeSemaphore(void)
	{
		semaphore = Semaphore_t::Taken;
	}

	/**
	 * @brief Free semaphore for read and write operations.
	 * 
	 * This is required if async read and write is used(eg., interrupt driven or DMA).
	 * 
	 * @return No return value.
	 */
	inline void freeSemaphore(void)
	{
		semaphore = Semaphore_t::Free;
	}	

	/**
	 * @brief Init the sensor.
	 * 
	 * @param interfaceCfg Pointer to interface config. Optional.
	 * 
	 * @return \c Return_t::NOK on failed init.
	 * @return \c Return_t::OK on successful init. 
	 */
	Return_t init(const interface_cfg_t* interfaceCfg = nullptr) const
	{
		// Call MSP init handler if provided
		if (mspInitHandler)
		{
			if (mspInitHandler() != Return_t::OK)
			{
				return Return_t::NOK;
			}
		}

		// Set interface config
		if (interfaceCfg)
		{
			if (interfaceConfig(interfaceCfg) != Return_t::OK)
			{
				return Return_t::NOK;
			}
		}

		// Check sensor ID
		uint8_t tmp = 0;
		if (whoAmI(tmp) == Return_t::OK)
		{
			if (tmp == chipID)
			{
				return Return_t::OK;
			}		
		}

		return Return_t::NOK;
	}

	/**
	 * @brief Deinit the sensor.
	 * 
	 * @return \c Return_t::NOK on failed init.
	 * @return \c Return_t::OK on successful init. 
	 */
	Return_t deinit(void) const
	{
		// Call deinit handler if provided
		if (mspDeinitHandler)
		{
			mspDeinitHandler();
		}
	}

	/**
	 * @brief Configure sensor interrupts.
	 * 
	 * @param config Reference to interrupt config. See \ref interrupt_cfg_t
	 * 
	 * @return Everything other than \c Return_t::OK means interrupts are not configured. 
	 * @return \c Return_t::OK on success.
	 */
	Return_t interruptConfig(const interrupt_cfg_t& config) const
	{	
		uint8_t tmp[2];	
		tmp[0] = Register_t::Interrupt;
		tmp[1] = 	(config.autoREFP << InterruptBitmap_t::AutoREFP) |
					(config.autoZero << InterruptBitmap_t::AutoZero) |
					(config.interruptLatch << InterruptBitmap_t::InterruptLatch) |
					(config.pressureHighInterrupt << InterruptBitmap_t::PressureHighEvent) |
					(config.pressureLowInterrupt << InterruptBitmap_t::PressureLowEvent) |
					(config.resetARP << InterruptBitmap_t::ResetAutoREFP) |
					(config.resetAZ << InterruptBitmap_t::ResetAutoZero);

		return writeRegister(tmp, sizeof(tmp));
	}

	/**
	 * @brief Get pressure threshold for pressure interrupt.
	 * 
	 * @param output Reference for pressure threshold value output.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	Return_t getPressureInterruptThreshold(uint16_t& output) const
	{
		uint16_t tmpOutput = 0;
		uint8_t tmp = 0;

		// Read high byte
		if (readRegister(Register_t::PressureThresholdHigh, tmp) != Return_t::OK)
		{
			return Return_t::NOK;
		}
		tmpOutput = tmp << 8;		

		// Read low byte
		if (readRegister(Register_t::PressureThresholdLow, tmp) != Return_t::OK)
		{
			return Return_t::NOK;
		}
		tmpOutput |= tmp;

		// Get pressure scale
		static constexpr uint8_t scaleDivider[2] = { 16, 8 };
		PressureScale_t scale = PressureScale_t::Scale1260hPa;

		if (getPressureScale(scale) != Return_t::OK)
		{
			return Return_t::NOK;
		}

		// Convert to output unit
		output = tmpOutput / scaleDivider[scale];
		return Return_t::OK;
	}

	/**
	 * @brief Get measuing scale for pressure.
	 * 
	 * @param output Reference to pressure scale output. See \ref PressureScale_t
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	Return_t getPressureScale(PressureScale_t& output)
	{
		// Read control 2 register
		uint8_t tmp = 0;

		if (readRegister(Register_t::Contorl2, tmp) != Return_t::OK)
		{
			return Return_t::NOK;
		}

		// Get pressure scale
		if (tmp & (1 << Control2Bitmap_t::FullScale))
		{
			output = PressureScale_t::Scale4060hPa;
		}
		else
		{
			output = PressureScale_t::Scale1260hPa;
		}

		return Return_t::OK;
	}

	/**
	 * @brief Set pressure scale.
	 * 
	 * @param scale Pressure scale. See \ref PressureScale_t
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	Return_t setPressureScale(const PressureScale_t scale) const
	{
		// Read control 2 register
		uint8_t tmp[2];

		if (readRegister(Register_t::Contorl2, tmp[1]) != Return_t::OK)
		{
			return Return_t::NOK;
		}

		tmp[0] = Register_t::Contorl2;
		tmp[1] &= ~(1 << Control2Bitmap_t::FullScale);
		tmp[1] |= (scale << Control2Bitmap_t::FullScale);

		return writeRegister(tmp, sizeof(tmp));		
	}

	/**
	 * @brief Get temperature scale.
	 * 
	 * @return See \ref TemperatureScale_t 
	 */
	inline TemperatureScale_t getTemperatureScale(void) const
	{
		return temperatureScale;
	}

	/**
	 * @brief Set temperature scale.
	 * 
	 * @param scale Output temperature scale.
	 * 
	 * @return No return values.
	 */
	inline void setTemperatureScale(const TemperatureScale_t scale)
	{
		temperatureScale = scale;
	}


	private:
	// ----- ENUMS
	/**
	 * @brief Enum class with register addresses.
	 * 
	 */
	enum class Register_t : uint8_t
	{
		Interrupt = 0x0B, /**< @brief Interrupt config for pressure acquisition configuration (R/W). */
		PressureThresholdLow = 0x0C, /**< @brief User-defined threshold value for pressure interrupt event (LSB)(R/W). */
		PressureThresholdHigh = 0x0D, /**< @brief User-defined threshold value for pressure interrupt event (MSB)(R/W). */
		Interface = 0x0E, /**< @brief Interface control (R/W). */
		WhoAmI = 0x0F, /**< @brief Device ID (R). */
		Control1 = 0x10, /**< @brief Control 1 (R/W). */
		Contorl2 = 0x11, /**< @brief Control 2 (R/W). */
		Control3 = 0x12, /**< @brief Control 3 (R/W). */
		ControlFIFO = 0x14, /**< @brief FIFO control (R/W). */
		FIFOWatermark = 0x15, /**< @brief FIFO watermark (R/W). */
		PressureReferenceLow = 0x16, /**< @brief Pressure reference (LSB)(R). */
		PressureReferenceHigh = 0x17, /**< @brief Pressure reference (MSB)(R). */
		I3CControl = 0x19, /**< @brief I3C control (R/W). */
		PressureOffsetLow = 0x1A, /**< @brief Pressure offset (LSB)(R). */
		PressureOffsetHigh = 0x1A, /**< @brief Pressure offset (MSB)(R). */
		InterruptSource = 0x24, /**< @brief Interrupt source for differential pressure (R). */
		FIFOStatus1 = 0x25, /**< @brief FIFO status (R). */
		FIFOStatus2 = 0x26, /**< @brief FIFO status (R). */
		Status = 0x27, /**< @brief Status (R). */
		PressureOutLow = 0x28, /**< @brief Either pressure output or AH/Qvar output value LSB data (R). */
		PressureOutMid = 0x29, /**< @brief Either pressure output or AH/Qvar output value middle data (R). */
		PressureOutHigh = 0x2A, /**< @brief Either pressure output or AH/Qvar output value MSB data (R). */
		TemperatureOutLow = 0x2B, /**< @brief Temperature LSB output (R). */
		TemperatureOutHigh = 0x2C, /**< @brief Temperature MSB output (R). */
		FIFOOutLow = 0x78, /**< @brief Either FIFO pressure or AH/Qvar output LSB data (R). */
		FIFOOutMid = 0x79, /**< @brief Either FIFO pressure or AH/Qvar output middle data (R). */
		FIFOOutHigh = 0x7A /**< @brief Either FIFO pressure or AH/Qvar output MSB data (R). */
	};

	/**
	 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::Interrupt
	 * 
	 */
	enum class InterruptBitmap_t : uint8_t
	{
		PressureHighEvent = 0, /**< @brief Enable interrupt on pressure high event. */
		PressureLowEvent = 1, /**< @brief Enable interrupt on pressure low event. */
		InterruptLatch = 2, /**< @brief Latch interrupt request. */
		ResetAutoZero = 4, /**< @brief Reset autozero function. */
		AutoZero = 5, /**< @brief Enable autozero function. */
		ResetAutoREFP = 6, /**< @brief Reset auto REFP function. */
		AutoREFP = 7 /**< @brief Enable auto REFP function. */
	};

	/**
	 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::Interface
	 * 
	 */
	enum class InterfaceBitmap_t : uint8_t
	{
		SSPullUpEnable = 1, /**< @brief Enable pull-up on slave select line. */
		SDAPullUpEnable = 4, /**< @brief Enable pull-up on SDA line. */
		SPIReadEnable = 5, /**< @brief Enable SPI read mode. */
		I2CDisable = 6 /**< @brief Disable I2C and I3C interface. */
	};

	/**
	 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::Control1
	 * 
	 */
	enum class Control1Bitmap_t : uint8_t
	{
		Average = 0, /**< @brief Average selection. */
		OutputDataRate = 3 /**< @brief Output data rate. */
	};

	/**
	 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::Control2
	 * 
	 */
	enum class Control2Bitmap_t : uint8_t
	{
		OneShot = 0, /**< @brief One shot mode enable. */
		Reset = 2, /**< @brief Software reset. */
		BlockDataUpdate = 3, /**< @brief Block data update until read. */
		LowPassFilterEnable = 4, /**< @brief Enable low pass filter. */
		LowPassFilterConfig = 5, /**< @brief Config low pass filter. */
		FullScale = 6, /**< @brief Enable full scale mode (from 1260hPa to 4060hPa). */
		Boot = 7 /**< @brief Reboot memory content. */
	};

	/**
	 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::Control3
	 * 
	 */
	enum class Control3BitMap_t : uint8_t
	{
		AddressIncrement = 0, /**< @brief Register address automatically incremented during a multiple byte access with a serial interface. */
		AnalogHubInterleaved = 5, /**<@brief Enables AH/Qvar and pressure hardware interleaved mode. */
		AnalogHubEnable = 7 /**< @brief Enable analog uhb/Qvar. */
	};

	/**
	 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::ControlFIFO
	 * 
	 */
	enum class FIFOControlBitmap_t : uint8_t
	{
		Mode = 0, /**< @brief FIFO mode control. */
		//TriggerMode = 2, // Glued with Mode value
		WatermarkStop = 3, /**< @brief Stop on FIFO waftermark. */
		AnalogHubFIFO = 4 /**< @brief Enables AH/Qvar and pressure hardware interleaved mode in FIFO buffer */
	};

	/**
	 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::I3CControl
	 * 
	 */
	enum class I3CControlBitmap_t : uint8_t
	{
		BusPeriod = 0, /**< @brief Bus available period when I3C IBI is used */
		AntiSpikeFilter = 5, /**< @brief Enable anti-spike filter. */
	};

	/**
	 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::InterruptSource
	 * 
	 */
	enum class InterruptSourceBitmap_t : uint8_t
	{
		PressureHigh = 0, /**< @brief Active high pressure interrupt. */
		PressureLow = 1, /**< @brief Active low pressure interrupt. */
		ActiveInterrupt = 2, /**< @brief One or more interrupts are active. */
		Boot = 7, /**< @brief Ongoing reboot. */
	};

	/**
	 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::FIFOStatus2
	 * 
	 */
	enum class FIFOStatus2Bitmap_t : uint8_t
	{
		FIFOFull = 5, /**< @brief FIFO full status. */
		FIFOOverrun = 6, /**< @brief FIFO overrun status. */
		FIFOWatermark = 7 /**< @brief FIFO watermark status. */
	};

	/**
	 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::Status
	 * 
	 */
	enum class StatusBitmap_t : uint8_t
	{
		PressureAvailable = 0, /**< @brief Pressure data available. */
		TemperatureAvailable = 1, /**< @brief Temperature data available. */
		PressureOverrun = 4, /**< @brief Pressure overrun. */
		TemperatureOverrun = 5 /**< @brief Temperature overrun. */
	};

	/**
	 * @brief Enum class with average selection values.
	 * 
	 */
	enum class Average_t : uint8_t
	{
		Average4 = 0b000,
		Average8 = 0b001,
		Average16 = 0b010,
		Average32 = 0b011,
		Average64 = 0b100,
		Average128 = 0b101,
		Average512 = 0b111
	};
	static constexpr uint8_t averageMask = 0b111; /**< @brief Bit mask for average selection. */

	/**
	 * @brief Enum class with output data rate values.
	 * 
	 */
	enum class OutputDataRate_t : uint8_t
	{
		OneShot = 0b0000, /**< @brief Power-down mode. */
		ODR1Hz = 0b0001,
		ODR4Hz = 0b0010,
		ODR10Hz = 0b0011,
		ODR25Hz = 0b0100,
		ODR50Hz = 0b0101,
		ODR75Hz = 0b0110,
		ODR100Hz = 0b0111,
		ODR200Hz = 0b1000
	};
	static constexpr uint8_t outputDataRateMask = 0b1111; /**< @brief Bit mask for output data rate. */

	/**
	 * @brief Enum class with FIFO mode values.
	 * 
	 */
	enum class FIFOMode_t : uint8_t
	{
		Bypass = 0b000,
		FIFO = 0b001,
		Continuous = 0b011,
		Bypass2FIFO = 0b101,
		Bypass2Continous = 0b110,
		Continous2FIFO = 0b111
	};
	static constexpr uint8_t FIFOModeMask = 0b111; /**< @brief Bit mask for FIFO mode. */

	/**
	 * @brief Enum class with I3C bus periods.
	 * 
	 */
	enum class I3CBusPeriod_t : uint8_t
	{
		Period50us = 0b00,
		Period2us = 0b01,
		Period1ms = 0b10,
		Period25ms = 0b11
	};
	static constexpr uint8_t busPeriodMask = 0b11; /**< @brief Bit mask for bus period. */


	// ----- VARIABLES
	static constexpr E& interface = static_cast<E&>(*this); /**< @brief Pointer to interface object. */
	static constexpr uint8_t timeout = 10; /**< @brief Driver R/W timeout in ms. */
	static constexpr uint8_t chipID = 0xB4; /**< @brief Chip ID from register \ref Register_t::WhoAmI. */

	MSP_f mspInitHandler = nullptr; /**< @brief Pointer to external function for MSP init. */
	MSP_f mspDeinitHandler = nullptr; /**< @brief Pointer to external function for MSP deinit. */
	Delay_f delayHandler = nullptr; /**< @brief Pointer to external function for wait operations. */
	Tick_f tickHandler = nullptr; /**< @brief Pointer to external function for retrieving tick. */

	uint8_t txBuffer[6]; /**< @brief Buffer for outgoing data. */
	TemperatureScale_t temperatureScale = TemperatureScale_t::Celsius; /**< @brief Output scale for temperature. */
	Semaphore_t semaphore = Semaphore_t::Free; /**< @brief Bus process semaphore. */


	// ----- METHOD DEFINITIONS
	/**
	 * @brief Read multiple bytes from register.
	 * 
	 * @param reg Register address to read from.
	 * @param output Pointer to output buffer for bytes from \c reg
	 * @param len Number of bytes to read from \c reg
	 * 
	 * @return \c Return_t::OK on success. 
	 */
	Return_t readRegister(const Register_t reg, uint8_t* output, const uint8_t len) const
	{
		uint8_t tmp = reg;
		if (interface->write(tmp, 1) != Return_t::OK)
		{
			return Return_t::NOK
		}

		if (interface->read(output, len) != Return_t::OK)
		{
			return Return_t::NOK;
		}

		return Return_t::OK;
	}	

	/**
	 * @brief Read single byte from register.
	 * 
	 * @param reg Register address to read from.
	 * @param output Reference to output for value from \c reg
	 * 
	 * @return \c Return_t::OK on success.
	 */
	Return_t readRegister(const Register_t reg, uint8_t& output) const
	{
		uint8_t tmp = 0;
		if (readRegister(reg, &tmp, sizeof(tmp)) != Return_t::OK)
		{
			return Return_t::NOK;
		}

		output = tmp;
		return Return_t::OK;
	}

	/**
	 * @brief Write \c len bytes into register.
	 * 
	 * @param input Pointer to input bytes. \c input[0] must contain register address.
	 * @param len Length of \c input
	 * 
	 * @return \c Return_t::OK on success.
	 */
	inline Return_t writeRegister(const uint8_t* input, const uint8_t len) const
	{
		return interface->write(input, len);
	}

	/**
	 * @brief Read chip ID.
	 * 
	 * @param output Reference for output ID
	 * 
	 * @return \c Return_t::OK on success.
	 */
	inline Return_t whoAmI(uint8_t& output) const
	{
		return readRegister(Register_t::WhoAmI, output);
	}

	/**
	 * @brief Set interface config.
	 * 
	 * @param config Pointer to interface config. See \ref interface_cfg_t
	 * 
	 * @return \c Return_t::OK on success.
	 */
	Return_t interfaceConfig(const interface_cfg_t* config) const
	{
		uint8_t tmp[2];

		tmp[0] = Register_t::Interface;
		tmp[1] =	(config->i2ci3cOff << InterfaceBitmap_t::I2CDisable) |
					(config->sdaPullUp << InterfaceBitmap_t::SDAPullUpEnable) |
					(config->SPIRead << InterfaceBitmap_t::SPIReadEnable) |
					(config->ssPullUpOff << InterfaceBitmap_t::SSPullUpEnable);

		return writeRegister(tmp, sizeof(tmp));
	}


	protected:
	// ----- METHOD DEFINITIONS
	/**
	 * @brief Object constructor.
	 * 
	 * @param mspInit Pointer to external function for MSP init.
	 * @param mspDeinit Pointer to external function for MSP deinit.
	 * @param tempScale Output temperature scale. See \ref TemperatureScale_t
	 * @param wait Pointer to external function for handling wait state.
	 * @param tick Pointer to external function for fetching tick.
	 * 
	 * @return No return value.
	 */
	Driver(const MSP_f mspInit, const MSP_f mspDeinit, const TemperatureScale_t tempScale, const Delay_f wait, const Tick_f tick)
	{
		mspInitHandler = mspInit;
		mspDeinitHandler = mspDeinit;
		delayHandler = wait;
		tickHandler = tick;

		freeSemaphore();
		setTemperatureScale(tempScale);
		memset(txBuffer, 0, sizeof(txBuffer));
	}

	/**
	 * @brief Wait for free semaphore.
	 * 
	 * @return \c Return_t::Timeout if function timeouted.
	 * @return \c Return_t::OK if semaphore is released in time.
	 */
	Return_t wait(void) const
	{
		// Get system tick if tick handler is provided
		uint32_t tick = 0;
		if (tickHandler)
		{
			tick = tickHandler();
		}

		// Check for process semaphore
		while (isSemaphoreFree() == Return_t::NOK)
		{
			// Call wait handler if provided
			if (delayHandler)
			{
				delayHandler(1);
			}

			// Check for timeout if tick handler is provided
			if (tickHandler)
			{
				if ((tickHandler() - tick) > timeout)
				{
					return Return_t::Timeout;
				}
			}
		}

		return Return_t::OK;
	}

	/**
	 * @brief Check is semaphore free.
	 * 
	 * @return \c Return_t::NOK if semaphore is taken. 
	 * @return \c Return_t::OK if semaphore is free.
	 */
	inline Return_t isSemaphoreFree(void) const
	{
		if (semaphore == Semaphore_t::Free)
		{
			return Return_t::OK;
		}

		return Return_t::NOK;
	}
};

/**
 * @brief Class for ILPS22QS I2C operations.
 * 
 */
class I2C : protected Driver<I2C>
{
	public:
	// ----- METHOD DEFINITIONS
	/**
	 * @brief Object constructor.
	 * 
	 * @param i2cRead Pointer to external function for I2C read operations.
	 * @param i2cWrite Pointer to external function for I2C write operations.
	 * @param mspInit Pointer to external function for MSP init. Optional.
	 * @param mspDeinit Pointer to external function for MSP deinit. Optional.
	 * @param tempScale Output temperature scale. Optional. See \ref TemperatureScale_t
	 * @param wait Pointer to external function for handling wait state. Optional.
	 * @param tick Pointer to external function for fetching tick. Optional.
	 * 
	 * @return No return value.
	 */	
	I2C(const I2CRW_f i2cRead, const I2CRW_f i2cWrite,
		const MSP_f mspInit = nullptr, const MSP_f mspDeinit = nullptr,
		const TemperatureScale_t tempScale = TemperatureScale_t::Celsius,
		const Delay_f wait = nullptr, const Tick_f tick = nullptr) :
		Driver<I2C>(mspInit, mspDeinit, tempScale, wait, tick)
	{
		readHandler = i2cRead;
		writeHandler = i2cWrite;
	}

	/**
	 * @brief Object deconstrctor.
	 * 
	 * @return No return value.
	 */
	~I2C(void)
	{

	}

	/**
	 * @brief Read from sensor.
	 * 
	 * @param data Pointer to output buffer for data from the sensor.
	 * @param len Size of \c data buffer.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::Timeout on timeout.
	 * @return \c Return_t::OK on success.
	 */
	Return_t read(uint8_t* data, const uint8_t len)
	{
		if (wait() != Return_t::OK)
		{
			return Return_t::Timeout;
		}

		return readHandler(address, data, len, readTimeout);
	}

	/**
	 * @brief Write to sensor.
	 * 
	 * @param data Pointer to data to write to the sensor.
	 * @param len Size of \c data buffer.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::Timeout on timeout.
	 * @return \c Return_t::OK on success.
	 */
	Return_t write(uint8_t* data, const uint8_t len)
	{
		if (wait() != Return_t::OK)
		{
			return Return_t::Timeout;
		}

		return writeHandler(address, data, len, writeTimeout);
	}

	private:
	static constexpr uint8_t address = 0x5C; /**< @brief ILPS22QS' address on I2C bus. */
	static constexpr uint8_t readTimeout = ILPS22QS_I2C_READ_TIMEOUT; /**< @brief Timeout in ms for read operation. */
	static constexpr uint8_t writeTimeout = ILPS22QS_I2C_WRITE_TIMEOUT; /**< @brief Timeout in ms for write operation. */

	I2CRW_f readHandler = nullptr; /**< @brief Pointer to external function for I2C read. */
	I2CRW_f writeHandler = nullptr; /**< @brief Pointer to external function for I2C write. */
};

/**
 * @brief Class for ILPS22QS 3-wire SPI operations.
 * 
 */
class SPI : protected Driver<SPI>
{
	public:

	private:
};

};


#endif // _ILPS22QS_HPP_


// END WITH NEW LINE
