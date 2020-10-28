#include "XinaBox_XK07.h"

XinaBox_XK07 xk07;

void setup()
{
  Wire.begin();
  
  if(xk07.setupProvisioning()) {
    Serial.println("setup provisioning");
    if(xk07.getData()) {
      Serial.println("Got Data");
      
      Serial.print("unit name: "); Serial.println(xk07.getUnitName());
      Serial.print("SSID: "); Serial.println(xk07.getSSID());
      Serial.print("password: "); Serial.println(xk07.getPassword());
      
    }
  }

  
}

void loop()
{}
