#include <Arduino.h>

#include "RFID.h"

MFRC522 RFID(MFRC522_SDA_PIN, MFRC522_RST_PIN);

void RFID_Init() {
    SPI.begin();
    RFID.PCD_Init();
    #ifdef USE_DEBUG_MODE
        DEBUG.println("###### Initializing MFRC522 ######");
        DEBUG.println("SDA is attached to: " + String(MFRC522_SDA_PIN));
        DEBUG.println("RST is attached to: " + String(MFRC522_RST_PIN));
        RFID.PCD_DumpVersionToSerial();
        DEBUG.println("MFRC522 Initialized!");
    #endif
}

#ifdef USE_DEBUG_MODE
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        DEBUG.print(buffer[i] < 0x10 ? " 0" : " ");
        DEBUG.print(buffer[i], HEX);
    }
}
#endif

byte * RFID_ReadUID() {
    if (RFID.PICC_IsNewCardPresent() && RFID.PICC_ReadCardSerial()){
        byte *uids = RFID.uid.uidByte;
        MFRC522::PICC_Type piccType = RFID.PICC_GetType(RFID.uid.sak);
        MFRC522::PICC_Type expectedType = RFID.PICC_GetType(0x08); //PICC_TYPE_MIFARE_1K in MFRC522.cpp line 1276
        if (piccType != expectedType){
            return nullptr;
        }
        #ifdef USE_DEBUG_MODE
            DEBUG.print("Card UID:");
            dump_byte_array(RFID.uid.uidByte, RFID.uid.size);
            DEBUG.println();
            DEBUG.print("PICC type: ");
            DEBUG.println(RFID.PICC_GetTypeName(piccType));
        #endif

        RFID.PICC_HaltA();
        RFID.PCD_StopCrypto1();

        return uids;
    }
    else{
        return nullptr;
    }
}