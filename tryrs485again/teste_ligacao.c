////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                         Port S4 High-Speed RS-485 Full Duplex Token Ring Tester
//
//  Program to test the code
//
// Autor  : Tiago Costa Gonçalves (ogait87)
// Release: 0.0.2
// Date   : 07/11/2008
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#define HSPEED_DEBUG

#include "HSpeed.c"

task main()
{
	// Setup
	string sFriendlyName;
	getFriendlyName(sFriendlyName);
	IAmNXT = sFriendlyName == "HRF2" ? 0 : 1;
	MaxNXT = 2;
	setupHighSpeedLink();

	//
	eraseDisplay();
	bNxtLCDStatusDisplay = true; // Enable top status line display

	nxtDisplayTextLine(2, "IAmNXT: %d MAX: %d", IAmNXT, MaxNXT);

	while(true)
	{
		wait1Msec(10);
	}
}
