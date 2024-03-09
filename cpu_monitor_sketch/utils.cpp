#include "utils.h"
#include "interface.h"
#include <GyverOLED.h>

namespace global
{
    volatile SCREENS currentScreen;
    volatile STATES currentState;
    GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
    String BUFFER_DATA;
}

void initializeDisplay()
{
    using namespace global;
    oled.init();
    oled.clear();
    oled.setScale(3);
    oled.home();
}

void waitForConnection()
{
    using namespace global;
    while (true)
    {
        if (Serial.available() == 0)
            return;

        if (Serial.readString() == HELLO_MESSAGE)
        {
            Serial.write(ACK_MESSAGE);
            currentState = SHOW_SCREEN;
            return;
        }
    }
}

void requestAnimationFrame()
{
    using namespace global;
    if (Serial.available() == 0)
        return;

    oled.clear();
    oled.home();
    oled.print(Serial.readString());
}

void switchScreen()
{
    using namespace global;
    currentScreen = static_cast<SCREENS>((static_cast<int>(currentScreen) + 1) % static_cast<int>(_SCREENS_LEN));
    currentState = CHANGE_SCREEN;
}

void requestNewScreenData()
{
    using namespace global;
    Serial.write(currentScreen);
    while (true)
    {
        if (Serial.available() == 0)
            continue;
        if (Serial.readString() == ACK_MESSAGE)
        {
            Serial.write(ACK_MESSAGE);
            currentState = SHOW_SCREEN;
            return;
        }
    }
}