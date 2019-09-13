//Motor Class - header
#ifndef	_motorCont300_h
#define	_motorCont300_h

#include "Arduino.h"
#include <Servo.h>
#include "pidFunc300.h"
#include "encodeMot300.h"

class motorCont300{
	
	
	private:
	int pin;
	int state;
	
	
	double minPower;
	double maxPower;
	
	double degPerF;
	double minStop = -5.0;//Range for final location of motor
	double maxStop = 5.0;//range for final location of motor
	//this prevents the motor jittering about one point
	
	pidFunc300 tFCalc;
	

	
	 
	  
	
	
	
	public:
	Servo motor1;	
	double angle;
	double throttle;
	encodeMot300 encoder1;
	//construct
	motorCont300();
	//methods
	bool move(int flr);
	
	void setState(int a);
	void setPulley(double p);
	void setStopRange(double mi, double ma);
	void setDir(char dir);

	void drive();
	void mapThrottle();
	void halt();
	void pidC();
	void home();
	
};

#endif	/* motorCont300.h */
