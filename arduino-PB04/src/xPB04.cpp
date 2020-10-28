/*
	This is a library for the PB04 
	Smart Battery Module

	The board uses I2C for communication.
	
	The board communicates with one I2C devices:
	- MAX11646
	
	Data Sheets:
	MAX11646 - https://datasheets.maximintegrated.com/en/ds/MAX11646-MAX11647.pdf
*/

#include <xPB04.h>
#include <math.h>

/********************************************************
 	Constructor
*********************************************************/
xPB04::xPB04(){
	MAX11646_I2C_ADDRESS = 0x36;
	current = 0.0;
	voltage = 0.0;
	power = 0.0;
}

/********************************************************
 	Configure Sensor
*********************************************************/
bool xPB04::begin(){
	return true;
}

/********************************************************
 	Read Data from MAX11646 Sensor
*********************************************************/
void xPB04::poll(){
	readVoltage();
	readCurrent();
	readPower();
}

/********************************************************
 	Request Current measurent
*********************************************************/
float xPB04::getCurrent(){
	return current;
}

/********************************************************
 	Request Voltage measurent
*********************************************************/
float xPB04::getVoltage(){
	return voltage;
}

/********************************************************
 	Request power calculation
*********************************************************/
float xPB04::getPower(){
	return power;
}

/* Private Functions */ 

/********************************************************
 	Read Current
*********************************************************/
void xPB04::readCurrent(){
	uint16_t data;
	
	setupADC(MAX11646_CURRENT);
	data = xCore.request16(MAX11646_I2C_ADDRESS);
	
	c = (uint8_t)((data & 0xFF00) >> 8);
	d = (uint8_t)((data & 0x00FF) >> 0);

	current = ((((((((float)c - 252)*256) + (float)(d-1))*2.048)/1024)/100)/0.02)*1000;
}

/********************************************************
 	Read Voltage
*********************************************************/
void xPB04::readVoltage(){
	uint16_t data;
	
	setupADC(MAX11646_VOLTAGE);
	data = xCore.request16(MAX11646_I2C_ADDRESS);
	
	a = (uint8_t)((data & 0xFF00) >> 8);
	b = (uint8_t)(data & 0x00FF);
	
	voltage = ((((((float)a - 252)*256) + (float)b)*2.048)/1024)*2;
}

/********************************************************
 	Read Power
*********************************************************/
void xPB04::readPower(){
	power = (current/1000) * voltage;
}

/********************************************************
 	Configure ADC 
*********************************************************/
void xPB04::setupADC(byte data){
	xCore.write8(MAX11646_I2C_ADDRESS, MAX11646_SETUP, (uint8_t)data);
}
