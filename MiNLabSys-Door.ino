#include "ESP8266.h"
#include "RFID.h"

void setup() {
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
			// Open the door & display name
		}
		#ifdef USE_DEBUG_MODE
		else{
			DEBUG.println("ACCESS DENIED");
		}
		#endif
	}
}

