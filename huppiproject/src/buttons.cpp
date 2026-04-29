#include <Arduino.h>
#include "buttons.h"
#include "lcd.h"

const int buttonPin1 = 1; // Näkymän vaihto
const int buttonPin2 = 2; // Kirkkauden säätö

static DisplayState currentState = STATE1;
static int brightnessLevel = 3; // 0 = pois, 1 = himmeä, 2 = keskiteho, 3 = täysi

void buttonSetup() {
    pinMode(buttonPin1, INPUT_PULLUP);
    pinMode(buttonPin2, INPUT_PULLUP);
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

    // Painike 2: Kirkkauden säätö (sykli: 3 -> 2 -> 1 -> 0 -> 3...)
    static bool btn2WasUp = true;
    bool btn2NowDown = (digitalRead(buttonPin2) == LOW);

    if (btn2NowDown && btn2WasUp) {
        brightnessLevel--;
        if (brightnessLevel < 0) brightnessLevel = 3; 
        
        updateBacklight(); // Kutsutaan funktiota, joka säätää fyysisen pinnin
        btn2WasUp = false;
    }
    if (!btn2NowDown) btn2WasUp = true;
}

DisplayState getState() {
    return currentState;
}

int getBrightness() {
    return brightnessLevel;
}