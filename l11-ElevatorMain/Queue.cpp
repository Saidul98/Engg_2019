#include "Arduino.h"
#include "Queue.h"

Queue::Queue(int floors)
{
    numFloors = floors;
    floorRequested = new bool[numFloors];
    for (int i = 0; i < numFloors; i++)
    {
        floorRequested[i] = false;
    }
}

bool Queue::requestFloor(int floor)
{
    // sanity check that the request is for a valid floor
    if (floor > numFloors || floor <= 0)
    {
        return false;
    }
    else
    {
        floorRequested[floor-1] = true;
        return true;
    }
}

bool Queue::resetFloor(int floor)
{
    // sanity check that the request is for a valid floor
    if (floor > numFloors || floor <= 0)
    {
        return false;
    }
    else
    {
        floorRequested[floor-1] = false;
        return true;
    }
} 



int Queue::getMove(){
  int i = currentFloor;
  bool switched = false;
  resetFloor(currentFloor);
  while(i>=0 && i<numFloors){
    if(floorRequested[i]){
      return(i-currentFloor);
    }
    if(up){
      i++;
    }else{
      i--;
    }
    if(!switched&&i>=0 && i<numFloors){
      up = !up;
      i = currentFloor;
    }
  }
}
