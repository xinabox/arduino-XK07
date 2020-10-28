/**
 * @file devXK07.cpp
 * @support (support@xinabox.cc)
 * @brief Class for handling Sensors on XK07 Flight Station
 */
#include "devXK07.h"

#if defined(ARDUINO_ARCH_ESP8266) // Include guard otherwise cpp files get compiled first and cause compilations errors

/**
 * @brief order of x-chips in 8-bit value : SN01 SL01 SW01 SG33 PB04 SI01 RL0X OD01
 * 
 * @return uint8_t 
 */
uint8_t devXK07::CheckXChips(void)
{
	uint8_t sensors_available = 0b00000000;

	if (checkRL0X())
	{
		sensors_available |= 0b00000010;
	}
	if (checkOD01())
	{
		sensors_available |= 0b00000001;
	}
	return sensors_available;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool devXK07::checkRL0X(void)
{
	if (xCore.ping(i2c.RL0X_ADDRESS))
	{
		return true;
	}

	return false;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool devXK07::checkOD01(void)
{
	if (xCore.ping(i2c.OD01_ADDRESS))
	{
		return true;
	}
	return false;
}

/**
 * @brief 
 * 
 * @param _data 
 */
void devXK07::debugPrint(String _data)
{
#ifdef DEBUG_SERIAL
	Serial.print("dev_xk07: ");
	Serial.println(_data);
#endif
}

/**
 * @brief 
 * 
 * @param _data 
 */
void devXK07::debugPrint(uint8_t _data)
{
#ifdef DEBUG_SERIAL
	Serial.print("dev_xk07: ");
	Serial.println(_data, BIN);
#endif
}

#endif