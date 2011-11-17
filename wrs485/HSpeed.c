////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                         Port S4 High-Speed RS-485 Full Duplex Token Ring
//
// Autor  : Tiago Costa Gonçalves (ogait87)
// Release: 0.0.5
// Date   : 05/10/2008
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
int  MaxNXT = 0;
int  IAmNXT = 0;

task HSpeed;

void setupHighSpeedLink();

void HSpeedSendByte(ubyte destino, ubyte dados);

bool HSpeedRecieveData(ubyte &sender, ubyte &len, ubyte *msg);

/////////////////////////////////////////////////////////////////////////////////////////
//                                     Private
/////////////////////////////////////////////////////////////////////////////////////////
ubyte  HSpeedTxHead[1];
ubyte  HSpeedTxData[16];
ubyte  HSpeedTxLen = 0;

ubyte  HSpeedRxHead[1];
ubyte  HSpeedRxData[16];
ubyte  HSpeedRxDataLen = 0;

//
// Initialize port S4 to "high speed" mode.
//
void setupHighSpeedLink()
{
	nxtEnableHSPort();
	//nxtSetHSBaudRate();
	nxtHS_Mode = hsMsgModeMaster ;
	nxtHS_Mode = hsRawMode;

	StartTask(HSpeed);

	return;
}

void HSpeedSendByte(ubyte destino, ubyte dados)
{
  while(HSpeedTxLen != 0)
    wait10Msec(1);

  HSpeedTxHead[0] = (IAmNXT << 6) | (destino << 4) | 1;
  HSpeedTxData[0] = dados;
  HSpeedTxLen     = 1;
}

bool HSpeedRecieveData(ubyte &sender, ubyte &len, ubyte *msg)
{
	if ( HSpeedRxDataLen == 0 )
	{
		return false;
	}

  sender = HSpeedTxHead[0];
  memcpy(msg, HSpeedRxData, HSpeedRxDataLen);
  len = HSpeedRxDataLen;

  HSpeedRxDataLen = 0;

  return true;
}

bool HSpeedAvailHSByte(int timeout = 0)
{
  ClearTimer(T1);

  while( time1[T1] < timeout )
  {
  	if ( nxtGetAvailHSBytes() > 0 )
  	  return true;

  	wait1Msec(1);
  }

  return false;
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
  	nxtWriteRawHS(HSpeedTxHead, 1,           0);
  	nxtWriteRawHS(HSpeedTxData, HSpeedTxLen, 0);
  	HSpeedTxLen = 0;
  }
  else
  {
  	nxtWriteRawHS((IAmNXT << 6), 1, 0);
  }
}

task HSpeed()
{
  int senderID, reciverID, length;
  int i;
	ubyte chr;

#ifdef HSPEED_DEBUG
	int head_count = 0;
	int loop_count = 0;
	int timeout_count = 0;
#endif

	while(true)
	{
#ifdef HSPEED_DEBUG
      loop_count++;
  		nxtDisplayTextLine(3, "LOOP: %d", loop_count);
#endif

  	// If there was a bug in the comunication and no one knows whos is the next to talk
		if ( !HSpeedAvailHSByte(random[HSPEED_TIMEOUT_RAND] + HSPEED_TIMEOUT_BASE) )
		{
#ifdef HSPEED_DEBUG
      timeout_count++;
			nxtDisplayTextLine(7, "TIMEOUT: %d", timeout_count);
#endif

      HSpeedMaster(); // so be me now :)
			continue;
		}

		// At least 1 byte recieved
		nxtReadRawHS(chr, 1);

		// Get Head fields
		senderID  = (chr >> 6) & 0x3;
		reciverID = (chr >> 4) & 0x3;
		length    = (chr >> 0) & 0xF;

#ifdef HSPEED_DEBUG
		nxtDisplayTextLine(4, "SRC:%d DEST:%d", senderID, reciverID);
    head_count++;
    nxtDisplayTextLine(5, "LEN: %d COUNT: %d", length, head_count);
#endif

		// If there is any data trasmited
		if( length != 0 )
		{
			for(i=0; i<length; i++)
			{
				// If someone do not send ally bytes required or there is a bug where
				if ( !HSpeedAvailHSByte(random[HSPEED_TIMEOUT_RAND] + HSPEED_TIMEOUT_BASE) )
				{
					break;
				}

				nxtReadRawHS(chr, 1);

				if ( reciverID == IAmNXT )
					HSpeedRxData[i] = chr;
			}

			if ( reciverID == IAmNXT )
			{
				HSpeedRxHead[0] = senderID;
				HSpeedRxDataLen = length;
			}
			else
			{
				HSpeedRxDataLen = 0;
			}
#ifdef HSPEED_DEBUG
			if ( reciverID == IAmNXT )
			{
				nxtDisplayTextLine(3, "FOR ME");
			}
			else
			{
				nxtDisplayTextLine(3, "IGNORE");
			}
#endif
		}

		// Sou am the next NXT to transmit?
		if ( (senderID + 1) % MaxNXT == IAmNXT )
		{
			HSpeedMaster();
		}
	}
}

#endif
