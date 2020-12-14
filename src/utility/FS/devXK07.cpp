/**
 * @file devXK07.cpp
 * @support (support@xinabox.cc)
 * @brief Class for handling Sensors on XK07 Flight Station
 */
#include "devXK07.h"
#if defined(ARDUINO_SAMD_ZERO)

/**
 * @brief order of x-chips in 8-bit value : SN01 SL01 SW01 SG33 PB04 SI01 RL0X OD01
 *
 * @return uint8_t
 */
uint8_t devXK07::CheckXChips(void)
{
	global.watchdog();
	uint8_t sensors_available = 0b00000000;

	if (checkSN01())
	{
		sensors_available |= 0b10000000;
	}
	if (checkSL01())
	{
		sensors_available |= 0b01000000;
	}
	if (checkSW01())
	{
		sensors_available |= 0b00100000;
	}
	if (checkSG33())
	{
		sensors_available |= 0b00010000;
	}
	if (checkPB04())
	{
		sensors_available |= 0b00001000;
	}
	if (checkSI01())
	{
		sensors_available |= 0b00000100;
	}
	if (checkRL0X())
	{
		sensors_available |= 0b00000010;
	}
	if (checkOD01())
	{
		sensors_available |= 0b00000001;
	}
	//Serial1.println(sensors_available, BIN);

	return sensors_available;
}

/**
 * @brief
 *
 */
void devXK07::initSensors(void)
{
	global.watchdog();
	if (xCore.ping(i2c.SG33_ADDRESS))
	{
		//xSG33 SG33 = xSG33();
		if (!SG33.begin())
		{
			debugPrint("SG33 FAIL START");
		}
	}
}

/**
 * @brief
 *
 * @param _lastRSSI
 * @return uint8_t*
 */
uint8_t *devXK07::payloadA(int16_t _lastRSSI)
{
	global.watchdog();
	memset(pdata.packet, '\0', MAX_PACKET_SIZE);
	String payload = "";
	if (xCore.ping(i2c.SW01_ADDRESS))
	{
		global.watchdog();
		xSW01 SW01;
		SW01.begin();
		SW01.poll();
		payload += "$SW01,";
		payload += String(SW01.getTempC(), 2);
		payload += ",";
		payload += String(SW01.getHumidity(), 0);
		payload += ",";
		payload += String(SW01.getPressure(), 0);
		payload += ",";
		payload += String(SW01.getQNE(), 0);
		payload += ",*,";
	}
	else
	{
		payload += "$SW01,ERR,*,";
	}

	// Add SI01
	if ((xCore.ping(i2c.SI01_ADDRESS_1) && xCore.ping(i2c.SI01_ADDRESS_2)) == true)
	{
		global.watchdog();
		xSI01 SI01;
		SI01.begin();
		SI01.poll();
		payload += "$SI01,";
		payload += String(SI01.getRoll(), 2);
		payload += ",";
		payload += String(SI01.getPitch(), 2);
		payload += ",";
		payload += String(SI01.getGForce(), 2);
		payload += ",*,";
	}
	else
	{
		payload += "$SI01,ERR,*,";
	}

	// Add SL01
	if (((xCore.ping(i2c.SL01_ADDRESS_1) && xCore.ping(i2c.SL01_ADDRESS_2)) == true) || (xCore.ping(i2c.SL01_ADDRESS_3) == true))
	{
		global.watchdog();
		xSL01 SL01;
		if (!sl01flag)
		{
			sl01flag = true;
			SL01.begin();
		}

		SL01.poll();
		if (SL01.checkVersion() == 1)
		{
			payload += "$SL01,";
			payload += String((uint32_t)SL01.getUVA());
			payload += ",";
			payload += String((uint32_t)SL01.getUVB());
			payload += ",";
		}
		else
		{
			payload += "$SL01,";
			payload += "";
			payload += ",";
			payload += "";
			payload += ",";
		}
		payload += String(SL01.getUVIndex(), 1);
		payload += ",";
		payload += String(SL01.getLUX(), 0);
		payload += ",*,";
	}
	else
	{
		sl01flag = false;
		payload += "$SL01,ERR,*,";
	}

	// Add PB04
	if (xCore.ping(i2c.PB04_ADDRESS))
	{
		global.watchdog();
		xPB04 PB04;
		PB04.begin();
		PB04.poll();
		payload += "$PB04,";
		payload += String(PB04.getVoltage(), 2);
		payload += ",";
		payload += String(PB04.getCurrent(), 0);
		payload += ",*,";
	}
	else
	{
		payload += "$PB04,ERR,*,";
	}

	if (xCore.ping(i2c.SG33_ADDRESS))
	{
		global.watchdog();
		unsigned long tim1 = millis();
		uint8_t tim2 = 0;
		while (!SG33.dataAvailable() && (tim2 < 13)) // wait for data but no more than 2.5 seconds
		{
			if ((millis() - tim1) > 200)
			{
				tim2++;
				tim1 = millis();
			}
		};
		if (SG33.getAlgorithmResults())
		{
			payload += "$SG33,";
			payload += String((uint16_t)SG33.getTVOC());
			payload += ",";
			payload += String((uint16_t)SG33.getCO2());
			payload += ",*,";
		}
		else
		{
			//debugPrint("unable to get data SG33");
			payload += "$SG33,ERR,*,";
		}
	}
	else
	{
		//debugPrint("no SG33");
		payload += "$SG33,ERR,*,";
	}

	payload += "$STAT,";
	payload += _lastRSSI;
	payload += ",*,";

	strcpy((char *)pdata.packet, payload.c_str());
	pdata.packetSize = strlen(payload.c_str());
	return pdata.packet;
}

