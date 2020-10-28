#include "XinaBox_XK07.h"

XinaBox_XK07 xk07;

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  if(xk07.setupWifi("Mistier", "cloggednozzle9292"))
  {
    Serial.println("setupWifi");
    if(xk07.downloadAlmanac())
    {
      Serial.println("download Almanac");
      int almSize = xk07.getAlmanacSize();
      Serial.println("got almanac size");
      uint8_t *almanac = xk07.getAlmanac();
      Serial.println("got almanac");
      Serial.println();
      Serial.println("Alm Size: "); Serial.print(almSize);
      Serial.println();
      Serial.write(almanac, almSize);  
    }
  }
}

void loop()
{}
