//Encoder class - body
#include "encodeMot300.h"

encodeMot300::encodeMot300(int pa, int pb, float ppr){
	encA = pa;
	encB = pb;
	tPDeg = 360.0 / ppr;
	pinMode(encA, INPUT);
	pinMode(encB, INPUT);
	count = 0;
}
encodeMot300::encodeMot300(){
	//code goes here
}

void encodeMot300::incCount()
{
	count = count + 1;
	return;
}

void encodeMot300::decCount()
{
	count = count - 1;
	return;
}

int encodeMot300::getCount()
{
	return count;
}

int encodeMot300::getA()
{
	return encA;
}

int encodeMot300::getB()
{
	return encB;
}

float  encodeMot300::getTicks()
{
	return tPDeg;
}

void encodeMot300::goA()
{
	if (digitalRead(getA()) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(getB()) == LOW) {
      incCount(); // CW
    }
    else {
      decCount(); // CCW
    }
  }
    else { // must be a high-to-low edge on channel A
    // check channel B to see which way encoder is turning
    if (digitalRead(getB()) == HIGH) {
      incCount(); // CW
    }
    else {
      decCount(); // CCW
    }
  }
}


void encodeMot300::goB()
{
	 if (digitalRead(getB()) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(getA()) == HIGH) {
      incCount(); // CW
    }
    else {
      decCount(); // CCW
    }
  }

  // Look for a high-to-low on channel B

  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(getA()) == LOW) {
      incCount(); // CW
    }
    else {
      decCount(); // CCW
    }
  }
}