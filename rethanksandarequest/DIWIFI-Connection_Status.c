////////////////////////////////////////////////////////////////////////////
// DIWIFI Connection Status
// This Utility is for checking the connection status of the
// Dexter Industries Wifi Sensor.
//
// The script will tell you if you are connected to a wifi network.
// Debugging console should be on.
//
// For more information on the Dexter Industries Wifi Sensor go to
// http://dexterindustries.com/manual/wifi/
////////////////////////////////////////////////////////////////////////////
#pragma platform(NXT)

#define DEBUG_WIFI 1

#ifdef DEBUG_WIFI
#define writeRawHS(X, Y) debugnxtWriteRawHS(X, Y)
#else
#define writeRawHS(X, Y) nxtWriteRawHS(X, Y)
#endif

#include "drivers/common.h"
#include "New_utils.h"
#include "DIWIFI-Connection_Status.h"

task main(){
  getInfo();
  getInfoWLAN();
}
