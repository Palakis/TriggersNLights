#define PIN_COUNT 14

struct Fader {
  unsigned short pin;
  char* commandLow;
  char* commandHigh;
  short lastState;
  unsigned long lastDebounce;
  bool sent;
  unsigned long debounceDelay;
};

struct Light {
  short pin;
  char* commandHigh;
  char* commandLow;
  char* commandBlink;
  bool blinking;
  unsigned long lastBlink;
  unsigned long blinkDuration;
};

class Faders
{
  private:
    int _count;
    Fader _faders[PIN_COUNT];
  public:
    Faders();
    void add(short pin, char* startCommand, char* stopCommand);
    void wakeUp();
};

class Lights 
{
  private:
    unsigned int _count;
    Light _lights[PIN_COUNT];
    char* _inputData;
    char _inputChar;
    bool _strComplete;
    unsigned int _inputDataLength;
  public:
    Lights();
    void add(short pin, char* commandHigh, char* commandLow, char* commandBlink);
    void wakeUp();
};

Faders::Faders() {
  _count = 0;
}

Lights::Lights() {
  _count = 0;
  _inputDataLength = 0;
  _strComplete = false;  
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

void Lights::add(short pin, char* commandHigh, char* commandLow, char* commandBlink) {
  _lights[_count].pin = pin;
  _lights[_count].commandHigh = commandHigh;
  _lights[_count].commandLow = commandLow;
  _lights[_count].commandBlink = commandBlink;
  _lights[_count].lastBlink = millis();
  _lights[_count].blinkDuration = 500;
  
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
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

void Lights::wakeUp() {
 if(_count < 1) {
   return;
 }
 
 while(Serial.available()) {
   char inputChar = (char)Serial.read();
   _inputData = (char*)realloc(_inputData, (_inputDataLength+1) * sizeof(char));
   if(inputChar == '\n') {
     _inputData[_inputDataLength] = '\0';
     for(int i = 0; i < _count; i++) {
       if(strcmp(_lights[i].commandHigh, _inputData) == 0) {
         _lights[i].blinking = false;
         digitalWrite(_lights[i].pin, HIGH);
       }
       if(strcmp(_lights[i].commandLow, _inputData) == 0) {
         _lights[i].blinking = false;
         digitalWrite(_lights[i].pin, LOW);
       }
       if(strcmp(_lights[i].commandBlink, _inputData) == 0) {
          _lights[i].blinking = true;
       }
     }
     free(_inputData);
     _inputData = 0;
     _inputDataLength = 0;
     
   } else {
     _inputData[_inputDataLength] = inputChar;
     _inputDataLength++;
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

Lights lights;
Faders faders;

void setup() {
  Serial.begin(9600);
  // Lecteurs A et B
  faders.add(3, "PLAYER 1-1 START", "PLAYER 1-1 STOP");
  faders.add(2, "PLAYER 1-2 START", "PLAYER 1-2 STOP");
  faders.add(12, "CARTWALL SHOW", "CARTWALL HIDE");
  
  // Lecteurs cartouchier
  faders.add(4, "CARTWALL 1 START/STOP", "");
  faders.add(5, "CARTWALL 2 START/STOP", "");
  faders.add(6, "CARTWALL 3 START/STOP", "");
  faders.add(7, "CARTWALL 4 START/STOP", "");
  
  // Lumières
  lights.add(8, "LIGHT 1 ON", "LIGHT 1 OFF", "LIGHT 1 BLINK");
  lights.add(9, "LIGHT 2 ON", "LIGHT 2 OFF", "LIGHT 2 BLINK");
  lights.add(10, "LIGHT 3 ON", "LIGHT 3 OFF", "LIGHT 3 BLINK");
  lights.add(11, "LIGHT 4 ON", "LIGHT 4 OFF", "LIGHT 4 BLINK");
}

void loop() {
  faders.wakeUp();
  lights.wakeUp();
}