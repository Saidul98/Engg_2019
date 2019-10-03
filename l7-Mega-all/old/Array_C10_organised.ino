/* L07_C10 PATH PLANNING
Gives "moveFloors", in order of first pressed, and will visit any requested floors inbetween destination and current position. Currently, it cannot visit a newly requested floor while already underway.

NOTES FOR FINAL CODE:
- Make array work with more values than just the set size of array - think this means re-structure as --> make code read binArray, order by checking if any are active in desired direction, starting at the next floor, then look in the other direction
- Determine if variable output "moveFloors" with C30, or do we need a function to return something
- Confirm name "moveFloors" with C30
- Confirm name "floorState" with C30
- Confirm operation of "floorState" with commands sent from C30 AND the order of functions being called between our codes
- Ignore the two "floorState = 2" lines if there is nothing simulating C30's return "true" or "false" check
- Initialise "floorState" to = 2 when done with testing
- Ignore/Remove "assignButtons" while testing with Serial inputs/in final code, respectively.
*/

//Organising array variables
#include "motorCont300.h"

motorCont300 motor0;

int input_array[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int input_num = 0;
int input_arraySize = 30; //change namee to input array size
int binArray[5];

//Status variables
int currentFloor = 1;   //Initial floor --> ENSURE IS "1"
int moveFloors;         //Final output variable to send to C30
int floorState = 1;     //SET THIS TO "2" ONCE DONE WITH TESTING - C30 to send 0/false = emergency || 1/true = normal. C10 write "2" to ignore functions.
int direc = 1;          //Direction of movement --> up (1) or down (-1)
int request = 0;        //Requested floor


//CAN REMOVE ONCE INTEGRATED:
//For replicating virtual inputs from C10
//int numb = 6;   //no. of floor buttons pressed


void setup() 
{
  Serial.begin(9600);     //INITIALISE SERIAL
  assignButtons();       //VIRTUALLY PRESSED BUTTONS - REMOVE in final code, and IGNORE in testing with serial inputs
  pinMode(12, OUTPUT);
  motor0.motor1.attach(12, 1000, 2000);
  attachInterrupt(0, doEncoderA, CHANGE); //encoder ON PIN 2 - Yellow
  attachInterrupt(1, doEncoderB, CHANGE); //encoder ON PIN 3 - Green
  motor0.setState(0); //drive
  
}

void loop() 
{
  //Call to read inputs:
  SerialEvent();       
  //Call if lift encountered problem and is ready to resume from floor 1:
  EmergencyReturn();    //"floorState" must == 0 to enter
  //Call when next floor destination is required:
  NormalRequest();      //"floorState" must == 1 to enter
   
   if (moveFloors != 0)
   {
	  motor0.setState(0);
   }
    
    while(motorMoving())
		{
			motor0.move(moveFloors);
		} 
}

//For assigning virtually pressed lift buttons - used only for testing without serial input
/*
void assignButtons(){
  Serial.println("Pressed buttons:");                  
for (int pressed = 0; pressed < numb; pressed ++){        //Fill array with "numb" amount of button presses
  int button = random(1,6);                               //Random levels are pressed
  input_array[pressed] = button;
    Serial.print(button);                                 //Tell us what floors are pressed
    Serial.print(", ");
   }
  Serial.println(" ");
}
*/

void SerialEvent()
{int value;
  if(input_num<input_arraySize)
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
      if(value == 6)
        {
        door_open();
        value = 0;
        return;
        }
      if(value == 7)
        {
        hault();//EMERGENCY BRAKE
        value = 0;
        return;
        }
      if(value > 7)
        {
        value = 0;
        return;
        }
      input_array[input_num] = value;
      input_num++;
      }
  }
}



//AFTER HAVING ENCOUNTERED A PROBLEM, AND ELEVATOR IS READY TO OPERATE AGAIN, WILL STOP AT CLOSEST FLOOR FIRST:
void EmergencyReturn()
{       
  if (floorState == 0)
  {             //Variable sent by C30 signifying elevator is now on level 1
    currentFloor = 1;               //Reset current floor to level 1
    readArray();                    //Make requested floors "active" in binArray
    emergencyMovement();            //Reads binArray and moves to closest floor requested
    for(int n =0;n<input_num;n++)
      {  //After Emergency return clear all floors
      input_array[n] = 0;
      input_num = 0;
      }
    return;
  }
}


