#include "utils.h"
#include "interface.h"
#include <GyverOLED.h>

namespace global
{
    volatile STATES currentState;
    GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
    String BUFFER_DATA;
}

void initializeDisplay()
{
    using namespace global;
    oled.init();
    oled.clear();
    oled.setScale(2);
    oled.home();
    oled.println("Waiting\r\nfor\r\nconnection");
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
            oled.clear();
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
    oled.home();
    oled.println(Serial.readString());
}

void switchScreen()
{
    using namespace global;
    currentState = CHANGE_SCREEN;
}

void requestNewScreenData()
{
    using namespace global;
    oled.clear();
    oled.home();
    oled.println("Please\r\nStand by...\n");
    oled.println("Fetching...");
    //
    Serial.flush();
    delay(500);
    Serial.write(SWITCH_SCREEN_MESSAGE);
    while (true)
    {
        if (Serial.available() == 0)
            continue;
        if (Serial.readString() == ACK_MESSAGE)
        {
            currentState = SHOW_SCREEN;
            break;
        }
    }
    oled.clear();
}