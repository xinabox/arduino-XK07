/*************************************************************
	This is an examples for the OC01
	Burn Wire Module
	
	You can buy one on our store!
	-----> https://xinabox.cc/products/OC01/
	
	This example in structs the OC01 to turn its output port
	on and off
	
	The sensor communicates over the I2C Bus.
	
*************************************************************/

/********************* Library Includes *********************/
#include <xOC01.h>
#include <xCore.h> 

const int DELAY_TIME = 500;

xOC01 OC01;

/*********************** Sketch Code ************************/
void setup() {
	// Start the Serial Monitor
	Serial.begin(115200);
	
	// Set the I2C Pins for CW01
	#ifdef ESP8266
	  Wire.pins(2, 14);
	  Wire.setClockStretchLimit(15000);
	#endif
	
	// Start the I2C Communication
	Wire.begin();
	
	// Start the OC01 port expander
	OC01.begin();

	Serial.println("OC01 Test");
}

void loop() {
	// Switch OUT0 On
	OC01.digitalWrite(OUT0, HIGH);
	delay(DELAY_TIME);
	
	// Switch OUT1 On
	OC01.digitalWrite(OUT1, HIGH);
	delay(DELAY_TIME);
	
	// Switch OUT2 On
	OC01.digitalWrite(OUT2, HIGH);
	delay(DELAY_TIME);
	
	// Switch OUT3 On
	OC01.digitalWrite(OUT3, HIGH);
	delay(DELAY_TIME);
	
	// Switch OUT0 off
	OC01.digitalWrite(OUT0, LOW);
	delay(DELAY_TIME);
	
	// Switch OUT1 off
	OC01.digitalWrite(OUT1, LOW);
	delay(DELAY_TIME);
	
	// Switch OUT2 off
	OC01.digitalWrite(OUT2, LOW);
	delay(DELAY_TIME);
	
	// Switch OUT3 off
	OC01.digitalWrite(OUT3, LOW);
	delay(DELAY_TIME);
}
