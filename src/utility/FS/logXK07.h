/**
 * @file logXK07.h
 * @support (support@xinabox.cc)
 * @brief Class for handling data logging on XK07 Flight Station
 */

#ifndef logXK07_h
#define logXK07_h

#if defined(ARDUINO_SAMD_ZERO)

#include <SPI.h> // STD Library
#include <SD.h>  // STD Library

#include "devXK07.h" //TODO - do we need to use dev class in log class?

#include <xCS11_SDU.h>
#include <xCore.h>
#include <xSW01.h>
#include <xSL01.h>
#include <xSI01.h>
#include <xSN01.h>
#include <xPB04.h>
#include <xRL0x.h>
#include <xSG33.h>
#include <utility/uiXK07.h>
#include "utility/xk07global.h"

const String headers[][2]{// TODO - could be improved by using seperate arrays for each sensor, or using vectors
						  {"Runtime", ""},
						  {"Date", ""},
						  {"Time", "(UTC)"},
						  {"Latitude", ""},
						  {"Longitude", ""},
						  {"GPS Altitude", "(m)"},
						  {"HDOP", ""},
						  {"SoG", "(kph)"},
						  {"CoG", "(deg)"},
						  {"Satellites", ""},
						  {"UVA", "(uW/cm^2)"},
						  {"UVB", "(uW/cm^2)"},
						  {"UV Index", ""},
						  {"Ambient Light", "(Lux)"},
						  {"Temperature", "(degC)"},
						  {"Pressure", "(Pa)"},
						  {"Humidity", "(RH%)"},
						  {"Altitude", "(m)"},
						  {"eC02", "(ppm)"},
						  {"TVOC", "(ppb)"},
						  {"Current", "(mA)"},
						  {"Voltage", "(V)"},
						  {"Roll", "(deg)"},
						  {"Pitch", "(deg)"},
						  {"G-Force", "(g)"},
						  {"Magnetometer X-Axis", "(gauss)"},
						  {"Magnetometer Y-Axis", "(gauss)"},
						  {"Magnetometer Z-Axis", "(gauss)"},
						  {"Gyroscope X-Axis", "(deg/s)"},
						  {"Gyroscope Y-Axis", "(deg/s)"},
						  {"Gyroscope Z-Axis", "(deg/s)"},
						  {"Accelerometer X-Axis", "(g)"},
						  {"Accelerometer Y-Axis", "(g)"},
						  {"Accelerometer Z-Axis", "(g)"},
						  {"Received RSSI", "(dB)"}};

const unsigned int HeaderArray_length{sizeof(headers) / sizeof(headers[0])};

//i2c Addresses
struct xchipi2caddr
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
};

const String InfoFileName{"XK07Info.txt"};
const String DataFolder{"/XK07Data"};

const unsigned int LOG_DELAY_LONG{120000};
const unsigned int LOG_DELAY_NORMAL{20000};
const unsigned int LOG_DELAY_SHORT{5000};

#define IMPORTANT 1
#define NORMAL 0

//#define DEBUG_SERIAL
#define CS11_SD_CS_PIN 3

class logXK07: public devXK07
{
  	public:
		/**
		 * @brief Construct a new logXK07 object
		 *
		 */
		logXK07(void);

		/**
		 * @brief Destroy the logXK07 object
		 *
		 */
		~logXK07(void);

		/**
		 * @brief setup the sd card, create a folder and log file
		 *
		 * @return true if SD card was setup and the folder and file were created.
		 * @return false if the SD card couldn't be set up properly, or the file couldn't be created
		 */
		uint8_t begin(void);

		/**
		 * @brief logs all data from all available sensors
		 *
		 * @return true if data was successfully logged
		 * @return false if data couldn't be logged
		 */
		uint8_t logAllSensors(int rssi, int id);

		/**
		 * @brief
		 *
		 * @param _enable
		 */
		void enableExcelLogOutput(bool _enable);

  	private:
	  bool sl01flag=false;

	  	xSG33 SG33 = xSG33();

		bool ENABLE_EXCEL_LOG = false;

		void initSensors(void);

		bool storageCardAvailable = false;
		bool storageCardReady = false;
		bool SD_CARD_INITIALIZED{false};

		unsigned int log_index;

		unsigned long allSeconds{};

		bool LOW_USE_MODE{false};

		File DataFile;
		File InfoFile;

		String DataFileName;

		unsigned long time_now{0};

