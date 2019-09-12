
#include <Servo.h>

#define encA 2 //color yellow
//make sure you are consistant with putting the correct cables in
#define encB 3 //color green

int Motor_pin0 = 12;
Servo Motor0;
int prevMill = 0;
float ticksPerDeg = 360.0 / 537.6; //Calculates the angle that each tick of the encoder represents.
volatile float count = 0;          //Encoder count

//Time Variables
unsigned long inputTime = 0;  //For finding the settling time
unsigned long lastSample = 0; //For finding the derivative correctly
unsigned long currentSample = 0;

//Range for stopping
//error bars
double minStop = -5.0;
double maxStop = 5.0;
double minPower = 1500.0; //below 100 and the voltage is too low for the motor to turn on
double maxPower = 1500.0; //if 255 it doesnt work

double angle = 1000.0;

//PID Controller Implementation (NEEDS TO BE TUNED)
double kp = 4.40;      //Proportional Gain
double kd = 0.05;      //Derivative Gain
double ki = 2.50;      //Intergral Gain
double sampleRate = 0.1; //in ms
//int sampleRate = 4;
double error = 0.0; //Current error
double pError = 0.0;  //Previous error.
double throttle = 0.0;   //Throttle value (calculated)
double temp = throttle;


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
  Serial.begin(9600);
  Motor0.attach(Motor_pin0, 1000, 2000); // This pin is the control signal for the Motor
  Motor0.writeMicroseconds(1500); //intit the motor to stop
  pinMode(encA, INPUT);
  pinMode(encB, INPUT);
  pinMode(7, INPUT);
  attachInterrupt(0, doEncoderA, CHANGE); //encoder ON PIN 2
  attachInterrupt(1, doEncoderB, CHANGE); //encoder ON PIN 3
  
}





int flor = 0;



void loop() {
  angle = flor * 241.47;
  controller(angle);

while(error > maxStop || error < minStop){
  angle = flor * 241.47;
  controller(angle); //CALLING MAIN PID FUNCTION
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







void controller(double input) {
  error = input - (count * ticksPerDeg);
//  if((millis() - prevMill) >= 200){
//    Serial.print("Error:    ");
//    Serial.println(pError);
   
  if (error > maxStop || error < minStop) 
  {
    currentSample = /**/micros()/*millis()*/; //if want a sample rate lower than 1ms, change to micros()
    if (currentSample - lastSample > sampleRate) {
    double deriv = (error - pError) / sampleRate;                   // gradient approximation, rise/run
    double integ = (error + ((1 / 2) * (error - pError))) * sampleRate; // rectangular approximation.
    pError = error;                                                 //for use in next calculation
    throttle = (kp * error) + (kd * deriv) + (ki * integ);  
    temp = throttle;

    if (throttle > 0.0) 
      {
        setDir('-');
        throttle *= -1;
      }
    else 
      {
        setDir('+');
      }
    lastSample = currentSample;
    if(throttle > 0)
      {
        throttle = map(throttle, 235, 67007, minPower, maxPower);
      }
    else
        throttle = map(throttle, -235, -67007, minPower, maxPower);
      //Motor0.writeMicroseconds(throttle);
    }
  }
  else 
  {
    Motor_stop();
    //Motor0.writeMicroseconds(throttle);
  }
  Motor0.writeMicroseconds(throttle);
}

void doEncoderA() { //SETUPS the interrupts of the encoder
  if (digitalRead(encA) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(encB) == LOW) {
      count = count + 1; // CW
    }
    else {
      count = count - 1; // CCW
    }
  }

  else { // must be a high-to-low edge on channel A
    // check channel B to see which way encoder is turning
    if (digitalRead(encB) == HIGH) {
      count = count + 1; // CW
    }
    else {
      count = count - 1; // CCW
    }
  }
}

void doEncoderB() { //SETUPS the interrupts of the encoder
  if (digitalRead(encB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(encA) == HIGH) {
      count = count + 1; // CW
    }
    else {
      count = count - 1; // CCW
    }
  }

  // Look for a high-to-low on channel B

  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(encA) == LOW) {
      count = count + 1; // CW
    }
    else {
      count = count - 1; // CCW
    }
  }
}

void setDir(char dir) { //DIRECTION SET FUNCTION
  if (dir == '-') { //Moves in Clockwise direction
    minPower = 1400;
    maxPower = 1000; 
  }
  else if (dir == '+') { //moves in Anti clockwise direction
    minPower = 1600;
    maxPower = 2000;
  }
  else { //motor off.
    Motor_stop();
  }
}

void Motor_stop() {
  throttle = 1500;
}
