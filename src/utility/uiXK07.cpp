/**
 * @file uiXK07.cpp
 * @support (support@xinabox.cc)
 * @brief Class for displaying information to OD01 on XK07 Flight Station
 */

#include "uiXK07.h"
#include "imgOD01.h"

Adafruit_SSD1306 display = Adafruit_SSD1306(-1);

/**
 * @brief Construct a new uiXK01::uiXK07 object
 *
 */
uiXK07::uiXK07(void)
{
    //Adafruit_SSD1306 display(-1);
}

void uiXK07::begin()
{
    if (xCore.ping(0x3C))
    {
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    }
    display.clearDisplay();
    display.display();
}

/**
 * @brief Destroy the uiXK07::uiXK07 object
 *
 */
uiXK07::~uiXK07(void)
{
    // nothing to destruct
}

/**
 * @brief
 *
 */
void uiXK07::drawConfigSaved(void)
{
    //Adafruit_SSD1306 display(-1);
    //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.drawBitmap(0, 17, PassLogo, PassLogo_Width, PassLogo_Height, WHITE);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(57, 36);
    display.println(F("UPLOADED"));
    display.setCursor(57, 45);
    display.print(F("COMPLETE"));
    display.display();
}

/**
 * @brief
 *
 */
void uiXK07::drawProvisionSuccess(void)
{
    //Adafruit_SSD1306 display(-1);
    //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.drawBitmap(0, 17, PassLogo, PassLogo_Width, PassLogo_Height, WHITE);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(57, 36);
    display.println(F("UPLOADED"));
    display.setCursor(57, 45);
    display.print(F("COMPLETE"));
    display.display();
}

/**
 * @brief
 *
 */
void uiXK07::drawWiFiConnected(void)
{
    if (xCore.ping(0x3C))
    {
        //Adafruit_SSD1306 display(-1);
        //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
        display.clearDisplay();
        display.drawBitmap(0, 17, PassLogo, PassLogo_Width, PassLogo_Height, WHITE);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(57, 36);
        display.println(F("UPLOADED"));
        display.setCursor(57, 45);
        display.print(F("COMPLETE"));
        display.display();
    }
}

/**
 * @brief
 *
 */
void uiXK07::drawXinaBoxLogo(void)
{
    if (xCore.ping(0x3C))
    {
        //Adafruit_SSD1306 display(-1);
        //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE); // Draw white text
        display.setCursor(29, 0);    // Start at top-left corner
        display.println(F("XinaBox XK07"));
        display.setCursor(16, 9); // Start at top-left corner
        display.print(F("FIRMWARE : "));
        if (firmwareVersion[0] == '/0')
        {
            display.print("?.?.?");
        }
        else
        {
            display.print(firmwareVersion);
        }
        display.drawBitmap(0, 14, xinaboxLogo, xinaboxLogo_Width, xinaboxLogo_Height, WHITE);
        display.display();
    }
}

/**
 * @brief
 *
 */
void uiXK07::drawLogoFS(void)
{
    if (xCore.ping(0x3C))
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE); // Draw white text
        display.setCursor(26, 0);    // Start at top-left corner
        display.print(F("Flight Station"));
        display.drawBitmap(40, 16, fsLogo, fsLogo_Width, fsLogo_Height, WHITE);
        display.display();
    }
}

/**
 * @brief
 *
 */
void uiXK07::drawLogoGS(void)
{
    if (xCore.ping(0x3C))
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE); // Draw white text
        display.setCursor(26, 0);    // Start at top-left corner
        display.print(F("Ground Station"));
        display.drawBitmap(40, 16, gsLogo, gsLogo_Width, gsLogo_Height, WHITE);
        display.display();
    }
}

/**
 * @brief
 *
 * @param _version
 */
void uiXK07::setFirmwareVersion(String _firmwareVersion)
{
    firmwareVersion = _firmwareVersion;
}

/**
 * @brief
 *
 */
void uiXK07::drawConnectingWiFi(void)
{
    //Adafruit_SSD1306 display(-1);
    //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();
    display.clearDisplay();

    display.drawBitmap(0, 17, connectingWiFi1, connectingWiFi_Width, connectingWiFi_Height, WHITE);
    delay(10);
    display.drawBitmap(0, 17, connectingWiFi2, connectingWiFi_Width, connectingWiFi_Height, WHITE);
    delay(10);
    display.display();
    delay(100);

    display.drawBitmap(0, 17, connectingWiFi3, connectingWiFi_Width, connectingWiFi_Height, WHITE);
    delay(10);
    display.display();
    delay(100);

    display.drawBitmap(0, 17, connectingWiFi4, connectingWiFi_Width, connectingWiFi_Height, WHITE);
    delay(10);
    display.display();
    delay(100);

    display.drawBitmap(0, 17, connectingWiFi5, connectingWiFi_Width, connectingWiFi_Height, WHITE);
    delay(10);
    display.display();
    delay(100);
}

