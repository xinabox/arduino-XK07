#include "XinaBox_XK07.h"
XinaBox_XK07 xk07;

void setup()
{
  SerialUSB.begin(115200);
  delay(2000);
  Wire.begin();
  
  xk07.setupSD();
}

void loop()
{
  if(xk07.logAllSensors())
  
  delay(3000);
}
