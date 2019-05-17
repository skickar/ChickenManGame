#ifndef HARDWARE_H
#define HARDWARE_H

// ========== Hardware ========== //

// [LED]
// Enabled
const bool LED_ENABLE = true;
// D1
const int LED_PIN_R = 5;
// D2
const int LED_PIN_G = 4;
// D3
const int LED_PIN_B = 0;

// [NEOPIXEL]
// Enabled
const bool NEOPIXEL = false;
// D1
const int NEOPIXEL_PIN = 5;
// Number of LEDs
const int NEOPIXEL_NUM = 1;
// Brightness ( 0 - 255 )
const int NEOPIXEL_BRIGHTNESS = 90;

// [Switch]
// Enabled
const bool SWITCH_ENABLE = true;
// D7
const int SWITCH_PIN = 13;

// [UART]
const unsigned int SERIAL_BAUD_RATE = 115200;
const char SERIAL_DELIMITER         = '\n';

// [EEPROM]
const int EEPROM_SIZE       = 4095;
const int EEPROM_STATS_ADDR = 1;
const int EEPROM_BOOT_ADDR  = 3210;

#endif /* ifndef HARDWARE_H */