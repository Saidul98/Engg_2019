//LimitSwitch class - body
#include "LimitSwitch.h"

LimitSwitch:: LimitSwitch(){
	//code goes here
}

/**
 * initialize pin to p
 * and set  pin mode
 * return - nothing
 */



/**
 * check pin if beam broken
 * 
 * return - bool 
 *  value of broken
 */
bool  LimitSwitch::bCheck()
{
	if(digitalRead(pin) == HIGH)
	{
		broken = true;
		
	}
	if(digitalRead(pin) == LOW)
	{
		broken = false;
	}
	return broken;
}

void LimitSwitch:: attach(int p){
	
	pin = p;
	pinMode(pin, INPUT);
	return;
}



