#include <Arduino.h>
#include "buttons.h"

const int buttonPin1 = 1;
const int buttonPin2 = 2;

static DisplayState currentState = STATE1;

void buttonSetup() {
    pinMode(buttonPin1, INPUT_PULLUP);
}

void checkButton() {
    static bool buttonWasUp = true; 

    bool buttonNowDown = (digitalRead(buttonPin1) == LOW);

    if (buttonNowDown && buttonWasUp) {
        if (currentState == STATE1) {
            currentState = STATE2;
        } else {
            currentState = STATE1;
        }
        buttonWasUp = false;
    }
    if (!buttonNowDown) {
        buttonWasUp = true;
    }
}

DisplayState getState() {
    return currentState;
}
