#define PIN_COUNT 14

#include <Keyboard.h>
#include "Light.h"
#include "Fader.h"
#include "Hotkey.h"

Lights lights;
Hotkeys hotkeys;

void setup() {
  Keyboard.begin();
  Serial.begin(9600);

  // Hotkeys
  hotkeys.add(A0, '1');
  hotkeys.add(A1, '2');
  hotkeys.add(A2, '3');
  hotkeys.add(A3, '4');
  hotkeys.add(A4, '5');
  hotkeys.add(A5, '6');
  hotkeys.add(2, '7');
  hotkeys.add(3, '8');
  
  // Lumières
  lights.add(4, "BT1");
  lights.add(5, "BT2");
  lights.add(6, "BT3");
  lights.add(7, "BT4");
  lights.add(8, "BT5");
  lights.add(9, "BT6");
  lights.add(10, "BT7");
  lights.add(11, "BT8");
}

void loop() {
  lights.wakeUp();
  hotkeys.wakeUp();
}
