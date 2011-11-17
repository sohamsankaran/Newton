// long nRcvChars = 0;
ubyte BytesRead[8];

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
//
//      Get LAN Information
//      Sends the command AT+WSTATUS=?
//      Upon deployment of this command, the adapter reports the current network configuration to the NXT
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

void getInfoWLAN() {
  writeDebugStreamLine("getInfo");
  ubyte status_cmd[] = {'a','t','+','W','S','T','A','T','U','S', 13};
  writeRawHS(status_cmd[0], sizeof(status_cmd));       // Send the command, byte by byte.
  wait10Msec(100);
  Receive();
  wait10Msec(100);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Get Status
//      Sends the command AT+NSTAT=?
//      Upon deployment of this command, the adapter reports the current network configuration to the NXT
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

void getInfo() {
  writeDebugStreamLine("getInfo");
  ubyte status_cmd[] = {'a','t','+','N','S','T','A','T', '=', '?', 13};
  writeRawHS(status_cmd[0], sizeof(status_cmd));       // Send the command, byte by byte.

  wait10Msec(100);
  Receive();
  wait10Msec(100);
}
