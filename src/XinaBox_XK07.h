/**
 * @file XinaBox_XK07.h
 * @bb (bb@xinabox.cc)
 * @brief XinaBox XK07 Firmware Library
 */

#ifndef XinaBox_XK07_h
#define XinaBox_XK07_h

//#define XK07_FW_VERSION "2.0.0"

#include "Wire.h"

#if defined(ARDUINO_SAMD_ZERO) // Flight Station
#include "utility/FS/FlightStationXK07.h"
//#include <xCS11_SDU.h>
class XinaBox_XK07: public FlightStation     // if CS11, inherit FlightStation class
#endif

#if defined(ARDUINO_ARCH_ESP8266) // Ground Station]
#include "utility/GS/GroundStationXK07.h"
class XinaBox_XK07: public GroundStation		// if CW01, inherit GroundStation class
#endif
{
	public:
		/**
		 * @brief Construct a new devXK07 object
		 *
		 */
		XinaBox_XK07(){};

		/**
		 * @brief Destroy the devXK07 object
		 *
		 */
		~XinaBox_XK07(){};


	private:

};

#endif
