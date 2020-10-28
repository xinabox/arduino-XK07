/**
 * @file comXK07.h
 * @support (support@xinabox.cc)
 * @brief Class for handling Radio Communication on XK07 Ground Station
 */

#ifndef comXK07_h
#define comXK07_h

#if defined(ARDUINO_ARCH_ESP8266) // Include guard otherwise cpp files get compiled first and cause compilations errors

#include <xCore.h> // links
#include <xRL0x.h> // links
#include "utility/uiXK07.h"
#include "utility/xk07global.h"
#define MAX_PACKET_SIZE 150
#define RUN_OK 0xFF
#define DEBUG_SERIAL

// Radio Defines
typedef struct radioData
{
	uint8_t RADIO_ID = 0x11;
	float RL03_FREQ = 915.0;
	float RL01_FREQ = 433.0;
	float RL02_FREQ = 868.0;
	uint8_t SN01_I2C_ADDR = 0x42;

	uint8_t packet[MAX_PACKET_SIZE];
	uint8_t packetSize = 0;
};

const String 	assistFile 		= "ASSIST.UBX";
const uint8_t SyncSequence [] 	= "$SYNC";
const uint8_t StartSequence [] 	= "$START";
const uint8_t EndSequence [] 	= "$END";
const uint8_t AckSequence [] 	= "$ACK";
const uint8_t NackSequence [] 	= "$NACK";
const uint8_t ConfigSequence [] = "$CONF";
const uint8_t ReadySequence [] 	= "$READY";

class comXK07
{
	public:
	bool testFSConnection();
	uint8_t cntTest=0;
		/**
		 * @brief Construct a new comXK07 object
		 *
		 */
		comXK07(void){};

		/**
		 * @brief Destroy the comXK07 object
		 *
		 */
		~comXK07(void){};

		/**
		 * @brief
		 *
		 * @return uint8_t
		 */
		uint8_t begin(void);

		/**
		 * @brief Set the Mode object
		 *
		 * @param _mode
		 * @return uint8_t
		 */
		uint8_t setMode(uint8_t _mode);

		/**
		 * @brief sends the start sequence over the radio which tells the FS that it is going to send data
		 *
		 * @return true if start sequence could be sent
		 * @return false if start sequence was not sent
		 */
		bool sendStartSequence(void);

		/**
		 * @brief sends the end sequence over the radio telling the FS that the data stream has ended
		 *
		 * @return true if end sequence could be sent
		 * @return false if end sequence was not sent
		 */
		bool sendEndSequence(void);

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		bool waitSync(void);

		/**
		 * @brief Send the whole almanac as a series of packets
		 *
		 * @param almanac the entire almanac file
		 * @param almSize the size of the almanac in bytes
		 * @return true 	if the almanac was sent
		 * @return false  if for some reason the almanac could not be sent
		 */
		bool sendAlmanac(uint8_t almanac[], int almSize);

		/**
		 * @brief sends the almanac to the FS
		 *
		 * @param almanac the almanac file
		 * @param almSize the number of bytes in the almanac
		 * @return true the almanac was sent
		 * @return false the almanac could not be sent
		 */
		bool sendPackets(const uint8_t almanac[], int almSize);

		/**
		 * @brief
		 *
		 * @param _packet
		 * @param _packetSize
		 * @return true
		 * @return false
		 */
		bool sendPacket(const uint8_t _packet[], uint8_t _packetSize);

		/**
		 * @brief
		 *
		 * @param _packetHeader
		 * @param _packet
		 * @param _packetSize
		 * @return true
		 * @return false
		 */
		bool sendPacket(const uint8_t _packetHeader[], uint8_t _packet[], uint8_t _packetSize);

		/**
		 * @brief Waits 1 second for a message to be received. If a packet is received it is saved.
		 *
		 * @return true if packet was successfully received
		 * @return false if nothing was received, or the message was not a proper packet
		 */
		bool receivePacket(void);

		/**
		 * @brief gets the last packet that was received and strips the first 2 bytes and the last byte from the array
		 *
		 * @return uint8_t returns the last packet received, with the bytes removed. The size of this array will be in data_length
		 */
		uint8_t *getPacket(void);

		/**
		 * @brief Get the Packet Size object
		 *
		 * @return uint8_t
		 */
		uint8_t getPacketSize(void);

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		bool dataPacket(void);

		/**
		 * @brief Get the Last R S S I object
		 *
		 * @return int16_t
		 */
		int16_t getLastRSSI(void);

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		bool sendConfigSequence(void);
		bool resetFlightStation();
		void parseSelectedRadio(String radio);

	private:
		//uiXK07 ui;
		radioData rdata;
		bool cmdPacket = false;
		int16_t _lastRSSI = 0;

		/**
		 * @brief
		 *
		 * @return uint8_t
		 */
		uint8_t setupRadio(void);

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
		 * @param _data
		 */
		void debugPrintFS(String _data);
		float _frequency;




};

#endif
#endif
