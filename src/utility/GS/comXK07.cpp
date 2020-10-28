/**
 * @file comXK07.cpp
 * @support (support@xinabox.cc)
 * @brief Class for handling Radio Communication on XK07 Ground Station
 */

#include "comXK07.h"
#if defined(ARDUINO_ARCH_ESP8266) // Include guard otherwise cpp files get compiled first and cause compilations errors

/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t comXK07::begin(void)
{
	return setupRadio();
}

/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t comXK07::setupRadio(void)
{
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
 * @return true
 * @return false
 */
bool comXK07::waitSync(void)
{
	sendPacket(SyncSequence, sizeof(SyncSequence));
	if (receivePacket())
	{
		//Serial.print("Got packet:");
		//Serial.println((char*)rdata.packet);

		if (dataPacket())
		{
			if ((strncmp((char *)rdata.packet, "$SYNC", 5) == 0))
			{
				return true;
			}
			debugPrint("sync invalid");
		}
		debugPrint("not a data packet");

		return false;
	}
	return false;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool comXK07::sendConfigSequence(void)
{
	do
	{
		sendPacket(ConfigSequence, sizeof(ConfigSequence));
	} while (!receivePacket());
	debugPrint("got packet");
	if (dataPacket())
	{
		debugPrint("if data packet");
		if (strncmp((char *)rdata.packet, "$CONF", 5) == 0)
		{
			debugPrint("$CONF reply");
			return true;
		}
		return false;
	}
	return false;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool comXK07::sendStartSequence(void)
{
	do
	{
		sendPacket(StartSequence, sizeof(StartSequence));
	} while (!receivePacket());

	if (dataPacket())
	{
		if (strncmp((char *)rdata.packet, "$START", 5) == 0)
		{
			debugPrint("$START reply");
			return true;
		}
		return false;
	}
	return false;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool comXK07::sendEndSequence(void)
{
	do
	{
		sendPacket(EndSequence, sizeof(EndSequence));
	} while (!receivePacket());
	if (dataPacket())
	{
		if (strncmp((char *)rdata.packet, "$END", 5) == 0)
		{
			debugPrint("$END reply");
			return true;
		}
		return false;
	}
	return false;
}

/**
 * @brief
 *
 * @param almanac
 * @param almSize
 * @return true
 * @return false
 */
bool comXK07::sendAlmanac(uint8_t almanac[], int almSize)
{
	uint8_t sendBuf[] = "$STARTALMANAC";
	uint8_t rcvBuf[15];
	uint8_t rcvBufLen = sizeof(rcvBuf);
	uint8_t count = 0;
	const char *r = "$STARTALMANAC";
	while (1)
	{
		RL0X.send(sendBuf, sizeof(sendBuf));
		if (RL0X.waitAvailableTimeout(3000))
		{
			cntTest = 0;
			if (RL0X.recv(rcvBuf, &rcvBufLen))
			{
				if (strstr((const char *)rcvBuf, "$STARTALMANAC") == 0)
				{
					break;
				}
			}
		}
		else
		{
			cntTest++;
		}
		delay(10);
		count++;
		if (count == 30)
			return false;
	}
	if (sendPackets(almanac, almSize))
	{
		return true;
	}
	else
		return false;
	sendPackets(almanac, almSize);
	while (sendStartSequence())
	{
		break; // send the start sequence to tell the FS that the almanac is going to be sent
			   // send all the packets one by one
	}
	sendPackets(almanac, almSize);
	sendEndSequence(); // send the end sequence to tell the FS that no more packets will be sent
	return true;
}

/**
 * @brief
 *
 * @param almanac
 * @param almSize
 * @return true
 * @return false
 */
bool comXK07::sendPackets(const uint8_t almanac[], int almSize) // TODO - should use &almanac instead. copying 5kB file when we dont need to
{
	Serial.println();

	int len = almSize;
	uint8_t rcvBuf[1];
	uint8_t rcvBufLen = sizeof(rcvBuf);
	while (len >= 120) // While the length of the almanac is more than 120, keep sending 123 byte packets
	{
		ui.loading("   Sending Almanac");
		uint8_t packet[MAX_PACKET_SIZE]; // create a 123 byte packet

		std::copy(almanac + (almSize - len), almanac + ((almSize - len) + 120), packet + 2); // copy 120 bytes from the almanac file to the packet array. the point at which it copies from is incremented by 120 each loop

		// Add the length to the first index of the array, as well as an 'S' and an 'E' so the FS knows this is a valid message
		packet[0] = MAX_PACKET_SIZE;
		packet[1] = 'S';
		packet[122] = 'E';

		uint8_t cnt = 10;
		do
		{
			if (RL0X.send(packet, 123))
			{
				if (RL0X.waitAvailableTimeout(1000))
				{
					cntTest = 0;
					if (RL0X.recv(rcvBuf, &rcvBufLen))
					{
						if (rcvBuf[0] == 255)
						{

							for (int i = 2; i < 121; i++)
							{
								//Serial.print((char)packet[i]);
							}
							cnt = 11;
							len -= 120;
						}
						else
							cnt--;
					}
					else
						cnt--;
				}
				else
				{
					cntTest++;
					cnt--;
				}
			}
		} while ((cnt != 0) && (cnt != 11));
		if (cnt == 0)
			return false;
	}

	if (len > 0 && len < 120) // If the almanac length is less than 120, we've reached the end of the almanac, and need to send the last bytes
	{
		ui.loading("   Sending Almanac");
		uint8_t *packet;
		packet = new uint8_t[len + 3];
		if (packet == nullptr)
			debugPrint("Error: memory could not be allocated");

		std::copy(almanac + (almSize - len), almanac + almSize, packet + 2);
		packet[0] = len;
		packet[1] = 'S';
		packet[len + 2] = 'E';

		uint8_t cnt = 10;
		do
		{
			if (RL0X.send(packet, len + 2))
			{
				if (RL0X.waitAvailableTimeout(1000))
				{
					cntTest = 0;
					if (RL0X.recv(rcvBuf, &rcvBufLen))
					{
						if (rcvBuf[0] == 255)
						{
							for (int i = 2; i < (len + 2); i++)
							{
								//Serial.print((char)packet[i]);
							}
							cnt = 11;
						}
						else
							cnt--;
					}
					else
						cnt--;
				}
				else
				{
					cntTest++;
					cnt--;
				}
			}
		} while ((cnt != 0) && (cnt != 11));
		if (cnt == 0)
			return false;

		delete[] packet;
	}
	Serial.println();
	//ui.almDownloaded("     Almanac Sent");

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
	if (RL0X.send(_packet, _packetSize))
	{
		debugPrint((char *)_packet);
		debugPrint("packet sent");
		return true;
	}
	else
	{
		debugPrint("packet not sent");
		return false;
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
	debugPrint((char *)_packetHeader);

	uint8_t msgLen = sizeof(_packetHeader) + sizeof(_packet);
	uint8_t combinedPacket[msgLen + 1];

	memset(combinedPacket, '\0', sizeof(combinedPacket));

	memcpy(combinedPacket, _packetHeader, sizeof(_packetHeader) + 1);
	memcpy(combinedPacket, _packet, sizeof(_packet) + 1);

	debugPrint((char *)combinedPacket);

	if (RL0X.send(combinedPacket, sizeof(combinedPacket)))
	{
		for (int i = 0; i < sizeof(combinedPacket); i++)
		{
			Serial.print(combinedPacket[i]);
		}
		debugPrint("packet sent");
		return true;
	}
	else
	{
		debugPrint("packet not sent");
		return false;
	}

	return false;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool comXK07::receivePacket(void)
{
	uint8_t check[MAX_PACKET_SIZE];
	uint8_t len = sizeof(check);

	if (RL0X.waitAvailableTimeout(1000))
	{
		cntTest = 0;
		if (RL0X.recv(check, &len))
		{
			_lastRSSI = RL0X.lastRssi();
			memset(rdata.packet, '\0', sizeof(rdata.packet));
			memcpy(rdata.packet, check, len + 1);
			rdata.packetSize = sizeof(rdata.packet);

			if ((strncmp((char *)check, "$SYNC", 5) == 0) || (strncmp((char *)check, "$NACK", 5) == 0) || (strncmp((char *)check, "$ACK", 4) == 0) || (strncmp((char *)check, "$START", 6) == 0) || (strncmp((char *)check, "$END", 4) == 0) || (strncmp((char *)check, "$CONF", 5) == 0))
			{
				debugPrint("cmd msg");
				cmdPacket = true;
			}
			else if ((strncmp((char *)check, "$SW01", 5) == 0) || (strncmp((char *)check, "$SN01", 5) == 0) || (strncmp((char *)check, "$STAT", 5) == 0))
			{
				debugPrint("data msg");
				cmdPacket = false;
			}
			return true;
		}
		else
		{
			//debugPrint("invalid msg");
			return false;
		}
	}
	else
	{
		cntTest++;
		//debugPrint("no new data recv > recievePacket(void)");
		return false;
	}
}

int16_t comXK07::getLastRSSI(void)
{
	return _lastRSSI;
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
 * @return uint8_t
 */
uint8_t comXK07::getPacketSize(void)
{

	return sizeof(rdata.packet);
}

/**
 * @brief
 *
 * @return uint8_t*
 */
uint8_t *comXK07::getPacket(void)
{
	return rdata.packet; // return the buffer. This buffer will normally by 120 bytes.
}

/**
 * @brief
 *
 * @param _data
 */
void comXK07::debugPrint(String _data)
{
#ifdef DEBUG_SERIAL
	Serial.print("com_xk07: ");
	Serial.println(_data);
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
#ifdef DEBUG_SERIAL
	Serial.print("com_xk07: ");
	Serial.print(_data);
	Serial.println(_num);
#endif
}

/**
 * @brief
 *
 * @param _data
 */
void comXK07::debugPrintFS(String _data)
{
#ifdef DEBUG_SERIAL
	Serial.print("com_xk07: ");
	Serial.println(_data);
#endif
}

bool comXK07::resetFlightStation()
{
	uint8_t sendBuf[] = "$RESTART";
	uint8_t rcvBuf[15];
	uint8_t rcvBufLen = sizeof(rcvBuf);
	uint8_t count = 0;
	while (1)
	{
		RL0X.send(sendBuf, sizeof(sendBuf));
		if (RL0X.waitAvailableTimeout(3000))
		{
			cntTest = 0;
			if (RL0X.recv(rcvBuf, &rcvBufLen))
			{
				Serial.println();
				Serial.print("RESET : ");
				Serial.println((const char *)rcvBuf);
				Serial.println();
				if (strstr((const char *)rcvBuf, "$RESTART") != NULL)
				{
					break;
				}
			}
		}
		else
		{
			cntTest++;
		}
		delay(50);
	}
}

bool comXK07::testFSConnection()
{
	ui.fsConnecting("   Connecting to FS");
}

void comXK07::parseSelectedRadio(String radio)
{
	if(!RL0X.begin()) {
		global.redDebug();
	};
	RL0X.setThisAddress(rdata.RADIO_ID);
	RL0X.setHeaderFrom(rdata.RADIO_ID);
	RL0X.setHeaderTo(rdata.RADIO_ID);
	uint8_t sendBuf1[] = "$!$$!$434";
	uint8_t sendBuf2[] = "$!$$!$868";
	uint8_t sendBuf3[] = "$!$$!$915";

	if (radio == "RL01")
	{
		_frequency = rdata.RL01_FREQ;
	}
	else if (radio == "RL02")
	{
		_frequency = rdata.RL02_FREQ;
	}
	else if (radio == "RL03")
	{
		_frequency = rdata.RL03_FREQ;
	}
	else
	{
		_frequency = rdata.RL03_FREQ;
	}

	if (!RL0X.begin())
	{
		return;
	}
	// RL0X.setThisAddress(rdata.RADIO_ID);
	// RL0X.setHeaderFrom(rdata.RADIO_ID);
	// RL0X.setHeaderTo(rdata.RADIO_ID);
	uint8_t rcvBuf[15];
	uint8_t rcvBufLen = sizeof(rcvBuf);
	uint8_t count = 0;
	while (1)
	{
		if (radio == "RL01")
		{
			RL0X.send(sendBuf1, sizeof(sendBuf1));
		}
		else if (radio == "RL02")
		{
			RL0X.send(sendBuf2, sizeof(sendBuf2));
		}
		else if (radio == "RL03")
		{
			RL0X.send(sendBuf3, sizeof(sendBuf3));
		}
		else
		{
			RL0X.send(sendBuf3, sizeof(sendBuf3));
		}

		if (RL0X.waitAvailableTimeout(3000))
		{
			cntTest = 0;
			if (RL0X.recv(rcvBuf, &rcvBufLen))
			{
				if (strstr((const char *)rcvBuf, "$!$$!$FREQSET") != NULL)
				{
					break;
				}
			}
		}
		else
		{
			cntTest++;
		}
		delay(50);
	}
}
#endif