/**
 * @file devXK07.h
 * @support (support@xinabox.cc)
 * @brief XinaBox XK07 Firmware Library
 */

#ifndef FlightStation_h
#define FlightStation_h

#if defined(ARDUINO_SAMD_ZERO)

#include <Arduino.h> // must be here

#include "utility/uiXK07.h"
#include <xCS11_SDU.h>
#include "comXK07.h"
#include "devXK07.h"
#include "logXK07.h"
#include "utility/xk07global.h"

#define MODE_SAT 0x10
#define MODE_LOG 0x20
#define RUN_OK 0xFF

#define DEBUG_SERIAL

#define LED_BLUE 4
#define LED_RED 11
#define LED_GREEN 12

class FlightStation : public comXK07, public devXK07,  public logXK07
{
    public:
        /**
         * @brief Construct a new FlightStation object
         *
         */
        FlightStation(){};

        /**
         * @brief Destroy the FlightStation object
         *
         */
        ~FlightStation(){};

        /**
         * @brief
         *
         */
        void begin(void);

        /**
         * @brief
         *
         * @param displayLogo
         * @param displayMode
         */
        void begin(unsigned long displayLogo, unsigned long displayMode);

        /**
         * @brief
         *
         */
        void loop(void);

    private:
    bool pa = false;
	    logXK07 log = logXK07();
	    comXK07 com = comXK07();
        struct radioData _radioData;
        /**
         * @brief
         *
         * @param _packetData
         * @return uint8_t
         */
        uint8_t parseDataPacket(uint8_t _packetData[]);

        /**
         * @brief
         *
         * @param long
         * @param long
         * @return true
         * @return false
         */
        bool displayBootInfo(unsigned long _fwdisplaytime, unsigned long _fslogodisplay);

        /**
         * @brief
         *
         * @return uint8_t
         */
        uint8_t checkFlightStation(void);

        /**
         * @brief Set the Flight Station Mode object
         *
         * @param _mode_set_check
         */
        void setFlightStationMode(uint8_t _mode_set_check);

        /**
         * @brief
         *
         */
        void modeSAT(void);

        /**
         * @brief
         *
         */
        void modeLOG(void);

        /**
         * @brief Get the Flight Info object
         *
         * @return true
         * @return false
         */
        bool getFlightInfo(void);

        /**
         * @brief
         *
         * @param _reccvdata
         * @param _sizeData
         * @return uint8_t
         */
        uint8_t decodePacket(uint8_t _recvdata [], uint8_t _sizeData);

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		bool syncGS(void);

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
        void decodeFlightStationChips(uint8_t _xchipData);

        /**
         * @brief
         *
         * @return uint8_t
         */
        uint8_t checkFlightStationChips(void);

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

        /**
         * @brief
         *
         */
        void satLoop(void);

	    uint8_t xchipSensorStatus = 0;
        uint8_t FS_MODE = 0;
	 	uint8_t syncReady = 0;

    protected:
        bool syncDone = false;

};
#endif
#endif
