/**
 * @file FlightStation.cpp
 * @support (support@xinabox.cc)
 * @brief XinaBox XK07 Firmware Library
 */

#include "FlightStationXK07.h"

#if defined(ARDUINO_SAMD_ZERO)

/**
 * @brief
 *
 */
void FlightStation::begin(void)
{
	Watchdog.enable(4000);
	Watchdog.reset();
	displayBootInfo(2500, 2500);
}

/**
 * @brief
 *
 * @param displayLogo
 * @param displayMode
 */
void FlightStation::begin(unsigned long displayLogo, unsigned long displayMode)
{
	Watchdog.reset();
	displayBootInfo(displayLogo, displayMode);
}

/**
 * @brief
 *
 */
void FlightStation::loop(void)
{
	Watchdog.reset();
	setFlightStationMode(checkFlightStation());
	if (FS_MODE == MODE_LOG)
	{
		modeLOG();
	}
	else if (FS_MODE == MODE_SAT)
	{
		modeSAT();
	}
	else
	{
		debugPrint("MODE UNKNOWN");
		debugPrint("MODE", FS_MODE);
		while (1)
		{
			global.redDebug();
		}
	}
}

/**
 * @brief
 *
 * @param _fwdisplaytime, amount of ms to show screen
 * @param _fslogodisplay, amount of ms to show screen
 */
bool FlightStation::displayBootInfo(unsigned long _fwdisplaytime, unsigned long _fslogodisplay)
{
	Watchdog.reset();
	if (xCore.ping(0x3C))
	{
		//uiXK07 ui; // create UI object
		ui.begin();
		Watchdog.reset();
		ui.setFirmwareVersion(XK07_FW_VERSION);
		Watchdog.reset();
		ui.drawXinaBoxLogo();
		Watchdog.reset();
		delay(_fwdisplaytime);
		Watchdog.reset();
		ui.drawLogoFS();
		Watchdog.reset();
		delay(_fslogodisplay);
		Watchdog.reset();
		return true;
	}
	Watchdog.reset();
	return false;
}

/**
 * @brief
 *
 * @param _mode_set_check
 */
void FlightStation::setFlightStationMode(uint8_t _mode_set_check)
{
	Watchdog.reset();
	if ((_mode_set_check >> 1) & 1)
	{
		debugPrint("MODE SAT");
		FS_MODE = MODE_SAT;
	}
	else if ((_mode_set_check >> 1) & 1)
	{
		debugPrint("MODE LOG");
		FS_MODE = MODE_LOG;
	}
}

/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t FlightStation::checkFlightStation(void)
{
	Watchdog.reset();
	xchipSensorStatus = checkFlightStationChips();
	decodeFlightStationChips(xchipSensorStatus);
	return xchipSensorStatus;
}

/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t FlightStation::checkFlightStationChips(void)
{
	devXK07 dev;
	return dev.CheckXChips();
}

/**
 * @brief
 *
 * @param _xchipData
 */
void FlightStation::decodeFlightStationChips(uint8_t _xchipData)
{
	Watchdog.reset();
	if ((~_xchipData >> 0) & 1)
	{
		errorHandler(0x10);
		//return 1;
	}
	if ((~_xchipData >> 1) & 1)
	{
		errorHandler(0x11);
		//return 0;
	}
	if ((~_xchipData >> 2) & 1)
	{
		errorHandler(0x12);
		//return 1;
	}
	if ((~_xchipData >> 3) & 1)
	{
		errorHandler(0x13);
		//return 1;
	}
	if ((~_xchipData >> 4) & 1)
	{
		errorHandler(0x14);
		//return 1;
	}
	if ((~_xchipData >> 5) & 1)
	{
		errorHandler(0x15);
		//return 1;
	}
	if ((~_xchipData >> 6) & 1)
	{
		errorHandler(0x16);
		//return 1;
	}
	if ((~_xchipData >> 7) & 1)
	{
		errorHandler(0x17);
		//return 1;
	}
	//return 1;
}

/**
 * @brief
 *
 * @param _errorCode
 */
