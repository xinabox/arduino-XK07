#include <XinaBox_XK07.h>

XinaBox_XK07 xk07;

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  if(xk07.setupWifi())
  {
    Serial.println("wifi setup");
  }

  if(xk07.downloadAlmanac())
  {
    Serial.println("downloaded almanac");
  }

  if(xk07.setupRadio()) {
    Serial.println("setup radio");

    if (xk07.sendAlmanac(xk07.getAlmanac(), xk07.getAlmanacSize())) {
      Serial.println("almanac sent");
    }
  }
  
}

void loop()
{
}