/**
 * @brief
 *
 * @param _countDown
 */
void uiXK07::drawConnectingWiFi(uint8_t _countDown)
{
    //Adafruit_SSD1306 display(-1);
    //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    //display.display();
    //display.clearDisplay();

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(40, 0);
    display.print(F("T-"));
    display.print(9 - _countDown);
    display.print(F("s"));

    display.drawBitmap(0, 17, connectingWiFi2, connectingWiFi_Width, connectingWiFi_Height, WHITE);
    delay(10);
    display.display();
    delay(115);

    display.drawBitmap(0, 17, connectingWiFi3, connectingWiFi_Width, connectingWiFi_Height, WHITE);
    delay(10);
    display.display();
    delay(115);

    display.drawBitmap(0, 17, connectingWiFi4, connectingWiFi_Width, connectingWiFi_Height, WHITE);
    delay(10);
    display.display();
    delay(115);

    display.drawBitmap(0, 17, connectingWiFi5, connectingWiFi_Width, connectingWiFi_Height, WHITE);
    delay(10);
    display.display();
    delay(115);
}

/**
 * @brief
 *
 */
void uiXK07::drawPixel(int16_t, int16_t, uint16_t)
{
    //display.drawPixel(x, y, z);
}

void uiXK07::clearScreen()
{
    display.clearDisplay();
}
void uiXK07::iconAndString(const uint8_t *bitmap, const char *str, int x, int y, int x1, int y1)
{
    if (xCore.ping(0x3C))
    {
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println((str));
        display.drawBitmap(x1, y1, bitmap, x, y, WHITE);
        display.display();
    }
}

void uiXK07::connectingToWifi(const char *str)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.clearDisplay();
        display.setTextSize(1);
        iconAndString(wifiSolid1, str);
        delay(150);
        iconAndString(wifiSolid2, str);
        delay(150);
        iconAndString(wifiSolid3, str);
        delay(150);
    }
}

void uiXK07::wifiPass(const char *str)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.clearDisplay();
        display.setTextSize(2);
        iconAndString(wifiSolid3, str);
        delay(1500);
    }
}
void uiXK07::wifiFail(const char *str)
{
    if (xCore.ping(0x3C))
    {
        display.clearDisplay();
        display.setTextSize(2);
        display.invertDisplay(true);
        iconAndString(wifiSolid3, str);
        delay(1500);
    }
}

void uiXK07::loading(const char *str)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.clearDisplay();
        display.setTextSize(1);
        switch (cnt)
        {
        case 0:
            iconAndString(loading1, str, 96, 48, 18, 16);
            break;
        case 1:
            iconAndString(loading2, str, 96, 48, 18, 16);
            break;
        case 2:
            iconAndString(loading3, str, 96, 48, 18, 16);
            break;
        case 3:
            iconAndString(loading4, str, 96, 48, 18, 16);
            break;
        case 4:
            iconAndString(loading5, str, 96, 48, 18, 16);
            break;
        case 5:
            iconAndString(loading6, str, 96, 48, 18, 16);
            break;
        case 6:
            iconAndString(loading7, str, 96, 48, 18, 16);
            break;
        case 7:
            iconAndString(loading8, str, 96, 48, 18, 16);
            break;
        }
        cnt++;
        if (cnt == 7)
            cnt = 0;
    }
}

void uiXK07::provFail(const char *str)
{
    if (xCore.ping(0x3C))
    {
        display.clearDisplay();
        display.setTextSize(2);
        display.invertDisplay(true);
        iconAndString(loading8, str, 96, 48, 18, 16);
        delay(1500);
    }
}

void uiXK07::provPass(const char *str)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.clearDisplay();
        display.setTextSize(2);
        iconAndString(loading8, str, 96, 48, 18, 16);
        delay(1500);
    }
}

void uiXK07::fsPass(const char *str)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.clearDisplay();
        display.setTextSize(2);
        iconAndString(broadcastTower2, str, 98, 48, 18, 16);
        delay(1500);
    }
}

void uiXK07::fsFail(const char *str)
{
    if (xCore.ping(0x3C))
    {
        display.clearDisplay();
        display.setTextSize(2);
        display.invertDisplay(true);
        iconAndString(broadcastTower2, str, 98, 48, 18, 16);
        delay(1500);
    }
}

