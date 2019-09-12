//PID Calculation class - header
#ifndef	_pidFunc300_h
#define	_pidFunc300_h

#include "Arduino.h"
class pidFunc300{
	
	
	private:
unsigned long lastSample; //
unsigned long currentSample; //
double kp;      //Proportional Gain
double kd;      //Derivative Gain
double ki;      //Intergral Gain
bool milli;
double sampleRate; //in ms
double error; //Current error
double pError; 
	
	
	public:
	//construct
	pidFunc300();
	pidFunc300(double p, double d, double i);
	//methods
	void setSampleRm(int x);
	void setSampleRu(double y);
	double getError();
	void sampleSel();
	double calcPID(double input, int count, float ticksPerDeg);
	

	
};

#endif	/* pidFunc300.h */