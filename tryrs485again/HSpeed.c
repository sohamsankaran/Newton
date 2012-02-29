////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                         Port S4 High-Speed RS-485 Full Duplex Token Ring
//
// Autor  : Tiago Costa Gonçalves (ogait87)
// Release: 0.0.6
// Date   : 07/11/2008
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define HSPEED_DEBUG

#ifndef __HSPEED_C__
#define __HSPEED_C__

#ifndef HSPEED_TIMEOUT_BASE
#define HSPEED_TIMEOUT_BASE 1000
#endif

#ifndef HSPEED_TIMEOUT_RAND
#define HSPEED_TIMEOUT_RAND 1000
#endif

//
// Public
//
int  MaxNXT = 1;
int  IAmNXT = 0;

ubyte HSpeedRxHead[1];
ubyte HSpeedRxData[16];
ubyte HSpeedRxDataLen = 0;

task HSpeed;

void setupHighSpeedLink();

void HSpeedSendByte(ubyte destino, ubyte data);
void HSpeedSendByte(ubyte destino, ubyte data_a, ubyte data_b);
void HSpeedSendByte(ubyte destino, ubyte data_a, ubyte data_b, ubyte data_c);

/////////////////////////////////////////////////////////////////////////////////////////
//                                     Private
/////////////////////////////////////////////////////////////////////////////////////////
ubyte HSpeedTxData[1+16];
ubyte HSpeedTxLen = 0;

ubyte chr[16];

void setupHighSpeedLink()
{
	nxtEnableHSPort();
	//nxtSetHSBaudRate();
	nxtHS_Mode = hsMsgModeMaster ;
	nxtHS_Mode = hsRawMode;

	StartTask(HSpeed);

	return;
}

void HSpeedSendByte(ubyte destino, ubyte data)
{
	while(HSpeedTxLen != 0)
		EndTimeSlice();

	HSpeedTxData[0] = (IAmNXT << 6) | (destino << 4) | 1;
	HSpeedTxData[1] = data;
	HSpeedTxLen     = 1;
}

void HSpeedSendByte(ubyte destino, ubyte data_a, ubyte data_b)
{
	while(HSpeedTxLen != 0)
		EndTimeSlice();

	HSpeedTxData[0] = (IAmNXT << 6) | (destino << 4) | 2;
	HSpeedTxData[1] = data_a;
	HSpeedTxData[2] = data_b;
	HSpeedTxLen     = 2;
}

void HSpeedSendByte(ubyte destino, ubyte data_a, ubyte data_b, ubyte data_c)
{
	while(HSpeedTxLen != 0)
		EndTimeSlice();

	HSpeedTxData[0] = (IAmNXT << 6) | (destino << 4) | 3;
	HSpeedTxData[1] = data_a;
	HSpeedTxData[2] = data_b;
	HSpeedTxData[3] = data_c;
	HSpeedTxLen     = 3;
}

void HSpeedMaster()
{
#ifdef HSPEED_DEBUG
	static int master_count = 0;
	master_count++;
	nxtDisplayTextLine(6, "MASTER: %d", master_count);
#endif

	if ( HSpeedTxLen > 0)
	{
		nxtWriteRawHS(HSpeedTxData, 1+HSpeedTxLen, 0);
		HSpeedTxLen = 0;
	}
	else
	{
		nxtWriteRawHS((IAmNXT << 6), 1, 0);
	}
}

bool HSpeed_ReadRawHS(int n)
{
	int i, timeout = HSPEED_TIMEOUT_BASE + random[HSPEED_TIMEOUT_RAND];

	ClearTimer(T1);
	for(i=0; i<n; )
	{
		i += nxtReadRawHS(chr[i], n-i);
		if ( time1[T1] >= timeout )
		{
			return false;
		}
	}

	return true;
}

task  HSpeed()
{
	int senderID, reciverID, length;

#ifdef HSPEED_DEBUG
	int head_count = 0;
	int loop_count = 0;
	int timeout_count = 0;
#endif

	if ( IAmNXT == 0 )	// So start roling the comunication
	{
		HSpeedMaster();
	}

	while(true)
	{
#ifdef HSPEED_DEBUG
		loop_count++;
  		nxtDisplayTextLine(3, "LOOP: %d", loop_count);
#endif

		// Receve 1 byte, the header
		if ( !HSpeed_ReadRawHS(1) )
		{
#ifdef HSPEED_DEBUG
			timeout_count++;
			nxtDisplayTextLine(7, "TIMEOUT1: %d", timeout_count);
#endif
			HSpeedMaster(); // so be me now :)
			continue;
		}

		// Get Head fields
		senderID  = (chr[0] >> 6) & 0x3;
		reciverID = (chr[0] >> 4) & 0x3;
		length    = (chr[0] >> 0) & 0xF;

#ifdef HSPEED_DEBUG
		nxtDisplayTextLine(4, "SRC:%d DEST:%d", senderID, reciverID);
		head_count++;
		nxtDisplayTextLine(5, "LEN: %d COUNT: %d", length, head_count);
#endif

		if ( length > 0 )
		{
			if ( !HSpeed_ReadRawHS(length) )
			{
#ifdef HSPEED_DEBUG
				timeout_count++;
				nxtDisplayTextLine(7, "TIMEOUT2: %d", timeout_count);
#endif
				HSpeedMaster(); // so be me now :)
				continue;
			}

			if ( reciverID == IAmNXT )
			{
				HSpeedRxHead[0] = senderID;
				memcpy(HSpeedRxData, chr, length);
				HSpeedRxDataLen = length;
			}
		}

		// I am the next NXT to transmit?
		if ( (senderID + 1) % MaxNXT == IAmNXT )
		{
			HSpeedMaster();
		}
	}
}

#endif
