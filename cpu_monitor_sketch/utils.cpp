#include "utils.h"
#include "interface.h"
#include <GyverOLED.h>

String BUFFER_DATA;
void initializeDisplay(GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> *oled)
{
    oled->init();
    oled->clear();
    oled->setScale(3);
    oled->home();
}

void waitForConnection(volatile STATES *currentState)
{
    while (true)
    {
        if (Serial.available() == 0)
            return;

        BUFFER_DATA = Serial.readString();
        if (BUFFER_DATA == HELLO_MESSAGE)
        {
            *currentState = SHOW_SCREEN;
            return;
        }
    }
}

void requestAnimationFrame(GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> *oled)
{
    if (Serial.available() == 0)
        return;
    oled->clear();
    oled->home();
    BUFFER_DATA = Serial.readString();
    oled->print(BUFFER_DATA);
}

void requestScreenChange()
{
    Serial.write(HELLO_MESSAGE);
}