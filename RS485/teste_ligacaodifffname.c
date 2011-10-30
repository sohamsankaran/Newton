
#define HSPEED_DEBUG
int themess = 255;
bool new;
ubyte nMessage[3];

void checkBTLinkConnected();
void readMessages();
//task head;

#include "HSpeed.c"

task main()
{
	// Setup
	string sFriendlyName;
	getFriendlyName(sFriendlyName);
	IAmNXT = 0;
	MaxNXT = 3;
	setupHighSpeedLink();

	//
	eraseDisplay();
	bNxtLCDStatusDisplay = true; // Enable top status line display

	nxtDisplayTextLine(2, "IAmNXT: %d MAX: %d", IAmNXT, MaxNXT);

	while(true)
	{
	  checkBTLinkConnected();
	  readMessages();

	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        Receive Messages
////////////////////////////////////////////////////////////////////////////////////////////////////////

void readMessages()
{





	  cCmdMessageRead(nMessage, 3, mailbox1);



	  HSpeedSendByte(1, nMessage[0]);
	  HSpeedSendByte(2, nMessage[1]);






}



////////////////////////////////////////////////////////////////////////////////////////////////////////

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
