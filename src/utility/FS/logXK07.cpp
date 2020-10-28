/**
 * @file logXK07.cpp
 * @support (support@xinabox.cc)
 * @brief Class for handling data logging on XK07 Flight Station
 */
#include "logXK07.h"

#if defined(ARDUINO_SAMD_ZERO)

/**
 * @brief Construct a new logXK07::logXK07 object
 *
 */
logXK07::logXK07(void) {}

/**
 * @brief Destroy the logXK07::logXK07 object
 *
 */
logXK07::~logXK07(void) {}

/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t logXK07::begin(void)
{
    global.watchdog();
    initSensors();

    if (initSD()) // Initialize the SD card
    {
        if (createLogFolder()) // create a folder XK07DATA/ on the sd card if one isn't already there
        {
            if (createLogFile()) // create a log file in that folder. The name is DATA<i>.csv where i is incremented each time.
            {
                global.watchdog();
                if (writeHeaders()) // write all the headers to the first line of the file
                {
                    return 0xFF;
                }
                else
                {
                    return 0x23;
                }
            }
            else
            {
                return 0x22;
            }
        }
        else
        {
            return 0x21;
        }
    }
    else
    {
        return 0x20;
    }
}

/**
 * @brief
 *
 */
void logXK07::initSensors(void)
{
    global.watchdog();
    xchipi2caddr addr;
    if (xCore.ping(addr.SG33_ADDRESS))
    {
        //xSG33 SG33 = xSG33();
        if (!SG33.begin())
        {
            debugPrint("SG33 FAIL START");
        }
    }
}

/**
 * @brief
 *
 * @param _enable
 */
