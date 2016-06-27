#include "Hotkey.h"

Hotkeys::Hotkeys() {
  _count = 0;
}

void Hotkeys::add(short pin, char key) {
  _hotkeys[_count].lastState = HIGH;
  _hotkeys[_count].lastDebounce = 0;
  _hotkeys[_count].sent = false;
  _hotkeys[_count].debounceDelay = 10;
  _hotkeys[_count].pin = pin;
  _hotkeys[_count].key = key;
  
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
  _count++;
}

void Hotkeys::wakeUp()
{
  for(int i = 0; i < _count; i++) {
    int reading = digitalRead(_hotkeys[i].pin);
    if(reading != _hotkeys[i].lastState) {
      _hotkeys[i].lastDebounce = millis();
    }
    
    if((millis() - _hotkeys[i].lastDebounce) > _hotkeys[i].debounceDelay) {
      if(reading == LOW && _hotkeys[i].sent == false) {
        Keyboard.press(KEY_RIGHT_CTRL);
        Keyboard.press(KEY_RIGHT_SHIFT);
        Keyboard.press(_hotkeys[i].key);
        Keyboard.releaseAll();
        
         _hotkeys[i].sent = true;
      }
      if(reading == HIGH && _hotkeys[i].sent == true) {
        _hotkeys[i].sent = false;
      }
    }
    _hotkeys[i].lastState = reading;
  }
}
