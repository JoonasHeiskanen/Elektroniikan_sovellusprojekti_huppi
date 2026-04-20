#ifndef BUTTONS_H
#define BUTTONS_H

// 1. Define possible states
enum DisplayState {
  STATE1,
  STATE2,
  STATE3
};

extern DisplayState currentState;

void button_setup(void);
void checkButton(void);

#endif