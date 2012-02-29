typedef ubyte buff_t[128];
buff_t buffer;
ubyte BytesRead[8];
ubyte newline[] = {0x0D};

int appendToBuff(buff_t &buf, const long index, const ubyte &pData, const long nLength)
{
  if (index == 0) memset(buf, 0, sizeof(buf));
  memcpy(buf[index], pData, nLength);
  return index + nLength;
}

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


void startUDP() {
  int index = 0;
  ubyte listen_cmd[] = {'A','T','+','N','S','U','D','P','=','1','0'};
  index = appendToBuff(buffer, index, listen_cmd, sizeof(listen_cmd));
  index = appendToBuff(buffer, index, newline, sizeof(newline));
  debugnxtWriteRawHS(buffer[0], index);
  Receive(true);
}

void read_UDP(){
  writeDebugStreamLine("UDP Server Started.");
  writeDebugStreamLine("Listening . . .");
  while(true){
     Receive(true);
  }
}
