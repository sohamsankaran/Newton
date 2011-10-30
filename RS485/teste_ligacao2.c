////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                         Port S4 High-Speed RS-485 Full Duplex Token Ring Tester
//
//  Program to test the efficiency of the HSpeed Connection
//
// Autor  : Tiago Costa Gonçalves (ogait87)
// Release: 0.0.2
// Date   : 07/11/2008
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HSPEED_TIMEOUT_BASE 10000
//#define HSPEED_DEBUG

#include "HSpeed.c"

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                        recieve Data
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
void recieve()
{
	int counta = 0;

	while(true)
	{
		if( HSpeedRxDataLen != 0  )
		{
			counta++;
			nxtDisplayTextLine(6, "Rx: ID %d Len %d", HSpeedRxHead[0]     , HSpeedRxDataLen);
			nxtDisplayTextLine(7, "Rx: %d %d"       , (int)HSpeedRxData[0], counta          );
			HSpeedRxDataLen = 0;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                        send Data
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
void send()
{
	int count = 0;

	ClearTimer(T2);
	while(true)
	{
		if( time1[T2] > 20 )
		{
			count++;
			HSpeedSendByte( (IAmNXT+1) % MaxNXT, count);
			nxtDisplayTextLine(4, "Send: %d", count);
			ClearTimer(T2);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                        Main Task
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	if ( IAmNXT == 0 )
	{
		send();
	}
	else
	{
		recieve();
	}
}
