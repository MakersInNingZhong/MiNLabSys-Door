# MiNLabSys-Door

This is the door part of MiNLabSystem Project.

This project uses Arduino Uno as main controller, ESP8266 to access WiFi, MFRC522 to read IC card.

It reads uid of a IC card and package it into json then send the json to the main server to certificate the identity of the card. Door will be opened if the server passes the certification. 

### This porject uses two following libraries:

- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [MFRC522](https://github.com/miguelbalboa/rfid)

**Warning: This project is only for demostration, DO NOT USE IT FOR SECURITY PURPOSE!!**

The uid of a IC card is easy to be made up, and this project is not using HTTPS to transfer data. So it is extremely unsafe.

## Pin Layout

#### ESP8266

- VCC -> Arduino 3.3V
- CH_PD -> Arduino 3.3V
- GND -> Arduino GND
- UTXD -> Arduino RXD(0)
- URXD -> Arduino TXD(1)

#### MFRC522

- VCC -> Arduino 3.3V
- GND -> Arduino GND
- RST -> 9(default)(can be changed in RFID.h)
- SDA/SS -> 10(default)(can be changed in RFID.h)
- MOSI -> 11
- MISO -> 12
- SCK -> 13

#### SoftwareSerial for debuging

*needs a USB to TTL device or an extra Arduino*

TX and RX pin can be changed in debug.h

## File Structure

#### MiNLabSys-Door.ino

Main file that init ESP8266 and MFRC522 and excute certification loop.

#### ESP8266.h & ESP8266.cpp

Contains ESP8266 related codes.
The server related macros are defined in ESP8266.h.

#### RFID.h & RFID.cpp

Contains MFRC522 related codes.

#### debug.h & debug.cpp

Contains codes for debuging(using software serial).