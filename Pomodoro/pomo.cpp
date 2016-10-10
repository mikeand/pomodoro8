#include "pomo.h"
#include <LiquidCrystal.h>
#include "Arduino.h"

const int WORK_SECONDS = 25 * 60;
const int BREAK_SECONDS = 5 * 60;

Pomo::Pomo(uint8_t rs, uint8_t enable,
         uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
       : _state(STANDBY), 
         _timerStart(0),
         _pauseStart(0),
         _buttonPressStart(0),
         _pomoNumber(0),
         _overTime(false),
         _lcd(rs, enable, d0, d1, d2, d3) {
  _lcd.begin(16, 2);
  _lcd.print("Ready.");          
}


void Pomo::startButton(uint8_t startPin) {
  _startPin = startPin;
  pinMode(_startPin, INPUT);
}


void Pomo::buzzerPin(uint8_t buzzerPin) {
  _buzzerPin = buzzerPin;
  pinMode(_buzzerPin, OUTPUT);
}

void Pomo::loop() {
  switch (_state) {
    case STANDBY:      
      break;
    case WORK:
      outputTime(WORK_SECONDS);
      break;
    case BREAK:
      if (_pomoNumber % 4 == 0) {
        outputTime(WORK_SECONDS);
      } else {
        outputTime(BREAK_SECONDS);
      }
      break;
  }

  checkStart();
  outputBuzzer();
}

void Pomo::checkStart() {
  int result = digitalRead(_startPin);
  if (result == HIGH && _buttonPressStart == 0) {
    _buttonPressStart = millis();
    switch (_state) {
      
      case STANDBY:
        startPomodoro();
        break; 
        
      case WORK:
        if (_overTime) {
          startBreak();
        } else if (_pauseStart == 0) {
          _pauseStart = millis();
        } else {
          _timerStart += millis() - _pauseStart;
          _pauseStart = 0;
        }
        break;

      case BREAK:
        if (_overTime) {
          startPomodoro();
        } else if (_pauseStart == 0) {
          _pauseStart = millis();
        } else { 
          _timerStart += millis() - _pauseStart;
          _pauseStart = 0;
        }
        break;
    }
    
  } else {
    if (millis() - _buttonPressStart > 250) {
      _buttonPressStart = 0;
    }
  }
}

void Pomo::startBreak() {
  _state = BREAK;
  _timerStart = millis();
  _pauseStart = 0; 

  _lcd.clear();
  _lcd.print("Break ");
  _lcd.print(_pomoNumber);
}

void Pomo::startPomodoro() {
  _state = WORK;
  _timerStart = millis();
  _pauseStart = 0;
  _pomoNumber++;
  
  _lcd.clear();
  _lcd.print("Pomodoro ");
  _lcd.print(_pomoNumber);
}

void Pomo::outputTime(unsigned long totalTime) {
  long ms;
  unsigned long seconds;
  unsigned long minutes;

  if (_pauseStart > 0) {
    ms = _pauseStart - _timerStart;
  } else {
    ms = millis() - _timerStart;
  }

  ms = totalTime * 1000 - ms;

  if (ms < 0) {
    _overTime = true;
    ms = abs(ms);
  } else {
    _overTime = false;
  }
  seconds = ms / 1000;
  minutes = seconds / 60;
  seconds -= minutes * 60;
  
  _lcd.setCursor(0, 1);
  _lcd.print(minutes);
  _lcd.print(":");

  if (seconds < 10) {
    _lcd.print("0");
  }

  _lcd.print(seconds);  
  unsigned long halfSeconds = millis() / 250;
  if (_pauseStart > 0 && (halfSeconds % 2 == 0)) {
    _lcd.print(" paused.");  
  } else {
    _lcd.print("         ");  
  }
}

void Pomo::outputBuzzer() {
  int output = LOW;
  unsigned long quarterSeconds = millis() / 250;
  if (_overTime && (quarterSeconds % 2) == 0) {
      output = HIGH;
  }
  digitalWrite(_buzzerPin, output);
}

