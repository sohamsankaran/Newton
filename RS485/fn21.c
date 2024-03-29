#pragma config(Motor,  motorA,          RND,           tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,          UP,            tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  motorC,          LEG,           tmotorNormal, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
#pragma config(Hubs,  S1, HTServo,  none,     none,     none)
#pragma config(Servo,  srvo_S1_C1_1,    ,                     tServoNormal)
#pragma config(Servo,  srvo_S1_C1_2,    ,                     tServoNormal)
#pragma config(Servo,  srvo_S1_C1_3,    ,                     tServoNormal)
#pragma config(Servo,  srvo_S1_C1_4,    ,                     tServoNormal)
#pragma config(Servo,  srvo_S1_C1_5,    ,                     tServoNormal)

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
	IAmNXT = 2;
	MaxNXT = 3;
	setupHighSpeedLink();

	//
	eraseDisplay();
	bNxtLCDStatusDisplay = true; // Enable top status line displayHSpeedRxData[1]

int delta = 2;                        // Create int 'delta' to the be Servo Change Rate.

servoChangeRate[servo1] = delta;          // Slow the Servo Change Rate down to only 'delta' positions per update.
    servoChangeRate[servo2] = delta;          // Slow the Servo Change Rate down to only 'delta' positions per update.
    servoChangeRate[servo3] = delta;          // Slow the Servo Change Rate down to only 'delta' positions per update.
    servoChangeRate[servo4] = delta;          // Slow the Servo Change Rate down to only 'delta' positions per update.
    servoChangeRate[servo5] = delta;          // Slow the Servo Change Rate down to only 'delta' positions per update.



	while(true)
	{



	  while( HSpeedRxDataLen == 0 ) wait1Msec(10);

	if(HSpeedRxData[0] == 1){

	   AbsMove(2000,2000, 360);

	  if(ServoValue[servo1] >= 191)             // If servo1 is closer to 255 (than 0):
    {
      while(ServoValue[servo1] > 125)                 // While the ServoValue of servo1 is greater than 0:
      {
        servo[servo1] = 125;                                // Move servo1 to position to 0.
      }
    }

     if(ServoValue[servo2] < 64)              // If servo1 is closer to 0 (than 255):
    {
      while(ServoValue[servo2] < 110)               // While the ServoValue of servo1 is less than 255:
      {
        servo[servo2] = 110;                              // Move servo1 to position to 255.
      }
    }

     if(ServoValue[servo3] < 64)              // If servo1 is closer to 0 (than 255):
    {
      while(ServoValue[servo3] < 110)               // While the ServoValue of servo1 is less than 255:
      {
        servo[servo3] = 110;                              // Move servo1 to position to 255.
      }
    }

     if(ServoValue[servo4] < 64)              // If servo1 is closer to 0 (than 255):
    {
      while(ServoValue[servo4] < 110)               // While the ServoValue of servo1 is less than 255:
      {
        servo[servo4] = 110;                              // Move servo1 to position to 255.
      }
    }

     if(ServoValue[servo5] < 64)              // If servo1 is closer to 0 (than 255):
    {
      while(ServoValue[servo5] < 110)               // While the ServoValue of servo1 is less than 255:
      {
        servo[servo5] = 110;                              // Move servo1 to position to 255.
      }
    }




	}

	if(HSpeedRxData[0] == 2){


	    if(ServoValue[servo1] < 191)              // If servo1 is closer to 0 (than 255):
    {
      while(ServoValue[servo1] < 255)               // While the ServoValue of servo1 is less than 255:
      {
        servo[servo1] = 255;                              // Move servo1 to position to 255.
      }
    }

     if(ServoValue[servo2] >= 64)             // If servo1 is closer to 255 (than 0):
    {
      while(ServoValue[servo2] > 0)                 // While the ServoValue of servo1 is greater than 0:
      {
        servo[servo2] = 0;                                // Move servo1 to position to 0.
      }
    }

     if(ServoValue[servo3] >= 64)             // If servo1 is closer to 255 (than 0):
    {
      while(ServoValue[servo3] > 0)                 // While the ServoValue of servo1 is greater than 0:
      {
        servo[servo3] = 0;                                // Move servo1 to position to 0.
      }
    }

     if(ServoValue[servo4] >= 64)             // If servo1 is closer to 255 (than 0):
    {
      while(ServoValue[servo4] > 0)                 // While the ServoValue of servo1 is greater than 0:
      {
        servo[servo4] = 0;                                // Move servo1 to position to 0.
      }
    }

     if(ServoValue[servo5] >= 64)             // If servo1 is closer to 255 (than 0):
    {
      while(ServoValue[servo5] > 0)                 // While the ServoValue of servo1 is greater than 0:
      {
        servo[servo5] = 0;                                // Move servo1 to position to 0.
      }
    }


    AbsMove(0,0, 0);


	}



	if(HSpeedRxData[0] == 3){


	   AbsMove(0,0, 0);


	}

	}
}
