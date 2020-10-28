/**
 * @file CheckSensors.ino
 * @support (support@xinabox.cc)
 * @brief Example Sketch for checking the FS Sensors
 */

#include "XinaBox_XK07.h"
XinaBox_XK07 xk07;

void setup()
{
  SerialUSB.begin(115200);
  delay(2000);
  Wire.begin();
  
  SerialUSB.println(xk07.CheckSensors());
}

void loop()
{
}
