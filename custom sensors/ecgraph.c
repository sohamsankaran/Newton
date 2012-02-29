

byte xyz[3];
int x,y,z;

void InitializeECG(tSensors nPort) {
  static byte write_dac[4] = {3, 0x90, 0x44, 0x00};



  SensorType[nPort] = sensorI2CCustom;
  wait10Msec(5);

  nxtDisplayTextLine(3, "starting i2c");

  nI2CBytesReady[nPort] = 0; // Clear any pending bytes
  sendI2CMsg(nPort, write_dac[0], 0);
  nxtDisplayTextLine(3, "waiting     ");
  while (nI2CStatus[nPort]==STAT_COMM_PENDING)
    wait1Msec(2); // Wait till I2C communication ends

  if (nI2CStatus[nPort]==NO_ERR) {
    nxtDisplayTextLine(3, "i2c no err  ");
  } else {
    nxtDisplayTextLine(3, "i2c err %d", nI2CStatus[nPort]);
  }
}

void ECG(tSensors nPort) {

	// set output, start read at channel 0, single ended, autoincrement
  //static const byte write_read_012[4] = {3, 0x90, 0x44, 0x00};
	// enable output, start read at channel 0, single ended, autoincrement
  //static const byte write_read_012[4] = {2, 0x90, 0x44};
  static const byte write_read_012[3] = {2, 0x90, 0x44};

  byte replyMsg[3];

  nxtDisplayTextLine(3, "starting i2c");

  nI2CBytesReady[nPort] = 0; // Clear any pending bytes
  sendI2CMsg(nPort, write_read_012[0], 3);
  nxtDisplayTextLine(3, "waiting     ");
  while (nI2CStatus[nPort]==STAT_COMM_PENDING)
    wait1Msec(2); // Wait till I2C communication ends

  if (nI2CStatus[nPort]==NO_ERR) {
    nxtDisplayTextLine(3, "i2c no err  ");
    nxtDisplayTextLine(1, "                 ");
    readI2CReply(nPort, replyMsg[0], 3);
    x = replyMsg[0];
    y = replyMsg[1];
    z = replyMsg[2];
  } else {
    nxtDisplayTextLine(3, "i2c err %d", nI2CStatus[nPort]);
  }
}

task main() {
  InitializeECG(S3);
  long i = 0;

  while (true)  {
     ECG(S3);
     //nxtDisplayTextLine(4, "                 ");
     nxtDisplayTextLine(4, "    ");
     nxtDisplayTextLine(5, "    ");
     nxtDisplayTextLine(6, "    ");
     nxtDisplayTextLine(4, "%d", x);
     //nxtDisplayTextLine(4, "%d %d %d", x, y, z);
     wait1Msec(50);
     int readValue;
  int displayValue;
  int maxReadValue;
  float devideBy = 2.6;
  int sensorValues[100];
  int i;

  nMotorEncoder[motorA] = 0;

    devideBy = (nMotorEncoder[motorA]/10)+2.6;
    readValue = x;
    displayValue = readValue/devideBy;
    sensorValues[99] = displayValue*0.52;
    for(i=0; i<100; i++){
      if(i != 99){
        sensorValues[i] = sensorValues[i+1];
      }
    }
    if(readValue > maxReadValue){
      maxReadValue = readValue;
    }
    eraseDisplay();
    for(i=0; i<100; i++){
      nxtDrawLine(i, 0, i, sensorValues[i]);
    }
    nxtDisplayStringAt(0, 63, "                ");
    nxtDisplayStringAt(3, 63, "Max: %3d", maxReadValue);
    nxtDisplayStringAt(82, 63, "%f", devideBy);
    wait10Msec(20);


  }

  return;
}
