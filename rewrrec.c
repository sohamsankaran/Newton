#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S2,     HTMC,                sensorLowSpeed)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorNormal, openLoop)

/*
rewrrec by soham sankaran
version 0.1
first worked 23-10-2011
Based on htmc-test1 by xander soldaat and the NXT BT Messaging example
*/

#include "drivers/HTMC-driver.h"

long nLastXmitTimeStamp = nPgmTime;
long nDeltaTime         = 0;
int cmps;


bool flagged = true;
#define MAXMOTORSPEED 50
int _target = 0;

int mSD, mSE, mSF, mSG;



int round(float f)
{
  return (f>0)?(int)(f+0.5):(int)(f - 0.5);
}

void runMotorSpeeds(int &motorSpeedD, int &motorSpeedE, int &motorSpeedF, int &motorSpeedG, int angle, int Vb) {
  float Vw1, Vw2, Vw3, Vw4, norm_factor;

  Vw1 = Vb*cosDegrees(angle);
  Vw2 = Vb*sinDegrees(angle);
  Vw3 = -Vw1;
  Vw4 = -Vw2;

  norm_factor = 1.0;

  if (Vw1 > MAXMOTORSPEED) {
    norm_factor = MAXMOTORSPEED / Vw1;
  } else if (Vw2 > MAXMOTORSPEED) {
    norm_factor = MAXMOTORSPEED / Vw2;
  } else if (Vw3 > MAXMOTORSPEED) {
    norm_factor = MAXMOTORSPEED / Vw3;
  } else if (Vw4 > MAXMOTORSPEED) {
    norm_factor = MAXMOTORSPEED / Vw4;
  }

  motorSpeedD = round(Vw1 * norm_factor);
  motorSpeedE = round(Vw2 * norm_factor);
  motorSpeedF = round(Vw3 * norm_factor);
  motorSpeedG = round(Vw4 * norm_factor);

  motor[motorD] = motorSpeedD;
  motor[motorE] = motorSpeedE;
  motor[motorF] = motorSpeedF;
  motor[motorG] = motorSpeedG;
}

void stopMotors()
{
  motor[motorD] = 0;
  motor[motorE] = 0;
  motor[motorF] = 0;
  motor[motorG] = 0;
}

void turntocmps(int lcmps){



  if(HTMCreadRelativeHeading(HTMC) > lcmps){

    if(HTMCreadRelativeHeading(HTMC) != lcmps){
      flagged = false;
     motor[motorD] = -15;
     motor[motorE] = 15;
     motor[motorF] = -15;
     motor[motorG] = 15;

 }

   }


  // stopMotors();

   if(HTMCreadRelativeHeading(HTMC) < lcmps){

     if(HTMCreadRelativeHeading(HTMC) != lcmps){
     flagged = false;
     motor[motorD] = 15;
     motor[motorE] = -15;
     motor[motorF] = 15;
     motor[motorG] = -15;

   }

}

 if(HTMCreadRelativeHeading(HTMC)-lcmps < 5 || -HTMCreadRelativeHeading(HTMC)+lcmps < 5){
   flagged = true;
 }
   }
  // stopMotors();


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Check to See if Bluetooth Link is Connected
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

void checkBTLinkConnected()
{
  if (nBTCurrentStreamIndex >= 0)
    return;

  PlaySound(soundLowBuzz);
  PlaySound(soundLowBuzz);
  eraseDisplay();
  nxtDisplayCenteredTextLine(3, "BT not");
  nxtDisplayCenteredTextLine(4, "Connected");
  wait1Msec(3000);
  StopAllTasks();
}



const int kMaxSizeOfMessage = 3;
const TMailboxIDs kQueueID = mailbox1;



////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                        Receive Messages Function
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

void readMultipleDataMsgs()
{
  TFileIOResult nBTCmdRdErrorStatus;
  int nSizeOfMessage;
  ubyte nRcvBuffer[kMaxSizeOfMessage];

  while (true)
  {
    // Check to see if a message is available

    nSizeOfMessage = cCmdMessageGetSize(kQueueID);
    if (nSizeOfMessage <= 0)
    {
      wait1Msec(1);    // Give other tasks a chance to run
      break;           // No more message this time
    }

    if (nSizeOfMessage > kMaxSizeOfMessage)
      nSizeOfMessage = kMaxSizeOfMessage;
    nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, kQueueID);





     cmps = nRcvBuffer[0]-nRcvBuffer[1];
     nxtDisplayTextLine(3, "button:   %4d", nRcvBuffer[2]);
     nxtDisplayTextLine(4, "Abs:   %4d", cmps);
     nxtDisplayTextLine(5, "curr:   %4d", HTMCreadRelativeHeading(HTMC));
     if(nRcvBuffer[2]==1){
       //stopMotors();
       runMotorSpeeds(mSD, mSE, mSF, mSG, 0, 25);
       flagged = true;
     }
    // else{
    // stopMotors();
     else{
      if (cmps-HTMCreadRelativeHeading(HTMC) > 5 || HTMCreadRelativeHeading(HTMC)-cmps > 5)
     {
     flagged = false;
    }





     if(flagged == true){

      stopMotors();
    }

    else{
      turntocmps(cmps);
    }
  }
  // }






     //}
  }
  return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                        Main Task
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
  bNxtLCDStatusDisplay = true;
  HTMCsetTarget(HTMC);
  wait10Msec(10);
  while(true){
    checkBTLinkConnected();
    readMultipleDataMsgs();
    wait1Msec(1);
  }

  return;
}
