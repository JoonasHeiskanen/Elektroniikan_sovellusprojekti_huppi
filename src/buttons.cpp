#include <Arduino.h>
#include "buttons.h"
#include "LCD.h"

const int buttonPin1 = 1;
const int buttonPin2 = 2; //Unused button --> example for GIFS

// 1. Define possible states
enum DisplayState {
  STATE1,
  STATE2,
  STATE3
};

DisplayState currentState = STATE1;

void button_setup() {
    pinMode(buttonPin1, INPUT_PULLUP);
  
    Serial.println("Device started - State: STATE1");
}

void checkButton() {
  static unsigned long lastPressTime = 0;
  static bool buttonWasUp = true; 

  bool buttonNowDown = (digitalRead(buttonPin1) == LOW);

  // Check if button is pressed (edge detection + debounce)
  if (buttonNowDown && buttonWasUp && (millis() - lastPressTime > 200)) {


    // 2. Change state logically
    if (currentState == STATE1) {
      currentState = STATE2;
      //Put your functions here
      Serial.println("State changed: STATE2");
    } 
    else if (currentState == STATE2) {
      currentState = STATE3;
      //Put your funcitons here
      Serial.println("State changed: STATE3");
    } 
    else {
      currentState = STATE1;
      //Put your functions here
      Serial.println("State changed: STATE1");
    }

    lastPressTime = millis();
    buttonWasUp = false; // Mark button as pressed
  }

  // Allow new press only after the button is released
  if (!buttonNowDown) {
    buttonWasUp = true;
  }
}