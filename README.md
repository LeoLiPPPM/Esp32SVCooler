## esp32
The software for SVCooler!

# Usage
1. Usage information

# Notes

Note: this code requires WifiManager and BluetoothSerial. It is possible to use one or the other, but try not to use both at once since it is not necessary! If this is confusing, please contact Leo at leo_li@berkeley.edu for inquiry.

# Build Instruction

Please add the following code to BlynkSimpleEsp32_Async_WM.h after line 2415:

```
          if (strlen(BlynkESP32_WM_config.Blynk_Creds[0].blynk_server) == 0)
          {
            strncpy(BlynkESP32_WM_config.Blynk_Creds[0].blynk_server, defaultConfig.Blynk_Creds[0].blynk_server, strlen(defaultConfig.Blynk_Creds[0].blynk_server));
            BlynkESP32_WM_config.blynk_port = defaultConfig.blynk_port;
          }
          if (strlen(BlynkESP32_WM_config.Blynk_Creds[0].blynk_token) == 0)
          {
            strncpy(BlynkESP32_WM_config.Blynk_Creds[0].blynk_token, defaultConfig.Blynk_Creds[0].blynk_token, strlen(defaultConfig.Blynk_Creds[0].blynk_token));
          }
          if (strlen(BlynkESP32_WM_config.Blynk_Creds[1].blynk_server) == 0)
          {
            strncpy(BlynkESP32_WM_config.Blynk_Creds[1].blynk_server, defaultConfig.Blynk_Creds[1].blynk_server, strlen(defaultConfig.Blynk_Creds[1].blynk_server));
            BlynkESP32_WM_config.blynk_port = defaultConfig.blynk_port;
          }
          if (strlen(BlynkESP32_WM_config.Blynk_Creds[1].blynk_token) == 0)
          {
            strncpy(BlynkESP32_WM_config.Blynk_Creds[1].blynk_token, defaultConfig.Blynk_Creds[1].blynk_token, strlen(defaultConfig.Blynk_Creds[1].blynk_token));
          }
          if (strlen(BlynkESP32_WM_config.board_name) == 0)
          {
            strncpy(BlynkESP32_WM_config.board_name, defaultConfig.board_name, strlen(defaultConfig.board_name));
          }
```
Please also be aware: \
In file .pio/libdeps/esp32dev/Blynk_Async_WM/src/BlynkSimpleEsp32_Async_WM.h lines 105-106:\
Replace
```
FS* filesystem =      &LITTLEFS;
#define FileFS        LITTLEFS 
```
With
```
FS* filesystem =      &LittleFS;
#define FileFS        LittleFS 
```

In file .pio/libdeps/esp32dev/LittleFS_esp32/src/LITTLEFS.cpp line 44:\
Replace
```
File f = open(path, "r");
```
With
```
File f = open(path, "r", false);
```

# License
