/**
 * @file devXK07.h
 * @support (support@xinabox.cc)
 * @brief Class for handling Radio Communication on XK07 Flight Station
 */

#ifndef devXK07_h
#define devXK07_h

#if defined(ARDUINO_ARCH_ESP8266) // Include guard otherwise cpp files get compiled first and cause compilations errors

// Library Includes
#include <xCore.h> // https://github.com/xinabox/arduino-CORE
#include "utility/xk07global.h"

struct i2cAddr
{
	uint8_t RL0X_ADDRESS = 0x2F;

	uint8_t OD01_ADDRESS = 0x3C;
	uint8_t OD01_ADDRESS_ALT = 0x3D;
};


class devXK07
{
	public:
		/**
		 * @brief Construct a new devXK07 object
		 *
		 */
		devXK07() {};

		/**
		 * @brief Destroy the devXK07 object
		 *
		 */
		~devXK07() {};

		/**
		 * @brief checks if all required sensors as well as the RL0X and OD01 are connected
		 *
		 * @return uint8_t value of availble sensors in the form SN01 SL01 SW01 SG33 PB04 SI01 RL0X OD01 where a 1 means the sensor is availble
		 */
		uint8_t CheckXChips(void);



	private:
		i2cAddr i2c;
		uint8_t xchipSensorStatus = 0;

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		bool checkRL0X(void);

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		bool checkOD01(void);



		/**
		 * @brief
		 *
		 * * @param _data
		 */
		void debugPrint(String _data);

		/**
		 * @brief
		 *
		 * @param _data
		 */
		void debugPrint(uint8_t _data);
};

#endif
#endif
