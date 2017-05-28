#ifndef MODULE_ESP8266
#define MODULE_ESP8266

#define ESP8266 Serial

#define USE_DEBUG_MODE

#define DEBUG_TX 2
#define DEBUG_RX 3

#define ESP8266_BAUD 115200
#define DEBUG_BAUD 9600

#ifdef USE_DEBUG_MODE
    #include <SoftwareSerial.h>
    extern SoftwareSerial debugserial;
    #define DEBUG debugserial
#endif

#include <ArduinoJson.h>

#define SERVER_IP "192.168.1.68"
#define SERVER_PORT 3000

#define POST_PATH "/door"

void ESP8266_Init();
String CertificateUID(byte uid1, byte uid2, byte uid3, byte uid4);

#endif
