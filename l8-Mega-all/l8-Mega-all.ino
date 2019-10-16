#include "motorCont300.h"

motorCont300 motor0;
int currentFloor;





int Floor1 = 0; //// Treat these as boolean. 1 = Yes/HIGH/On/Button Is pressed
                /////// 0 = Not On/Not pressed ////
int Floor2 = 0;
int Floor3 = 0;
int Floor4 = 0;
int Floor5 = 0;
int StopButton = 0;
int DoorCloseOpen = 0; /// Decide how to handle this - maybe different variable. 1 = doing
       /// something? 0 = keep whatever state it is (open or closed)?? Several
       /// ways to do this, just a choice
int FloorTracker = 1;
int DirectionTrack = 0; /// 0 = not moving /// 1 = UP /// -1 = DOWN
// int Destination = 1;
int MoveFloors = 0; /// amount of floors to move, maybe same as destination
int receivedNum = 0;
boolean newInput = false;
int interval = 250;
int interval2 = 4000;
unsigned long ms1 = 0;
unsigned long ms2 = 0;
unsigned long ms3 = 0;
unsigned long ms4 = 0;
bool stopPressed = false;










void setup() {
  // put your setup code here, to run once:
 Serial2.begin(9600); 
  currentFloor = 0;

  pinMode(12, OUTPUT);
  motor0.attach();
  attachInterrupt(0, doEncoderA, CHANGE); //encoder ON PIN 2 Yellow
  attachInterrupt(1, doEncoderB, CHANGE); //encoder ON PIN 3 Green

  pinMode(7, INPUT);

  
}





int flor = 0;



void loop() {


if (flor != 0)
   {
    motor0.setState(0);
   }
  while(motor0.motorMoving())
  {
  motor0.move(flor);
  } 


  
  readOneNumInput();
  ms1 = millis();
  ms3 = millis();
  if ((ms1 - ms2) > interval) {
    readNewInput();
  }
      

  if ((ms3 - ms4) > interval2) {
    Serial.println(Floor1);
    Serial.println(Floor2);
    Serial.println(Floor3);
    Serial.println(Floor4);
    Serial.println(Floor5);
    Serial.println(stopPressed);
    ms4 = millis();
    Serial.print("flor - ");
    Serial.println(flor);
  }
  flor = DecideDestination(FloorTracker);
}













void readOneNumInput() {
  if (Serial.available() > 0) {
    receivedNum = Serial.read();
    newInput = true;
  }
}

void readNewInput() {

  if (newInput == true) {

    if (receivedNum == 1) {
      Floor1 = 1;
    }
    if (receivedNum == 2) {
      Floor2 = 1;
    }
    if (receivedNum == 3) {
      Floor3 = 1;
      
    }
    if (receivedNum == 4) {
      Floor4 = 1;
    }
    if (receivedNum == 5) {
      Floor5 = 1;
    }
    if (receivedNum == 6) {
      Serial.println("wefrwef");
      /// Doors = 1;  Change this variabe to whatever name
    }
    if (receivedNum == 7) {
      stopPressed = true;
    }

    Serial.print("receivedNum: ");
    Serial.println(receivedNum);
    newInput = false;
    ms2 = millis();
  }
}