void uiXK07::fsConnecting(const char *str)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.clearDisplay();
        display.setTextSize(1);
        iconAndString(broadcastTower0, str, 98, 48, 18, 16);
        iconAndString(broadcastTower1, str, 98, 48, 18, 16);
        iconAndString(broadcastTower2, str, 98, 48, 18, 16);
    }
}

void uiXK07::almDownloading(const char *str)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.clearDisplay();
        display.setTextSize(1);
        iconAndString(calender, str, 98, 48, 18, 16);
        delay(1000);
    }
}

void uiXK07::almDownloaded(const char *str)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.clearDisplay();
        display.setTextSize(1);
        iconAndString(calender, str, 98, 48, 15, 16);
        delay(1500);
    }
}

void uiXK07::almDownloadedFail(const char *str)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(true);
        display.clearDisplay();
        display.setTextSize(1);
        iconAndString(calender, str, 98, 48, 15, 16);
        delay(1500);
    }
}

void uiXK07::info(String time, String date, String rssi, String batPercentage)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(0, 0);
        //display.drawBitmap(0, 0, clockIcon, 16, 16, WHITE);
        display.println(time);
        display.setCursor(0, 8);
        display.println(date);
        //display.drawBitmap(75, 0, dayCalenIcon, 14, 16, WHITE);
        //display.drawBitmap(48, 0, rssiIcon, 17, 15, WHITE);
        display.setCursor(66, 0);
        display.println("RSSI");
        display.setCursor(62, 8);
        display.println(rssi);
        if (batPercentage != "100")
        {
            display.setCursor(103, 4);
        }
        else
        {
            display.setCursor(98, 4);
        }
        display.println(batPercentage);
        display.drawBitmap(97, 0, batIcon, 32, 25, WHITE);
        display.setCursor(0, 56);
        display.println("XinaBox");
        display.setCursor(55, 56);
        display.println(XK07_FW_VERSION);
        display.setCursor(104, 56);
        display.println("XK07");
        display.display();
    }
}

void uiXK07::sn01(uint8_t status, String lat, String lon, String alt)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        if (status == 0)
        {
            double _lat = atof(lat.c_str());
            if ((_lat < 0) && (_lat > -10))
            {
                display.setCursor(32, 24);
                display.println("Lat:   " + lat);
            }
            else if ((_lat < -10) && (_lat > -100))
            {
                display.setCursor(32, 24);
                display.println("Lat:  " + lat);
            }
            else if (_lat <= -100)
            {
                display.setCursor(32, 24);
                display.println("Lat: " + lat);
            }
            else if ((_lat >= 0) && (_lat < 10))
            {
                display.setCursor(32, 24);
                display.println("Lat:    " + lat);
            }
            else if ((_lat >= 10) && (_lat < 100))
            {
                display.setCursor(32, 24);
                display.println("Lat:   " + lat);
            }
            else if (_lat >= 100)
            {
                display.setCursor(32, 24);
                display.println("Lat:  " + lat);
            }


            double _lon = atof(lon.c_str());
            if ((_lon < 0) && (_lon > -10))
            {
                display.setCursor(32, 32);
                display.println("Lon:   " + lon);
            }
            else if ((_lon < -10) && (_lon > -100))
            {
                display.setCursor(32, 32);
                display.println("Lon:  " + lon);
            }
            else if (_lon <= -100)
            {
                display.setCursor(32, 32);
                display.println("Lon: " + lon);
            }
            else if ((_lon >= 0) && (_lon < 10))
            {
                display.setCursor(32, 32);
                display.println("Lon:    " + lon);
            }
            else if ((_lon >= 10) && (_lon < 100))
            {
                display.setCursor(32, 32);
                display.println("Lon:   " + lon);
            }
            else if (_lon >= 100)
            {
                display.setCursor(32, 32);
                display.println("Lon:  " + lon);
            }

            int32_t _alt = atof(alt.c_str());
            if ((_alt < 0) && (_alt > -10))
            {
                display.setCursor(32, 40);
                display.println("Alt:   " + alt);
            }
            else if ((_alt < -10) && (_alt > -100))
            {
                display.setCursor(32, 40);
                display.println("Alt:  " + alt);
            }
            else if (_alt <= -100)
            {
                display.setCursor(32, 40);
                display.println("Alt: " + alt);
            }
            else if ((_alt >= 0) && (_alt < 10))
            {
                display.setCursor(32, 40);
                display.println("Alt:    " + alt);
            }
            else if ((_alt >= 10) && (_alt < 100))
            {
                display.setCursor(32, 40);
                display.println("Alt:   " + alt);
            }
            else if (_alt >= 100)
            {
                display.setCursor(32, 40);
                display.println("Alt:  " + alt);
            }

        }
        else if (status == 1)
        {
            display.setCursor(49, 32);
            display.println("SN01 No Fix");
        }
        else
        {
            display.setCursor(35, 32);
            display.println("SN01 undetected");
        }
        display.drawBitmap(0, 20, markerIcon, 30, 27, WHITE);
        display.display();
    }
}

