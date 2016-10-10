/* Pomodoro Timer */

// include the library code:
#include "pomo.h"

// initialize the library with the numbers of the interface pins
Pomo pomo(12, 11, 5, 4, 3, 2);

void setup() {
  pomo.startButton(7);
  pomo.buzzerPin(8);
}


void loop() {
  pomo.loop();
}