bool FlightStation::errorHandler(uint8_t _errorCode)
{
	Watchdog.reset();
	switch (_errorCode)
	{
	case 0x10: // NO OD01
		debugPrint("ERROR 0x10> NO OD01");
		return true;
	case 0x11: // NO RL0x
		debugPrint("ERROR 0x11> NO RL0x");
		return true;
	case 0x12: // NO SI01
		debugPrint("ERROR 0x12> NO SI01");
		return true;
	case 0x13: // NO PB04
		debugPrint("ERROR 0x13> NO PB04");
		return true;
	case 0x14: // NO SG33
		debugPrint("ERROR 0x14> NO SG33");
		return true;
	case 0x15: // NO SW01
		debugPrint("ERROR 0x15> NO SW01");
		return true;
	case 0x16: // NO SL01
		debugPrint("ERROR 0x16> NO SL01");
		return true;
	case 0x17: // NO SN01
		debugPrint("ERROR 0x17> NO SN01");
		return true;
	case 0x20: // SD Card Failed to Load
		debugPrint("ERROR 0x20> SD init Failed");
		return true;
	case 0x21: // SD Card Failed to create folder
		debugPrint("ERROR 0x21> SD Failed - Failed to create folder");
		return true;
	case 0x22: // SD Card Failed to ccreate file
		debugPrint("ERROR 0x22> SD Failed - Failed to ccreate file");
		return true;
	case 0x23: // SD Card Failed to write file headers
		debugPrint("ERROR 0x23> SD Failed - Failed to write file headers");
		return true;
	case 0x30: // Radio Setup Fail
		debugPrint("ERROR 0x30> Radio Setup Fail");
		return true;
	case 0x31: // Radio Almanac Invalid
		debugPrint("ERROR 0x31> Radio Almanac Fail");
		return true;
	case 0x32: // Raiod Packet Invalid
		debugPrint("ERROR 0x32> Radio Packet recv fail");
		return true;
	case 0x33: // Radio ??
		debugPrint("ERROR 0x33> Error");
		return true;
	case 0xFF:
		debugPrint("RUN_OK");
		return false;
	default:
		return true;
	}
}

/**
 * @brief
 *
 */
void FlightStation::modeLOG(void)
{
	Watchdog.reset();
	errorHandler(log.begin());
	log.enableExcelLogOutput(true);

	while (1)
	{
		Watchdog.reset();
		log.logAllSensors(com.getLastRSSI(),_radioData.RADIO_ID);
	}
}

/**
 * @brief
 *
 */
void FlightStation::modeSAT(void)
{
	Watchdog.reset();
	if (!errorHandler(com.begin()))
	{
		receiveAlmanac();
		// if (alma != "")
		// {
		// 	int len = strlen(alma);
		// 	//SerialUSB.println("length of almanac" + String(len));
		// 	if (xCore.ping(0x42))
		// 	{
		// 		for (int i = 0; i < len; i++)
		// 		{
		// 			Wire.beginTransmission(0x42);
		// 			Wire.write((uint8_t)alma[i]);
		// 			if (i == (len - 1))
		// 				Wire.endTransmission(true);
		// 			else
		// 				Wire.endTransmission(false);
		// 		}
		// 	}
		// }
		SerialUSB.println("");
		errorHandler(log.begin());
		//log.enableExcelLogOutput(true);
		satLoop();
	}
	else
	{
		debugPrint("FS boot halted Radio cannot initialize");
		errorHandler(log.begin());
		log.enableExcelLogOutput(true);
		while (1)
		{
			Watchdog.reset();
			digitalWrite(LED_RED, HIGH);
			delay(1000);
			digitalWrite(LED_RED, LOW);
			delay(1000);
			log.logAllSensors(com.getLastRSSI(), _radioData.RADIO_ID);
		}
	}
}

/**
 * @brief
 *
 */
