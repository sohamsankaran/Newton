// UDP Server Example
// This example starts a UDP Server up on the Dexter Industries Wifi Sensor
//
//  For more information and documentation please see our website: http://dexterindustries.com/manual/wifi/
//  Output is in the debugging console of RobotC.
//  Be sure to turn on the debugging console.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "drivers/common.h"
#include "New_utils.h"

#include "DIWIFI-UDP_Server.h"

task main(){
  // Command to start a UDP server is AT+NSUDP = <port>
  startUDP();           // Starts the server.
  clear_read_buffer();
  read_UDP();           // Start listening for something, anything!
}
