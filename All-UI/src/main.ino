#include <SoftwareSerial.h>

#define STARTPIN 2
#define ENDPIN 8
#define BUTTON_IGNORE_TIME 5000

SoftwareSerial softSerial(12, 13); // RX, TX

/*
  Starting from pin 2 because pins 0 and 8 are reserved for serial

  PIN LAYOUT
  Floor Buttons:  Pin:
  1               2
  2               3
  3               4
  4               5
  5               6

  Door toggle button pin: 7
  Emergency stop button pin: 8
*/

// An array to contain the state of all the buttons
int button_states[9];

// An array to store whether a button is being ignored or not
int button_sleep_states[9];

// An array to store the time at which a button was pressed
unsigned long button_press_times[9];

// A long to store how much time has passed between button presses
unsigned long time_passed = 0;

void setup() {

  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }

  softSerial.begin(9600);

  // Set all the input pins as inputs:
  for (int i = STARTPIN; i <= ENDPIN; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  for (int i = 0; i < 7; i++) {
    button_states[i] = 0;
  }
}

void loop() {
  // If BUTTON_IGNORE_TIME or more has passed for a specific button, allow that button to be sent again
  for (int pin_number = STARTPIN; pin_number <= ENDPIN; pin_number++) {
    time_passed = millis() - button_press_times[pin_number];

    if (time_passed >= BUTTON_IGNORE_TIME) {
      button_sleep_states[pin_number] = 0;
    }
  }

  // Update the state of all the buttons, 1 if HIGH, 0 if not
  for (int pin_number = STARTPIN; pin_number <= ENDPIN; pin_number++) {
    button_states[pin_number] = (digitalRead(pin_number) == HIGH) ? 1 : 0;
  }

  // Print debug to serial monitor
  print_debug(button_states);

  // Write the value of the pin to serial out if it is HIGH and not sleeping
  for (int pin_number = STARTPIN; pin_number <= ENDPIN; pin_number++) {
    if (button_states[pin_number] == 1 && button_sleep_states[pin_number] == 0) {
      softSerial.write(pin_number);
      button_press_times[pin_number] = millis();
      button_sleep_states[pin_number] = 1;
    }
  }
}

// Prints the status of button_states to serial monitor
void print_debug(int button_states[]) {
  for (int pin_number = STARTPIN; pin_number <= ENDPIN; pin_number++) {
    Serial.print("[");
    Serial.print(pin_number);
    Serial.print("]: ");
    Serial.print(button_states[pin_number]);
    Serial.print(" ");
  }
  Serial.println();
}