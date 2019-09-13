//Motor Class - body
#include "motorCont300.h"

/**
 * initialize pin to p
 * and set  pin mode
 * Write stop to the motor
 * 
 */
motorCont300::motorCont300(){
	pin = 12;
	//pinMode(pin, OUTPUT);
	//motor1.attach(pin, 1000, 2000);
	throttle = 1500;
	//motor1.writeMicroseconds(throttle);
	state = 1;
	angle = 0;
	
	
	//////////////////////////////////////////////////////////////////////	Optional - Can be set by user	//
	encoder1 = encodeMot300(2, 3, 537.6); //	(intrupt pins, ppr)
	tFCalc = pidFunc300(4.40, 0.05, 2.5); //	(kp, kd, ki)
	setPulley(0.070); //Set Pulley Circumfrence in mm
	setStopRange((-5.0), 5.0); //set error bars for position control
	
	
	//Default sample rate is 4ms 
	//tFCalc.setSampleRu(0.1); //To change the samplerate to micro
}


bool motorCont300::move(float flr)
{
	
	angle = flr * degPerF;
	motor1.writeMicroseconds(throttle);
	switch(state)
	{
		case 0: //pid calc
			tFCalc.errorCalc(angle, encoder1.getCount(), encoder1.getTicks());
			if( tFCalc.getError() < maxStop && tFCalc.getError() > minStop)
			{
			state = 2;
			}
			else
			{
				drive();
				
			}
			break;
		case 1: //home
			home();
			break;
		case 2: //halt
			halt();
			break;
		default: //Do Nothing
		
			break;
	}
	
	return;
	
}

void motorCont300::setState(int a)
{
	state = a;
	return;
}

void motorCont300::home(){
	noInterrupts();
	throttle = 1400;
	//while(limit =! true){
	motor1.writeMicroseconds(throttle);
	//}
	interrupts();
	return;
}

void  motorCont300::halt()
{
	minPower = 1500;
	maxPower = 1500;
	throttle = 1500;
	return;
	
}

void motorCont300::drive()
{
	pidC(); //Do Math on throttle from PID
	mapThrottle();// Set the direction of the motor and map the throttle to be within the min and max.
	return;
}

void motorCont300::mapThrottle()
{
	if (throttle > 0.0) 
      {
        setDir('-');
      }
    else 
      {
        setDir('+');
      }
        throttle = map(abs(throttle), 235, 67007, minPower, maxPower);
		return;
}

void  motorCont300::pidC()
{
	throttle = tFCalc.calcPID(angle, encoder1.getCount(), encoder1.getTicks());
	return;
}

void motorCont300::setDir(char dir)
{
	if (dir == '-')//Moves in Clockwise direction
	{
		minPower = 1400;
		maxPower = 1000; 
	}
	else if (dir == '+')  //moves in Anti clockwise direction
	{
		minPower = 1600;
		maxPower = 2000;
		}
  else { //motor off.
  minPower = 1500;
  maxPower = 1500;
  }
 return;
}

void motorCont300::setPulley(double p)
{
	double cir = p * 2 * PI;
	double r1 = 360 * 0.295;
	degPerF = r1 / cir;
	return;
}

void motorCont300::setStopRange(double mi, double ma)
{
	minStop = mi;
	maxStop = ma;
	return;
}


int motorCont300::getState()
{
	return state;
}