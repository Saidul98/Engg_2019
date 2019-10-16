#include "motorCont300.h"
motorCont300 motor0;

void setup() {
  // put your setup code here, to run once:
  //motor0 = motorCont300();
  Serial.begin(115200);
  pinMode(12, OUTPUT);
  
  motor0.attach();
  pinMode(4, INPUT);
  attachInterrupt(0, doEncoderA, CHANGE); //encoder ON PIN 2 - Yellow
  attachInterrupt(1, doEncoderB, CHANGE); //encoder ON PIN 3 - Green
  motor0.setState(0); //drive
}
int flor = 1;  //11 is # per F
// L7
// 3 is up 1.1
// -4 is down 


//L7 up is 4
//L7 down -4.4

void loop() {
  //Serial.println(motor0.getState());
  // put your main code here, to run repeatedly:
  while(motorMoving()){
 motor0.move(flor);
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
