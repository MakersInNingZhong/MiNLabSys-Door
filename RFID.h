#ifndef MODULE_RFID
#define MODULE_RFID

#include "debug.h"

#include <SPI.h>
#include <MFRC522.h>

#define MFRC522_RST_PIN 9
#define MFRC522_SDA_PIN 10

extern MFRC522 RFID;

void RFID_Init();
byte * RFID_ReadUID();

#endif