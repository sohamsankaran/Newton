ubyte BytesRead[8];
ubyte newline[] = {0x0D};
ubyte rxbuffer[1500];
typedef ubyte buff_t[128];
buff_t buffer;

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

int appendToBuff(buff_t &buf, const long index, const ubyte &pData, const long nLength)
{
  if (index == 0) memset(buf, 0, sizeof(buf));
  memcpy(buf[index], pData, nLength);
  return index + nLength;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
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
//      Clear Read Buffer
//      Run this to clear out the reading buffer.
//      Simply sends a carriage return, then clears the buffer out.
////////////////////////////////////////////////////////////////////////////////////////////////////////

void clear_read_buffer()
{
    ubyte nData[] = {13};
    debugnxtWriteRawHS(nData[0], 1);   // Send the carriage return
    wait10Msec(100);
    while(BytesRead[0] < 0){
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

// Starts up a TCP server gets the CID.
int startTCP(long port) {
  int index = 0;
  int cid = 0;
  ubyte listen_cmd[] = {'A','T','+','n','s','t','c','p','=','2','0'};
  index = appendToBuff(buffer, index, listen_cmd, sizeof(listen_cmd));
  index = appendToBuff(buffer, index, newline, sizeof(newline));
  debugnxtWriteRawHS(buffer[0], index);
  while (nxtGetAvailHSBytes() == 0) wait1Msec(5);
  while (nxtGetAvailHSBytes() > 0) {
    nxtReadRawHS(BytesRead[0], 1);
    writeDebugStream("%c", BytesRead[0]);
    if(BytesRead[0] < 58 && BytesRead > 47){  // So if it's a number . . .
      cid = BytesRead[0]-48;  // Works for connections 0 through 9.
    }
    wait1Msec(2);
  }

  return cid;
}

void parseTCP(int cid_connection)
{
  writeDebugStreamLine("Parse TCP");

  ubyte BytesRead[20];
  ubyte currByte[] = {0};
  ubyte prevByte[] = {0};
  ubyte conn[] = {0};
  int tone = 0;
  int lineNum = 6;
  int colNum = 0;
  string tmpString;

  int index = 0;
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
          while (true) {
            while (nxtGetAvailHSBytes() == 0) EndTimeSlice();
            nxtReadRawHS(currByte[0], 1);
					  if ((prevByte[0] == 27) && (currByte[0] == 'E')) {
					    rxbuffer[index--] = 0;
					    rxbuffer[index--] = 0;
					    break;
					  }
					  prevByte[0] = currByte[0];
            rxbuffer[index++] = currByte[0];
				  }
				  for (int i = 0; i < ((index / 19) + 1); i++) {
					  memset(BytesRead[0], 0, 20);
					  memcpy(BytesRead[0], rxbuffer[i*19], 19);
					  StringFromChars(tmpString, BytesRead);
					  tone = (int)BytesRead[0];
					  tone = tone*10;
					  PlayTone(tone, 10);
            while(bSoundActive) EndTimeSlice();
					  writeDebugStream(tmpString);

					  colNum++;
					  if(colNum > 12) {
					    colNum = 0;
					    lineNum--;
					  }
					  if(lineNum == 0) lineNum = 6;
					  nxtDisplayStringAt(colNum*6, lineNum*10, tmpString);
				  }
        }
        prevByte[0] = currByte[0];
	    }
	   wait1Msec(1);
 }
}

void echo_tcp(int cid){
  writeDebugStream("TCP Server Started.");
  writeDebugStreamLine("Listening.");
  parseTCP(cid);
}
