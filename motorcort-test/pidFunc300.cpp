//PID Calculation class - body
#include "pidFunc300.h"


pidFunc300::pidFunc300(double p, double d, double i){
	kp = p;
	kd = d;
	ki = i;
	setSampleRm(4);
	error = 0.0;
	pError = 0.0;
	currentSample = 0;
	lastSample = 0;
	
}
pidFunc300::pidFunc300(){

}
void pidFunc300::errorCalc(double input, int count, float ticksPerDeg)
{
	error = input - (count * ticksPerDeg);
	return;
}

double pidFunc300::calcPID(double input, int count, float ticksPerDeg)
{
	errorCalc(input, count, ticksPerDeg);
	sampleSel(); //updates currentSample
	if (currentSample - lastSample > sampleRate)
	{
		double deriv = (error - pError) / sampleRate;
		double integ = (error + ((1 / 2) * (error - pError))) * sampleRate;
		pError = error;
		double throttle = (kp * error) + (kd * deriv) + (ki * integ);
		lastSample = currentSample;
		pthrottle = throttle;
		return throttle;
		
	}
	return pthrottle;
}



double pidFunc300::getError()
{
	return error;
}

void pidFunc300::sampleSel(){
	if(milli == true){
		currentSample = millis();
	}
	
	else
	{
		currentSample = micros();
	}
	
	return;
	}

void pidFunc300::setSampleRm(int x)
{
	milli = true;
	sampleRate = x;
	return;
}
void pidFunc300::setSampleRu(double y)
{
	milli = false;
	sampleRate = y;
	return;
}


