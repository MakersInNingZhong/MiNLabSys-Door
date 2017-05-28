#include <Arduino.h>

#include "ESP8266.h"

#if (defined USE_DEBUG_MODE) && (defined DEBUG_TX) && (defined DEBUG_RX)
SoftwareSerial debugserial(DEBUG_RX, DEBUG_TX);
#endif

DynamicJsonBuffer jsonBuffer;

String ESP8266_ReadData() {
    while(!ESP8266.available());
    String temp;
    while(ESP8266.available()){
        temp += ESP8266.readString();
        delay(2);
    }
    return temp;
}

void ESP8266_Init() {
    #ifdef USE_DEBUG_MODE
        DEBUG.begin(DEBUG_BAUD);
        DEBUG.println("###### Initializing ESP8266 ######");
        DEBUG.println("SET BAUD: " + String(ESP8266_BAUD));
    #endif
    ESP8266.begin(ESP8266_BAUD);
    delay(20);
    /*#ifdef USE_DEBUG_MODE
        DEBUG.println("RESET ESP8266");
    #endif
    ESP8266.println("AT+RST");
    String rst_msg = ESP8266_ReadData();
    rst_msg.trim();
    while(!rst_msg.endsWith("GOT IP")){
        delay(500);
        rst_msg = ESP8266_ReadData();
        rst_msg.trim();
    }
    ESP8266.println("AT+CIPMUX=0");
    ESP8266_ReadData();
    #ifdef USE_DEBUG_MODE
        DEBUG.println("ESP8266 HAS GOT IP");
        ESP8266.println("AT+CIFSR");
        String ip_msg = ESP8266_ReadData();
        ip_msg.trim();
        if(ip_msg.endsWith("OK")){
            ip_msg.remove(ip_msg.indexOf("OK"));
            ip_msg.trim();
            DEBUG.print("IP: ");
            ip_msg.remove(0, ip_msg.indexOf("\"") + 1);
            DEBUG.println(ip_msg.substring(0, ip_msg.indexOf("\"")));
            ip_msg.remove(0, ip_msg.indexOf("\"") + 1);
            DEBUG.print("MAC: ");
            ip_msg.remove(0, ip_msg.indexOf("\"") + 1);
            DEBUG.println(ip_msg.substring(0, ip_msg.indexOf("\"")));
        }
        DEBUG.println("ESP8266 Initialized!");
    #endif*/
}

String CreateHTTPHeader(String method, String path, String content_type, int content_length){
    String header = method + " " + path + " HTTP/1.1\r\n";
    header += "Connection: close\r\n";
    header += "Content-Type: " + content_type + ";charset=utf-8\r\n";
    header += "Content-Length: " + String(content_length) + "\r\n\r\n";
    return header;
}

String CreatePOSTRequest(String path, String content_type, String body) {
    String header = CreateHTTPHeader("POST", path, content_type, body.length());
    body.trim();
    return header + body + "\r\n";
}

bool ESP8266_Start_TCP(String ip, int port){
    ESP8266.println("AT+CIPSTART=\"TCP\",\"" + ip + "\"," + String(port));
    String start_msg = ESP8266_ReadData();
    start_msg.trim();
    return start_msg.endsWith("OK");
}

String ESP8266_Send_Json(String json_str) {
    String req = CreatePOSTRequest(POST_PATH, "application/json", json_str);
    if(ESP8266_Start_TCP(SERVER_IP, SERVER_PORT)){
        ESP8266.println("AT+CIPSEND=" + String(req.length()));
        ESP8266_ReadData();
        ESP8266.print(req);
    }
    else{
        #ifdef USE_DEBUG_MODE
            DEBUG.println("Can't Reach Server " + String(SERVER_IP) + ":" + String(SERVER_PORT) + "!!!");
        #endif
    }
}

String CertificateUID(byte uid1, byte uid2, byte uid3, byte uid4) {
    //Create Json: {"uid":[uid1, uid2, uid3, uid4]}
    JsonObject& root = jsonBuffer.createObject();
    JsonArray& uid = root.createNestedArray("uid");
    uid.add(uid1);
    uid.add(uid2);
    uid.add(uid3);
    uid.add(uid4);
    String uid_json;
    root.printTo(uid_json);
    //Send Json
    ESP8266_Send_Json(uid_json);
    //Receive name
    String user_name;
    String rec_msg = ESP8266.readString();
    rec_msg.trim();
    if(rec_msg.endsWith("CLOSED")){
        int status_index = rec_msg.indexOf("200 OK");
        if(status_index != -1){
            String rec_json = rec_msg.substring(rec_msg.indexOf('{'), rec_msg.lastIndexOf('}') + 1);
            JsonObject& rec_json_obj = jsonBuffer.parseObject(rec_json);
            if (rec_json_obj.success()) {
                user_name = rec_json_obj["name"].as<String>();
            }
            #ifdef USE_DEBUG_MODE
            else{
                DEBUG.println("Can't parse json!!!");
                DEBUG.println(rec_json);
            }
            #endif
        }
        else{
            //deal with 404 or 500
            #ifdef USE_DEBUG_MODE
                DEBUG.println("404 or 500 ERROR!!!");
                DEBUG.println(rec_msg);
            #endif
        }
    }
    #ifdef USE_DEBUG_MODE
    else{
        DEBUG.println("Received Data is incorrect!!!");
        DEBUG.println(rec_msg);
    }
    #endif
    return user_name;
}
