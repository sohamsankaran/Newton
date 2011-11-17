//
// TCP Server Example for Dexter Industries Wifi Sensor
//
//    This program will setup a TCP server on the Dexter Industries Wifi Server.
//  You can connect to it with a Terminal emulator such as Telnet or Tera Term.
//  Simply connect to the IP on Port 20 (after connecting to a wifi network).
//
//  After connecting with a terminal, everything you type will be shown on
//  the screen of the NXT and a musical noise will play after each character.
//
//  For more information on the Dexter Industries WIFI sensor,
//  visit http://dexterindustries.com/manual/wifi/
//
//  This project relies heavily on code from Xander Soldaat.
//  See some of his brilliant projects at http://www.botbench.com
////////////////////////////////////////////////////////////////////////////////////
#include "../drivers/common.h"
#include "New_utils.h"
#include "DIWIFI-TCP_Server.h"

task main()
{
  closeAllConns();                      // Close all connections.  If any other CID's are open, close them.
  int cid = startTCP(20);               // Start TCP on Port 20.  Gets a CID for the connection.
                                        // *A CID is a connection identification.
  writeDebugStreamLine("CID:%i", cid);  // Write the CID assigned by the sensor.
  clear_read_buffer();                  // Clean up.
  echo_tcp(cid);                        // Wait for a response and run.
}
