#include "xk07global.h"
#include <xCore.h>

void xk07_global::redDebug()
{
    //watchdog();
    if (!setup)
    {
        pinModeOutput();
        setup = true;
    }
    while (1)
    {
        //watchdog();
        digitalWrite(CORE_RED, !digitalRead(CORE_RED));
        delay(500);
    }
}

void xk07_global::blueDebug()
{
    watchdog();
    if (!setup)
    {
        pinModeOutput();
        setup = true;
    }

    digitalWrite(CORE_BLUE, HIGH);
}

void xk07_global::greenDebug()
{
    watchdog();
    if (!setup)
    {
        pinModeOutput();
        setup = true;
    }
    pinMode(CORE_GREEN, OUTPUT);
    digitalWrite(CORE_GREEN, HIGH);
}

void xk07_global::pinModeOutput()
{
    watchdog();
    pinMode(CORE_RED, OUTPUT);
    pinMode(CORE_GREEN, OUTPUT);
    pinMode(CORE_BLUE, OUTPUT);
    digitalWrite(CORE_RED, LOW);
    digitalWrite(CORE_GREEN, LOW);
    digitalWrite(CORE_BLUE, LOW);
}

void xk07_global::rgbOff()
{
    watchdog();
    digitalWrite(CORE_RED, LOW);
    digitalWrite(CORE_GREEN, LOW);
    digitalWrite(CORE_BLUE, LOW);
}

void xk07_global::watchdog()
{
    #if defined(ARDUINO_SAMD_ZERO)
    Watchdog.reset();
    #endif
    return;
}

xk07_global global = xk07_global();