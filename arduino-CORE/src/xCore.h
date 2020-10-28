/********************************************************
 	This is a library for the XinaBox Core Functions

 	Core Functions include:
 	-	I2C Communication
 	-	Inter device communication

*********************************************************/

#ifndef	xCore_h
#define xCore_h

#pragma once

#include "inttypes.h"
#include <Arduino.h>
#include "xVersion.h"
#include "Wire.h"

#if not defined(CORE_RED) || defined(CORE_GREEN) ||defined(CORE_BLUE)
#ifdef ESP8266
#define CORE_RED 12
#define CORE_GREEN 13
#define CORE_BLUE 5
#elif ESP32
#define CORE_RED 25
#define CORE_GREEN 26
#define CORE_BLUE 27
#elif ARDUINO_SAMD_ZERO
#define CORE_RED 11
#define CORE_GREEN 12
#define CORE_BLUE 13
#endif
#endif


/*********************************************************/
class xCoreClass{
	private:
		int a,b,c,d;
	public:
		xCoreClass(){}
		uint16_t	request16(byte device);
		void 		write(byte value);
		void 		write1(byte device, byte value);
		void 		write8(byte device, byte reg, byte value);
		void 		write16(byte device, byte reg, uint16_t value);
		uint8_t		readStream(byte device);
		uint8_t 	read8(byte device, byte reg);
		uint16_t	read16(byte device, byte reg);
		int16_t 	readS16(byte device,byte reg);
		uint16_t  	read16_LE(byte device, byte reg);
		int16_t   	readS16_LE(byte device, byte reg);
		uint32_t 	read24(byte device, byte reg);
		bool		ping(byte device);
};

extern xCoreClass xCore;
// end of header
#endif
