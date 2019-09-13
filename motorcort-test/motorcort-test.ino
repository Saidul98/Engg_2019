#include "motorCont300.h"
motorCont300 motor0;

void setup() {
  // put your setup code here, to run once:
  //motor0 = motorCont300();
  Serial.begin(115200);
  pinMode(12, OUTPUT);
  motor0.motor1.attach(12, 1000, 2000);
  attachInterrupt(0, doEncoderA, CHANGE); //encoder ON PIN 2
  attachInterrupt(1, doEncoderB, CHANGE); //encoder ON PIN 3
  motor0.setState(0); //home
}


void loop() {
  // put your main code here, to run repeatedly:
  while(motorMoving()){
 motor0.move(1);
} 
}
bool motorMoving()
{
  if(motor0.getState() == 0 || motor0.getState() == 1)
  {
    return true;
  }
  motor0.move(0);
  return false;
}

void doEncoderA()
{
  motor0.encoder1.goA();
}


void doEncoderB()
{
 motor0.encoder1.goB();
}
