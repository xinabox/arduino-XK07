/**
 * @file errXK07.h
 * @bb (bb@xinabox.cc)
 * @brief XinaBox XK07 Firmware 
 */
#ifndef errXK07_h
#define errXK07_h

/**
 * @brief 
 * 
 */
typedef struct errorCodeState
{
  // Main Check
  bool ERROR_OCCURED = false;

  // Sensor Errors
  bool SW01_M = false;
  bool SL01_M = false;
  bool OD01_M = false;

  // MQTT Error
  bool MQTT_CONN_E = false;
  bool MQTT_PUB_E = false;
  bool MQTT_SELC = false;

  // Wi-Fi Error
  bool WIFI_CONN = false;
};

/**
 * @brief 
 * 
 */
typedef struct errorCountsOccured
{
  uint16_t count_MQTT = 0;
  uint16_t count_WIFI = 0;
};

/**************************** ERROR CODES *********************************/
#define SW01_E "E100"
#define SL01_E "E101"
#define SI01_E "E102"
#define SN01_E "E103"

#define BLTNK_E_CONN "E200"
#define BLYNK_E_AUTH "E201"

#define MQTT_E_CONN "E300"
#define MQTT_E_PUBB "E301"
#define MQTT_E_SELC "E302"

#define CAP_E_INDEX "E400"
#define CAP_E_LONG "E401"
#define CAP_E_LAT "E402"
#define CAP_E_ALT "E403"
#define CAP_E_NAME "E404"
#define CAP_E_ORG "E405"

#define WIFI_E_CONN "E001"

#endif