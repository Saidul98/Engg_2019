#ifndef QUEUE
#define QUEUE

#include "Arduino.h"

class Queue
{
    public:
    //attribures
        int numFloors;              //number of floors
        bool *floorRequested;     //I dont like pointers but maybe change to pointer
        int currentFloor;
        bool up;                    //state
       
        //constructor
        Queue(int floors);
        
        //methods
        bool requestFloor(int floor);
        bool resetFloor(int floor);
        int getMove();
        
        //just for testing if pathfind doesnt work 
    
    
    //sams original code below
    
};

#endif
