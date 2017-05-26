#include "ESP8266.h"

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  ESP8266_Init();
  CertificateUID(1,2,3,4);
}

void loop() {
  
}

