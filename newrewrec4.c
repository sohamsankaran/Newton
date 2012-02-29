#pragma config(Hubs,  S1, HTServo, HTMotor,  HTMotor,  none)
#pragma config(Sensor, S2,     HTMC,                sensorLowSpeed)
#pragma config(Sensor, S3,     cam,                 sensorI2CCustomFastSkipStates)
#pragma config(Motor,  mtr_S1_C2_1,     motorD,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorE,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     motorF,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorG,        tmotorNormal, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    servo1,                     tServoNormal)

/*
rewrrec by soham sankaran
version 0.1
first worked 23-10-2011
Based on htmc-test1 by xander soldaat and the NXT BT Messaging example
*/

#include "drivers/HTMC-driver.h"
#include "drivers/NXTCAM-driver.h"

#define C_A 10
#define C_STOP 65

long nLastXmitTimeStamp = nPgmTime;
long nDeltaTime         = 0;
int cmps;


bool flagged = true;
#define MAXMOTORSPEED 50
int _target = 0;

int mSD, mSE, mSF, mSG;

// int xscale(int x) - Scales x values from camera coordinates to screen coordinates.
int xscale(int x) {
  return ((x - 12) * 99) / 175;
}

// int yscale(int y) - Scales y values from camera coordinates to screen coordinates.
int yscale(int y) {
  return ((143 - y) * 63) / 143;
}


int roundit(float f)
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

  motorSpeedD = roundit(Vw1 * norm_factor);
  motorSpeedE = roundit(Vw2 * norm_factor);
  motorSpeedF = roundit(Vw3 * norm_factor);
  motorSpeedG = roundit(Vw4 * norm_factor);

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

