// DIWIFI-HTTP_Webserver.h
//
//

#include "drivers/DTMP-driver.h"

ubyte BytesRead[8];
ubyte rxbuffer[1500];
ubyte newline[] = {0x0D};
ubyte endmarker[] = {27, 'E'};

typedef ubyte buff_t[128];
buff_t buffer;

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Receive Bytes
// Reads whatever is in the buffer and prints to debug
////////////////////////////////////////////////////////////////////////////////////////////////////////
void Receive(bool wait=false)
{
  if (wait)
    while (nxtGetAvailHSBytes() == 0) wait1Msec(5);
  while (nxtGetAvailHSBytes() > 0) {
    nxtReadRawHS(BytesRead[0], 1);
    writeDebugStream("%c", BytesRead[0]);
    wait1Msec(2);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//  This is a utility to put data into a buffer to send over Port4.
////////////////////////////////////////////////////////////////////////////////////////////////////////
int appendToBuff(buff_t &buf, const long index, const ubyte &pData, const long nLength)
{
  if (index == 0) memset(buf, 0, sizeof(buf));
  memcpy(buf[index], pData, nLength);
  return index + nLength;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//  This is a utility to send data over Port4.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void debugnxtWriteRawHS(const ubyte &pData, const short nLength)
{
  string tmpString;
  ubyte buff[30];
  memset(buff[0], 0, 30);
  memcpy(buff[0], pData, nLength);
  StringFromChars(tmpString, buff);
  writeDebugStream("%s", tmpString);
  nxtWriteRawHS(pData, nLength);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//      Clear Read Buffer
//      Run this to clear out the reading buffer on Port 4.
//      Simply sends a carriage return, then clears the buffer out.  Anything left in the buffer this will
//      dispose of it.
////////////////////////////////////////////////////////////////////////////////////////////////////////

void clear_read_buffer()
{
    ubyte nData[] = {13};
    debugnxtWriteRawHS(nData[0], 1);            // Send the carriage return
    wait10Msec(100);
    while(BytesRead[0] < 0){            // Do this while there's something in the buffer.
      nxtReadRawHS(BytesRead[0], 1);    // Read the response.  Probably an error.
    }
    wait10Msec(100);
}

void closeAllConns() {
  writeDebugStreamLine("closeAllCons");
  ubyte close_cmd[] = {'A','T','+','N','C','L','O','S','E','A','L','L',0x0D};
  debugnxtWriteRawHS(close_cmd[0], sizeof(close_cmd));       // Send the command, byte by byte.
  Receive(true);
}

void closeConn(int cid) {
  writeDebugStreamLine("closeConn");
  cid += 48;
  ubyte close_cmd[] = {'A','T','+','N','C','L','O','S','E','=',cid,0x0D};
  debugnxtWriteRawHS(close_cmd[0], sizeof(close_cmd));       // Send the command, byte by byte.
  Receive(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function starts the sensor listening to Port 81.  You can listne on any port
// you like, just change the "81" to whatever number port you want to listen in on.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void startListen(long port) {
  int index = 0;
  ubyte listen_cmd[] = {'A','T','+','N','S','T','C','P','=','8','1'};

  index = appendToBuff(buffer, index, listen_cmd, sizeof(listen_cmd));
  index = appendToBuff(buffer, index, newline, sizeof(newline));
  debugnxtWriteRawHS(buffer[0], index);
  Receive(true);
}

void genResponse(int cid) {
  //int power; // = motor[motorA];
  string tmpString;
  string request_string;
  string strTemp;
  float numTemp;

  int index = 0;
  ubyte linebuff[20];
  StringFromChars(tmpString,rxbuffer);
  index = StringFind(tmpString, "/");
  StringDelete(tmpString, 0, index);
  index = StringFind(tmpString, "HTTP");
  StringDelete(tmpString, index, strlen(tmpString));
  writeDebugStreamLine("Request:%s", tmpString);
  nxtDisplayTextLine(2, "Request: ");
  nxtDisplayTextLine(3, tmpString);
  request_string = tmpString;

  while(nxtHS_Status == HS_SENDING) wait1Msec(5);

  wait1Msec(100);

  //char temp_char[] = {'1', '2', '3', '4', '5', '6'};

  //for(int j = 0; j < 6; j++){
    index = 0;
    linebuff[0] = 27; // escape;
    linebuff[1] = 'S'; // Start;
    linebuff[2] = (ubyte)cid + 48; // the CID;
    index = appendToBuff(buffer, index, linebuff, 3);

   debugnxtWriteRawHS(buffer, index);   // We have to do send, and more importantly, receive here.
   index = 0;
   Receive(true);               // Receiving helps us avoid getting the accent a`

   tmpString = "   <html><head>";
   memcpy(linebuff, tmpString, strlen(tmpString));
   index = appendToBuff(buffer, index, linebuff, strlen(tmpString));

   tmpString = "<title>Temp ";
   memcpy(linebuff, tmpString, strlen(tmpString));
   index = appendToBuff(buffer, index, linebuff, strlen(tmpString));

   tmpString = "Server</title>";
   memcpy(linebuff, tmpString, strlen(tmpString));
   index = appendToBuff(buffer, index, linebuff, strlen(tmpString));

   tmpString = "</head><body>";
   memcpy(linebuff, tmpString, strlen(tmpString));
   index = appendToBuff(buffer, index, linebuff, strlen(tmpString));

   tmpString = "Requested: ";                                        // Here's where the request is worked into the system.
   memcpy(linebuff, tmpString, strlen(tmpString));
   index = appendToBuff(buffer, index, linebuff, strlen(tmpString));

   tmpString = request_string;                                        // Here's where the request is worked into the system.
   memcpy(linebuff, tmpString, strlen(tmpString));
   index = appendToBuff(buffer, index, linebuff, strlen(tmpString));

   tmpString = "<BR>";
   memcpy(linebuff, tmpString, strlen(tmpString));
   index = appendToBuff(buffer, index, linebuff, strlen(tmpString));

   tmpString = "The temp is: ";
   memcpy(linebuff, tmpString, strlen(tmpString));
   index = appendToBuff(buffer, index, linebuff, strlen(tmpString));

   DTMPreadTemp(DTMP, numTemp); // Read the temperature.
   StringFormat(strTemp, "%4.2f", numTemp); // Put it into a string.
   tmpString = strTemp;  // Put that string into the buffer.
   memcpy(linebuff, tmpString, strlen(tmpString));
   index = appendToBuff(buffer, index, linebuff, strlen(tmpString));

   tmpString = " C.";
   memcpy(linebuff, tmpString, strlen(tmpString));
   index = appendToBuff(buffer, index, linebuff, strlen(tmpString));

   tmpString = "</body>";
   memcpy(linebuff, tmpString, strlen(tmpString));
   index = appendToBuff(buffer, index, linebuff, strlen(tmpString));

   tmpString = "</html>";
   memcpy(linebuff, tmpString, strlen(tmpString));
   index = appendToBuff(buffer, index, linebuff, strlen(tmpString));

   //index = appendToBuff(buffer, index, temp_char[j], 1);
   linebuff[0] = 27; // escape;
   linebuff[1] = 'E'; // the CID;
   index = appendToBuff(buffer, index, endmarker, 2);

   clear_read_buffer();
   while(nxtHS_Status == HS_SENDING) wait1Msec(5);

   debugnxtWriteRawHS(buffer, index);
  //}

  wait1Msec(300);
  closeConn(1);                             // Close the connection or CID; we're done with our transaction.
  memset(rxbuffer, 0, sizeof(rxbuffer));    // Blow the rx buffer clear.
  wait1Msec(100);
  clear_read_buffer();                      // Clear the read buffer.
  Receive(true);                            // Clear it again.  This is necessary for some reason, and makes it work.
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function parses input.  When a web connection is made, data is sent to the sensor.
// function parses it out and looks for the beginning and ending of the message.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void parseInput()
{
  writeDebugStreamLine("Waiting to Parse.");

  ubyte BytesRead[20];
  ubyte currByte[] = {0};
  ubyte prevByte[] = {0};
  ubyte conn[] = {0};
  int cid;

  string tmpString;
  int index = 0;
  ClearTimer(T1);
  //int reset_counter = 0;
  while(true)
  {
      alive();
      if (nxtGetAvailHSBytes() > 0)
        {
        nxtReadRawHS(currByte[0], 1);
        if ((prevByte[0] == 27) && (currByte[0] == 'S')) {
          index = 0;
          memset(rxbuffer, 0, sizeof(rxbuffer));
          wait1Msec(1);
          nxtReadRawHS(conn[0], 1);
          cid = conn[0] - 48;
          writeDebugStreamLine("Conn: %d", cid);
          while (true) {
            while (nxtGetAvailHSBytes() == 0) EndTimeSlice();
            nxtReadRawHS(currByte[0], 1);
                      if ((prevByte[0] == 27) && (currByte[0] == 'E')) {
                        rxbuffer[index--] = 0;
                        rxbuffer[index--] = 0;
                        while(bSoundActive) EndTimeSlice();
                        break;
                      }
                      prevByte[0] = currByte[0];
            rxbuffer[index++] = currByte[0];
                  }
                  for (int i = 0; i < ((index / 19) + 1); i++) {
                      memset(BytesRead[0], 0, 20);
                      memcpy(BytesRead[0], rxbuffer[i*19], 19);

                      StringFromChars(tmpString, BytesRead);
                      writeDebugStream(tmpString);
                  }
                  genResponse(cid);
                  writeDebugStream("Start over parsing.");
        }
        prevByte[0] = currByte[0];
        }
       wait1Msec(1);
 }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function begins waiting for input.  Rather, it waits for an incoming connection from a
// browser on port 81.  When it gets a message, it parses the message out.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void read_input()
{
  writeDebugStreamLine("Beging parsing...");
  //ubyte BytesRead[20];
  //ubyte currByte[] = {0};
  //ubyte prevByte[] = {0};
  //ubyte conn[] = {0};
  //int cid;
  //string tmpString;
  //int index = 0;
  ClearTimer(T1);

  while (true)
  {
    alive();
      if (nxtGetAvailHSBytes() > 0)
      {
     parseInput();
      }
      wait1Msec(5);

    }
}
