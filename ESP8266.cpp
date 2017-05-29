#include <Arduino.h>

#include "ESP8266.h"

void ESP8266_ReadData(String& temp, int delay_time = 4) {
    #ifdef USE_DEBUG_MODE
        DEBUG.println("Reading data...");
    #endif
    while(!ESP8266.available());
    while(ESP8266.available()){
        temp += ESP8266.readString();
        delay(delay_time);
    }
}

void ESP8266_Init() {
    #ifdef USE_DEBUG_MODE
        DEBUG.begin(DEBUG_BAUD);
        DEBUG.println("###### Initializing ESP8266 ######");
        DEBUG.println("SET BAUD: " + String(ESP8266_BAUD));
    #endif
    ESP8266.begin(ESP8266_BAUD);
    #ifdef USE_DEBUG_MODE
        DEBUG.println("RESET ESP8266");
    #endif
    ESP8266.println("AT+RST");
    String rst_msg;
    ESP8266_ReadData(rst_msg);
    rst_msg.trim();
    while(!rst_msg.endsWith("GOT IP")){
        rst_msg = "";
        delay(200);
        ESP8266_ReadData(rst_msg);
        rst_msg.trim();
    }
    delay(200);
    #ifdef USE_DEBUG_MODE
        ESP8266.println("AT+CIFSR");
        String ip_msg;
        ESP8266_ReadData(ip_msg);
        DEBUG.println(ip_msg);
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
    #endif
}

void CreateHTTPHeader(String& header, String method, String path, String content_type, int content_length){
    header = method + " " + path + " HTTP/1.1\r\n";
    header += "Connection: close\r\n";
    header += "Content-Type: " + content_type + ";charset=utf-8\r\n";
    header += "Content-Length: " + String(content_length) + "\r\n\r\n";
}

bool ESP8266_Start_TCP(String ip, int port){
    ESP8266.println("AT+CIPSTART=\"TCP\",\"" + ip + "\"," + String(port));
    String start_msg;
    ESP8266_ReadData(start_msg);
    start_msg.trim();
    return start_msg.endsWith("OK");
}

bool ESP8266_Send_Json(String json_str) {
    if(ESP8266_Start_TCP(SERVER_IP, SERVER_PORT)){
        String header;
        CreateHTTPHeader(header, "POST", POST_PATH, "application/json", json_str.length());
        json_str += "\r\n";
        ESP8266.println("AT+CIPSEND=" + String(header.length() + json_str.length()));
        String temp;
        ESP8266_ReadData(temp);
        ESP8266.print(header);
        ESP8266.print(json_str);
        return true;
    }
    else{
        #ifdef USE_DEBUG_MODE
            DEBUG.println("Can't Reach Server " + String(SERVER_IP) + ":" + String(SERVER_PORT) + "!!!");
        #endif
        return false;
    }
}

void CertificateUID(String& user_name, byte uid1, byte uid2, byte uid3, byte uid4) {
    //Create Json: {"uid":[uid1, uid2, uid3, uid4]}
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    JsonArray& uid = root.createNestedArray("uid");
    uid.add(uid1);
    uid.add(uid2);
    uid.add(uid3);
    uid.add(uid4);
    String uid_json;
    root.printTo(uid_json);
    //Send Json
    if(!ESP8266_Send_Json(uid_json)){
        user_name = String("");
        return;
    }
    //Receive name
    String rec_msg;
    ESP8266_ReadData(rec_msg, 2);
    #ifdef USE_DEBUG_MODE
        DEBUG.println(rec_msg);
    #endif
    rec_msg.trim();
    if(rec_msg.endsWith("CLOSED")){
        int status_index = rec_msg.indexOf("200 OK");
        if(status_index != -1){
            String rec_json = rec_msg.substring(rec_msg.indexOf('{'), rec_msg.lastIndexOf('}') + 1);
            JsonObject& rec_json_obj = jsonBuffer.parseObject(rec_json);
            if (rec_json_obj.success()) {
                user_name = rec_json_obj["name"].as<String>();
                return;
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
}
