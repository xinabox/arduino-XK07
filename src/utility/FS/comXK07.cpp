/**
 * @file comXK07.cpp
 * @support (support@xinabox.cc)
 * @brief Class for handling Radio Communication on XK07 Flight Station
 */

#include "comXK07.h"

#if defined(ARDUINO_SAMD_ZERO)

/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t comXK07::begin(void)
{
	global.watchdog();
	return setupRadio();
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
uint8_t comXK07::setupRadio(void)
{
	global.watchdog();
	if (!RL0X.begin())
	{
		debugPrint("Check the connector to the RL0X");
		return 0x30;
	}
	else
	{
		debugPrint("Radio Initialized");
		RL0X.setFrequency(rdata.RL03_FREQ);
		RL0X.setTxPower(23, false);
		RL0X.setThisAddress(rdata.RADIO_ID);
		RL0X.setHeaderFrom(rdata.RADIO_ID);
		RL0X.setHeaderTo(rdata.RADIO_ID);
		return RUN_OK;
	}
}

/**
 * @brief
 *
 * @param _mode
 * @return uint8_t
 */
uint8_t comXK07::setMode(uint8_t _mode)
{
	global.watchdog();
	uint8_t state = 0;
	return state;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool comXK07::waitSync(void)
{
	global.watchdog();
	if (RL0X.waitAvailableTimeout(500))
	{
		uint8_t buf[sizeof(SyncSequence)];
		uint8_t len = sizeof(buf);

		//debugPrint("waiting for SYNC");

		if (RL0X.recv(buf, &len))
		{
			SerialUSB.println((const char *)buf);
			restartFlightStation(buf);
			_lastRSSI = RL0X.lastRssi();

			debugPrint((char *)buf);
			if (equalArrays(SyncSequence, buf, len)) // check if the message received is equal to [START]
			{
				sendPacket(SyncSequence, sizeof(SyncSequence));
				debugPrint("sync recv ack");
				return true;
			}
			else
			{
				sendPacket(NackSequence, sizeof(NackSequence)); // If it isn't equal, tell GS that it wasn't received, then GS will send start sequence again
				debugPrint("sync recv nack");
				return false;
			}
		}
		else
		{
			sendPacket(NackSequence, sizeof(NackSequence));
			debugPrint("recv failed");
		}
	}
	else
	{
		sendPacket(NackSequence, sizeof(NackSequence));
		debugPrint("no new data recv > waitSync(void)");
		return false;
	}
}

/**
 * @brief
 *
 * @return int16_t
 */
int16_t comXK07::getLastRSSI(void)
{
	global.watchdog();
	return RL0X.lastRssi();
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool comXK07::receiveStartSequence(void)
{
	global.watchdog();
	if (RL0X.waitAvailableTimeout(250))
	{
		uint8_t buf[sizeof(StartSequence)];
		uint8_t len = sizeof(buf);

		debugPrint("waiting start sequence");

		if (RL0X.recv(buf, &len))
		{
			SerialUSB.println((const char *)buf);
			restartFlightStation(buf);
			if (equalArrays(StartSequence, buf, len)) // check if the message received is equal to [START]
			{
				sendPacket(AckSequence, sizeof(AckSequence)); // If it is equal, tell GS that the start sequence was received
				debugPrint("start sequence ack");
				return true;
			}
			else
			{
				sendPacket(NackSequence, sizeof(NackSequence)); // If it isn't equal, tell GS that it wasn't received, then GS will send start sequence again
				debugPrint("start sequence nack");
				return false;
			}
		}
		else
		{
			sendPacket(NackSequence, sizeof(NackSequence));
			debugPrint("recv failed");
			return false;
		}
	}
	else
	{
		sendPacket(NackSequence, sizeof(NackSequence));
		debugPrint("no new data recv > receiveStartSequence(void)");
		return false;
	}
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool comXK07::receiveEndSequence(void)
{
	global.watchdog();
	if (RL0X.waitAvailableTimeout(1000))
	{
		Watchdog.reset();
		uint8_t buf[sizeof(EndSequence)];
		uint8_t len = sizeof(buf);

		debugPrint("waiting for end sequence");

		if (RL0X.recv(buf, &len))
		{
			SerialUSB.println((const char *)buf);
			restartFlightStation(buf);
			if (equalArrays(EndSequence, buf, len)) // check if the message received is equal to [END]
			{
				sendPacket(AckSequence, sizeof(AckSequence)); // If it is equal, tell GS that the end sequence was received
				debugPrint("end sequence ack");
				return true;
			}
			else
			{
				sendPacket(NackSequence, sizeof(NackSequence)); // If it isn't equal, tell GS that it wasn't received, then GS will send end sequence again
				debugPrint("end sequence nack");
				return false;
			}
		}
		else
		{
			sendPacket(NackSequence, sizeof(NackSequence));
			debugPrint("recv fail");
			return false;
		}
	}
	else
	{
		sendPacket(NackSequence, sizeof(NackSequence));
		debugPrint("no new data recv > receiveEndSequence(void)");
		return false;
	}
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
const char *comXK07::receiveAlmanac(void)
{
	global.watchdog();
	char almanac_string[5000] = {NULL};
	uint8_t rcvBuf[15];
	uint8_t rcvBufLen = sizeof(rcvBuf);
	uint8_t sendBuf[] = "$STARTALMANAC";
	uint8_t count = 0;
	const char*r = "$STARTALMANAC";
	while (1)
	{
		Watchdog.reset();
		debugPrint("waiting for almanac");
		if (RL0X.waitAvailableTimeout(500))
		{
			Watchdog.reset();
			if (RL0X.recv(rcvBuf, &rcvBufLen))
			{
				SerialUSB.println((const char *)rcvBuf);
				restartFlightStation(rcvBuf);

				if (strstr((const char *)rcvBuf, r) != NULL)
				{
					RL0X.send(sendBuf, sizeof(sendBuf));
					break;
				}
			}
		}
		count++;
		if (count == 20)
			return "";
	}

	uint8_t buf[123];
	int c = 0;

	while (1)
	{
		uint8_t cnt[1] = {0};
		if (RL0X.waitAvailableTimeout(3000))
		{
			Watchdog.reset();
			ui.loading("  Receiving Almanac");
			uint8_t len = sizeof(buf);
			if (RL0X.recv(buf, &len))
			{
				Watchdog.reset();
				SerialUSB.println((const char *)buf);
				restartFlightStation(buf);

				if ((char)buf[0] != 150)
				{
					uint8_t k = (char)buf[0];
					if (xCore.ping(0x42))
					{
						for (int h = 2; h < (uint8_t)buf[0] + 2; h++)
						{
							//SerialUSB.print((char)buf[h]);
							Wire.beginTransmission(0x42);
							Wire.write((uint8_t)buf[h]);
							Wire.endTransmission(true);
							char c = (char)buf[h];
							//strncat(almanac_string, &c, 1);
						}
					}
					cnt[0] = 255;
					RL0X.send(cnt, sizeof(cnt));
					break;
				}
				else
				{
					Watchdog.reset();
					if (xCore.ping(0x42))
					{
						for (int h = 2; h < 121; h++)
						{
							//SerialUSB.print((char)buf[h]);
							Wire.beginTransmission(0x42);
							Wire.write((uint8_t)buf[h]);
							Wire.endTransmission(false);
							char c = (char)buf[h];
							//strncat(almanac_string, &c, 1);
						}
					}
					cnt[0] = 255;
					RL0X.send(cnt, sizeof(cnt));
					continue;
				}
			}
			else
			{
				//SerialUSB.println("recv failed");
			}
		}
	}
	ui.almDownloaded("   Almanac Received");
	// for (int i = 0; i < strlen(almanac_string); i++)
	// {
	// 	SerialUSB.print((char)almanac_string[i]);
	// }
	//return true;

	// bool EndSequenceReceived{false};

	// while (receiveStartSequence())
	// {
	// 	debugPrint("waiting for start");
	// 	delay(50);
	// } // Wait for the start sequence to be received

	// //Wire.beginTransmission(rdata.SN01_I2C_ADDR);

	// while (!EndSequenceReceived)
	// { // while the end string hasn't been received from the GS
	// 	debugPrint("waiting for for end");
	// 	delay(250);
	// 	if (receivePacket())
	// 	{
	// 		//Wire.write(getPacket(), data_length);
	// 		sendPacket(AckSequence, sizeof(AckSequence));
	// 	}
	// 	else if (receiveEndSequence())
	// 	{
	// 		//Wire.endTransmission();
	// 		EndSequenceReceived = true;
	// 	}
	// 	else
	// 	{
	// 		sendPacket(NackSequence, sizeof(NackSequence));
	// 	}
	// }
	return almanac_string;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool comXK07::receivePacket(void)
{
	global.watchdog();
	uint8_t check[MAX_PACKET_SIZE];
	uint8_t len = sizeof(check);
	if (RL0X.waitAvailableTimeout(1000))
	{
		global.watchdog();
		if (RL0X.recv(check, &len))
		{
			SerialUSB.println((const char *)check);
			global.watchdog();
			restartFlightStation(check);
			//debugPrint((char *)check);
			memset(rdata.packet, '\0', sizeof(rdata.packet));
			memcpy(rdata.packet, check, len + 1);
			if ((strncmp((char *)check, "$SYNC", 5) == 0) || (strncmp((char *)check, "$NACK", 5) == 0) || (strncmp((char *)check, "$ACK", 4) == 0) || (strncmp((char *)check, "$START", 6) == 0) || (strncmp((char *)check, "$END", 4) == 0) || (strncmp((char *)check, "$CONF", 5) == 0))
			{
				//debugPrint("cmd msg");
				cmdPacket = true;
			}
			else
			{
				//debugPrint("data msg");
				cmdPacket = false;
			}
			return true;
		}
		return false;
	}
	return false;
}

const char *comXK07::getAlmanac()
{
	global.watchdog();
	return almanac_char;
}

/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t comXK07::getPacketSize(void)
{
	global.watchdog();
	return rdata.packetSize;
}

/**
 * @brief
 *
 * @return uint8_t*
 */
uint8_t *comXK07::getPacket(void)
{
	global.watchdog();
	return rdata.packet; // return the buffer. This buffer will normally by 120 bytes.
}

/**
 * @brief
 *
 * @param a
 * @param b
 * @param n
 * @return true
 * @return false
 */
bool comXK07::equalArrays(const uint8_t a[], const uint8_t b[], uint8_t n)
{
	global.watchdog();
	for (int i = 0; i < n; i++)
	{
		if (a[i] != b[i])
		{
			return false;
		}
	}
	return true;
}

/**
 * @brief
 *
 * @param _packet
 * @param _packetSize
 * @return true
 * @return false
 */
bool comXK07::sendPacket(const uint8_t _packet[], uint8_t _packetSize) // TODO - should use &almanac instead. copying 5kB file when we dont need to
{
	global.watchdog();
	if (RL0X.send(_packet, _packetSize))
	{
		return true;
	}
	return false;
}

/**
 * @brief
 *
 * @param _packet
 * @param _packetSize
 * @return true
 * @return false
 */
bool comXK07::sendPacket(const uint8_t _packetHeader[], uint8_t _packet[], uint8_t _packetSize) // TODO - should use &almanac instead. copying 5kB file when we dont need to
{
	global.watchdog();
	//debugPrint((char*)_packetHeader);
	uint8_t msgLen = sizeof(_packetHeader) + sizeof(_packet);
	uint8_t combinedPacket[msgLen + 1];
	memset(combinedPacket, '\0', sizeof(combinedPacket));
	memcpy(combinedPacket, _packetHeader, sizeof(_packetHeader) + 1);
	memcpy(combinedPacket, _packet, sizeof(_packet) + 1);
	//debugPrint((char*)combinedPacket);
	if (RL0X.send(combinedPacket, sizeof(combinedPacket)))
	{
		debugPrint("packet sent");
		return true;
	}
	debugPrint("packet not sent");
	return false;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool comXK07::dataPacket(void)
{
	return cmdPacket;
}

/**
 * @brief
 *
 * @param _data
 */
void comXK07::debugPrint(String _data)
{
	global.watchdog();
#ifdef DEBUG_SERIAL
	SerialUSB.print("com_xk07: ");
	SerialUSB.println(_data);
#endif
}

/**
 * @brief
 *
 * @param _data
 * @param _num
 */
void comXK07::debugPrint(String _data, uint16_t _num)
{
	global.watchdog();
#ifdef DEBUG_SERIAL
	SerialUSB.print("com_xk07: ");
	SerialUSB.print(_data);
	SerialUSB.println(_num);
#endif
}

void comXK07::restartFlightStation(uint8_t buf[])
{
	SerialUSB.println((const char*)buf);
	const char *r = "$RESTART";
	if (strstr((const char *)buf, r) != NULL)
	{
		SerialUSB.println("RESTARTED FLIGHT STATION");
		uint8_t s[] = "$RESTART";
		delay(50);
		RL0X.send(s, sizeof(s));
		NVIC_SystemReset();
	}

	// String _buf = (const char*)buf;
	// _buf.substring(sizeof(buf) - 9, sizeof(buf));
	// SerialUSB.println(_buf);
	// Watchdog.reset();
	// if (strcmp(_buf.c_str(), "$RESTART") == 0)
	// {
	// 	SerialUSB.println("RESTARTED FLIGHT STATION");
	// 	uint8_t s[] = "$RESTART";
	// 	delay(50);
	// 	RL0X.send(s, sizeof(s));
	// 	NVIC_SystemReset();
	// }
	Watchdog.reset();
}

void comXK07::receiveRadioParameters()
{
	if (!RL0X.begin())
	{
		global.redDebug();
		return;
	}
	RL0X.setThisAddress(rdata.RADIO_ID);
	RL0X.setHeaderFrom(rdata.RADIO_ID);
	RL0X.setHeaderTo(rdata.RADIO_ID);

	uint8_t buf[15];
	uint8_t len = sizeof(buf);
	const char *_rl01 = "$!$$!$434";
	const char *_rl02 = "$!$$!$868";
	const char *_rl03 = "$!$$!$915";
	while (1)
	{
		if (RL0X.waitAvailableTimeout(3000))
		{
			if (RL0X.recv(buf, &len))
			{
				if (strstr((const char *)buf, _rl01) != NULL)
				{
					_frequency = rdata.RL01_FREQ;
					uint8_t s[] = "$!$$!$FREQSET";
					delay(50);
					RL0X.send(s, sizeof(s));
					break;
				}
				else if (strstr((const char *)buf, _rl02) != NULL)
				{
					_frequency = rdata.RL02_FREQ;
					uint8_t s[] = "$!$$!$FREQSET";
					delay(50);
					RL0X.send(s, sizeof(s));
					break;
				}
				else if (strstr((const char *)buf, _rl03) != NULL)
				{
					_frequency = rdata.RL03_FREQ;
					uint8_t s[] = "$!$$!$FREQSET";
					delay(50);
					RL0X.send(s, sizeof(s));
					break;
				}
				else
				{
					_frequency = rdata.RL03_FREQ;
					break;
				}
			}
		}
	}
}

#endif