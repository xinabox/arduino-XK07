/**
 * @file provXK07.h
 * @support (support@xinabox.cc)
 * @brief XinaBox class to handle provisioning on XK07 Ground Station
 */

#ifndef provXK07_h
#define provXK07_h

#if defined(ARDUINO_ARCH_ESP8266) // Include guard otherwise cpp files get compiled first and cause compilations errors

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <xProvision.h>  // https://github.com/xinabox/arduino-Provision
#include "utility/xk07global.h"

//#define DEBUG_SERIAL

class provXK07: public xProvision
{
	public:
		/**
		 * @brief Construct a new provXK07 object
		 *
		 */
		provXK07(void) {};

		/**
		 * @brief Destroy the provXK07 object
		 *
		 */
		~provXK07(void) {};

		/**
		 * @brief set up everything needed for provisiong
		 *
		 * @return true if everything was set up
		 * @return false if setup failed
		 */
		bool setupProvisioning(void);

		/**
		 * @brief waits for data to be sent from the XinaBox_Uploader
		 *
		 * @return true if information was successfully received
		 * @return false if information could not be received
		 */
		bool getData(void);

		/**
		 * @brief Get the unit name
		 *
		 * @return const String the unit name
		 */
		String getUnitName(void);

		/**
		 * @brief get the wifi SSID
		 *
		 * @return const String SSID
		 */
		String getSSID(void);

		/**
		 * @brief Get the wifi Password
		 *
 		 * @return const String password
		 */
		String getPassword(void);
		String getRadio(void);

	private:
		xProvision prv = xProvision();
		String wifiSSID, wifiPSK, unitName, radio;

		/**
		 * @brief
		 *
		 * @param _data
		 */
		void debugPrint(String _data);

		/**
		 * @brief
		 *
		 * @param _data
		 * @param _num
		 */
		void debugPrint(String _data, uint16_t _num);
};

#endif
#endif