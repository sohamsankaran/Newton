
#pragma config(Motor,  motorA,          M1,            tmotorNormal, openLoop, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../drivers/common.h"
#include "DIWIFI-driver.h"
//#include "New_utils.h"

ubyte rxbuffer[1500];
//ubyte txbuffer[300];

typedef enum {
  WIFI_STATE_NONE = 0,
  WIFI_STATE_START,
  WIFI_STATE_SENDING,
  WIFI_STATE_READING,
  WIFI_STATE_LISTENING,
  WIFI_STATE_SUCCESS
} tWiFiState;


void sendHeader(int cid) {
  ubyte linebuff[20];
  int index = 0;
  string tmpString;

  linebuff[0] = 27; // escape;
  linebuff[1] = 'S'; // the CID;
  linebuff[2] = (ubyte)cid + 48; // the CID;
  index = appendToBuff(buffer, index, linebuff, 3);

  tmpString = "HTTP/1.1 200 OK";
  memcpy(linebuff, tmpString, strlen(tmpString));
  index = appendToBuff(buffer, index, linebuff, strlen(tmpString));
  index = appendToBuff(buffer, index, linefeed, sizeof(linefeed));

  tmpString = "Content-Type:";
  memcpy(linebuff, tmpString, strlen(tmpString));
  index = appendToBuff(buffer, index, linebuff, strlen(tmpString));

  tmpString = " text/plain";
  memcpy(linebuff, tmpString, strlen(tmpString));
  index = appendToBuff(buffer, index, linebuff, strlen(tmpString));
  index = appendToBuff(buffer, index, linefeed, sizeof(newline));
  index = appendToBuff(buffer, index, linefeed, sizeof(newline));
  linebuff[0] = 27; // escape;
  linebuff[1] = 'E'; // the CID;
  index = appendToBuff(buffer, index, endmarker, 2);
  writeRawHS(buffer, index);
}


void genResponse(int cid) {
  int power = motor[motorA];
  string tmpString;
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
  if (StringFind(tmpString, "MOTA") > 0) {
    StringDelete(tmpString, 0, 6);
    index = StringFind(tmpString, " ");
    if (index > -1)
      StringDelete(tmpString, index, strlen(tmpString));
    //power = RC_atoix(tmpString);
    power = clip(RC_atoix(tmpString), -100, 100);
    writeDebugStreamLine("Power:%d", power);
  } else {
    writeDebugStreamLine("NO POWER: %s", tmpString);
  }

  sendHeader(cid);
  while(nxtHS_Status == HS_SENDING) wait1Msec(5);

  wait1Msec(100);

  index = 0;
  linebuff[0] = 27; // escape;
  linebuff[1] = 'S'; // the CID;
  linebuff[2] = (ubyte)cid + 48; // the CID;
  index = appendToBuff(buffer, index, linebuff, 3);
  StringFormat(tmpString, "MotorA=%d", power);
  memcpy(linebuff, tmpString, strlen(tmpString));
  index = appendToBuff(buffer, index, linebuff, strlen(tmpString));
  linebuff[0] = 27; // escape;
  linebuff[1] = 'E'; // the CID;
  index = appendToBuff(buffer, index, endmarker, 2);
  writeRawHS(buffer, index);
  motor[motorA] = power;
  closeConn(1);
  memset(rxbuffer, 0, sizeof(rxbuffer));
  wait1Msec(100);
  clear_read_buffer();
}


void parseInput()
{
  writeDebugStreamLine("Beging parsing...");

  ubyte BytesRead[20];
  ubyte currByte[] = {0};
  ubyte prevByte[] = {0};
  ubyte conn[] = {0};
  int cid;
  string tmpString;
  int index = 0;
  while (true)
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
					  PlaySound(soundShortBlip);
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
      }
      prevByte[0] = currByte[0];
	  }
	}
}

task main()
{
  long rate = 0;
  eraseDisplay();
  bNxtLCDStatusDisplay = true; // Enable top status line display
  writeDebugStream("Scanning for wifi sensor: ");
  rate = scanBaudRate();
  writeDebugStreamLine("%d baud", rate);
  configureWiFi();
  set_verbose(false);
  Receive();
  wait1Msec(100);
  closeAllConns();
  wait1Msec(1000);
  clear_read_buffer();
  startListen(80);
  clear_read_buffer();
  parseInput();
}
