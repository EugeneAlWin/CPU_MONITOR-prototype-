#pragma once
#include <GyverOLED.h>
#include "interface.h"

namespace global
{
    extern volatile SCREENS currentScreen;
    extern volatile STATES currentState;
    extern GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
    extern String BUFFER_DATA;
}

void initializeDisplay();
void waitForConnection();
void requestAnimationFrame();
void requestScreenChange();