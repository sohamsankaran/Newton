
#define HSPEED_DEBUG
#define ALL_OK 1
#define FAIL_TO_LOCATE 2
#define OBJECT_PICKED_UP 3
#define NO_MESSAGE 0
#include "HSpeed.c"
int status = ALL_OK;
int rec = NO_MESSAGE;


task main()
{

	// Setup

	IAmNXT = 2;
	MaxNXT = 3;
	setupHighSpeedLink();

	//
	eraseDisplay();
	bNxtLCDStatusDisplay = true; // Enable top status line display

	nxtDisplayTextLine(2, "IAmNXT: %d MAX: %d", IAmNXT, MaxNXT);

		while(true){

if( HSpeedRxDataLen != 0  )
		{
		 // rec = (int)HSpeedRxData[0];
		  nxtDisplayTextLine(6, "Rx: ID %d Len %d", HSpeedRxHead[0]     , HSpeedRxDataLen);
			nxtDisplayTextLine(7, "Rx: %d "       , (int)HSpeedRxData[0] );
		}
		status = FAIL_TO_LOCATE;
		HSpeedSendByte( (IAmNXT+1) % MaxNXT , status);
	}
}
