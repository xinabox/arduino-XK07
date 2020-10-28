/**
 * @file appXK07.cpp
 * @support (support@xinabox.cc)
 * @brief Class for handling WiFi on XK07 Ground Station
 */

#include "appXK07.h"

#if defined(ARDUINO_ARCH_ESP8266) // Include guard otherwise cpp files get compiled first and cause compilations errors

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool appXK07::connectWiFi(void)
{
	uint8_t count_1s = 0;
	unsigned long timerMillis = millis();
	if (WiFi.status() != WL_CONNECTED)
	{
		digitalWrite(LED_GREEN, LOW);
		while ((wifiMulti.run() != WL_CONNECTED) && (count_1s < 5))
		{
			if (1000 > (millis() - timerMillis))
			{
				timerMillis = millis();
				count_1s++;
			}
			digitalWrite(LED_RED, HIGH);
			delay(100);
			digitalWrite(LED_RED, LOW);
			delay(100);
		}

		if (timerMillis == 5)
		{
			//TODO Timeout Occured
		}
	}
	else if (WiFi.status() == WL_CONNECTED)
	{
		digitalWrite(LED_GREEN, HIGH);
		return true;
	}
	return false;
}

/**
 * @brief
 *
 * @param ssid
 * @param psk
 * @return true
 * @return false
 */
