/**
 * @file devXK07.h
 * @support (support@xinabox.cc)
 * @brief Class for handling Radio Communication on XK07 Flight Station
 */

#ifndef devXK07_h
#define devXK07_h

#if defined(ARDUINO_SAMD_ZERO)

// Library Includes
#include <xCore.h> // https://github.com/xinabox/arduino-CORE
#include <xSW01.h> // https://github.com/xinabox/arduino-SW01
#include <xSL01.h> // https://github.com/xinabox/arduino-SL01
#include <xSI01.h> // https://github.com/xinabox/arduino-SI01
#include <xSG33.h> // https://github.com/xinabox/arduino-SG33
#include <xSN01.h> // https://github.com/xinabox/arduino-SN01
#include <xPB04.h> // https://github.com/xinabox/arduino-PB04
#include <xOC01.h>
#include "utility/xk07global.h"
#include "utility/uiXK07.h"
#define MAX_PACKET_SIZE 150
#define DEBUG_SERIAL
struct i2cAddr
{
	uint8_t SN01_ADDRESS = 0x42;

	uint8_t SL01_ADDRESS_1 = 0x10;
	uint8_t SL01_ADDRESS_2 = 0x29;
	uint8_t SL01_ADDRESS_3 = 0x52;

	uint8_t SW01_ADDRESS = 0x76;
	uint8_t SW01_ADDRESS_ALT = 0x77;

	uint8_t SG33_ADDRESS = 0x5A;
	uint8_t SG33_ADDRESS_ALT = 0x5B;

	uint8_t PB04_ADDRESS = 0x36;

	uint8_t SI01_ADDRESS_1 = 0x1C;
	uint8_t SI01_ADDRESS_1_ALT = 0x1E;
	uint8_t SI01_ADDRESS_2 = 0x6A;
	uint8_t SI01_ADDRESS_2_ALT = 0x6B;

	uint8_t RL0X_ADDRESS = 0x2F;

	uint8_t OD01_ADDRESS = 0x3C;
	uint8_t OD01_ADDRESS_ALT = 0x3D;
};

typedef struct packetData
{
	uint8_t packet[MAX_PACKET_SIZE];
	uint8_t packetSize = 0;

};

class devXK07 : public xSW01, public xSL01, public xSI01, public xSG33, public xSN01, public xPB04
{
	public:
	void uifs();
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

		/**
		 * @brief
		 *
		 */
		void initSensors(void);

		/**
		 * @brief
		 *
		 * @param _lastRSSI
		 * @return uint8_t*
		 */
		uint8_t *payloadA(int16_t _lastRSSI);

		/**
		 * @brief
		 *
		 * @param _lastRSSI
		 * @return uint8_t*
		 */
		uint8_t *payloadB(int16_t _lastRSSI);

		/**
		 * @brief
		 *
		 * @return uint8_t*
		 */
		uint8_t *payloadC(int16_t _lastRSSI);

		uint8_t getPacketSize(void);

		bool testMovement();

	private:
		friend class FlightStation;

		bool sl01flag=false;
		i2cAddr i2c;

		packetData pdata;

		uint8_t xchipSensorStatus = 0;

		xSG33 SG33 = xSG33();

		/**
		 * @brief ping the SN01
		 *
		 * @return true if the SN01 is connected
		 * @return false if the SN01 isn't connected
		 */
		bool checkSN01(void);

		/**
		 * @brief ping the SL01
		 *
		 * @return true if the SL01 is connected
		 * @return false if the SL01 isn't connected
		 */
		bool checkSL01(void);

		/**
		 * @brief ping the SW01
		 *
		 * @return true if the SW01 is connected
		 * @return false if the SW01 isn't connected
		 */
		bool checkSW01(void);

		/**
		 * @brief ping the SG33
		 *
		 * @return true if the SG33 is connected
		 * @return false if the SG33 isn't connected
		 */
		bool checkSG33(void);

		/**
		 * @brief ping the PB04
		 *
		 * @return true if the PB04 is connected
		 * @return false if the PB04 isn't connected
		 */
		bool checkPB04(void);

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		bool checkSI01(void);

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
		uint8_t uiCnt=0;
};

#endif
#endif