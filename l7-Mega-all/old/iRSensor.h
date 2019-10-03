//IR Sensor class - header
#ifndef	_iRSensor_h
#define	_iRSensor_h

#include "Arduino.h"
class iRSensor{
	
	
	private:
	int pin;
	bool broken;
	
	
	public:
	//construct
	iRSensor(int p);
	iRSensor();
	//methods
	bool bCheck();
	void attach(int p);
	
};

#endif	/* iRSensor.h */