//IF ELEVATOR IS READY AND AWAITING NEW DESTINATION:
void NormalRequest()
{
  if (floorState == 1)              //Variable sent by C30 signifying ready for normal operation
    {                         
    readArray();                    //Make requested floors "active" - used in further calculations to determine if there are desired floors between the destination and current floor for efficiency in path movement
    movement();                     //Read through input_array and return path as "moveFloors" variable, in order from first pressed, while stopping at required floors between destination and current floor
    }
}

void door_open(){}
void hault(){}


void readArray()
{
  for(int n = 0; n < input_arraySize; n ++)  //Read floor requests from input array and change state of floors
    {
     int level = input_array[n];
 
     //Make floors "active"
     if (level == 1){binArray[1] = 1;}
     if (level == 2){binArray[2] = 2;}
     if (level == 3){binArray[3] = 3;}
     if (level == 4){binArray[4] = 4;}
     if (level == 5){binArray[5] = 5;}
    }
}
void movement()
{
  for(int n = 0; n < input_arraySize; n++)
     {                                     //Read through input_array - this set-up needs to be re-configured for new values to be assigned to used positions in array, or something else
     request = input_array[n];
     //Serial.println("Requested Floor:" );
     //Serial.println(request);
     direcCalc();                        //Calculate Direction of next movement (saves variable "direc") - used in determining if any floors are requested in direction between destination and current floor, for efficiency of movement
     if (request != 0)
       {                                   //If a floor is requested in input_array
       inbetweenFloors();                  //Function to check and go to any floors inbetween next chronological destination and current floor position.
       nextPosition();                     //Function to go to next requested floor
       return;   
       } 
    }  
}
void direcCalc()
{
  int moveTest = request - currentFloor; //Determining if next is floor up or down
  if (moveTest >= 0)
    {
    direc = 1;
    } 
  else
    {
    direc = -1;
    }
}
void inbetweenFloors()
{                                                                                               //This checks and goes to any floors between next requested floor and current floor. Should this export(or something) a return? understand them
  int nextFloor = currentFloor + direc;                                                         //This is used to check the next floor in direction of movement in binArray - this is not the next requested floor
  for (int floorBetween = currentFloor; abs(floorBetween - request) > 1; floorBetween ++)       //If destination is not the next floor, and a floor inbetween is also requested, checking through all inbetween floors:
    {
    if (binArray[nextFloor] == nextFloor
      {                                                      //If next floor in path direction is also requested
      moveFloors = binArray[nextFloor] - currentFloor;                                            //THIS IS THE OUTPUT. C30 to read "moveFloors" variable - or can we make it return this?
      currentFloor = currentFloor + moveFloors;                                                   //Update current elevator position
      Serial.print("moveFloors inbetween:" );
      Serial.println(moveFloors);
      clearValues();                                                                              //Function to clear double up's of requested floors and floor value after having visited it
      //floorState = 2;                                                                               //Wait until C30 send next request for new position
      return;                                                                                         //Exit/make return floor value?
      } 
    else 
      {
      nextFloor = nextFloor + direc;                                                                 //Search for more requested floors between current position and destination
      }
    }
return;
}


void nextPosition(){                          //make this return something?
if ((request - currentFloor) != 0){
 moveFloors = request - currentFloor;         //OUTPUT movement distance - C30 to READ
 currentFloor = currentFloor + moveFloors;    //Update current known position

  Serial.print("moveFloors:" );
  Serial.println(moveFloors);

  input_num = 0;                              //Does this have to be copied to all other movement parts of the code? If so, include it also in the "inbetweenFloors" function
 
clearValues();                                //Function to clear double up's of requested floors and floor value after having visited it

//floorState = 2;                               //Wait until C30 send next request for new position
return;
  }
}



void clearValues(){
 for (int check = 0; check < input_arraySize; check ++){    //Used to clear the input_array of any double up's of floor requests
   int compare = input_array[check];
  if (compare == currentFloor){                       //If current floor is also requested again
    input_array[check] = 0;                           //Clear this floor request from input_array
  }
 }
  binArray[currentFloor] = 0;                         //Clear this floor request from binArray
}


void emergencyMovement(){
  for (int index = 1; index <= 5; index ++){  //Read through input_array - this set-up needs to be re-configured for new values to be assigned to used positions in array, or something else
   if (binArray[index] != 0){
    request = binArray[index];
      nextPosition();                         //Function to go to next floor
      index = 5;
   }  
  } 
     return;
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
