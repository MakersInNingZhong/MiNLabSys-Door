#ifndef ESP8266
#define ESP8266

#define USE_SOFTWARE_SERIAL

#define ESP8266_TX 2
#define ESP8266_RX 3

#define ESP8266_BAUD 9600

#ifdef USE_SOFTWARE_SERIAL

#include <SoftwareSerial.h>
extern SoftwareSerial esp8266;
#define ESP8266 esp8266

#else

#define ESP8266 Serial

#endif

void InitESP8266();

#endif