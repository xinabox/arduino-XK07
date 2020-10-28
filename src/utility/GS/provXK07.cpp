/**
 * @file provXK07.cpp
 * @support (support@xinabox.cc)
 * @brief class for handling provisioning of data from XinaboxUploader on XK07 Ground Station
 */

#if defined(ARDUINO_ARCH_ESP8266)       // Include guard otherwise cpp files get compiled first and cause compilations errors

#include "provXK07.h"
#include "utility/uiXK07.h"
/**
 * @brief
 *
 * @return true
 * @return false
 */
bool provXK07::setupProvisioning(void)
{
	debugPrint("StartProv");
	prv.nameJsonFile("/XinaBoxUploaderXK07");
	prv.addVariable("WiFi_SSID", "your_network_name");
	prv.addVariable("WiFi_Password", "your_network_password");
	prv.addVariable("Unit_Name", "your_device_name");
	prv.radioSelect();
	if(prv.begin())
	{
		debugPrint("EndProv1");
		return true;
	}
	else
	{
		prv.saveConfigFile();
		//debugPrint("EndProv2");
		return prv.begin();
	}

	return false;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool provXK07::getData(void)
{
	prv.transmit();
	prv.receive();
	if (prv.success())
	{
		if (prv.getVariable("WiFi_Password", wifiPSK) && prv.getVariable("WiFi_SSID", wifiSSID) && prv.getVariable("Unit_Name", unitName) && prv.getVariable("Radio", radio))
		{
			return true;
		}
	}
	else
	{
		//debugPrint("Could not get provision data");
		return false;
	}
}

/**
 * @brief
 *
 * @return String
 */
String provXK07::getUnitName(void)
{
  	return unitName;
}

/**
 * @brief
 *
 * @return String
 */
String provXK07::getSSID(void)
{
  	return wifiSSID;
}

/**
 * @brief
 *
 * @return String
 */
String provXK07::getPassword(void)
{
	return wifiPSK;
}

String provXK07::getRadio(void)
{
	return radio;
}

/**
 * @brief
 *
 * @param _data
 */
void provXK07::debugPrint(String _data)
{
#ifdef DEBUG_SERIAL
	Serial.print("prov_xk07: ");
	Serial.println(_data);
#endif
}

/**
 * @brief
 *
 * @param _data
 * @param _num
 */
void provXK07::debugPrint(String _data, uint16_t _num)
{
#ifdef DEBUG_SERIAL
	Serial.print("prov_xk07: ");
	Serial.print(_data);
	Serial.println(_num);
#endif
}

#endif