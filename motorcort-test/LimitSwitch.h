//Limit Switch class - header
#ifndef	_LimitSwitch_h
#define	_LimitSwitch_h

#include "Arduino.h"
class LimitSwitch{
	
	
	private:
	int pin;
	bool broken;
	
	
	public:
	//construct
	LimitSwitch();
	//methods
	bool bCheck();
	void attach(int p);
	
};

#endif	/* LimitSwitch.h */