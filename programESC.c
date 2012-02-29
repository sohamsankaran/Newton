#pragma config(Hubs,  S1, HTServo,  none, none,     none)
#pragma config(Servo,  srvo_S1_C1_1,    servo1,                     tServoNormal)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*--------------------------------------------------------------------------------------------------------*\
|*                                                                                                        *|
|*                                    - Servo Control by NXT Motor -                                      *|
|*                                        ROBOTC on NXT + TETRIX                                          *|
|*                                                                                                        *|
|*  This program simply sweeps the servo back and forth.  Servo Change Rate can be changed by changing    *|
|*  'delta'.  (0 = max servo speed, # > 0 = how many servo positions to move per update (every 20ms).     *|
|*                                                                                                        *|
|*                                        ROBOT CONFIGURATION                                             *|
|*    NOTES:                                                                                              *|
|*    1)  Be sure to attach a servo somewhere on the robot.                                               *|
|*                                                                                                        *|
|*    MOTORS & SENSORS:                                                                                   *|
|*    [I/O Port]              [Name]              [Type]              [Description]                       *|
|*    Port 1                  none                TETRIX Controller   TETRIX                              *|
|*    Port 1 - Chan. 1        servo1              std. Servo          The servo that you wish to test.    *|
\*---------------------------------------------------------------------------------------------------4246-*/
task main()
{
  servo[servo1] = 255;
    wait1Msec(11000);                          // Wait 10 seconds.
    servo[servo1] = 0;
    wait1Msec(4000);                          // Wait 10 seconds.
    servo[servo1] = 128;
    wait1Msec(4000);                          // Wait 10 seconds.
    servo[servo1] = 0;
    wait1Msec(10000);                          // Wait 10 seconds.
    servo[servo1] = 255;
    wait1Msec(4000);                          // Wait 10 seconds.
    servo[servo1] = 0;
    wait1Msec(10000);                          // Wait 10 seconds.
    servo[servo1] = 128;
    wait1Msec(4000);                          // Wait 10 seconds.
    servo[servo1] = 0;
    wait1Msec(10000);                          // Wait 10 seconds.
    servo[servo1] = 255;
   wait1Msec(4000);                          // Wait 10 seconds.
    servo[servo1] = 0;
    wait1Msec(10000);                          // Wait 10 seconds.

}