void FlightStation::satLoop(void)
{
	Watchdog.reset();
	unsigned long timLog = 0, timPayloadA = 0, timPayloadB = 0, timPayloadC = 0;
	unsigned long timMovement = 0;
	timLog = timPayloadA = timPayloadB, timPayloadC = timMovement = millis();
	devXK07 dev;
	dev.initSensors();
	while (1)
	{
		dev.uifs();
		if (com.receivePacket())
		{
			Watchdog.reset();
			if (com.dataPacket())
			{
				Watchdog.reset();
				uint8_t _cmd = parseDataPacket(com.getPacket());
				if (_cmd == 0x70)
				{
					syncReady = 0;
					syncGS();
				}
				else if (_cmd == 0x73)
				{
					Watchdog.reset();
					debugPrint("getting info");
					getFlightInfo();
				}
			}
			debugPrint((char *)com.getPacket());
		}

		if ((timLog - millis()) > 5000)
		{
			Watchdog.reset();
			log.logAllSensors(com.getLastRSSI(),_radioData.RADIO_ID);
			timLog = millis();
		}

		if ((timPayloadA - millis()) > 1000)
		{
			Watchdog.reset();
			debugPrint("send payloadA");
			com.sendPacket(dev.payloadA(com.getLastRSSI()), dev.getPacketSize());
			if (com.receivePacket())
			{
				Watchdog.reset();
				debugPrint((char *)com.getPacket());
			}
			timPayloadA = millis();
		}

		if ((timPayloadB - millis()) > 5000)
		{
			global.watchdog();
			// TODO: add payloadA code to send to GS
			debugPrint("send payloadB");
			com.sendPacket(dev.payloadB(com.getLastRSSI()), dev.getPacketSize());
			if (com.receivePacket())
			{
				// TODO: add function to process FS reboot
				debugPrint((char *)com.getPacket());
			}
			global.watchdog();
			timPayloadB = millis();
		}

		// if ((timPayloadC - millis()) > 30000)
		// {
		// 	Watchdog.reset();
		// 	// TODO: add payloadA code to send to GS
		// 	timPayloadC = millis();
		// }

		if ((timMovement - millis()) > 20000) {
			Watchdog.reset();
			dev.testMovement();
			timMovement = millis();
		}

	}
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool FlightStation::syncGS(void)
{
	Watchdog.reset();
	unsigned long tim1 = millis();
	uint8_t tim2 = 0;

	while ((syncReady < 3) && (tim2 < 30))
	{
		if ((millis() - tim1) > 1000)
		{
			tim2++;
			tim1 = millis();
			debugPrint("sync waiting");
		}
		if (com.waitSync())
		{
			syncReady++;
			debugPrint("sync > ", syncReady);
		}
	}
	if (tim2 == 30)
	{
		debugPrint("sync failed > timeout 120s");
		return false;
	}
	if (syncReady == 3)
	{
		debugPrint("sync completed");
		return true;
	}
	return false;
}

/**
 * @brief
 *
 * @return uint8_t
 */
bool FlightStation::getFlightInfo(void)
{
	Watchdog.reset();
	bool _setupCompleted = false, _almStart = true, _almEnd = false, _sync = false;
	com.sendPacket(ConfigSequence, sizeof(ConfigSequence));
	while (!_setupCompleted)
	{
		if (com.receivePacket())
		{
			uint8_t cmd = decodePacket(com.getPacket(), com.getPacketSize());
			if (cmd == 0x50)
			{
				debugPrint("sync done");
				_sync = true;
			}
			else if (cmd == 0x51)
			{
				debugPrint("alm done");
				_almStart = true;
			}
			if (_sync && _almStart)
			{
				_setupCompleted = true;
			}
			else
			{
				//com.sendPacket(ConfigSequence, sizeof(ConfigSequence));
				if (!_sync)
				{
					debugPrint("waiting for sync");
				}
				if (!_almStart)
				{
					debugPrint("waiting for almStart");
				}
			}
		}
		Serial.println("looping");
	}
	debugPrint("sat is now configured");
	return _setupCompleted;
}

/**
 * @brief
 *
 * @param _recvdata
 * @param _sizeData
 * @return uint8_t
 */
uint8_t FlightStation::decodePacket(uint8_t _recvdata[], uint8_t _sizeData)
{
	Watchdog.reset();
	char *parseptr;
	if (strncmp((char *)_recvdata, (char *)SyncSequence, 5) == 0)
	{
		syncReady = 0;
		syncDone = syncGS();
		return 0x50;
	}
	else if (strncmp((char *)_recvdata, (char *)StartSequence, 6) == 0)
	{
		com.sendPacket(StartSequence, sizeof(StartSequence));
		//com.receiveAlmanac();
		com.sendPacket(EndSequence, sizeof(EndSequence));
		return 0x51;
	}
	else if (strncmp((char *)_recvdata, (char *)EndSequence, 4) == 0)
	{
		com.sendPacket(EndSequence, sizeof(EndSequence));
		return 0x52;
	}
	else if (strncmp((char *)_recvdata, (char *)AckSequence, 4) == 0)
	{
		com.sendPacket(AckSequence, sizeof(AckSequence));
		return 0x53;
	}
	else if (strncmp((char *)_recvdata, (char *)ConfigSequence, 5) == 0)
	{
		com.sendPacket(ConfigSequence, sizeof(ConfigSequence));
		return 0x54;
	}
	else
	{
		debugPrint("got nothing");
		return 0x55;
	}
	return 0x55;
}

/**
 * @brief
 *
 * @param _packetRadio
 * @return true
 * @return false
 */
uint8_t FlightStation::parseDataPacket(uint8_t _packetData[])
{
	Watchdog.reset();
	if (strncmp((char *)_packetData, "$SYNC", 5) == 0)
	{
		return 0x70;
	}
	else if (strncmp((char *)_packetData, "$START", 6) == 0)
	{
		return 0x71;
	}
	else if (strncmp((char *)_packetData, "$END", 4) == 0)
	{
		return 0x72;
	}
	else if (strncmp((char *)_packetData, "$CONF", 5) == 0)
	{
		return 0x73;
	}
}

/**
 * @brief
 *
 * @param _data
 */
void FlightStation::debugPrint(String _data)
{
	Watchdog.reset();
#ifdef DEBUG_SERIAL
	SerialUSB.print("fs_xk07: ");
	SerialUSB.println(_data);
#endif
}

/**
 * @brief
 *
 * @param _data
 * @param _num
 */
void FlightStation::debugPrint(String _data, uint16_t _num)
{
	Watchdog.reset();
#ifdef DEBUG_SERIAL
	SerialUSB.print("fs_xk07: ");
	SerialUSB.print(_data);
	SerialUSB.println(_num);
#endif
}

#endif