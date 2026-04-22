#ifndef BUTTONS_H
#define BUTTONS_H

enum DisplayState {
    STATE1,
    STATE2
};

void buttonSetup();
void checkButton();

DisplayState getState();

#endif
