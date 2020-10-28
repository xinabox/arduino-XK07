/**
 * @file GroundStationXK07.cpp
 * @support (support@xinabox.cc)
 * @brief
 */
#include "GroundStationXK07.h"

#if defined(ARDUINO_ARCH_ESP8266) // Include guard otherwise cpp files get compiled first and cause compilations errors

/**
 * @brief
 *
 */
bool GroundStation::begin(void)
{
	return begin(2500, 2500);
}

/**
 * @brief
 *
 * @param displayLogo
 * @param displayMode
 */
bool GroundStation::begin(unsigned long displayLogo, unsigned long displayMode)
{
	//com.parseSelectedRadio("RL01");
	ui.begin();
	errorHandler(startProvisionRoutine());
	displayBootInfo(displayLogo, displayMode);
	setGroundStationMode(checkGroundStation());
	return setupGroundStation();
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool GroundStation::setupGroundStation(void) // TODO: add error handler stuff
{
	app.setFirmwareVersion(fwBeta);
	app.setDeviceName(_deviceName);
	app.setMqttClientName();
	app.setMqttIndexName();

	if (!app.connectWiFi(_wifiSSID.c_str(), _wifiPSK.c_str()))
	{
		ui.wifiFail(" WIFI FAIL");
		global.redDebug();
	}
	ui.wifiPass(" WIFI PASS");

	if (!app.checkInternet())
	{
		ui.wifiFail(" INET FAIL");
		global.redDebug();
	}
	ui.wifiPass(" INET PASS");

	if (!app.connectMQTT())
	{
		ui.wifiFail(" MQTT FAIL");
		global.redDebug();
	}

	ui.wifiPass(" MQTT PASS");

	if (errorHandler(com.begin()))
	{
		debugPrint("GS boot halted Radio (RL0x) cannot initialize");
		global.redDebug();
	}

	com.resetFlightStation();

	if (!app.downloadAlmanac())
	{
		almaDown = true;
		ui.almDownloadedFail(" ALMA FAIL");
		global.blueDebug();
	}
	else
	{
		ui.almDownloaded("  Almanac Downloaded");
		debugPrint("almanac download complete");
		if (com.sendAlmanac(app.getAlmanac(), app.getAlmanacSize()))
		{
			ui.almDownloaded("     Almanac Sent");
		}
		else
		{
			ui.almDownloadedFail("  Almanac Send Fail");
			global.blueDebug();
		}
	}
	return true;
}

bool GroundStation::configSAT(void)
{
	if (com.sendConfigSequence())
	{
		if (syncFS())
		{
			if (almaDown)
			{
				if (com.sendStartSequence())
				{
					if (com.receivePacket())
					{
						com.sendPacket(StartSequence, sizeof(StartSequence));
						com.sendPacket(StartSequence, sizeof(StartSequence));
						//com.sendAlmanac(app.getAlmanac(), app.getAlmanacSize());
						com.sendPacket(EndSequence, sizeof(EndSequence));
						return true; // FS should be configured now
					}
					return false;
				}
				return false;
			}
			return false;
		}
		return false;
	}
	return false;
}

/**
 * @brief
 *
 */
void GroundStation::loop(void)
{
	if (GS_MODE == MODE_FAULT)
	{
		modeFAULT();
	}
	else if (GS_MODE == MODE_GS)
	{
		modeGS();
	}
	else
	{
		debugPrint("MODE UNKNOWN");
		while (1)
		{
			delay(100);
			yield();
		}
	}
}

/**
 * @brief
 *
 */
void GroundStation::modeGS(void)
{
	configSAT();
	gsLoop();
}

/**
 * @brief
 *
 */
void GroundStation::modeFAULT(void)
{
	digitalWrite(LED_BUILTIN, HIGH);
	while (1)
	{
		digitalWrite(LED_RED, HIGH);
		delay(100);
		digitalWrite(LED_RED, LOW);
		delay(100);
		yield();
	}
}

/**
 * @brief Main GS loop after config has been completed
 *
 */
void GroundStation::gsLoop(void)
{
	devXK07 dev;
	unsigned long timNoDataPublish = 0;
	while (1)
	{
		if (com.receivePacket())
		{
			if (com.dataPacket())
			{
				uint8_t _cmd = app.parseDataPacket(com.getPacket());
				if (_cmd == 0x70)
				{
					syncFS();
				}
				else if (_cmd == 0x73)
				{
					configSAT();
				}
			}
			else
			{
				if (app.parseRadioBuffer(com.getPacket()))
				{
					com.sendPacket(AckSequence, sizeof(AckSequence));
					if (app.connectMQTT())
					{
						app.publishMQTT(com.getLastRSSI());
					}
				}
			}
		}
		else
		{
			if ((millis() - timNoDataPublish) > 10000)
			{
				if (app.connectMQTT())
				{
					app.publishMQTT();
				}
				timNoDataPublish = millis();
			}
			yield();
		}

		if ((millis() - timNoDataPublish) > 5000)
		{
			if (com.cntTest > 30)
			{
				ui.fsConnecting("   Connecting to FS");
			}
			else
			{
				app.uigs();
			}
			timNoDataPublish = millis();
		}
		app.ownTracks();
	}
}

/**
 * @brief
 *
 * @param _recvdata
 * @param _sizeData
 * @return uint8_t
 */
uint8_t GroundStation::decodePacket(uint8_t _recvdata[], uint8_t _sizeData)
{
	//debugPrint("decodePacket");
	//debugPrint((char*)_recvdata);

	if (strncmp((char *)_recvdata, (char *)SyncSequence, 5) == 0)
	{
		syncFS();
		return 0x50;
	}
	else if (strncmp((char *)_recvdata, (char *)StartSequence, 6) == 0)
	{
		debugPrint("get alamanc");
		com.sendPacket(AckSequence, sizeof(AckSequence));
		//com.receiveAlmanac();
		return 0x51;
	}
	else if (strncmp((char *)_recvdata, (char *)EndSequence, 4) == 0)
	{
		//debugPrint("got end");
		com.sendPacket(EndSequence, sizeof(EndSequence));
		return 0x52;
	}
	else if (strncmp((char *)_recvdata, (char *)AckSequence, 4) == 0)
	{
		//debugPrint("got ack");
		com.sendPacket(AckSequence, sizeof(AckSequence));
		return 0x53;
	}
	else if (strncmp((char *)_recvdata, (char *)NackSequence, 4) == 0)
	{
		//debugPrint("got nack");
		com.sendPacket(NackSequence, sizeof(NackSequence));
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
 * @param _mode_set_check
 */
void GroundStation::setGroundStationMode(uint8_t _mode_set_check)
{
	if ((_mode_set_check >> 1) & 1)
	{
		debugPrint("MODE GS");
		GS_MODE = MODE_GS;
	}
	else
	{
		debugPrint("MODE FAULT");
		GS_MODE = MODE_FAULT;
	}
}

/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t GroundStation::checkGroundStation(void)
{
	xchipSensorStatus = checkGroundStationChips();
	decodeGroundStationChips(xchipSensorStatus);
	return xchipSensorStatus;
}

/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t GroundStation::checkGroundStationChips(void)
{
	devXK07 dev;
	return dev.CheckXChips();
}

/**
 * @brief
 *
 * @param _fwdisplaytime, amount of ms to show screen
 * @param _fslogodisplay, amount of ms to show screen
 */
bool GroundStation::displayBootInfo(unsigned long _fwdisplaytime, unsigned long _fslogodisplay)
{
	if (xCore.ping(0x3C))
	{

		//ui.begin();
		// while (1)
		// {
		// 	app.uigs();
		// 	delay(5000);
		// }
		ui.setFirmwareVersion(XK07_FW_VERSION); // TODO: update when new version
		ui.drawXinaBoxLogo();
		delay(_fwdisplaytime);
		ui.drawLogoGS();
		delay(_fslogodisplay);
		return true;
	}
	return false;
}

/**
 * @brief
 *
 * @param _xchipData
 */
void GroundStation::decodeGroundStationChips(uint8_t _xchipData)
{
	if ((~_xchipData >> 0) & 1)
	{
		errorHandler(0x10);
	}
	if ((~_xchipData >> 1) & 1)
	{
		errorHandler(0x11);
	}
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool GroundStation::syncFS(void)
{
	unsigned long tim1 = millis();
	uint8_t tim2 = 0;

	while ((syncReady < 3) && (tim2 < 30))
	{
		if ((millis() - tim1) > 1000)
		{
			tim2++;
			tim1 = millis();
		}
		if (com.waitSync())
		{
			syncReady++;
			debugPrint("sync > ", syncReady);
		}
	}

	if (tim2 == 30)
	{
		debugPrint("timeout occurred 30s");
		//return false;
		return true;
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
 * @return true
 * @return false
 */
bool GroundStation::sendAlmanacFS(uint8_t *_almData, int _almSize)
{
	debugPrint("alamanac size :", (uint16_t)_almSize);
	com.sendAlmanac(_almData, _almSize);
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
uint8_t GroundStation::startProvisionRoutine(void)
{
	provXK07 prov = provXK07();

	if (prov.setupProvisioning())
	{
		if (prov.getData())
		{
			_deviceName = prov.getUnitName();
			_wifiSSID = prov.getSSID();
			_wifiPSK = prov.getPassword();
			_radio = prov.getRadio();
			return 0xFF;
		}
		debugPrint("Passed_Provision");
		return 0x91;
	}
	debugPrint("Passed_Fail");
	return 0x90;
}

/**
 * @brief
 *
 * @param _errorCode
 */
bool GroundStation::errorHandler(uint8_t _errorCode)
{
	//uiXK07 ui;
	switch (_errorCode)
	{
	case 0x10: // NO OD01
		debugPrint("ERROR 0x10> NO OD01");
		return true;
	case 0x11: // NO RL0x
		debugPrint("ERROR 0x11> NO RL0x");
		return true;
	case 0x20: //
		debugPrint("ERROR 0x20> ");
		return true;
	case 0x21: //
		debugPrint("ERROR 0x21> ");
		return true;
	case 0x22: //
		debugPrint("ERROR 0x22> ");
		return true;
	case 0x23: //
		debugPrint("ERROR 0x23> ");
		return true;
	case 0x24: //
		debugPrint("ERROR 0x24> ");
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
	case 0x90: //
		debugPrint("ERROR 0x90> startProvisionRoutine() provisioning failed");
		ui.provFail(" PROV FAIL");
		global.redDebug();
		return true;
	case 0x91: //
		debugPrint("ERROR 0x91> startProvisionRoutine() provision data invalid");
		ui.provFail(" PROV FAIL");
		global.redDebug();
		return true;
	case RUN_OK:
		return false;
	default:
		return true;
	}
}

/**
 * @brief
 *
 * @param _data
 */
void GroundStation::debugPrint(String _data)
{
#ifdef DEBUG_SERIAL
	Serial.print("gs_xk07: " + _data + "\n\r");
	//Serial.println(_data);
#endif
}

/**
 * @brief
 *
 * @param _data
 * @param _num
 */
void GroundStation::debugPrint(String _data, uint16_t _num)
{
#ifdef DEBUG_SERIAL
	Serial.print("gs_xk07: ");
	Serial.print(_data);
	Serial.println(_num);
#endif
}

#endif