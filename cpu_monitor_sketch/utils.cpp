#include "utils.h"
#include "interface.h"
#include <GyverOLED.h>

volatile SCREENS currentScreen;
volatile STATES currentState;
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
String BUFFER_DATA;

void initializeDisplay()
{
    oled.init();
    oled.clear();
    oled.setScale(3);
    oled.home();
}

void waitForConnection()
{
    while (true)
    {
        if (Serial.available() == 0)
            return;

        BUFFER_DATA = Serial.readString();
        if (BUFFER_DATA == HELLO_MESSAGE)
        {
            currentState = SHOW_SCREEN;
            return;
        }
    }
}

void requestAnimationFrame()
{
    if (Serial.available() == 0)
        return;

    oled.clear();
    oled.home();
    BUFFER_DATA = Serial.readString();
    oled.print(BUFFER_DATA);
}

void requestScreenChange()
{
    currentScreen = static_cast<SCREENS>((static_cast<int>(currentScreen) + 1) % static_cast<int>(_SCREENS_LEN));
    currentState = CHANGE_SCREEN;
}