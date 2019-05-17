#ifndef LED_H
#define LED_H

// ========== Includes ========== //
#include <Arduino.h> // digitalWrite, digitalRead

#include <Adafruit_NeoPixel.h>

#include "types.h"

// From hardware.h
extern const int  LED_PIN_R;
extern const int  LED_PIN_G;
extern const int  LED_PIN_B;
extern const bool NEOPIXEL;
extern const int  NEOPIXEL_PIN;
extern const int  NEOPIXEL_NUM;
extern const int  NEOPIXEL_BRIGHTNESS;

// ========== LED Class ========== //

class LED {
    private:
        bool enabled = false;

        Adafruit_NeoPixel* pixels = NULL;

        unsigned long lastBlink = 0;

        int ledR = 0;
        int ledG = 0;
        int ledB = 0;

        bool blinked = false;

    public:
        ~LED();

        void begin();
        void setColor(int r, int g, int b);
        void setColor(TEAM flag);
        unsigned int blink(unsigned long interval, TEAM flag, unsigned int times = 0);
};

#endif /* ifndef LED_H */