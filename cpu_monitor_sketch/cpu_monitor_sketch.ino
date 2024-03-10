#include <GyverOLED.h>
#include "utils.h"
#include "interface.h"
constexpr byte BUTTON_PIN_DIGITAL = 2;
constexpr byte INTERRUPT_PIN_D2 = 0;

using namespace global;

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN_DIGITAL, INPUT_PULLUP);
  attachInterrupt(INTERRUPT_PIN_D2, switchScreen, RISING);
  currentState = WAIT_FOR_CONNECTION;

  initializeDisplay();
}

void loop()
{
  if (currentState == WAIT_FOR_CONNECTION)
    return waitForConnection();

  else if (currentState == SHOW_SCREEN)
    return requestAnimationFrame();

  else if (currentState == CHANGE_SCREEN)
    return requestNewScreenData();
}