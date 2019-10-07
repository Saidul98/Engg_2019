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
	
	prevMillis = 0;
	interval = 250;
	
	//////////////////////////////////////////////////////////////////////	Optional - Can be set by user	//
	encoder1 = encodeMot300(2, 3, 537.6); //	(intrupt pins, ppr) //1680 - 537.6
	tFCalc = pidFunc300(4.40, 0.05, 2.5); //	(kp, kd, ki)
	setPulley(0.045); //Set Pulley Circumfrence in mm   //38 for 11 - //70 for 8
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
//remap throttle to stop jump from 0-100
//slowly map it up to 100
void motorCont300::jumpNegat()
{
	long = currentMillis = millis();
	double diff = abs(pthrottle - throttle)
	if(diff >= 50)
	{
		if(currentMillis - previousMillis >= interval)
		{
			if(no_jump)
			{
				if(throttle > pthrottle)
					throttle = pthrottle + 50;
				else if(throttle < pthrottle)
				{
					throttle = pthrottle - 50;
				}
				pthrottle = throttle;
			}
		}
	}
}

//check if throttle is trying to go from 0-100
//stop throttle checking once done mapping.
void motorCont300::jumpcatch()
{
	long = currentMillis = millis();
	if(currentMillis - previousMillis >= interval)
	{
		if(no_jump)
		{
			if(abs(throttle - pthrottle) >= 120)
			{
				
			}
		}
	}
}

bool motorCont300::motorMoving()
{
  if(getState() == 0 || getState() == 1)
  {
    return true;
  }
  move(0);
  return false;
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
	jumpNegat();
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
		minPower = 1300;
		maxPower = 1000; 
	}
	else if (dir == '+')  //moves in Anti clockwise direction
	{
		minPower = 1700;
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
