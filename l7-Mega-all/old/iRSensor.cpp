//IR Sensor class - body
#include "iRSensor.h"

iRSensor::iRSensor(int p){
	pin = p;
	pinMode(pin,  OUTPUT);
	broken = false;
}
iRSensor::iRSensor(){
	//code goes here
}

/**
 * initialize pin to p
 * and set  pin mode
 * return - nothing
 */
void iRSensor::attach(int p)
{
	pin = p;
	broken = false;
	pinMode(pin, OUTPUT);
	return;
}


/**
 * check pin if beam broken
 * 
 * return - bool 
 *  value of broken
 */
bool  iRSensor::bCheck()
{
	if(digitalRead(pin) == HIGH)
	{
		broken = true;
	}
	else
	{
		broken = false;
	}
	return broken;
}





