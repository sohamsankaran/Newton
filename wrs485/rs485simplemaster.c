task main () {
  int num;
  ubyte outgoing[16] = {15, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  //memset (outgoing, 0x00, 16);
  nxtEnableHSPort();
  nxtHS_Mode = hsRawMode;
  nxtSetHSBaudRate(9600);
  wait1Msec(2000);


  for (int i = 0; i < 100; i++) {
    outgoing[0] = i;
    nxtWriteRawHS(outgoing[0], 16);
    wait1Msec(100);
  }
}
