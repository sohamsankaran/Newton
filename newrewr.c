#pragma config(Sensor, S1,     HTMC,                sensorLowSpeed)

#include "/drivers/HTMC-driver.h"

byte CMDNunchukInit[] = {0x03, 0xA4, 0x40, 0x00};
byte CMDNunchukWrite[] = {0x02, 0xA4, 0x00};
byte CMDNunchukRead[] = {0x01, 0xA4};

byte outbuf[6];
int outbuf1[6];   //ubyte workaround for RobotC

long nNearEndSent = 0;
long nSendBusy    = 0;
long nMsgXmit     = 0;
int cmps = 0;



//  This function allows conversion of an unsigned byte to a signed int
//  This is a workaround for RobotC
int ubyteToInt(ubyte _byte) {
   int _ret = 0;
   _ret = (_byte & 0x80) ? (_byte & 0x7F) + 0x80 : _byte;
   return _ret;
}

sub NunchukGetParamter(tSensors port, byte i2c_cmd)
{
  // Initialize the Nunchuk
   SensorType[port] = sensorI2CCustom;

   sendI2CMsg(port, CMDNunchukInit[0], 0);

  while (nI2CStatus[port] == STAT_COMM_PENDING) ;


  CMDNunchukWrite[2] =i2c_cmd;


   sendI2CMsg(port, CMDNunchukWrite[0], 0);


  while (nI2CStatus[port] == STAT_COMM_PENDING) ;
  byte count = 6;

   sendI2CMsg(port, CMDNunchukRead[0], count);


  while (nI2CStatus[port] == STAT_COMM_PENDING) ; // Wait for I2C bus to be ready


  if(nI2CBytesReady[port] == count){


     readI2CReply(port, outbuf[0], 6);

    for(int i=0; i<count; i++ ){
       outbuf1[i]=ubyteToInt(outbuf[i]);   //ubyte workaround for RobotC
      outbuf1[i]=(outbuf1[i]^0x17) + 0x17;
    }
  }
  else {

     memset(outbuf, 0, 0);
  }
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

task compass () {
  int _target = 0;

  nxtDisplayCenteredTextLine(0, "HiTechnic");
  nxtDisplayCenteredBigTextLine(1, "Compass");
  nxtDisplayCenteredTextLine(3, "Test 1");
  nxtDisplayTextLine(5, "Press enter");
  nxtDisplayTextLine(6, "to set target");

  wait1Msec(2000);
  eraseDisplay();
  time1[T1] = 0;
  while(true) {
    // Reset the target no more than once a second
    // This also helps with debouncing the [enter] button.
    if (time1[T1] > 1000) {
      eraseDisplay();
      nxtDisplayTextLine(1, "Changing");
      nxtDisplayTextLine(2, "target");
      wait1Msec(500);
      // Set the current heading as the value for the offset to be used as the
      // new zero-point for the relative heading returned by
      // HTMCreadRelativeHeading()
      _target = HTMCsetTarget(HTMC);
      PlaySound(soundBlip);
      while(bSoundActive);
      time1[T1] = 0;
    }

    // Get the true heading and relative heading from the sensor and
    // display them on the screen.
    while(nNxtButtonPressed != kEnterButton) {
      eraseDisplay();
      nxtDisplayTextLine(1, "Reading");
      nxtDisplayTextLine(2, "Target: %4d", _target);
      nxtDisplayTextLine(4, "Abs:   %4d", HTMCreadHeading(HTMC));
      nxtDisplayTextLine(5, "Rel:   %4d", HTMCreadRelativeHeading(HTMC));
      cmps = HTMCreadRelativeHeading(HTMC);
      nxtDisplayTextLine(6, "Press enter");
      nxtDisplayTextLine(7, "to set target");
      wait1Msec(100);
    }
  }
}


task sendMessages()
{
	const int kMinTimeBetweenMsg = 50;
	static long nLastSendTime = 0;

	while (true)
	{
		//
		// Delay between messages.
		//

	  checkBTLinkConnected();
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

			ubyte krik[4]={0,0,0,0};
			if(cmps==0){
			  krik[0] = 0;
			  krik[1] = 0;
			}
			if(cmps>0){
			  krik[0] = cmps;
			  krik[1] = 0;
			}
			if(cmps<0){
			  krik[1] = -cmps;
			  krik[0] = 0;
			}


			 NunchukGetParamter(S3, 0x00);
			  if (sizeof(outbuf)==6)    {



			if((outbuf1[5]&0x02)!=0){
			   krik[2] = 1;
			}

			if((outbuf1[5]&0x01)!=0){
			  krik[3] = 1;
			}
		}




	  cCmdMessageWriteToBluetooth(3,krik,4,mailbox1);

	  wait10Msec(1);

	  nLastSendTime = nPgmTime;
	}
	return;
}



task main()
{

	StartTask(compass);
	StartTask(sendMessages);
  wait1Msec(1);

while(true){
}
}
