#pragma config(Sensor, S1,     DTMP,               sensorAnalogInactive)
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DIWIFI-HTTP_Webserver.c
//
// This example will read input from the url request, and return the latest Temperature in Celsius.
//
// This example establishes a webserver that can be accessed through the sensor's IP number.
// After establishing the the connection to a wifi network, using either WPA or WEP, you should have
// an IP number for the sensor.
//
// Running this program will establish a server on the IP number, at port 81.  It can be accessed
// through a web browser by typing http://(SENSOR IP NUMBER):81/(YOUR MESSAGE)
// For example http://192.168.0.104:81/hello_world
//
// The server should return your message and a web page with the current temperature on it.
// The temperature uses the Dexter Industries thermometer.
//
// This example was written with an incredible amount of help from Xander Soldaat.
// You can see more of his brilliant work here:  http://botbench.com/
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include drivers/common.h
#include wifi-server.h

////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        Main Task
////////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
  eraseDisplay();               // Housekeeping
  bNxtLCDStatusDisplay = true;  // Enable top status line display
  clear_read_buffer();          // Housekeeping: clear out the buffer on Port4
  closeAllConns();              // Disconnect from any lingering
  startListen(81);              // Start listening on a specific port
  clear_read_buffer();          // Clear out any response.
  read_input();                 // Start the webserver, wait for a connection.
}
