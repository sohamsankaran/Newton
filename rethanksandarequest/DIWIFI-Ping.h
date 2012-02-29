/////////////////////////
// DIFWIFI Ping Functions
/////////////////////////

ubyte BytesRead[8];
typedef ubyte buff_t[512];
buff_t buffer;
ubyte newline[] = {0x0D};

// writeArray function writes an array to the RawHS function
// and prints it out in the debug stream.
void writeArray(int indx)
{
  string buff_out;
  char buff_iter;
  int index = indx;
  nxtWriteRawHS(buffer, index);             // Write to high speed port

  writeDebugStream("%c", newline);          // Give us a new line to make it look clean.
  for(int t = 0; t < index; t++)            // Write to DebugStream
  {
    buff_iter = buffer[t];                  // Load up the specific iteration.
    StringFromChars(buff_out, buff_iter);   // Make it a string.
    writeDebugStream(buff_out);             // Now print to DebugStream
  }
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


int appendToBuff(buff_t &buf, const long index, const ubyte &pData, const long nLength)
{
  if (index == 0) memset(buf, 0, sizeof(buf));
  memcpy(buf[index], pData, nLength);
  return index + nLength;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function reads an IP address and puts it into a
// ubyte array.
////////////////////////////////////////////////////////////////////////////////////////////////////////

void Read_IP()
{
  int t = 0;
  while (nxtGetAvailHSBytes() == 0) wait1Msec(5);
  while (nxtGetAvailHSBytes() > 0) {
    nxtReadRawHS(BytesRead[0], 1);
    if(t>4 && BytesRead != 13 && BytesRead[0] != 'O' && BytesRead[0] != 'K'){
      IP_num[t-5] = BytesRead[0];
    }
    wait1Msec(2);
    t++;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// This will get the IP address of a specific domain name.
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void get_IP_address() {
  int index = 0;
  ubyte DNSlookupCmd[] = {'A','T','+','D','N','S','L','O','O','K','U','P','='};
  index = appendToBuff(buffer, index, DNSlookupCmd, sizeof(DNSlookupCmd));
  index = appendToBuff(buffer, index, DNS_Name, sizeof(DNS_Name));
  index = appendToBuff(buffer, index, newline, sizeof(newline));
  writeArray(index);
  Read_IP();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function pings an IP
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void ping_IP() {
  Receive();
  int index = 0;
  ubyte listen_cmd[] = {'a','t','+','p','i','n','g','='};
  index = appendToBuff(buffer, index, listen_cmd, sizeof(listen_cmd));
  index = appendToBuff(buffer, index, IP_num, 14);
  index = appendToBuff(buffer, index, newline, sizeof(newline));
  writeArray(index);
  while(true){
    Receive(true);
  }
}
