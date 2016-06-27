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

  Serial.println("MSG: Initializing...");

  // Hotkeys
  hotkeys.add(A0, 'e');
  hotkeys.add(A1, 'r');
  hotkeys.add(A2, 't');
  hotkeys.add(A3, 'y');
  hotkeys.add(A4, 'u');
  hotkeys.add(A5, 'i');
  hotkeys.add(2, 'o');
  hotkeys.add(3, 'p');
  
  // Lumières
  lights.add(4, "B1");
  lights.add(5, "B2");
  lights.add(6, "B3");
  lights.add(7, "B4");
  lights.add(8, "B5");
  lights.add(9, "B6");
  lights.add(10, "B7");
  lights.add(11, "B8");

  Serial.println("MSG: Initialized!");
}

void loop() {
  lights.wakeUp();
  hotkeys.wakeUp();
}
