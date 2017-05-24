#include <Arduino.h>

#include "ESP8266.h"

#if (defined USE_SOFTWARE_SERIAL) && (defined ESP8266_TX) && (defined ESP8266_RX)
SoftwareSerial esp8266(ESP8266_RX, ESP8266_TX);
#endif

String ESP8266_ReadData() {
    while(!ESP8266.available());
    String temp;
    while(ESP82666.available()){
        temp += (char)ESP8266.read();
        delay(4);
    }
    return temp;
}

void ESP8266_Init() {
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
    String rst_msg = ESP8266_ReadData().trim();
    while(!rst_msg.endsWith("GOT IP")){
        delay(500);
        String rst_msg = ESP8266_ReadData().trim();
    }
    #ifdef USE_SOFTWARE_SERIAL
        Serial.println("ESP8266 HAS GOT IP");
        Serial.print("IP: ");
        ESP8266.println("AT+CIFSR");
        String ip_msg = ESP8266_ReadData().trim();
        if(i_pmsg.endsWith("OK")){
            ip_msg = ip_msg.remove(0,8);
            ip_msg = ip_msg.remove(ip_msg.indexOf("OK"));
            Serial.println(ip_msg.trim());
        }
        Serial.println("ESP8266 Initialized!");
    #endif
}

String CreateHTTPHeader(String method, String path, String content_type, String content_length){
    //String header = method + " " + path+ " HTTP/1.1\r\n" + ...
}

String CreatePOSTRequest(String path, String content_type, String body) {
    String header = CreateHTTPHeader("POST", path, content_type);
}

void ESP8266_Send_Json(String json_str) {
    String req = CreateHTTPRequest();
}

String CertificateUID(byte uid1, byte udi2, byte uid3, byte uid4) {
    String uid_json; //package uid into json
    ESP8266_Send_Json(uid_json);
    delay(200);
    String cer_msg = ESP8266_ReadData().trim();
    //unpackge cer_msg
    if(true){  //if Certificated
        return name;
    }
    else{
        return String("");
    }
}