#include "project.h"

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