bool appXK07::connectWiFi(const char *ssid, const char *psk)
{
	//uiXK07 ui; // create ui object

	uint8_t connectCount1s = 0;
	unsigned long counterTimer = millis();
	WiFi.mode(WIFI_STA);
	wifiMulti.addAP("XinaBox", "RapidIoT");
	//wifiMulti.addAP("Homer69", "nopassword");
	wifiMulti.addAP(ssid, psk);
	// attempt to connect WiFi Hotspots and timeout after 30 seconds
	uint32_t period = 1 * 30000L;
	for (uint32_t tStart = millis(); (millis() - tStart) < period;)
	{
		ui.connectingToWifi("  Connecting to WiFi");
		if (wifiMulti.run() == WL_CONNECTED)
		{
			debugPrint("Connected");
			return true;
		}
	}
	return false;
	// while ((wifiMulti.run() != WL_CONNECTED) && (connectCount1s < 30))
	// {
	// 	if (1000 < (millis() - counterTimer))
	// 	{
	// 		//Serial.print(".");
	// 		connectCount1s++;
	// 		counterTimer = millis();
	// 	}
	// 	//digitalWrite(LED_RED, HIGH);
	// 	ui.drawConnectingWiFi(connectCount1s);
	// 	//digitalWrite(LED_RED, LOW);
	// 	ui.drawConnectingWiFi(connectCount1s);
	// }

	// if (WiFi.status() == WL_CONNECTED)
	// {
	// 	ui.wifiPass(" WiFi PASS");
	// 	debugPrint("Connected");
	// 	digitalWrite(LED_GREEN, HIGH);
	// 	return true;
	// }
	// return false;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool appXK07::checkInternet(void)
{
	//uiXK07 ui;
	ubloxData uData;
	WiFiClient client = WiFiClient();
	uint32_t period = 1 * 10000L;
	for (uint32_t tStart = millis(); (millis() - tStart) < period;)
	{
		ui.connectingToWifi("   Testing Internet");
		if (client.connect(uData.google, uData.googlePort))
		{
			return true;
		}
	}
	return false;
	// ubloxData uData;
	// if (client.connect(uData.google, uData.googlePort))
	// {
	// 	return true;
	// }
	// else
	// {
	// 	return true;
	// }
}

/**
 * @brief
 *
 * @return int32_t
 */
int32_t appXK07::getWifiSignalStrength(void)
{
	return WiFi.RSSI();
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool appXK07::connectMQTT(void)
{
	mdata.cw01ClientName = ESP.getChipId();
	if (connectMQTT(mdata.xinaboxMqttServerPort1))
	{
		if (!fmqtt)
		{
			fmqtt = true;
		}
		return true;
	}
	else if (connectMQTT(mdata.xinaboxMqttServerPort2))
	{
		if (!fmqtt)
		{
			fmqtt = true;
		}
		return true;
	}
	return false;
}

/**
 * @brief
 *
 * @param _mqttPort
 * @return true
 * @return false
 */
bool appXK07::connectMQTT(uint16_t _mqttPort)
{
	uint8_t tick_MQTT = 0;
	uint8_t setupComplete = 0;
	//uiXK07 ui;
	if (connectWiFi())
	{
		setMqttServerConnectionSetting(_mqttPort);

		if (!mqtt.connected())
		{
			errCount.count_MQTT++;
			debugPrint("connecting to XinaBox MQTT Server...PORT:", _mqttPort);

			// Connect to MQTT Server
			// Try to reconnect 5 Times
			while (!mqtt.connected() && (tick_MQTT < 5))
			{
				ui.connectingToWifi("  Connecting to MQTT");
				tick_MQTT++;
				if (mqtt.connect(mdata.cw01ClientName.c_str(), mdata.xinaboxMqttUsername.c_str(), mdata.xinaboxMqttPassword.c_str()))
				{
					if (mqtt.connected())
					{
						debugPrint("connection succesful");
						setupComplete = 1;
						errCode.MQTT_CONN_E = false;
						mqtt.loop();
					}
				}
			}
			// Check to make sure we connected
			if (!mqtt.connected())
			{
				ui.wifiFail(" MQTT FAIL");
				errCount.count_MQTT = 0;
				debugPrint("connection failed");
				tick_MQTT = 0;
				return false;
			}
			else
			{
				ui.wifiPass(" MQTT PASS");
				return true;
			}

			if (tick_MQTT > 4)
			{
				ui.wifiFail(" MQTT FAIL");
				errCount.count_MQTT = 0;
				debugPrint("connection timeout");
				errCode.MQTT_CONN_E = errCode.ERROR_OCCURED = true;
				return false;
			}
		}
		else
		{
			errCode.MQTT_CONN_E = false;
			return true;
		}
	}
	else
	{
		errCode.WIFI_CONN = errCode.ERROR_OCCURED = true;
		return false;
	}
}

/**
 * @brief
 *
 * @param _mqttHostName
 * @param _mqttPort
 */
void appXK07::setMqttServerConnectionSetting(const char *_mqttHostName, uint16_t _mqttPort)
{
	mqtt.setServer(_mqttHostName, _mqttPort);
}

/**
 * @brief
 *
 * @param _mqttPort
 */
void appXK07::setMqttServerConnectionSetting(uint16_t _mqttPort)
{
	mqtt.setServer(mdata.xinaboxMqttServerHostname, _mqttPort);
}

/**
 * @brief
 *
 * @param _lastRSSI
 */
void appXK07::publishMQTT(int16_t _lastRSSI)
{
	info.rssi = _lastRSSI;
	const size_t capacity = JSON_ARRAY_SIZE(2) + 4 * JSON_OBJECT_SIZE(1) + 4 * JSON_OBJECT_SIZE(2) + 4 * JSON_OBJECT_SIZE(3) + 2 * JSON_OBJECT_SIZE(4) + 2 * JSON_OBJECT_SIZE(9);
	DynamicJsonDocument doc(capacity);

	JsonObject GS = doc.createNestedObject("GS");

	JsonObject GS_CW01 = GS.createNestedObject("CW01");
	GS_CW01["RSSI(dB)"] = WiFi.RSSI();
	GS_CW01["Firmware"] = _firmwareVersion;
	GS_CW01["MAC"] = WiFi.macAddress();
	GS_CW01["Chip ID"] = ESP.getChipId();

	JsonObject GS_Input = GS.createNestedObject("Input");
	GS_Input["Unit Name"] = _deviceName;

	if (getTimeStamp())
	{
		getTimeStamp();
		JsonObject GS_Timestamp = GS.createNestedObject("Timestamp");
		GS_Timestamp["Created"] = _timeStamp;
	}

	JsonObject SAT = doc.createNestedObject("SAT");
	JsonObject SAT_Connection = SAT.createNestedObject("Connection");
	SAT_Connection["Status"] = 1;
	if (getTimeStamp())
	{
		SAT_Connection["Last Seen"] = _timeStamp;
	}

	if (sdata.SAT_SW01)
	{
		JsonObject SAT_SW01 = SAT.createNestedObject("SW01");
		SAT_SW01["Temperature(C)"] = atof(sdata.SW01_T.c_str());
		SAT_SW01["Humidity(%)"] = atoi(sdata.SW01_H.c_str());
		SAT_SW01["Pressure(hPa)"] = atof(sdata.SW01_P.c_str()) / 100;
		SAT_SW01["QNE(m)"] = atoi(sdata.SW01_A.c_str());
		sdata.SAT_SW01 = false;
	}
	if (sdata.SAT_SL01)
	{
		JsonObject SAT_SL01 = SAT.createNestedObject("SL01");
		SAT_SL01["Lux"] = atoi(sdata.SL01_L.c_str());
		if ((atoi(sdata.SL01_A.c_str()) != 255) || (atoi(sdata.SL01_B.c_str()) != 255))
		{
			SAT_SL01["UVA(mW/m^2)"] = atoi(sdata.SL01_A.c_str());
			SAT_SL01["UVB(mW/m^2)"] = atoi(sdata.SL01_B.c_str());
		}
		SAT_SL01["UVI"] = atof(sdata.SL01_I.c_str());
		sdata.SAT_SL01 = false;
	}

	if (sdata.SAT_PB04)
	{
		JsonObject SAT_PB04 = SAT.createNestedObject("PB04");
		SAT_PB04["Current(mA)"] = atof(sdata.PB04_C.c_str());
		;
		SAT_PB04["Voltage(V)"] = atof(sdata.PB04_V.c_str());
		SAT_PB04["Power Level(%)"] = (uint8_t)((atof(sdata.PB04_V.c_str()) / 3.3) * 100);
		sdata.SAT_PB04 = false;
	}
	if (sdata.SAT_SI01)
	{
		JsonObject SAT_SI01 = SAT.createNestedObject("SI01");
		SAT_SI01["G-Force"] = atof(sdata.SI01_GF.c_str());
		SAT_SI01["Roll(Deg)"] = atof(sdata.SI01_R.c_str());
		SAT_SI01["Pitch(Deg)"] = atof(sdata.SI01_P.c_str());
		sdata.SAT_SI01 = false;
	}
	if (sdata.SAT_SG33)
	{
		JsonObject SAT_SG33 = SAT.createNestedObject("SG33");
		SAT_SG33["TVOC(ppb)"] = atoi(sdata.SG33_TVOC.c_str());
		SAT_SG33["eCO2(ppm)"] = atoi(sdata.SG33_CO2.c_str());
		sdata.SAT_SG33 = false;
	}
	if (sdata.SAT_SN01)
	{
		JsonObject SAT_SN01 = SAT.createNestedObject("SN01");
		SAT_SN01["HDOP"] = atof(sdata.SN01_HDOP.c_str());
		SAT_SN01["Number Satellites"] = atoi(sdata.SN01_SAT.c_str());
		SAT_SN01["STATUS"] = "ONLINE";
		JsonObject SAT_SN01_Timestamp = SAT_SN01.createNestedObject("Timestamp");
		//SAT_SN01_Timestamp["Created"] = _timeStamp; // TODO: update to SN01 timestamp
		SAT_SN01_Timestamp["Created"] = sdata.SN01_D + "T" + sdata.SN01_T; // TODO: update to SN01 timestamp

		if ((atof(sdata.SN01_HDOP.c_str()) < 10) && (atof(sdata.SN01_HDOP.c_str()) > 0))
		{
			JsonArray SAT_SN01_Location = SAT_SN01.createNestedArray("Location");
			SAT_SN01_Location.add(atof(sdata.SN01_LONG.c_str()));
			SAT_SN01_Location.add(atof(sdata.SN01_LAT.c_str()));
			SAT_SN01["Altitude(m)"] = atof(sdata.SN01_ALT.c_str());
			SAT_SN01["SOG(m/s)"] = atof(sdata.SN01_SOG.c_str());
			if (atof(sdata.SN01_COG.c_str()) > 360)
				SAT_SN01["COG(Deg)"] = 360;
			else
				SAT_SN01["COG(Deg)"] = atof(sdata.SN01_COG.c_str());
			SAT_SN01["Ascend Rate(m/s)"] = atof(sdata.SN01_SAT.c_str());
		}
		sdata.SAT_SN01 = false;
	}

	JsonObject SAT_RL03 = SAT.createNestedObject("RL03");
	SAT_RL03["GS RSSI(dB)"] = _lastRSSI;
	SAT_RL03["GS ID"] = 0x11;
	SAT_RL03["Satellite RSSI(dB)"] = atof(sdata.SAT_RSSI.c_str());
	SAT_RL03["SAT ID"] = 0x11;

	if (getTimeStamp())
	{
		getTimeStamp();
		JsonObject SAT_Timestamp = SAT.createNestedObject("Timestamp");
		SAT_Timestamp["Created"] = _timeStamp;
	}

	char mqttPayload[capacity];

	if (serializeJson(doc, mqttPayload) == 0)
	{
		debugPrint("Failed to write to buffer");
	}

	unsigned int lenSAT = strlen(mqttPayload);

	if (mqtt.publish(mdata.indexName, mqttPayload, lenSAT))
	{
		serializeJsonPretty(doc, Serial);
		Serial.println();
	}
	//mqtt.publish("esp32/humidity", humString);
}

/**
 * @brief
 *
 */
void appXK07::publishMQTT(void)
{
	const size_t capacity = 3 * JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4);
	DynamicJsonDocument doc(capacity);

	JsonObject GS = doc.createNestedObject("GS");

	JsonObject GS_CW01 = GS.createNestedObject("CW01");
	GS_CW01["RSSI(dB)"] = WiFi.RSSI();
	GS_CW01["Firmware"] = _firmwareVersion;
	GS_CW01["MAC"] = WiFi.macAddress();
	GS_CW01["Chip ID"] = ESP.getChipId();

	JsonObject GS_Input = GS.createNestedObject("Input");
	GS_Input["Unit Name"] = _deviceName;

	if (getTimeStamp())
	{
		getTimeStamp();
		JsonObject GS_Timestamp = GS.createNestedObject("Timestamp");
		GS_Timestamp["Created"] = _timeStamp;
	}

	JsonObject SAT = doc.createNestedObject("SAT");
	JsonObject SAT_Connection = SAT.createNestedObject("Connection");
	SAT_Connection["Status"] = 0;
	SAT_Connection["Last Seen"] = _timeStamp;

	char mqttPayload[capacity];

	if (serializeJson(doc, mqttPayload) == 0)
	{
		debugPrint("Failed to write to buffer");
	}

	unsigned int lenSAT = strlen(mqttPayload);

	if (mqtt.publish(mdata.indexName, mqttPayload, lenSAT))
	{
		serializeJsonPretty(doc, Serial);
		Serial.println();
	}
	//mqtt.publish("esp32/humidity", humString);
}

/**
 * @brief
 *
 * @return uint8_t
 */
bool appXK07::parseRadioBuffer(uint8_t _packetRadio[])
{
	if (_packetRadio[0] != '$')
	{
		Serial1.println(_packetRadio[0]);
		return false;
	}
	bool data_check;
	char data = 0;
	for (uint8_t i = 0; i <= MAX_PACKET_SIZE; i++)
	{
		data = _packetRadio[i];
		if (data == 0x98)
		{
			i = 151;
			data_check = false;
		}
		if (data == '$')
		{
			sdata.bufferidx = 0;
			sdata.buffer[sdata.bufferidx++] = data;
			delay(1);
			continue;
		}
		if (data == '*')
		{
			sdata.buffer[sdata.bufferidx++] = 0;
			delay(1);
			parsePacketSnippet(sdata.buffer);
			data_check = true;
		}
		else
		{
			if (sdata.bufferidx < 150)
			{
				sdata.buffer[sdata.bufferidx++] = data;
				delay(1);
			}
			else
			{
				sdata.bufferidx = 0;
			}
		}
	}
	return data_check;
}

/**
 * @brief
 *
 * @param _packetRadio
 * @return true
 * @return false
 */
uint8_t appXK07::parseDataPacket(uint8_t _packetData[])
{
	if (strncmp((char *)_packetData, "$SYNC", 5) == 0)
	{
		return 0x70;
	}
	else if (strncmp((char *)_packetData, "$START", 6) == 0)
	{
		return 0x71;
	}
	else if (strncmp((char *)_packetData, "$END", 6) == 0)
	{
		return 0x72;
	}
	else if (strncmp((char *)_packetData, "$CONF", 6) == 0)
	{
		return 0x73;
	}
}

/**
 * @brief
 *
 * @param _packetSnippet
 * @return true
 * @return false
 */
void appXK07::parsePacketSnippet(uint8_t _packetSnippet[])
{
	char *parseptr;

	if (strncmp((char *)_packetSnippet, "$SW01", 5) == 0)
	{
		parseptr = strchr((char *)_packetSnippet, ',') + 1;
		sdata.SW01_T = parseBuffer(parseptr);
		if (sdata.SW01_T != "ERR")
		{
			debugPrint((char *)_packetSnippet);
			sdata.SAT_SW01 = true;
			parseptr = strchr(parseptr, ',') + 1;
			sdata.SW01_H = parseBuffer(parseptr);
			parseptr = strchr(parseptr, ',') + 1;
			sdata.SW01_P = parseBuffer(parseptr);
			parseptr = strchr(parseptr, ',') + 1;
			sdata.SW01_A = parseBuffer(parseptr);
		}
		else
		{
			sdata.SAT_SW01 = false;
			sdata.SW01_H = "NULL";
			sdata.SW01_P = "NULL";
			sdata.SW01_A = "NULL";
		}
	}
	else if (strncmp((char *)_packetSnippet, "$SL01", 5) == 0)
	{
		parseptr = strchr((char *)_packetSnippet, ',') + 1;
		sdata.SL01_A = parseBuffer(parseptr);
		if (sdata.SL01_A != "ERR")
		{
			debugPrint((char *)_packetSnippet);
			sdata.SAT_SL01 = true;
			parseptr = strchr(parseptr, ',') + 1;
			sdata.SL01_B = parseBuffer(parseptr);
			parseptr = strchr(parseptr, ',') + 1;
			sdata.SL01_I = parseBuffer(parseptr);
			parseptr = strchr(parseptr, ',') + 1;
			sdata.SL01_L = parseBuffer(parseptr);
		}
		else
		{
			sdata.SAT_SL01 = false;
			sdata.SL01_A = "NULL";
			sdata.SL01_B = "NULL";
			sdata.SL01_I = "NULL";
			sdata.SL01_L = "NULL";
		}
	}
	else if (strncmp((char *)_packetSnippet, "$SI01", 5) == 0)
	{
		parseptr = strchr((char *)_packetSnippet, ',') + 1;
		sdata.SI01_R = parseBuffer(parseptr);
		if (sdata.SI01_R != "ERR")
		{
			debugPrint((char *)_packetSnippet);
			sdata.SAT_SI01 = true;
			parseptr = strchr(parseptr, ',') + 1;
			sdata.SI01_P = parseBuffer(parseptr);
			parseptr = strchr(parseptr, ',') + 1;
			sdata.SI01_GF = parseBuffer(parseptr);
		}
		else
		{
			sdata.SAT_SI01 = false;
			sdata.SI01_R = "NULL";
			sdata.SI01_P = "NULL";
			sdata.SI01_GF = "NULL";
		}
	}
	else if (strncmp((char *)_packetSnippet, "$PB04", 5) == 0)
	{
		parseptr = strchr((char *)_packetSnippet, ',') + 1;
		sdata.PB04_V = parseBuffer(parseptr);
		if (sdata.PB04_V != "ERR")
		{
			debugPrint((char *)_packetSnippet);
			sdata.SAT_PB04 = true;
			parseptr = strchr(parseptr, ',') + 1;
			sdata.PB04_C = parseBuffer(parseptr);
		}
		else
		{
			sdata.SAT_PB04 = false;
			sdata.PB04_V = "NULL";
			sdata.PB04_C = "NULL";
		}
	}
	else if (strncmp((char *)_packetSnippet, "$RL03", 5) == 0)
	{
		parseptr = strchr((char *)_packetSnippet, ',') + 1;
		sdata.LOC_RSSI = parseBuffer(parseptr);
	}
	else if (strncmp((char *)_packetSnippet, "$RL02", 5) == 0)
	{
		sdata.GS_RL02 = true;
		parseptr = strchr((char *)_packetSnippet, ',') + 1;
		sdata.LOC_RSSI = parseBuffer(parseptr);
		parseptr = strchr(parseptr, ',') + 1;
		sdata.GS_ID = parseBuffer(parseptr);
	}
	else if (strncmp((char *)_packetSnippet, "$RL01", 5) == 0)
	{
		sdata.GS_RL01 = true;
		parseptr = strchr((char *)_packetSnippet, ',') + 1;
		sdata.LOC_RSSI = parseBuffer(parseptr);
		parseptr = strchr(parseptr, ',') + 1;
		sdata.GS_ID = parseBuffer(parseptr);
	}
	else if (strncmp((char *)_packetSnippet, "$SN01", 5) == 0)
	{
		parseptr = strchr((char *)_packetSnippet, ',') + 1;
		sdata.SN01_HDOP = parseBuffer(parseptr);
		if (sdata.SN01_HDOP != "ERR")
		{
			debugPrint((char *)_packetSnippet);
			sdata.SAT_SN01 = true;
			parseptr = strchr(parseptr, ',') + 1;
			sdata.SN01_SAT = parseBuffer(parseptr);
			parseptr = strchr(parseptr, ',') + 1;
			sdata.SN01_D = parseBuffer(parseptr);
			parseptr = strchr(parseptr, ',') + 1;
			sdata.SN01_T = parseBuffer(parseptr);
			parseptr = strchr(parseptr, ',') + 1;
			sdata.SN01_STAT = parseBuffer(parseptr);

			if (sdata.SN01_STAT != "0")
			{
				parseptr = strchr(parseptr, ',') + 1;
				sdata.SN01_LAT = parseBuffer(parseptr);
				parseptr = strchr(parseptr, ',') + 1;
				sdata.SN01_LONG = parseBuffer(parseptr);
				parseptr = strchr(parseptr, ',') + 1;
				sdata.SN01_ALT = parseBuffer(parseptr);
				parseptr = strchr(parseptr, ',') + 1;
				// sdata.SN01_D = parseBuffer(parseptr);
				// parseptr = strchr(parseptr, ',') + 1;
				// sdata.SN01_T = parseBuffer(parseptr);
				// parseptr = strchr(parseptr, ',') + 1;
				sdata.SN01_SOG = parseBuffer(parseptr);
				parseptr = strchr(parseptr, ',') + 1;
				sdata.SN01_COG = parseBuffer(parseptr);
			}
			else
			{
				sdata.SN01_LAT = "NULL";
				sdata.SN01_LONG = "NULL";
				sdata.SN01_ALT = "NULL";
				// sdata.SN01_D = "NULL";
				// sdata.SN01_T = "NULL";
				sdata.SN01_SOG = "NULL";
				sdata.SN01_COG = "NULL";
			}
		}
		else
		{
			sdata.SAT_SN01 = false;
			sdata.SN01_HDOP = "NULL";
			sdata.SN01_SAT = "NULL";
			sdata.SN01_LAT = "NULL";
			sdata.SN01_LONG = "NULL";
			sdata.SN01_ALT = "NULL";
			sdata.SN01_D = "NULL";
			sdata.SN01_T = "NULL";
			sdata.SN01_SOG = "NULL";
			sdata.SN01_COG = "NULL";
			sdata.SN01_STAT = "NULL";
		}
	}
	else if (strncmp((char *)_packetSnippet, "$SG33", 5) == 0)
	{
		debugPrint((char *)_packetSnippet);
		parseptr = strchr((char *)_packetSnippet, ',') + 1;
		sdata.SG33_TVOC = parseBuffer(parseptr);
		if (sdata.SG33_TVOC != "ERR")
		{
			//debugPrint((char *)_packetSnippet);
			sdata.SAT_SG33 = true;
			parseptr = strchr(parseptr, ',') + 1;
			sdata.SG33_CO2 = parseBuffer(parseptr);
		}
		else
		{
			sdata.SAT_SG33 = false;
			sdata.SG33_TVOC = "NULL";
			sdata.SG33_CO2 = "NULL";
		}
	}
	else if (strncmp((char *)_packetSnippet, "$STAT", 5) == 0)
	{

		debugPrint((char *)_packetSnippet);
		parseptr = strchr((char *)_packetSnippet, ',') + 1;
		sdata.SAT_RSSI = parseBuffer(parseptr);
	}
}

/**
 * @brief
 *
 * @param str
 * @return String
 */
String appXK07::parseBuffer(char *str)
{
	char temp_str[150];
	memset(temp_str, 0, sizeof(temp_str));
	strcpy(temp_str, str);
	const char s[2] = ",";
	char *id;
	char *parse_ptr;
	delay(1);
	parse_ptr = strtok_r(temp_str, s, &id);
	return (String)parse_ptr;
}

/**
 * @brief
 *
 * @return int
 */
int appXK07::getAlmanacSize(void)
{
	return almSize;
}

/**
 * @brief
 *
 * @return uint8_t*
 */
uint8_t *appXK07::getAlmanac(void)
{
	return almanac; //TODO - is this right? does it need to be *almanac?
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool appXK07::downloadAlmanac(void) //TODO - seperate radio stuff
{

	WiFiClient client = WiFiClient();
	HTTPClient http1;

	// Try and connect to Live1, if that doesn't work, connect to Live2
	if (http1.begin(client, "http://online-live1.services.u-blox.com/GetOnlineData.ashx?token=60MHKSUo10q7ydnB5I9GgQ;gnss=gps;datatype=eph,alm,aux,pos;filteronpos;format=aid")) //TODO add latency tag?
	{																																											 // HTTP
		debugPrint("[HTTP] GET...");
		// start connection and send HTTP header
		int httpCode = http1.GET();

		// httpCode will be negative on error
		if (httpCode > 0)
		{
			// HTTP header has been send and Server response header has been handled
			debugPrint("[HTTP] GET... code: ", httpCode);

			// file found at server
			if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
			{
				// get lenght of document (is -1 when Server sends no Content-Length header)
				int len = http1.getSize();
				almSize = len;
				almanac = new uint8_t[len];

				// create buffer for read
				uint8_t buff[120] = {0};

				WiFiClient *stream = &client;
				// read all data from server
				// https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266HTTPClient/examples/StreamHttpClient/StreamHttpClient.ino
				while (http1.connected() && (len > 0 || len == -1))
				{
					ui.loading(" Downloading Almanac");
					delay(20);
					size_t c;
					// read up to 120 bytes
					c = stream->readBytes(buff, std::min((size_t)len, sizeof(buff)));
					debugPrint("readBytes: ", c); // TODO - for testing, remove
					if (!c)
					{
						debugPrint("read timeout");
					}
					// write it to Serial
					//debugPrint((char*)buff, c); // TODO - for testing, remove

					std::copy(buff + 0, buff + c, almanac + (almSize - len));

					if (len > 0)
					{
						len -= c;
					}
				}

				//ui.almDownloaded("  Almanac Downloaded");
				return true;
			}
			else
			{
				debugPrint("File not Found on Server");
				return false;
			}
		}
		else
		{
			global.blueDebug();
			ui.almDownloaded(" ALMA FAIL");
			debugPrint("[HTTP] GET... failed, error: ", httpCode);
			return false;
		}

		http1.end();
	}
	else
	{
		global.blueDebug();
		ui.almDownloaded(" ALMA FAIL");
		debugPrint("[HTTP] Unable to connect to UBlox API");
		return false;
	}
	global.blueDebug();
	ui.almDownloaded(" ALMA FAIL");
	return false;
}

/**
 * @brief
 *
 * @param _fwversion
 */
void appXK07::setFirmwareVersion(String _fwversion)
{
	strcpy(_firmwareVersion, _fwversion.c_str());
}

/**
 * @brief
 *
 * @param _fwversion
 */
void appXK07::setDeviceName(String _dname)
{
	strcpy(_deviceName, _dname.c_str());
}

/**
 * @brief
 *
 */
bool appXK07::getTimeStamp(void)
{
	dateTime = NTPch.getNTPtime(0.0, 0);
	String timeStamp = "";
	if (dateTime.valid)
	{
		String tmp = String(dateTime.year);
		info.year = tmp.substring(2, 4);
		timeStamp = dateTime.year;
		timeStamp += "-";
		if (dateTime.month < 10)
		{
			info.month = "0" + String(dateTime.month);
			timeStamp += "0";
			timeStamp += dateTime.month;
			timeStamp += "-";
		}
		else
		{
			info.month = dateTime.month;
			timeStamp += dateTime.month;
			timeStamp += "-";
		}
		if (dateTime.day < 10)
		{
			info.day = "0" + String(dateTime.day);
			timeStamp += "0";
			timeStamp += dateTime.day;
		}
		else
		{
			info.day = dateTime.day;
			timeStamp += dateTime.day;
		}
		timeStamp += "T";
		if (dateTime.hour < 10)
		{
			info.hour = "0" + String(dateTime.hour);
			timeStamp += "0";
			timeStamp += dateTime.hour;
			timeStamp += ":";
		}
		else
		{
			info.hour = dateTime.hour;
			timeStamp += dateTime.hour;
			timeStamp += ":";
		}
		if (dateTime.minute < 10)
		{
			info.minute = "0" + String(dateTime.minute);
			timeStamp += "0";
			timeStamp += dateTime.minute;
			timeStamp += ":";
		}
		else
		{
			info.minute = dateTime.minute;
			timeStamp += dateTime.minute;
			timeStamp += ":";
		}
		if (dateTime.second < 10)
		{
			info.second = "0" + String(dateTime.second);
			timeStamp += "0";
			timeStamp += dateTime.second;
		}
		else
		{
			info.second = dateTime.second;
			timeStamp += dateTime.second;
		}
		strcpy(_timeStamp, timeStamp.c_str());
		return true;
	}
	return false;
}

/**
 * @brief
 *
 */
void appXK07::setMqttClientName(void)
{
	mdata.cw01ClientName = ESP.getChipId();
	debugPrint(mdata.cw01ClientName);
}

/**
 * @brief
 *
 */
void appXK07::setMqttIndexName(void)
{
	sprintf(mdata.indexName, "x/data/xk07/%s", mdata.cw01ClientName.c_str());
	debugPrint(mdata.indexName);
}

/**
 * @brief
 *
 * @param _data
 */
void appXK07::debugPrint(String _data)
{
#ifdef DEBUG_SERIAL
	//Serial.begin(115200);
	Serial.print("app_xk07: ");
	Serial.println(_data);
	//Serial.end();
#endif
}

/**
 * @brief
 *
 * @param _data
 * @param _num
 */
void appXK07::debugPrint(String _data, uint16_t _num)
{
#ifdef DEBUG_SERIAL
	//Serial.begin(115200);
	Serial.print("app_xk07: ");
	Serial.print(_data);
	Serial.println(_num);
	//Serial.end();
#endif
}

void appXK07::uigs()
{
	if (sdata.PB04_V != "NULL")
		ui.info(info.hour + ":" + info.minute + ":" + info.second + " Z", "20" + info.year + "-" + info.month + "-" + info.day, String(info.rssi) + "dB", String((uint8_t)((atof(sdata.PB04_V.c_str()) / 3.3) * 100)) + "%");
	else
		ui.info(info.hour + ":" + info.minute + ":" + info.second + " Z", "20" + info.year + "-" + info.month + "-" + info.day, String(info.rssi) + "dB", "N/A");
	switch (uiCnt)
	{
	case 0:
		if (atof(sdata.SN01_HDOP.c_str()) < 6 && atof(sdata.SN01_HDOP.c_str()) > 0)
			ui.sn01(0, sdata.SN01_LAT, sdata.SN01_LONG, sdata.SN01_ALT);
		else if (sdata.SN01_HDOP != "NULL" && atof(sdata.SN01_HDOP.c_str()) > 6)
			ui.sn01(1, sdata.SN01_LAT, sdata.SN01_LONG, sdata.SN01_ALT);
		else
			ui.sn01(2, sdata.SN01_LAT, sdata.SN01_LONG, sdata.SN01_ALT);
		break;
	case 1:
		if (sdata.SI01_R != "NULL")
			ui.si01(true, sdata.SI01_R, sdata.SI01_P, sdata.SI01_GF);
		else
			ui.si01(false, sdata.SI01_R, sdata.SI01_P, sdata.SI01_GF);
		break;
	case 2:
		if (sdata.SW01_T != "ERR")
			ui.sw01(true, sdata.SW01_T, sdata.SW01_H, String(atof(sdata.SW01_P.c_str()) / 100));
		else
			ui.sw01(false, sdata.SW01_T, sdata.SW01_H, String(atof(sdata.SW01_P.c_str()) / 100));
		break;
	case 3:
		if (sdata.SG33_CO2 != "NULL")
			ui.sg33(true, sdata.SG33_CO2 + "ppm", sdata.SG33_TVOC + "ppb");
		else
			ui.sg33(false, sdata.SG33_CO2 + "ppm", sdata.SG33_TVOC + "ppb");
		break;
	case 4:
		if (sdata.SL01_L != "NULL")
			ui.sl01(true, sdata.SL01_L, sdata.SL01_I);
		else
			ui.sl01(false, sdata.SL01_L, sdata.SL01_I);
		break;
	}
	uiCnt++;
	if (uiCnt == 5)
		uiCnt = 0;
}

void appXK07::ownTracks()
{
	if ((atof(sdata.SN01_HDOP.c_str()) < 10) && (atof(sdata.SN01_HDOP.c_str()) > 0))
	{
#include <NTPClient.h>
#include <WiFiUdp.h>
		WiFiUDP ntpUDP;
		NTPClient timeClient(ntpUDP);
		timeClient.begin();

		StaticJsonDocument<200> doc;

		doc["cog"] = atof(sdata.SN01_COG.c_str());
		doc["batt"] = (uint8_t)((atof(sdata.PB04_V.c_str()) / 3.3) * 100);
		doc["lon"] = atof(sdata.SN01_LONG.c_str());
		doc["acc"] = atof(sdata.SN01_HDOP.c_str()) * 5;
		doc["p"] = atoi(sdata.SW01_P.c_str()) / 100;
		doc["bs"] = 1;
		doc["vel"] = atof(sdata.SN01_SOG.c_str()) * 3.6;
		doc["lat"] = atof(sdata.SN01_LAT.c_str());
		doc["tst"] = timeClient.getEpochTime();
		doc["alt"] = atof(sdata.SN01_ALT.c_str());
		doc["_type"] = "location";
		doc["tid"] = _deviceName;
		char buffer[256];
		char indexBuf[30];
		serializeJson(doc, buffer);
		sprintf(indexBuf, "owntracks/user/%s", mdata.cw01ClientName.c_str());
		if (connectMQTT(80))
		{
			mqtt.publish(indexBuf, buffer);
		}
	}
}

#endif