void uiXK07::si01(bool status, String x, String y, String z)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.setTextSize(1);
        if (status)
        {
            display.setCursor(40, 24);
            display.println("Roll   : " + x);
            display.setCursor(40, 32);
            display.println("Pitch  : " + y);
            display.setCursor(40, 40);
            display.println("G-Force: " + z);
        }
        else
        {
            display.setCursor(37, 32);
            display.println("SI01 undetected");
        }
        display.drawBitmap(0, 22, tachIcon, 30, 27, WHITE);
        display.display();
    }
}

void uiXK07::sw01(bool status, String temp, String hum, String pres)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.setTextSize(1);
        if (status)
        {
            display.setCursor(48, 24);
            display.println(temp + "   *C");
            display.setCursor(48, 32);
            display.println(hum + "      %RH");
            display.setCursor(48, 40);
            display.println(pres + " hPa");
        }
        else
        {
            display.setCursor(37, 32);
            display.println("SW01 undetected");
        }
        display.drawBitmap(0, 23, weatherIcon, 30, 24, WHITE);
        display.display();
    }
}

void uiXK07::sg33(bool status, String co2, String voc)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.setTextSize(1);
        if (status)
        {
            display.setCursor(40, 27);
            display.println("CO2 : " + co2);
            display.setCursor(40, 38);
            display.println("VOC : " + voc);
        }
        else
        {
            display.setCursor(37, 32);
            display.println("SG33 undetected");
        }
        display.drawBitmap(0, 23, smogIcon, 30, 24, WHITE);
        display.display();
    }
}

void uiXK07::sl01(bool status, String lux, String uvi)
{
    if (xCore.ping(0x3C))
    {
        display.invertDisplay(false);
        display.setTextSize(1);
        if (status)
        {
            display.setCursor(40, 27);
            display.println("Lux : " + lux);
            display.setCursor(40, 38);
            display.println("UVI : " + uvi);
        }
        else
        {
            display.setCursor(37, 32);
            display.println("SL01 undetected");
        }
        display.drawBitmap(0, 21, sunIcon, 30, 30, WHITE);
        display.display();
    }
}

void uiXK07::sdPass()
{
    if (xCore.ping(0x3C))
    {
        display.clearDisplay();
        display.setTextSize(2);
        display.invertDisplay(false);
        iconAndString(sdIcon, "  SD PASS", 90, 44, 18, 18);
        //delay(1500);
    }
}

void uiXK07::sdFail()
{
    if (xCore.ping(0x3C))
    {
        display.clearDisplay();
        display.setTextSize(2);
        display.invertDisplay(true);
        iconAndString(sdIcon, "  SD FAIL", 90, 44, 18, 18);
        delay(1500);
    }
}

void uiXK07::uifs(bool sn01, bool sg33, bool si01, bool sw01, bool sl01, bool rl0x, bool pb04)
{
    if (xCore.ping(0x3C))
    {
        fsState = "";
        if (sn01)
        {
            fsState += "SN01 pass\n";
        }
        else
        {
            fsState += "SN01 undetected\n";
        }

        if (sg33)
        {
            fsState += "SG33 pass\n";
        }
        else
        {
            fsState += "SG33 undetected\n";
        }

        if (si01)
        {
            fsState += "SI01 pass\n";
        }
        else
        {
            fsState += "SI01 undetected\n";
        }

        if (sw01)
        {
            fsState += "SW01 pass\n";
        }
        else
        {
            fsState += "SW01 undetected\n";
        }
        if (sl01)
        {
            fsState += "SL01 pass\n";
        }
        else
        {
            fsState += "SL01 undetected\n";
        }
        if (rl0x)
        {
            fsState += "RL0x pass\n";
        }
        else
        {
            fsState += "RL0x undetected\n";
        }

        if (pb04)
        {
            fsState += "PB04 pass\n";
        }
        else
        {
            fsState += "PB04 undetected\n";
        }
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.setTextColor(WHITE);
        display.invertDisplay(false);
        display.print(fsState);
        display.display();
        //Watchdog.reset();
    }
}
uiXK07 ui = uiXK07();