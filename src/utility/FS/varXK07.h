/**
 * @file varXK07.h
 * @support (support@xinabox.cc)
 * @brief file for miscellaneous constants
 */
#ifndef varXK07_h
#define varXK07_h

/********* SYSTEM VARIABLES *********/
extern uint8_t recvCMD;   // = 0;
#define ACK 0xFF
extern bool sendDATA; // = false;
extern int receivedRSSI; // = 0;
extern uint8_t len; // = 0;
extern uint8_t I2C_BUF[5];
extern uint8_t bufidx; // = 0;
extern uint16_t GS_MISSING; // = 0;
  // Station Define
extern bool SAT;  // = false;
extern uint8_t GS_bytesAvailable; // = 0;
extern char INTERFACE_MODE[6];
extern char RADIO_ID_GS[6];
/********* DEBUG PRINT CONTROL *********/
extern bool PRINT_DEBUG; // = false;
extern bool PRINT_EN; // = false;
extern bool GPS_FIX; // = false;
/********* DEBUG PRINT CONTROL *********/
struct Config {
  int RADIO_ID;
  String RADIO_TYPE;
};
/********* TIMER VARIABLES *********/
extern long satTimeout;
extern long tick_readSensor;
extern long tick_Recieve;
extern long tick_WDT;
extern long tick_SN01;
extern long tick_Fix;
extern long tick_Last_Recieve;
extern long tick_Location;
extern long tick_LOG;
extern long tick_LED;
extern long tick_TELEMETRY;
/********* SD CARD **********/
extern bool Ready_SD;
extern char satBuffer[150];
/********* XCHIP I2C ADDRESS **********/
#define SN01_ADDRESS 0x42

#define SL01_ADDRESS_1 0x10
#define SL01_ADDRESS_2 0x29

#define SW01_ADDRESS 0x76
#define SW01_ADDRESS_ALT 0x77

#define SG33_ADDRESS 0x5A
#define SG33_ADDRESS_ALT 0x5B

#define PB04_ADDRESS 0x36

#define SI01_ADDRESS_1 0x1C
#define SI01_ADDRESS_1_ALT 0x1E
#define SI01_ADDRESS_2 0x6A
#define SI01_ADDRESS_2_ALT 0x6B

#define RL0X_ADDRESS 0x2F

#define OD01_ADDRESS 0x3C
#define OD01_ADDRESS_ALT 0x3D
/********* END **********/

#endif