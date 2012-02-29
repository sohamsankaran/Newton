#pragma config(Hubs,  S1, HTServo,  HTMotor,  none,     none)
#pragma config(Sensor, S2,     Lct,                 sensorLightActive)
#pragma config(Motor,  motorA,          motorA,        tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorB,          motorB,        tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorC,          motorC,        tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     motorD,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorE,        tmotorNormal, openLoop)
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

	IAmNXT = 1;
	MaxNXT = 2;
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
			if((int)HSpeedRxData[0] == 2){

			  nMotorEncoder[motorC] = 0;
  while (nMotorEncoder[motorC] > -90)

  {

    motor[motorE] =  -50;

  }



  servo[servo2] = 255;
  servo[servo3] = 50;
  servo[servo4] = 255;
  servo[servo5] = 128;
  servo[servo6] = 0;
  wait1Msec(10000);
  servo[servo2] = 100;
  servo[servo3] = 100;
  servo[servo4] = 100;
  servo[servo5] = 190;
  servo[servo6] = 100;
  wait1Msec(10000);
  //servo[servo2] = 0;

 // servo[servo2] = 0;


			}
		}
		status = OBJECT_PICKED_UP;
		HSpeedSendByte( (IAmNXT+1) % MaxNXT , status);
	}
}
