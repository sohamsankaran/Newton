

byte xyz[3];
int x,y,z;

void InitializeAccel(tSensors nPort, int scale /* 2 for 2g, 6 for 6g */) {
  static byte write_dac[4] = {3, 0x90, 0x44, 0x00};

  if (scale == 2) write_dac[3] = 0x00;
  if (scale == 6) write_dac[3] = 0xff;

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

void Accel(tSensors nPort) {

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
  InitializeAccel(S3, 2);
  long i = 0;

  while (true)  {
     Accel(S3);
     //nxtDisplayTextLine(4, "                 ");
     nxtDisplayTextLine(4, "    ");
     nxtDisplayTextLine(5, "    ");
     nxtDisplayTextLine(6, "    ");
     nxtDisplayTextLine(4, "%d", x);
     nxtDisplayTextLine(5, "%d", y);
     nxtDisplayTextLine(6, "%d", z);
     //nxtDisplayTextLine(4, "%d %d %d", x, y, z);
     wait1Msec(50);
  }
  return;
}
