#include "arduino.h"
#include "Queue.h"
#include "motorCont300.h"

#define _NUMFLOORS 5
#define __F1 45
#define __F2 47
#define __F3 49
#define __F4 51
#define __F5 53

Queue _Queue = Queue(_NUMFLOORS);
//motor300 obj;
motorCont300 motor0;
int currentFloor;

void setup(){
  Serial2.begin(9600); 
  currentFloor = 0;

  pinMode(12, OUTPUT);
  motor0.motor1.attach(12, 1000, 2000);
  attachInterrupt(0, doEncoderA, CHANGE); //encoder ON PIN 2
  attachInterrupt(1, doEncoderB, CHANGE); //encoder ON PIN 3

  
  
  
  pinMode(__F1,INPUT_PULLUP );
  attachInterrupt(digitalPinToInterrupt(__F1),currentFloor1,FALLING );
  pinMode(__F2,INPUT_PULLUP );
  attachInterrupt(digitalPinToInterrupt(__F2),currentFloor2,FALLING );
  pinMode(__F3,INPUT_PULLUP );
  attachInterrupt(digitalPinToInterrupt(__F3),currentFloor3,FALLING );
  pinMode(__F4,INPUT_PULLUP );
  attachInterrupt(digitalPinToInterrupt(__F4),currentFloor4,FALLING );
  pinMode(__F5,INPUT_PULLUP );
  attachInterrupt(digitalPinToInterrupt(__F5),currentFloor5,FALLING );
  int x = 10;
  Serial2.print(x);
  
  
  
  motor0.setState(0); //home
}


int flor = 0;

void loop(){
  if (Serial2.available()){
    int comm = Serial2.read();
    if(comm >0 &&comm <= 5){
      _Queue.requestFloor(comm);
    }else if(comm == 6){
      DOORS();
    }else if(comm == 7){
      EMERGENCY();
    }
  }
  flor = _Queue.getMove();
	//motor set state if new floor here
	
  while(motor0.motorMoving())
  {
	motor0.move(flor);
  } 
  
}

void serialEvent(){
  
}

void DOORS(){
  
}

void EMERGENCY(){
  
}

void currentFloor1(){
  currentFloor = 1;
}
void currentFloor2(){
  currentFloor = 2;
}
void currentFloor3(){
  currentFloor = 3;
}
void currentFloor4(){
  currentFloor = 4;
}
void currentFloor5(){
  currentFloor = 5;
}

void doEncoderA()
{
  motor0.encoder1.goA();
}


void doEncoderB()
{
 motor0.encoder1.goB();
}