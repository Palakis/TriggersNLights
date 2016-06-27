#include "Light.h"

Lights::Lights() {
  _count = 0;
  _inputDataLength = 0;
  _strComplete = false;  
}

void Lights::add(short pin, char* name) {
  _lights[_count].pin = pin;
  _lights[_count].name = name;
  _lights[_count].lastBlink = millis();
  _lights[_count].blinkDuration = 500;
  
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  _count++;
}

void Lights::wakeUp() {
 if(_count < 1) {
   return;
 }
 
 while(Serial.available()) {
   char inputChar = (char)Serial.read();
   _inputData = (char*)realloc(_inputData, (_inputDataLength+1) * sizeof(char));
   _inputData[_inputDataLength] = inputChar;
   _inputDataLength++;
   
   if(inputChar == '\n') {
     _inputData[_inputDataLength-1] = '\0';
     
     for(int i = 0; i < _count; i++) {
       char* cmd = (char*)malloc(_inputDataLength * sizeof(char));
       memcpy(cmd, _inputData, _inputDataLength);
       
       char** parts = (char**)malloc(0);
       int lastStart = 0, partsCount = 0;
       
       char* name = (char*)malloc(0);
       char* command = (char*)malloc(0);
       char* commandParam = (char*)malloc(0);
       
       for(int y = 0; y <= _inputDataLength; y++) {
         if(cmd[y] == ' ' || cmd[y] == '\0') {           
           int wLength;
           char* w;
           
           wLength = y - lastStart;
           w = (char*)malloc((wLength+1) * sizeof(char));
           memset(w, 0, wLength+1);
           w[wLength+1] = '\0';
           
           memcpy(w, cmd + lastStart, wLength);  
           lastStart = y + 1;
           
           switch(partsCount) {
             case 0:
               name = (char*)malloc(wLength + 1);
               memset(name, 0, wLength + 1);
               memcpy(name, w, wLength + 1);
               break;
             case 1:
               command = (char*)malloc(wLength + 1);
               memset(command, 0, wLength + 1);
               memcpy(command, w, wLength + 1);
               break;
             case 2:
               commandParam = (char*)malloc(wLength + 1);
               memset(commandParam, 0, wLength + 1);
               memcpy(commandParam, w, wLength + 1);
               break;
             default:
               break;
           }
           
           free(w);
           partsCount++;
         }
       }
       
       if(strcmp(name, _lights[i].name) == 0) {
         if(strcmp(command, "ON") == 0) {
           _lights[i].blinking = false;
           digitalWrite(_lights[i].pin, HIGH);
         }
         if(strcmp(command, "OFF") == 0) {
           _lights[i].blinking = false;
           digitalWrite(_lights[i].pin, LOW);
         }
         if(strcmp(command, "BLINK") == 0) {
           _lights[i].blinking = true;
         }   
       }
       
       free(name);
       free(command);
       free(commandParam);
       
       free(parts);
       free(cmd);
     }
     free(_inputData);
     _inputData = 0;
     _inputDataLength = 0;
     
   }
 }
 
 for(int i = 0; i < _count; i++) {
   if(_lights[i].blinking == true && (millis() - _lights[i].lastBlink) >= _lights[i].blinkDuration) {
     short valeur;
     if(digitalRead(_lights[i].pin) == LOW) {
       valeur = HIGH;
     } else {
       valeur = LOW;
     }
     
     digitalWrite(_lights[i].pin, valeur);
     _lights[i].lastBlink = millis();
   }
 }
}
