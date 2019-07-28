#include "LED.h"

#if defined(LED_ANALOG)
#include <Arduino.h> // digitalWrite, digitalRead
#elif defined(LED_NEOPIXEL)
#include <Adafruit_NeoPixel.h>
#endif // if defined(LED_ANALOG)

namespace led {
    bool enabled { false };
    bool blinked { false };

    unsigned long lastBlink { 0 };

    int ledR { 0 };
    int ledG { 0 };
    int ledB { 0 };

#if defined(LED_ANALOG)
    int pinR;
    int pinG;
    int pinB;

    void begin(const int pinR, const int pinG, const int pinB) {
        pinMode(pinR, OUTPUT);
        pinMode(pinG, OUTPUT);
        pinMode(pinB, OUTPUT);

        led::pinR = pinR;
        led::pinG = pinG;
        led::pinB = pinB;

        enabled = true;

        setColor(1, 1, 1);
    }

    void setColor(const int r, const int g, const int b) {
        if (!enabled) return;

        ledR = r ? 1 : 0;
        ledG = g ? 1 : 0;
        ledB = b ? 1 : 0;

        digitalWrite(pinR, ledR);
        digitalWrite(pinG, ledG);
        digitalWrite(pinB, ledB);
    }

#elif defined(LED_NEOPIXEL)

    Adafruit_NeoPixel* pixels { nullptr };

    void begin(const int num, const int pin, const int brightness) {
        pixels = new Adafruit_NeoPixel(num, pin, NEO_GRB + NEO_KHZ800);
        pixels->setBrightness(brightness);
        pixels->begin();

        enabled = true;

        setColor(1, 1, 1);
    }

    void setColor(const int r, const int g, const int b) {
        if (!enabled) return;

        ledR = r > 0 ? 255 : 0;
        ledG = g > 0 ? 255 : 0;
        ledB = b > 0 ? 255 : 0;

        for (int i = 0; i<pixels->numPixels(); ++i) {
            pixels->setPixelColor(i, ledR, ledG, ledB);
        }
        pixels->show();
    }

#endif // if defined(LED_ANALOG)

    void setColor(const TEAM flag) {
        setColor(flag == RED, flag == GREEN, flag == BLUE);
    }

    unsigned int blink(const unsigned long interval, const TEAM flag, unsigned int times) {
        if ((times) && (millis() - lastBlink >= 1000 / interval)) {
            if (blinked) {
                setColor(0, 0, 0);
            }  else {
                if (flag == NO_TEAM) setColor(1, 1, 1);
                else setColor(flag);
            }

            lastBlink = millis();
            blinked   = !blinked;

            return --times;
        }
        return times;
    }
}