/* L07_C10 PATH PLANNING
  Returns "moveFloors", in order of first pressed, and will visit any requested floors inbetween destination and current position. will go to any others pressed within that time that are along the route, as long as those are pressed before the elevator starts moving (don't send our command until doors have closed to allow multiple presses) but could construct a more intelligent course after next arrival. Currently, it cannot visit a newly requested floor while already underway.
*/
#include <Servo.h>

#define encA 2 //color yellow
//make sure you are consistant with putting the correct cables in
#define encB 3 //color green


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

//Power Restrictions (wont operate outside of these bounds)
double minPower = 1500.0; //below 100 and the voltage is too low for the motor to turn on
double maxPower = 1500.0; //if 255 it doesnt work

double angle = 1000.0;
double kp = 4.40;      //Proportional Gain
double kd = 0.05;      //Derivative Gain
double ki = 2.50;      //Intergral Gain
double sampleRate = 0.1; //in ms
//int sampleRate = 4;
double error = 0.0; //Current error
double pError = 0.0;  //Previous error.
double throttle = 0.0;   //Throttle value (calculated)
double temp = throttle;

int flor = 0;
/*

  NOTES FOR FINAL CODE:
  - Confirm operation of "floorState" true/false sequence
  - Re-set "floorState" to 2 afer EmergencyReturn and NormalRequest
  - Confirm output variable name "moveFloors" with C30
  - Confirm name "floorState" with C30
  - Make array work with more values than in the set size of array

*/
int input_array[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int input_num = 0;
int arraySize = 30;

//Our variables
int currentFloor = 1;  //Initial floor - will be 1, but can try other variables for testing
int binArray[4];       //Array with states of which floors to go to - should this be a double array?
//also work out what to do with the 6 and 7 numbers - will need a check for them outside of my loops - either before, or filter them out elsewhere
int moveFloors;        //Final output variable to send to C30


//Other variables
int direc = 1;  //Direction of movement --> either up (1) or down (-1)
int y;
int moveTest;

//C20's part
int numb = 5; //# of floor buttons pressed

//C30's part
//0 = false, 1 = true, 2 = ignore this code - move on - will stay this way until they send something back asking to re-evaluate the destination
int floorState = 1; //should be false to begin with - CHANGE TO FALSE ONCE DONE WITH TESTING












void setup() {

  //INITIALISE SERIAL
  Serial.begin(9600);

  Motor0.attach(Motor_pin0, 1000, 2000); // This pin is the control signal for the Motor
  Motor0.writeMicroseconds(1500); //intit the motor to stop
  pinMode(encA, INPUT);
  pinMode(encB, INPUT);
  attachInterrupt(0, doEncoderA, CHANGE); //encoder ON PIN 2
  attachInterrupt(1, doEncoderB, CHANGE); //encoder ON PIN 3


  /*assignButtons();
    Serial.println("randomly pressed lift buttons:");  //ASSIGN VIRTUAL BUTTONS - simulating C20
    for (int t = 0; t < numb; t = t + 1){
    int r = random(1,6);
    largeArray[t] = r;
      Serial.print(r);
      Serial.print(", ");
      }
    Serial.println(" ");
  */
}


void loop() {
  SerialEvent();

  EmergencyReturn();

  NormalRequest();

  angle = flor * 241.47;
  controller(angle);
  while (error > maxStop || error < minStop) {
    angle = flor * 241.47;
    controller(angle); //CALLING MAIN PID FUNCTION
  }

}  //End of loop


void SerialEvent()
{
  int value;
  if (input_num < arraySize)
  {
    if (Serial.available())
    {
      value = Serial.read();
      Serial.print("value");
      Serial.println(value);
      //value -= 48;
      //Serial.print("VALUE ");
      //Serial.print(value);
      //Serial.print(" RECIEVED");
      //Serial.println();
      if (value == 6)
      {
        door_open();
        value = 0;
        return;
      }
      if (value == 7)
      {
        hault();//EMERGENCY BRAKE
        value = 0;
        return;
      }
      if (value > 7)
      {
        value = 0;
        return;
      }

      input_array[input_num] = value;
      input_num++;

    }
  }
}


void EmergencyReturn() {

  //IF RECEIVED THAT ELEVATOR HAS RETURNED TO LEVEL 1:

  if (floorState == 0) {
    moveFloors = currentFloor - 1;   //currentFloor will be updated after the last command, thus will be the distance needed to travel to continue to desired floor.
    //  currentFloor = currentFloor + moveFloors; //New current level
    return;
  }
}


void NormalRequest() {
  //RECEIVED THAT ELEVATOR HAS ARRIVED AND AWAITING NEW DESTINATION:
  if (floorState == 1) {

    //readInputs();
    for (int z = 0; z < arraySize; z = z + 1) {
      int e = input_array[z];

      //Turning destination levels true:
      if (e == 1) {
        binArray[0] = 1;
      }
      if (e == 2) {
        binArray[1] = 2;
      }
      if (e == 3) {
        binArray[2] = 3;
      }
      if (e == 4) {
        binArray[3] = 4;
      }
      if (e == 5) {
        binArray[4] = 5;
      }

    }


    //readArray();
    for (int p = 0; p < arraySize; p = p + 1) {
      int d = input_array[p];
      //Serial.println("value" );
      //Serial.println(d);

      //direcCalc();
      moveTest = d - currentFloor; //Up or down
      if (moveTest >= 0) {
        direc = 1;
      } else {
        direc = -1;
      }

      //floorTrue();
      if (d != 0) {      //if Floor is desired
        y = (currentFloor + (1 * direc)) - 1;

        //inbetweenFloors();   - will export a return
        for (int n = currentFloor; abs(n - d) > 1; n = n + 1) { //If destination is not the next floor, and a floor inbetween is also requested:
          if (binArray[y] != 0) {
            moveFloors = binArray[y] - currentFloor;
            Serial.print("moveFloors h" );
            Serial.println(moveFloors);

            currentFloor = currentFloor + moveFloors;


            //EMPTY ARRAY
            for (int f = 0; f < arraySize; f = f + 1) {
              int g = input_array[f];
              if (g == currentFloor) {
                input_array[f] = 0;      //Clear this number from large array
              }
            }
            binArray[y] = 0;        //Clear this floor value from repeating
            return;
          }
        }


        moveFloors = d - currentFloor;

        Serial.print("moveFloors " );
        Serial.println(moveFloors);

        input_num = 0;

        currentFloor = currentFloor + moveFloors;

        for (int f = 0; f < arraySize; f = f + 1) {
          int g = input_array[f];
          if (g == currentFloor) {
            input_array[f] = 0;      //Clear this number from large array
          }
        }
        binArray[y] = 0;


        return;
      }

      //floorState = 2;
    }  //End of floorState = true
  }
}


void door_open() {

}


void hault() {

}

//PID FUNCTION
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
      if (throttle > 0)
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
