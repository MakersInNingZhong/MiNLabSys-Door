#ifndef DEBUG_INFO
#define DEBUG_INFO

#define USE_DEBUG_MODE

#ifdef USE_DEBUG_MODE

    #define DEBUG_TX 2
    #define DEBUG_RX 3

    #define DEBUG_BAUD 115200

    #include <SoftwareSerial.h>
    extern SoftwareSerial debugserial;
    #define DEBUG debugserial

#endif

#endif