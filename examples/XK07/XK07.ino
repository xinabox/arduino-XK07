   /*
    This is an example for the XinaBox XK07 Starter Kit

    The STEM Environment Monitoring kit includes xCHIPS to build both a "Satellite/ FlatSat" or "Flight Station"
    (which can be moved in the environment, including being flown on balloons or drones), and a Ground Station,
    to receive and display data.

    You can buy one on our store!
    -----> https://xinabox.cc/products/xk07
*/

#include <XinaBox_XK07.h>
#include <xCore.h>

XinaBox_XK07 xk07;

#if defined(ARDUINO_SAMD_ZERO)
#define Serial Serial1
#endif

void setup()
{

#ifdef ESP8266
    Wire.setClockStretchLimit(12000);
#endif
    Wire.begin();

    xk07.begin();
}

void loop()
{
    xk07.loop();
}
