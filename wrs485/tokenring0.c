#define HSPEED_DEBUG

#include "HSpeed.c"

task main()
{
	// Setup
	string sFriendlyName;
	getFriendlyName(sFriendlyName);
	IAmNXT = 0;
	MaxNXT = 2;
	setupHighSpeedLink();

	//
	eraseDisplay();
	bNxtLCDStatusDisplay = true; // Enable top status line display





	while(true)
	{
	  HSpeedSendByte(1, 1);

	}
}
