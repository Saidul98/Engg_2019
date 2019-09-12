//Encoder class - body
#include "encodeMot300.h"

encodeMot300::encodeMot300(int pa, int pb, float ppr){
	encA = pa;
	encB = pb;
	tPDeg = 360.0 / ppr;
	pinMode(encA, INPUT);
	pinMode(encB, INPUT);
}
encodeMot300::encodeMot300(){
	//code goes here
}

void encodeMot300::incCount()
{
	count = count + 1;
}

void encodeMot300::decCount()
{
	count = count - 1;
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

/*
void encodeMot300::doEncoderA()
{
	if (digitalRead(encA) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(encB) == LOW) {
      count = count + 1; // CW
    }
    else {
      count = count - 1; // CCW
    }
  }
    else { // must be a high-to-low edge on channel A
    // check channel B to see which way encoder is turning
    if (digitalRead(encB) == HIGH) {
      count = count + 1; // CW
    }
    else {
      count = count - 1; // CCW
    }
  }
}


void encodeMot300::doEncoderB()
{
	if (digitalRead(encB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(encA) == HIGH) {
      count = count + 1; // CW
    }
    else {
      count = count - 1; // CCW
    }
  }

  // Look for a high-to-low on channel B

  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(encA) == LOW) {
      count = count + 1; // CW
    }
    else {
      count = count - 1; // CCW
    }
  }
}



*/
