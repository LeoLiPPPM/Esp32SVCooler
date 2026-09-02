#include <Arduino.h>

// relay module is conntected to ESP32 GIOP16
#define RELAY_DIO 16

int relayState = LOW;

void turnOnOffRelay(bool on)
{
    if (on)
    {
        if (relayState != HIGH)
        {
            digitalWrite(RELAY_DIO, HIGH);
            relayState = HIGH;
        }
    }
    else
    {
        if (relayState != LOW)
        {
            digitalWrite(RELAY_DIO, LOW);
            relayState = LOW;
        }
    }
}