void turntocmps(int gcmps, int loxmps){

  int lcmps;
  if (gcmps-HTMCreadRelativeHeading(HTMC) >= 180){lcmps = (gcmps-HTMCreadRelativeHeading(HTMC)) - 360;}
  if (gcmps-HTMCreadRelativeHeading(HTMC) < 180 && gcmps-HTMCreadRelativeHeading(HTMC) >= 0){lcmps = gcmps-HTMCreadRelativeHeading(HTMC) ;}
  if (gcmps-HTMCreadRelativeHeading(HTMC) >= -180 && gcmps-HTMCreadRelativeHeading(HTMC) < 0){lcmps = gcmps-HTMCreadRelativeHeading(HTMC);}
  if (gcmps-HTMCreadRelativeHeading(HTMC) < -180){lcmps = 360+(gcmps-HTMCreadRelativeHeading(HTMC)) ;}




  if(lcmps > 0){

    if(HTMCreadRelativeHeading(HTMC) != loxmps){
      flagged = false;
     motor[motorD] = 15;
     motor[motorE] = 15;
     motor[motorF] = 15;
     motor[motorG] = 15;

 }

   }


  // stopMotors();

   if(lcmps < 0){

     if(HTMCreadRelativeHeading(HTMC) != loxmps){
     flagged = false;
     motor[motorD] = -15;
     motor[motorE] = -15;
     motor[motorF] = -15;
     motor[motorG] = -15;

   }

}

  if (gcmps-HTMCreadRelativeHeading(HTMC) >= 180){lcmps = (gcmps-HTMCreadRelativeHeading(HTMC)) - 360;}
  if (gcmps-HTMCreadRelativeHeading(HTMC) < 180 && gcmps-HTMCreadRelativeHeading(HTMC) >= 0){lcmps = gcmps-HTMCreadRelativeHeading(HTMC) ;}
  if (gcmps-HTMCreadRelativeHeading(HTMC) >= -180 && gcmps-HTMCreadRelativeHeading(HTMC) < 0){lcmps = gcmps-HTMCreadRelativeHeading(HTMC);}
  if (gcmps-HTMCreadRelativeHeading(HTMC) < -180){lcmps = 360+(gcmps-HTMCreadRelativeHeading(HTMC)) ;}

 if(abs(loxmps-HTMCreadRelativeHeading(HTMC)) < 5){
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







////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                        Receive Messages Function
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

void readMultipleDataMsgs()
{
  const int kMaxSizeOfMessage = 4;
const TMailboxIDs kQueueID = mailbox1;
  TFileIOResult nBTCmdRdErrorStatus;
  int nSizeOfMessage;
  ubyte nRcvBuffer[kMaxSizeOfMessage];

  while (true)
  {
    // Check to see if a message is available

    nSizeOfMessage = cCmdMessageGetSize(kQueueID);
  /*  if (nSizeOfMessage <= 0)
    {
      wait1Msec(1);    // Give other tasks a chance to run
      break;           // No more message this time
    }
*/
    if (nSizeOfMessage > kMaxSizeOfMessage){
      nSizeOfMessage = kMaxSizeOfMessage;}
    nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, kQueueID);





     cmps = 180+(nRcvBuffer[0]-nRcvBuffer[1]);
    int oxmps;
   if(nRcvBuffer[1] == 0){oxmps = nRcvBuffer[0];}
   if(nRcvBuffer[0] == 0){oxmps = -nRcvBuffer[1];}

     nxtDisplayTextLine(3, "button:   %4d", nRcvBuffer[2]);
     nxtDisplayTextLine(4, "Abs:   %4d", oxmps);
     nxtDisplayTextLine(5, "curr:   %4d", HTMCreadRelativeHeading(HTMC));
     if(nRcvBuffer[2]==1){
       int mspeed = 0;
       int mangle = 0;
       //stopMotors();
         blob_array _blobs;
         // combine all colliding blobs into one
         bool _condensed = true;

          //blob_array _blobs;
         int _l, _t, _r, _b;
         int _nblobs;
         eraseDisplay();

            // Initialise the camera
           NXTCAMinit(cam);
           servo[servo1] = 35;
           while(true){
              eraseDisplay();

              _nblobs = NXTCAMgetBlobs(cam, _blobs, _condensed);
              // Fetch all the blobs, have the driver combine all
              // the colliding blobs.
//
              if(_nblobs>0){

                int cx = 176-(SIDE_CENTER(_blobs[0].x1, _blobs[0].x2));
                float cy = SIDE_CENTER(_blobs[0].y1, _blobs[0].y2);
                float dx = cx-88;


                  mspeed=50;
                  if(cy > C_STOP){
              /*      float dy = ((65-cy)/C_STOP)*50;
                mspeed = dy;
              }
                else{*/
                    mspeed = 0;
                  }

                  float rathura = (dx*90)/88;
                mangle = rathura;


                      // Draw the scaled blobs
                          _l = xscale(_blobs[0].x1);
                          _t = yscale(_blobs[0].y1);
                         _r = xscale(_blobs[0].x2);
                          _b = yscale(_blobs[0].y2);
                        nxtFillRect(_l, _t, _r, _b);


                  nxtDisplayTextLine(1, "%d", _nblobs);
                  nxtDisplayTextLine(2, "%d", mangle);
                  nxtDisplayTextLine(3, "%d", (int)dx);
                  runMotorSpeeds(mSD, mSE, mSF, mSG, mangle, mspeed);
                  _nblobs = 0;
                   wait1Msec(100);


                 }

            nSizeOfMessage = cCmdMessageGetSize(kQueueID);
           /* if (nSizeOfMessage <= 0)
           {
           wait1Msec(1);    // Give other tasks a chance to run
           break;           // No more message this time
           }*/

          if (nSizeOfMessage > kMaxSizeOfMessage){
          nSizeOfMessage = kMaxSizeOfMessage;}
          nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, kQueueID);

		  flagged = true;
}
}




     if(nRcvBuffer[3]==1){
       int mspeed = 0;
       int mangle = 0;
       //stopMotors();
         blob_array _blobs;
         // combine all colliding blobs into one
         bool _condensed = true;

          //blob_array _blobs;
         int _l, _t, _r, _b;
         int _nblobs;
         eraseDisplay();

            // Initialise the camera
           NXTCAMinit(cam);
           servo[servo1] = 35;
             while(true) {
              eraseDisplay();

              _nblobs = NXTCAMgetBlobs(cam, _blobs, _condensed);
              // Fetch all the blobs, have the driver combine all
              // the colliding blobs.



              if(_nblobs>0){

                for (int i = 0; i < _nblobs; i++) {
             // replace blob 0 with the biggest blob of the second colour
              if (_blobs[i].colour == 1){
              _blobs[0] = _blobs[i];
            }

              }

                int cx = 176-(SIDE_CENTER(_blobs[0].x1, _blobs[0].x2));
                float cy = SIDE_CENTER(_blobs[0].y1, _blobs[0].y2);
                float dx = cx-88;


                  mspeed=50;
                  if(cy > C_STOP){
              /*      float dy = ((65-cy)/C_STOP)*50;
                mspeed = dy;
              }
                else{*/
                    mspeed = 0;
                  }

                  float rathura = (dx*90)/88;
                mangle = rathura;




                      // Draw the scaled blobs
                          _l = xscale(_blobs[0].x1);
                          _t = yscale(_blobs[0].y1);
                         _r = xscale(_blobs[0].x2);
                          _b = yscale(_blobs[0].y2);
                        nxtFillRect(_l, _t, _r, _b);


                  nxtDisplayTextLine(1, "%d", _nblobs);
                  nxtDisplayTextLine(2, "%d", mangle);
                  nxtDisplayTextLine(3, "%d", (int)dx);
                  runMotorSpeeds(mSD, mSE, mSF, mSG, mangle, mspeed);
                  _nblobs = 0;
                   wait1Msec(100);
                 }



            nSizeOfMessage = cCmdMessageGetSize(kQueueID);
          /*  if (nSizeOfMessage <= 0)
           {
           wait1Msec(1);    // Give other tasks a chance to run
           break;           // No more message this time
           }*/

          if (nSizeOfMessage > kMaxSizeOfMessage){
          nSizeOfMessage = kMaxSizeOfMessage;}
          nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, kQueueID);
          }
		  flagged = true;
}




   else{
      if (abs(oxmps-HTMCreadRelativeHeading(HTMC)) > 5)
     {
     flagged = false;
    }
  }






     if(flagged == true){

      stopMotors();
    }

    else{
      turntocmps(cmps, oxmps);
    }









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
 //HTMCsetTarget(HTMC);
 /*while(HTMCreadRelativeHeading(HTMC)>-90){
    motor[motorD] =  15;
     motor[motorE] = 15;
     motor[motorF] = 15;
     motor[motorG] = 15;
  }*/
  stopMotors();
  HTMCsetTarget(HTMC);




  wait10Msec(10);
  while(true){
    checkBTLinkConnected();
    readMultipleDataMsgs();
    wait1Msec(1);
  }

  return;
}
