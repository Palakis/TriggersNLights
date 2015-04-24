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

struct Light {
  short pin;
  char* name;
  bool blinking;
  unsigned long lastBlink;
  unsigned long blinkDuration;
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
    void add(short pin, char* name);
    void wakeUp();
};

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
           Serial.print("Match numero ");
           Serial.println(partsCount);
           
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
       
       Serial.print("Name :");
       Serial.println(name);
       Serial.print("Command : ");
       Serial.println(command);
       Serial.print("Command param : ");
       Serial.println(commandParam);
       
       if(strcmp(name, _lights[i].name) == 0) {
         Serial.println("C'est pour moi !");
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
  lights.add(8, "LIGHT1");
  lights.add(9, "LIGHT2");
  lights.add(10, "LIGHT3");
  lights.add(11, "LIGHT4");
}

void loop() {
  faders.wakeUp();
  lights.wakeUp();
}
