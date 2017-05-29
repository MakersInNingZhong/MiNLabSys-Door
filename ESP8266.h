#ifndef MODULE_ESP8266
#define MODULE_ESP8266

#include "debug.h"

#define ESP8266 Serial

#define ESP8266_BAUD 115200

#include <ArduinoJson.h>

#define SERVER_IP "192.168.1.68"
#define SERVER_PORT 3000

#define POST_PATH "/door"

void ESP8266_Init();
void CertificateUID(String& user_name, byte uid1, byte uid2, byte uid3, byte uid4);

#endif
