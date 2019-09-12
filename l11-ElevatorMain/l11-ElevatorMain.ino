#include "arduino.h"
#include "Queue.h"
#include <Servo.h>

#define _NUMFLOORS 5
#define __F1 45
#define __F2 47
#define __F3 49
#define __F4 51
#define __F5 53

#define encA 2 //color yellow
//make sure you are consistant with putting the correct cables in
#define encB 3 //color green



Queue _Queue = Queue(_NUMFLOORS);
//motor300 obj;
int currentFloor;

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

//Power Restrictions (wont operate outside of these bounds)
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
double throttle = 0.0;
double temp = throttle;




void setup(){
  Serial2.begin(9600);
  //Serial.begin(9600);
  //obj = motor300(); 
  currentFloor = 0;

  Motor0.attach(Motor_pin0, 1000, 2000); // This pin is the control signal for the Motor
  Motor0.writeMicroseconds(1500); //intit the motor to stop
  pinMode(encA, INPUT);
  pinMode(encB, INPUT);
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
 angle = flor * 241.47;
  controller(angle);
  while(error > maxStop || error < minStop){
  angle = flor * 241.47;
  controller(angle); //CALLING MAIN PID FUNCTION
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
