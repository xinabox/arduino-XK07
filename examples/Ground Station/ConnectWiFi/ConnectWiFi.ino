#include "XinaBox_XK07.h"

XinaBox_XK07 xk07;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  String ssid = "Mistier";
  String password = "cloggednozzle9292";

  if(xk07.setupWifi(ssid, password))        
  {
    Serial.println("Connected to WiFi");
    if(xk07.checkInternet())
    {
      Serial.println("Connected to internet");
    }
  }
}

void loop()
{
  Serial.println();
  Serial.print("rssi: "); Serial.println(xk07.getWifiSignalStrength());\

  delay(1000);
}
