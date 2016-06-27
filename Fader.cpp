#include "Fader.h"

Faders::Faders() {
  _count = 0;
}

void Faders::add(short pin, char* startCommand, char* stopCommand) {
  _faders[_count].lastState = HIGH;
  _faders[_count].lastDebounce = 0;
  _faders[_count].sent = false;
  _faders[_count].debounceDelay = 10;
  _faders[_count].pin = pin;
  _faders[_count].commandLow = startCommand;
  _faders[_count].commandHigh = stopCommand;
  
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
  _count++;
}

void Faders::wakeUp()
{
  for(int i = 0; i < _count; i++) {
    int reading = digitalRead(_faders[i].pin);
    if(reading != _faders[i].lastState) {
      _faders[i].lastDebounce = millis();
    }
    
    if((millis() - _faders[i].lastDebounce) > _faders[i].debounceDelay) {
      if(reading == LOW && _faders[i].sent == false) {
         if(_faders[i].commandLow != "")
           Serial.println(_faders[i].commandLow);
           
         _faders[i].sent = true;
      }
      if(reading == HIGH && _faders[i].sent == true) {
        if(_faders[i].commandHigh != "")
          Serial.println(_faders[i].commandHigh);
        
        _faders[i].sent = false;
      }
    }
    _faders[i].lastState = reading;
  }
}
