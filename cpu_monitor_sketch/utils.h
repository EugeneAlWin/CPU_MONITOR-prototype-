#pragma once
#include <GyverOLED.h>
#include "interface.h"

void initializeDisplay(GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> *);
void waitForConnection(volatile STATES *);
void requestAnimationFrame(GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> *);
void requestScreenChange();