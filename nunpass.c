

#pragma platform(NXT)
#pragma config(Sensor, S3,     HTMC,                sensorLowSpeed)
#include "drivers/HTMC-driver.h"
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

int quinley;


byte CMDNunchukInit[] = {0x03, 0xA4, 0x40, 0x00};
byte CMDNunchukWrite[] = {0x02, 0xA4, 0x00};
byte CMDNunchukRead[] = {0x01, 0xA4};

byte outbuf[6];
int outbuf1[6];   //ubyte workaround for ROBOTC

int chuck1val[5];
int chuck2val[3];
int buttonval[4];
ubyte chuck1dat[8];
ubyte buttondat[2];
//  This function allows conversion of an unsigned byte to a signed int
//  This is a workaround for ROBOTC
/*int ubyteToInt(ubyte _byte) {
  int _ret = 0;
_ret = (_byte & 0x80) ? (_byte & 0x7F) + 0x80 : _byte;
  return _ret;
}*/

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
  HTMCsetTarget(HTMC);
  chuck1val[0] = 0;
  chuck1val[1] = 0;
  chuck1val[2] = 0;
  chuck1val[3] = 0;
  chuck1val[4] = 0;
  chuck2val[0] = 0;
  chuck2val[1] = 0;
  chuck2val[2] = 0;
  buttonval[0] = 0;
  buttonval[1] = 0;
  while(true){



  NunchukGetParamter(S1, 0x00);          //see comment for i2c_cmd

    if (sizeof(outbuf)==6)    {

      // analog stick
      eraseDisplay();
      nxtDrawCircle((outbuf1[0]-0x1E)/2, (outbuf1[1]-0x1E)/3, 3);
       nxtDisplayTextLine(2,"1 = %d", (outbuf1[2]-0x80));

       chuck1val[2] = ((outbuf1[1])-0x80);
       buttonval[0] =  (outbuf1 [5] & 0x01);
       buttonval[1] =  (outbuf1 [5] & 0x02);
       if(((outbuf1 [5]) & 0x01)==0){
       chuck1val[0] = ((outbuf1[2])-0x80);
       chuck1val[1] = ((outbuf1[3])-0x80);
       chuck1val[4] = ((outbuf1[4])-0x80);
     }

     else{
        //chuck1val[0] = 0;
        //chuck1val[1] = 0;
        //chuck1val[4] = 0;

     }

    }
    NunchukGetParamter(S2, 0x00);          //see comment for i2c_cmd

    if (sizeof(outbuf)==6)    {

      chuck1val[3]  = ((outbuf1[1])-0x80);
      buttonval[2] =  (outbuf1 [5] & 0x01);
      buttonval[3] =  (outbuf1 [5] & 0x02);
       if(((outbuf1 [5]) & 0x01)==0){
     chuck2val[0] = ((outbuf1[2])-0x80);
      chuck2val[1] = ((outbuf1[3])-0x80);
      chuck2val[2] = ((outbuf1[4])-0x80);
    }

     else{
        //chuck2val[0] = 0;
        //chuck2val[1] = 0;
        //chuck2val[2] = 0;

     }


    }
	checkBTLinkConnected();
	eraseDisplay();
	bNxtLCDStatusDisplay = true; // Enable top status line display
	chuck1dat[0] = chuck1val[0]+100;
chuck1dat[1] = chuck1val[1]+100;
chuck1dat[4] = chuck1val[4]+100;
chuck1dat[2] = chuck1val[2]+100;
chuck1dat[3] = chuck1val[3]+100;
chuck1dat[5] = chuck2val[0]+100;
chuck1dat[6] = chuck2val[1]+100;
chuck1dat[7] = chuck2val[2]+100;
buttondat[0] = buttonval[0]+100;
buttondat[1] = buttonval[1]+100;

if(1==1){

  nxtDisplayTextLine(2,"UP = %d",2);
quinley=0;

if(HTMCreadRelativeHeading(HTMC) > 40 && HTMCreadRelativeHeading(HTMC) < 100){
  quinley = 1;
}

if(HTMCreadRelativeHeading(HTMC) < -40 && HTMCreadRelativeHeading(HTMC) > 100){
  quinley = 2;
}
}




	StartTask(sendMessages);
	wait1Msec(1);

	//return;
}
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


			ubyte datathree[3] = {0,0,0};
			ubyte datatone[21];
		if(quinley == 0){
		  //donothing
		}
		if(quinley == 1){
		  datathree[0] = 0;
		  datatone[0] = 1;
		  cCmdMessageWriteToBluetooth(3,datathree,3,mailbox1);
		  cCmdMessageWriteToBluetooth(1,datatone,1,mailbox1);
		  wait1Msec(100);
		  datatone[0] = 0;

		  cCmdMessageWriteToBluetooth(1,datatone,1,mailbox1);
		  wait10Msec(500);
		  datathree[0] = 1;
		  cCmdMessageWriteToBluetooth(3,datathree,3,mailbox1);
		//  cCmdMessageWriteToBluetooth(1,datatone,1,mailbox1);
		  wait10Msec(2);
		  datathree[0] = 2;
		  cCmdMessageWriteToBluetooth(3,datathree,3,mailbox1);
		//  cCmdMessageWriteToBluetooth(1,datatone,1,mailbox1);
		 wait10Msec(4000);
		 datatone[0] = 3;
		 wait10Msec(2);
		datatone[0] = 0;

		}
		if(quinley == 2){
		  //donothing
		}

	  cCmdMessageWriteToBluetooth(3,chuck1dat,8,mailbox1);
	  wait1Msec(1);
	  cCmdMessageWriteToBluetooth(1,buttondat,2,mailbox1);
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
