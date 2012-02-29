#include "DL-lib.c"

task main()
{
  DL_Init();
  ClearTimer(T1);
  while(time10[T1] < 6000){
    nxtDisplayTextLine(2, "%d", SensorRaw[S1]);
    DL_Insert((string)(SensorRaw[S1]));

  }

  DL_Close();
}
