/**
 * @file comXK07.h
 * @support (support@xinabox.cc)
 * @brief Class for handling Radio Communication on XK07 Flight Station
 */

#ifndef comXK07_h
#define comXK07_h

#if defined(ARDUINO_SAMD_ZERO)

#include <xCore.h> // link
#include <xRL0x.h> // link
#include "utility/uiXK07.h"
#include "utility/xk07global.h"
#include <Adafruit_SleepyDog.h>
#define MAX_PACKET_SIZE 150
//#define DEBUG_SERIAL
#define RUN_OK 0xFF

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

const String assistFile = "ASSIST.UBX";
const uint8_t SyncSequence[] = "$SYNC";
const uint8_t StartSequence[] = "$START";
const uint8_t EndSequence[] = "$END";
const uint8_t AckSequence[] = "$ACK";
const uint8_t NackSequence[] = "$NACK";
const uint8_t ConfigSequence[] = "$CONF";
const uint8_t ReadySequence[] = "$READY";

class comXK07
{
public:
void receiveRadioParameters();
	/**
		 * @brief Construct a new comXK07 object
		 *
		 */
	comXK07(){};

	/**
		 * @brief Destroy the comXK07 object
		 *
		 */
	~comXK07(){};

	/**
		 * @brief
		 *
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
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
	bool waitSync(void);

	/**
		 * @brief Attemps to receive the entire almanac file from the GS via a series of packets. When a valid packet is received, it is sent to the SN01
		 *
		 * @return true if the almanac was successfully received and loaded onto the SN01`
		 * @return false if the almanac couldn't be received or loaded onto the SN01
		 */
	const char* receiveAlmanac(void);

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

	const char* getAlmanac();
	void restartFlightStation(uint8_t buf[]);

private:
	const char *almanac_char;

	radioData rdata;
	int data_length;
	bool cmdPacket = false;
	int16_t _lastRSSI = 0;

	/**
		 * @brief Initialize Radio unit
		 *
		 * @return true if radio was successfully Initialized
		 * @return false if radio could not be Initialized
		 */
	uint8_t setupRadio(void);

	/**
		 * @brief checks if two arrays are equal
		 *
		 * @param a the first uint8_t array
		 * @param b the second uint8_t array
		 * @param n the size of the arrays
		 * @return true if the arrays are equal
		 * @return false if the arrays are not equal
		 */
	bool equalArrays(const uint8_t a[], const uint8_t b[], uint8_t n);

	/**
		 * @brief tries to receive Start Sequence from GS.
		 *
		 * @return true if a Start sequence [START] was received
		 * @return false if nothing was received, or the message received was not the start sequence.
		 */
	bool receiveStartSequence(void);

	/**
		 * @brief tries to receive an End sequence from the GS.
		 *
		 * @return true if a Start sequence [END] was received
		 * @return false if nothing was received, or the message received was not the end sequence.
		 */
	bool receiveEndSequence(void);

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

	String _frequency;

protected:
};

#endif
#endif