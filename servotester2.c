#pragma config(Hubs,  S1, HTServo,  HTMotor,  none,     none)
#pragma config(Sensor, S2,     lightR,              sensorLightActive)
#pragma config(Motor,  mtr_S1_C2_1,     motorD,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorE,        tmotorNormal, openLoop)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,                     tServoStandard)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,                     tServoStandard)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,                     tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    servo3,                     tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    servo2,                     tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
 /* nMotorEncoder[motorC] = 0;
  while (nMotorEncoder[motorC] > -90)

  {

    motor[motorE] =  -100;

  }

  motor[motorD] =  -40;*/

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
