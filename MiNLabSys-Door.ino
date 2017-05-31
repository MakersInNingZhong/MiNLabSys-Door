#include "ESP8266.h"
#include "RFID.h"

#define RELAY_PIN 6

void Open_the_Door(){
	digitalWrite(RELAY_PIN, LOW);
	delay(4000);
	digitalWrite(RELAY_PIN, HIGH);
}

void setup() {
	pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  ESP8266_Init();
	RFID_Init();
}

void loop() {
	byte *uid = RFID_ReadUID();
	if(uid != nullptr){
		String user_name;
		CertificateUID(user_name, uid[0], uid[1], uid[2], uid[3]);
		if(user_name != String("")){
			#ifdef USE_DEBUG_MODE
				DEBUG.println(user_name + " has opened the door!");
			#endif
			Open_the_Door();
		}
		#ifdef USE_DEBUG_MODE
		else{
			DEBUG.println("ACCESS DENIED");
		}
		#endif
	}
}

