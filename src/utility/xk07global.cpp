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
        digitalWrite(LED_RED, !digitalRead(LED_RED));
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

    digitalWrite(LED_BUILTIN, HIGH);
}

void xk07_global::greenDebug()
{
    watchdog();
    if (!setup)
    {
        pinModeOutput();
        setup = true;
    }
    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_GREEN, HIGH);
}

void xk07_global::pinModeOutput()
{
    watchdog();
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BUILTIN, LOW);
}

void xk07_global::rgbOff()
{
    watchdog();
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BUILTIN, LOW);
}

void xk07_global::watchdog()
{
    #if defined(ARDUINO_SAMD_ZERO)
    Watchdog.reset();
    #endif
    return;
}

xk07_global global = xk07_global();