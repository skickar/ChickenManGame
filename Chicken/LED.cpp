#include "LED.h"

void LED::begin() {
    pinMode(LED_PIN_R, OUTPUT);
    pinMode(LED_PIN_G, OUTPUT);
    pinMode(LED_PIN_B, OUTPUT);

    setColor(1, 1, 1);
}

void LED::setColor(int r, int g, int b) {
    ledR = r ? 1 : 0;
    ledG = g ? 1 : 0;
    ledB = b ? 1 : 0;

    digitalWrite(LED_PIN_R, ledR);
    digitalWrite(LED_PIN_G, ledG);
    digitalWrite(LED_PIN_B, ledB);
}

void LED::setColor(TEAM flag) {
    if (millis() - lastBlink >= 1000) {
        lastBlink = millis();
        if (flag == NONE) flag = (TEAM)random((int)RED, (int)NONE);
        setColor(flag == RED, flag == GREEN, flag == BLUE);
    }
}

void LED::blink(unsigned long interval, TEAM flag) {
    if (millis() - lastBlink >= 1000 / interval) {
        /*if (flag == NONE) {
            setColor(1, 1, 1);
           } else {*/
        setColor(flag);
        // }

        if (blinked) {
            digitalWrite(LED_PIN_R, 0);
            digitalWrite(LED_PIN_G, 0);
            digitalWrite(LED_PIN_B, 0);
        }  else {
            digitalWrite(LED_PIN_R, ledR);
            digitalWrite(LED_PIN_G, ledG);
            digitalWrite(LED_PIN_B, ledB);
        }

        lastBlink = millis();
        blinked   = !blinked;
    }
}