#include "debug.h"

#if (defined USE_DEBUG_MODE) && (defined DEBUG_TX) && (defined DEBUG_RX)
SoftwareSerial debugserial(DEBUG_RX, DEBUG_TX);
#endif