#pragma once

#include "types.h"

#include "hardware.h"

namespace led {
#if defined(LED_ANALOG)
    void begin(const int pinR, const int pinG, const int pinB);
#elif defined(LED_NEOPIXEL)
    void begin(const int num, const int pin, const int brightness);
#endif // if defined(LED_ANALOG)

    void setColor(const int r, const int g, const int b);
    void setColor(const TEAM flag);
    unsigned int blink(const unsigned long interval, const TEAM flag, unsigned int times = 0);
};