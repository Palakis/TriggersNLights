class Fader
{
  private:
    int _pin;
    int _lastState;
    long _lastDebounce;
    bool _sent;
    long _debounceDelay;
    String _commandLow;
    String _commandHigh;
  public:
    Fader(int pin, String startCommand, String stopCommand);
    void read();
};

Fader::Fader(int pin, String startCommand, String stopCommand) {
  _debounceDelay = 10;
  _sent = false;
  _lastDebounce = 0;
  _lastState = HIGH;
  _pin = pin;
  _commandLow = startCommand;
  _commandHigh = stopCommand;
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void Fader::read()
{
  int reading = digitalRead(_pin);
  if(reading != _lastState) {
    _lastDebounce = millis();
  }
  
  if((millis() - _lastDebounce) > _debounceDelay) {
    if(reading == LOW && _sent == false) {
       Serial.println(_commandLow);
       _sent = true;
    }
    if(reading == HIGH && _sent == true) {
      Serial.println(_commandHigh);
      _sent = false;
    }
  }
  _lastState = reading;
}

//Fader fader1(2, "PLAYER 1-1 START", "PLAYER 1-1 STOP");
Fader fader1(2, "PLAYER 1-1 START", "PLAYER 1-1 STOP");
Fader fader2(3, "PLAYER 1-2 START", "PLAYER 1-2 STOP");
Fader fader3(4, "CART 0 START", "CART 0 STOP");
Fader fader4(5, "CART 1 START", "CART 1 STOP");

void setup() {
  Serial.begin(19200);
}

void loop() {
  fader1.read();
  fader2.read();
  fader3.read();
  fader4.read();
}