int DecideDestination(int CurrentFloor) {
  if (CurrentFloor == 1) { ///////////// Level 1 /////////////
    DirectionTrack = 1;

    if (Floor2 == 1) {
      Serial.println("asd");
      MoveFloors = 1;
      return MoveFloors;
    }

    else if (Floor3 == 1) {
      MoveFloors = 2;
      return MoveFloors;
    }

    else if (Floor4 == 1) {
      MoveFloors = 3;
      return MoveFloors;
    }

    else if (Floor5 == 1) {
      MoveFloors = 4;
      return MoveFloors;
    }

    else {
      DirectionTrack = 0;
      MoveFloors = 0;
      return MoveFloors;
    }
  }

  if (CurrentFloor == 2) { ///////////// Level 2 //////////////////

    if (DirectionTrack == -1) {
      if (Floor1 == 1) {
        MoveFloors = -1;
        return MoveFloors;
      } else if (Floor3 == 1) {
        DirectionTrack = 1;
        MoveFloors = 1;
        return MoveFloors;
      } else if (Floor4 == 1) {
        DirectionTrack = 1;
        MoveFloors = 2;
        return MoveFloors;
      } else if (Floor5 == 1) {
        DirectionTrack = 1;
        MoveFloors = 3;
        return MoveFloors;
      } else {
        DirectionTrack = 0;
        MoveFloors = 0;
        return MoveFloors;
      }
    }

    else if (DirectionTrack == 1) {
      if (Floor3 == 1) {
        MoveFloors = 1;
        return MoveFloors;
      } else if (Floor4 == 1) {
        MoveFloors = 2;
        return MoveFloors;
      } else if (Floor5 == 1) {
        MoveFloors = 3;
        return MoveFloors;
      } else if (Floor1 == 1) {
        DirectionTrack = -1;
        MoveFloors = -1;
        return MoveFloors;
      } else {
        DirectionTrack = 0;
        MoveFloors = 0;
        return MoveFloors;
      }
    }

    else {
      if (Floor1 == 1) {
        DirectionTrack = -1;
        MoveFloors = -1;
        return MoveFloors;
      } else if (Floor3 == 1) {
        DirectionTrack = 1;
        MoveFloors = 1;
        return MoveFloors;
      } else if (Floor4 == 1) {
        DirectionTrack = 1;
        MoveFloors = 2;
        return MoveFloors;
      } else if (Floor5 == 1) {
        DirectionTrack = 1;
        MoveFloors = 3;
        return MoveFloors;
      } else {
        DirectionTrack = 0;
        MoveFloors = 0;
        return MoveFloors;
      }
    }
  }

  if (CurrentFloor == 3) { ///////////// Level 3 //////////////////

    if (DirectionTrack == -1) {
      if (Floor2 == 1) {
        MoveFloors = -1;
        return MoveFloors;
      } else if (Floor1 == 1) {
        MoveFloors = -2;
        return MoveFloors;
      } else if (Floor4 == 1) {
        DirectionTrack = 1;
        MoveFloors = 1;
        return MoveFloors;
      } else if (Floor5 == 1) {
        DirectionTrack = 1;
        MoveFloors = 2;
        return MoveFloors;
      } else {
        DirectionTrack = 0;
        MoveFloors = 0;
        return MoveFloors;
      }
    }

    else if (DirectionTrack == 1) {
      if (Floor4 == 1) {
        MoveFloors = 1;
        return MoveFloors;
      } else if (Floor5 == 1) {
        MoveFloors = 2;
        return MoveFloors;
      } else if (Floor2 == 1) {
        DirectionTrack = -1;
        MoveFloors = -1;
        return MoveFloors;
      } else if (Floor1 == 1) {
        DirectionTrack = -1;
        MoveFloors = -2;
        return MoveFloors;
      } else {
        DirectionTrack = 0;
        MoveFloors = 0;
        return MoveFloors;
      }
    }

    else {
      if (Floor2 == 1) {
        DirectionTrack = -1;
        MoveFloors = -1;
        return MoveFloors;
      } else if (Floor1 == 1) {
        DirectionTrack = -1;
        MoveFloors = -2;
        return MoveFloors;
      } else if (Floor4 == 1) {
        DirectionTrack = 1;
        MoveFloors = 1;
        return MoveFloors;
      } else if (Floor5 == 1) {
        DirectionTrack = 1;
        MoveFloors = 2;
        return MoveFloors;
      } else {
        DirectionTrack = 0;
        MoveFloors = 0;
        return MoveFloors;
      }
    }
  }

  if (CurrentFloor == 4) { ///////////// Level 4 //////////////////

    if (DirectionTrack == 1) {
      if (Floor5 == 1) {
        MoveFloors = 1;
        return MoveFloors;
      } else if (Floor3 == 1) {
        DirectionTrack = -1;
        MoveFloors = -1;
        return MoveFloors;
      } else if (Floor2 == 1) {
        DirectionTrack = -1;
        MoveFloors = -2;
        return MoveFloors;
      } else if (Floor1 == 1) {
        DirectionTrack = -1;
        MoveFloors = -3;
        return MoveFloors;
      } else {
        DirectionTrack = 0;
        MoveFloors = 0;
        return MoveFloors;
      }
    }

    else if (DirectionTrack == -1) {
      if (Floor3 == 1) {
        MoveFloors = -1;
        return MoveFloors;
      } else if (Floor2 == 1) {
        MoveFloors = -2;
        return MoveFloors;
      } else if (Floor1 == 1) {
        MoveFloors = -3;
        return MoveFloors;
      } else if (Floor5 == 1) {
        DirectionTrack = 1;
        MoveFloors = 1;
        return MoveFloors;
      } else {
        DirectionTrack = 0;
        MoveFloors = 0;
        return MoveFloors;
      }
    }

    else {
      if (Floor5 == 1) {
        DirectionTrack = 1;
        MoveFloors = 1;
        return MoveFloors;
      } else if (Floor3 == 1) {
        DirectionTrack = -1;
        MoveFloors = -1;
        return MoveFloors;
      } else if (Floor2 == 1) {
        DirectionTrack = -1;
        MoveFloors = -2;
        return MoveFloors;
      } else if (Floor1 == 1) {
        DirectionTrack = -1;
        MoveFloors = -3;
        return MoveFloors;
      } else {
        DirectionTrack = 0;
        MoveFloors = 0;
        return MoveFloors;
      }
    }
  }

  if (CurrentFloor == 5) { ///////////////// Level 5 /////////////////////
    DirectionTrack = -1;

    if (Floor4 == 1) {
      MoveFloors = -1;
      return MoveFloors;
    }

    else if (Floor3 == 1) {
      MoveFloors = -2;
      return MoveFloors;
    }

    else if (Floor2 == 1) {
      MoveFloors = -3;
      return MoveFloors;
    }

    else if (Floor1 == 1) {
      MoveFloors = -4;
      return MoveFloors;
    }

    else {
      DirectionTrack = 0;
      MoveFloors = 0;
      return MoveFloors;
    }
  }
}








void doEncoderA()
{
  motor0.encoder1.goA();
}


void doEncoderB()
{
 motor0.encoder1.goB();
}
