#include <Arduino.h>

#include "ESP8266.h"

#if (defined USE_SOFTWARE_SERIAL) && (defined ESP8266_TX) && (defined ESP8266_RX)
SoftwareSerial esp8266(ESP8266_RX, ESP8266_TX);
#endif

String ESP8266_ReadData() {
    while(!ESP8266.available());
    String temp;
    while(ESP8266.available()){
        temp += (char)ESP8266.read();
        delay(4);
    }
    return temp;
}

void ESP8266_Init() {
    #ifdef USE_SOFTWARE_SERIAL
        Serial.begin(115200);
        Serial.println("###### Initializing ESP8266 ######");
        Serial.println("SET BAUD: " + String(ESP8266_BAUD));
    #endif
    ESP8266.begin(ESP8266_BAUD);
    delay(20);
    #ifdef USE_SOFTWARE_SERIAL
        Serial.println("RESET ESP8266");
    #endif
    ESP8266.println("AT+RST");
    String rst_msg = ESP8266_ReadData();
    rst_msg.trim();
    while(!rst_msg.endsWith("GOT IP")){
        delay(500);
        rst_msg = ESP8266_ReadData();
        rst_msg.trim();
    }
    #ifdef USE_SOFTWARE_SERIAL
        Serial.println("ESP8266 HAS GOT IP");
        ESP8266.println("AT+CIFSR");
        String ip_msg = ESP8266_ReadData();
        ip_msg.trim();
        if(ip_msg.endsWith("OK")){
            ip_msg.remove(ip_msg.indexOf("OK"));
            ip_msg.trim();
            Serial.print("IP: ");
            ip_msg.remove(0, ip_msg.indexOf("\"") + 1);
            Serial.println(ip_msg.substring(0, ip_msg.indexOf("\"")));
            ip_msg.remove(0, ip_msg.indexOf("\"") + 1);
            Serial.print("MAC: ");
            ip_msg.remove(0, ip_msg.indexOf("\"") + 1);
            Serial.println(ip_msg.substring(0, ip_msg.indexOf("\"")));
        }
        Serial.println("ESP8266 Initialized!");
    #endif
}

String CreateHTTPHeader(String method, String path, String content_type, int content_length){
    //String header = method + " " + path+ " HTTP/1.1\r\n" + ...
}

String CreatePOSTRequest(String path, String content_type, String body) {
    String header = CreateHTTPHeader("POST", path, content_type, body.length());
}

void ESP8266_Send_Json(String json_str) {
    //String req = CreatePOSTRequest();
}

String CertificateUID(byte uid1, byte udi2, byte uid3, byte uid4) {
    String uid_json; //package uid into json
    ESP8266_Send_Json(uid_json);
    delay(200);
    String cer_msg = ESP8266_ReadData();
    cer_msg.trim();
    //unpackge cer_msg
    if(true){  //if Certificated
        return String("");
    }
    else{
        return String("");
    }
}
