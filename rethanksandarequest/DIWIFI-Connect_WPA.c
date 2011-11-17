////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Setup for Wifi Sensor, Associate with a WPA Network
//
//  This scrip will connect the DI Wifi sensor to a WPA network.  Output is in the debugging console of RobotC.
//  Be sure to turn on the debugging console.
//
//  You must replace the values of SSID and WPA_PAK
//
//  For more information and documentation please see our website: http://dexterindustries.com/manual/wifi/
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DEBUG_WIFI 1
#ifdef DEBUG_WIFI
#define writeRawHS(X, Y) debugnxtWriteRawHS(X, Y)
#else
#define writeRawHS(X, Y) nxtWriteRawHS(X, Y)
#endif

// Declare the SSID and WPA_PSK we're connecting to
ubyte ssid[] = {'b','e','l','k','i','n','.','7','1','4'};
ubyte wpa_psk[] = {'7','e','6','6','7','6','4','a'};

#include "../drivers/common.h"
#include "DIWIFI-Connect_WPA.h"

void wifi_startup(){

  config_wifi();            // This function sets up the wifi sensor for operation.  Basic housekeeping.
  wifi_auth_mode(2);        // Set the authentication mode.
                            // 0 is none or WPA // 1 is Open network // 2 is Shared with WEP
  setDHCP(1);               // Get us an IP number
  set_wpa_psk();            // Calculates your psk
  Receive(true);            // Housekeeping
  Receive(true);            // Housekeeping
  connect_to_ssid();        // Connects to the wifi network you've designated
  PlaySound(soundBeepBeep); // After connection, announce to the world you're connected.
  Receive(true);            // Housekeeping.
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        Main Task
////////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
  eraseDisplay();
  bNxtLCDStatusDisplay = true; // Enable top status line display
  wifi_startup();             // This is where the work is done.
  PlaySound(soundBeepBeep);   // Make some noise when we're connected.
  PlayTone(500, 100);         // Make some noise when we're connected.
}
