/////////////////s///////////////////////////////////////////////////////////////////////////////////////
//
//                                    Nunchuck Send 0.5
//                                    Measures Pitch, Yaw, Roll
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma platform(NXT)

long nElapsedTime = 0;
long nNearEndSent = 0;
long nNearEndRead = 0;
int  nFarEndSent  = 0;
int  nFarEndRead  = 0;
long nSendBusy    = 0;

int  nMessage  = 0;
long nRcxNoMsg = 0;
long nReadOutOfSequence  = 0;  // Out Of Sequence

long nMsgXmit     = 0;


byte CMDNunchukInit[] = {0x03, 0xA4, 0x40, 0x00};
byte CMDNunchukWrite[] = {0x02, 0xA4, 0x00};
byte CMDNunchukRead[] = {0x01, 0xA4};

byte outbuf[6];
int outbuf1[6];   //ubyte workaround for ROBOTC

ubyte fakeDat[17] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};

int chuck1val[5];
int chuck2val[3];
int buttonval[4];
ubyte chuck1dat[8];
ubyte buttondat[2];
//  This function allows conversion of an unsigned byte to a signed int
//  This is a workaround for ROBOTC
int ubyteToInt(ubyte _byte) {
  int _ret = 0;
_ret = (_byte & 0x80) ? (_byte & 0x7F) + 0x80 : _byte;
  return _ret;
}

// port is the NXT port, i2c_cmd is the nunchuk internal address register
// at i2c_cmd=0...5 are 6 Byte of sensor values
// at i2c_cmd=0x20 ... 0x2F  you get 16 Byte of calibration data

sub NunchukGetParamter(tSensors port, byte i2c_cmd)
{
  // Initialize the Nunchuk
  SensorType[port] = sensorI2CCustom;

  //Ask for 0 bytes
  sendI2CMsg(port, CMDNunchukInit[0], 0);

  while (nI2CStatus[port] == STAT_COMM_PENDING) ; // ROBOTC Wait for I2C bus to be ready

  //Write at 0x00: 6 Bytes of sensor values
  CMDNunchukWrite[2] =i2c_cmd;

  //Write Nunchuk register address
  sendI2CMsg(port, CMDNunchukWrite[0], 0);

  while (nI2CStatus[port] == STAT_COMM_PENDING) ; // ROBOTC Wait for I2C bus to be ready

  byte count = 6;
  //Ask for count bytes
  sendI2CMsg(port, CMDNunchukRead[0], count);

  while (nI2CStatus[port] == STAT_COMM_PENDING) ; // Wait for I2C bus to be ready

  if(nI2CBytesReady[port] == count){

    //Read data from buffer
    readI2CReply(port, outbuf[0], 6);

    for(int i=0; i<count; i++ ){
      outbuf1[i]=ubyteToInt(outbuf[i]);   //ubyte workaround for ROBOTC
      outbuf1[i]=(outbuf1[i]^0x17) + 0x17;
    }
  }
  else {
    // error
    memset(outbuf, 0, 0);
  }
}

//
// Function Prorotype Declarations
//
task sendMessages();
void checkBTLinkConnected();




////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                        Main Task
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{

	StartTask(sendMessages);
	wait1Msec(1);

	//return;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        Send Messages Task
////////////////////////////////////////////////////////////////////////////////////////////////////////

task sendMessages()
{
	const int kMinTimeBetweenMsg = 50;
	long nInterMsgTime;
	static long nLastSendTime = 0;

	while (true)
	{
		//
		// Delay between messages.
		//
		if ((nPgmTime - nLastSendTime) < kMinTimeBetweenMsg)
		{
			wait1Msec(6);
			continue;
		}

		if (bBTBusy)
		{
			++nSendBusy;
			wait1Msec(5);
			continue;
		}




	  ++nNearEndSent;
		++nMsgXmit;
		//if (nMsgXmit == 0)
			nMsgXmit = 1; // Don't send illegal value

			ubyte krik[3] = {1,1,0};


	  cCmdMessageWriteToBluetooth(3,krik,3,mailbox1);
	  wait10Msec(10);
	  ubyte moop[3] = {2,2,0};
	  cCmdMessageWriteToBluetooth(3,moop,3,mailbox1);
	  wait10Msec(1);
StopAllTasks();

	  wait1Msec(1);
	  //cCmdMessageWriteToBluetooth(1,buttondat,2,mailbox1);
	  nLastSendTime = nPgmTime;
	}
	return;
}

void checkBTLinkConnected()
{
	if (nBTCurrentStreamIndex >= 0)
	  return;

	PlaySound(soundLowBuzz);
	PlaySound(soundLowBuzz);
	eraseDisplay();
	nxtDisplayCenteredTextLine(3, "BT not");
	nxtDisplayCenteredTextLine(4, "Connected");
	wait1Msec(3000);
	StopAllTasks();
}
