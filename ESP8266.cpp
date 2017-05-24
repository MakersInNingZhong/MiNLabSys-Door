#include <Arduino.h>

#include "ESP8266.h"

#if (defined USE_SOFTWARE_SERIAL) && (defined ESP8266_TX) && (defined ESP8266_RX)
SoftwareSerial esp8266(ESP8266_RX, ESP8266_TX);
#endif

void InitESP8266(){
    #ifdef USE_SOFTWARE_SERIAL
        Serial.println("###### Initializing ESP8266 ######");
        Serial.println("SET BAUD:" + String(ESP8266_BAUD));
    #endif
    ESP8266.begin(ESP8266_BAUD);
    delay(20);
    #ifdef USE_SOFTWARE_SERIAL
        Serial.println("RESET ESP8266");
    #endif
    ESP8266.println("AT+RST");
    String rstmsg = ReadESP8266Data().trim();
    while(!rstmsg.endsWith("GOT IP")){
        delay(500);
        String rstmsg = ReadESP8266Data().trim();
    }
    #ifdef USE_SOFTWARE_SERIAL
        Serial.println("ESP8266 HAS GOT IP");
        Serial.print("IP: ");
        ESP8266.println("AT+CIFSR");
        String ipmsg = ReadESP8266Data().trim();
        if(ipmsg.endsWith("OK")){
            ipmsg = ipmsg.remove(0,8);
            ipmsg = ipmsg.remove(ipmsg.indexOf("OK"));
            Serial.println(ipmsg.trim());
        }
        Serial.println("ESP8266 Initialized!");
    #endif
}

String ReadESP8266Data(){
    while(!ESP8266.available());
    String temp;
    while(ESP82666.available()){
        temp += (char)ESP8266.read();
        delay(4);
    }
    return temp;
}