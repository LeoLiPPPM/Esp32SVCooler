/*************************************************************
  Smart Temperature Controller.
  by SVCooler.
*************************************************************/
#include <Arduino.h>
#include "relay.h"
#include "temperature_sensor.h"
#include "BluetoothSerial.h"

#define BUILTIN_LED 2

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#define USE_LITTLEFS true
#define USE_SPIFFS false
#define SCAN_WIFI_NETWORKS true
#define CONFIG_TIMEOUT 600000L
#define BLYNK_WM_DEBUG 4

#include <BlynkSimpleEsp32_Async_WM.h>

// init Class:
BluetoothSerial ESP_BT;
String wifi;
String w;

// Parameters for Bluetooth interface
String incoming = "";

int time_expired = 0;

bool LOAD_DEFAULT_CONFIG_DATA = false;
Blynk_WM_Configuration defaultConfig = {
    // char header[16], dummy, not used
    "NonSSL",
    // WiFi_Credentials  WiFi_Creds  [NUM_WIFI_CREDENTIALS]
    // WiFi_Creds.wifi_ssid and WiFi_Creds.wifi_pw
    "SSID1", "password1",
    "SSID2", "password2",
    // Blynk_Credentials Blynk_Creds [NUM_BLYNK_CREDENTIALS];
    // Blynk_Creds.blynk_server and Blynk_Creds.blynk_token
    "blynk.cloud", "LtqR4CuIH9ZNJPvzt3l0CkMV483vou0q",
    "blynk.cloud", "LtqR4CuIH9ZNJPvzt3l0CkMV483vou0q",
    80,
    // char board_name     [24];
    "SVCooler-1.0",
    // int  checkSum, dummy, not used
    0};

BlynkTimer timer;
TempSensor interior;
int t = 0;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);

  if (value == 1)
  {
    digitalWrite(BUILTIN_LED, HIGH);
  }
  else
  {
    digitalWrite(BUILTIN_LED, LOW);
  }
}
BLYNK_WRITE(V6)
{
  int value = param.asInt();
  if (value == 1)
  {
    time_expired = 0;
  }
}

bool bt = false;

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if (event == ESP_SPP_SRV_OPEN_EVT)
  {
    bt = true;
    Serial.println("Client Connected");
  }
  if (event == ESP_SPP_CLOSE_EVT)
  {
    bt = false;
    Serial.println("Client disconnected");
  }
}

int prev = 0;

void sendSensor()
{
  // Read temperature from sensors with retry.
  float tempC, tempF;
  interior.readTemperaturesWithRetries(&tempC, &tempF, MAX_READ_TEMPERATURE_RETRIES);
  Serial.print("current temperature: ");
  Serial.println(tempC);
  Serial.print("current relay state: ");
  Serial.println(relayState ? "HIGH" : "LOW");

  int decision = simpleDetermineRelayState(tempC);
  if (decision == ON)
  {
    turnOnOffRelay(true);
  }
  else if (decision == OFF)
  {
    turnOnOffRelay(false);
  }
  if (tempC > upperTempC)
  {
    time_expired += millis() - prev;
  }

  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 3600000.0);
  Blynk.virtualWrite(V4, tempC); // select your virtual pins accordingly
  Blynk.virtualWrite(V5, relayState);
  Blynk.virtualWrite(V7, time_expired / 60000.0);
  if (millis() - t >= 1000 && bt)
  {
    t = millis();
    Serial.println("BLUETOOTH");
    ESP_BT.println(tempC);
  }
  prev = millis();
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(BUILTIN_LED, OUTPUT);

  // Uncomment below if you want to clear existed config data on ESP32.
  // FileFS.begin(); Blynk.clearConfigData();

  // initialize digital pin RELAY_DIO as an output.
  pinMode(RELAY_DIO, OUTPUT);
  digitalWrite(RELAY_DIO, relayState);

  interior.init(ONE_WIRE_BUS);

  // Start Blynk
  Blynk.setConfigPortal("SVCooler", "LakeTahoe");
  Blynk.setConfigPortalIP(IPAddress(192, 168, 8, 1));
  Blynk.begin();

  ESP_BT.register_callback(callback);
  ESP_BT.begin("SVCooler"); // Name of your Bluetooth interface -> will show up on your phone

  timer.setInterval(1000L, sendSensor);
  prev = millis();
}

/*
bool connected = false;

char buf1[100], buf2[100];
*/

void loop()
{
  /*
if (!connected || !Blynk.connected() || ESP_BT.available())
{
  wifi = "";
  w = "";
  // -------------------- Receive Bluetooth signal ----------------------
  while (ESP_BT.available())
  {
    char character = ESP_BT.read();
    if (character == '\\')
    {
      while (ESP_BT.available())
      {
        char character = ESP_BT.read();
        if (character == '\\')
        {
          wifi.toCharArray(buf1, 100);
          w.toCharArray(buf2, 100);
          Serial.println(buf1);
          Serial.println(buf2);
          Blynk.begin();
          connected = true;
          ESP_BT.println(0);
          t = millis();
          break;
        }
        w = w + character;
      }
      break;
    }
    wifi = wifi + character;
  }
   if(connected && !Blynk.connected()){
     Blynk.begin();
   }
}
else
{
}
   */
  Blynk.run();
  timer.run();
}
