#include "LED.h"

LED::~LED() {
    if (pixels) delete pixels;
}

void LED::begin() {
    if (NEOPIXEL) {
        pixels = new Adafruit_NeoPixel(NEOPIXEL_NUM, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
        pixels->setBrightness(NEOPIXEL_BRIGHTNESS);
        pixels->begin();
    } else {
        pinMode(LED_PIN_R, OUTPUT);
        pinMode(LED_PIN_G, OUTPUT);
        pinMode(LED_PIN_B, OUTPUT);
    }

    enabled = true;

    setColor(1, 1, 1);
}

void LED::setColor(int r, int g, int b) {
    if (!enabled) return;

    if (NEOPIXEL) {
        ledR = r ? 255 : 0;
        ledG = g ? 255 : 0;
        ledB = b ? 255 : 0;

        if (pixels) {
            for (int i = 0; i<NEOPIXEL_NUM; i++) {
                pixels->setPixelColor(i, ledR, ledG, ledB);
            }
            pixels->show();
        }
    } else {
        ledR = r ? 1 : 0;
        ledG = g ? 1 : 0;
        ledB = b ? 1 : 0;

        digitalWrite(LED_PIN_R, ledR);
        digitalWrite(LED_PIN_G, ledG);
        digitalWrite(LED_PIN_B, ledB);
    }

    // Serial.printf("color(%d,%d,%d)\n", ledR, ledG, ledB);
}

void LED::setColor(TEAM flag) {
    setColor(flag == RED, flag == GREEN, flag == BLUE);
}

unsigned int LED::blink(unsigned long interval, TEAM flag, unsigned int times) {
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