int counter = 0;
task main () {
  int num;
  int chksum;
  ubyte incoming[16];
  nxtEnableHSPort();
  nxtHS_Mode = hsRawMode;
  nxtSetHSBaudRate(9600);
  wait1Msec(2000);
  writeDebugStreamLine("slave alive");
  while(true) {
    num = nxtGetAvailHSBytes();
    if (num > 0) {
      nxtReadRawHS(incoming[0], num);
      if (num == 16)
      {
        chksum = 0;
        for (int i = 1; i < 15; i++) {
          chksum ^= incoming[i];
        }
        if ((chksum == incoming[0]) && (chksum == incoming[15]))
        {
          counter++;
          writeDebugStreamLine("%d %d", counter, incoming[0]);
        }
        else
          writeDebugStreamLine("BAD CHKSUM");
      }
      else
      {
         writeDebugStreamLine("SMALL %d", num);
      }
    }
  }
}
