ubyte BytesRead[8];
const ubyte newline[] = {0x0D};
typedef ubyte buff_t[128];
buff_t buffer;

long baudrates[] = {9600, 19200, 38400, 57600, 115200, 230400,460800, 921600};

// This function sends the array out Port 4.

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
//      Run this to clear out the reading buffer.
//      Simply sends a carriage return, then clears the buffer out.
////////////////////////////////////////////////////////////////////////////////////////////////////////

void clear_read_buffer()
{
    ubyte nData[] = {13};
    writeRawHS(nData[0], 1);   // Send the carriage return
    wait10Msec(100);
    while(BytesRead[0] < 0){
      nxtReadRawHS(BytesRead[0], 1);    // Read the response.  Probably an error.
    }
    wait10Msec(100);
}

/////////////////////////////////////////////////////////
//Scan Baud Rate - Scans the baud rate, sets up the sensor.
/////////////////////////////////////////////////////////
long scanBaudRate() {
  ubyte tmpbuff[8];
  string tmpString;
  ubyte attention[] = {'+','+','+',13};
  for (int i = 0; i < 8; i++) {
    memset(tmpbuff, 0, sizeof(tmpbuff));
    nxtDisableHSPort();
      wait1Msec(10);
      nxtEnableHSPort();
      nxtSetHSBaudRate(baudrates[i]);
      nxtHS_Mode = hsRawMode;
      clear_read_buffer();
      wait1Msec(1000);
      nxtWriteRawHS(attention, sizeof(attention));
      nxtReadRawHS(tmpbuff, 7);  // make sure last ubyte is always NULL
    StringFromChars(tmpString, tmpbuff);
    if ((StringFind(tmpString, "ERR") > -1) ||
        (StringFind(tmpString, "OK") > -1) ||
        (StringFind(tmpString, "0") > -1) ||
        (StringFind(tmpString, "2") > -1)) {
      clear_read_buffer();
      return baudrates[i];
    }
    }
    clear_read_buffer();
    return 0;
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
//      Echo All Input Off - Turns off the echo effect on the wifi.
//      Sending the serial command "ate0" which turns off the echo effect.
//      Sends one single byte at a time, pauses.
//      Drains receiver with a read each time.
////////////////////////////////////////////////////////////////////////////////////////////////////////

void echo_all_input_off()
{
  writeDebugStreamLine("echo_all_input_off");
  ubyte nData[] = {'a','t','e','0',13};

  for(int i = 0; i < 5; i++){
    writeRawHS(nData[i], 1);               // Send the command, byte by byte.
    nxtReadRawHS(BytesRead[0], 8);         // Clear out the echo.
    wait10Msec(10);
  }

  Receive(true);
  eraseDisplay();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//      Software Flow Control On
//      Send string "at&k1" and carriage return.
//      Shouldn't need the wait or read now that we've got the echo off.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void software_flow_control()
{
  writeDebugStreamLine("sfwr_flw_cntrl");
    ubyte nData[] = {'a','t','&','k','1',13};
    writeRawHS(nData[0], 6);            // Send the command, byte by byte.
    // wait10Msec(100);

    Receive(true);
    // wait10Msec(100);
}

//////////////////////////////////////////////////////////////////
// Configure the verbose mode of the wifi sensor.
// n = 0, verbose disabled.
// n = 1, verbose enable.
//////////////////////////////////////////////////////////////////
void set_verbose(int n)
{
  if(n>1) n = 1;                                  // Shouldn't be larger than 1 so set it back.
  n = n+48;
  writeDebugStreamLine("set_verbose");
  ubyte nData[] = {'a','t','v',n,13};
  writeRawHS(nData[0], sizeof(nData));            // Send the command, byte by byte.
  Receive(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//      Set the Infrastructure Mode.
//      Send string "AT+WM = n" and carriage return.
//      This sets the infrastructure mode as either infrastructure or ad-hoc network.
////////////////////////////////////////////////////////////////////////////////////////////////////////

void infrastructure_mode(int state)
{
  writeDebugStreamLine("wifi_infra_mode.");
  char state_n = state+48;
  ubyte nData[] = {'a','t','+','w','m','=',state_n,13};
  writeRawHS(nData[0], sizeof(nData));            // Send the command, byte by byte.
  Receive(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//      Disassociate from any networks.
//      Send string "AT+WD" and carriage return.
//      This dissaciates from any networks we're currently connected to.
////////////////////////////////////////////////////////////////////////////////////////////////////////

void disass()
{
  writeDebugStream("discon_frm_networks");
  ubyte nData[] = {'a','t','+','w','d',13};
  writeRawHS(nData[0], sizeof(nData));            // Send the command, byte by byte.
  writeRawHS(newline[0], sizeof(newline));        // send new line
  Receive(true);
}

void set_network_config()
{
  ubyte wpa_psk_cmd[] = {'A','T','S','1','=','1','0','0','0'};
  int index = 0;

  index = appendToBuff(buffer, index, wpa_psk_cmd, sizeof(wpa_psk_cmd));
  index = appendToBuff(buffer, index, newline, sizeof(newline));

  writeRawHS(buffer[0], index);
  Receive(true);
}

// Keep Association Alive for 20 Minutes
void Keep_Alive() {
  int index = 0;
  ubyte Keep_Alive_cmd[] = {'A','T','+','P','S','P','O','L','L','I','N','T','R','L','=','1','2','0','0'};

  index = appendToBuff(buffer, index, Keep_Alive_cmd, sizeof(Keep_Alive_cmd));
  index = appendToBuff(buffer, index, newline, sizeof(newline));
  writeRawHS(buffer[0], index);
  Receive(true);
}

void max_transmit_power() {
  writeDebugStreamLine("Set Max Xmit Power");
  ubyte status_cmd[] = {'a','t','+','W','P','=', '7', 13};
  writeRawHS(status_cmd[0], sizeof(status_cmd));       // Send the command, byte by byte.
  Receive(true);
}

void config_wifi(){

  scanBaudRate();           // This function scans the baud rate and sets up the server.  You can also call the "setupHighSpeedLink" function
                            // if you want a specific baud rate.  If the baud rate of the wifi sensor was a different rate, this will
                            // connect the sensor to the wifi server.
  // Configure the link for raw read and write. User program will have complete control over the link.
  // User program will be responsible for managing the half-duplex operation and must prevent collisions!
  nxtHS_Mode = hsRawMode;

  clear_read_buffer();      // Clear out the buffer and test TX/RX.
  wait10Msec(100);
  echo_all_input_off();     // Turn off serial echo.
  software_flow_control();  // Turn on software flow control
  set_verbose(1);            // Turn on verbose responses.
  infrastructure_mode(0);   // Setup as infrastructure.  1 would be adhoc network.  0 is infrastructure.
  disass();                 // Disassociate from any networks we might be hooked to.
  set_network_config();
  Keep_Alive();             // Keep association with network alive.
  max_transmit_power();     // Max transmit power . . .  rock out and forget about the batteries.
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//      WIFI:  Set Authentication Mode
//             Security of the wifi system we're connecting to.
//             Send string "at+wauth=<n>" and carriage return.
//              n = 0 --> No security mode
//              n = 1 --> Open security mode
//              n = 2 --> Shared with WEP
////////////////////////////////////////////////////////////////////////////////////////////////////////

void wifi_auth_mode(int state)
{
  writeDebugStreamLine("wifi_auth_mode");
  char state_n = state+48;
  ubyte nData[] = {'a','t','+','w','a','u','t','h','=',state_n,13};
  writeRawHS(nData[0], sizeof(nData));            // Send the command, byte by byte.
  //wait10Msec(100);
  Receive(true);
  //wait10Msec(100);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Dynamic Host Configuration Protocol
//      Sends the command AT+NDHCP=<n>
//      We're gong to turn it on or turn it off.  This will acquire an IP address automatically.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

void setDHCP(int n)
{
  writeDebugStreamLine("setDHCP");
  ubyte dhcp_cmd[] = {'a','t','+','N','D','H','C','P', '=', n+48, 13};
  writeRawHS(dhcp_cmd[0], sizeof(dhcp_cmd));       // Send the command, byte by byte.
  //wait10Msec(100);
  //wait10Msec(400);
  Receive(true);
  //wait10Msec(00);
}

void set_wpa_psk()
{
  ubyte wpa_psk_cmd[] = {'A','T','+','W','P','A','P','S','K','='};
  unsigned byte dummy[] = {','};
  int index = 0;

  index = appendToBuff(buffer, index, wpa_psk_cmd, sizeof(wpa_psk_cmd));
  index = appendToBuff(buffer, index, ssid, sizeof(ssid));
  index = appendToBuff(buffer, index, dummy, sizeof(dummy));
  index = appendToBuff(buffer, index, wpa_psk, sizeof(wpa_psk));
  index = appendToBuff(buffer, index, newline, sizeof(newline));

  writeRawHS(buffer[0], index);
}

bool Receive_connect(bool wait=false)
{
  int i = 0;
  ubyte Bytes_Error[128];
  if (wait)
    while (nxtGetAvailHSBytes() == 0) wait1Msec(5);

  while (nxtGetAvailHSBytes() > 0) {
    nxtReadRawHS(BytesRead[0], 1);
    writeDebugStream("%c", BytesRead[0]);
    wait1Msec(2);
    Bytes_Error[i] = BytesRead[0];
    i++;
  }

  writeDebugStream("%c", Bytes_Error[0]);
  writeDebugStream("%c", Bytes_Error[1]);
  writeDebugStream("%c", Bytes_Error[2]);
  writeDebugStream("%c", Bytes_Error[3]);

  if(Bytes_Error[2] == 'E' && Bytes_Error[3] == 'R'){
    return false;
  }
  return true;
}

void set_ssid() {
  ubyte ssid_cmd[] = {'A','T','+','W','A','='};
  int index = 0;

  index = appendToBuff(buffer, index, ssid_cmd, sizeof(ssid_cmd));
  index = appendToBuff(buffer, index, ssid, sizeof(ssid));
  index = appendToBuff(buffer, index, newline, sizeof(newline));
  writeRawHS(buffer, index);
}

// This repeatedly tries to connect until succesful.
void connect_to_ssid(){
  disass();
  bool quit = false;
  while(quit == false){
    set_ssid();
    quit = Receive_connect(true);
  }
}
