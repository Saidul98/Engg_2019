#include "motorCont300.h"
motorCont300 motor0;

void setup() {
  // put your setup code here, to run once:
  motor0 = motorCont300();
  attachInterrupt(0, doEncoderA, CHANGE); //encoder ON PIN 2
  attachInterrupt(1, doEncoderB, CHANGE); //encoder ON PIN 3
  Serial.begin(115200);
  motor0.setState(2);

  
}

void loop() {
  // put your main code here, to run repeatedly:
//motor0.move(0);
Serial.println(motor0.throttle);
} 



void doEncoderA()
{
  if (digitalRead(motor0.encoder1.getA()) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(motor0.encoder1.getB()) == LOW) {
      motor0.encoder1.incCount(); // CW
    }
    else {
      motor0.encoder1.decCount(); // CCW
    }
  }
    else { // must be a high-to-low edge on channel A
    // check channel B to see which way encoder is turning
    if (digitalRead(motor0.encoder1.getB()) == HIGH) {
      motor0.encoder1.incCount(); // CW
    }
    else {
      motor0.encoder1.decCount(); // CCW
    }
  }
}


void doEncoderB()
{
  if (digitalRead(motor0.encoder1.getB()) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(motor0.encoder1.getA()) == HIGH) {
      motor0.encoder1.incCount(); // CW
    }
    else {
      motor0.encoder1.decCount(); // CCW
    }
  }

  // Look for a high-to-low on channel B

  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(motor0.encoder1.getA()) == LOW) {
      motor0.encoder1.incCount(); // CW
    }
    else {
      motor0.encoder1.decCount(); // CCW
    }
  }
}
