/*
	This is a library for the OC01 
	BURN WIRE

	The board uses I2C for communication.
	The board communicates with the following I2C device:
	* 	PCA9536
	
	Data Sheets:
	PCA9536 - http://www.ti.com/lit/ds/symlink/pca9536.pdf
*/

#ifndef xOC01_h
#define xOC01_h

// Include File Headers
#include "xCore.h"

// PCA9536 Register Definitions
#define PCA9536_REG_INPUT_PORT		0x00
#define PCA9536_REG_OUTPUT_PORT		0x01	
#define PCA9536_REG_POL_INVERSION	0x02
#define PCA9536_REG_CONFIG			0x03

#define PCA9536_CONF_OUTPUT			0x00
#define PCA9536_CONF_INPUT			0x0F

#define PCA9536_ALL_OUTPUTS_OFF		0x00

#define PCA9536_PIN0_OUTPUT			0x00
#define PCA9536_PIN0_INPUT			0x01

#define PCA9536_PIN1_OUTPUT			0x00
#define PCA9536_PIN1_INPUT			0x02

#define PCA9536_PIN2_OUTPUT			0x00
#define PCA9536_PIN2_INPUT			0x04

#define PCA9536_PIN3_OUTPUT			0x00
#define PCA9536_PIN3_INTPUT			0x08

#define OUT0 						0x01
#define OUT1 						0x02
#define OUT2 						0x04
#define OUT3 						0x08

/********************************************************************/

class xOC01: public xCoreClass
{
	public:
		/**
		* Constructor
		* Creates a new instance of Sensor class.
		*/		
		xOC01();
		
		/*
		* Runs the setup of the sensor. 
		* Call this in setup(), before reading any sensor data.
		*
		* @return true if setup was successful.
		*/		
		bool begin(void);

		/*
		* Runs the setup of the sensor. 
		* Call this in setup(), before reading any sensor data.
		*
		* @param pins, allows users to configure specific pins 
		* to be high on start up.
		*
		* @return true if setup was successful.
		*/	
		bool begin(uint8_t pins);		
		
		/*
		* Used to write to the pin. HIGH or LOW. 
		* Call this in loop().
		*
		* @param pin. OC01 has 4 Pin options to write to.
		* @param state. State which you would like to write to the output pin.
		* @return none
		*/			
		void digitalWrite(uint8_t pin, bool state);

		/*
		* Used to determine the status of Output Pins 
		* Call this in loop().
		*
		* @param pin. OC1 has 4 Pin options to write to.
		* @param state. State which you would like to write to the output pin.
		* @return status of bytes
		*/
		uint8_t getStatus(void);
		
	private:
	
		// PCA9536 Device I2C Address
		uint8_t PCA9536_I2C_ADDRESS;

		uint8_t pin_state;
};

#endif
