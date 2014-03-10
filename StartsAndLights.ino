#define PIN_COUNT 14

class Faders
{
  private:
    int _count;
    int _pin[PIN_COUNT];
    int _lastState[PIN_COUNT];
    long _lastDebounce[PIN_COUNT];
    bool _sent[PIN_COUNT];
    long _debounceDelay[PIN_COUNT];
    String _commandLow[PIN_COUNT];
    String _commandHigh[PIN_COUNT];
  public:
    Faders();
    void add(int pin, String startCommand, String stopCommand);
    void wakeUp();
};

Faders::Faders() {
  _count = 0;
}

void Faders::add(int pin, String startCommand, String stopCommand) {
  _debounceDelay[_count] = 10;
  _sent[_count] = false;
  _lastDebounce[_count] = 0;
  _lastState[_count] = HIGH;
  _pin[_count] = pin;
  _commandLow[_count] = startCommand;
  _commandHigh[_count] = stopCommand;
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
  _count++;
}

void Faders::wakeUp()
{
  for(int i = 0; i < _count; i++) {
  int reading = digitalRead(_pin[i]);
  if(reading != _lastState[i]) {
    _lastDebounce[i] = millis();
  }
  
  if((millis() - _lastDebounce[i]) > _debounceDelay[i]) {
    if(reading == LOW && _sent[i] == false) {
       if(_commandLow[i] != "")
         Serial.println(_commandLow[i]);
         
       _sent[i] = true;
    }
    if(reading == HIGH && _sent[i] == true) {
      if(_commandHigh[i] != "")
        Serial.println(_commandHigh[i]);
      
      _sent[i] = false;
    }
  }
  _lastState[i] = reading;
  }
}

class Lights 
{
  private:
    int _pins[PIN_COUNT];
    String _commandsHigh[PIN_COUNT];
    String _commandsLow[PIN_COUNT];
    unsigned int _count;
    unsigned short int _strComplete;
    char* _inputData;
    int _inputDataLength;
  public:
    Lights();
    void add(int pin, String commandHigh, String commandLow);
    void wakeUp();
};

Lights::Lights() {
  _count = 0;
  _strComplete = 0;
  _inputData = 0;  
}

void Lights::add(int pin, String commandHigh, String commandLow) {
  _pins[_count] = pin;
  _commandsHigh[_count] = commandHigh;
  _commandsLow[_count] = commandLow;
  
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
   _inputDataLength++;
   _inputData = (char*)realloc(_inputData, _inputDataLength * sizeof(char));
   if(inputChar == '\n') {
     _strComplete = 1;
     _inputData[_inputDataLength - 1] = '\0';
   } else {
     _inputData[_inputDataLength - 1] = inputChar;
   }
 }
 
 if(_strComplete != 1) {
   return;
 }

 for(int i = 0; i <= _count; i++) {
   if(_commandsHigh[i] == _inputData) {
     digitalWrite(_pins[i], HIGH); 
   }
   if(_commandsLow[i] == _inputData) {
     digitalWrite(_pins[i], LOW);
   }
 }
 
 _strComplete = 0;
 _inputData = 0;
 _inputDataLength = 0;
}

Lights lights;
Faders faders;

void setup() {
  Serial.begin(9600);
  /*
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
  lights.add(3, "LIGHT 1 ON", "LIGHT 1 OFF");
  lights.add(5, "LIGHT 2 ON", "LIGHT 2 OFF");
  lights.add(6, "LIGHT 3 ON", "LIGHT 3 OFF");
  */
}

void loop() {
  faders.wakeUp();
}

void serialEvent() {
  lights.wakeUp(); 
}
