
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Setup for Wifi Sensor, and Associate with a WEP Network
//
//  This scrip will connect the DI Wifi sensor to a WEP network.  After connecting the
//  NXT will query the network status and print it out on the debugging console.
//
//  Output is in the debugging console of RobotC. So be sure to turn on the debugging console.
//
//  You must replace the values of SSID and WEP_KEY for your system.
//
//  For more information and documentation please see our website: http://dexterindustries.com/manual/wifi/
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma platform(NXT)

#define DEBUG_WIFI 1

#ifdef DEBUG_WIFI
#define writeRawHS(X, Y) debugnxtWriteRawHS(X, Y)
#else
#define writeRawHS(X, Y) nxtWriteRawHS(X, Y)
#endif

// Declare the SSID we're connecting to, character by character.  Nothing at the end of the array.
ubyte ssid[] = {'D', 'E', 'X', 'T', 'E', 'R','_','W','A','N'};
// Declaring the WEP_KEY: This example has a 26-digit WEP Key.
// If you have a 10-digit WEP Key, delete the extra spaces.
// Don't put line returns at the end of this array.
ubyte wep_key[] = {'*','*','*','*','5','*','5','*','5','*','5','*','5','*','4','*','5','*','4','*','5','*','5','*','5','*'};        // Space for 10 digit WEP key.

#include "New_utils.h"
#include "../drivers/common.h"

#include "DIWIFI-Connect_WEP.h"

void wifi_startup(){
  config_wifi();
  set_wep_key(1);           // Setup the WEP key.
  setDHCP(1);               // Get us an IP number
  wifi_auth_mode(1);        // Set the authentication mode.
                            // 0 is none or WPA // 1 is Open network // 2 is Shared with WEP
  associate_with_network(); // Connect to Wifi network

  getInfo();                // Get current lan status.
  getInfoWLAN();            // Get current lan status.

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                        Main Task
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{

  eraseDisplay();
  bNxtLCDStatusDisplay = true; // Enable top status line display
  wifi_startup();
  PlaySound(soundBeepBeep);   // Make some noise when we're connected.
  PlayTone(500, 100);         // Make some noise when we're connected.


}
