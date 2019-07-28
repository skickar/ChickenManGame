#pragma once

#include "types.h"

#include "hardware.h"

namespace led {
    void begin();
    void setColor(const int r, const int g, const int b);
    void setColor(const TEAM flag);
    unsigned int blink(const unsigned long interval, const TEAM flag, unsigned int times = 0);
};