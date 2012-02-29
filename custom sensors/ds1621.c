/*****************************************************************
**                                                              **
**  DS1621 RobotC driver                                        **
**                                                              **
**  file: testds1621count.c                                     **
**                                                              **
**  written by Tanja Neumann                                    **
**                                                              **
**  http://www.stud.uni-karlsruhe.de/~uqbdx/                    **
**                                                              **
**  Version: 1.0                                                **
**                                                              **
*****************************************************************/

// sensor on port S1
const tSensors ds1621 = S2;

task main(){

  // regular i2c mode
  SensorType[ds1621]=sensorI2CCustom ;

  // variable for i2c message
  byte msg[3];

  msg[0] = 2;
  msg[1] = 0x90;
  msg[2] = 0xEE;

  // send initialisation message to sensor, no reply bytes
  sendI2CMsg(ds1621,msg[0],0);

  // wait until port is ready
  while(nI2CStatus[ds1621]== STAT_COMM_PENDING){}

  //variables for output
  byte receive[2];
  byte countRemain[1];
  byte countPerC[1];
  int remain;
  int perC;
  int cycle=0;
  int readTemp;
  int value;

  eraseDisplay();
  nxtDisplayTextLine(0,"ds1621");

  // loop, polling each second
  while(1){

    msg[0] = 2;
    msg[1] = 0x90;
    msg[2] = 0xAA;

    // send message to read temperature, 2 reply bytes
    sendI2CMsg(ds1621,msg[0],2);

    // wait until port is ready
    while(nI2CStatus[ds1621]== STAT_COMM_PENDING){}

    // read temperature
    readI2CReply(ds1621,receive[0] ,2);

    // convert to int
    readTemp = receive[0]*100+receive[1]*25/64;

    msg[0] = 2;
    msg[1] = 0x90;
    msg[2] = 0xA8;

    // send message to read count_remain, 1 reply byte
    sendI2CMsg(ds1621,msg[0],1);

    // wait until port is ready
    while(nI2CStatus[ds1621]== STAT_COMM_PENDING){}

    // read count_remain
    readI2CReply(ds1621,countRemain[0] ,1);
    remain=countRemain[0];

    msg[0] = 2;
    msg[1] = 0x90;
    msg[2] = 0xA9;

    // send message to read count_per_c, 1 reply byte
    sendI2CMsg(ds1621,msg[0],1);

    // wait until port is ready
    while(nI2CStatus[ds1621]== STAT_COMM_PENDING){}

    // read count_remain
    readI2CReply(ds1621,countPerC[0] ,1);
    perC=countRemain[0];

    // convert readTemp, see datasheet ds 1621
    value = readTemp-25+(perC-remain)*100/perC;

    // output
    nxtDisplayTextLine(3,"count: %d.%d",value/100,value%100);
    nxtDisplayTextLine(5,"cycle: %d",++cycle);
    wait10Msec(100);
    }
}