/**
 * @brief
 *
 * @return String
 */
uint8_t *devXK07::payloadB(int16_t _lastRSSI)
{
	memset(pdata.packet, '\0', MAX_PACKET_SIZE);
	String payload = "";
	if (xCore.ping(i2c.SN01_ADDRESS))
	{
		global.watchdog();
		xSN01 SN01;

		while (!SN01.readGPS())
			;

		if (SN01.getHDOP() == 0)
		{
			while (!SN01.readGPS())
			{
				delay(150);
			}
		}

		payload = "$SN01,";
		payload += String(SN01.getHDOP(), 1);
		payload += ",";
		payload += String(SN01.getSatelitesConnected());
		payload += ",";
		payload += String(SN01.getDate());
		payload += ",";
		payload += String(SN01.getTime());
		payload += ",";

		SerialUSB.println(SN01.getLatitude(), 5);
		SerialUSB.println(SN01.getLongitude(), 5);
		if ((SN01.getHDOP() < 10.0) && (SN01.getHDOP() > 0.0))
		{
			global.watchdog();
			//GPS_FIX = true;
			payload += "1,";
			payload += String(SN01.getLatitude(), 5);
			payload += ",";
			payload += String(SN01.getLongitude(), 5);
			payload += ",";
			payload += String(SN01.getAltitude(), 0);
			payload += ",";
			// payload += String(SN01.getDate());
			// payload += ",";
			// payload += String(SN01.getTime());
			// payload += ",";
			payload += String(SN01.getSOG(), 2);
			payload += ",";
			payload += String(SN01.getCOG(), 2);
			payload += ",*,";
		}
		else
		{
			//GPS_FIX = false;
			payload += "0,*,";
		}
	}
	else
	{
		payload = "$SN01,ERR,*,";
	}

	// Add PB04
	if (xCore.ping(i2c.PB04_ADDRESS))
	{
		global.watchdog();
		xPB04 PB04;
		PB04.begin();
		PB04.poll();
		payload += "$PB04,";
		payload += String(PB04.getVoltage(), 2);
		payload += ",";
		payload += String(PB04.getCurrent(), 0);
		payload += ",*";
	}
	else
	{
		payload += "$PB04,ERR,*";
	}

	payload += "$STAT,";
	payload += _lastRSSI;
	payload += ",*,";

	strcpy((char *)pdata.packet, payload.c_str());
	pdata.packetSize = strlen(payload.c_str());
	return pdata.packet;
}

/**
 * @brief
 *
 * @return String
 */
uint8_t *devXK07::payloadC(int16_t _lastRSSI)
{
	global.watchdog();
	memset(pdata.packet, '\0', MAX_PACKET_SIZE);
	String payload = "";
	payload += "$STAT,";
	payload += _lastRSSI;
	payload += ",*,";
	strcpy((char *)pdata.packet, payload.c_str());
	pdata.packetSize = strlen(payload.c_str());
	return pdata.packet;
}

