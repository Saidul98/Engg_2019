//Encoder class - header
#ifndef	_encodeMot300_h
#define	_encodeMot300_h

#include "Arduino.h"
class encodeMot300{
	
	
	private:
	int encA;
	int encB;
	int count;
	float tPDeg;
	
	
	public:
	//construct
	encodeMot300(int pa, int pb, float ppr);
	encodeMot300();
	//methods
	int getCount();
	float getTicks();
	int getA();
	int getB();
	void incCount();
	void decCount();
	void goA();
	void goB();
	
};

#endif	/* encodeMot300.h */