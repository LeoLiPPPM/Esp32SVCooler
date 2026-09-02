#include <OneWire.h>
#include <DallasTemperature.h>

// Absolute minimal temperature, most it means the temperature sensor has reading error.
#define ABS_MIN_TEMP_C -127
#define ABS_MIN_TEMP_F -196.6
#define MAX_READ_TEMPERATURE_RETRIES 10
#define ON 2
#define NEUT 1
#define OFF 0

// temperature sensor is conntected to ESP32 GIOP17
#define ONE_WIRE_BUS 17

const float upperTempC = 8;   // will count as out of bound if we exceed this temperature
const float maximalTempC = 7; // will turn on the relay if we reach this temperature
const float minimalTempC = 5; // will turn off the relay if we reach this temperature

class TempSensor
{
private:
    int BUS;
    DallasTemperature sensors;

public:
    void init(int bus)
    {
        BUS = bus;
        // Setup a oneWire instance to communicate with any OneWire devices
        OneWire oneWire(BUS);
        // Pass our oneWire reference to Dallas Temperature sensor
        DallasTemperature sensor(&oneWire);
        sensors = sensor;
        // Start up the temperature sensor library.
        sensors.begin();
    }
    void readTemperaturesWithRetries(float *tempC, float *tempF, int maxRetries)
    {
        for (int i = 0; i < maxRetries; ++i)
        {
            // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
            sensors.requestTemperatures();
            *tempC = sensors.getTempCByIndex(0);
            *tempF = sensors.getTempFByIndex(0);
            if (*tempC > ABS_MIN_TEMP_C && *tempF > ABS_MIN_TEMP_F)
            {
                return;
            }
            // delay 1 seconds before retry.
            delay(1000);
        }
    }
};


int simpleDetermineRelayState(int tempC){
    if(tempC > maximalTempC){
        return ON;
    }
    else if(tempC < maximalTempC){
        return OFF;
    }
    else{
        return NEUT;
    }
}