#ifndef LED_H
#define LED_H

// ========== Includes ========== //
#include "config.h" // LED_PIN_R, LED_PIN_G, LED_PIN_B

// ========== Global Variables ========== //
unsigned long lastBlink = 0;

int ledR = 0;
int ledG = 0;
int ledB = 0;

bool blinked = false;

// ========== Global Functions ========== //
void setColor(int r, int g, int b) {
    ledR = r ? 1 : 0;
    ledG = g ? 1 : 0;
    ledB = b ? 1 : 0;

    digitalWrite(LED_PIN_R, ledR);
    digitalWrite(LED_PIN_G, ledG);
    digitalWrite(LED_PIN_B, ledB);
}

void setupLED() {
    pinMode(LED_PIN_R, OUTPUT);
    pinMode(LED_PIN_G, OUTPUT);
    pinMode(LED_PIN_B, OUTPUT);

    setColor(0, 0, 0);
}

void setFlagColor(int flag) {
    if ((flag < 0) || (flag > 3)) {
        // Wrong number!
        return;
    }

    setColor(flag == 0, flag == 1, flag == 2);
}

void blink(unsigned long interval) {
    if ((interval > 0) && (millis() - lastBlink >= 1000 / interval)) {
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

#endif /* ifndef LED_H */