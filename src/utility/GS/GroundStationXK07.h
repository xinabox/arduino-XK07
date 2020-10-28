/**
 * @file GroundStationXK07.h
 * @support (support@xinabox.cc)
 * @brief
 *
 */

#ifndef GroundStation_h
#define GroundStation_h

#if defined(ARDUINO_ARCH_ESP8266) // Include guard otherwise cpp files get compiled first and cause compilations errors

// #include "mqttXK07.h"
#include "comXK07.h"
#include "provXK07.h"
#include "appXK07.h"
#include "devXK07.h"
#include "utility/uiXK07.h"
#include "utility/xk07global.h"

#define MODE_GS 0x10
#define MODE_FAULT 0x20
#define RUN_OK 0xFF

#define DEBUG_SERIAL

#define fwBeta "B2.0.1"

class GroundStation : public appXK07, public provXK07, public comXK07, /*public uiXK07,*/ public devXK07
{
	public:
		/**
		 * @brief Construct a new GroundStation object
		 *
		 */
		GroundStation(void){};

		/**
		 * @brief Destroy the GroundStation object
		 *
		 */
		~GroundStation(void){};

		/**
		 * @brief sets up all the classes needed for the GS to operate
		 *
		 * @return true everything was successfully set up
		 * @return false one or more classes couldnt be set up
		 */
		bool setupGroundStation(void);

		/**
		 * @brief
		 *
		 */
		bool begin(void);

		/**
		 * @brief
		 *
		 * param displayLogo
		 * @param displayMode
		 */
		bool begin(unsigned long displayLogo, unsigned long displayMode);

		/**
		 * @brief
		 *
		 */
		void loop(void);

		/**
		 * @brief
		 *
		 */
		void gsLoop(void);

		/**
		 * @brief
		 *
		 * @param _recvdata
		 * @param _sizeData
		 */
		uint8_t decodePacket(uint8_t _recvdata [], uint8_t _sizeData);

	private:
	bool almaDown=false;
		friend class XinaBox_XK07;

		comXK07 com = comXK07();
		appXK07 app; // = appXK07();

		uint8_t xchipSensorStatus = 0, GS_MODE = 0;

		/**
		 * @brief
		 *
		 * @param _almdata
		 * @param _almSize
		 * @return true
		 * @return false
		 */
		bool sendAlmanacFS(uint8_t* _almData, int _almSize);

		/**
		 * @brief
		 *
		 * @param long
		 * @param long
		 * return true
		 * @return false
		 */
		bool displayBootInfo(unsigned long _fwdisplaytime, unsigned long _fslogodisplay);

		/**
		 * @brief Set the Ground Station Mode object
		 *
		 * @param _mode_set_check
		 */
		void setGroundStationMode(uint8_t _mode_set_check);

		/**
		 * @brief
		 *
		 * @param _errorCode
		 * @return true
		 * @return false
		 */
		bool errorHandler(uint8_t _errorCode);

		/**
		 * @brief
		 *
		 * @param _xchipData
		 * @return uint8_t
		 */
		void decodeGroundStationChips(uint8_t _xchipData);

		/**
		 * @brief
		 *
		 * @return uint8_t
		 */
		uint8_t checkGroundStation(void);

		/**
		 * @brief
		 *
		 * @return uint8_t
		 */
		uint8_t checkGroundStationChips(void);

		/**
		 * @brief
		 *
		 */
		void modeFAULT(void);

		/**
		 * @brief
		 *
		 */
		void modeGS(void);

		/**
			 * @brief
			 *
			 * @return true
			 * @return false
			 */
		bool syncFS(void);

		/**
		 * @brief
		 *
		 * @return uint8_t
		 */
		uint8_t startProvisionRoutine(void);

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		bool configSAT(void);

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

	protected:
		uint8_t syncReady = 0;
		String _wifiSSID, _wifiPSK, _deviceName, _radio;
};

#endif
#endif
