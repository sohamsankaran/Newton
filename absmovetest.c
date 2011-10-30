#pragma config(Motor,  motorA,          RND,           tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,          UP,            tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  motorC,          LEG,           tmotorNormal, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define RANGE 3
int distUp;
int distRnd;
int distLeg;
ubyte Rec[3];
int LRec[3];
int fRec[3];
bool new;

//void checkBTLinkConnected();

//task get;

int AbsMove(int MUp, int MRnd, int MLeg){
  int testRnd = 0;
  int testUp = 0;
  int testLeg = 0;
  while(testUp==0 || testRnd==0 || testLeg==0){
    //if(new){
      //break;
    //}
    nxtDisplayTextLine(1,"UP = %d",nMotorEncoder[UP]);
    nxtDisplayTextLine(2,"RND = %d",nMotorEncoder[RND]);
    distUp = MUp-nMotorEncoder[UP];
    if(MUp != nMotorEncoder[UP]){
     motor[UP] = distUp;
    }
    else{
      testUp = 1;
      motor[UP] = 0;
    }

    distRnd = MRnd-nMotorEncoder[RND];
    if(MRnd != nMotorEncoder[RND]){
      motor[RND] = distRnd;
    }
    else{
      testRnd = 1;
      motor[RND] = 0;
    }

    distLeg = MLeg-nMotorEncoder[LEG];
    if(MLeg != nMotorEncoder[LEG]){
     motor[LEG] = distLeg;
    }
    else{
      testLeg = 1;
      motor[LEG] = 0;
    }
  }
  motor[UP]= 0;
  motor[RND]= 0;
  motor[LEG]= 0;
  return 0;
}

task main(){
 // checkBTLinkConnected();
  nMotorEncoder[UP] = 0;
  nMotorEncoder[RND] = 0;
  //StartTask(get);
  while(true){
    nxtDisplayTextLine(3,"RUP = %d",500);
    nxtDisplayTextLine(4,"RRND = %d",500);
    nxtDisplayTextLine(5,"RRND = %d",500);
    AbsMove(2000, 2000, 500);
     AbsMove(0, 0, 0);
 //   motor[UP] = 100;
   // motor{RND] = 100;
  }
}

/*void checkBTLinkConnected()
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

task get(){
  new = false;
  LRec[0] = 0;
  LRec[1] = 0;
  LRec[2] = 0;
  while(true){
    cCmdMessageRead(Rec, 3, mailbox1);
    nxtDisplayTextLine(6,"BUP = %d",Rec[0]);
    nxtDisplayTextLine(7,"BRND = %d",Rec[1]);
    nxtDisplayTextLine(8,"BLEG = %d",Rec[2]);
    if(LRec[0]!= Rec[0] || LRec[1]!= Rec[1] || LRec[2]!= Rec[2]){
      new = true;
      LRec[0] = Rec[0];
      LRec[1] = Rec[1];
      LRec[2] = Rec[2];
      wait1Msec(10);
    }
    else{
      new = false;
    }
  }
}*/