		/**
		 * @brief Initialize the SD Card
		 *
		 * @return true if card was successfully Initialized
		 * @return false if card couldn't be Initialized
		 */
		bool initSD(void);

		/**
		 * @brief Writes to infofile on SD Card. If mode is set to true, it will write to SD Card even if LOW_USE_MODE is active
		 *
		 * @param info The information to be written
		 * @param mode The mode: Either IMPORTANT, in which case info is always written, or NORMAL, in which case info is sometimes written
		 * @return true if information was written
		 * @return false if information could not be written
		 */
		bool writeInfo(char info[], bool mode = NORMAL);

		/**
		 * @brief Writes all the headers in the headers array to the datafile
		 *
		 * @return true if successfully wrote to file
		 * @return false if write wasn't successful
		 */
		bool writeHeaders(void);

		/**
		 * @brief Create a Log Folder if one doesn't already exist
		 *
		 * @return true if log folder was successfully created or already exists
		 * @return false if couldn't create log folder OR log folder already exists
		 */
		bool createLogFolder(void);

		/**
		 * @brief create a folder on the sd card
		 *
		 * @param folderName the name of the folder (has to be 8.3 file format)
		 * @return true if folder was created or already exists
		 * @return false if folder couldnt be created
		 */
		bool createLogFolder(char folderName[]);

		/**
		 * @brief Create a new Datafile in the Log folder
		 *
		 * @return true if the file is successfully created
		 * @return false if the file fails to be created
		 */
		bool createLogFile(void);

		/**
		 * @brief create a new file on the SD Card
		 *
		 * @param filename name of the file
		 * @return true if the file is successfully created
		 * @return false if the file fails to be created
		 */
		bool createLogFile(char filename[]);

		/**
		 * @brief logs float data
		 *
		 * @param index of data to be logged
		 * @param data to be logged
		 */
		void logData(int index, float data);

		/**
		 * @brief logs interger data
		 *
		 * @param index of data to be logged
		 * @param data to be logged
		 */
		void logData(int index, int data);

		/**
		 * @brief logs long data
		 *
		 * @param index of data to be logged
		 * @param data to be logged
		 */
		void logData(int index, long data);

		/**
		 * @brief logs character data
		 *
		 * @param index of data to be logged
		 * @param data to be logged
		 */
		void logData(int index, char data[]);

		/**
		 * @brief logs 32bit unsigned interger data
		 *
		 * @param index of data to be logged
		 * @param data to be logged
		 */
		void logData(int index, uint32_t data);

		/**
		 * @brief logs 16 unsigned interger data
		 *
		 * @param index of data to be logged
		 * @param data to be logged
		 */
		void logData(int index, uint16_t data);

		/**
		 * @brief logs string data
		 *
		 * @param index of data to be logged
		 * @param data to be logged
		 */
		void logData(int index, String data);

		/**
		 * @brief log the run time
		 *
		 */
		void LogRuntime(void);

		/**
		 * @brief Logs SN01 Data
		 *
		 * @return true if successfully wrote data to file
		 * @return false if couldn't write to file
		 */
		bool LogSN01(void);

		/**
		 * @brief Logs SL01 Data
		 *
		 * @return true if successfully wrote data to file
		 * @return false if couldn't write to file
		 */
		bool LogSL01(void);

		/**
		 * @brief Logs SW01 Data
		 *
		 * @return true if successfully wrote data to file
		 * @return false if couldn't write to file
		 */
		bool LogSW01(void);

		/**
		 * @brief Logs SG33 Data
		 *
		 * @return true if successfully wrote data to file
		 * @return false if couldn't write to file
		 */
		bool LogSG33(void);

		/**
		 * @brief Logs PB04 Data
		 *
		 * @return true if successfully wrote data to file
		 * @return false if couldn't write to file
		 */
		bool LogPB04(void);

		/**
		 * @brief Logs SI01 Data
		 *
		 * @return true if successfully wrote data to file
		 * @return false if couldn't write to file
		 */
		bool LogSI01(void);

		/**
		 * @brief Logs RL0X Data
		 *
		 * @return true if successfully wrote data to file
		 * @return false if couldn't write to file
		 */
		bool LogRL0X(int rssi, int id);

		/**
		 * @brief Counts the number of datafiles already in the log folder.
		 *
		 * @return uint16_t The number of dataFiles already on the SD Card
		 */
		uint16_t CountDataFiles(void);

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
		 * @brief generates a new filename depending on the files already present in the log folder
		 *
		 * @return String The new filename
		 */
		String newFilename(void);
};
#endif
#endif
