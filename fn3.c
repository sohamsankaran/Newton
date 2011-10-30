#pragma config(Motor,  motorA,          RND,           tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorB,          UP,            tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorC,          LEG,           tmotorNormal, PIDControl, encoder)

#define HSPEED_DEBUG

#include "HSpeed.c"

#define RANGE 3
int distUp;
int distRnd;
int distLeg;
ubyte Rec[3];
int LRec[3];
int fRec[3];
bool new;

//void checkBTLinkConnected();

//task get;

int AbsMove(int MUp, int MRnd, int MLeg){
  int testRnd = 0;
  int testUp = 0;
  int testLeg = 0;
  while(testUp==0 || testRnd==0 || testLeg==0){
    //if(new){
      //break;
    //}
    nxtDisplayTextLine(1,"UP = %d",nMotorEncoder[UP]);
    nxtDisplayTextLine(2,"RND = %d",nMotorEncoder[RND]);
    distUp = MUp-nMotorEncoder[UP];
    if(MUp != nMotorEncoder[UP]){
     motor[UP] = distUp;
    }
    else{
      testUp = 1;
      motor[UP] = 0;
    }

    distRnd = MRnd-nMotorEncoder[RND];
    if(MRnd != nMotorEncoder[RND]){
      motor[RND] = distRnd;
    }
    else{
      testRnd = 1;
      motor[RND] = 0;
    }

    distLeg = MLeg-nMotorEncoder[LEG];
    if(MLeg != nMotorEncoder[LEG]){
     motor[LEG] = distLeg;
    }
    else{
      testLeg = 1;
      motor[LEG] = 0;
    }
  }
  motor[UP]= 0;
  motor[RND]= 0;
  motor[LEG]= 0;
  return 0;
}



task main()
{
	// Setup
	string sFriendlyName;
	IAmNXT = 1;
	MaxNXT = 4;
	setupHighSpeedLink();

	//
	eraseDisplay();
	bNxtLCDStatusDisplay = true; // Enable top status line displayHSpeedRxData[1]





	while(true)
	{
	  while( HSpeedRxDataLen == 0 ) wait1Msec(10);

	if(HSpeedRxData[0] == 1){


	}

	if(HSpeedRxData[0] == 2){


	}



	if(HSpeedRxData[0] == 3){


	}

	}
}
