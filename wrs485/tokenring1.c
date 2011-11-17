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
	IAmNXT = 1;
	MaxNXT = 2;
	setupHighSpeedLink();

	//
	eraseDisplay();
	bNxtLCDStatusDisplay = true; // Enable top status line display





	while(true)
	{
	  while( HSpeedRxDataLen == 0 ) wait1Msec(10);
	  nxtDisplayTextLine(2, "%d", HSpeedRxData[0]);

	}
}
