#include <Arduino.h>

#include "ESP8266.h"

#if (defined USE_SOFTWARE_SERIAL) && (defined ESP8266_TX) && (defined ESP8266_RX)
SoftwareSerial esp8266(ESP8266_RX, ESP8266_TX);
#endif

void InitESP8266(){
    ESP8266.begin(ESP8266_BAUD);
}