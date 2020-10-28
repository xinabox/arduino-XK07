/* 
    This is an example for the CS11 and SN01

    Steps to follow:
        1. Make sure you have soldered the connections on the
           rear of the SN01 to pass the UART onto xBus
        2. Apply for a token on Ublox Website
        3. Download files using the below provided line and
           replace the text between the symbols with your token
        4. Place the downloaded files on the SD for the CS11
        5. Insert your SD Card into the CS11
        6. Now we can upload the sketch
*/

// Apply here to get your unique token from Ublox -> https://www.u-blox.com/en/assistnow-service-registration-form

/*
    Get the assist file by doing this from a Mac or Linux:
    curl -o assist.ubx  "http://online-live1.services.u-blox.com/GetOnlineData.ashx?token=<your-token>;gnss=gps;datatype=eph,alm,aux,pos;filteronpos;format=aid;lat=-34.085336;lon=18.822303;pacc=5000"
    modify the GPS coordinates to an accuracy of 5000m (5km)
    move the assist.ubx to the SD card
    the assist.ubx file is valid for 2 hours
*/

#include <xCore.h>
#include <SPI.h>
#include <SD.h>

File assist;
#define R 11
const int DELAY_TIME = 2000;

void setup()
{
    Serial1.begin(9600);     // SN01 Uart
    SerialUSB.begin(115200); //Console
    while (!SerialUSB)
        ;               //Wait for console
    pinMode(R, OUTPUT); //red RGB LED

    SerialUSB.println("SN01 Time Trials");
    //SD card init
    SerialUSB.print("Initializing SD card...");

    if (!SD.begin(3))
    {
        SerialUSB.println("initialization failed!");
        while (1);
    }

    SerialUSB.println("initialization done.");
}
void loop()
{
    // Ready the GPS
    delay(1000); // Give the SN01 a second to start

    SerialUSB.println("Cleaning out GPS buffer");

    while (Serial1.available() > 0)
    {
        byte b = Serial1.read();
    }

    SerialUSB.println("GPS buffer empty. Now uploading assist file.");
    assist = SD.open("assist.ubx");

    if (assist)
    {
        int i = 0;
        while (assist.available())
        {
            Serial1.write(assist.read());
            i++;
        }
        Serial1.flush();
        assist.close();
        SerialUSB.print("Assist file uploaded. ");
        SerialUSB.print(i);
        SerialUSB.println(" byte uploaded.");
    }
    else
    {
        SerialUSB.println("error opening assist.ubx");
    }
    
    SerialUSB.println("Ready!");
    digitalWrite(R, LOW);
    SerialUSB.print("Reset...");
    SerialUSB.print("start...");
    long starttime = millis(); // start timer
    delay(1000);               //just wait one second to get the inital away
    long endtime;
    bool go_on = true;
    int l = 0;
    while (go_on)
    {
        if (Serial1.available() > 0)
        {
            String data = Serial1.readStringUntil('\n');
            //SerialUSB.println(data);
            if (data.startsWith("$GPGGA"))
            {
                if (data.length() > 60)
                {
                    endtime = millis();
                    go_on = false;
                }
                else
                {
                    if (l++ > 10)
                    {
                        SerialUSB.print(".");
                        l = 0;
                    }
                }
            }
        }
    }
    SerialUSB.print("time to fix: ");
    SerialUSB.print((endtime - starttime) / 1000);
    SerialUSB.println(" seconds!");
}