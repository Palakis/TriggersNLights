#include "project.h"
#include <Keyboard.h>

struct Hotkey {
  unsigned short pin;
  char key;
  short lastState;
  unsigned long lastDebounce;
  bool sent;
  unsigned long debounceDelay;
};

class Hotkeys
{
  private:
    int _count;
    Hotkey _hotkeys[PIN_COUNT];
  public:
    Hotkeys();
    void add(short pin, char key);
    void wakeUp();
};



