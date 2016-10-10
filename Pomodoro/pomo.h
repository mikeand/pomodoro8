#ifndef POMO_H_INC
#define POMO_H_INC

#include <LiquidCrystal.h>

typedef enum {
  STANDBY,
  WORK,
  BREAK,
} POMOTYPE;

class Pomo {

  public:
    Pomo(uint8_t rs, uint8_t enable,
         uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

    /** Set the microcontroller pin used for start/pause.
     */
    void startButton(uint8_t startPin);

    /** Set the pin that will be used to drive the buzzer.
     */
    void buzzerPin(uint8_t buzzerPin);

    /** This is called every cycls to process events.
     */
    void loop();

  private:
    /// Check the status of the start/pause and process.
    void checkStart();

    /// Commence the next pomodoro at time 0.
    void startPomodoro();

    /// Display the total time supplied as a paramter (milliseconds).
    void outputTime(unsigned long totalTime);

    /// Will beep the buzzer if the time is passed the end.
    void outputBuzzer();

    /// Start the next break at time zero.
    void startBreak();


  private:
    POMOTYPE _state;
    unsigned long _timerStart;
    unsigned long _pauseStart;
    bool _overTime;
    unsigned long _buttonPressStart;
    uint8_t _startPin;
    uint8_t _buzzerPin;
    LiquidCrystal _lcd;
    uint8_t _pomoNumber;
};


#endif
