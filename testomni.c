#pragma config(Hubs,  S1, HTServo, HTMotor,  HTMotor,  none)
#pragma config(Motor,  mtr_S1_C2_1,     motorD,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorE,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     motorF,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorG,        tmotorNormal, openLoop)


#define MAXMOTORSPEED 50

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

task main(){
int mSD, mSE, mSF, mSG;
runMotorSpeeds(mSD, mSE, mSF, mSG, -60, 50);
wait10Msec(1000);
stopMotors();
}
