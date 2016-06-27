#include "project.h"

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