/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t devXK07::getPacketSize(void)
{
	return pdata.packetSize;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool devXK07::checkSN01(void)
{
	global.watchdog();
	if (xCore.ping(i2c.SN01_ADDRESS))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool devXK07::checkSL01(void)
{
	global.watchdog();
	if (xCore.ping(i2c.SL01_ADDRESS_1) && xCore.ping(i2c.SL01_ADDRESS_2))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool devXK07::checkSW01(void)
{
	global.watchdog();
	if (xCore.ping(i2c.SW01_ADDRESS))
	{
		return true;
	}
	else if (xCore.ping(i2c.SW01_ADDRESS_ALT))
	{
		debugPrint("[ERROR] [SW01] Alternate I2C Address is being used.");
		return false;
	}
	else
	{
		return false;
	}
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool devXK07::checkSG33(void)
{
	global.watchdog();
	if (xCore.ping(i2c.SG33_ADDRESS))
	{
		return true;
	}
	else if (xCore.ping(i2c.SG33_ADDRESS_ALT))
	{
		debugPrint("[ERROR] [SG33] Alternate I2C Address is being used.");

		return false;
	}
	else
	{
		return false;
	}
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool devXK07::checkPB04(void)
{
	global.watchdog();
	if (xCore.ping(i2c.PB04_ADDRESS))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool devXK07::checkSI01(void)
{
	global.watchdog();
	if (xCore.ping(i2c.SI01_ADDRESS_1) && xCore.ping(i2c.SI01_ADDRESS_2))
	{
		return true;
	}
	else if (xCore.ping(i2c.SI01_ADDRESS_1_ALT) || xCore.ping(i2c.SI01_ADDRESS_2_ALT))
	{
		debugPrint("[ERROR] [SI01] Alternate I2C Address is being used.");
		return false;
	}
	else
	{
		return false;
	}
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool devXK07::checkRL0X(void)
{
	global.watchdog();
	if (xCore.ping(i2c.RL0X_ADDRESS))
	{
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
bool devXK07::checkOD01(void)
{
	global.watchdog();
	if (xCore.ping(i2c.OD01_ADDRESS))
	{
		return true;
	}

	return false;
}

/**
 * @brief
 *
 * @param _data
 */
void devXK07::debugPrint(String _data)
{
	global.watchdog();
#ifdef DEBUG_SERIAL
	SerialUSB.print("dev_xk07: ");
	SerialUSB.println(_data);
#endif
}

/**
 * @brief
 *
 * @param _data
 */
void devXK07::debugPrint(uint8_t _data)
{
	global.watchdog();
#ifdef DEBUG_SERIAL
	SerialUSB.print("dev_xk07: ");
	SerialUSB.println(_data, BIN);
#endif
}

void devXK07::uifs()
{
	ui.uifs(xCore.ping(i2c.SN01_ADDRESS), xCore.ping(i2c.SG33_ADDRESS), xCore.ping(i2c.SI01_ADDRESS_1), xCore.ping(i2c.SW01_ADDRESS),
			xCore.ping(i2c.SL01_ADDRESS_1) || xCore.ping(i2c.SL01_ADDRESS_3), xCore.ping(i2c.RL0X_ADDRESS), xCore.ping(i2c.PB04_ADDRESS));
}

bool devXK07::testMovement()
{
	global.watchdog();
	if (xCore.ping(i2c.SI01_ADDRESS_1))
	{
		global.watchdog();
		xSI01 SI01;
		SI01.begin();
		SI01.poll();
		global.watchdog();
		float x = SI01.getAX();
		float y = SI01.getAY();
		float z = SI01.getAZ();
		float magnitude = sqrt(x * x + y * y + z * z);
		global.watchdog();
		if (magnitude > 0.95 && magnitude < 1.05)
		{
			if (xCore.ping(0x41))
			{
				global.watchdog();
				xOC01 OC01;
				OC01.begin();
				OC01.digitalWrite(OUT0, HIGH);
				OC01.digitalWrite(OUT1, HIGH);
				global.watchdog();
				delay(1000);
				global.watchdog();
				OC01.digitalWrite(OUT0, LOW);
				OC01.digitalWrite(OUT1, LOW);
				global.watchdog();
			}
		}
		else
		{
			if (xCore.ping(0x41))
			{
				global.watchdog();
				xOC01 OC01;
				OC01.begin();
				OC01.digitalWrite(OUT0, LOW);
				OC01.digitalWrite(OUT1, LOW);
				global.watchdog();
			}
		}
	}
}
#endif