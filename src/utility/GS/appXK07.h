/**
 * @file appXK07.h
 * @support (support@xinabox.cc)
 * @brief XinaBox class to handle wifi on XK07 Ground Station
 */

#ifndef appXK07_h
#define appXK07_h

#if defined(ARDUINO_ARCH_ESP8266) // Include guard otherwise cpp files get compiled first and cause compilations errors

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "errXK07.h"
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient
#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
#include "xCore.h"		  // https://github.com/xinabox/arduino-CORE
#include <NTPtimeESP.h>
#include "utility/uiXK07.h"
#include "utility/xk07global.h"
#include <NTPClient.h>

#define MAX_PACKET_SIZE 150

typedef struct ubloxData
{
	const String GET_REQUEST_MESSAGE{"http://online-live1.services.u-blox.com/GetOnlineData.ashx?token=SjneqlMN-UG2zAzl_Egmdw;gnss=gps;datatype=eph,alm,aux,pos;filteronpos;format=aid"};
	const String GET_REQUEST_MESSAGE_ALT{"http://online-live2.services.u-blox.com/GetOnlineData.ashx?token=SjneqlMN-UG2zAzl_Egmdw;gnss=gps;datatype=eph,alm,aux,pos;filteronpos;format=aid"};
	const char* startSequence = "[START]";
	const char* google = "www.google.com";
	uint16_t googlePort = 80;
	uint8_t packetSize{120};
};

typedef struct mqttData
{
	const char* xinaboxMqttServerHostname = "mqtt.xinabox.cc";
	uint16_t xinaboxMqttServerPort1 = 80;
	uint16_t xinaboxMqttServerPort2 = 1883;
	String xinaboxMqttUsername = "xchip";
	String xinaboxMqttPassword = "Vy3t2VwhhuKMLSa26PnJ4eQa";
	String cw01ClientName;
	char indexName[30];
};

typedef struct satData
{
	uint8_t buffer[MAX_PACKET_SIZE];
	uint8_t bufferidx;
	bool SAT_SW01 = false, SAT_SL01 = false, SAT_SI01 = false, SAT_SN01 = false, SAT_PB04 = false, SAT_SG33 = false;
	bool SAT_RL03 = false, GS_RL03 = false, GS_RL02 = false, SAT_RL02 = false, GS_RL01 = false, SAT_RL01 = false;
	String SW01_T, SW01_H, SW01_P, SW01_A;
	String SW03_T, SW03_P;
	String SG33_CO2, SG33_TVOC;
	String SL01_L, SL01_A, SL01_B, SL01_I;
	String PB04_V, PB04_C;
	String SI01_GF, SI01_R, SI01_P, SI01_Y;
	String SN01_S, SN01_STAT, SN01_D, SN01_T, SN01_LAT, SN01_LONG, SN01_ALT, SN01_SOG, SN01_COG, SN01_HDOP, SN01_SAT;
	String SAT_RSSI, LOC_RSSI;
	String CALC_AR;
	String SAT_STATUS, SAT_ID, GS_ID, SAT_SENSOR;
	bool satMissing = false;
};

typedef struct oledInfo
{
	String day;
	String month;
	String year;
	String hour;
	String minute;
	String second;
	int rssi;
};

#define DEBUG_SERIAL

class appXK07//: public uiXK07
{
	public:
		/**
		 * @brief Construct a new appXK07 object
		 *
		 */
		appXK07(void) {};

		/**
		 * @brief Destroy the wifiXK07 object
		 *
		 */
		~appXK07(void) {};

		/**
		 * @brief setup and connect to xinabox wifi
		 *
		 * @return true connection was successfull
		 * @return false if could not connect to xinbox wifi
		 */
		bool connectWiFi();

		/**
		 * @brief setup and connect to wifi
		 *
		 * @param SSID the ssid to connect to
		 * @param password the wifi password
		 * @return true if could connect to wifi
		 * @return false if could not connect to wifi
		 */
		bool connectWiFi(const char *ssid, const char *psk);