void logXK07::enableExcelLogOutput(bool _enable)
{
    global.watchdog();
    ENABLE_EXCEL_LOG = _enable;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool logXK07::initSD(void)
{
    global.watchdog();
    if (!SD.begin(CS11_SD_CS_PIN))
    {
        ui.sdFail();
        SD_CARD_INITIALIZED = false;
        debugPrint("[SD CARD] Card failed, or not present");
        writeInfo("[SD CARD] Card failed or not present", NORMAL);
    }
    else
    {
        ui.sdPass();
        global.watchdog();
        SD_CARD_INITIALIZED = true;
        debugPrint("SD Card initialized.");
        writeInfo("[SD CARD] SD Card Initialized", NORMAL);
    }
}

/**
 * @brief
 *
 * @param info
 * @param mode
 * @return true
 * @return false
 */
bool logXK07::writeInfo(char info[], bool mode)
{
    global.watchdog();
    if (mode || !LOW_USE_MODE) // this method only writes to SD card if it is important (mode == 1) or if it's not LOW_USE_MODE
    {
        InfoFile = SD.open(InfoFileName, FILE_WRITE);
        if (SD_CARD_INITIALIZED)
        {
            if (InfoFile)
            {
                InfoFile.println(info); // write the info to the textfile
            }
            else
                return false;
        }
        else
            return false;

        InfoFile.close();
    }
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool logXK07::createLogFile(void)
{
    global.watchdog();
    createLogFolder(); // first attempt to create a log folder if one isn't already there

    DataFileName = newFilename(); // create a new filename DATA<i>.csv where i depends on how many files are already present

    DataFile = SD.open(DataFileName, FILE_WRITE); // create the new file
    global.watchdog();

    if (DataFile)
    {
        debugPrint("Created Data File");
        writeInfo("Created Data File", NORMAL);
        DataFile.close();
        return true;
    }
    else
    {
        DataFile.close();
        return false;
    }
}

/**
 * @brief
 *
 * @param filename
 * @return true
 * @return false
 */
bool logXK07::createLogFile(char filename[])
{
    global.watchdog();
    File DataFile = SD.open(filename, FILE_WRITE);

    if (DataFile)
    {
        debugPrint("Created Data File");
        writeInfo("Created Data File", NORMAL);
        DataFile.close();
        return true;
    }
    else
    {
        DataFile.close();
        return false;
    }
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool logXK07::createLogFolder(void)
{
    global.watchdog();
    if (SD.mkdir(DataFolder))
    {
        global.watchdog();
        debugPrint("Folder Created");
        writeInfo("[SD CARD] Folder Created", NORMAL);
        global.watchdog();
        return true;
    }
    else
    {
        debugPrint("Could not create Folder");
        writeInfo("[SD CARD] Could not create Folder", NORMAL);
        return false;
    }
}

/**
 * @brief
 *
 * @param folderName
 * @return true
 * @return false
 */
bool logXK07::createLogFolder(char folderName[])
{
    global.watchdog();
    if (sizeof(folderName) > 12) //TODO - could be more safe, taking into account 8.3 format
    {
        debugPrint("File name too long");

        return false;
    }

    if (SD.mkdir(folderName))
    {
        debugPrint("Folder Created");
        writeInfo("[SD CARD] Folder Created", NORMAL);
        return true;
    }
    else
    {
        debugPrint("Could not create Folder");
        writeInfo("[SD CARD] Could not create Folder", NORMAL);
        return false;
    }
}

/**
 * @brief
 *
 * @return uint16_t
 */
uint16_t logXK07::CountDataFiles(void)
{
    global.watchdog();
    uint16_t count = 0;
    if (SD_CARD_INITIALIZED)
    {
        String tempfile;
        tempfile = DataFolder + "/DATA" + String(count) + ".CSV";
        while (SD.exists(tempfile))
        {
            global.watchdog();
            count++;
            tempfile = DataFolder + "/DATA" + String(count) + ".CSV";
        }
    }
    return count;
}

/**
 * @brief
 *
 * @return String
 */
String logXK07::newFilename(void)
{
    global.watchdog();
    String newFilename = DataFolder + "/DATA" + String(CountDataFiles()) + ".CSV";
    return newFilename;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool logXK07::writeHeaders(void)
{
    global.watchdog();
    String header;
    for (size_t i = 0; i < HeaderArray_length; i++)
    {
        if (i == HeaderArray_length - 1)
        {
            header += headers[i][0] + " " + headers[i][1]; // write the header name and the units
        }
        else
        {
            header += headers[i][0] + " " + headers[i][1] + ","; // if the end of the line hasn't been reached, add a comma
        }
    }

    if (SD_CARD_INITIALIZED)
    {
        DataFile = SD.open(DataFileName, FILE_WRITE);
        if (DataFile)
        {
            DataFile.println(header);
            DataFile.close();
            return true;
        }
        else
        {
            DataFile.close();
            return false;
        }
    }
    else
        return false;
}

/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t logXK07::logAllSensors(int rssi, int id)
{
    global.watchdog();
    log_index = 0;
    bool allSensorsLogged{true};
    DataFile = SD.open(DataFileName, FILE_WRITE);

    LogRuntime();

    if (!LogSN01())
    {
        allSensorsLogged = false;
        debugPrint("SN01 not logged");
    }
    if (!LogSL01())
    {
        allSensorsLogged = false;
        debugPrint("SL01 not logged");
    }
    if (!LogSW01())
    {
        allSensorsLogged = false;
        debugPrint("SW01 not logged");
    }
    if (!LogSG33())
    {
        allSensorsLogged = false;
        debugPrint("SG33 not logged");
    }
    if (!LogPB04())
    {
        allSensorsLogged = false;
        debugPrint("PB04 not logged");
    }
    if (!LogSI01())
    {
        allSensorsLogged = false;
        debugPrint("SI01 not logged");
    }
    if (!LogRL0X(rssi, id))
    {
        allSensorsLogged = false;
        debugPrint("RL0X not logged");
    }
    if (ENABLE_EXCEL_LOG)
    {
        Serial1.println();
    }
    DataFile.println();
    DataFile.close();

    return allSensorsLogged;
}

/**
 * @brief logs float data
 *
 * @param index of data to be logged
 * @param data to be logged
 */
void logXK07::logData(int index, float data)
{
    global.watchdog();
    // Log To SD Card
    if (SD_CARD_INITIALIZED)
    {
        if (DataFile)
        {
            DataFile.print(data);
            DataFile.print(",");
            if (ENABLE_EXCEL_LOG)
            {
                Serial1.print(data);
                Serial1.print(",");
            }
        }
    }
}

/**
 * @brief logs interger data
 *
 * @param index of data to be logged
 * @param data to be logged
 */
void logXK07::logData(int index, int data)
{
    global.watchdog();
    // Log To SD Card
    if (SD_CARD_INITIALIZED)
    {
        if (DataFile)
        {
            DataFile.print(data);
            DataFile.print(",");
            if (ENABLE_EXCEL_LOG)
            {
                Serial1.print(data);
                Serial1.print(",");
            }
        }
    }
}

/**
 * @brief logs long data
 *
 * @param index of data to be logged
 * @param data to be logged
 */
void logXK07::logData(int index, long data)
{
    global.watchdog();
    // Log To SD Card
    if (SD_CARD_INITIALIZED)
    {
        if (DataFile)
        {
            DataFile.print(data);
            DataFile.print(",");
            if (ENABLE_EXCEL_LOG)
            {
                Serial1.print(data);
                Serial1.print(",");
            }
        }
    }
}

/**
 * @brief logs character data
 *
 * @param index of data to be logged
 * @param data to be logged
 */
void logXK07::logData(int index, char data[])
{
    global.watchdog();
    // Log To SD Card
    if (SD_CARD_INITIALIZED)
    {
        if (DataFile)
        {
            DataFile.print(data);
            DataFile.print(",");
            if (ENABLE_EXCEL_LOG)
            {
                Serial1.print(data);
                Serial1.print(",");
            }
        }
    }
}

/**
 * @brief logs 32bit unsigned interger data
 *
 * @param index of data to be logged
 * @param data to be logged
 */
void logXK07::logData(int index, uint32_t data)
{
    global.watchdog();
    // Log To SD Card
    if (SD_CARD_INITIALIZED)
    {
        if (DataFile)
        {
            DataFile.print(data);
            DataFile.print(",");
            if (ENABLE_EXCEL_LOG)
            {
                Serial1.print(data);
                Serial1.print(",");
            }
        }
    }
}

/**
 * @brief logs 16bit unsigned interger data
 *
 * @param index of data to be logged
 * @param data to be logged
 */
void logXK07::logData(int index, uint16_t data)
{
    global.watchdog();
    // Log To SD Card
    if (SD_CARD_INITIALIZED)
    {
        if (DataFile)
        {
            DataFile.print(data);
            DataFile.print(",");
            if (ENABLE_EXCEL_LOG)
            {
                Serial1.print(data);
                Serial1.print(",");
            }
        }
    }
}

/**
 * @brief logs string data
 *
 * @param index of data to be logged
 * @param data to be logged
 */
void logXK07::logData(int index, String data)
{
    global.watchdog();
    // Log To SD Card
    if (SD_CARD_INITIALIZED)
    {
        if (DataFile)
        {
            DataFile.print(data);
            DataFile.print(",");
            if (ENABLE_EXCEL_LOG)
            {
                Serial1.print(data);
                Serial1.print(",");
            }
        }
    }
}

/**
 * @brief
 *
 */
void logXK07::LogRuntime(void)
{
    global.watchdog();
    int allSeconds = millis() / 1000;

    int runHours = allSeconds / 3600;
    int secsRemaining = allSeconds % 3600;
    int runMinutes = secsRemaining / 60;
    int runSeconds = secsRemaining % 60;

    char buf[21];
    sprintf(buf, "%02d:%02d:%02d", runHours, runMinutes, runSeconds);
    logData(log_index++, buf);
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool logXK07::LogSN01(void)
{
    global.watchdog();
    xchipi2caddr addr;
    if (xCore.ping(addr.SN01_ADDRESS))
    {
        xSN01 SN01 = xSN01();

        if (SN01.begin())
        {
            //Serial1.println("Started SN01");
            SN01.poll();

            if ((SN01.getHDOP() < 12.0) && (SN01.getHDOP() > 0.0))
            {
                //Serial1.println("Logging SN01");
                //GPS_FIX = false;        // TODO - needs to be added after GPS stuff is working
                String date = SN01.getDate();
                String time = SN01.getTime();

                logData(log_index++, date);
                logData(log_index++, time);
                logData(log_index++, String(SN01.getLatitude(), 5));
                logData(log_index++, String(SN01.getLongitude(), 5));
                logData(log_index++, SN01.getAltitude());
                logData(log_index++, SN01.getHDOP());
                logData(log_index++, SN01.getSOG());
                if (SN01.getCOG() > 360)
                    logData(log_index++, 360);
                else
                    logData(log_index++, SN01.getCOG());
                logData(log_index++, (long)SN01.getSatelitesConnected());
                return true;
            }
            else
            {
                //Serial1.println(SN01.getHDOP());
                logData(log_index++, "");
                logData(log_index++, "");
                logData(log_index++, "");
                logData(log_index++, "");
                logData(log_index++, "");
                logData(log_index++, "");
                logData(log_index++, "");
                logData(log_index++, "");
                logData(log_index++, "");
                return false;
            }
        }
        else
        {
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            return false;
        }
    }
    else
    {
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        return false;
    }
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool logXK07::LogSL01(void)
{
    global.watchdog();
    xchipi2caddr addr;
    if ((xCore.ping(addr.SL01_ADDRESS_1) & xCore.ping(addr.SL01_ADDRESS_2) == true) || (xCore.ping(addr.SL01_ADDRESS_3)))
    {
        xSL01 SL01;
        if (SL01.begin())
        {
            SL01.poll();
            if (SL01.checkVersion() == 1)
            {
                logData(log_index++, SL01.getUVA());
                logData(log_index++, SL01.getUVB());
            }
            logData(log_index++, SL01.getUVIndex());
            logData(log_index++, SL01.getLUX());
            return true;
        }
        else
        {
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            return false;
        }
    }
    else
    {
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        //Serial1.println("[ERROR] [SW01] Alternate I2C Address is being used.");
        return false;
    }
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool logXK07::LogSW01(void)
{
    global.watchdog();
    xchipi2caddr addr;
    if (xCore.ping(addr.SW01_ADDRESS))
    {
        xSW01 SW01;
        if (SW01.begin())
        {
            SW01.poll();
            logData(log_index++, SW01.getTempC());
            logData(log_index++, SW01.getPressure());
            logData(log_index++, SW01.getHumidity());
            logData(log_index++, SW01.getQNE());
            return true;
        }
    }
    else if (xCore.ping(addr.SW01_ADDRESS_ALT))
    {
        debugPrint("[ERROR] [SW01] Alternate I2C Address is being used.");
        writeInfo("[ERROR] [SW01] Alternate I2C Address is being used.");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        return false;
    }
    else
    {
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        return false;
    }
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool logXK07::LogSG33(void)
{
    global.watchdog();
    xchipi2caddr addr;
    if (xCore.ping(addr.SG33_ADDRESS))
    {
        if (1)
        {
            unsigned long tim1 = millis();
            uint8_t tim2 = 0;

            while (!SG33.dataAvailable() && (tim2 < 13)) // wait for data but no more than 2.5 seconds
            {
                if ((millis() - tim1) > 200)
                {
                    tim2++;
                    tim1 = millis();
                }
            };
            if (SG33.getAlgorithmResults())
            {
                logData(log_index++, SG33.getTVOC());
                logData(log_index++, SG33.getCO2());
                return true;
            }
            else
            {
                logData(log_index++, "");
                logData(log_index++, "");
                return false;
            }
        }
        else
        {
            logData(log_index++, "");
            logData(log_index++, "");
            return false;
        }
    }
    else if (xCore.ping(addr.SG33_ADDRESS_ALT))
    {
        debugPrint("[SG33] Alternate I2C Address is being used.");
        writeInfo("[SG33] Alternate I2C Address is being used.");
        logData(log_index++, "");
        logData(log_index++, "");
        return false;
    }
    else
    {
        logData(log_index++, "");
        logData(log_index++, "");
        return false;
    }
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool logXK07::LogPB04(void)
{
    global.watchdog();
    xchipi2caddr addr;
    if (xCore.ping(addr.PB04_ADDRESS))
    {
        xPB04 PB04;
        PB04.begin();
        PB04.poll();
        logData(log_index++, PB04.getCurrent());
        logData(log_index++, PB04.getVoltage());
        return true;
    }
    else
    {
        logData(log_index++, "");
        logData(log_index++, "");
        return false;
    }
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool logXK07::LogSI01(void)
{
    global.watchdog();
    xchipi2caddr addr;
    if (xCore.ping(addr.SI01_ADDRESS_1) && xCore.ping(addr.SI01_ADDRESS_2))
    {
        xSI01 SI01;
        if (SI01.begin())
        {
            SI01.poll();
            logData(log_index++, SI01.getRoll());
            logData(log_index++, SI01.getPitch());
            logData(log_index++, SI01.getGForce());
            logData(log_index++, SI01.getMX());
            logData(log_index++, SI01.getMY());
            logData(log_index++, SI01.getMZ());
            logData(log_index++, SI01.getGX());
            logData(log_index++, SI01.getGY());
            logData(log_index++, SI01.getGZ());
            logData(log_index++, SI01.getAX());
            logData(log_index++, SI01.getAY());
            logData(log_index++, SI01.getAZ());
            return true;
        }
        else
        {
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            logData(log_index++, "");
            return false;
        }
    }
    else if (xCore.ping(addr.SI01_ADDRESS_1_ALT) || xCore.ping(addr.SI01_ADDRESS_2_ALT))
    {
        debugPrint("[ERROR] [SI01] Alternate I2C Address is being used.");
        writeInfo("[ERROR] [SI01] Alternate I2C Address is being used.");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        return false;
    }
    else
    {

        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        logData(log_index++, "");
        return false;
    }
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool logXK07::LogRL0X(int rssi, int id)
{
    global.watchdog();
    if (1) // TODO - add RL03.lastRssi()) or equivalent
    {
        logData(log_index++, rssi); //TODO need to add this line in at some point
        // logData(log_index++, "");
        // logData(log_index++, "");
        //logData(log_index++, id);       //TODO need to add this line in when the other RADIO stuff has been added
        return true;
    }
    else
    {
        logData(log_index++, "");
        //logData(log_index++, "");
        return false;
    }
}

/**
 * @brief
 *
 * @param _data
 */
void logXK07::debugPrint(String _data)
{
    global.watchdog();
#ifdef DEBUG_SERIAL
    SerialUSB.print("log_xk07: ");
    SerialUSB.println(_data);
#endif
}

/**
 * @brief
 *
 * @param _data
 * @param _num
 */
void logXK07::debugPrint(String _data, uint16_t _num)
{
    global.watchdog();
#ifdef DEBUG_SERIAL
    SerialUSB.print("log_xk07: ");
    SerialUSB.print(_data);
    SerialUSB.println(_num);
#endif
}

#endif