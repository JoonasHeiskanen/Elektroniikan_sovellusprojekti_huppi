#include <Arduino.h>
#include "buttons.h"
#include "LCD.h"

const int buttonPin1 = 1;
const int buttonPin2 = 2; //Unused button --> example for GIFS

DisplayState currentState = STATE1;

void button_setup() {
    pinMode(buttonPin1, INPUT_PULLUP);
  
    Serial.println("Device started - State: STATE1");
}

void checkButton() {
  static bool buttonWasUp = true; 

  bool buttonNowDown = (digitalRead(buttonPin1) == LOW);

  // Check if button is pressed
  if (buttonNowDown && buttonWasUp) {

    // 2. Change state logically
    if (currentState == STATE1) {
      currentState = STATE2;
      Serial.println("State changed: STATE2");
    } 
    else if (currentState == STATE2) {
      currentState = STATE3;
      Serial.println("State changed: STATE3");
    } 
    else {
      currentState = STATE1;
      Serial.println("State changed: STATE1");
    }
    buttonWasUp = false; // Mark button as pressed
  }
  // Allow new press only after the button is released
  if (!buttonNowDown) {
    buttonWasUp = true;
  }
}