		/**
		 * @brief Get the size of the almanac from ublox
		 *
		 * @return int the size in bytes of the almanac to be downloaded
		 */
		int getAlmanacSize(void);

		/**
		 * @brief get the almanac array
		 *
		 * @return uint8_t* the pointer to the almanac array
		 */
		uint8_t *getAlmanac(void);

		/**
		 * @brief download the almanac from ublox and store in array
		 *
		 * @return true if almanac could be downloaded
		 * @return false if almanac could not be downloaded
		 */
		bool downloadAlmanac(void);

		/**
		 * @brief check if can connect to internet
		 *
		 * @return true if could connect to internet
		 * @return false if could not connect to internet
		 */
		bool checkInternet(void);

		/**
		 * @brief Get the Wifi Signal Strength object
		 *
		 * @return uint32_t the RSSI of the received wifi signal in dBm
		 */
		int32_t getWifiSignalStrength(void);

		/**
		 * @brief Get the Local IP address
		 *
		 * @return String the ip address
		 */
		String getLocalIP(void);

		/**
		 * @brief
		 *
		 * @param _packet
		 * @return true
		 * @return false
		 */
		bool parseRadioBuffer(uint8_t _packet[]);

		/**
		 * @brief
		 *
		 * @param _packetRadio
		 * @return uint8_t
		 */
		uint8_t parseDataPacket(uint8_t _packetData[]);

		/**
		 * @brief
		 *
		 * @param _lastRSSI
		 */
		void publishMQTT(int16_t _lastRSSI);

		/**
		 * @brief
		 *
		 */
		void publishMQTT(void);

		/**
		 * @brief Set the Firmware Version object
		 *
		 * @param _fwversion
		 */
		void setFirmwareVersion(String _fwversion);

		/**
		 * @brief Set the Device Name object
		 *
		 * @param _dname
		 */
		void setDeviceName(String _dname);

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		bool connectMQTT(void);

		/**
		 * @brief Set the Mqtt Client Name object
		 *
		 */
		void setMqttClientName(void);

		/**
		 * @brief Set the Mqtt Index Name object
		 *
		 */
		void setMqttIndexName(void);
		String almanac_string;
		void uigs();
		void ownTracks();

	private:
	uint8_t uiCnt=0;
	struct oledInfo info;

	bool fmqtt=false;
		ESP8266WiFiMulti wifiMulti =  ESP8266WiFiMulti();
		// WiFiClient client =  WiFiClient();
		// HTTPClient http1;
		NTPtime NTPch = NTPtime("time.google.com");
		strDateTime dateTime;
		satData sdata;
		mqttData mdata;

		WiFiClient espClient = WiFiClient();
		PubSubClient mqtt = PubSubClient(espClient);

		errorCodeState errCode;
		errorCountsOccured errCount;

		uint8_t *almanac;
		int almSize{};

		char _firmwareVersion[10];
		char _deviceName[20];
		char _timeStamp[20];

		/**
		 * @brief
		 *
		 * @param _mqttPortNumber
		 * @return true
		 * @return false
		 */
		bool connectMQTT(uint16_t _mqttPortNumber);

		/**
		 * @brief Set the Mqtt Server Connection Setting object
		 *
		 * @param _mqttHostName
		 * @param _mqttPort
		 */
		void setMqttServerConnectionSetting(const char *_mqttHostName, uint16_t _mqttPort);

		/**
		 * @brief Set the Mqtt Server Connection Settings object
		 *
		 * @param _mqttPort
		 */
		void setMqttServerConnectionSetting(uint16_t _mqttPort);

		/**
		 * @brief
		 *
		 * @param _packet
		 * @return true
		 * @return false
		 */
		void parsePacketSnippet(uint8_t _packet[]);

		/**
		 * @brief
		 *
		 * @param str
		 * @return String
		 */
		String parseBuffer(char *str);

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
		 * @brief Get the Time Stamp object
		 *
		 * @return true
		 * @return false
		 */
		bool getTimeStamp(void);


};
#endif
#endif