/**
 * @file uiXK07.h
 * @support (support@xinabox.cc)
 * @brief Class for displaying information to OD01 on XK07 Flight Station
 */

#ifndef uiXK07_h
#define uiXK07_h

#define OD01_ADRESS 0x3C

#include "Arduino.h"
#include <xCore.h> // https://github.com/xinabox/arduino-CORE
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "xk07global.h"

#define SSD1306_128_64

class uiXK07// : public Adafruit_SSD1306
{
  	public:
		/**
		 * @brief Construct a new ui XK07 object
		 *
		 */
		uiXK07();

		/**
		 * @brief Destroy the log XK07 object
		 *
		 */
		~uiXK07();

		void begin();
		/**
		 * @brief
		 *
		 */
		void drawXinaBoxLogo(void);

		/**
		 * @brief
		 *
		 */
		void drawWiFiConnected(void);

		/**
		 * @brief
		 *
		 */
		void drawConfigSaved(void);

		/**
		 * @brief
		 *
		 * @param _countDown
		 */
		void drawConnectingWiFi(uint8_t _countDown);

		/**
		 * @brief
		 *
		 */
		void drawProvisionSuccess(void);

		/**
		 * @brief
		 *
		 */
		void drawConnectingWiFi(void);

		void drawLogoFS(void);

		void drawLogoGS(void);

		void setFirmwareVersion(String _firmwareVersion);

		void drawPixel(int16_t, int16_t, uint16_t);
		void iconAndString(const uint8_t *bitmap, const char* str, int x = 112, int y = 56, int x1 = 8, int y1 = 13);
		void connectingToWifi(const char* str);
		void wifiPass(const char* str);
		void wifiFail(const char* str);
		void loading(const char* str);
		void provFail(const char *str);
		void provPass(const char *str);
		void fsPass(const char* str);
		void fsFail(const char* str);
		void fsConnecting(const char* str);
		void almDownloading(const char* str);
		void almDownloaded(const char* str);
		void almDownloadedFail(const char* str);
		void info(String time, String data, String rssi, String batPercentage);
		void sn01(uint8_t status, String lat, String lon, String alt);
		void si01(bool status, String a, String x, String z);
		void sw01(bool status, String temp, String hum, String pres);
		void sg33(bool status, String co2, String voc);
		void sl01(bool status, String lux, String uvi);
		void sdPass();
		void sdFail();
		void clearScreen();
		void uifs(bool sn01, bool sg33, bool si01, bool sw01, bool sl01, bool rl0x, bool pb04);

  	private:
	  	// friend class FlightStation;
		// friend class GroundStation;
		uint8_t cnt;
		String fsState="";
		String firmwareVersion;
};
extern uiXK07 ui;

#endif