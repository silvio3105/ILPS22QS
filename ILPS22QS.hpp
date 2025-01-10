/**
 * @file ILPS22QS.hpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief ILPS22QS driver header file.
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


// ----- TYPEDEFS
/**
 * @brief Typedef for generic function without return value and arguments.
 * 
 */
typedef void (*Void_f)(void);

typedef void (*Delay_f)(const uint32_t period);
typedef uint32_t (*Tick_f)(void);
typedef Return_t (*I2CRW_f)(const uint8_t address, void* data, const uint8_t len, const uint8_t timeout);
typedef Return_t (*SPIRXTX_f)(const void* txData, const uint8_t txLen, void* rxData, const uint8_t rxLen, const uint8_t timeout);
typedef Return_t (*Select_f)(void);


// ----- CLASSES
/**
 * @brief Class for ILPS22QS driver.
 * 
 * @tparam External Object for external operations.
 */
template<class E>
class Driver
{
	protected:
	// ----- METHOD DECLARATIONS


	// ----- METHOD DEFINITIONS
	#ifdef ILPS22QS_IMPLEMENTATION
	~Driver(void)
	{
		if (mspDeinitHandler)
		{
			mspDeinitHandler();
		}

		memset(this, 0, sizeof(E));
	}	

	/**
	 * @brief Free semaphore for read and write operations.
	 * 
	 * This is required if async read and/or write is used(interrupt driven or DMA).
	 * 
	 * @return No return value.
	 */
	inline void freeSemaphore(void)
	{
		semaphore = Semaphore_t::Free;
	}	

	inline Return_t init(void) const
	{
		if (whoAmI() == chipID)
		{
			return Return_t::OK;			
		}

		return Return_t::NOK;
	}	
	#endif // ILPS22QS_IMPLEMENTATION


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
		LatchInterrupt = 2, /**< @brief Latch interrupt request. */
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
		OneShot = 0b0000,
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

	Void_f mspDeinitHandler = nullptr; /**< @brief Pointer to external function for MSP deinit. */
	Delay_f delayHandler = nullptr; /**< @brief Pointer to external function for wait operations. */
	Tick_f tickHandler = nullptr; /**< @brief Pointer to external function for retrieving tick. */

	uint8_t data[6];
	Semaphore_t semaphore = Semaphore_t::Free; /**< @brief Bus process semaphore. */ 

	// ----- METHOD DECLARATIONS


	// ----- METHOD DEFINITIONS
	uint8_t whoAmI(void) const
	{
		const uint8_t tmp = (uint8_t)Register_t::WhoAmI;
		if (interface->write(&tmp, 1) != Return_t::OK)
		{
			return 0;
		}

		if (interface->read(data, 1) != Return_t::OK)
		{
			return 0;
		}

		return data[0];
	}


	protected:
	// ----- METHOD DECLARATIONS


	// ----- METHOD DEFINITIONS
	#ifdef ILPS22QS_IMPLEMENTATION
	/**
	 * @brief Object constructor.
	 * 
	 * @param mspInit Pointer to external function for MSP init.
	 * @param mspDeinit Pointer to external function for MSP deinit.
	 * @param wait Pointer to external function for handling wait state.
	 * @param tick Pointer to external function for fetching tick.
	 * 
	 * @return No return value.
	 */
	Driver(const Void_f mspInit, const Void_f mspDeinit, const Delay_f wait, const Tick_f tick)
	{
		mspDeinitHandler = mspDeinit;
		delayHandler = wait;
		tickHandler = tick;
		semaphore = Semaphore_t::Free;
		memset(data, 0, sizeof(data));

		if (mspInit)
		{
			mspInit();
		}
	}

	/**
	 * @brief Wait function for free semaphore.
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

	/**
	 * @brief Take semaphore for read and write operations.
	 * 
	 * This is required if async read and/or write is used(interrupt driven or DMA).
	 * 
	 * @return No return value.
	 */
	inline void takeSemaphore(void)
	{
		semaphore = Semaphore_t::Taken;
	}

	#endif // ILPS22QS_IMPLEMENTATION	
};

/**
 * @brief Class for ILPS22QS I2C operations.
 * 
 */
class I2C : protected Driver<I2C>
{
	public:
	// ----- METHOD DECLARATIONS


	// ----- METHOD DEFINITIONS
	#ifdef ILPS22QS_IMPLEMENTATION
	I2C(const I2CRW_f i2cRead, const I2CRW_f i2cWrite, const Void_f mspInit = nullptr, const Void_f mspDeinit = nullptr, const Delay_f wait = nullptr, const Tick_f tick = nullptr) : Driver<I2C>(mspInit, mspDeinit, wait, tick)
	{
		readHandler = i2cRead;
		writeHandler = i2cWrite;
	}

	~I2C(void)
	{

	}

	Return_t read(uint8_t* data, const uint8_t len)
	{
		if (wait() != Return_t::OK)
		{
			return Return_t::Timeout;
		}

		takeSemaphore();
		return readHandler(address, data, len, readTimeout);
	}

	Return_t write(uint8_t* data, const uint8_t len)
	{
		if (wait() != Return_t::OK)
		{
			return Return_t::Timeout;
		}

		takeSemaphore();
		return writeHandler(address, data, len, writeTimeout);
	}

	#endif // ILPS22QS_IMPLEMENTATION

	private:
	static constexpr uint8_t address = 0x5C; /**< @brief ILPS22QS' address on I2C bus. */
	static constexpr uint8_t readTimeout = 10; /**< @brief Timeout in ms for read operation. */
	static constexpr uint8_t writeTimeout = 10; /**< @brief Timeout in ms for write operation. */

	I2CRW_f readHandler = nullptr; /**< @brief Pointer to external function for I2C transmit. */
	I2CRW_f writeHandler = nullptr; /**< @brief Pointer to external function for I2C receive. */
};

/**
 * @brief Class for ILPS22QS SPI operations.
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
