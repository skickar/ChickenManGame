#ifndef LED_H
#define LED_H

// ========== Includes ========== //
#include "Arduino.h" // digitalWrite, digitalRead

// From hardware.h
extern const int LED_PIN_R;
extern const int LED_PIN_G;
extern const int LED_PIN_B;

// ========== LED Class ========== //

class LED {
    private:
        unsigned long lastBlink = 0;

        int ledR = 0;
        int ledG = 0;
        int ledB = 0;

        bool blinked = false;

    public:
        void begin();
        void setColor(int r, int g, int b);
        void setFlagColor(int flag);
        void blink(unsigned long interval);
        void blinkError();
};

#endif /* ifndef LED_H */